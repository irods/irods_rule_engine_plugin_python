#include "irods_types.hpp"

#define BOOST_SYSTEM_NO_DEPRECATED BOOST_PYTHON_MAX_ARITY 45

#include <boost/python/module_init.hpp>
#include <boost/python/detail/none.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/exception_translator.hpp>
#include <boost/format.hpp>


#include "init_struct.hpp"
#include "array_indexing_suite.hpp"
#include "array_ref.hpp"

namespace bp = boost::python;

bool operator==(const sqlResult_t& s1, const sqlResult_t& s2) {
    return false;
}

namespace {
    BOOST_PYTHON_MODULE(irods_types)
    {
        bp::enum_<specCollClass_t>("specCollClass")
            .value("NO_SPEC_COLL", specCollClass_t::NO_SPEC_COLL)
            .value("STRUCT_FILE_COLL", specCollClass_t::STRUCT_FILE_COLL)
            .value("MOUNTED_COLL", specCollClass_t::MOUNTED_COLL)
            .value("LINKED_COLL", specCollClass_t::LINKED_COLL)
            .export_values();

        bp::enum_<structFileType_t>("structFileType")
            .value("NONE_STRUCT_FILE", structFileType_t::NONE_STRUCT_FILE_T)
            .value("HAAW_STRUCT_FILE", structFileType_t::HAAW_STRUCT_FILE_T)
            .value("TAR_STRUCT_FILE", structFileType_t::TAR_STRUCT_FILE_T)
            .value("MSSO_STRUCT_FILE", structFileType_t::MSSO_STRUCT_FILE_T)
            .export_values();

        bp::enum_<objType_t>("objType")
            .value("UNKNOWN_OBJ", objType_t::UNKNOWN_OBJ_T)
            .value("DATA_OBJ", objType_t::DATA_OBJ_T)
            .value("COLL_OBJ", objType_t::COLL_OBJ_T)
            .value("UNKNOWN_FILE", objType_t::UNKNOWN_FILE_T)
            .value("LOCAL_FILE", objType_t::LOCAL_FILE_T)
            .value("LOCAL_DIR", objType_t::LOCAL_DIR_T)
            .export_values();

        bp::enum_<procState_t>("procState")
            .value("PROCESSING_STATE", procState_t::PROCESSING_STATE)
            .value("RECEIVING_STATE", procState_t::RECEIVING_STATE)
            .value("SENDING_STATE", procState_t::SENDING_STATE)
            .value("CONN_WAIT_STATE", procState_t::CONN_WAIT_STATE)
            .export_values();

        bp::enum_<procLogFlag_t>("procLogFlag")
            .value("PROC_LOG_NOT_DONE", procLogFlag_t::PROC_LOG_NOT_DONE)
            .value("PROC_LOG_DONE", procLogFlag_t::PROC_LOG_DONE)
            .export_values();

        bp::enum_<irodsProt_t>("irodsProt")
            .value("NATIVE_PROT", irodsProt_t::NATIVE_PROT)
            .value("XML_PROT", irodsProt_t::XML_PROT)
            .export_values();

        bp::enum_<fileRestartFlag_t>("fileRestartFlag")
            .value("FILE_RESTART_OFF", fileRestartFlag_t::FILE_RESTART_OFF)
            .value("FILE_RESTART_ON", fileRestartFlag_t::FILE_RESTART_ON)
            .export_values();

        bp::enum_<fileRestartStatus_t>("fileRestartStatus")
            .value("FILE_NOT_RESTART", fileRestartStatus_t::FILE_NOT_RESTART)
            .value("FILE_RESTARTED", fileRestartStatus_t::FILE_RESTARTED)
            .export_values();

        bp::class_<array_ref<int>>("int_array")
            .def( array_indexing_suite<array_ref<int>>{} )
            ;

        bp::class_<array_ref<sqlResult_t>>("SqlResult_array")
            .def( array_indexing_suite<array_ref<sqlResult_t>>{} )
            ;

        bp::class_<array_ref<rErrMsg_t*>>("RErrMsg_pointer_array")
            .def( array_indexing_suite<array_ref<rErrMsg_t*>>{} ) //, bp::return_internal_reference<1>>{} )
            ;

        bp::class_<array_ref<msParam_t*>>("MsParam_pointer_array")
            .def( array_indexing_suite<array_ref<msParam_t*>>{} ) //, bp::return_internal_reference<1>>{} )
            ;

        bp::class_<array_ref<char>>("char_array")
            .def( array_indexing_suite<array_ref<char>>{} )
            .def( "__str__", +[](array_ref<char>* s) { return std::string{s->data()}; })
            ;

        bp::class_<array_ref<unsigned char>>("unsigned_char_array")
            .def( array_indexing_suite<array_ref<unsigned char>>{} )
            ;

        bp::class_<array_ref<array_ref<char, true>>>("c_string_array")
            .def( array_indexing_suite<array_ref<array_ref<char, true>>>{} )
            ;

        bp::class_<array_ref<char, true>>("c_string")
            .def( array_indexing_suite<array_ref<char, true>>{} )
            .def( "__str__", +[](array_ref<char, true>* s) { return std::string{s->data()}; })
            ;

        bp::class_<specColl_t>("SpecColl", bp::no_init)
            .def("__init__", make_init_function<specColl_t>(
                            &specColl_t::collClass,
                            &specColl_t::type,
                            &specColl_t::collection,
                            &specColl_t::objPath,
                            &specColl_t::resource,
                            &specColl_t::rescHier,
                            &specColl_t::phyPath,
                            &specColl_t::cacheDir,
                            &specColl_t::cacheDirty,
                            &specColl_t::replNum))
            .def_readwrite("collClass", &specColl_t::collClass)
            .def_readwrite("type", &specColl_t::type)
            .add_property("collection", +[](specColl_t *s) { return array_ref<char>{s->collection}; })
            .add_property("objPath", +[](specColl_t *s) { return array_ref<char>{s->objPath}; })
            .add_property("resource", +[](specColl_t *s) { return array_ref<char>{s->resource}; })
            .add_property("rescHier", +[](specColl_t *s) { return array_ref<char>{s->rescHier}; })
            .add_property("phyPath", +[](specColl_t *s) { return array_ref<char>{s->phyPath}; })
            .add_property("cacheDir", +[](specColl_t *s) { return array_ref<char>{s->cacheDir}; })
            .def_readwrite("cacheDirty", &specColl_t::cacheDirty)
            .def_readwrite("replNum", &specColl_t::replNum)
            ;

        bp::class_<rodsObjStat_t>("RodsObjStat", bp::no_init)
            .def("__init__", make_init_function<rodsObjStat_t>(
                            &rodsObjStat_t::objSize,
                            &rodsObjStat_t::objType,
                            &rodsObjStat_t::dataMode,
                            &rodsObjStat_t::dataId,
                            &rodsObjStat_t::chksum,
                            &rodsObjStat_t::ownerName,
                            &rodsObjStat_t::ownerZone,
                            &rodsObjStat_t::createTime,
                            &rodsObjStat_t::modifyTime,
                            &rodsObjStat_t::specColl,
                            &rodsObjStat_t::rescHier))
            .def_readwrite("objSize", &rodsObjStat_t::objSize)
            .def_readwrite("objType", &rodsObjStat_t::objType)
            .def_readwrite("dataMode", &rodsObjStat_t::dataMode)
            .add_property("dataId", +[](rodsObjStat_t *s) { return array_ref<char>{s->dataId}; })
            .add_property("chksum", +[](rodsObjStat_t *s) { return array_ref<char>{s->chksum}; })
            .add_property("ownerName", +[](rodsObjStat_t *s) { return array_ref<char>{s->ownerName}; })
            .add_property("ownerZone", +[](rodsObjStat_t *s) { return array_ref<char>{s->ownerZone}; })
            .add_property("createTime", +[](rodsObjStat_t *s) { return array_ref<char>{s->createTime}; })
            .add_property("modifyTime", +[](rodsObjStat_t *s) { return array_ref<char>{s->modifyTime}; })
            .add_property("specColl",
                    bp::make_getter(&rodsObjStat_t::specColl, bp::return_internal_reference<1>{}),
                    bp::make_setter(&rodsObjStat_t::specColl, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("rescHier", +[](rodsObjStat_t *s) { return array_ref<char>{s->rescHier}; })
            ;

        bp::class_<keyValPair_t>("KeyValPair", bp::no_init)
            .def("__init__", make_init_function<keyValPair_t>(
                            &keyValPair_t::len,
                            &keyValPair_t::keyWord,
                            &keyValPair_t::value))
            .add_property("len", &keyValPair_t::len)
            .add_property("key", +[](keyValPair_t *s) { return array_ref<array_ref<char, true>>{s->keyWord, static_cast<std::size_t>(s->len)}; })
            .add_property("value", +[](keyValPair_t *s) { return array_ref<array_ref<char, true>>{s->value, static_cast<std::size_t>(s->len)}; })
            .def("__getitem__", +[](keyValPair_t* s, const std::string key) {
                    for (int i = 0; i < s->len; ++i) {
                        if ( strcmp(key.c_str(), s->keyWord[i]) == 0 ) {
                            return std::string{s->value[i]};
                        }
                    }
                    PyErr_SetString(PyExc_KeyError, (boost::format{"%s was not found in the list of keys."} % key).str().c_str());
                    bp::throw_error_already_set();
                    return std::string{};
                })

            ;

        bp::class_<inxIvalPair_t>("InxIvalPair", bp::no_init)
            .def("__init__", make_init_function<inxIvalPair_t>(
                            &inxIvalPair_t::len,
                            &inxIvalPair_t::inx,
                            &inxIvalPair_t::value))
            .add_property("len", &inxIvalPair_t::len)
            .add_property("inx", +[](inxIvalPair_t *s) { return array_ref<int>{s->inx, static_cast<std::size_t>(s->len)}; })
            .add_property("value", +[](inxIvalPair_t *s) { return array_ref<int>{s->value, static_cast<std::size_t>(s->len)}; })
            ;

        bp::class_<inxValPair_t>("InxValPair", bp::no_init)
            .def("__init__", make_init_function<inxValPair_t>(
                            &inxValPair_t::len,
                            &inxValPair_t::inx,
                            &inxValPair_t::value))
            .add_property("len", &inxValPair_t::len)
            .add_property("inx", +[](inxValPair_t *s) { return array_ref<int>{s->inx, static_cast<std::size_t>(s->len)}; })
            .add_property("value", +[](inxValPair_t *s) { return array_ref<array_ref<char, true>>{s->value, static_cast<std::size_t>(s->len)}; })
            ;

        bp::class_<genQueryInp_t>("GenQueryInp", bp::no_init)
            .def("__init__", make_init_function<genQueryInp_t>(
                            &genQueryInp_t::maxRows,
                            &genQueryInp_t::continueInx,
                            &genQueryInp_t::rowOffset,
                            &genQueryInp_t::options,
                            &genQueryInp_t::condInput,
                            &genQueryInp_t::selectInp,
                            &genQueryInp_t::sqlCondInp))
            .def_readwrite("maxRows", &genQueryInp_t::maxRows)
            .def_readwrite("continueInx", &genQueryInp_t::continueInx)
            .def_readwrite("rowOffset", &genQueryInp_t::rowOffset)
            .def_readwrite("options", &genQueryInp_t::options)
            .def_readwrite("condInput", &genQueryInp_t::condInput)
            .def_readwrite("selectInp", &genQueryInp_t::selectInp)
            .def_readwrite("sqlCondInp", &genQueryInp_t::sqlCondInp)
            ;

        bp::class_<sqlResult_t>("SqlResult", bp::no_init)
            .def("__init__", make_init_function<sqlResult_t>(
                            &sqlResult_t::attriInx,
                            &sqlResult_t::len,
                            &sqlResult_t::value))
            .def_readwrite("attriInx", &sqlResult_t::attriInx)
            .add_property("len", &sqlResult_t::len)
            .def("row", +[](sqlResult_t *s, std::size_t row) { return std::string{&(s->value[row * s->len])}; })
            .add_property("value", +[](sqlResult_t *s) {
                    PyErr_SetString(PyExc_RuntimeError, "Value cannot be directly accessed due to unknown number of rows, access the rows in value using \"row\" property instead.");
                    bp::throw_error_already_set();
                    })
            ;

        bp::class_<genQueryOut_t>("GenQueryOut", bp::no_init)
            .def("__init__", make_init_function<genQueryOut_t>(
                            &genQueryOut_t::rowCnt,
                            &genQueryOut_t::attriCnt,
                            &genQueryOut_t::continueInx,
                            &genQueryOut_t::totalRowCount,
                            &genQueryOut_t::sqlResult))
            .add_property("rowCnt", &genQueryOut_t::rowCnt)
            .add_property("attriCnt", &genQueryOut_t::attriCnt)
            .add_property("continueInx", &genQueryOut_t::continueInx)
            .add_property("totalRowCount", &genQueryOut_t::totalRowCount)
            .add_property("sqlResult", +[](genQueryOut_t *s) { return array_ref<sqlResult_t>{s->sqlResult}; })
            ;

        bp::class_<collInfo_t>("CollInfo", bp::no_init)
            .def("__init__", make_init_function<collInfo_t>(
                            &collInfo_t::collId,
                            &collInfo_t::collName,
                            &collInfo_t::collParentName,
                            &collInfo_t::collOwnerName,
                            &collInfo_t::collOwnerZone,
                            &collInfo_t::collMapId,
                            &collInfo_t::collAccessInx,
                            &collInfo_t::collComments,
                            &collInfo_t::collInheritance,
                            &collInfo_t::collExpiry,
                            &collInfo_t::collCreate,
                            &collInfo_t::collModify,
                            &collInfo_t::collAccess,
                            &collInfo_t::collType,
                            &collInfo_t::collInfo1,
                            &collInfo_t::collInfo2,
                            &collInfo_t::condInput,
                            &collInfo_t::next))
            .add_property("collId", &collInfo_t::collId)
            .add_property("collName", +[](collInfo_t *s) { return array_ref<char>{s->collName}; })
            .add_property("collParentName", +[](collInfo_t *s) { return array_ref<char>{s->collParentName}; })
            .add_property("collOwnerName", +[](collInfo_t *s) { return array_ref<char>{s->collOwnerName}; })
            .add_property("collOwnerZone", +[](collInfo_t *s) { return array_ref<char>{s->collOwnerZone}; })
            .add_property("collMapId", &collInfo_t::collMapId)
            .add_property("collAccessInx", &collInfo_t::collAccessInx)
            .add_property("collComments", +[](collInfo_t *s) { return array_ref<char>{s->collComments}; })
            .add_property("collInheritance", +[](collInfo_t *s) { return array_ref<char>{s->collInheritance}; })
            .add_property("collExpiry", +[](collInfo_t *s) { return array_ref<char>{s->collExpiry}; })
            .add_property("collCreate", +[](collInfo_t *s) { return array_ref<char>{s->collCreate}; })
            .add_property("collModify", +[](collInfo_t *s) { return array_ref<char>{s->collModify}; })
            .add_property("collAccess", +[](collInfo_t *s) { return array_ref<char>{s->collAccess}; })
            .add_property("collType", +[](collInfo_t *s) { return array_ref<char>{s->collType}; })
            .add_property("collInfo1", +[](collInfo_t *s) { return array_ref<char>{s->collInfo1}; })
            .add_property("collInfo2", +[](collInfo_t *s) { return array_ref<char>{s->collInfo2}; })
            .add_property("condInput", &collInfo_t::condInput)
            .add_property("next",
                    bp::make_getter(&collInfo_t::next, bp::return_internal_reference<1>{}),
                    bp::make_setter(&collInfo_t::next, bp::with_custodian_and_ward<1, 2>{}))
            ;

        bp::class_<dataObjInfo_t>("DataObjInfo", bp::no_init)
            .def("__init__", make_init_function<dataObjInfo_t>(
                            &dataObjInfo_t::objPath,
                            &dataObjInfo_t::rescName,
                            &dataObjInfo_t::rescHier,
                            &dataObjInfo_t::dataType,
                            &dataObjInfo_t::dataSize,
                            &dataObjInfo_t::chksum,
                            &dataObjInfo_t::version,
                            &dataObjInfo_t::filePath,
                            &dataObjInfo_t::dataOwnerName,
                            &dataObjInfo_t::dataOwnerZone,
                            &dataObjInfo_t::replNum,
                            &dataObjInfo_t::replStatus,
                            &dataObjInfo_t::statusString,
                            &dataObjInfo_t::dataId,
                            &dataObjInfo_t::collId,
                            &dataObjInfo_t::dataMapId,
                            &dataObjInfo_t::flags,
                            &dataObjInfo_t::dataComments,
                            &dataObjInfo_t::dataMode,
                            &dataObjInfo_t::dataExpiry,
                            &dataObjInfo_t::dataCreate,
                            &dataObjInfo_t::dataModify,
                            &dataObjInfo_t::dataAccess,
                            &dataObjInfo_t::dataAccessInx,
                            &dataObjInfo_t::writeFlag,
                            &dataObjInfo_t::destRescName,
                            &dataObjInfo_t::backupRescName,
                            &dataObjInfo_t::subPath,
                            &dataObjInfo_t::specColl,
                            &dataObjInfo_t::regUid,
                            &dataObjInfo_t::otherFlags,
                            &dataObjInfo_t::condInput,
                            &dataObjInfo_t::in_pdmo,
                            &dataObjInfo_t::next,
                            &dataObjInfo_t::rescId))
            .add_property("objPath", +[](dataObjInfo_t *s) { return array_ref<char>{s->objPath}; })
            .add_property("rescName", +[](dataObjInfo_t *s) { return array_ref<char>{s->rescName}; })
            .add_property("rescHier", +[](dataObjInfo_t *s) { return array_ref<char>{s->rescHier}; })
            .add_property("dataType", +[](dataObjInfo_t *s) { return array_ref<char>{s->dataType}; })
            .add_property("dataSize", &dataObjInfo_t::dataSize)
            .add_property("chksum", +[](dataObjInfo_t *s) { return array_ref<char>{s->chksum}; })
            .add_property("version", +[](dataObjInfo_t *s) { return array_ref<char>{s->version}; })
            .add_property("filePath", +[](dataObjInfo_t *s) { return array_ref<char>{s->filePath}; })
            .add_property("dataOwnerName", +[](dataObjInfo_t *s) { return array_ref<char>{s->dataOwnerName}; })
            .add_property("dataOwnerZone", +[](dataObjInfo_t *s) { return array_ref<char>{s->dataOwnerZone}; })
            .add_property("replNum", &dataObjInfo_t::replNum)
            .add_property("replStatus", &dataObjInfo_t::replStatus)
            .add_property("statusString", +[](dataObjInfo_t *s) { return array_ref<char>{s->statusString}; })
            .add_property("dataId", &dataObjInfo_t::dataId)
            .add_property("collId", &dataObjInfo_t::collId)
            .add_property("dataMapId", &dataObjInfo_t::dataMapId)
            .add_property("flags", &dataObjInfo_t::flags)
            .add_property("dataComments", +[](dataObjInfo_t *s) { return array_ref<char>{s->dataComments}; })
            .add_property("dataMode", +[](dataObjInfo_t *s) { return array_ref<char>{s->dataMode}; })
            .add_property("dataExpiry", +[](dataObjInfo_t *s) { return array_ref<char>{s->dataExpiry}; })
            .add_property("dataCreate", +[](dataObjInfo_t *s) { return array_ref<char>{s->dataCreate}; })
            .add_property("dataModify", +[](dataObjInfo_t *s) { return array_ref<char>{s->dataModify}; })
            .add_property("dataAccess", +[](dataObjInfo_t *s) { return array_ref<char>{s->dataAccess}; })
            .add_property("dataAccessInx", &dataObjInfo_t::dataAccessInx)
            .add_property("writeFlag", &dataObjInfo_t::writeFlag)
            .add_property("destRescName", +[](dataObjInfo_t *s) { return array_ref<char>{s->destRescName}; })
            .add_property("backupRescName", +[](dataObjInfo_t *s) { return array_ref<char>{s->backupRescName}; })
            .add_property("subPath", +[](dataObjInfo_t *s) { return array_ref<char>{s->subPath}; })
            .add_property("specColl",
                    bp::make_getter(&dataObjInfo_t::specColl, bp::return_internal_reference<1>{}),
                    bp::make_setter(&dataObjInfo_t::specColl, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("regUid", &dataObjInfo_t::regUid)
            .add_property("otherFlags", &dataObjInfo_t::otherFlags)
            .add_property("condInput", &dataObjInfo_t::condInput)
            .add_property("in_pdmo", +[](dataObjInfo_t *s) { return array_ref<char>{s->in_pdmo}; })
            .add_property("next",
                    bp::make_getter(&dataObjInfo_t::next, bp::return_internal_reference<1>{}),
                    bp::make_setter(&dataObjInfo_t::next, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("rescId", &dataObjInfo_t::rescId)
            ;

        bp::class_<dataObjInp_t>("DataObjInp", bp::no_init)
            .def("__init__", make_init_function<dataObjInp_t>(
                            &dataObjInp_t::objPath,
                            &dataObjInp_t::createMode,
                            &dataObjInp_t::openFlags,
                            &dataObjInp_t::offset,
                            &dataObjInp_t::dataSize,
                            &dataObjInp_t::numThreads,
                            &dataObjInp_t::oprType,
                            &dataObjInp_t::specColl,
                            &dataObjInp_t::condInput))
            .add_property("objPath", +[](dataObjInp_t *s) { return array_ref<char>{s->objPath}; })
            .add_property("createMode", &dataObjInp_t::createMode)
            .add_property("openFlags", &dataObjInp_t::openFlags)
            .add_property("offset", &dataObjInp_t::offset)
            .add_property("dataSize", &dataObjInp_t::dataSize)
            .add_property("numThreads", &dataObjInp_t::numThreads)
            .add_property("oprType", &dataObjInp_t::oprType)
            .add_property("specColl",
                    bp::make_getter(&dataObjInp_t::specColl, bp::return_internal_reference<1>{}),
                    bp::make_setter(&dataObjInp_t::specColl, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("condInput", &dataObjInp_t::condInput)
            ;

        bp::class_<msParam_t>("MsParam", bp::no_init)
            .def("__init__", make_init_function<msParam_t>(
                            &msParam_t::label,
                            &msParam_t::type,
                            &msParam_t::inOutStruct,
                            &msParam_t::inpOutBuf))
            .add_property("label", +[](msParam_t *s) { return array_ref<char, true>{s->label}; })
            .add_property("type", +[](msParam_t *s) { return array_ref<char, true>{s->label}; })
            // Making inOutStruct and inpOutBuf readable in python is beyond scope at the moment
            ;

        bp::class_<msParamArray_t>("MsParamArray", bp::no_init)
            .def("__init__", make_init_function<msParamArray_t>(
                            &msParamArray_t::len,
                            &msParamArray_t::oprType,
                            &msParamArray_t::msParam))
            .add_property("len", &msParamArray_t::len)
            .add_property("oprType", &msParamArray_t::oprType)
            .add_property("msParam", +[](msParamArray_t *s) { return array_ref<msParam_t*>{s->msParam, static_cast<std::size_t>(s->len)}; })
            ;

        //No accessors for thread_context. No one needs this and it would require exposing
        //quite a lot of the internals of boost
        bp::class_<thread_context>("ThreadContext", bp::no_init)
            .def("__init__", make_init_function<thread_context>(
                            &thread_context::reconnThr,
                            &thread_context::lock,
                            &thread_context::cond))
            ;

        bp::class_<perfStat_t>("PerfStat", bp::no_init)
            .def("__init__", make_init_function<perfStat_t>(
                            &perfStat_t::orphanCnt,
                            &perfStat_t::nonOrphanCnt))
            .add_property("orphanCnt", &perfStat_t::orphanCnt)
            .add_property("nonOrphanCnt", &perfStat_t::nonOrphanCnt)
            ;

        bp::class_<portList_t>("PortList", bp::no_init)
            .def("__init__", make_init_function<portList_t>(
                            &portList_t::portNum,
                            &portList_t::cookie,
                            &portList_t::sock,
                            &portList_t::windowSize,
                            &portList_t::hostAddr))
            .add_property("portNum", &portList_t::portNum)
            .add_property("cookie", &portList_t::cookie)
            .add_property("sock", &portList_t::sock)
            .add_property("windowSize", &portList_t::windowSize)
            .add_property("hostAddr", +[](portList_t *s) { return array_ref<char>{s->hostAddr}; })
            ;
/*
        bp::class_<dataOprInp_t>("DataOprInp", bp::no_init)
            .def("__init__", make_init_function<dataOprInp_t>(
                            &dataOprInp_t::oprType,
                            &dataOprInp_t::numThreads,
                            &dataOprInp_t::srcL3descInx,
                            &dataOprInp_t::destL3descInx,
                            &dataOprInp_t::srcRescTypeInx,
                            &dataOprInp_t::destRescTypeInx,
                            &dataOprInp_t::offset,
                            &dataOprInp_t::dataSize,
                            &dataOprInp_t::condInput))
            .add_property("oprType", &dataOprInp_t::oprType)
            .add_property("numThreads", &dataOprInp_t::numThreads)
            .add_property("srcL3descInx", &dataOprInp_t::srcL3descInx)
            .add_property("destL3descInx", &dataOprInp_t::destL3descInx)
            .add_property("srcRescTypeInx", &dataOprInp_t::srcRescTypeInx)
            .add_property("destRescTypeInx", &dataOprInp_t::destRescTypeInx)
            .add_property("offset", &dataOprInp_t::offset)
            .add_property("dataSize", &dataOprInp_t::dataSize)
            .add_property("condInput", &dataOprInp_t::condInput)
            ;
*/
        bp::class_<portalOpr_t>("PortalOpr", bp::no_init)
            .def("__init__", make_init_function<portalOpr_t>(
                            &portalOpr_t::oprType,
                            &portalOpr_t::dataOprInp,
                            &portalOpr_t::portList,
                            &portalOpr_t::shared_secret))
            .add_property("oprType", &portalOpr_t::oprType)
            .add_property("dataOprInp", &portalOpr_t::dataOprInp)
            .add_property("portList", &portalOpr_t::portList)
            .add_property("shared_secret", +[](portalOpr_t *s) { return array_ref<char>{s->shared_secret}; })
            ;

        bp::class_<rErrMsg_t>("RErrMsg", bp::no_init)
            .def("__init__", make_init_function<rErrMsg_t>(
                            &rErrMsg_t::status,
                            &rErrMsg_t::msg))
            .add_property("status", &rErrMsg_t::status)
            .add_property("msg", +[](rErrMsg_t *s) { return array_ref<char>{s->msg}; })
            ;

        bp::class_<rError_t>("RError", bp::no_init)
            .def("__init__", make_init_function<rError_t>(
                            &rError_t::len,
                            &rError_t::errMsg))
            .add_property("len", &rError_t::len)
            .add_property("errMsg", +[](rError_t *s) { return array_ref<rErrMsg_t*>{s->errMsg, static_cast<std::size_t>(s->len)}; })
            ;

        bp::class_<version_t>("Version", bp::no_init)
            .def("__init__", make_init_function<version_t>(
                            &version_t::status,
                            &version_t::relVersion,
                            &version_t::apiVersion,
                            &version_t::reconnPort,
                            &version_t::reconnAddr,
                            &version_t::cookie))
            .add_property("status", &version_t::status)
            .add_property("relVersion", +[](version_t *s) { return array_ref<char>{s->relVersion}; })
            .add_property("apiVersion", +[](version_t *s) { return array_ref<char>{s->apiVersion}; })
            .add_property("reconnPort", &version_t::reconnPort)
            .add_property("reconnAddr", +[](version_t *s) { return array_ref<char>{s->reconnAddr}; })
            .add_property("cookie", &version_t::cookie)
            ;

        bp::class_<in_addr>("In_Addr", bp::no_init)
            .def("__init__", make_init_function<in_addr>(
                            &in_addr::s_addr))
            .add_property("s_addr", &in_addr::s_addr)
            ;

        bp::class_<sockaddr_in>("Sockaddr_In", bp::no_init)
            .def("__init__", make_init_function<sockaddr_in>(
                            &sockaddr_in::sin_family,
                            &sockaddr_in::sin_port,
                            &sockaddr_in::sin_addr))
            .add_property("sin_family", &sockaddr_in::sin_family)
            .add_property("sin_port", &sockaddr_in::sin_port)
            .add_property("sin_addr", &sockaddr_in::sin_addr)
            ;

        bp::class_<authInfo_t>("AuthInfo", bp::no_init)
            .def("__init__", make_init_function<authInfo_t>(
                            &authInfo_t::authScheme,
                            &authInfo_t::authFlag,
                            &authInfo_t::flag,
                            &authInfo_t::ppid,
                            &authInfo_t::host,
                            &authInfo_t::authStr))
            .add_property("authScheme", +[](authInfo_t *s) { return array_ref<char>{s->authScheme}; })
            .add_property("authFlag", &authInfo_t::authFlag)
            .add_property("flag", &authInfo_t::flag)
            .add_property("ppid", &authInfo_t::ppid)
            .add_property("host", +[](authInfo_t *s) { return array_ref<char>{s->host}; })
            .add_property("authStr", +[](authInfo_t *s) { return array_ref<char>{s->authStr}; })
            ;

        bp::class_<userOtherInfo_t>("UserOtherInfo", bp::no_init)
            .def("__init__", make_init_function<userOtherInfo_t>(
                            &userOtherInfo_t::userInfo,
                            &userOtherInfo_t::userComments,
                            &userOtherInfo_t::userCreate,
                            &userOtherInfo_t::userModify))
            .add_property("userInfo", +[](userOtherInfo_t *s) { return array_ref<char>{s->userInfo}; })
            .add_property("userComments", +[](userOtherInfo_t *s) { return array_ref<char>{s->userComments}; })
            .add_property("userCreate", +[](userOtherInfo_t *s) { return array_ref<char>{s->userCreate}; })
            .add_property("userModify", +[](userOtherInfo_t *s) { return array_ref<char>{s->userModify}; })
            ;

        bp::class_<userInfo_t>("UserInfo", bp::no_init)
            .def("__init__", make_init_function<userInfo_t>(
                            &userInfo_t::userName,
                            &userInfo_t::rodsZone,
                            &userInfo_t::userType,
                            &userInfo_t::sysUid,
                            &userInfo_t::authInfo,
                            &userInfo_t::userOtherInfo))
            .add_property("userName", +[](userInfo_t *s) { return array_ref<char>{s->userName}; })
            .add_property("rodsZone", +[](userInfo_t *s) { return array_ref<char>{s->rodsZone}; })
            .add_property("userType", +[](userInfo_t *s) { return array_ref<char>{s->userType}; })
            .add_property("sysUid", &userInfo_t::sysUid)
            .add_property("authInfo", &userInfo_t::authInfo)
            .add_property("userOtherInfo", &userInfo_t::userOtherInfo)
            ;

        bp::class_<rodsEnv>("RodsEnv", bp::no_init)
            .def("__init__", make_init_function<rodsEnv>(
                            &rodsEnv::rodsUserName,
                            &rodsEnv::rodsHost,
                            &rodsEnv::rodsPort,
                            &rodsEnv::xmsgHost,
                            &rodsEnv::xmsgPort,
                            &rodsEnv::rodsHome,
                            &rodsEnv::rodsCwd,
                            &rodsEnv::rodsAuthScheme,
                            &rodsEnv::rodsDefResource,
                            &rodsEnv::rodsZone,
                            &rodsEnv::rodsLogLevel,
                            &rodsEnv::rodsAuthFile,
                            &rodsEnv::rodsDebug,
                            &rodsEnv::rodsClientServerPolicy,
                            &rodsEnv::rodsClientServerNegotiation,
                            &rodsEnv::rodsEncryptionKeySize,
                            &rodsEnv::rodsEncryptionSaltSize,
                            &rodsEnv::rodsEncryptionNumHashRounds,
                            &rodsEnv::rodsEncryptionAlgorithm,
                            &rodsEnv::rodsDefaultHashScheme,
                            &rodsEnv::rodsMatchHashPolicy,
                            &rodsEnv::irodsSSLCACertificatePath,
                            &rodsEnv::irodsSSLCACertificateFile,
                            &rodsEnv::irodsSSLVerifyServer,
                            &rodsEnv::irodsSSLCertificateChainFile,
                            &rodsEnv::irodsSSLCertificateKeyFile,
                            &rodsEnv::irodsSSLDHParamsFile,
                            &rodsEnv::irodsCtrlPlaneKey,
                            &rodsEnv::irodsCtrlPlanePort,
                            &rodsEnv::irodsCtrlPlaneEncryptionNumHashRounds,
                            &rodsEnv::irodsCtrlPlaneEncryptionAlgorithm,
                            &rodsEnv::irodsMaxSizeForSingleBuffer,
                            &rodsEnv::irodsDefaultNumberTransferThreads,
                            &rodsEnv::irodsTransBufferSizeForParaTrans,
                            &rodsEnv::irodsPluginHome))
            .add_property("rodsUserName", +[](rodsEnv *s) { return array_ref<char>{s->rodsUserName}; })
            .add_property("rodsHost", +[](rodsEnv *s) { return array_ref<char>{s->rodsHost}; })
            .add_property("rodsPort", &rodsEnv::rodsPort)
            .add_property("xmsgHost", +[](rodsEnv *s) { return array_ref<char>{s->xmsgHost}; })
            .add_property("xmsgPort", &rodsEnv::xmsgPort)
            .add_property("rodsHome", +[](rodsEnv *s) { return array_ref<char>{s->rodsHome}; })
            .add_property("rodsCwd", +[](rodsEnv *s) { return array_ref<char>{s->rodsCwd}; })
            .add_property("rodsAuthScheme", +[](rodsEnv *s) { return array_ref<char>{s->rodsAuthScheme}; })
            .add_property("rodsDefResource", +[](rodsEnv *s) { return array_ref<char>{s->rodsDefResource}; })
            .add_property("rodsZone", +[](rodsEnv *s) { return array_ref<char>{s->rodsZone}; })
            .add_property("rodsLogLevel", &rodsEnv::rodsLogLevel)
            .add_property("rodsAuthFile", +[](rodsEnv *s) { return array_ref<char>{s->rodsAuthFile}; })
            .add_property("rodsDebug", +[](rodsEnv *s) { return array_ref<char>{s->rodsDebug}; })
            .add_property("rodsClientServerPolicy", +[](rodsEnv *s) { return array_ref<char>{s->rodsClientServerPolicy}; })
            .add_property("rodsClientServerNegotiation", +[](rodsEnv *s) { return array_ref<char>{s->rodsClientServerNegotiation}; })
            .add_property("rodsEncryptionKeySize", &rodsEnv::rodsEncryptionKeySize)
            .add_property("rodsEncryptionSaltSize", &rodsEnv::rodsEncryptionSaltSize)
            .add_property("rodsEncryptionNumHashRounds", &rodsEnv::rodsEncryptionNumHashRounds)
            .add_property("rodsEncryptionAlgorithm", +[](rodsEnv *s) { return array_ref<char>{s->rodsEncryptionAlgorithm}; })
            .add_property("rodsDefaultHashScheme", +[](rodsEnv *s) { return array_ref<char>{s->rodsDefaultHashScheme}; })
            .add_property("rodsMatchHashPolicy", +[](rodsEnv *s) { return array_ref<char>{s->rodsMatchHashPolicy}; })
            .add_property("irodsSSLCACertificatePath", +[](rodsEnv *s) { return array_ref<char>{s->irodsSSLCACertificatePath}; })
            .add_property("irodsSSLCACertificateFile", +[](rodsEnv *s) { return array_ref<char>{s->irodsSSLCACertificateFile}; })
            .add_property("irodsSSLVerifyServer", +[](rodsEnv *s) { return array_ref<char>{s->irodsSSLVerifyServer}; })
            .add_property("irodsSSLCertificateChainFile", +[](rodsEnv *s) { return array_ref<char>{s->irodsSSLCertificateChainFile}; })
            .add_property("irodsSSLCertificateKeyFile", +[](rodsEnv *s) { return array_ref<char>{s->irodsSSLCertificateKeyFile}; })
            .add_property("irodsSSLDHParamsFile", +[](rodsEnv *s) { return array_ref<char>{s->irodsSSLDHParamsFile}; })
            .add_property("irodsCtrlPlaneKey", +[](rodsEnv *s) { return array_ref<char>{s->irodsCtrlPlaneKey}; })
            .add_property("irodsCtrlPlanePort", &rodsEnv::irodsCtrlPlanePort)
            .add_property("irodsCtrlPlaneEncryptionNumHashRounds", &rodsEnv::irodsCtrlPlaneEncryptionNumHashRounds)
            .add_property("irodsCtrlPlaneEncryptionAlgorithm", +[](rodsEnv *s) { return array_ref<char>{s->irodsCtrlPlaneEncryptionAlgorithm}; })
            .add_property("irodsMaxSizeForSingleBuffer", &rodsEnv::irodsMaxSizeForSingleBuffer)
            .add_property("irodsDefaultNumberTransferThreads", &rodsEnv::irodsDefaultNumberTransferThreads)
            .add_property("irodsTransBufferSizeForParaTrans", &rodsEnv::irodsTransBufferSizeForParaTrans)
            .add_property("irodsPluginHome", +[](rodsEnv *s) { return array_ref<char>{s->irodsPluginHome}; })
            ;

        bp::class_<rsComm_t>("RsComm", bp::no_init)
            .def("__init__", make_init_function<rsComm_t>(
                            &rsComm_t::irodsProt,
                            &rsComm_t::sock,
                            &rsComm_t::connectCnt,
                            &rsComm_t::localAddr,
                            &rsComm_t::remoteAddr,
                            &rsComm_t::clientAddr,
                            &rsComm_t::proxyUser,
                            &rsComm_t::clientUser,
                            &rsComm_t::myEnv,
                            &rsComm_t::cliVersion,
                            &rsComm_t::option,
                            &rsComm_t::procLogFlag,
                            &rsComm_t::rError,
                            &rsComm_t::portalOpr,
                            &rsComm_t::apiInx,
                            &rsComm_t::status,
                            &rsComm_t::perfStat,
                            &rsComm_t::windowSize,
                            &rsComm_t::reconnFlag,
                            &rsComm_t::reconnSock,
                            &rsComm_t::reconnPort,
                            &rsComm_t::reconnectedSock,
                            &rsComm_t::reconnAddr,
                            &rsComm_t::cookie,
                            &rsComm_t::thread_ctx,
                            &rsComm_t::agentState,
                            &rsComm_t::clientState,
                            &rsComm_t::reconnThrState,
                            &rsComm_t::gsiRequest,
                            &rsComm_t::auth_scheme,
                            &rsComm_t::ssl_on,
                            &rsComm_t::ssl_ctx,
                            &rsComm_t::ssl,
                            &rsComm_t::ssl_do_accept,
                            &rsComm_t::ssl_do_shutdown,
                            &rsComm_t::negotiation_results,
                            &rsComm_t::shared_secret,
                            &rsComm_t::key_size,
                            &rsComm_t::salt_size,
                            &rsComm_t::num_hash_rounds,
                            &rsComm_t::encryption_algorithm))
            .add_property("irodsProt", &rsComm_t::irodsProt)
            .add_property("sock", &rsComm_t::sock)
            .add_property("connectCnt", &rsComm_t::connectCnt)
            .add_property("localAddr", &rsComm_t::localAddr)
            .add_property("remoteAddr", &rsComm_t::remoteAddr)
            .add_property("clientAddr", +[](rsComm_t *s) { return array_ref<char>{s->clientAddr}; })
            .add_property("proxyUser", &rsComm_t::proxyUser)
            .add_property("clientUser", &rsComm_t::clientUser)
            .add_property("myEnv", &rsComm_t::myEnv)
            .add_property("cliVersion", &rsComm_t::cliVersion)
            .add_property("option", +[](rsComm_t *s) { return array_ref<char>{s->option}; })
            .add_property("procLogFlag", &rsComm_t::procLogFlag)
            .add_property("rError", &rsComm_t::rError)
            .add_property("portalOpr",
                    bp::make_getter(&rsComm_t::portalOpr, bp::return_internal_reference<1>{}),
                    bp::make_setter(&rsComm_t::portalOpr, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("apiInx", &rsComm_t::apiInx)
            .add_property("status", &rsComm_t::status)
            .add_property("perfStat", &rsComm_t::perfStat)
            .add_property("windowSize", &rsComm_t::windowSize)
            .add_property("reconnFlag", &rsComm_t::reconnFlag)
            .add_property("reconnSock", &rsComm_t::reconnSock)
            .add_property("reconnPort", &rsComm_t::reconnPort)
            .add_property("reconnectedSock", &rsComm_t::reconnectedSock)
            .add_property("reconnAddr", +[](rsComm_t *s) { return array_ref<char>{s->clientAddr}; })
            .add_property("cookie", &rsComm_t::cookie)
            .add_property("thread_ctx",
                    bp::make_getter(&rsComm_t::thread_ctx, bp::return_internal_reference<1>{}),
                    bp::make_setter(&rsComm_t::thread_ctx, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("agentState", &rsComm_t::agentState)
            .add_property("clientState", &rsComm_t::clientState)
            .add_property("reconnThrState", &rsComm_t::reconnThrState)
            .add_property("gsiRequest", &rsComm_t::gsiRequest)
            .add_property("auth_scheme", +[](rsComm_t *s) { return array_ref<char, true>{s->auth_scheme}; })
            .add_property("ssl_on", &rsComm_t::ssl_on)
            // No accessors for the ssl types, as this would require exposing much of the internals of ssl
            // .add_property("ssl_ctx",
            //         bp::make_getter(&rsComm_t::ssl_ctx, bp::return_internal_reference<1>{}),
            //         bp::make_setter(&rsComm_t::ssl_ctx, bp::with_custodian_and_ward<1, 2>{}))
            // .add_property("ssl",
            //         bp::make_getter(&rsComm_t::ssl, bp::return_internal_reference<1>{}),
            //         bp::make_setter(&rsComm_t::ssl, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("ssl_do_accept", &rsComm_t::ssl_on)
            .add_property("ssl_do_shutdown", &rsComm_t::ssl_on)
            .add_property("negotiation_results", +[](rsComm_t *s) { return array_ref<char>{s->negotiation_results}; })
            .add_property("shared_secret", +[](rsComm_t *s) { return array_ref<unsigned char>{s->shared_secret}; })
            .add_property("key_size", &rsComm_t::key_size)
            .add_property("salt_size", &rsComm_t::salt_size)
            .add_property("num_hash_rounds", &rsComm_t::num_hash_rounds)
            .add_property("encryption_algorithm", +[](rsComm_t *s) { return array_ref<char>{s->encryption_algorithm}; })
            ;

        bp::class_<operProgress_t>("OperProgress", bp::no_init)
            .def("__init__", make_init_function<operProgress_t>(
                            &operProgress_t::oprType,
                            &operProgress_t::flag,
                            &operProgress_t::totalNumFiles,
                            &operProgress_t::totalFileSize,
                            &operProgress_t::totalNumFilesDone,
                            &operProgress_t::totalFileSizeDone,
                            &operProgress_t::curFileName,
                            &operProgress_t::curFileSize,
                            &operProgress_t::curFileSizeDone))
            .add_property("oprType", &operProgress_t::oprType)
            .add_property("flag", &operProgress_t::flag)
            .add_property("totalNumFiles", &operProgress_t::totalNumFiles)
            .add_property("totalFileSize", &operProgress_t::totalFileSize)
            .add_property("totalNumFilesDone", &operProgress_t::totalNumFilesDone)
            .add_property("totalFileSizeDone", &operProgress_t::totalFileSizeDone)
            .add_property("curFileName", +[](operProgress_t *s) { return array_ref<char>{s->curFileName}; })
            .add_property("curFileSize", &operProgress_t::curFileSize)
            .add_property("curFileSizeDone", &operProgress_t::curFileSizeDone)
            ;

        bp::class_<transferStat_t>("TransferStat", bp::no_init)
            .def("__init__", make_init_function<transferStat_t>(
                            &transferStat_t::numThreads,
                            &transferStat_t::flags,
                            &transferStat_t::bytesWritten))
            .add_property("len", &transferStat_t::numThreads)
            .add_property("len", &transferStat_t::flags)
            .add_property("offset", &transferStat_t::bytesWritten)
            ;

        bp::class_<dataSeg_t>("DataSeg", bp::no_init)
            .def("__init__", make_init_function<dataSeg_t>(
                            &dataSeg_t::len,
                            &dataSeg_t::offset))
            .add_property("len", &dataSeg_t::len)
            .add_property("offset", &dataSeg_t::offset)
            ;

        bp::class_<fileRestartInfo_t>("FileRestartInfo", bp::no_init)
            .def("__init__", make_init_function<fileRestartInfo_t>(
                            &fileRestartInfo_t::fileName,
                            &fileRestartInfo_t::objPath,
                            &fileRestartInfo_t::numSeg,
                            &fileRestartInfo_t::status,
                            &fileRestartInfo_t::fileSize,
                            &fileRestartInfo_t::dataSeg))
            .add_property("fileName", +[](fileRestartInfo_t *s) { return array_ref<char>{s->fileName}; })
            .add_property("objPath", +[](fileRestartInfo_t *s) { return array_ref<char>{s->objPath}; })
            .add_property("numSeg", &fileRestartInfo_t::numSeg)
            .add_property("status", &fileRestartInfo_t::status)
            .add_property("fileSize", &fileRestartInfo_t::fileSize)
            .add_property("dataSeg", +[](fileRestartInfo_t *s) { return array_ref<dataSeg_t>{s->dataSeg}; })
            ;

        bp::class_<fileRestart_t>("FileRestart", bp::no_init)
            .def("__init__", make_init_function<fileRestart_t>(
                            &fileRestart_t::flags,
                            &fileRestart_t::writtenSinceUpdated,
                            &fileRestart_t::infoFile,
                            &fileRestart_t::info))
            .add_property("flags", &fileRestart_t::flags)
            .add_property("writtenSinceUpdated", &fileRestart_t::writtenSinceUpdated)
            .add_property("infoFile", +[](fileRestart_t *s) { return array_ref<char>{s->infoFile}; })
            .add_property("info", &fileRestart_t::info)
            ;

        bp::class_<rcComm_t>("RcComm", bp::no_init)
            .def("__init__", make_init_function<rcComm_t>(
                            &rcComm_t::irodsProt,
                            &rcComm_t::host,
                            &rcComm_t::sock,
                            &rcComm_t::portNum,
                            &rcComm_t::loggedIn,
                            &rcComm_t::localAddr,
                            &rcComm_t::remoteAddr,
                            &rcComm_t::proxyUser,
                            &rcComm_t::clientUser,
                            &rcComm_t::svrVersion,
                            &rcComm_t::rError,
                            &rcComm_t::flag,
                            &rcComm_t::transStat,
                            &rcComm_t::apiInx,
                            &rcComm_t::status,
                            &rcComm_t::windowSize,
                            &rcComm_t::reconnectedSock,
                            &rcComm_t::reconnTime,
                            &rcComm_t::exit_flg,
                            &rcComm_t::thread_ctx,
                            &rcComm_t::agentState,
                            &rcComm_t::clientState,
                            &rcComm_t::reconnThrState,
                            &rcComm_t::operProgress,
                            &rcComm_t::key_size,
                            &rcComm_t::salt_size,
                            &rcComm_t::num_hash_rounds,
                            &rcComm_t::encryption_algorithm,
                            &rcComm_t::negotiation_results,
                            &rcComm_t::shared_secret,
                            &rcComm_t::ssl_on,
                            &rcComm_t::ssl_ctx,
                            &rcComm_t::ssl,
                            &rcComm_t::fileRestart))
            .add_property("irodsProt", &rcComm_t::irodsProt)
            .add_property("host", +[](rcComm_t *s) { return array_ref<char>{s->host}; })
            .add_property("sock", &rcComm_t::sock)
            .add_property("portNum", &rcComm_t::portNum)
            .add_property("loggedIn", &rcComm_t::loggedIn)
            .add_property("localAddr", &rcComm_t::localAddr)
            .add_property("remoteAddr", &rcComm_t::remoteAddr)
            .add_property("proxyUser", &rcComm_t::proxyUser)
            .add_property("clientUser", &rcComm_t::clientUser)
            .add_property("svrVersion",
                    bp::make_getter(&rcComm_t::svrVersion, bp::return_internal_reference<1>{}),
                    bp::make_setter(&rcComm_t::svrVersion, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("rError",
                    bp::make_getter(&rcComm_t::rError, bp::return_internal_reference<1>{}),
                    bp::make_setter(&rcComm_t::rError, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("flag", &rcComm_t::flag)
            .add_property("transStat", &rcComm_t::transStat)
            .add_property("apiInx", &rcComm_t::apiInx)
            .add_property("status", &rcComm_t::status)
            .add_property("windowSize", &rcComm_t::windowSize)
            .add_property("reconnectedSock", &rcComm_t::reconnectedSock)
            .add_property("reconnTime", &rcComm_t::reconnTime)
            .add_property("exit_flg", &rcComm_t::exit_flg)
            .add_property("thread_ctx",
                    bp::make_getter(&rcComm_t::thread_ctx, bp::return_internal_reference<1>{}),
                    bp::make_setter(&rcComm_t::thread_ctx, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("agentState", &rcComm_t::agentState)
            .add_property("clientState", &rcComm_t::clientState)
            .add_property("reconnThrState", &rcComm_t::reconnThrState)
            .add_property("operProgress", &rcComm_t::operProgress)
            .add_property("key_size", &rcComm_t::key_size)
            .add_property("salt_size", &rcComm_t::salt_size)
            .add_property("num_hash_rounds", &rcComm_t::num_hash_rounds)
            .add_property("encryption_algorithm", +[](rcComm_t *s) { return array_ref<char>{s->encryption_algorithm}; })
            .add_property("negotiation_results", +[](rcComm_t *s) { return array_ref<char>{s->negotiation_results}; })
            .add_property("shared_secret", +[](rcComm_t *s) { return array_ref<unsigned char>{s->shared_secret}; })
            .add_property("ssl_on", &rcComm_t::ssl_on)
            // No accessors for the ssl types, as this would require exposing much of the internals of ssl
            // .add_property("ssl_ctx",
            //         bp::make_getter(&rcComm_t::ssl_ctx, bp::return_internal_reference<1>{}),
            //         bp::make_setter(&rcComm_t::ssl_ctx, bp::with_custodian_and_ward<1, 2>{}))
            // .add_property("ssl",
            //         bp::make_getter(&rcComm_t::ssl, bp::return_internal_reference<1>{}),
            //         bp::make_setter(&rcComm_t::ssl, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("fileRestart", &rcComm_t::fileRestart)
            ;

        bp::class_<ruleExecInfo_t>("RuleExecInfo", bp::no_init)
            .def("__init__", make_init_function<ruleExecInfo_t>(
                            &ruleExecInfo_t::status,
                            &ruleExecInfo_t::statusStr,
                            &ruleExecInfo_t::ruleName,
                            &ruleExecInfo_t::rsComm,
                            &ruleExecInfo_t::pluginInstanceName,
                            &ruleExecInfo_t::msParamArray,
                            &ruleExecInfo_t::inOutMsParamArray,
                            &ruleExecInfo_t::l1descInx,
                            &ruleExecInfo_t::doinp,
                            &ruleExecInfo_t::doi,
                            &ruleExecInfo_t::rescName,
                            &ruleExecInfo_t::uoic,
                            &ruleExecInfo_t::uoip,
                            &ruleExecInfo_t::coi,
                            &ruleExecInfo_t::uoio,
                            &ruleExecInfo_t::condInputData,
                            &ruleExecInfo_t::ruleSet,
                            &ruleExecInfo_t::next))
            .add_property("status", &ruleExecInfo_t::status)
            .add_property("statusStr", +[](ruleExecInfo_t *s) { return array_ref<char>{s->statusStr}; })
            .add_property("ruleName", +[](ruleExecInfo_t *s) { return array_ref<char>{s->ruleName}; })
            .add_property("rsComm",
                    bp::make_getter(&ruleExecInfo_t::rsComm, bp::return_internal_reference<1>{}),
                    bp::make_setter(&ruleExecInfo_t::rsComm, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("pluginInstanceName", +[](ruleExecInfo_t *s) { return array_ref<char>{s->pluginInstanceName}; })
            .add_property("msParamArray",
                    bp::make_getter(&ruleExecInfo_t::msParamArray, bp::return_internal_reference<1>{}),
                    bp::make_setter(&ruleExecInfo_t::msParamArray, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("inOutMsParamArray", &ruleExecInfo_t::inOutMsParamArray)
            .add_property("l1descInx", &ruleExecInfo_t::l1descInx)
            .add_property("doinp",
                    bp::make_getter(&ruleExecInfo_t::doinp, bp::return_internal_reference<1>{}),
                    bp::make_setter(&ruleExecInfo_t::doinp, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("doi",
                    bp::make_getter(&ruleExecInfo_t::doi, bp::return_internal_reference<1>{}),
                    bp::make_setter(&ruleExecInfo_t::doi, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("rescName", +[](ruleExecInfo_t *s) { return array_ref<char>{s->rescName}; })
            .add_property("uoic",
                    bp::make_getter(&ruleExecInfo_t::uoic, bp::return_internal_reference<1>{}),
                    bp::make_setter(&ruleExecInfo_t::uoic, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("uoip",
                    bp::make_getter(&ruleExecInfo_t::uoip, bp::return_internal_reference<1>{}),
                    bp::make_setter(&ruleExecInfo_t::uoip, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("coi",
                    bp::make_getter(&ruleExecInfo_t::coi, bp::return_internal_reference<1>{}),
                    bp::make_setter(&ruleExecInfo_t::coi, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("uoio",
                    bp::make_getter(&ruleExecInfo_t::uoio, bp::return_internal_reference<1>{}),
                    bp::make_setter(&ruleExecInfo_t::uoio, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("condInputData",
                    bp::make_getter(&ruleExecInfo_t::condInputData, bp::return_internal_reference<1>{}),
                    bp::make_setter(&ruleExecInfo_t::condInputData, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("ruleSet", +[](ruleExecInfo_t *s) { return array_ref<char>{s->ruleSet}; })
            .add_property("next",
                    bp::make_getter(&ruleExecInfo_t::next, bp::return_internal_reference<1>{}),
                    bp::make_setter(&ruleExecInfo_t::next, bp::with_custodian_and_ward<1, 2>{}))
            ;

        bp::class_<irods::plugin_context>("PluginContext", bp::no_init)
            //prop_map contains boost::any, don't directly support that in python
            //.add_property("__getitem__", +[](irods::plugin_context* _ctx, const std::string& key) { return _ctx.prop_map().get(key) }
            //.add_property("__setitem__", +[](irods::plugin_context* _ctx, const std::string& key) { return _ctx.prop_map().set(key) }
            .add_property("rule_results", +[](irods::plugin_context* _ctx) { return _ctx->rule_results(); } )
            .add_property("__getitem__", +[](irods::plugin_context* _ctx, const std::string& key) { irods::rule_engine_vars_t map; _ctx->fco()->get_re_vars(map); return map[key]; })
            ;

        bp::class_<irods::hierarchy_parser>("HierarchyParser", bp::no_init)
            .add_property("str", +[](irods::hierarchy_parser* _parser) { std::string ret_string; _parser->str(ret_string, ""); return ret_string; } )
            ;

        //bp::class_<void*>("VoidPointer", bp::no_init)
            //;

        bp::class_<struct stat>("Stat", bp::no_init)
            ;

        bp::class_<bytesBuf_t>("BytesBuf", bp::no_init)
            .def("__init__", make_init_function<bytesBuf_t>(
                        &bytesBuf_t::len,
                        &bytesBuf_t::buf))
            .add_property("len", &bytesBuf_t::len)
            .add_property("buf", +[](bytesBuf_t *s) { return s->buf ? bp::object{array_ref<char>{static_cast<char*>(s->buf), static_cast<std::size_t>(s->len)}} : bp::object{}; })
            ;

        bp::class_<ruleExecSubmitInp_t>("RuleExecSubmitInp", bp::no_init)
            .def("__init__", make_init_function<ruleExecSubmitInp_t>(
                        &ruleExecSubmitInp_t::ruleName,
                        &ruleExecSubmitInp_t::reiFilePath,
                        &ruleExecSubmitInp_t::userName,
                        &ruleExecSubmitInp_t::exeAddress,
                        &ruleExecSubmitInp_t::exeTime,
                        &ruleExecSubmitInp_t::exeFrequency,
                        &ruleExecSubmitInp_t::priority,
                        &ruleExecSubmitInp_t::lastExecTime,
                        &ruleExecSubmitInp_t::exeStatus,
                        &ruleExecSubmitInp_t::estimateExeTime,
                        &ruleExecSubmitInp_t::notificationAddr,
                        &ruleExecSubmitInp_t::condInput,
                        &ruleExecSubmitInp_t::packedReiAndArgBBuf,
                        &ruleExecSubmitInp_t::ruleExecId))
            .add_property("ruleName", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->ruleName}; })
            .add_property("reiFilePath", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->reiFilePath}; })
            .add_property("userName", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->userName}; })
            .add_property("exeAddress", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->exeAddress}; })
            .add_property("exeTime", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->exeTime}; })
            .add_property("exeFrequency", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->exeFrequency}; })
            .add_property("priority", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->priority}; })
            .add_property("lastExecTime", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->lastExecTime}; })
            .add_property("exeStatus", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->exeStatus}; })
            .add_property("estimateExeTime", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->estimateExeTime}; })
            .add_property("notificationAddr", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->notificationAddr}; })
            .add_property("condInput", &ruleExecSubmitInp_t::condInput)
            .add_property("packedReiAndArgBBuf",
                    bp::make_getter(&ruleExecSubmitInp_t::packedReiAndArgBBuf, bp::return_internal_reference<1>{}),
                    bp::make_setter(&ruleExecSubmitInp_t::packedReiAndArgBBuf, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("ruleExecId", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->ruleExecId}; })
            ;

        bp::class_<rodsDirent_t>("RodsDirent", bp::no_init)
            .def("__init__", make_init_function<rodsDirent_t>(
                            &rodsDirent_t::d_offset,
                            &rodsDirent_t::d_ino,
                            &rodsDirent_t::d_reclen,
                            &rodsDirent_t::d_namlen,
                            &rodsDirent_t::d_name))
            .add_property("d_offset", &rodsDirent_t::d_offset)
            .add_property("d_ino", &rodsDirent_t::d_ino)
            .add_property("d_reclen", &rodsDirent_t::d_reclen)
            .add_property("d_namlen", &rodsDirent_t::d_namlen)
            .add_property("d_name", +[](rodsDirent_t *s) { return array_ref<char>{s->d_name}; })
            ;

        bp::class_<std::map<std::string, std::string>>("StringToStringMap")
            .add_property("__getitem__", +[](std::map<std::string, std::string>* m, const std::string& key) { return (*m)[key]; })
            ;

        boost::python::class_<std::vector<int>>("IntVector")
            .def(bp::vector_indexing_suite<std::vector<int>>{});
        ;

        bp::class_<specificQueryInp_t>("SpecificQueryInp", bp::no_init)
            .def("__init__", make_init_function<specificQueryInp_t>(
                            &specificQueryInp_t::sql,
                            &specificQueryInp_t::args,
                            &specificQueryInp_t::maxRows,
                            &specificQueryInp_t::continueInx,
                            &specificQueryInp_t::rowOffset,
                            &specificQueryInp_t::options,
                            &specificQueryInp_t::condInput))
            .add_property("sql", +[](specificQueryInp_t *s) { return array_ref<char, true>{s->sql}; })
            .add_property("args", +[](specificQueryInp_t *s) { return array_ref<array_ref<char, true>>{s->args}; })
            .add_property("maxRows", &specificQueryInp_t::maxRows)
            .add_property("continueInx", &specificQueryInp_t::continueInx)
            .add_property("rowOffset", &specificQueryInp_t::rowOffset)
            .add_property("options", &specificQueryInp_t::options)
            .add_property("condInput", &specificQueryInp_t::condInput)
            ;

        bp::class_<icatSessionStruct>("IcatSessionStruct", bp::no_init)
        ;

        bp::class_<generalUpdateInp_t>("GeneralUpdateInp", bp::no_init)
            .def("__init__", make_init_function<generalUpdateInp_t>(
                            &generalUpdateInp_t::type,
                            &generalUpdateInp_t::values))
            .add_property("type", &generalUpdateInp_t::type)
            .add_property("values", &generalUpdateInp_t::values)
            ;

        bp::class_<struct timeval>("Timeval", bp::no_init)
            .add_property("tv_sec", &timeval::tv_sec)
            .add_property("tv_usec", &timeval::tv_usec)
            ;

        bp::class_<msgHeader_t>("MsgHeader", bp::no_init)
            .def("__init__", make_init_function<msgHeader_t>(
                            &msgHeader_t::type,
                            &msgHeader_t::msgLen,
                            &msgHeader_t::errorLen,
                            &msgHeader_t::bsLen,
                            &msgHeader_t::intInfo))
            .add_property("type", +[](msgHeader_t *s) { return array_ref<char>{s->type}; })
            .add_property("msgLen", &msgHeader_t::msgLen)
            .add_property("errorLen", &msgHeader_t::errorLen)
            .add_property("bsLen", &msgHeader_t::bsLen)
            .add_property("intInfo", &msgHeader_t::intInfo)
            ;

        bp::class_<bulkOprInp_t>("BulkOprInp", bp::no_init)
            .def("__init__", make_init_function<bulkOprInp_t>(
                            &bulkOprInp_t::objPath,
                            &bulkOprInp_t::attriArray,
                            &bulkOprInp_t::condInput))
            .add_property("objPath", +[](bulkOprInp_t *s) { return array_ref<char>{s->objPath}; })
            .add_property("attriArray", &bulkOprInp_t::attriArray)
            .add_property("condInput", &bulkOprInp_t::condInput)
            ;

        bp::class_<collInp_t>("CollInp", bp::no_init)
            .def("__init__", make_init_function<collInp_t>(
                            &collInp_t::collName,
                            &collInp_t::flags,
                            &collInp_t::oprType,
                            &collInp_t::condInput))
            .add_property("collName", +[](collInp_t *s) { return array_ref<char>{s->collName}; })
            .add_property("flags", &collInp_t::flags)
            .add_property("oprType", &collInp_t::oprType)
            .add_property("condInput", &collInp_t::condInput)
        ;

        bp::class_<collOprStat_t>("CollOprStat", bp::no_init)
            .def("__init__", make_init_function<collOprStat_t>(
                        &collOprStat_t::filesCnt,
                        &collOprStat_t::totalFileCnt,
                        &collOprStat_t::bytesWritten,
                        &collOprStat_t::lastObjPath))
            .add_property("filesCnt", &collOprStat_t::filesCnt)
            .add_property("totalFileCnt", &collOprStat_t::totalFileCnt)
            .add_property("bytesWritten", &collOprStat_t::bytesWritten)
            .add_property("lastObjPath", +[](collOprStat_t *s) { return array_ref<char>{s->lastObjPath}; })
        ;

        bp::class_<dataCopyInp_t>("DataCopyInp", bp::no_init)
            .def("__init__", make_init_function<dataCopyInp_t>(
                        &dataCopyInp_t::dataOprInp,
                        &dataCopyInp_t::portalOprOut))
            .add_property("dataOprInp", &dataCopyInp_t::dataOprInp)
            .add_property("portalOprOut", &dataCopyInp_t::portalOprOut)
        ;

        bp::class_<dataOprInp_t>("DataOprInp", bp::no_init)
            .def("__init__", make_init_function<dataOprInp_t>(
                        &dataOprInp_t::oprType,
                        &dataOprInp_t::numThreads,
                        &dataOprInp_t::srcL3descInx,
                        &dataOprInp_t::destL3descInx,
                        &dataOprInp_t::srcRescTypeInx,
                        &dataOprInp_t::destRescTypeInx,
                        &dataOprInp_t::offset,
                        &dataOprInp_t::dataSize,
                        &dataOprInp_t::condInput))
            .add_property("oprType", &dataOprInp_t::oprType)
            .add_property("numThreads", &dataOprInp_t::numThreads)
            .add_property("srcL3descInx", &dataOprInp_t::srcL3descInx)
            .add_property("destL3descInx", &dataOprInp_t::destL3descInx)
            .add_property("srcRescTypeInx", &dataOprInp_t::srcRescTypeInx)
            .add_property("destRescTypeInx", &dataOprInp_t::destRescTypeInx)
            .add_property("offset", &dataOprInp_t::offset)
            .add_property("dataSize", &dataOprInp_t::dataSize)
            .add_property("condInput", &dataOprInp_t::condInput)
        ;

        bp::class_<portalOprOut_t>("PortalOprOut", bp::no_init)
            .def("__init__", make_init_function<portalOprOut_t>(
                        &portalOprOut_t::status,
                        &portalOprOut_t::l1descInx,
                        &portalOprOut_t::numThreads,
                        &portalOprOut_t::chksum,
                        &portalOprOut_t::portList))
            .add_property("status", &portalOprOut_t::status)
            .add_property("l1descInx", &portalOprOut_t::l1descInx)
            .add_property("numThreads", &portalOprOut_t::numThreads)
            .add_property("chksum", +[](portalOprOut_t *s) { return array_ref<char>{s->chksum}; })
            .add_property("portList", &portalOprOut_t::portList)
        ;

        bp::class_<openedDataObjInp_t>("OpenedDataObjInp", bp::no_init)
            .def("__init__", make_init_function<openedDataObjInp_t>(
                        &openedDataObjInp_t::l1descInx,
                        &openedDataObjInp_t::len,
                        &openedDataObjInp_t::whence,
                        &openedDataObjInp_t::oprType,
                        &openedDataObjInp_t::offset,
                        &openedDataObjInp_t::bytesWritten,
                        &openedDataObjInp_t::condInput))
            .add_property("l1descInx", &openedDataObjInp_t::l1descInx)
            .add_property("len", &openedDataObjInp_t::len)
            .add_property("whence", &openedDataObjInp_t::whence)
            .add_property("oprType", &openedDataObjInp_t::oprType)
            .add_property("offset", &openedDataObjInp_t::offset)
            .add_property("bytesWritten", &openedDataObjInp_t::bytesWritten)
            .add_property("condInput", &openedDataObjInp_t::condInput)
        ;

        bp::class_<dataObjCopyInp_t>("DataObjCopyInp", bp::no_init)
            .def("__init__", make_init_function<dataObjCopyInp_t>(
                        &dataObjCopyInp_t::srcDataObjInp,
                        &dataObjCopyInp_t::destDataObjInp))
            .add_property("srcDataObjInp", &dataObjCopyInp_t::srcDataObjInp)
            .add_property("destDataObjInp", &dataObjCopyInp_t::destDataObjInp)

        ;

        bp::class_<openStat_t>("OpenStat", bp::no_init)
            .def("__init__", make_init_function<openStat_t>(
                        &openStat_t::dataSize,
                        &openStat_t::dataType,
                        &openStat_t::dataMode,
                        &openStat_t::l3descInx,
                        &openStat_t::replStatus,
                        &openStat_t::rescTypeInx,
                        &openStat_t::replNum))
            .add_property("dataSize", &openStat_t::dataSize)
            .add_property("dataType", +[](openStat_t *s) { return array_ref<char>{s->dataType}; })
            .add_property("dataMode", +[](openStat_t *s) { return array_ref<char>{s->dataMode}; })
            .add_property("l3descInx", &openStat_t::l3descInx)
            .add_property("replStatus", &openStat_t::replStatus)
            .add_property("rescTypeInx", &openStat_t::rescTypeInx)
            .add_property("replNum", &openStat_t::replNum)
        ;

        bp::class_<execCmd_t>("ExecCmd", bp::no_init)
            .def("__init__", make_init_function<execCmd_t>(
                        &execCmd_t::cmd,
                        &execCmd_t::cmdArgv,
                        &execCmd_t::execAddr,
                        &execCmd_t::hintPath,
                        &execCmd_t::addPathToArgv,
                        &execCmd_t::dummy,
                        &execCmd_t::condInput))
            .add_property("cmd", +[](execCmd_t *s) { return array_ref<char>{s->cmd}; })
            .add_property("cmdArgv", +[](execCmd_t *s) { return array_ref<char>{s->cmdArgv}; })
            .add_property("execAddr", +[](execCmd_t *s) { return array_ref<char>{s->execAddr}; })
            .add_property("hintPath", +[](execCmd_t *s) { return array_ref<char>{s->hintPath}; })
            .add_property("addPathToArgv", &execCmd_t::addPathToArgv)
            .add_property("dummy", &execCmd_t::dummy)
            .add_property("condInput", &execCmd_t::condInput)
        ;

        bp::class_<execCmdOut_t>("ExecCmdOut", bp::no_init)
            .def("__init__", make_init_function<execCmdOut_t>(
                        &execCmdOut_t::stdoutBuf,
                        &execCmdOut_t::stderrBuf,
                        &execCmdOut_t::status))
            .add_property("stdoutBuf", &execCmdOut_t::stdoutBuf)
            .add_property("stderrBuf", &execCmdOut_t::stderrBuf)
            .add_property("status", &execCmdOut_t::status)

        ;

        bp::class_<fileOpenInp_t>("FileOpenInp", bp::no_init)
            .def("__init__", make_init_function<fileOpenInp_t>(
                        &fileOpenInp_t::resc_name_,
                        &fileOpenInp_t::resc_hier_,
                        &fileOpenInp_t::objPath,
                        &fileOpenInp_t::otherFlags,
                        &fileOpenInp_t::addr,
                        &fileOpenInp_t::fileName,
                        &fileOpenInp_t::flags,
                        &fileOpenInp_t::mode,
                        &fileOpenInp_t::dataSize,
                        &fileOpenInp_t::condInput,
                        &fileOpenInp_t::in_pdmo))
            .add_property("resc_name", +[](fileOpenInp_t *s) { return array_ref<char>{s->resc_name_}; })
            .add_property("resc_hier", +[](fileOpenInp_t *s) { return array_ref<char>{s->resc_hier_}; })
            .add_property("objPath", +[](fileOpenInp_t *s) { return array_ref<char>{s->objPath}; })
            .add_property("otherFlags", &fileOpenInp_t::otherFlags)
            .add_property("addr", &fileOpenInp_t::addr)
            .add_property("fileName", +[](fileOpenInp_t *s) { return array_ref<char>{s->fileName}; })
            .add_property("flags", &fileOpenInp_t::flags)
            .add_property("mode", &fileOpenInp_t::mode)
            .add_property("dataSize", &fileOpenInp_t::dataSize)
            .add_property("condInput", &fileOpenInp_t::condInput)
            .add_property("in_pdmo", +[](fileOpenInp_t *s) { return array_ref<char>{s->in_pdmo}; })
        ;

        bp::class_<fileCreateOut_t>("FileCreateOut", bp::no_init)
            .def("__init__", make_init_function<fileCreateOut_t>(
                        &fileCreateOut_t::file_name))
            .add_property("file_name", +[](fileCreateOut_t *s) { return array_ref<char>{s->file_name}; })
        ;

        bp::class_<fileRenameInp_t>("FileRenameInp", bp::no_init)
            .def("__init__", make_init_function<fileRenameInp_t>(
                        &fileRenameInp_t::addr,
                        &fileRenameInp_t::oldFileName,
                        &fileRenameInp_t::newFileName,
                        &fileRenameInp_t::rescHier,
                        &fileRenameInp_t::objPath))
            .add_property("addr", &fileRenameInp_t::addr)
            .add_property("oldFileName", +[](fileRenameInp_t *s) { return array_ref<char>{s->oldFileName}; })
            .add_property("newFileName", +[](fileRenameInp_t *s) { return array_ref<char>{s->newFileName}; })
            .add_property("rescHier", +[](fileRenameInp_t *s) { return array_ref<char>{s->rescHier}; })
            .add_property("objPath", +[](fileRenameInp_t *s) { return array_ref<char>{s->objPath}; })
        ;

        bp::class_<fileRenameOut_t>("FileRenameOut", bp::no_init)
            .def("__init__", make_init_function<fileRenameOut_t>(
                        &fileRenameOut_t::file_name))
            .add_property("file_name", +[](fileRenameOut_t *s) { return array_ref<char>{s->file_name}; })
        ;

        bp::class_<rodsStat_t>("RodsStat", bp::no_init)
            .def("__init__", make_init_function<rodsStat_t>(
                        &rodsStat_t::st_size,
                        &rodsStat_t::st_dev,
                        &rodsStat_t::st_ino,
                        &rodsStat_t::st_mode,
                        &rodsStat_t::st_nlink,
                        &rodsStat_t::st_uid,
                        &rodsStat_t::st_gid,
                        &rodsStat_t::st_rdev,
                        &rodsStat_t::st_atim,
                        &rodsStat_t::st_mtim,
                        &rodsStat_t::st_ctim,
                        &rodsStat_t::st_blksize,
                        &rodsStat_t::st_blocks))
            .add_property("st_size", &rodsStat_t::st_size)
            .add_property("st_dev", &rodsStat_t::st_dev)
            .add_property("st_ino", &rodsStat_t::st_ino)
            .add_property("st_mode", &rodsStat_t::st_mode)
            .add_property("st_nlink", &rodsStat_t::st_nlink)
            .add_property("st_uid", &rodsStat_t::st_uid)
            .add_property("st_gid", &rodsStat_t::st_gid)
            .add_property("st_rdev", &rodsStat_t::st_rdev)
            .add_property("st_atim", &rodsStat_t::st_atim)
            .add_property("st_mtim", &rodsStat_t::st_mtim)
            .add_property("st_ctim", &rodsStat_t::st_ctim)
            .add_property("st_blksize", &rodsStat_t::st_blksize)
            .add_property("st_blocks", &rodsStat_t::st_blocks)
        ;

        bp::class_<rodsHostAddr_t>("RodsHostAddr", bp::no_init)
            .def("__init__", make_init_function<rodsHostAddr_t>(
                        &rodsHostAddr_t::hostAddr,
                        &rodsHostAddr_t::zoneName,
                        &rodsHostAddr_t::portNum,
                        &rodsHostAddr_t::dummyInt))
            .add_property("hostAddr", +[](rodsHostAddr_t *s) { return array_ref<char>{s->hostAddr}; })
            .add_property("zoneName", +[](rodsHostAddr_t *s) { return array_ref<char>{s->zoneName}; })
            .add_property("portNum", &rodsHostAddr_t::portNum)
            .add_property("dummyInt", &rodsHostAddr_t::dummyInt)
        ;

        bp::class_<getXmsgTicketInp_t>("GetXmsgTicketInp", bp::no_init)
            .def("__init__", make_init_function<getXmsgTicketInp_t>(
                        &getXmsgTicketInp_t::expireTime,
                        &getXmsgTicketInp_t::flag))
            .add_property("expireTime", &getXmsgTicketInp_t::expireTime)
            .add_property("flag", &getXmsgTicketInp_t::flag)
        ;

        bp::class_<xmsgTicketInfo_t>("XmsgTicketInfo", bp::no_init)
            .def("__init__", make_init_function<xmsgTicketInfo_t>(
                        &xmsgTicketInfo_t::sendTicket,
                        &xmsgTicketInfo_t::rcvTicket,
                        &xmsgTicketInfo_t::expireTime,
                        &xmsgTicketInfo_t::flag))
            .add_property("sendTicket", &xmsgTicketInfo_t::sendTicket)
            .add_property("rcvTicket", &xmsgTicketInfo_t::rcvTicket)
            .add_property("expireTime", &xmsgTicketInfo_t::expireTime)
            .add_property("flag", &xmsgTicketInfo_t::flag)
        ;

        bp::class_<modAccessControlInp_t>("ModAccessControlInp", bp::no_init)
            .def("__init__", make_init_function<modAccessControlInp_t>(
                        &modAccessControlInp_t::recursiveFlag,
                        &modAccessControlInp_t::accessLevel,
                        &modAccessControlInp_t::userName,
                        &modAccessControlInp_t::zone,
                        &modAccessControlInp_t::path))
            .add_property("recursiveFlag", &modAccessControlInp_t::recursiveFlag)
            .add_property("accessLevel", +[](modAccessControlInp_t *s) { return array_ref<char, true>{s->accessLevel}; })
            .add_property("userName", +[](modAccessControlInp_t *s) { return array_ref<char, true>{s->userName}; })
            .add_property("zone", +[](modAccessControlInp_t *s) { return array_ref<char, true>{s->zone}; })
            .add_property("path", +[](modAccessControlInp_t *s) { return array_ref<char, true>{s->path}; })
        ;

        bp::class_<structFileExtAndRegInp_t>("StructFileExtAndRegInp", bp::no_init)
            .def("__init__", make_init_function<structFileExtAndRegInp_t>(
                        &structFileExtAndRegInp_t::objPath,
                        &structFileExtAndRegInp_t::collection,
                        &structFileExtAndRegInp_t::oprType,
                        &structFileExtAndRegInp_t::flags,
                        &structFileExtAndRegInp_t::condInput))
            .add_property("objPath", +[](structFileExtAndRegInp_t *s) { return array_ref<char>{s->objPath}; })
            .add_property("collection", +[](structFileExtAndRegInp_t *s) { return array_ref<char>{s->collection}; })
            .add_property("oprType", &structFileExtAndRegInp_t::oprType)
            .add_property("flags", &structFileExtAndRegInp_t::flags)
            .add_property("condInput", &structFileExtAndRegInp_t::condInput)
        ;

        bp::class_<rcvXmsgInp_t>("RcvXmsgInp", bp::no_init)
            .def("__init__", make_init_function<rcvXmsgInp_t>(
                        &rcvXmsgInp_t::rcvTicket,
                        &rcvXmsgInp_t::msgNumber,
                        &rcvXmsgInp_t::seqNumber,
                        &rcvXmsgInp_t::msgCondition))
            .add_property("rcvTicket", &rcvXmsgInp_t::rcvTicket)
            .add_property("msgNumber", &rcvXmsgInp_t::msgNumber)
            .add_property("seqNumber", &rcvXmsgInp_t::seqNumber)
            .add_property("msgCondition", +[](rcvXmsgInp_t *s) { return array_ref<char>{s->msgCondition}; })

        ;

        bp::class_<rcvXmsgOut_t>("RcvXmsgOut", bp::no_init)
            .def("__init__", make_init_function<rcvXmsgOut_t>(
                        &rcvXmsgOut_t::msgType,
                        &rcvXmsgOut_t::sendUserName,
                        &rcvXmsgOut_t::sendAddr,
                        &rcvXmsgOut_t::msgNumber,
                        &rcvXmsgOut_t::seqNumber,
                        &rcvXmsgOut_t::msg))
            .add_property("msgType", +[](rcvXmsgOut_t *s) { return array_ref<char>{s->msgType}; })
            .add_property("sendUserName", +[](rcvXmsgOut_t *s) { return array_ref<char>{s->sendUserName}; })
            .add_property("sendAddr", +[](rcvXmsgOut_t *s) { return array_ref<char>{s->sendAddr}; })
            .add_property("msgNumber", &rcvXmsgOut_t::msgNumber)
            .add_property("seqNumber", &rcvXmsgOut_t::seqNumber)
            .add_property("msg", +[](rcvXmsgOut_t *s) { return array_ref<char, true>{s->msg}; })
        ;

        bp::class_<collEnt_t>("CollEnt", bp::no_init)
            .def("__init__", make_init_function<collEnt_t>(
                        &collEnt_t::objType,
                        &collEnt_t::replNum,
                        &collEnt_t::replStatus,
                        &collEnt_t::dataMode,
                        &collEnt_t::dataSize,
                        &collEnt_t::collName,
                        &collEnt_t::dataName,
                        &collEnt_t::dataId,
                        &collEnt_t::createTime,
                        &collEnt_t::modifyTime,
                        &collEnt_t::chksum,
                        &collEnt_t::resource,
                        &collEnt_t::resc_hier,
                        &collEnt_t::phyPath,
                        &collEnt_t::ownerName,
                        &collEnt_t::dataType,
                        &collEnt_t::specColl))
            .add_property("objType", &collEnt_t::objType)
            .add_property("replNum", &collEnt_t::replNum)
            .add_property("replStatus", &collEnt_t::replStatus)
            .add_property("dataMode", &collEnt_t::dataMode)
            .add_property("dataSize", &collEnt_t::dataSize)
            .add_property("collName", +[](collEnt_t *s) { return array_ref<char, true>{s->collName}; })
            .add_property("dataName", +[](collEnt_t *s) { return array_ref<char, true>{s->dataName}; })
            .add_property("dataId", +[](collEnt_t *s) { return array_ref<char, true>{s->dataId}; })
            .add_property("createTime", +[](collEnt_t *s) { return array_ref<char, true>{s->createTime}; })
            .add_property("modifyTime", +[](collEnt_t *s) { return array_ref<char, true>{s->modifyTime}; })
            .add_property("chksum", +[](collEnt_t *s) { return array_ref<char, true>{s->chksum}; })
            .add_property("resource", +[](collEnt_t *s) { return array_ref<char, true>{s->resource}; })
            .add_property("resc_hier", +[](collEnt_t *s) { return array_ref<char, true>{s->resc_hier}; })
            .add_property("phyPath", +[](collEnt_t *s) { return array_ref<char, true>{s->phyPath}; })
            .add_property("ownerName", +[](collEnt_t *s) { return array_ref<char, true>{s->ownerName}; })
            .add_property("dataType", +[](collEnt_t *s) { return array_ref<char, true>{s->dataType}; })
            .add_property("specColl", &collEnt_t::specColl)
        ;

        bp::class_<ruleExecDelInp_t>("RuleExecDelInp", bp::no_init)
            .def("__init__", make_init_function<ruleExecDelInp_t>(
                        &ruleExecDelInp_t::ruleExecId))
            .add_property("ruleExecId", +[](ruleExecDelInp_t *s) { return array_ref<char>{s->ruleExecId}; })
        ;

        bp::class_<sendXmsgInp_t>("SendXmsgInp", bp::no_init)
            .def("__init__", make_init_function<sendXmsgInp_t>(
                        &sendXmsgInp_t::ticket,
                        &sendXmsgInp_t::sendAddr,
                        &sendXmsgInp_t::sendXmsgInfo))
            .add_property("ticket", &sendXmsgInp_t::ticket)
            .add_property("sendAddr", +[](sendXmsgInp_t *s) { return array_ref<char>{s->sendAddr}; })
            .add_property("sendXmsgInfo", &sendXmsgInp_t::sendXmsgInfo)
        ;

        bp::class_<structFileOprInp_t>("StructFileOprInp", bp::no_init)
            .def("__init__", make_init_function<structFileOprInp_t>(
                        &structFileOprInp_t::addr,
                        &structFileOprInp_t::oprType,
                        &structFileOprInp_t::flags,
                        &structFileOprInp_t::specColl,
                        &structFileOprInp_t::condInput))
            .add_property("addr", &structFileOprInp_t::addr)
            .add_property("oprType", &structFileOprInp_t::oprType)
            .add_property("flags", &structFileOprInp_t::flags)
            .add_property("specColl",
                    bp::make_getter(&structFileOprInp_t::specColl, bp::return_internal_reference<1>{}),
                    bp::make_setter(&structFileOprInp_t::specColl, bp::with_custodian_and_ward<1, 2>{}))
            .add_property("condInput", &structFileOprInp_t::condInput)
        ;

        bp::class_<subStructFileFdOprInp_t>("SubStructFileFdOprInp", bp::no_init)
            .def("__init__", make_init_function<subStructFileFdOprInp_t>(
                        &subStructFileFdOprInp_t::addr,
                        &subStructFileFdOprInp_t::type,
                        &subStructFileFdOprInp_t::fd,
                        &subStructFileFdOprInp_t::len,
                        &subStructFileFdOprInp_t::resc_hier))
            .add_property("addr", &subStructFileFdOprInp_t::addr)
            .add_property("type", &subStructFileFdOprInp_t::type)
            .add_property("fd", &subStructFileFdOprInp_t::fd)
            .add_property("len", &subStructFileFdOprInp_t::len)
            .add_property("resc_hier", +[](subStructFileFdOprInp_t *s) { return array_ref<char>{s->resc_hier}; })
        ;

        bp::class_<subFile_t>("SubFile", bp::no_init)
            .def("__init__", make_init_function<subFile_t>(
                        &subFile_t::addr,
                        &subFile_t::subFilePath,
                        &subFile_t::mode,
                        &subFile_t::flags,
                        &subFile_t::offset,
                        &subFile_t::specColl))
            .add_property("addr", &subFile_t::addr)
            .add_property("subFilePath", +[](subFile_t *s) { return array_ref<char>{s->subFilePath}; })
            .add_property("mode", &subFile_t::mode)
            .add_property("flags", &subFile_t::flags)
            .add_property("specColl",
                    bp::make_getter(&subFile_t::specColl, bp::return_internal_reference<1>{}),
                    bp::make_setter(&subFile_t::specColl, bp::with_custodian_and_ward<1, 2>{}))
        ;

        bp::class_<fileLseekInp_t>("FileLseekInp_t", bp::no_init)
            .def("__init__", make_init_function<fileLseekInp_t>(
                            &fileLseekInp_t::fileInx,
                            &fileLseekInp_t::offset,
                            &fileLseekInp_t::whence))
            .add_property("fileInx", &fileLseekInp_t::fileInx)
            .add_property("offset", &fileLseekInp_t::offset)
            .add_property("whence", &fileLseekInp_t::whence)
        ;

        bp::class_<fileLseekOut_t>("FileLseekOut_t", bp::no_init)
            .def("__init__", make_init_function<fileLseekOut_t>(
                            &fileLseekOut_t::offset))
            .add_property("offset", &fileLseekOut_t::offset)

        ;
    }
}
