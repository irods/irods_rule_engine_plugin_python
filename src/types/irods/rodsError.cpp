// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/rodsError.hpp"

#include <irods/rodsError.h>

#include "irods/private/re/python/types/init_struct.hpp"
#include "irods/private/re/python/types/array_indexing_suite.hpp"
#include "irods/private/re/python/types/array_ref.hpp"

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
	__attribute__((visibility("hidden"))) void export_RErrMsg()
	{
		// clang-format off
		bp::class_<rErrMsg_t>("RErrMsg", bp::no_init)
			.def("__init__", make_init_function<rErrMsg_t>(
					&rErrMsg_t::status,
					&rErrMsg_t::msg))
			.add_property("status", &rErrMsg_t::status)
			.add_property("msg", +[](rErrMsg_t *s) { return array_ref<char>{s->msg}; })
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_RErrMsg_pointer_array()
	{
		// clang-format off
		bp::class_<array_ref<rErrMsg_t*>>("RErrMsg_pointer_array")
			.def(array_indexing_suite<array_ref<rErrMsg_t*>>{}) //, bp::return_internal_reference<1>>{} )
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_RError()
	{
		// clang-format off
		bp::class_<rError_t>("RError", bp::no_init)
			.def("__init__", make_init_function<rError_t>(
					&rError_t::len,
					&rError_t::errMsg))
			.add_property("len", &rError_t::len)
			.add_property("errMsg", +[](rError_t *s) { return array_ref<rErrMsg_t*>{s->errMsg, static_cast<std::size_t>(s->len)}; })
			;
		// clang-format on
	}
}
