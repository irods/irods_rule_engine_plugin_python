// include this first to fix macro redef warnings
#include <pyconfig.h>

#include <cassert>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <fstream>
#include <list>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <mutex>

#include <boost/version.hpp>
#pragma GCC diagnostic push
#if BOOST_VERSION < 108100
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <boost/date_time.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/filesystem/operations.hpp>
#pragma GCC diagnostic pop

#include <irods/rodsErrorTable.h>
#include <irods/irods_default_paths.hpp>
#include <irods/irods_error.hpp>
#include <irods/irods_exception.hpp>
#include <irods/irods_logger.hpp>
#include <irods/irods_re_plugin.hpp>
#include <irods/irods_re_structs.hpp>
#include <irods/irods_re_ruleexistshelper.hpp>
#include <irods/irods_re_serialization.hpp>
#include <irods/irods_ms_plugin.hpp>
#include <irods/irods_server_properties.hpp>
#include <irods/msParam.h>
#include <irods/rsExecMyRule.hpp>

#include "irods/private/re/python.hpp"

#include <patchlevel.h>
#pragma GCC diagnostic push
#if PY_VERSION_HEX < 0x030400A2
#  pragma GCC diagnostic ignored "-Wregister"
#endif
#if PY_VERSION_HEX >= 0x03090000 && BOOST_VERSION < 107500
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <boost/python/slice.hpp>
#include <boost/python/module_init.hpp>
#pragma GCC diagnostic pop

#include <fmt/format.h>

irods::ms_table& get_microservice_table();

// writeLine is not in the microservice table in 4.2.0 - #3408
int writeLine(msParam_t*, msParam_t*, ruleExecInfo_t*);

static int remote_exec_msvc(msParam_t* _pd, msParam_t* _pa, msParam_t* _pb, msParam_t* /*_pc*/, ruleExecInfo_t* _rei)
{
	execMyRuleInp_t exec_inp;
	memset(&exec_inp, 0, sizeof(exec_inp));
	rstrcpy(exec_inp.outParamDesc, ALL_MS_PARAM_KW, LONG_NAME_LEN);

	char tmp_str[LONG_NAME_LEN];
	rstrcpy(tmp_str, static_cast<char*>(_pd->inOutStruct), LONG_NAME_LEN);
	parseHostAddrStr(tmp_str, &exec_inp.addr);

	std::string rule_text = "@external\n";
	rule_text += static_cast<char*>(_pb->inOutStruct);
	snprintf(exec_inp.myRule, META_STR_LEN, "%s", rule_text.c_str());

	// TODO:
	// It is not yet known whether execCondition is actually used by rsExecMyRule or
	// any functions it calls. If in resolution of [irods/irods#6567] we find it is
	// unused, the following addKeyVal call may be unnecessary.
	addKeyVal(&exec_inp.condInput, "execCondition", static_cast<char*>(_pa->inOutStruct));

	auto taggedValues = getTaggedValues(static_cast<char*>(_pa->inOutStruct));
	auto it = taggedValues.find("INST_NAME");
	if (it != taggedValues.end()) {
		addKeyVal(&exec_inp.condInput, INSTANCE_NAME_KW, it->second.front().c_str());
		taggedValues.erase(it);
	}

	msParamArray_t* out_arr = NULL;
	return rsExecMyRule(_rei->rsComm, &exec_inp, &out_arr);
} // remote_exec_msvc

const std::string ELEMENT_TYPE = "ELEMENT_TYPE";
const std::string STRING_TYPE = "STRING_TYPE";
const std::string STRING_VALUE_KEY = "STRING_VALUE_KEY";
const std::string IRODS_ERROR_PREFIX = "[iRods__Error__Code:";

const std::string STATIC_PEP_RULE_REGEX = "ac[^ ]*";
const std::string DYNAMIC_PEP_RULE_REGEX = "[^ ]*pep_[^ ]*_(pre|post)";

namespace bp = boost::python;

namespace
{
	static inline constexpr const char* const rule_engine_name = "python";
	using log_re = irods::experimental::log::rule_engine;

	// Data we hang onto for the interpreter
	namespace python_state
	{
		// Thread state object for main Python interpreter
		static PyThreadState* ts_main;

#if PY_VERSION_HEX >= 0x03080000
		// List of default module search paths
		static std::vector<std::wstring> default_module_search_paths;
		// Whether or not default_module_search_paths is populated
		static bool default_module_search_paths_set = false;
#endif
	} //namespace python_state
}

void register_regexes_from_array(const nlohmann::json& _array, const std::string& _instance_name)
{
	try {
		for (const auto& elem : _array) {
			try {
				const auto& tmp = elem.get_ref<const std::string&>();
				RuleExistsHelper::Instance()->registerRuleRegex(tmp);
				// clang-format off
				log_re::debug({
					{"rule_engine_plugin", rule_engine_name},
					{"instance_name", _instance_name},
					{"regex", tmp},
				});
				// clang-format on
			}
			catch (const boost::bad_any_cast&) {
				// clang-format off
				log_re::error({
					{"rule_engine_plugin", rule_engine_name},
					{"instance_name", _instance_name},
					{"log_message", "failed to cast pep_regex_to_match to string"},
				});
				// clang-format on
				continue;
			}
		}
	}
	catch (const boost::bad_any_cast&) {
		std::stringstream msg;
		msg << "[" << _instance_name << "] failed to any_cast a std::vector<boost::any>&";
		THROW(INVALID_ANY_CAST, msg.str());
	}
}

