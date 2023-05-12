// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/irods_plugin_context.hpp"

// include boost/any.hpp before system irods includes to silence boost-caused deprecation warnings
#include <boost/version.hpp>
#if BOOST_VERSION < 108100
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#  include <boost/any.hpp>
#  pragma GCC diagnostic pop
#endif

#include <irods/irods_plugin_context.hpp>
#include <irods/irods_re_serialization.hpp>

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
#include <boost/python/dict.hpp>
#pragma GCC diagnostic pop

namespace bp = boost::python;

namespace irods::re::python::types
{
	__attribute__((visibility("hidden"))) void export_PluginContext()
	{
		// clang-format off
		bp::class_<irods::plugin_context>("PluginContext", bp::no_init)
			// prop_map contains boost::any, don't directly support that in python
			//.add_property("__getitem__", +[](irods::plugin_context* _ctx, const std::string& key) { return _ctx.prop_map().get(key) }
			//.add_property("__setitem__", +[](irods::plugin_context* _ctx, const std::string& key) { return _ctx.prop_map().set(key) }
			.add_property("rule_results", +[](irods::plugin_context* _ctx) { return _ctx->rule_results(); })
			.def("map", +[](irods::plugin_context* _ctx) {
					bp::dict x;
					if (auto fco = _ctx->fco(); fco.get() != nullptr) {
						irods::rule_engine_vars_t vars_map;
						fco->get_re_vars(vars_map);
						for (auto &pr : vars_map) { x[pr.first]=pr.second; }
					}
					irods::re_serialization::serialized_parameter_t comm_map;
					auto err = irods::re_serialization::serialize_parameter(boost::any{ _ctx->comm() }, comm_map);
					if (err.ok()) {
						for (auto &pr : comm_map) { x[pr.first]=pr.second; }
					}
					return x;
				})
			;
		// clang-format on
	}
} //namespace irods::re::python::types
