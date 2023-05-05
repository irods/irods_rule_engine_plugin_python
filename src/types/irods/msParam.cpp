// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/msParam.hpp"

#include <irods/msParam.h>

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
	__attribute__((visibility("hidden"))) void export_MsParam()
	{
		// clang-format off
		bp::class_<msParam_t>("MsParam", bp::no_init)
			.def("__init__", make_init_function<msParam_t>(
					&msParam_t::label,
					&msParam_t::type,
					&msParam_t::inOutStruct,
					&msParam_t::inpOutBuf))
			.add_property("label", +[](msParam_t *s) { return array_ref<char, true>{s->label}; })
			.add_property("type", +[](msParam_t *s) { return array_ref<char, true>{s->label}; })
			// Making inOutStruct and inpOutBuf readable in python is beyond scope at the moment
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_MsParam_pointer_array()
	{
		// clang-format off
		bp::class_<array_ref<msParam_t*>>("MsParam_pointer_array")
			.def(array_indexing_suite<array_ref<msParam_t*>>{}) //, bp::return_internal_reference<1>>{} )
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_MsParamArray()
	{
		// clang-format off
		bp::class_<msParamArray_t>("MsParamArray", bp::no_init)
			.def("__init__", make_init_function<msParamArray_t>(
					&msParamArray_t::len,
					&msParamArray_t::oprType,
					&msParamArray_t::msParam))
			.add_property("len", &msParamArray_t::len)
			.add_property("oprType", &msParamArray_t::oprType)
			.add_property("msParam", +[](msParamArray_t *s) { return array_ref<msParam_t*>{s->msParam, static_cast<std::size_t>(s->len)}; })
			;
		// clang-format on
	}
}