namespace
{
	irods::error to_irods_error_object(const bp::object& object)
	{
		if (bp::extract<int> result{object}; result.check()) {
			const int ec = result();

			if (ec < 0) {
				return ERROR(ec, "");
			}

			return CODE(ec);
		}

		return SUCCESS();
	}

	std::string extract_python_exception(void)
	{
		PyObject *exc, *val, *tb;
		PyErr_Fetch(&exc, &val, &tb);
		PyErr_NormalizeException(&exc, &val, &tb);
		bp::handle<> hexc(exc), hval(bp::allow_null(val)), htb(bp::allow_null(tb));

		if (!hval) {
			return bp::extract<std::string>(bp::str(hexc));
		}
		else {
			bp::object traceback(bp::import("traceback"));
			bp::object format_exception(traceback.attr("format_exception"));
			bp::object formatted_list(format_exception(hexc, hval, htb));
			bp::object formatted(bp::str("").join(formatted_list));

			return bp::extract<std::string>(formatted);
		}
	}

	namespace StringFromPythonUnicode
	{
		void* convertible(PyObject* py_obj)
		{
			if (PyUnicode_Check(py_obj)) {
				return py_obj;
			}

			return nullptr;
		}

		void construct(PyObject* py_obj, boost::python::converter::rvalue_from_python_stage1_data* data)
		{
			bp::handle<> h_encoded(PyUnicode_AsUTF8String(py_obj));
			void* storage = reinterpret_cast<boost::python::converter::rvalue_from_python_storage<std::string>*>(data)
			                    ->storage.bytes;
			new (storage) std::string(PyBytes_AsString(h_encoded.get()));
			data->convertible = storage;
		}

		void register_converter()
		{
			bp::converter::registry::push_back(&convertible, &construct, bp::type_id<std::string>());
		}
	} // namespace StringFromPythonUnicode

	ruleExecInfo_t* get_rei_from_effect_handler(irods::callback effect_handler)
	{
		ruleExecInfo_t* rei = nullptr;
		irods::error err = effect_handler("unsafe_ms_ctx", &rei);

		if (!err.ok()) {
			// clang-format off
			log_re::error({
				{"rule_engine_plugin", rule_engine_name},
				{"log_message", "Could not retrieve RuleExecInfo_t object from effect handler"},
			});
			// clang-format on
			return nullptr;
		}

		if (!rei) {
			// clang-format off
			log_re::error({
				{"rule_engine_plugin", rule_engine_name},
				{"log_message", "RuleExecInfo object is NULL - cannot populate session vars"},
			});
			// clang-format on
			return nullptr;
		}

		return rei;
	}

	// Helper class that acquires the GIL while in scope
	class python_gil_lock
	{
	public:
		python_gil_lock()
			: _previous_gil_state(PyGILState_Ensure())
		{ }

		~python_gil_lock()
		{
			if (!_released) {
				PyGILState_Release(_previous_gil_state);
			}
		}

		python_gil_lock(const python_gil_lock&) = delete;

		python_gil_lock& operator=(const python_gil_lock&) = delete;

		inline bool released()
		{
			return _released;
		}

		// restores python state to prior to last PyGILState_Ensure call
		// returns active PyThreadState
		// doesn't necessarily release the GIL, only this instance's hold on it.
		// if GIL was acquired further up the stack, it will still be locked. 
		inline PyThreadState* release()
		{
			if (_released) {
				THROW(RULE_ENGINE_ERROR, "release called on already-released python_gil_lock");
			}

			PyThreadState* current_thread_state = PyThreadState_Get();
			if (current_thread_state->gilstate_counter <= 1) {
				current_thread_state = nullptr;
			}

			PyGILState_Release(_previous_gil_state);

			_released = true;
			return current_thread_state;
		}

		// reacquires GIL after a call to release
		inline  void reacquire()
		{
			if (_released) {
				THROW(RULE_ENGINE_ERROR, "reacquire called on non-released python_gil_lock");
			}

			_previous_gil_state = PyGILState_Ensure();
			_released = false;
		}

	private:
		PyGILState_STATE _previous_gil_state; // GIL state prior to calling PyGILState_Ensure
		bool _released = false; // whether or not PyGILState_Release has already been called

	}; //class python_gil_lock

	// Helper class that unlocks GIL while in scope
	class python_gil_unlock
	{
	public:
		// Saves the current thread state, releasing the GIL
		// Does not otherwise alter thread state in any way
		python_gil_unlock()
			: _previous_thread_state(PyEval_SaveThread())
		{ }

		// Calls release on gil_lock and, if necessary, saves the current thread state
		// if should_reacquire is true, reacquire is called on gil_lock during destruction
		python_gil_unlock(python_gil_lock* const gil_lock, bool should_reacquire)
			: _gil_lock(gil_lock), _should_reacquire(should_reacquire)
		{
			if (_gil_lock->release() != nullptr) {
				_previous_thread_state = PyEval_SaveThread();
			}
		}

		~python_gil_unlock()
		{
			if (_previous_thread_state != nullptr) {
				PyEval_RestoreThread(_previous_thread_state);
			}
			if (_should_reacquire) {
				assert(_gil_lock != nullptr);
				_gil_lock->reacquire();
			}
		}

		python_gil_unlock(const python_gil_unlock&) = delete;

