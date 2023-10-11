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
					&sockaddr_in::sin_addr,
					&sockaddr_in::sin_zero))
			.add_property("sin_family", &sockaddr_in::sin_family)
			.add_property("sin_port", &sockaddr_in::sin_port)
			.add_property("sin_addr", &sockaddr_in::sin_addr)
			// sin_zero is intentionally omitted here because the member is not part of the standard. It is included
			// above in the list of members for sockaddr_in because all currently supported platforms include the
			// sin_zero member. As such, the struct member is included for make_init_function, but it is not exposed
			// as a property in the event that it must be removed in the future. For more information, see bwg2001-004
			// here: http://www.opengroup.org/platform/resolutions/bwg2001-many.html
			;
		// clang-format on
	}
} //namespace irods::re::python::types
