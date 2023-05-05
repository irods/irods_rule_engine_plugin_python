// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/execCmd.hpp"

#include <irods/execCmd.h>

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
	__attribute__((visibility("hidden"))) void export_ExecCmd()
	{
		// clang-format off
		bp::class_<execCmd_t>("ExecCmd", bp::no_init)
			.def("__init__", make_init_function<execCmd_t>(
					&execCmd_t::cmd,
					&execCmd_t::cmdArgv,
					&execCmd_t::execAddr,
					&execCmd_t::hintPath,
					&execCmd_t::addPathToArgv,
					&execCmd_t::dummy,
					&execCmd_t::condInput))
			.add_property("cmd", +[](execCmd_t *s) { return array_ref<char>{s->cmd}; })
			.add_property("cmdArgv", +[](execCmd_t *s) { return array_ref<char>{s->cmdArgv}; })
			.add_property("execAddr", +[](execCmd_t *s) { return array_ref<char>{s->execAddr}; })
			.add_property("hintPath", +[](execCmd_t *s) { return array_ref<char>{s->hintPath}; })
			.add_property("addPathToArgv", &execCmd_t::addPathToArgv)
			.add_property("dummy", &execCmd_t::dummy)
			.add_property("condInput", &execCmd_t::condInput)
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_ExecCmdOut()
	{
		// clang-format off
		bp::class_<execCmdOut_t>("ExecCmdOut", bp::no_init)
			.def("__init__", make_init_function<execCmdOut_t>(
					&execCmdOut_t::stdoutBuf,
					&execCmdOut_t::stderrBuf,
					&execCmdOut_t::status))
			.add_property("stdoutBuf", &execCmdOut_t::stdoutBuf)
			.add_property("stderrBuf", &execCmdOut_t::stderrBuf)
			.add_property("status", &execCmdOut_t::status)
			;
		// clang-format on
	}
}
