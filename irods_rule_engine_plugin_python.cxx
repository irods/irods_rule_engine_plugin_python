#include <ctime>
#include <fstream>
#include <list>
#include <string>

#include "boost/any.hpp"
#include "boost/date_time.hpp"
#include "boost/algorithm/string/replace.hpp"
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"
#include "boost/python.hpp"
#include "boost/python/raw_function.hpp"
#include "boost/python/slice.hpp"

#include "irods_error.hpp"
#include "irods_re_plugin.hpp"
#include "irods_re_ruleexistshelper.hpp"
#include "irods_re_serialization.hpp"

//#include "configuration.hpp"
#include "irods_server_properties.hpp"

#include "reRuleExistsHelper.hpp"

#define LOG(...) log(__func__, ":", __LINE__, " ", __VA_ARGS__)

static std::map<std::string, std::string> PYTHON_GLOBALS {{"PYTHON_RE_RET_CODE", "code"}, {"PYTHON_RE_RET_STATUS", "status"}, {"PYTHON_RE_RET_OUTPUT", "output"}, {"PYTHON_MSPARAM_TYPE", "PYTHON_MSPARAM_TYPE"}, {"PYTHON_RODSOBJSTAT", "PYTHON_RODSOBJSTAT"}, {"PYTHON_INT_MS_T", "PYTHON_INT_MS_T"}, {"PYTHON_DOUBLE_MS_T", "PYTHON_DOUBLE_MS_T"}, {"PYTHON_GENQUERYINP_MS_T", "PYTHON_GENQUERYINP_MS_T"}, {"PYTHON_GENQUERYOUT_MS_T", "PYTHON_GENQUERYOUT_MS_T"}};

const std::string ELEMENT_TYPE = "ELEMENT_TYPE";
const std::string STRING_TYPE = "STRING_TYPE";
const std::string STRING_VALUE_KEY = "STRING_VALUE_KEY";
const std::string IRODS_ERROR_PREFIX = "[iRods__Error__Code:";

const std::string DEFAULT_RULE_REGEX = "ac[^ ]*";

namespace bp = boost::python;