		python_gil_unlock& operator=(const python_gil_unlock&) = delete;

	private:
		PyThreadState* _previous_thread_state = nullptr;
		python_gil_lock* const _gil_lock = nullptr;
		const bool _should_reacquire = false;

	}; //class python_gil_unlock

	struct RuleCallWrapper
	{
		RuleCallWrapper(irods::callback& effect_handler, std::string rule_name)
			: effect_handler{effect_handler}
			, rule_name{rule_name}
		{
		}

		irods::callback& effect_handler;
		std::string rule_name;

		static bp::dict call(const bp::tuple& args, const bp::dict&)
		{
			RuleCallWrapper& self = bp::extract<RuleCallWrapper&>(args[0]);

			bp::tuple rule_args_python = bp::extract<bp::tuple>(args[bp::slice(1, bp::len(args))]);
			std::list<boost::any> rule_args_cpp;
			std::list<msParam_t> msParams;
			std::list<std::string> strings;

			for (auto&& rule_arg_python : rule_args_python) {
				bp::extract<std::string> s{rule_arg_python};
				if (s.check()) {
					strings.push_back(s());
					rule_args_cpp.emplace_back(&strings.back());
				}
				else {
					msParams.push_back(msParam_from_object<genQueryInp_t,
					                                       genQueryOut_t,
					                                       keyValPair_t,
					                                       fileLseekOut_t,
					                                       rodsObjStat_t,
					                                       bytesBuf_t,
					                                       int,
					                                       float>(rule_arg_python));
					rule_args_cpp.emplace_back(&msParams.back());
				}
			}

			const auto err = self.effect_handler(self.rule_name, irods::unpack(rule_args_cpp));

			const auto error_occurred =
				!err.ok() && err.code() != CAT_NO_ROWS_FOUND && err.code() != CAT_SUCCESS_BUT_WITH_NO_INFO;

			bp::list ret_list{};
			while (!rule_args_cpp.empty()) {
				auto& rule_arg_cpp = rule_args_cpp.front();

				if (rule_arg_cpp.type() == typeid(std::string*)) {
					ret_list.append(strings.front());
					strings.pop_front();
				}
				else {
					ret_list.append(object_from_msParam(msParams.front()));

					if (!error_occurred) {
						clearMsParam(&msParams.front(), 1);
					}

					msParams.pop_front();
				}

				rule_args_cpp.pop_front();
			}

			if (error_occurred) {
				std::string returnString =
					IRODS_ERROR_PREFIX + boost::lexical_cast<std::string>(err.code()) + "] " + err.result().c_str();
				PyErr_SetString(PyExc_RuntimeError, returnString.c_str());
				bp::throw_error_already_set();
			}

			bp::dict ret;
			ret["code"] = err.code();
			ret["status"] = err.status();
			ret["arguments"] = ret_list;
			return ret;
		}
	}; // struct RuleCallWrapper

	struct CallbackWrapper
	{
		CallbackWrapper(irods::callback& effect_handler)
			: effect_handler{effect_handler}
		{
		}

		irods::callback& effect_handler;

		RuleCallWrapper getAttribute(const std::string& rule_name)
		{
			return RuleCallWrapper{effect_handler, rule_name};
		}
	}; // struct CallbackWrapper

	BOOST_PYTHON_MODULE(plugin_wrappers)
	{
		bp::class_<RuleCallWrapper>("RuleCallWrapper", bp::no_init)
			.def("__call__", bp::raw_function(&RuleCallWrapper::call, 1));

		bp::class_<CallbackWrapper>("CallbackWrapper", bp::no_init)
			.def("__getattribute__", &CallbackWrapper::getAttribute);
	}

	static inline void initialize_python(const std::string& _instance_name)
	{
		auto etc_irods_path = irods::get_irods_config_directory();

#if PY_VERSION_HEX >= 0x03080000
		if (python_state::default_module_search_paths_set) {
			PyConfig py_config;
			PyConfig_InitPythonConfig(&py_config);

			py_config.install_signal_handlers = 0;

			for (auto&& module_search_path : python_state::default_module_search_paths) {
				PyWideStringList_Append(&py_config.module_search_paths, module_search_path.c_str());
			}
			PyWideStringList_Append(&py_config.module_search_paths, etc_irods_path.generic_wstring().c_str());
			py_config.module_search_paths_set = 1;

			PyStatus py_status = Py_InitializeFromConfig(&py_config);

			if (!PyStatus_Exception(py_status)) {
				return;
			}

			// clang-format off
			log_re::error({
				{"rule_engine_plugin", rule_engine_name},
				{"instance_name", _instance_name},
				{"log_message", "failed to initialize interpreter with PyConfig; falling back to legacy initialization"},
				{"PyStatus.exitcode", fmt::to_string(py_status.exitcode)},
				{"PyStatus.err_msg", py_status.err_msg},
				{"PyStatus.func", py_status.func},
			});
			// clang-format on
		}
#endif

		Py_InitializeEx(0);
#if PY_VERSION_HEX < 0x03070000
		PyEval_InitThreads();
#endif
		bp::object mod_sys = bp::import("sys");
		bp::list sys_path = bp::extract<bp::list>(mod_sys.attr("path"));
		sys_path.append(bp::str(etc_irods_path.generic_string().c_str()));
	}

} // anonymous namespace

