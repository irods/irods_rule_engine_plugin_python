// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/check_auth_credentials.hpp"

#include <irods/check_auth_credentials.h>

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
	__attribute__((visibility("hidden"))) void export_CheckAuthCredentialsInput()
	{
		// clang-format off
		bp::class_<CheckAuthCredentialsInput>("CheckAuthCredentialsInput", bp::no_init)
			.def("__init__", make_init_function<CheckAuthCredentialsInput>(
					&CheckAuthCredentialsInput::username,
					&CheckAuthCredentialsInput::zone,
					&CheckAuthCredentialsInput::password))
			.add_property("username", +[](CheckAuthCredentialsInput *s) { return array_ref<char>{s->username}; })
			.add_property("zone", +[](CheckAuthCredentialsInput *s) { return array_ref<char>{s->zone}; })
			.add_property("password", +[](CheckAuthCredentialsInput *s) { return array_ref<char>{s->password}; })
			;
		// clang-format on
	}
} //namespace irods::re::python::types
