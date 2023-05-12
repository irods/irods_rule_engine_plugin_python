// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/modAccessControl.hpp"

#include <irods/modAccessControl.h>

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
	__attribute__((visibility("hidden"))) void export_ModAccessControlInp()
	{
		// clang-format off
		bp::class_<modAccessControlInp_t>("ModAccessControlInp", bp::no_init)
			.def("__init__", make_init_function<modAccessControlInp_t>(
					&modAccessControlInp_t::recursiveFlag,
					&modAccessControlInp_t::accessLevel,
					&modAccessControlInp_t::userName,
					&modAccessControlInp_t::zone,
					&modAccessControlInp_t::path))
			.add_property("recursiveFlag", &modAccessControlInp_t::recursiveFlag)
			.add_property("accessLevel", +[](modAccessControlInp_t *s) { return array_ref<char, true>{s->accessLevel}; })
			.add_property("userName", +[](modAccessControlInp_t *s) { return array_ref<char, true>{s->userName}; })
			.add_property("zone", +[](modAccessControlInp_t *s) { return array_ref<char, true>{s->zone}; })
			.add_property("path", +[](modAccessControlInp_t *s) { return array_ref<char, true>{s->path}; })
			;
		// clang-format on
	}
} //namespace irods::re::python::types
