// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/system/sys/time.hpp"

#include <sys/time.h>

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
#pragma GCC diagnostic pop

namespace bp = boost::python;

namespace irods::re::python::types
{
	__attribute__((visibility("hidden"))) void export_Timeval()
	{
		// clang-format off
		bp::class_<struct timeval>("Timeval", bp::no_init)
			.add_property("tv_sec", &timeval::tv_sec)
			.add_property("tv_usec", &timeval::tv_usec)
			;
		// clang-format on
	}
}
