// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/fileRename.hpp"

#include <irods/fileRename.h>

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
	__attribute__((visibility("hidden"))) void export_FileRenameInp()
	{
		// clang-format off
		bp::class_<fileRenameInp_t>("FileRenameInp", bp::no_init)
			.def("__init__", make_init_function<fileRenameInp_t>(
					&fileRenameInp_t::addr,
					&fileRenameInp_t::oldFileName,
					&fileRenameInp_t::newFileName,
					&fileRenameInp_t::rescHier,
					&fileRenameInp_t::objPath))
			.add_property("addr", &fileRenameInp_t::addr)
			.add_property("oldFileName", +[](fileRenameInp_t *s) { return array_ref<char>{s->oldFileName}; })
			.add_property("newFileName", +[](fileRenameInp_t *s) { return array_ref<char>{s->newFileName}; })
			.add_property("rescHier", +[](fileRenameInp_t *s) { return array_ref<char>{s->rescHier}; })
			.add_property("objPath", +[](fileRenameInp_t *s) { return array_ref<char>{s->objPath}; })
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_FileRenameOut()
	{
		// clang-format off
		bp::class_<fileRenameOut_t>("FileRenameOut", bp::no_init)
			.def("__init__", make_init_function<fileRenameOut_t>(
					&fileRenameOut_t::file_name))
			.add_property("file_name", +[](fileRenameOut_t *s) { return array_ref<char>{s->file_name}; })
			;
		// clang-format on
	}
}
