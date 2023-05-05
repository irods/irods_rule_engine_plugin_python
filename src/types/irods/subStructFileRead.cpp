// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/subStructFileRead.hpp"

#include <irods/subStructFileRead.h>

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
	__attribute__((visibility("hidden"))) void export_SubStructFileFdOprInp()
	{
		// clang-format off
		bp::class_<subStructFileFdOprInp_t>("SubStructFileFdOprInp", bp::no_init)
			.def("__init__", make_init_function<subStructFileFdOprInp_t>(
					&subStructFileFdOprInp_t::addr,
					&subStructFileFdOprInp_t::type,
					&subStructFileFdOprInp_t::fd,
					&subStructFileFdOprInp_t::len,
					&subStructFileFdOprInp_t::resc_hier))
			.add_property("addr", &subStructFileFdOprInp_t::addr)
			.add_property("type", &subStructFileFdOprInp_t::type)
			.add_property("fd", &subStructFileFdOprInp_t::fd)
			.add_property("len", &subStructFileFdOprInp_t::len)
			.add_property("resc_hier", +[](subStructFileFdOprInp_t *s) { return array_ref<char>{s->resc_hier}; })
			;
		// clang-format on
	}
}