static irods::error start(irods::default_re_ctx&, const std::string& _instance_name)
{
	python_state::ts_main = nullptr;
	try {
		PyImport_AppendInittab("plugin_wrappers", &PyInit_plugin_wrappers);
		PyImport_AppendInittab("irods_types", &PyInit_irods_types);
		PyImport_AppendInittab("irods_errors", &PyInit_irods_errors);

		initialize_python(_instance_name);

		bp::object plugin_wrappers = bp::import("plugin_wrappers");
		bp::object irods_types = bp::import("irods_types");
		bp::object irods_errors = bp::import("irods_errors");

		StringFromPythonUnicode::register_converter();
	}
	catch (const bp::error_already_set&) {
		const std::string formatted_python_exception = extract_python_exception();
		// clang-format off
		log_re::error({
			{"rule_engine_plugin", rule_engine_name},
			{"instance_name", _instance_name},
			{"log_message", "caught python exception"},
			{"python_exception", formatted_python_exception},
		});
		// clang-format on
		std::string err_msg = std::string("irods_rule_engine_plugin_python::") + __PRETTY_FUNCTION__ +
		                      " Caught Python exception.\n" + formatted_python_exception;
		return ERROR(RULE_ENGINE_ERROR, err_msg);
	}

	python_state::ts_main = PyEval_SaveThread();
	// NO MORE PYTHON IN THIS FUNCTION PAST THIS POINT

	// Initialize microservice table
	irods::ms_table& ms_table = get_microservice_table();
	// writeLine is not in the microservice table in 4.2.0 - #3408
	if (!ms_table.has_entry("writeLine")) {
		ms_table["writeLine"] = new irods::ms_table_entry(
			"writeLine", 2, std::function<int(msParam_t*, msParam_t*, ruleExecInfo_t*)>(writeLine));
	}

	ms_table["py_remote"] = new irods::ms_table_entry(
		"py_remote",
		4,
		std::function<int(msParam_t*, msParam_t*, msParam_t*, msParam_t*, ruleExecInfo_t*)>(remote_exec_msvc));

	try {
		const auto& re_plugin_arr = irods::get_server_property<const nlohmann::json&>(
			std::vector<std::string>{irods::KW_CFG_PLUGIN_CONFIGURATION, irods::KW_CFG_PLUGIN_TYPE_RULE_ENGINE});
		for (const auto& plugin_config : re_plugin_arr) {
			const auto& inst_name = plugin_config.at(irods::KW_CFG_INSTANCE_NAME).get_ref<const std::string&>();
			if (inst_name == _instance_name) {
				const auto& plugin_spec_cfg = plugin_config.at(irods::KW_CFG_PLUGIN_SPECIFIC_CONFIGURATION);

				// TODO Enable non core.py Python rulebases

				if (plugin_spec_cfg.count(irods::KW_CFG_RE_PEP_REGEX_SET)) {
					register_regexes_from_array(plugin_spec_cfg.at(irods::KW_CFG_RE_PEP_REGEX_SET), _instance_name);
				}
				else {
					RuleExistsHelper::Instance()->registerRuleRegex(STATIC_PEP_RULE_REGEX);
					RuleExistsHelper::Instance()->registerRuleRegex(DYNAMIC_PEP_RULE_REGEX);

					// clang-format off
					log_re::debug({
						{"rule_engine_plugin", rule_engine_name},
						{"instance_name", _instance_name},
						{"log_message", "No regexes found in server_config for Python RE - using default regexes"},
						{"static_pep_rule_regex", STATIC_PEP_RULE_REGEX},
						{"dynamic_pep_rule_regex", DYNAMIC_PEP_RULE_REGEX},
					});
					// clang-format on
				}

				return SUCCESS();
			}
		}
	}
	catch (const irods::exception& e) {
		return irods::error(e);
	}
	catch (const boost::bad_any_cast& e) {
		return ERROR(INVALID_ANY_CAST, e.what());
	}
	catch (const std::out_of_range& e) {
		return ERROR(KEY_NOT_FOUND, e.what());
	}

	// clang-format off
	log_re::error({
		{"rule_engine_plugin", rule_engine_name},
		{"instance_name", _instance_name},
		{"log_message", "failed to find configuration for plugin"},
	});
	// clang-format on
	std::stringstream msg;
	msg << "failed to find configuration for re-python plugin [" << _instance_name << "]";
	return ERROR(SYS_INVALID_INPUT_PARAM, msg.str());
}

static irods::error stop(irods::default_re_ctx&, const std::string&)
{
	PyEval_RestoreThread(python_state::ts_main);
	// Boost.Python's documentation advises not to call Py_Finalize
	// https://www.boost.org/doc/libs/1_78_0/libs/python/doc/html/tutorial/tutorial/embedding.html
	//Py_Finalize();
	return SUCCESS();
}

static irods::error rule_exists(const irods::default_re_ctx&, const std::string& rule_name, bool& _return)
{
	_return = false;
	python_gil_lock gil_lock;
	try {
		// TODO Enable non core.py Python rulebases
		bp::object core_module = bp::import("core");
		_return = PyObject_HasAttrString(core_module.ptr(), rule_name.c_str());
	}
	catch (const bp::error_already_set&) {
		const std::string formatted_python_exception = extract_python_exception();
		python_gil_unlock gil_release(&gil_lock, false);
		// clang-format off
		log_re::error({
			{"rule_engine_plugin", rule_engine_name},
			{"log_message", "caught python exception"},
			{"python_exception", formatted_python_exception},
		});
		// clang-format on
		std::string err_msg = std::string("irods_rule_engine_plugin_python::") + __PRETTY_FUNCTION__ +
		                      " Caught Python exception.\n" + formatted_python_exception;
		return ERROR(RULE_ENGINE_ERROR, err_msg);
	}
	// NOTE: If adding more catch blocks, nest this try/catch in another try block
	// along with the gil_lock definition. They need to stay in-scope for extract_python_exception,
	// but should be out of scope for other exception handlers.

	return SUCCESS();
}

