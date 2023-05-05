// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/fileOpen.hpp"

#include <irods/fileOpen.h>

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
	__attribute__((visibility("hidden"))) void export_FileOpenInp()
	{
		// clang-format off
		bp::class_<fileOpenInp_t>("FileOpenInp", bp::no_init)
			.def("__init__", make_init_function<fileOpenInp_t>(
					&fileOpenInp_t::resc_name_,
					&fileOpenInp_t::resc_hier_,
					&fileOpenInp_t::objPath,
					&fileOpenInp_t::otherFlags,
					&fileOpenInp_t::addr,
					&fileOpenInp_t::fileName,
					&fileOpenInp_t::flags,
					&fileOpenInp_t::mode,
					&fileOpenInp_t::dataSize,
					&fileOpenInp_t::condInput,
					&fileOpenInp_t::in_pdmo))
			.add_property("resc_name", +[](fileOpenInp_t *s) { return array_ref<char>{s->resc_name_}; })
			.add_property("resc_hier", +[](fileOpenInp_t *s) { return array_ref<char>{s->resc_hier_}; })
			.add_property("objPath", +[](fileOpenInp_t *s) { return array_ref<char>{s->objPath}; })
			.add_property("otherFlags", &fileOpenInp_t::otherFlags)
			.add_property("addr", &fileOpenInp_t::addr)
			.add_property("fileName", +[](fileOpenInp_t *s) { return array_ref<char>{s->fileName}; })
			.add_property("flags", &fileOpenInp_t::flags)
			.add_property("mode", &fileOpenInp_t::mode)
			.add_property("dataSize", &fileOpenInp_t::dataSize)
			.add_property("condInput", &fileOpenInp_t::condInput)
			.add_property("in_pdmo", +[](fileOpenInp_t *s) { return array_ref<char>{s->in_pdmo}; })
			;
		// clang-format on
	}
}
