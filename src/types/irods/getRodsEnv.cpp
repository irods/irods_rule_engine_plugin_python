// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/getRodsEnv.hpp"

#include <irods/getRodsEnv.h>

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
	__attribute__((visibility("hidden"))) void export_RodsEnv()
	{
		// clang-format off
		bp::class_<rodsEnv>("RodsEnv", bp::no_init)
			.def("__init__", make_init_function<rodsEnv>(
					&rodsEnv::rodsUserName,
					&rodsEnv::rodsHost,
					&rodsEnv::rodsPort,
					&rodsEnv::rodsHome,
					&rodsEnv::rodsCwd,
					&rodsEnv::rodsAuthScheme,
					&rodsEnv::rodsDefResource,
					&rodsEnv::rodsZone,
					&rodsEnv::rodsLogLevel,
					&rodsEnv::rodsAuthFile,
					&rodsEnv::rodsClientServerPolicy,
					&rodsEnv::rodsClientServerNegotiation,
					&rodsEnv::rodsEncryptionKeySize,
					&rodsEnv::rodsEncryptionSaltSize,
					&rodsEnv::rodsEncryptionNumHashRounds,
					&rodsEnv::rodsEncryptionAlgorithm,
					&rodsEnv::rodsDefaultHashScheme,
					&rodsEnv::rodsMatchHashPolicy,
					&rodsEnv::irodsSSLCACertificatePath,
					&rodsEnv::irodsSSLCACertificateFile,
					&rodsEnv::irodsSSLVerifyServer,
					&rodsEnv::irodsCtrlPlaneKey,
					&rodsEnv::irodsCtrlPlanePort,
					&rodsEnv::irodsCtrlPlaneEncryptionNumHashRounds,
					&rodsEnv::irodsCtrlPlaneEncryptionAlgorithm,
					&rodsEnv::irodsMaxSizeForSingleBuffer,
					&rodsEnv::irodsDefaultNumberTransferThreads,
					&rodsEnv::irodsTransBufferSizeForParaTrans,
					&rodsEnv::irodsConnectionPoolRefreshTime,
					&rodsEnv::irodsPluginHome,
					&rodsEnv::tcp_keepalive_intvl,
					&rodsEnv::tcp_keepalive_probes,
					&rodsEnv::tcp_keepalive_time))
			.add_property("rodsUserName", +[](rodsEnv *s) { return array_ref<char>{s->rodsUserName}; })
			.add_property("rodsHost", +[](rodsEnv *s) { return array_ref<char>{s->rodsHost}; })
			.add_property("rodsPort", &rodsEnv::rodsPort)
			.add_property("rodsHome", +[](rodsEnv *s) { return array_ref<char>{s->rodsHome}; })
			.add_property("rodsCwd", +[](rodsEnv *s) { return array_ref<char>{s->rodsCwd}; })
			.add_property("rodsAuthScheme", +[](rodsEnv *s) { return array_ref<char>{s->rodsAuthScheme}; })
			.add_property("rodsDefResource", +[](rodsEnv *s) { return array_ref<char>{s->rodsDefResource}; })
			.add_property("rodsZone", +[](rodsEnv *s) { return array_ref<char>{s->rodsZone}; })
			.add_property("rodsLogLevel", &rodsEnv::rodsLogLevel)
			.add_property("rodsAuthFile", +[](rodsEnv *s) { return array_ref<char>{s->rodsAuthFile}; })
			.add_property("rodsClientServerPolicy", +[](rodsEnv *s) { return array_ref<char>{s->rodsClientServerPolicy}; })
			.add_property("rodsClientServerNegotiation", +[](rodsEnv *s) { return array_ref<char>{s->rodsClientServerNegotiation}; })
			.add_property("rodsEncryptionKeySize", &rodsEnv::rodsEncryptionKeySize)
			.add_property("rodsEncryptionSaltSize", &rodsEnv::rodsEncryptionSaltSize)
			.add_property("rodsEncryptionNumHashRounds", &rodsEnv::rodsEncryptionNumHashRounds)
			.add_property("rodsEncryptionAlgorithm", +[](rodsEnv *s) { return array_ref<char>{s->rodsEncryptionAlgorithm}; })
			.add_property("rodsDefaultHashScheme", +[](rodsEnv *s) { return array_ref<char>{s->rodsDefaultHashScheme}; })
			.add_property("rodsMatchHashPolicy", +[](rodsEnv *s) { return array_ref<char>{s->rodsMatchHashPolicy}; })
			.add_property("irodsSSLCACertificatePath", +[](rodsEnv *s) { return array_ref<char>{s->irodsSSLCACertificatePath}; })
			.add_property("irodsSSLCACertificateFile", +[](rodsEnv *s) { return array_ref<char>{s->irodsSSLCACertificateFile}; })
			.add_property("irodsSSLVerifyServer", +[](rodsEnv *s) { return array_ref<char>{s->irodsSSLVerifyServer}; })
			.add_property("irodsCtrlPlaneKey", +[](rodsEnv *s) { return array_ref<char>{s->irodsCtrlPlaneKey}; })
			.add_property("irodsCtrlPlanePort", &rodsEnv::irodsCtrlPlanePort)
			.add_property("irodsCtrlPlaneEncryptionNumHashRounds", &rodsEnv::irodsCtrlPlaneEncryptionNumHashRounds)
			.add_property("irodsCtrlPlaneEncryptionAlgorithm", +[](rodsEnv *s) { return array_ref<char>{s->irodsCtrlPlaneEncryptionAlgorithm}; })
			.add_property("irodsMaxSizeForSingleBuffer", &rodsEnv::irodsMaxSizeForSingleBuffer)
			.add_property("irodsDefaultNumberTransferThreads", &rodsEnv::irodsDefaultNumberTransferThreads)
			.add_property("irodsTransBufferSizeForParaTrans", &rodsEnv::irodsTransBufferSizeForParaTrans)
			.add_property("irodsConnectionPoolRefreshTime", &rodsEnv::irodsConnectionPoolRefreshTime)
			.add_property("irodsPluginHome", +[](rodsEnv *s) { return array_ref<char>{s->irodsPluginHome}; })
			.add_property("tcp_keepalive_intvl", &rodsEnv::tcp_keepalive_intvl)
			.add_property("tcp_keepalive_probes", &rodsEnv::tcp_keepalive_probes)
			.add_property("tcp_keepalive_time", &rodsEnv::tcp_keepalive_time)
			;
		// clang-format on
	}
} //namespace irods::re::python::types