static irods::error list_rules(const irods::default_re_ctx&, std::vector<std::string>& rule_vec)
{
	try {
		python_gil_lock gil_lock;
		// gil_lock needs to stay in scope for extract_python_exception
		// hence the nested exception handling
		try {
			bp::object core_module = bp::import("core");
			bp::object core_namespace = core_module.attr("__dict__");

			bp::exec("import inspect\n"
			         "import sys\n"
			         "function_list = inspect.getmembers(sys.modules['core'], inspect.isfunction)\n"
			         "function_names = [ tup[0] for tup in function_list ]\n",
			         core_namespace,
			         core_namespace);

			bp::list function_names = bp::extract<bp::list>(core_namespace["function_names"]);

			std::size_t len_names = bp::extract<std::size_t>(function_names.attr("__len__")());
			for (std::size_t i = 0; i < len_names; ++i) {
				rule_vec.push_back(bp::extract<std::string>(function_names[i]));
				std::string tmp = bp::extract<std::string>(function_names[i]);
			}
		}
		catch (const bp::error_already_set&) {
			const std::string formatted_python_exception = extract_python_exception();
			python_gil_unlock gil_release(&gil_lock, false);
			// clang-format off
			log_re::error({
				{"rule_engine_plugin", rule_engine_name},
				{"log_message", "caught python exception"},
				{"python_exception", formatted_python_exception},
			});
			// clang-format on
			auto start_pos = formatted_python_exception.find(IRODS_ERROR_PREFIX);
			int error_code_int = -1;
			if (start_pos != std::string::npos) {
				start_pos += IRODS_ERROR_PREFIX.size();
				auto end_pos = formatted_python_exception.find_first_of("]", start_pos);
				std::string error_code = formatted_python_exception.substr(start_pos, end_pos - start_pos);
				error_code_int = boost::lexical_cast<int>(error_code);
			}
			std::string err_msg = std::string("irods_rule_engine_plugin_python::") + __PRETTY_FUNCTION__ +
			                      " Caught Python exception.\n" + formatted_python_exception;
			return ERROR(error_code_int, err_msg);
		}
	}
	catch (const boost::bad_any_cast& e) {
		// clang-format off
		log_re::error({
			{"rule_engine_plugin", rule_engine_name},
			{"log_message", "bad any cast"},
			{"exception", e.what()},
		});
		// clang-format on
		std::string err_msg =
			std::string("irods_rule_engine_plugin_python::") + __PRETTY_FUNCTION__ + " bad_any_cast : " + e.what();
		return ERROR(INVALID_ANY_CAST, e.what());
	}

	return SUCCESS();
}

static irods::error exec_rule(const irods::default_re_ctx&,
                              const std::string& rule_name,
                              std::list<boost::any>& rule_arguments_cpp,
                              irods::callback effect_handler)
{
	try {
		python_gil_lock gil_lock;
		// gil_lock needs to stay in scope for extract_python_exception
		// hence the nested exception handling
		try {
			// TODO Enable non core.py Python rulebases
			bp::object core_module = bp::import("core");
			bp::object core_namespace = core_module.attr("__dict__");
			bp::object irods_types = bp::import("irods_types");
			bp::object irods_errors = bp::import("irods_errors");

			core_namespace["irods_types"] = irods_types;
			core_namespace["irods_errors"] = irods_errors;

			bp::object rule_function = core_module.attr(rule_name.c_str());

			const auto rei = get_rei_from_effect_handler(effect_handler);
			bp::list rule_arguments_python{};
			for (auto& cpp_argument : rule_arguments_cpp) {
				rule_arguments_python.append(object_from_any(cpp_argument));
			}

			const bp::object ec = rule_function(rule_arguments_python, CallbackWrapper{effect_handler}, rei);

			int i = 0;
			for (auto& cpp_argument : rule_arguments_cpp) {
				bp::object py_argument = rule_arguments_python[i];
				update_argument(cpp_argument, py_argument);
				++i;
			}

			return to_irods_error_object(ec);
		}
		catch (const bp::error_already_set&) {
			const std::string formatted_python_exception = extract_python_exception();
			// clang-format off
			python_gil_unlock gil_release(&gil_lock, false);
			log_re::error({
				{"rule_engine_plugin", rule_engine_name},
				{"log_message", "caught python exception"},
				{"python_exception", formatted_python_exception},
			});
			// clang-format on
			auto start_pos = formatted_python_exception.find(IRODS_ERROR_PREFIX);
			int error_code_int = -1;
			if (start_pos != std::string::npos) {
				start_pos += IRODS_ERROR_PREFIX.size();
				auto end_pos = formatted_python_exception.find_first_of("]", start_pos);
				std::string error_code = formatted_python_exception.substr(start_pos, end_pos - start_pos);
				error_code_int = boost::lexical_cast<int>(error_code);
			}
			std::string err_msg = std::string("irods_rule_engine_plugin_python::") + __PRETTY_FUNCTION__ +
			                      " Caught Python exception.\n" + formatted_python_exception;
			return ERROR(error_code_int, err_msg);
		}
	}
	catch (const boost::bad_any_cast& e) {
		// clang-format off
		log_re::error({
			{"rule_engine_plugin", rule_engine_name},
			{"log_message", "bad any cast"},
			{"exception", e.what()},
		});
		// clang-format on
		std::string err_msg =
			std::string("irods_rule_engine_plugin_python::") + __PRETTY_FUNCTION__ + " bad_any_cast : " + e.what();
		return ERROR(INVALID_ANY_CAST, e.what());
	}
	catch (...) {
		// clang-format off
		log_re::error({
			{"rule_engine_plugin", rule_engine_name},
			{"log_message", "Caught unknown exception"},
		});
		// clang-format on
		return ERROR(SYS_UNKNOWN_ERROR, "Caught unknown exception.");
	}

	return SUCCESS();
}

