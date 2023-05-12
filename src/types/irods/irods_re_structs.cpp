// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/irods_re_structs.hpp"

// include boost/any.hpp before system irods includes to silence boost-caused deprecation warnings
#include <boost/version.hpp>
#if BOOST_VERSION < 108100
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#  include <boost/any.hpp>
#  pragma GCC diagnostic pop
#endif

#include <irods/irods_re_structs.hpp>

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
	__attribute__((visibility("hidden"))) void export_RuleExecInfo()
	{
		// clang-format off
		bp::class_<ruleExecInfo_t>("RuleExecInfo", bp::no_init)
			.def("__init__", make_init_function<ruleExecInfo_t>(
					&ruleExecInfo_t::status,
					&ruleExecInfo_t::statusStr,
					&ruleExecInfo_t::ruleName,
					&ruleExecInfo_t::rsComm,
					&ruleExecInfo_t::pluginInstanceName,
					&ruleExecInfo_t::msParamArray,
					&ruleExecInfo_t::inOutMsParamArray,
					&ruleExecInfo_t::l1descInx,
					&ruleExecInfo_t::doinp,
					&ruleExecInfo_t::doi,
					&ruleExecInfo_t::rescName,
					&ruleExecInfo_t::uoic,
					&ruleExecInfo_t::uoip,
					&ruleExecInfo_t::coi,
					&ruleExecInfo_t::uoio,
					&ruleExecInfo_t::condInputData,
					&ruleExecInfo_t::ruleSet,
					&ruleExecInfo_t::next))
			.add_property("status", &ruleExecInfo_t::status)
			.add_property("statusStr", +[](ruleExecInfo_t *s) { return array_ref<char>{s->statusStr}; })
			.add_property("ruleName", +[](ruleExecInfo_t *s) { return array_ref<char>{s->ruleName}; })
			.add_property("rsComm",
			              bp::make_getter(&ruleExecInfo_t::rsComm, bp::return_internal_reference<1>{}),
			              bp::make_setter(&ruleExecInfo_t::rsComm, bp::with_custodian_and_ward<1, 2>{}))
			.add_property("pluginInstanceName", +[](ruleExecInfo_t *s) { return array_ref<char>{s->pluginInstanceName}; })
			.add_property("msParamArray",
			              bp::make_getter(&ruleExecInfo_t::msParamArray, bp::return_internal_reference<1>{}),
			              bp::make_setter(&ruleExecInfo_t::msParamArray, bp::with_custodian_and_ward<1, 2>{}))
			.add_property("inOutMsParamArray", &ruleExecInfo_t::inOutMsParamArray)
			.add_property("l1descInx", &ruleExecInfo_t::l1descInx)
			.add_property("doinp",
			              bp::make_getter(&ruleExecInfo_t::doinp, bp::return_internal_reference<1>{}),
			              bp::make_setter(&ruleExecInfo_t::doinp, bp::with_custodian_and_ward<1, 2>{}))
			.add_property("doi",
			              bp::make_getter(&ruleExecInfo_t::doi, bp::return_internal_reference<1>{}),
			              bp::make_setter(&ruleExecInfo_t::doi, bp::with_custodian_and_ward<1, 2>{}))
			.add_property("rescName", +[](ruleExecInfo_t *s) { return array_ref<char>{s->rescName}; })
			.add_property("uoic",
			              bp::make_getter(&ruleExecInfo_t::uoic, bp::return_internal_reference<1>{}),
			              bp::make_setter(&ruleExecInfo_t::uoic, bp::with_custodian_and_ward<1, 2>{}))
			.add_property("uoip",
			              bp::make_getter(&ruleExecInfo_t::uoip, bp::return_internal_reference<1>{}),
			              bp::make_setter(&ruleExecInfo_t::uoip, bp::with_custodian_and_ward<1, 2>{}))
			.add_property("coi",
			              bp::make_getter(&ruleExecInfo_t::coi, bp::return_internal_reference<1>{}),
			              bp::make_setter(&ruleExecInfo_t::coi, bp::with_custodian_and_ward<1, 2>{}))
			.add_property("uoio",
			              bp::make_getter(&ruleExecInfo_t::uoio, bp::return_internal_reference<1>{}),
			              bp::make_setter(&ruleExecInfo_t::uoio, bp::with_custodian_and_ward<1, 2>{}))
			.add_property("condInputData",
			              bp::make_getter(&ruleExecInfo_t::condInputData, bp::return_internal_reference<1>{}),
			              bp::make_setter(&ruleExecInfo_t::condInputData, bp::with_custodian_and_ward<1, 2>{}))
			.add_property("ruleSet", +[](ruleExecInfo_t *s) { return array_ref<char>{s->ruleSet}; })
			.add_property("next",
			              bp::make_getter(&ruleExecInfo_t::next, bp::return_internal_reference<1>{}),
			              bp::make_setter(&ruleExecInfo_t::next, bp::with_custodian_and_ward<1, 2>{}))
			;
		// clang-format on
	}
} //namespace irods::re::python::types
