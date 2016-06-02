#include <ctime>
#include <fstream>
#include <list>
#include <string>

#include "boost/any.hpp"
#include "boost/date_time.hpp"
#include "boost/python.hpp"
#include "boost/python/raw_function.hpp"
#include "boost/python/slice.hpp"

#include "irods_error.hpp"
#include "irods_re_plugin.hpp"
#include "irods_re_serialization.hpp"

#define LOG(...) log(__func__, ":", __LINE__, " ", __VA_ARGS__)

static std::map<std::string, std::string> PYTHON_GLOBALS {{"PYTHON_RE_RET_CODE", "code"}, {"PYTHON_RE_RET_STATUS", "status"}, {"PYTHON_RE_RET_OUTPUT", "output"}, {"PYTHON_MSPARAM_TYPE", "PYTHON_MSPARAM_TYPE"}, {"PYTHON_RODSOBJSTAT", "PYTHON_RODSOBJSTAT"}, {"PYTHON_INT_MS_T", "PYTHON_INT_MS_T"}, {"PYTHON_DOUBLE_MS_T", "PYTHON_DOUBLE_MS_T"}, {"PYTHON_GENQUERYINP_MS_T", "PYTHON_GENQUERYINP_MS_T"}};

const std::string ELEMENT_TYPE = "ELEMENT_TYPE";
const std::string STRING_TYPE = "STRING_TYPE";
const std::string STRING_VALUE_KEY = "STRING_VALUE_KEY";

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

    namespace StringFromPythonUnicode {
        void*
        convertible(PyObject* py_obj) {
            if (PyUnicode_Check(py_obj)) {
                return py_obj;
            }
            return nullptr;
        }

        void
        construct(
            PyObject *py_obj,
            boost::python::converter::rvalue_from_python_stage1_data *data) {
            bp::handle<> h_encoded(PyUnicode_AsUTF8String(py_obj));
            void *storage = reinterpret_cast<boost::python::converter::rvalue_from_python_storage<std::string>*>(data)->storage.bytes;
            new (storage) std::string(PyString_AsString(h_encoded.get()));
            data->convertible = storage;
        }

        void
        register_converter() {
            bp::converter::registry::push_back(&convertible, &construct, bp::type_id<std::string>());
        }
    }
    
    bp::list
    serialize_parameter_list_of_boost_anys_to_python_list(std::list<boost::any> parameter_list_cpp) {
        bp::list parameter_list_python;

        try {
            for (auto itr = begin(parameter_list_cpp); itr != end(parameter_list_cpp); ++itr) {
                irods::re_serialization::serialized_parameter_t param;
                irods::error ret = irods::re_serialization::serialize_parameter(*itr, param);

                if (!ret.ok()) {
                    LOG("ERROR: Unsupported argument for calling re rules from the rule language");
                } else {
                    if (param.size() == 0) {
                        LOG("DEBUG: empty serialized map for parameter ");
                        parameter_list_python.append("");
                    }
                    else if (param.size() == 1) {
                        LOG("INFO: One parameter, assume it's a string");
                        parameter_list_python.append(param.begin()->second.c_str());
                    }
                    else {
                        bp::dict paramDict;
                        for (auto i : param) {
                            paramDict[i.first.c_str()] = i.second.c_str();
                        }
                        parameter_list_python.append(paramDict);	
                    }
                }
            }
        } catch (const bp::error_already_set&) {
            const std::string formatted_python_exception = extract_python_exception();
            LOG("caught python exception\n", formatted_python_exception);
            std::string err_msg = std::string("irods_rule_engine_plugin_python::") + __PRETTY_FUNCTION__ + " Caught Python exception.\n" + formatted_python_exception;
        }
        
        return parameter_list_python;
    }
    
    std::list< std::map<std::string, std::string> >
    convert_python_iterable_to_list_of_maps(const bp::object& in_itr) {
        std::list<std::map<std::string, std::string> > out_list;
        const auto N = bp::len(in_itr);
        for (int i=0; i<N; ++i) {
            std::map<std::string, std::string> tmpMap;
            bp::extract<std::string> s{in_itr[i]};
            if (s.check()) {
                std::string tmpStr = s;
                tmpMap[ELEMENT_TYPE] = STRING_TYPE;
                tmpMap[STRING_VALUE_KEY] = tmpStr;
                out_list.push_back(tmpMap);
            } else {
                bp::dict d = bp::extract<bp::dict>(in_itr[i]);
                if (d.has_key(PYTHON_GLOBALS.at("PYTHON_MSPARAM_TYPE"))) {
                    std::string type = bp::extract<std::string>(d[PYTHON_GLOBALS.at("PYTHON_MSPARAM_TYPE")]);
                    tmpMap[ELEMENT_TYPE] = type;
                    bp::list keys = d.keys();
                    for (int j=0; j < len(keys); ++j) {
                        bp::extract<std::string> extracted_key(keys[j]);
                        std::string key = extracted_key;

                        if (key == PYTHON_GLOBALS.at("PYTHON_MSPARAM_TYPE")) {
                            // already got type, don't need to repeat
                            continue;
                        } else {
                            bp::extract<std::string> extracted_val(d[key]);
                            std::string val = extracted_val;
                            tmpMap[key] = val;
                        }
                    }

                    out_list.push_back(tmpMap);

                } else {
                    std::string object_classname = boost::python::extract<std::string>(in_itr[i].attr("__class__").attr("__name__"));
                    std::stringstream err;
                    err << __PRETTY_FUNCTION__ << ": extract to msParam failed on element[" << i << "] of type [" << object_classname << "]";
                    LOG(err.str());
                    PyErr_SetString(PyExc_RuntimeError, err.str().c_str());
                    bp::throw_error_already_set();
                }
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
        static bp::dict call(const bp::tuple& args, const bp::dict& kwargs) {
            RuleCallWrapper& self = bp::extract<RuleCallWrapper&>(args[0]);
            auto time = std::time(nullptr);
            LOG("rule_name: ", self.rule_name);
            bp::tuple rule_args_python = bp::extract<bp::tuple>(args[bp::slice(1, bp::len(args))]);

            std::list< std::map<std::string, std::string> > rule_args_maps = convert_python_iterable_to_list_of_maps(rule_args_python);

            std::list<boost::any> rule_args_cpp;

            std::stringstream log_msg;

            log_msg << "REAL INPUT [";
            for (auto& itr : rule_args_maps) {
                if (itr[ELEMENT_TYPE] == STRING_TYPE) {
                    log_msg << itr[STRING_VALUE_KEY] << ", ";

                    rule_args_cpp.push_back(&itr[STRING_VALUE_KEY]);
                } else {
                    log_msg << itr[ELEMENT_TYPE] << "{";
                    for (std::map<std::string, std::string>::iterator mapItr = itr.begin(); mapItr != itr.end(); ++mapItr) {
                        log_msg << mapItr->first << " : " << mapItr->second << ", ";
                    }
                    log_msg << "}, ";

                    // TODO CONVERT DICT TO APPROPRIATE TYPE
                    msParam_t* tmpMsParam = (msParam_t*) malloc(sizeof(*tmpMsParam));
                    memset(tmpMsParam, 0, sizeof(*tmpMsParam));
 
                    rule_args_cpp.push_back(tmpMsParam);
                }
            }
            LOG(log_msg.str(), "]");

            irods::error retVal = self.effect_handler(self.rule_name, irods::unpack(rule_args_cpp));

            if (!retVal.ok()) {
                if ((retVal.code() != CAT_NO_ROWS_FOUND) && (retVal.code() != CAT_SUCCESS_BUT_WITH_NO_INFO)) {
                    PyErr_SetString(PyExc_RuntimeError, retVal.result().c_str());
                    bp::throw_error_already_set();
                }
            }

            log_msg.str(std::string{}); log_msg.clear();
            log_msg << "output [";
            bp::dict ret;
            ret[PYTHON_GLOBALS.at("PYTHON_RE_RET_CODE")] = retVal.code();
            ret[PYTHON_GLOBALS.at("PYTHON_RE_RET_STATUS")] = retVal.status();

            bp::list retList;
            
            std::list<boost::any> rule_returns_cpp;

            for (auto&& itr : rule_args_cpp) {
                if ((itr.type() == typeid(std::string)) || (itr.type() == typeid(std::string*))) {
                    rule_returns_cpp.push_back(itr);
                } else if (itr.type() == typeid(msParam_t*)) {
                    // convert to subtype
                    msParam_t* tmpMsParam = boost::any_cast<msParam_t*>(itr);
                    char* realType = tmpMsParam->type;

                    if (strcmp(realType, RodsObjStat_MS_T) == 0) {
                        rodsObjStat_t* tmpRodsObj = (rodsObjStat_t*) tmpMsParam->inOutStruct;
                        rule_returns_cpp.push_back(tmpRodsObj);
                    } else if (strcmp(realType, INT_MS_T) == 0) {
                        int* tmpInt = (int*) tmpMsParam->inOutStruct;
                        rule_returns_cpp.push_back(tmpInt);
                    } else if (strcmp(realType, DOUBLE_MS_T) == 0) {
                        double* tmpDouble = (double*) tmpMsParam->inOutStruct;
                        rule_returns_cpp.push_back(tmpDouble);
                    } else if (strcmp(realType, GenQueryInp_MS_T) == 0) {
                        genQueryInp_t* tmpGenQueryInp = (genQueryInp_t*) tmpMsParam->inOutStruct;
                        rule_returns_cpp.push_back(tmpGenQueryInp);
                    }// TODO Need else if for each supported msParam type
                    
                } else {
                    // XXX Error - unsupported type
                }
            }
            
            retList = serialize_parameter_list_of_boost_anys_to_python_list(rule_returns_cpp);

            ret[PYTHON_GLOBALS.at("PYTHON_RE_RET_OUTPUT")] = retList;
        
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
start(irods::default_re_ctx&, const std::string&) {
    dlopen("libpython2.7.so", RTLD_LAZY | RTLD_GLOBAL); // https://mail.python.org/pipermail/new-bugs-announce/2008-November/003322.html
    Py_InitializeEx(0);
    
    try {
        bp::object main_module = bp::import("__main__");
        bp::object main_namespace = main_module.attr("__dict__");
        bp::exec("import sys\n"
             "sys.path.append('/etc/irods')",
             main_namespace);

        bp::object core_module = bp::import("core");
        bp::object core_namespace = core_module.attr("__dict__");
        for (const auto& it : PYTHON_GLOBALS) {
            core_namespace[it.first] = it.second;
        }

        initplugin_wrappers();
        StringFromPythonUnicode::register_converter();
    } catch (const bp::error_already_set&) {
        const std::string formatted_python_exception = extract_python_exception();
        LOG("caught python exception\n", formatted_python_exception);
        std::string err_msg = std::string("irods_rule_engine_plugin_python::") + __PRETTY_FUNCTION__ + " Caught Python exception.\n" + formatted_python_exception;
        return ERROR(-1, err_msg);
    }

    return SUCCESS();
}

irods::error
stop(irods::default_re_ctx&, const std::string&) {
    return SUCCESS();
}

irods::error
rule_exists(irods::default_re_ctx&, std::string rule_name, bool& _return) {
    _return = false;
    try {
        bp::object core_module = bp::import("core");
        _return = PyObject_HasAttrString(core_module.ptr(), rule_name.c_str());
    } catch (const bp::error_already_set&) {
        const std::string formatted_python_exception = extract_python_exception();
        LOG("caught python exception\n", formatted_python_exception);
        std::string err_msg = std::string("irods_rule_engine_plugin_python::") + __PRETTY_FUNCTION__ + " Caught Python exception.\n" + formatted_python_exception;
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

        rule_arguments_python = serialize_parameter_list_of_boost_anys_to_python_list(rule_arguments_cpp);
        
        CallbackWrapper callback_wrapper{effect_handler};
        bp::object outVal = rule_function(rule_arguments_python, callback_wrapper);

        int i = 0;
        for (auto itr = begin(rule_arguments_cpp); itr != end(rule_arguments_cpp); ++itr) {
            if (itr->type() == typeid(std::string)) {
                boost::any_cast<std::string>(*itr) = bp::extract<std::string>(rule_arguments_python[i]);
            } else if (itr->type() == typeid(std::string*)) {
                *boost::any_cast<std::string*>(*itr) = bp::extract<std::string>(rule_arguments_python[i]);
            }
            i++;
        }
    } catch (const bp::error_already_set&) {
        const std::string formatted_python_exception = extract_python_exception();
        LOG("caught python exception\n", formatted_python_exception);
        std::string err_msg = std::string("irods_rule_engine_plugin_python::") + __PRETTY_FUNCTION__ + " Caught Python exception.\n" + formatted_python_exception;
        return ERROR(-1, err_msg);
    } catch (const boost::bad_any_cast& e) {
        LOG("bad any cast : ", e.what());
        std::string err_msg = std::string("irods_rule_engine_plugin_python::") + __PRETTY_FUNCTION__ + " bad_any_cast : " + e.what();
        return ERROR(-1, err_msg);
    }
    return SUCCESS();
}

irods::error
exec_rule_text(irods::default_re_ctx&, std::string rule_text, std::list<boost::any>& rule_arguments_cpp, irods::callback effect_handler) {
    try {
        LOG(rule_text);
        auto itr = begin(rule_arguments_cpp);
        ++itr;  // skip tuple
        ++itr;  // skip callback

        msParamArray_t* ms_params = boost::any_cast<msParamArray_t*>(*itr);

        ++itr;  // skip msparam
        std::string out_desc = *boost::any_cast<std::string*>(*itr);

        // Convert input/output params to Python dict
        bp::dict rule_arguments_python;

        int i = 0;
        for (i = 0; i < ms_params->len; i++) {
            msParam_t* mp = ms_params->msParam[i];
            std::string label(mp->label);

            LOG("msParam : ", mp->inOutStruct);

            if (mp->type == NULL) {
                rule_arguments_python[label] = NULL;
                LOG("msParam : ", mp->inOutStruct);
            } else if (strcmp(mp->type, DOUBLE_MS_T) == 0) {
                double* tmpDouble = (double*) mp->inOutStruct;
                LOG("msParam : ", tmpDouble);
                rule_arguments_python[label] = tmpDouble;
            } else if (strcmp(mp->type, INT_MS_T) == 0) {
                int* tmpInt = (int*) mp->inOutStruct;
                LOG("msParam : ", tmpInt);
                rule_arguments_python[label] = tmpInt;
            } else if (strcmp(mp->type, STR_MS_T) == 0) {
                char* tmpChar = (char*) mp->inOutStruct;
                std::string tmpStr(tmpChar);
                LOG("msParam : ", tmpStr);
                rule_arguments_python[label] = tmpStr;
            } else if (strcmp(mp->type, DATETIME_MS_T) == 0) {
                rodsLong_t* tmpRodsLong = (rodsLong_t*) mp->inOutStruct;
                LOG("msParam : ", tmpRodsLong);
                rule_arguments_python[label] = tmpRodsLong;
            }
        }

        LOG("Passed msParams list");
        // Create out parameter
        // XXX shouldn't just be null?
        // XXX what is ruleExecOut in the context of Python Re
        rule_arguments_python[out_desc] = NULL;

        // Parse input rule_text into useable Python fcns
        bp::object main_module = bp::import("__main__");
        bp::object main_namespace = main_module.attr("__dict__");
        // Import globals
        for (const auto& it : PYTHON_GLOBALS) {
            main_namespace[it.first] = it.second;
        }
        // Delete first line ("@external")
        std::string trimmed_rule = rule_text.substr(rule_text.find_first_of('\n')+1);
        LOG("trimmed rule text : ", trimmed_rule);
        // Extract rule name ("def RULE_NAME(parameters):")
        std::string rule_name = trimmed_rule.substr(4, trimmed_rule.find_first_of('(')-4);
        LOG("rule name : ", rule_name);
        bp::exec(trimmed_rule.c_str(), main_namespace, main_namespace);
        bp::object rule_function = main_module.attr(rule_name.c_str());

        // Call fcns
        CallbackWrapper callback_wrapper{effect_handler};
        bp::object outVal = rule_function(rule_arguments_python, callback_wrapper);

    } catch (const bp::error_already_set&) {
        const std::string formatted_python_exception = extract_python_exception();
        LOG("caught python exception\n", formatted_python_exception);
        std::string err_msg = std::string("irods_rule_engine_plugin_python::") + __PRETTY_FUNCTION__ + " Caught Python exception.\n" + formatted_python_exception;
        return ERROR(-1, err_msg);
    } catch (const boost::bad_any_cast& e) {
        LOG("bad any cast : ", e.what());
        std::string err_msg = std::string("irods_rule_engine_plugin_python::") + __PRETTY_FUNCTION__ + " bad_any_cast : " + e.what();
        return ERROR(-1, err_msg);
    }

    return SUCCESS();
}

irods::error
exec_rule_expression(irods::default_re_ctx&, std::string rule_text, std::list<boost::any>& rule_arguments_cpp, irods::callback effect_handler) {
    return SUCCESS();
}


extern "C"
irods::pluggable_rule_engine<irods::default_re_ctx>*
plugin_factory(const std::string& _inst_name, const std::string& _context) {
    irods::pluggable_rule_engine<irods::default_re_ctx>* re = new irods::pluggable_rule_engine<irods::default_re_ctx>( _inst_name , _context);
     re->add_operation<irods::default_re_ctx&,const std::string&>(
             "start",
             std::function<irods::error(irods::default_re_ctx&,const std::string&)>( start ) );
 
     re->add_operation<irods::default_re_ctx&,const std::string&>(
             "stop",
             std::function<irods::error(irods::default_re_ctx&,const std::string&)>( stop ) );
 
     re->add_operation<irods::default_re_ctx&, std::string, bool&>(
             "rule_exists",
             std::function<irods::error(irods::default_re_ctx&, std::string, bool&)>( rule_exists ) );

     re->add_operation<irods::default_re_ctx&,std::string,std::list<boost::any>&,irods::callback>(
             "exec_rule",
             std::function<irods::error(irods::default_re_ctx&,std::string,std::list<boost::any>&,irods::callback)>( exec_rule ) );

     re->add_operation<irods::default_re_ctx&,std::string,std::list<boost::any>&,irods::callback>(
             "exec_rule_text",
             std::function<irods::error(irods::default_re_ctx&,std::string,std::list<boost::any>&,irods::callback)>( exec_rule_text ) );
 
     re->add_operation<irods::default_re_ctx&,std::string,std::list<boost::any>&,irods::callback>(
             "exec_rule_expression",
             std::function<irods::error(irods::default_re_ctx&,std::string,std::list<boost::any>&,irods::callback)>( exec_rule_expression ) );

    return re;
}