//irule
static irods::error exec_rule_text(const irods::default_re_ctx&,
                                   const std::string& rule_text,
                                   msParamArray_t* ms_params,
                                   const std::string& out_desc,
                                   irods::callback effect_handler)
{
	// Because Python is not sandboxed, need to restrict irule to admin users only
	const auto rei = get_rei_from_effect_handler(effect_handler);

	if (!rei) {
		// clang-format off
		log_re::error({
			{"rule_engine_plugin", rule_engine_name},
			{"log_message", "RuleExecInfo object is NULL - cannot authenticate user"},
		});
		// clang-format on
		return ERROR(NULL_VALUE_ERR, "Null rei pointer in exec_rule_text");
	}

	int client_user_authflag = 0;
	if (rei->uoic) {
		client_user_authflag = rei->uoic->authInfo.authFlag;
	}
	else if (rei->rsComm) {
		client_user_authflag = rei->rsComm->clientUser.authInfo.authFlag;
	}

	int proxy_user_authflag = 0;
	if (rei->uoip) {
		proxy_user_authflag = rei->uoip->authInfo.authFlag;
	}
	else if (rei->rsComm) {
		proxy_user_authflag = rei->rsComm->proxyUser.authInfo.authFlag;
	}

	if ((client_user_authflag < REMOTE_PRIV_USER_AUTH) || (proxy_user_authflag < REMOTE_PRIV_USER_AUTH)) {
		// clang-format off
		log_re::debug({
			{"rule_engine_plugin", rule_engine_name},
			{"log_message", "Insufficient privileges to run irule in Python rule engine plugin"},
		});
		// clang-format on
		return ERROR(SYS_NO_API_PRIV, "Insufficient privileges to run irule in Python rule engine plugin");
	}

	try {
		python_gil_lock gil_lock;
		// gil_lock needs to stay in scope for extract_python_exception
		// hence the nested exception handling
		try {
			execCmdOut_t* myExecCmdOut = (execCmdOut_t*) malloc(sizeof(*myExecCmdOut));
			memset(myExecCmdOut, 0, sizeof(*myExecCmdOut));

			addMsParam(ms_params, out_desc.c_str(), ExecCmdOut_MS_T, myExecCmdOut, NULL);

			// Convert INPUT and OUTPUT rule vars to Python dict
			bp::dict rule_vars_python;

			int i = 0;
			for (i = 0; i < ms_params->len; i++) {
				msParam_t* mp = ms_params->msParam[i];
				std::string label(mp->label);

				if (mp->type == NULL) {
					rule_vars_python[label] = NULL;
				}
				else if (std::strcmp(mp->type, DOUBLE_MS_T) == 0) {
					double* tmpDouble = (double*) mp->inOutStruct;
					rule_vars_python[label] = tmpDouble;
				}
				else if (std::strcmp(mp->type, INT_MS_T) == 0) {
					int* tmpInt = (int*) mp->inOutStruct;
					rule_vars_python[label] = tmpInt;
				}
				else if (std::strcmp(mp->type, STR_MS_T) == 0) {
					char* tmpChar = (char*) mp->inOutStruct;
					std::string tmpStr(tmpChar);
					rule_vars_python[label] = tmpStr;
				}
				else if (std::strcmp(mp->type, DATETIME_MS_T) == 0) {
					rodsLong_t* tmpRodsLong = (rodsLong_t*) mp->inOutStruct;
					rule_vars_python[label] = tmpRodsLong;
				}
			}

			if (strncmp(rule_text.c_str(), "@external\n", 10) == 0) {
				// If rule_text begins with "@external\n", call is of form
				//  irule -F inputFile ...

				// Import rule INPUT and OUTPUT variables
				bp::object builtin_module = bp::import("builtins");
				builtin_module.attr("irods_rule_vars") = rule_vars_python;

				bp::object main_module = bp::import("__main__");
				bp::object main_namespace = main_module.attr("__dict__");
				bp::object irods_types = bp::import("irods_types");
				bp::object irods_errors = bp::import("irods_errors");

				// deprecated alias for irods_rule_vars
				main_namespace["global_vars"] = rule_vars_python;

				// Import global constants
				main_namespace["irods_types"] = irods_types;
				main_namespace["irods_errors"] = irods_errors;

				// Parse input rule_text into useable Python fcns
				// Delete first line ("@external")
				std::string trimmed_rule = rule_text.substr(rule_text.find_first_of('\n') + 1);

				bp::exec(trimmed_rule.c_str(), main_namespace, main_namespace);
				bp::object rule_function = main_module.attr("main");

				bp::list rule_arguments_python{};
				return to_irods_error_object(
					rule_function(rule_arguments_python, CallbackWrapper{effect_handler}, rei));
			}
			else if (strncmp(rule_text.c_str(), "@external rule", 14) == 0) {
				// If rule_text begins with "@external ", call is of form
				//  irule rule ...

				// Import rule INPUT and OUTPUT variables
				bp::object builtin_module = bp::import("builtins");
				builtin_module.attr("irods_rule_vars") = rule_vars_python;

				// TODO Enable non core.py Python rulebases
				bp::object core_module = bp::import("core");
				bp::object core_namespace = core_module.attr("__dict__");

				// deprecated alias for irods_rule_vars
				core_namespace["global_vars"] = rule_vars_python;

				// Delete "@external rule { " from the start of the rule_text
				std::string trimmed_rule = rule_text.substr(17);

				// Extract rule name ("@external rule { RULE_NAME }")
				std::string rule_name = trimmed_rule.substr(0, trimmed_rule.find_first_of(' '));

				bp::object rule_function = core_module.attr(rule_name.c_str());

				bp::list rule_arguments_python{};
				return to_irods_error_object(
					rule_function(rule_arguments_python, CallbackWrapper{effect_handler}, rei));
			}
			else {
				python_gil_unlock gil_release(&gil_lock, false);
				// clang-format off
				log_re::error({
					{"rule_engine_plugin", rule_engine_name},
					{"log_message", "Improperly formatted rule text"},
				});
				// clang-format on
				return ERROR(RULE_ENGINE_ERROR, "Improperly formatted rule_text");
			}
		}
		catch (const bp::error_already_set&) {
			const std::string formatted_python_exception = extract_python_exception();
			python_gil_unlock gil_release(&gil_lock, false);
			// clang-format off
			log_re::error({
				{"rule_engine_plugin", rule_engine_name},
				{"log_message", "caught python exception"},
				{"python_exception", formatted_python_exception},
			});
			// clang-format on
			std::string err_msg = std::string("irods_rule_engine_plugin_python::") + __PRETTY_FUNCTION__ +
			                      " Caught Python exception.\n" + formatted_python_exception;
			return ERROR(RULE_ENGINE_ERROR, err_msg);
		}
	}
	catch (const boost::bad_any_cast& e) {
		// clang-format off
		log_re::error({
			{"rule_engine_plugin", rule_engine_name},
			{"log_message", "bad any cast"},
			{"exception", e.what()},
		});
		// clang-format on
		std::string err_msg =
			std::string("irods_rule_engine_plugin_python::") + __PRETTY_FUNCTION__ + " bad_any_cast : " + e.what();
		return ERROR(INVALID_ANY_CAST, e.what());
	}
	catch (...) {
		// clang-format off
		log_re::error({
			{"rule_engine_plugin", rule_engine_name},
			{"log_message", "Caught unknown exception"},
		});
		// clang-format on
		return ERROR(SYS_UNKNOWN_ERROR, "Caught unknown exception.");
	}

	return SUCCESS();
}

