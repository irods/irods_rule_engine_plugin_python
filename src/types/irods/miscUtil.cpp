// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/miscUtil.hpp"

#include <irods/miscUtil.h>

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
	__attribute__((visibility("hidden"))) void export_CollEnt()
	{
		// clang-format off
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
		// clang-format on
	}
} //namespace irods::re::python::types
