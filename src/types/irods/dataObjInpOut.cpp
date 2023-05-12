// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/dataObjInpOut.hpp"

#include <irods/dataObjInpOut.h>

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
#pragma GCC diagnostic pop

namespace bp = boost::python;

namespace irods::re::python::types
{
	__attribute__((visibility("hidden"))) void export_PortList()
	{
		// clang-format off
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
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_DataObjInp()
	{
		// clang-format off
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
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_OpenedDataObjInp()
	{
		// clang-format off
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
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_PortalOprOut()
	{
		// clang-format off
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
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_DataOprInp()
	{
		// clang-format off
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
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_CollInp()
	{
		// clang-format off
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
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_PortalOpr()
	{
		// clang-format off
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
		// clang-format on
	}
} //namespace irods::re::python::types
