// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/fileLseek.hpp"

#include <irods/fileLseek.h>

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
	__attribute__((visibility("hidden"))) void export_FileLseekInp_t()
	{
		// clang-format off
		bp::class_<fileLseekInp_t>("FileLseekInp_t", bp::no_init)
			.def("__init__", make_init_function<fileLseekInp_t>(
					&fileLseekInp_t::fileInx,
					&fileLseekInp_t::offset,
					&fileLseekInp_t::whence))
			.add_property("fileInx", &fileLseekInp_t::fileInx)
			.add_property("offset", &fileLseekInp_t::offset)
			.add_property("whence", &fileLseekInp_t::whence)
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_FileLseekOut_t()
	{
		// clang-format off
		bp::class_<fileLseekOut_t>("FileLseekOut_t", bp::no_init)
			.def("__init__", make_init_function<fileLseekOut_t>(
					&fileLseekOut_t::offset))
			.add_property("offset", &fileLseekOut_t::offset)
			;
		// clang-format on
	}
}
