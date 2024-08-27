// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/objInfo.hpp"

#include <cstring>

#include <irods/objInfo.h>

#include "irods/private/re/python/types/array_ref.hpp"
#include "irods/private/re/python/types/init_struct.hpp"

#include <patchlevel.h>
#include <boost/version.hpp>
#pragma GCC diagnostic push
#if PY_VERSION_HEX < 0x030400A2
#  pragma GCC diagnostic ignored "-Wregister"
#endif
#if PY_VERSION_HEX >= 0x03090000 && BOOST_VERSION < 107500
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <boost/python/class.hpp>
#include <boost/python/enum.hpp>
#pragma GCC diagnostic pop

#include <fmt/format.h>

namespace bp = boost::python;

namespace irods::re::python::types
{
	__attribute__((visibility("hidden"))) void export_specCollClass()
	{
		// clang-format off
		bp::enum_<specCollClass_t>("specCollClass")
			.value("NO_SPEC_COLL", specCollClass_t::NO_SPEC_COLL)
			.value("STRUCT_FILE_COLL", specCollClass_t::STRUCT_FILE_COLL)
			.value("MOUNTED_COLL", specCollClass_t::MOUNTED_COLL)
			.value("LINKED_COLL", specCollClass_t::LINKED_COLL)
			.export_values();
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_structFileType()
	{
		// clang-format off
		bp::enum_<structFileType_t>("structFileType")
			.value("NONE_STRUCT_FILE", structFileType_t::NONE_STRUCT_FILE_T)
			.value("HAAW_STRUCT_FILE", structFileType_t::HAAW_STRUCT_FILE_T)
			.value("TAR_STRUCT_FILE", structFileType_t::TAR_STRUCT_FILE_T)
			.value("MSSO_STRUCT_FILE", structFileType_t::MSSO_STRUCT_FILE_T)
			.export_values();
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_SpecColl()
	{
		// clang-format off
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
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_KeyValPair()
	{
		// clang-format off
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
						if (std::strcmp(key.c_str(), s->keyWord[i]) == 0) {
							return std::string{s->value[i]};
						}
					}
					PyErr_SetString(PyExc_KeyError, fmt::format("{0} was not found in the list of keys.", key).c_str());
					bp::throw_error_already_set();
					return std::string{};
				})
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_DataObjInfo()
	{
		// clang-format off
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
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_CollInfo()
	{
		// clang-format off
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
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_InxIvalPair()
	{
		// clang-format off
		bp::class_<inxIvalPair_t>("InxIvalPair", bp::no_init)
			.def("__init__", make_init_function<inxIvalPair_t>(
					&inxIvalPair_t::len,
					&inxIvalPair_t::inx,
					&inxIvalPair_t::value))
			.add_property("len", &inxIvalPair_t::len)
			.add_property("inx", +[](inxIvalPair_t *s) { return array_ref<int>{s->inx, static_cast<std::size_t>(s->len)}; })
			.add_property("value", +[](inxIvalPair_t *s) { return array_ref<int>{s->value, static_cast<std::size_t>(s->len)}; })
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_InxValPair()
	{
		// clang-format off
		bp::class_<inxValPair_t>("InxValPair", bp::no_init)
			.def("__init__", make_init_function<inxValPair_t>(
					&inxValPair_t::len,
					&inxValPair_t::inx,
					&inxValPair_t::value))
			.add_property("len", &inxValPair_t::len)
			.add_property("inx", +[](inxValPair_t *s) { return array_ref<int>{s->inx, static_cast<std::size_t>(s->len)}; })
			.add_property("value", +[](inxValPair_t *s) { return array_ref<array_ref<char, true>>{s->value, static_cast<std::size_t>(s->len)}; })
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_TransferStat()
	{
		// clang-format off
		bp::class_<transferStat_t>("TransferStat", bp::no_init)
			.def("__init__", make_init_function<transferStat_t>(
					&transferStat_t::numThreads,
					&transferStat_t::flags,
					&transferStat_t::bytesWritten))
			.add_property("len", &transferStat_t::numThreads)
			.add_property("len", &transferStat_t::flags)
			.add_property("offset", &transferStat_t::bytesWritten)
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_CollOprStat()
	{
		// clang-format off
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
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_SubFile()
	{
		// clang-format off
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
		// clang-format on
	}
} //namespace irods::re::python::types
