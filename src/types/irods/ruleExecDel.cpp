// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/ruleExecDel.hpp"

#include <irods/ruleExecDel.h>

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
	__attribute__((visibility("hidden"))) void export_RuleExecDelInp()
	{
		// clang-format off
		bp::class_<ruleExecDelInp_t>("RuleExecDelInp", bp::no_init)
			.def("__init__", make_init_function<ruleExecDelInp_t>(
					&ruleExecDelInp_t::ruleExecId))
			.add_property("ruleExecId", +[](ruleExecDelInp_t *s) { return array_ref<char>{s->ruleExecId}; })
			;
		// clang-format on
	}
}
