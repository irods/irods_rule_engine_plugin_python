// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/dataObjOpenAndStat.hpp"

#include <irods/dataObjOpenAndStat.h>

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
	__attribute__((visibility("hidden"))) void export_OpenStat()
	{
		// clang-format off
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
		// clang-format on
	}
} //namespace irods::re::python::types
