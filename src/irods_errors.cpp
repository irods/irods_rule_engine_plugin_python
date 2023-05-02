// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include <map>
#include <string>

#define MAKE_IRODS_ERROR_MAP
#include <irods/rodsErrorTable.h>
#define MAKE_IRODS_STATE_MAP
#include <irods/irods_state_table.h>

#include <patchlevel.h>
#include <boost/version.hpp>
#pragma GCC diagnostic push
#if PY_VERSION_HEX < 0x030400A2
#  pragma GCC diagnostic ignored "-Wregister"
#endif
#if BOOST_VERSION < 108100
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <boost/python/module.hpp>
#include <boost/python/scope.hpp>
#pragma GCC diagnostic pop

namespace bp = boost::python;

namespace
{
	BOOST_PYTHON_MODULE(irods_errors)
	{
		std::map<std::string, int> irods_constants;
		irods::fill_error_constants(irods_constants);
		irods::fill_state_constants(irods_constants);
		bp::scope current;
		for (const auto& [k, v] : irods_constants) {
			current.attr(k.c_str()) = v;
		}
	}
} //namespace
