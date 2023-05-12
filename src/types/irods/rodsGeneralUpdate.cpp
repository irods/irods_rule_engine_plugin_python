// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/rodsGeneralUpdate.hpp"

#include <irods/rodsGeneralUpdate.h>

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
	__attribute__((visibility("hidden"))) void export_GeneralUpdateInp()
	{
		// clang-format off
		bp::class_<generalUpdateInp_t>("GeneralUpdateInp", bp::no_init)
			.def("__init__", make_init_function<generalUpdateInp_t>(
					&generalUpdateInp_t::type,
					&generalUpdateInp_t::values))
			.add_property("type", &generalUpdateInp_t::type)
			.add_property("values", &generalUpdateInp_t::values)
			;
		// clang-format on
	}
} //namespace irods::re::python::types
