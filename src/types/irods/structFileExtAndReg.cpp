// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/structFileExtAndReg.hpp"

#include <irods/structFileExtAndReg.h>

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
	__attribute__((visibility("hidden"))) void export_StructFileExtAndRegInp()
	{
		// clang-format off
		bp::class_<structFileExtAndRegInp_t>("StructFileExtAndRegInp", bp::no_init)
			.def("__init__", make_init_function<structFileExtAndRegInp_t>(
					&structFileExtAndRegInp_t::objPath,
					&structFileExtAndRegInp_t::collection,
					&structFileExtAndRegInp_t::oprType,
					&structFileExtAndRegInp_t::flags,
					&structFileExtAndRegInp_t::condInput))
			.add_property("objPath", +[](structFileExtAndRegInp_t *s) { return array_ref<char>{s->objPath}; })
			.add_property("collection", +[](structFileExtAndRegInp_t *s) { return array_ref<char>{s->collection}; })
			.add_property("oprType", &structFileExtAndRegInp_t::oprType)
			.add_property("flags", &structFileExtAndRegInp_t::flags)
			.add_property("condInput", &structFileExtAndRegInp_t::condInput)
			;
		// clang-format on
	}
} //namespace irods::re::python::types
