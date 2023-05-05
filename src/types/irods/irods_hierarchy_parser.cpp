// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/irods_hierarchy_parser.hpp"

// include boost/container_hash/hash.hpp before system irods includes to silence boost-caused deprecation warnings
#include <boost/version.hpp>
#if BOOST_VERSION < 108100
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <boost/container_hash/hash.hpp>
#pragma GCC diagnostic pop
#endif

#include <irods/irods_hierarchy_parser.hpp>

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
	__attribute__((visibility("hidden"))) void export_HierarchyParser()
	{
		// clang-format off
		bp::class_<irods::hierarchy_parser>("HierarchyParser", bp::no_init)
			.add_property("str", +[](irods::hierarchy_parser* _parser) { std::string ret_string; _parser->str(ret_string, ""); return ret_string; } )
			;
		// clang-format on
	}
}