//delay execution
static irods::error exec_rule_expression(irods::default_re_ctx&,
                                         const std::string& rule_text,
                                         msParamArray_t* ms_params,
                                         irods::callback effect_handler)
{
	try {
		python_gil_lock gil_lock;
		// gil_lock needs to stay in scope for extract_python_exception
		// hence the nested exception handling
		try {
			bp::dict rule_vars_python;

			// Convert INPUT/OUTPUT rule vars to Python dict
			if (ms_params) {
				for (int i = 0; i < ms_params->len; i++) {
					if (msParam_t* mp = ms_params->msParam[i]) {
						std::string label(mp->label);

						if (mp->type == NULL) {
							rule_vars_python[label] = boost::python::object{};
						}
						else if (std::strcmp(mp->type, DOUBLE_MS_T) == 0) {
							double* tmpDouble = (double*) mp->inOutStruct;
							rule_vars_python[label] = tmpDouble;
						}
						else if (std::strcmp(mp->type, INT_MS_T) == 0) {
							int* tmpInt = (int*) mp->inOutStruct;
							rule_vars_python[label] = tmpInt;
						}
						else if (std::strcmp(mp->type, STR_MS_T) == 0) {
							char* tmpChar = (char*) mp->inOutStruct;
							std::string tmpStr(tmpChar);
							rule_vars_python[label] = tmpStr;
						}
						else if (std::strcmp(mp->type, DATETIME_MS_T) == 0) {
							rodsLong_t* tmpRodsLong = (rodsLong_t*) mp->inOutStruct;
							rule_vars_python[label] = tmpRodsLong;
						}
					}
				}
			}

			// Import rule INPUT and OUTPUT variables
			bp::object builtin_module = bp::import("builtins");
			builtin_module.attr("irods_rule_vars") = rule_vars_python;

			// Parse input rule_text into useable Python fcns
			bp::object main_module = bp::import("__main__");
			bp::object irods_types = bp::import("irods_types");
			bp::object irods_errors = bp::import("irods_errors");
			bp::object main_namespace = main_module.attr("__dict__");

			// deprecated alias for irods_rule_vars
			main_namespace["global_vars"] = rule_vars_python;

			// Import globals
			main_namespace["irods_types"] = irods_types;
			main_namespace["irods_errors"] = irods_errors;

			// Add def expressionFcn(rule_args, callback):\n to start of rule text
			std::string rule_name = "expressionFcn";
			std::string fcn_text = "def " + rule_name + "(rule_args, callback, rei):\n" + rule_text;
			// Replace every '\n' with '\n '
			boost::replace_all(fcn_text, "\n", "\n ");
			bp::exec(fcn_text.c_str(), main_namespace, main_namespace);
			bp::object rule_function = main_module.attr(rule_name.c_str());

			const auto rei = get_rei_from_effect_handler(effect_handler);
			bp::list rule_arguments_python{};
			return to_irods_error_object(rule_function(rule_arguments_python, CallbackWrapper{effect_handler}, rei));
		}
		catch (const bp::error_already_set&) {
			const std::string formatted_python_exception = extract_python_exception();
			python_gil_unlock gil_release(&gil_lock, false);
			// clang-format off
			log_re::error({
				{"rule_engine_plugin", rule_engine_name},
				{"log_message", "caught python exception"},
				{"python_exception", formatted_python_exception},
			});
			// clang-format on
			std::string err_msg = std::string("irods_rule_engine_plugin_python::") + __PRETTY_FUNCTION__ +
			                      " Caught Python exception.\n" + formatted_python_exception;
			return ERROR(RULE_ENGINE_ERROR, err_msg);
		}
	}
	catch (const boost::bad_any_cast& e) {
		// clang-format off
		log_re::error({
			{"rule_engine_plugin", rule_engine_name},
			{"log_message", "bad any cast"},
			{"exception", e.what()},
		});
		// clang-format on
		std::string err_msg =
			std::string("irods_rule_engine_plugin_python::") + __PRETTY_FUNCTION__ + " bad_any_cast : " + e.what();
		return ERROR(INVALID_ANY_CAST, e.what());
	}
	catch (...) {
		// clang-format off
		log_re::error({
			{"rule_engine_plugin", rule_engine_name},
			{"log_message", "Caught unknown exception"},
		});
		// clang-format on
		return ERROR(SYS_UNKNOWN_ERROR, "Caught unknown exception.");
	}

	return SUCCESS();
}

