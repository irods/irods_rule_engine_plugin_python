// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/specificQuery.hpp"

#include <irods/specificQuery.h>

#include "irods/private/re/python/types/array_ref.hpp"
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
	__attribute__((visibility("hidden"))) void export_SpecificQueryInp()
	{
		// clang-format off
		bp::class_<specificQueryInp_t>("SpecificQueryInp", bp::no_init)
			.def("__init__", make_init_function<specificQueryInp_t>(
					&specificQueryInp_t::sql,
					&specificQueryInp_t::args,
					&specificQueryInp_t::maxRows,
					&specificQueryInp_t::continueInx,
					&specificQueryInp_t::rowOffset,
					&specificQueryInp_t::options,
					&specificQueryInp_t::condInput))
			.add_property("sql", +[](specificQueryInp_t *s) { return array_ref<char, true>{s->sql}; })
			.add_property("args", +[](specificQueryInp_t *s) { return array_ref<array_ref<char, true>>{s->args}; })
			.add_property("maxRows", &specificQueryInp_t::maxRows)
			.add_property("continueInx", &specificQueryInp_t::continueInx)
			.add_property("rowOffset", &specificQueryInp_t::rowOffset)
			.add_property("options", &specificQueryInp_t::options)
			.add_property("condInput", &specificQueryInp_t::condInput)
			;
		// clang-format on
	}
} //namespace irods::re::python::types