void register_regexes_from_array(
    boost::any          _array,
    const std::string&  _instance_name) {

    try {
        const auto& arr = boost::any_cast< const std::vector< boost::any >& >( _array );
        for ( const auto& elem : arr ) {
            try {
                const auto tmp = boost::any_cast< const std::string& <( boost::any_cast< const std::unordered_map< std::string, boost::any >& >( elem ).at( irods::CFG_REGEX_KW ) );
                RuleExistsHelper::Instance()->registerRuleRegex( tmp );
                rodsLog( LOG_NOTICE, "register_regexes_from_array - regex: [%s]", tmp.c_str() );
            } catch ( const boost::bad_any_cast& ) {
                rodsLog( LOG_ERROR,
                         "[%s] - failed to cast pep_regex_to_match to string",
                         __FUNCTION__ );
                continue;
            }
        }
    } catch ( const boost::bad_any_cast& ) {
        std::stringstream msg;
        msg << "[" << _instance_name << "] failed to any_cast a std::vector<boost::any>&";
        THROW( INVALID_ANY_CAST, msg.str() );
    }
}

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
            //auto time = std::time(nullptr);
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

                    msParam_t* tmpMsParam = (msParam_t*) malloc(sizeof(*tmpMsParam));
                    memset(tmpMsParam, 0, sizeof(*tmpMsParam));

                    // TODO Refactor into irods_re_deserialization?
                    if (itr[ELEMENT_TYPE] == PYTHON_GLOBALS.at("PYTHON_GENQUERYINP_MS_T")) {
                        try {
                            genQueryInp_t* genQueryInp = (genQueryInp_t*) malloc(sizeof(genQueryInp_t));
                            memset(genQueryInp, 0, sizeof(genQueryInp_t));
                            
                            std::map<int, int> selectInpMap;
                            std::map<int, std::string> sqlCondInpMap;
                            std::map<std::string, std::string> condInputMap;
                            
                            for (std::map<std::string, std::string>::iterator map_itr = itr.begin(); map_itr != itr.end(); ++map_itr) {
                                if (map_itr->first == "maxRows")
                                    genQueryInp->maxRows = boost::lexical_cast<int>(map_itr->second);
                                else if (map_itr->first == "continueInx")
                                    genQueryInp->continueInx = boost::lexical_cast<int>(map_itr->second);
                                else if (map_itr->first == "rowOffset") 
                                    genQueryInp->rowOffset = boost::lexical_cast<int>(map_itr->second);
                                else if (map_itr->first == "options")
                                    genQueryInp->options = boost::lexical_cast<int>(map_itr->second);
                                else if (map_itr->first.substr(0,7) == "select_") {
                                    int selectInx = boost::lexical_cast<int>(map_itr->first.substr(7, map_itr->first.size()));
                                    int selectVal = boost::lexical_cast<int>(map_itr->second);
                                    selectInpMap[selectInx] = selectVal;
                                } else if (map_itr->first.substr(0,6) == "where_") {
                                    int sqlCondInx = boost::lexical_cast<int>(map_itr->first.substr(6, map_itr->first.size()));
                                    sqlCondInpMap[sqlCondInx] = map_itr->second;
                                } else if (map_itr->first == "ELEMENT_TYPE") {
                                    continue;
                                } else {
                                    // Any other key came from the condInput keyValPair
                                    condInputMap[map_itr->first] = map_itr->second;
                                }
                            }

                            for (auto& tmp : selectInpMap) {
                                addInxIval(&genQueryInp->selectInp, tmp.first, tmp.second);
                            }

                            for (auto& tmp: sqlCondInpMap) {
                                addInxVal(&genQueryInp->sqlCondInp, tmp.first, tmp.second.c_str());
                            }

                            for (auto& tmp: condInputMap) {
                                addKeyVal(&genQueryInp->condInput, tmp.first.c_str(), tmp.second.c_str());
                            }

                            fillMsParam(tmpMsParam, NULL, GenQueryInp_MS_T, genQueryInp, NULL);

                        } catch (std::exception&) {
                            std::string error_msg = "Bad any cast";
                            PyErr_SetString(PyExc_RuntimeError, error_msg.c_str());
                            bp::throw_error_already_set();
                        }
                    } else if (itr[ELEMENT_TYPE] == PYTHON_GLOBALS.at("PYTHON_GENQUERYOUT_MS_T")) {
                        try {
                            genQueryOut_t* genQueryOut = (genQueryOut_t*) malloc(sizeof(genQueryOut_t));
                            memset(genQueryOut, 0, sizeof(genQueryOut_t));

                            std::map<int, int> attriInxMap;
                            std::map<int, int> lenMap;
                            std::map<std::string, std::string> valueMap;

                            for (std::map<std::string, std::string>::iterator map_itr = itr.begin(); map_itr != itr.end(); ++map_itr) {
                                std::string firstVal = map_itr->first;
                                std::string secondVal = map_itr->second;
                                if (map_itr->first == "rowCnt")
                                    genQueryOut->rowCnt = boost::lexical_cast<int>(map_itr->second);
                                else if (map_itr->first == "attriCnt")
                                    genQueryOut->attriCnt = boost::lexical_cast<int>(map_itr->second);
                                else if (map_itr->first == "continueInx")
                                    genQueryOut->continueInx = boost::lexical_cast<int>(map_itr->second);
                                else if (map_itr->first == "totalRowCount")
                                    genQueryOut->totalRowCount = boost::lexical_cast<int>(map_itr->second);
                                else if (map_itr->first.substr(0,9) == "attriInx_") {
                                    int colInx = boost::lexical_cast<int>(map_itr->first.substr(9, map_itr->first.size()));
                                    int attriInx = boost::lexical_cast<int>(map_itr->second);
                                    attriInxMap[colInx] = attriInx;
                                } else if (map_itr->first.substr(0,4) == "len_") {
                                    int colInx = boost::lexical_cast<int>(map_itr->first.substr(4, map_itr->first.size()));
                                    int len = boost::lexical_cast<int>(map_itr->second);
                                    lenMap[colInx] = len;
                                } else if (map_itr->first.substr(0,6) == "value_") {
                                    std::string attribute_row = map_itr->first.substr(6, map_itr->first.size());
                                    valueMap[attribute_row] = map_itr->second;
                                } else
                                    continue;
                            }

                            for (int i = 0; i < genQueryOut->attriCnt; ++i) {
                                genQueryOut->sqlResult[i].attriInx = attriInxMap[i];
                                genQueryOut->sqlResult[i].len = lenMap[i];
                                int len = lenMap[i];
                                genQueryOut->sqlResult[i].value = (char *) malloc(genQueryOut->rowCnt * len);
                                for (auto& tmp: valueMap) {
                                    std::vector<std::string> tokens;
                                    boost::split(tokens, tmp.first, boost::is_any_of("_"));
                                    int colInx = boost::lexical_cast<int>(tokens[1]);
                                    if (colInx == i) {
                                        int rowInx = boost::lexical_cast<int>(tokens[0]);
                                        char *valuePtr = genQueryOut->sqlResult[i].value + rowInx * len;
                                        snprintf(valuePtr, len, "%s", tmp.second.c_str());
                                    }
                                }
                            }

                            fillMsParam(tmpMsParam, NULL, GenQueryOut_MS_T, genQueryOut, NULL);

                        } catch (std::exception&) {
                            std::string error_msg = "Bad any cast";
                            PyErr_SetString(PyExc_RuntimeError, error_msg.c_str());
                            bp::throw_error_already_set();
                        }
                    }
 
                    rule_args_cpp.push_back(tmpMsParam);
                }
            }
            LOG(log_msg.str(), "]");

            irods::error retVal = self.effect_handler(self.rule_name, irods::unpack(rule_args_cpp));

            if (!retVal.ok()) {
                if ((retVal.code() != CAT_NO_ROWS_FOUND) && (retVal.code() != CAT_SUCCESS_BUT_WITH_NO_INFO)) {
                    std::string returnString = IRODS_ERROR_PREFIX + boost::lexical_cast<std::string>(retVal.code()) + "] " + retVal.result().c_str();
                    PyErr_SetString(PyExc_RuntimeError, returnString.c_str());
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
                    } else if (strcmp(realType, GenQueryOut_MS_T) == 0) {
                        genQueryOut_t* tmpGenQueryOut = (genQueryOut_t*) tmpMsParam->inOutStruct;
                        rule_returns_cpp.push_back(tmpGenQueryOut);
                    } // TODO Need else if for each supported msParam type
                    else {
                        std::string error_msg = "Unsupported msParam type :";
                        error_msg += realType;
                        PyErr_SetString(PyExc_RuntimeError, error_msg.c_str());
                        bp::throw_error_already_set();
                    }
                    
                } else {
                    std::string error_msg = "Unsupported return type :";
                    error_msg += itr.type().name();
                    PyErr_SetString(PyExc_RuntimeError, error_msg.c_str());
                    bp::throw_error_already_set();
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
start(irods::default_re_ctx&, const std::string& _instance_name) {
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

    try {
        const auto& re_plugin_arr = irods::get_server_property< const std::vector< boost::any >& >( irods::CFG_RULE_ENGINES_KW );
        for ( const auto& elem : re_plugin_arr ) {
            const auto& plugin_config = boost::any_cast< const std::unordered_map< std::string, boost::any>& >( elem );
            const auto& inst_name = boost::any_cast< const std::string& >( plugin_config.at( irods::CFG_INSTANCE_NAME_KW ) );
            if ( inst_name == _instance_name ) {
                const auto& plugin_spec_cfg = boost::any_cast< const std::unordered_map< std::string, boost::any >& >( plugin_config.at( irods::CFG_PLUGIN_SPECIFIC_CONFIGURATION_KW ) );
                std::string core_py = get_string_array_from_array( plugin_spec_cfg.at( irods::CFG_RE_RULEBASE_SET_KW ) );

                if ( plugin_spec_cfg.count( irods::CFG_RE_PEP_REGEX_SET_KW ) > 0 ) {
                    register_regexes_from_array( plugin_spec_cfg.at( irods::CFG_RE_PEP_REGEX_SET_KW ),
                                                 _instance_name );
                } else {
                    RuleExistsHelper::Instance()->registerRuleRegex( STATIC_PEP_RULE_REGEX );
                    RuleExistsHelper::Instance()->registerRuleRegex( DYNAMIC_PEP_RULE_REGEX );

                    rodsLog( LOG_DEBUG,
                             "No regexes found in server_config for Python RE - using default regexes: [%s], [%s]",
                             STATIC_PEP_RULE_REGEX.c_str(),
                             DYNAMIC_PEP_RULE_REGEX.c_str() );
                }

                return SUCCESS();
            }
        }
    } catch ( const irods::exception& e ) {
        return irods::error(e);
    } catch ( const boost::bad_any_cast& e ) {
        return ERROR( INVALID_ANY_CAST, e.what() );
    } catch ( const std::out_of_range& e ) {
        return ERROR( KEY_NOT_FOUND, e.what() );
    }

    std::stringstream msg;
    msg << "failed to find configuration for re-python plugin ["
        << _instance_name << "]";
    rodsLog( LOG_ERROR, "%s" msg.str().c_str() );
    return ERROR( SYS_INVALID_INPUT_PARAM, msg.str() );
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
        auto start_pos = formatted_python_exception.find(IRODS_ERROR_PREFIX);
        int error_code_int = -1;
        if (start_pos != std::string::npos) {
            start_pos += IRODS_ERROR_PREFIX.size();
            auto end_pos = formatted_python_exception.find_first_of("]", start_pos);
            std::string error_code = formatted_python_exception.substr(start_pos, end_pos-start_pos);
            error_code_int = boost::lexical_cast<int>(error_code);
        }
        std::string err_msg = std::string("irods_rule_engine_plugin_python::") + __PRETTY_FUNCTION__ + " Caught Python exception.\n" + formatted_python_exception;
        return ERROR(error_code_int, err_msg);
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
        rule_arguments_python[out_desc] = NULL;

        // Parse input rule_text into useable Python fcns
        bp::object main_module = bp::import("__main__");
        bp::object main_namespace = main_module.attr("__dict__");
        // Import globals
        for (const auto& it : PYTHON_GLOBALS) {
            main_namespace[it.first] = it.second;
        }
        // Add def expressionFcn(rule_args, callback):\n to start of rule text
        std::string rule_name = "expressionFcn";
        std::string fcn_text = "def expressionFcn(rule_args, callback):\n" + rule_text;
        // Replace every '\n' with '\n '
        boost::replace_all(fcn_text, "\n", "\n ");
        bp::exec(fcn_text.c_str(), main_namespace, main_namespace);
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
