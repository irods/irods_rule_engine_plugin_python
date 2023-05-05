// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/ruleExecSubmit.hpp"

#include <irods/ruleExecSubmit.h>

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
	__attribute__((visibility("hidden"))) void export_RuleExecSubmitInp()
	{
		// clang-format off
		bp::class_<ruleExecSubmitInp_t>("RuleExecSubmitInp", bp::no_init)
			.def("__init__", make_init_function<ruleExecSubmitInp_t>(
					&ruleExecSubmitInp_t::ruleName,
					&ruleExecSubmitInp_t::reiFilePath,
					&ruleExecSubmitInp_t::userName,
					&ruleExecSubmitInp_t::exeAddress,
					&ruleExecSubmitInp_t::exeTime,
					&ruleExecSubmitInp_t::exeFrequency,
					&ruleExecSubmitInp_t::priority,
					&ruleExecSubmitInp_t::lastExecTime,
					&ruleExecSubmitInp_t::exeStatus,
					&ruleExecSubmitInp_t::estimateExeTime,
					&ruleExecSubmitInp_t::notificationAddr,
					&ruleExecSubmitInp_t::condInput,
					&ruleExecSubmitInp_t::packedReiAndArgBBuf,
					&ruleExecSubmitInp_t::ruleExecId))
			.add_property("ruleName", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->ruleName}; })
			.add_property("reiFilePath", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->reiFilePath}; })
			.add_property("userName", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->userName}; })
			.add_property("exeAddress", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->exeAddress}; })
			.add_property("exeTime", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->exeTime}; })
			.add_property("exeFrequency", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->exeFrequency}; })
			.add_property("priority", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->priority}; })
			.add_property("lastExecTime", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->lastExecTime}; })
			.add_property("exeStatus", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->exeStatus}; })
			.add_property("estimateExeTime", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->estimateExeTime}; })
			.add_property("notificationAddr", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->notificationAddr}; })
			.add_property("condInput", &ruleExecSubmitInp_t::condInput)
			.add_property("packedReiAndArgBBuf",
			              bp::make_getter(&ruleExecSubmitInp_t::packedReiAndArgBBuf, bp::return_internal_reference<1>{}),
			              bp::make_setter(&ruleExecSubmitInp_t::packedReiAndArgBBuf, bp::with_custodian_and_ward<1, 2>{}))
			.add_property("ruleExecId", +[](ruleExecSubmitInp_t *s) { return array_ref<char>{s->ruleExecId}; })
			;
		// clang-format on
	}
}
