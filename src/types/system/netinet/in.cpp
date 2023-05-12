// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/system/netinet/in.hpp"

#include <netinet/in.h>

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
	__attribute__((visibility("hidden"))) void export_In_Addr()
	{
		// clang-format off
		bp::class_<in_addr>("In_Addr", bp::no_init)
			.def("__init__", make_init_function<in_addr>(
					&in_addr::s_addr))
			.add_property("s_addr", &in_addr::s_addr)
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_Sockaddr_In()
	{
		// clang-format off
		bp::class_<sockaddr_in>("Sockaddr_In", bp::no_init)
			.def("__init__", make_init_function<sockaddr_in>(
					&sockaddr_in::sin_family,
					&sockaddr_in::sin_port,
					&sockaddr_in::sin_addr))
			.add_property("sin_family", &sockaddr_in::sin_family)
			.add_property("sin_port", &sockaddr_in::sin_port)
			.add_property("sin_addr", &sockaddr_in::sin_addr)
			;
		// clang-format on
	}
} //namespace irods::re::python::types
