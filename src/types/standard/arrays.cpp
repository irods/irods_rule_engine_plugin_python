// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/standard/arrays.hpp"

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
	__attribute__((visibility("hidden"))) void export_int_array()
	{
		// clang-format off
		bp::class_<array_ref<int>>("int_array")
			.def(array_indexing_suite<array_ref<int>>{})
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_char_array()
	{
		// clang-format off
		bp::class_<array_ref<char>>("char_array")
			.def(array_indexing_suite<array_ref<char>>{})
			.def("__str__", +[](array_ref<char>* s) { return std::string{s->data()}; })
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_unsigned_char_array()
	{
		// clang-format off
		bp::class_<array_ref<unsigned char>>("unsigned_char_array")
			.def(array_indexing_suite<array_ref<unsigned char>>{})
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_c_string()
	{
		// clang-format off
		bp::class_<array_ref<char, true>>("c_string")
			.def(array_indexing_suite<array_ref<char, true>>{})
			.def("__str__", +[](array_ref<char, true>* s) { return std::string{s->data()}; })
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_c_string_array()
	{
		// clang-format off
		bp::class_<array_ref<array_ref<char, true>>>("c_string_array")
			.def(array_indexing_suite<array_ref<array_ref<char, true>>>{})
			;
		// clang-format on
	}
}
