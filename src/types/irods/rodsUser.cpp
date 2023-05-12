// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/rodsUser.hpp"

#include <irods/rodsUser.h>

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
	__attribute__((visibility("hidden"))) void export_AuthInfo()
	{
		// clang-format off
		bp::class_<authInfo_t>("AuthInfo", bp::no_init)
			.def("__init__", make_init_function<authInfo_t>(
					&authInfo_t::authScheme,
					&authInfo_t::authFlag,
					&authInfo_t::flag,
					&authInfo_t::ppid,
					&authInfo_t::host,
					&authInfo_t::authStr))
			.add_property("authScheme", +[](authInfo_t *s) { return array_ref<char>{s->authScheme}; })
			.add_property("authFlag", &authInfo_t::authFlag)
			.add_property("flag", &authInfo_t::flag)
			.add_property("ppid", &authInfo_t::ppid)
			.add_property("host", +[](authInfo_t *s) { return array_ref<char>{s->host}; })
			.add_property("authStr", +[](authInfo_t *s) { return array_ref<char>{s->authStr}; })
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_UserOtherInfo()
	{
		// clang-format off
		bp::class_<userOtherInfo_t>("UserOtherInfo", bp::no_init)
			.def("__init__", make_init_function<userOtherInfo_t>(
					&userOtherInfo_t::userInfo,
					&userOtherInfo_t::userComments,
					&userOtherInfo_t::userCreate,
					&userOtherInfo_t::userModify))
			.add_property("userInfo", +[](userOtherInfo_t *s) { return array_ref<char>{s->userInfo}; })
			.add_property("userComments", +[](userOtherInfo_t *s) { return array_ref<char>{s->userComments}; })
			.add_property("userCreate", +[](userOtherInfo_t *s) { return array_ref<char>{s->userCreate}; })
			.add_property("userModify", +[](userOtherInfo_t *s) { return array_ref<char>{s->userModify}; })
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_UserInfo()
	{
		// clang-format off
		bp::class_<userInfo_t>("UserInfo", bp::no_init)
			.def("__init__", make_init_function<userInfo_t>(
					&userInfo_t::userName,
					&userInfo_t::rodsZone,
					&userInfo_t::userType,
					&userInfo_t::sysUid,
					&userInfo_t::authInfo,
					&userInfo_t::userOtherInfo))
			.add_property("userName", +[](userInfo_t *s) { return array_ref<char>{s->userName}; })
			.add_property("rodsZone", +[](userInfo_t *s) { return array_ref<char>{s->rodsZone}; })
			.add_property("userType", +[](userInfo_t *s) { return array_ref<char>{s->userType}; })
			.add_property("sysUid", &userInfo_t::sysUid)
			.add_property("authInfo", &userInfo_t::authInfo)
			.add_property("userOtherInfo", &userInfo_t::userOtherInfo)
			;
		// clang-format on
	}
} //namespace irods::re::python::types
