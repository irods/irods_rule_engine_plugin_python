// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/irods_threads.hpp"

// include boost/thread.hpp before system irods includes to silence boost-caused deprecation warnings
#include <boost/version.hpp>
#if BOOST_VERSION < 108100
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#  include <boost/thread.hpp>
#  pragma GCC diagnostic pop
#endif

#include <irods/irods_threads.hpp>

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
	__attribute__((visibility("hidden"))) void export_ThreadContext()
	{
		// clang-format off
		//No accessors for thread_context. No one needs this and it would require exposing
		//quite a lot of the internals of boost
		bp::class_<thread_context>("ThreadContext", bp::no_init)
			.def("__init__", make_init_function<thread_context>(
					&thread_context::reconnThr,
					&thread_context::lock,
					&thread_context::cond))
			;
		// clang-format on
	}
} //namespace irods::re::python::types