extern "C" irods::pluggable_rule_engine<irods::default_re_ctx>* plugin_factory(const std::string& _inst_name,
                                                                               const std::string& _context)
{
	irods::pluggable_rule_engine<irods::default_re_ctx>* re =
		new irods::pluggable_rule_engine<irods::default_re_ctx>(_inst_name, _context);
	re->add_operation<irods::default_re_ctx&, const std::string&>(
		"start", std::function<irods::error(irods::default_re_ctx&, const std::string&)>(start));

	re->add_operation<irods::default_re_ctx&, const std::string&>(
		"stop", std::function<irods::error(irods::default_re_ctx&, const std::string&)>(stop));

	re->add_operation<irods::default_re_ctx&, const std::string&, bool&>(
		"rule_exists", std::function<irods::error(irods::default_re_ctx&, const std::string&, bool&)>(rule_exists));

	re->add_operation<irods::default_re_ctx&, std::vector<std::string>&>(
		"list_rules", std::function<irods::error(irods::default_re_ctx&, std::vector<std::string>&)>(list_rules));

	re->add_operation<irods::default_re_ctx&, const std::string&, std::list<boost::any>&, irods::callback>(
		"exec_rule",
		std::function<irods::error(
			irods::default_re_ctx&, const std::string&, std::list<boost::any>&, irods::callback)>(exec_rule));

	re->add_operation<irods::default_re_ctx&, const std::string&, msParamArray_t*, const std::string&, irods::callback>(
		"exec_rule_text",
		std::function<irods::error(
			irods::default_re_ctx&, const std::string&, msParamArray_t*, const std::string&, irods::callback)>(
			exec_rule_text));

	re->add_operation<irods::default_re_ctx&, const std::string&, msParamArray_t*, irods::callback>(
		"exec_rule_expression",
		std::function<irods::error(irods::default_re_ctx&, const std::string&, msParamArray_t*, irods::callback)>(
			exec_rule_expression));

#if PY_VERSION_HEX >= 0x03080000
	Py_InitializeEx(0);
	try {
		bp::object mod_sys = bp::import("sys");
		bp::list sys_path = bp::extract<bp::list>(mod_sys.attr("path"));
		std::size_t sys_path_len = bp::extract<std::size_t>(sys_path.attr("__len__")());
		python_state::default_module_search_paths.reserve(sys_path_len);
		for (std::size_t i = 0; i < sys_path_len; ++i) {
			python_state::default_module_search_paths.push_back(bp::extract<std::wstring>(sys_path[i]));
		}
		python_state::default_module_search_paths_set = true;
	}
	catch (const bp::error_already_set&) {
		const std::string formatted_python_exception = extract_python_exception();
		// clang-format off
		log_re::error({
			{"rule_engine_plugin", rule_engine_name},
			{"instance_name", _inst_name},
			{"log_message", "caught python exception in plugin factory; will use legacy module search path initialization"},
			{"python_exception", formatted_python_exception},
		});
		// clang-format on
		python_state::default_module_search_paths.clear();
		python_state::default_module_search_paths.shrink_to_fit();
		python_state::default_module_search_paths_set = false;
	}
	Py_Finalize(); // We're not supposed to do this, but let's try it anyway.
#endif

	return re;
}
