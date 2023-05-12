// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/structFileSync.hpp"

#include <irods/structFileSync.h>

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
	__attribute__((visibility("hidden"))) void export_StructFileOprInp()
	{
		// clang-format off
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
		// clang-format on
	}
} //namespace irods::re::python::types
