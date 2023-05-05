// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/standard/containers.hpp"

#include <patchlevel.h>
#include <boost/version.hpp>
#pragma GCC diagnostic push
#if PY_VERSION_HEX < 0x030400A2
#  pragma GCC diagnostic ignored "-Wregister"
#endif
#if BOOST_VERSION < 108100
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <boost/python/class.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#pragma GCC diagnostic pop

namespace bp = boost::python;

namespace irods::re::python::types
{
	__attribute__((visibility("hidden"))) void export_StringToStringMap()
	{
		// clang-format off
		bp::class_<std::map<std::string, std::string>>("StringToStringMap")
			.def("__getitem__", +[](std::map<std::string, std::string>* m, const std::string& key) { return (*m)[key]; })
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_IntVector()
	{
		// clang-format off
		boost::python::class_<std::vector<int>>("IntVector")
			.def(bp::vector_indexing_suite<std::vector<int>>{})
			;
		// clang-format on
	}
}
