// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/guiProgressCallback.hpp"

#include <irods/rodsType.h>
#include <irods/guiProgressCallback.h>

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
	__attribute__((visibility("hidden"))) void export_OperProgress()
	{
		// clang-format off
		bp::class_<operProgress_t>("OperProgress", bp::no_init)
			.def("__init__", make_init_function<operProgress_t>(
					&operProgress_t::oprType,
					&operProgress_t::flag,
					&operProgress_t::totalNumFiles,
					&operProgress_t::totalFileSize,
					&operProgress_t::totalNumFilesDone,
					&operProgress_t::totalFileSizeDone,
					&operProgress_t::curFileName,
					&operProgress_t::curFileSize,
					&operProgress_t::curFileSizeDone))
			.add_property("oprType", &operProgress_t::oprType)
			.add_property("flag", &operProgress_t::flag)
			.add_property("totalNumFiles", &operProgress_t::totalNumFiles)
			.add_property("totalFileSize", &operProgress_t::totalFileSize)
			.add_property("totalNumFilesDone", &operProgress_t::totalNumFilesDone)
			.add_property("totalFileSizeDone", &operProgress_t::totalFileSizeDone)
			.add_property("curFileName", +[](operProgress_t *s) { return array_ref<char>{s->curFileName}; })
			.add_property("curFileSize", &operProgress_t::curFileSize)
			.add_property("curFileSizeDone", &operProgress_t::curFileSizeDone)
			;
		// clang-format on
	}
}
