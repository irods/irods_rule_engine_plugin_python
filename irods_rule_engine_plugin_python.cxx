#include <ctime>
#include <fstream>
#include <list>
#include <string>

#include <boost/any.hpp>
#include <boost/date_time.hpp>
#include <boost/python.hpp>
#include <boost/python/raw_function.hpp>
#include <boost/python/slice.hpp>

#include <irods/irods_error.hpp>
#include <irods/irods_re_plugin.hpp>

#define LOG(...) log(__func__, ":", __LINE__, " ", __VA_ARGS__)

namespace bp = boost::python;

namespace {
    const std::string LOG_FILE = "/tmp/irods-python-plugin.log";

    template<typename L, typename T>
    void
    log_impl(L& logger, const T& arg) {
        logger << arg;
    }

    template<typename L, typename T, typename... Types>
    void
    log_impl(L& logger, const T& arg, Types&&... args) {
        logger << arg;
        log_impl(logger, args...);
    }

    template<typename... Types>
    void
    log(Types&&... args) {
        std::ofstream l{LOG_FILE, std::ofstream::out | std::ofstream::app};
        auto now = boost::posix_time::microsec_clock::universal_time();
        l << boost::posix_time::to_iso_extended_string(now) << " ";
        log_impl(l, args...);
        l << std::endl;
    }

    std::string
    extract_python_exception(void) {
        PyObject *exc,*val,*tb;
        PyErr_Fetch(&exc,&val,&tb);
        PyErr_NormalizeException(&exc,&val,&tb);
        bp::handle<> hexc(exc), hval(bp::allow_null(val)), htb(bp::allow_null(tb));
        if (!hval) {
            return bp::extract<std::string>(bp::str(hexc));
        } else {
            bp::object traceback(bp::import("traceback"));
            bp::object format_exception(traceback.attr("format_exception"));
            bp::object formatted_list(format_exception(hexc,hval,htb));
            bp::object formatted(bp::str("").join(formatted_list));
            return bp::extract<std::string>(formatted);
        }
    }

    std::list<std::string>
    convert_python_iterable_of_strings_to_cpp(const bp::object& in_itr) {
        std::list<std::string> out_list;
        const auto N = bp::len(in_itr);
        for (int i=0; i<N; ++i) {
            bp::extract<std::string> s{in_itr[i]};
            if (s.check()) {
                out_list.push_back(s);
            } else {
                std::stringstream err;
                err << __PRETTY_FUNCTION__ << ": extract to string failed on element[" << i << "]";
                LOG(err.str());
                PyErr_SetString(PyExc_RuntimeError, err.str().c_str());
                bp::throw_error_already_set();
            }
        }
        return out_list;
    }

    struct RuleCallWrapper {
        RuleCallWrapper(irods::callback& effect_handler, std::string rule_name)
            : effect_handler{effect_handler}
            , rule_name{rule_name}
            {}
        irods::callback& effect_handler;
        std::string rule_name;
        static bp::list call(const bp::tuple& args, const bp::dict& kwargs) {
            RuleCallWrapper& self = bp::extract<RuleCallWrapper&>(args[0]);
            auto time = std::time(nullptr);
            LOG("rule_name: ", self.rule_name);
            bp::tuple rule_args_python = bp::extract<bp::tuple>(args[bp::slice(1, bp::len(args))]);
            std::list<std::string> rule_args_storage = convert_python_iterable_of_strings_to_cpp(rule_args_python);

            std::list<boost::any> rule_args_cpp;
            std::stringstream log_msg;
            log_msg << "input  [";
            for (auto&& itr : rule_args_storage) {
                rule_args_cpp.push_back(&itr);
                log_msg << itr << ", ";
            }
            LOG(log_msg.str(), "]");

            self.effect_handler(self.rule_name, irods::unpack(rule_args_cpp));

            log_msg.str(std::string{}); log_msg.clear();
            log_msg << "output [";
            bp::list ret;
            for (auto&& itr : rule_args_storage) {
                ret.append(itr);
                log_msg << itr << ", ";
            }
            LOG(log_msg.str(), "]");
            return ret;
        }
    };

    struct CallbackWrapper {
        CallbackWrapper(irods::callback& effect_handler) : effect_handler{effect_handler} {}
        irods::callback& effect_handler;
        RuleCallWrapper getAttribute(const std::string& rule_name) {
            return RuleCallWrapper{effect_handler, rule_name};
        }
    };

