// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/objStat.hpp"

#include <irods/objStat.h>

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
	__attribute__((visibility("hidden"))) void export_RodsObjStat()
	{
		// clang-format off
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
		// clang-format on
	}
}