    BOOST_PYTHON_MODULE(plugin_wrappers)
    {
        bp::class_<RuleCallWrapper>("RuleCallWrapper", bp::no_init)
            .def("__call__", bp::raw_function(&RuleCallWrapper::call, 1))
            ;

        bp::class_<CallbackWrapper>("CallbackWrapper", bp::no_init)
            .def("__getattribute__", &CallbackWrapper::getAttribute)
            ;
    }
}

irods::error
start(irods::default_re_ctx&) {
    dlopen("libpython2.7.so", RTLD_LAZY | RTLD_GLOBAL); // https://mail.python.org/pipermail/new-bugs-announce/2008-November/003322.html
    Py_InitializeEx(0);
    bp::object main_module = bp::import("__main__");
    bp::object main_namespace = main_module.attr("__dict__");
    bp::exec("import sys\n"
             "sys.path.append('/etc/irods')",
             main_namespace);
    initplugin_wrappers();
    return SUCCESS();
}

irods::error
stop(irods::default_re_ctx&) {
    return SUCCESS();
}

irods::error
rule_exists(irods::default_re_ctx&, std::string rule_name, bool& _return) {
    _return = false;
    try {
        bp::object core_module = bp::import("core");
        _return = PyObject_HasAttrString(core_module.ptr(), rule_name.c_str());
    } catch (const bp::error_already_set&) {
        LOG("caught python exception\n", extract_python_exception());
        std::string err_msg = "irods-re-python::rule_exists Python exception. Check " + LOG_FILE;
        return ERROR(-1, err_msg);
    }
    return SUCCESS();
}

irods::error
exec_rule(irods::default_re_ctx&, std::string rule_name, std::list<boost::any>& rule_arguments_cpp, irods::callback effect_handler) {
    try {
        bp::object core_module = bp::import("core");
        bp::object rule_function = core_module.attr(rule_name.c_str());
        bp::list rule_arguments_python;
        for (auto&& itr : rule_arguments_cpp) {
            if (itr.type() == typeid(std::string)) {
                rule_arguments_python.append(boost::any_cast<std::string>(itr));
            } else if (itr.type() == typeid(std::string*)) {
                rule_arguments_python.append(*boost::any_cast<std::string*>(itr));
            } else {
                LOG("unhandled type in boost::any ", itr.type().name());
                return ERROR(-1, std::string("irods-re-python::exec_rule unhandled type in boost::any ") + itr.type().name());
            }
        }

        CallbackWrapper callback_wrapper{effect_handler};
        rule_function(rule_arguments_python, callback_wrapper);

        std::list<std::string> processed_arguments_cpp = convert_python_iterable_of_strings_to_cpp(rule_arguments_python);
        auto out = rule_arguments_cpp.begin();
        for (auto&& in : processed_arguments_cpp) {
            if (out->type() == typeid(std::string)) {
                *out = in;
            } else {
                *boost::any_cast<std::string*>(*out) = in;
            }
            ++out;
        }

    } catch (const bp::error_already_set&) {
        LOG("caught python exception\n", extract_python_exception());
        std::string err_msg = "irods-re-python::exec_rule Python exception. Check " + LOG_FILE;
        return ERROR(-1, err_msg);
    } catch (const boost::bad_any_cast& e) {
        LOG("bad any cast : ", e.what());
        std::string err_msg = std::string("irods-re-python::exec_rule bad_any_cast : ") + e.what();
        return ERROR(-1, err_msg);
    }
    return SUCCESS();
}

extern "C"
irods::pluggable_rule_engine<irods::default_re_ctx>*
plugin_factory(const std::string& _inst_name, const std::string& _context) {
    irods::pluggable_rule_engine<irods::default_re_ctx>* re = new irods::pluggable_rule_engine<irods::default_re_ctx>( _inst_name , _context);
    re->add_operation<irods::default_re_ctx&>("start", std::function<irods::error(irods::default_re_ctx&)>(start));
    re->add_operation<irods::default_re_ctx&>("stop", std::function<irods::error(irods::default_re_ctx&)>(stop));
    re->add_operation<irods::default_re_ctx&, std::string, bool&>("rule_exists", std::function<irods::error(irods::default_re_ctx&, std::string, bool&)>(rule_exists));
    re->add_operation<irods::default_re_ctx&, std::string, std::list<boost::any>&, irods::callback>("exec_rule", std::function<irods::error(irods::default_re_ctx&, std::string, std::list<boost::any>&, irods::callback)>(exec_rule));
    return re;
}
