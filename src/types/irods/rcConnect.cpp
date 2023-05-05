// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/rcConnect.hpp"

// include boost/thread.hpp before system irods includes to silence boost-caused deprecation warnings
#include <boost/version.hpp>
#if BOOST_VERSION < 108100
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <boost/thread.hpp>
#pragma GCC diagnostic pop
#endif

#include <irods/rcConnect.h>
#include <irods/irods_threads.hpp>

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
#include <boost/python/enum.hpp>
#pragma GCC diagnostic pop

namespace bp = boost::python;

namespace irods::re::python::types
{
	__attribute__((visibility("hidden"))) void export_procState()
	{
		// clang-format off
		bp::enum_<procState_t>("procState")
			.value("PROCESSING_STATE", procState_t::PROCESSING_STATE)
			.value("RECEIVING_STATE", procState_t::RECEIVING_STATE)
			.value("SENDING_STATE", procState_t::SENDING_STATE)
			.value("CONN_WAIT_STATE", procState_t::CONN_WAIT_STATE)
			.export_values();
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_DataSeg()
	{
		// clang-format off
		bp::class_<dataSeg_t>("DataSeg", bp::no_init)
			.def("__init__", make_init_function<dataSeg_t>(
					&dataSeg_t::len,
					&dataSeg_t::offset))
			.add_property("len", &dataSeg_t::len)
			.add_property("offset", &dataSeg_t::offset)
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_fileRestartFlag()
	{
		// clang-format off
		bp::enum_<fileRestartFlag_t>("fileRestartFlag")
			.value("FILE_RESTART_OFF", fileRestartFlag_t::FILE_RESTART_OFF)
			.value("FILE_RESTART_ON", fileRestartFlag_t::FILE_RESTART_ON)
			.export_values();
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_fileRestartStatus()
	{
		// clang-format off
		bp::enum_<fileRestartStatus_t>("fileRestartStatus")
			.value("FILE_NOT_RESTART", fileRestartStatus_t::FILE_NOT_RESTART)
			.value("FILE_RESTARTED", fileRestartStatus_t::FILE_RESTARTED)
			.export_values();
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_FileRestartInfo()
	{
		// clang-format off
		bp::class_<fileRestartInfo_t>("FileRestartInfo", bp::no_init)
			.def("__init__", make_init_function<fileRestartInfo_t>(
					&fileRestartInfo_t::fileName,
					&fileRestartInfo_t::objPath,
					&fileRestartInfo_t::numSeg,
					&fileRestartInfo_t::status,
					&fileRestartInfo_t::fileSize,
					&fileRestartInfo_t::dataSeg))
			.add_property("fileName", +[](fileRestartInfo_t *s) { return array_ref<char>{s->fileName}; })
			.add_property("objPath", +[](fileRestartInfo_t *s) { return array_ref<char>{s->objPath}; })
			.add_property("numSeg", &fileRestartInfo_t::numSeg)
			.add_property("status", &fileRestartInfo_t::status)
			.add_property("fileSize", &fileRestartInfo_t::fileSize)
			.add_property("dataSeg", +[](fileRestartInfo_t *s) { return array_ref<dataSeg_t>{s->dataSeg}; })
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_FileRestart()
	{
		// clang-format off
		bp::class_<fileRestart_t>("FileRestart", bp::no_init)
			.def("__init__", make_init_function<fileRestart_t>(
					&fileRestart_t::flags,
					&fileRestart_t::writtenSinceUpdated,
					&fileRestart_t::infoFile,
					&fileRestart_t::info))
			.add_property("flags", &fileRestart_t::flags)
			.add_property("writtenSinceUpdated", &fileRestart_t::writtenSinceUpdated)
			.add_property("infoFile", +[](fileRestart_t *s) { return array_ref<char>{s->infoFile}; })
			.add_property("info", &fileRestart_t::info)
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_procLogFlag()
	{
		// clang-format off
		bp::enum_<procLogFlag_t>("procLogFlag")
			.value("PROC_LOG_NOT_DONE", procLogFlag_t::PROC_LOG_NOT_DONE)
			.value("PROC_LOG_DONE", procLogFlag_t::PROC_LOG_DONE)
			.export_values();
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_RcComm()
	{
		// clang-format off
		bp::class_<rcComm_t>("RcComm", bp::no_init)
			.def("__init__", make_init_function<rcComm_t>(
					&rcComm_t::irodsProt,
					&rcComm_t::host,
					&rcComm_t::sock,
					&rcComm_t::portNum,
					&rcComm_t::loggedIn,
					&rcComm_t::localAddr,
					&rcComm_t::remoteAddr,
					&rcComm_t::proxyUser,
					&rcComm_t::clientUser,
					&rcComm_t::svrVersion,
					&rcComm_t::rError,
					&rcComm_t::flag,
					&rcComm_t::transStat,
					&rcComm_t::apiInx,
					&rcComm_t::status,
					&rcComm_t::windowSize,
					&rcComm_t::reconnectedSock,
					&rcComm_t::reconnTime,
					&rcComm_t::exit_flg,
					&rcComm_t::thread_ctx,
					&rcComm_t::agentState,
					&rcComm_t::clientState,
					&rcComm_t::reconnThrState,
					&rcComm_t::operProgress,
					&rcComm_t::key_size,
					&rcComm_t::salt_size,
					&rcComm_t::num_hash_rounds,
					&rcComm_t::encryption_algorithm,
					&rcComm_t::negotiation_results,
					&rcComm_t::shared_secret,
					&rcComm_t::ssl_on,
					&rcComm_t::ssl_ctx,
					&rcComm_t::ssl,
					&rcComm_t::fileRestart))
			.add_property("irodsProt", &rcComm_t::irodsProt)
			.add_property("host", +[](rcComm_t *s) { return array_ref<char>{s->host}; })
			.add_property("sock", &rcComm_t::sock)
			.add_property("portNum", &rcComm_t::portNum)
			.add_property("loggedIn", &rcComm_t::loggedIn)
			.add_property("localAddr", &rcComm_t::localAddr)
			.add_property("remoteAddr", &rcComm_t::remoteAddr)
			.add_property("proxyUser", &rcComm_t::proxyUser)
			.add_property("clientUser", &rcComm_t::clientUser)
			.add_property("svrVersion",
			              bp::make_getter(&rcComm_t::svrVersion, bp::return_internal_reference<1>{}),
			              bp::make_setter(&rcComm_t::svrVersion, bp::with_custodian_and_ward<1, 2>{}))
			.add_property("rError",
			              bp::make_getter(&rcComm_t::rError, bp::return_internal_reference<1>{}),
			              bp::make_setter(&rcComm_t::rError, bp::with_custodian_and_ward<1, 2>{}))
			.add_property("flag", &rcComm_t::flag)
			.add_property("transStat", &rcComm_t::transStat)
			.add_property("apiInx", &rcComm_t::apiInx)
			.add_property("status", &rcComm_t::status)
			.add_property("windowSize", &rcComm_t::windowSize)
			.add_property("reconnectedSock", &rcComm_t::reconnectedSock)
			.add_property("reconnTime", &rcComm_t::reconnTime)
			.add_property("exit_flg", &rcComm_t::exit_flg)
			.add_property("thread_ctx",
			              bp::make_getter(&rcComm_t::thread_ctx, bp::return_internal_reference<1>{}),
			              bp::make_setter(&rcComm_t::thread_ctx, bp::with_custodian_and_ward<1, 2>{}))
			.add_property("agentState", &rcComm_t::agentState)
			.add_property("clientState", &rcComm_t::clientState)
			.add_property("reconnThrState", &rcComm_t::reconnThrState)
			.add_property("operProgress", &rcComm_t::operProgress)
			.add_property("key_size", &rcComm_t::key_size)
			.add_property("salt_size", &rcComm_t::salt_size)
			.add_property("num_hash_rounds", &rcComm_t::num_hash_rounds)
			.add_property("encryption_algorithm", +[](rcComm_t *s) { return array_ref<char>{s->encryption_algorithm}; })
			.add_property("negotiation_results", +[](rcComm_t *s) { return array_ref<char>{s->negotiation_results}; })
			.add_property("shared_secret", +[](rcComm_t *s) { return array_ref<unsigned char>{s->shared_secret}; })
			.add_property("ssl_on", &rcComm_t::ssl_on)
			// No accessors for the ssl types, as this would require exposing much of the internals of ssl
			//.add_property("ssl_ctx",
			//              bp::make_getter(&rcComm_t::ssl_ctx, bp::return_internal_reference<1>{}),
			//              bp::make_setter(&rcComm_t::ssl_ctx, bp::with_custodian_and_ward<1, 2>{}))
			//.add_property("ssl",
			//              bp::make_getter(&rcComm_t::ssl, bp::return_internal_reference<1>{}),
			//              bp::make_setter(&rcComm_t::ssl, bp::with_custodian_and_ward<1, 2>{}))
			.add_property("fileRestart", &rcComm_t::fileRestart)
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_PerfStat()
	{
		// clang-format off
		bp::class_<perfStat_t>("PerfStat", bp::no_init)
			.def("__init__", make_init_function<perfStat_t>(
					&perfStat_t::orphanCnt,
					&perfStat_t::nonOrphanCnt))
			.add_property("orphanCnt", &perfStat_t::orphanCnt)
			.add_property("nonOrphanCnt", &perfStat_t::nonOrphanCnt)
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_RsComm()
	{
		// clang-format off
		bp::class_<rsComm_t>("RsComm", bp::no_init)
			.def("__init__", make_init_function<rsComm_t>(
					&rsComm_t::irodsProt,
					&rsComm_t::sock,
					&rsComm_t::connectCnt,
					&rsComm_t::localAddr,
					&rsComm_t::remoteAddr,
					&rsComm_t::clientAddr,
					&rsComm_t::proxyUser,
					&rsComm_t::clientUser,
					&rsComm_t::myEnv,
					&rsComm_t::cliVersion,
					&rsComm_t::option,
					&rsComm_t::procLogFlag,
					&rsComm_t::rError,
					&rsComm_t::portalOpr,
					&rsComm_t::apiInx,
					&rsComm_t::status,
					&rsComm_t::perfStat,
					&rsComm_t::windowSize,
					&rsComm_t::reconnFlag,
					&rsComm_t::reconnSock,
					&rsComm_t::reconnPort,
					&rsComm_t::reconnectedSock,
					&rsComm_t::reconnAddr,
					&rsComm_t::cookie,
					&rsComm_t::thread_ctx,
					&rsComm_t::agentState,
					&rsComm_t::clientState,
					&rsComm_t::reconnThrState,
					&rsComm_t::gsiRequest,
					&rsComm_t::auth_scheme,
					&rsComm_t::ssl_on,
					&rsComm_t::ssl_ctx,
					&rsComm_t::ssl,
					&rsComm_t::ssl_do_accept,
					&rsComm_t::ssl_do_shutdown,
					&rsComm_t::negotiation_results,
					&rsComm_t::shared_secret,
					&rsComm_t::key_size,
					&rsComm_t::salt_size,
					&rsComm_t::num_hash_rounds,
					&rsComm_t::encryption_algorithm))
			.add_property("irodsProt", &rsComm_t::irodsProt)
			.add_property("sock", &rsComm_t::sock)
			.add_property("connectCnt", &rsComm_t::connectCnt)
			.add_property("localAddr", &rsComm_t::localAddr)
			.add_property("remoteAddr", &rsComm_t::remoteAddr)
			.add_property("clientAddr", +[](rsComm_t *s) { return array_ref<char>{s->clientAddr}; })
			.add_property("proxyUser", &rsComm_t::proxyUser)
			.add_property("clientUser", &rsComm_t::clientUser)
			.add_property("myEnv", &rsComm_t::myEnv)
			.add_property("cliVersion", &rsComm_t::cliVersion)
			.add_property("option", +[](rsComm_t *s) { return array_ref<char>{s->option}; })
			.add_property("procLogFlag", &rsComm_t::procLogFlag)
			.add_property("rError", &rsComm_t::rError)
			.add_property("portalOpr",
			              bp::make_getter(&rsComm_t::portalOpr, bp::return_internal_reference<1>{}),
			              bp::make_setter(&rsComm_t::portalOpr, bp::with_custodian_and_ward<1, 2>{}))
			.add_property("apiInx", &rsComm_t::apiInx)
			.add_property("status", &rsComm_t::status)
			.add_property("perfStat", &rsComm_t::perfStat)
			.add_property("windowSize", &rsComm_t::windowSize)
			.add_property("reconnFlag", &rsComm_t::reconnFlag)
			.add_property("reconnSock", &rsComm_t::reconnSock)
			.add_property("reconnPort", &rsComm_t::reconnPort)
			.add_property("reconnectedSock", &rsComm_t::reconnectedSock)
			.add_property("reconnAddr", +[](rsComm_t *s) { return array_ref<char>{s->clientAddr}; })
			.add_property("cookie", &rsComm_t::cookie)
			.add_property("thread_ctx",
			              bp::make_getter(&rsComm_t::thread_ctx, bp::return_internal_reference<1>{}),
			              bp::make_setter(&rsComm_t::thread_ctx, bp::with_custodian_and_ward<1, 2>{}))
			.add_property("agentState", &rsComm_t::agentState)
			.add_property("clientState", &rsComm_t::clientState)
			.add_property("reconnThrState", &rsComm_t::reconnThrState)
			.add_property("gsiRequest", &rsComm_t::gsiRequest)
			.add_property("auth_scheme", +[](rsComm_t *s) { return array_ref<char, true>{s->auth_scheme}; })
			.add_property("ssl_on", &rsComm_t::ssl_on)
			// No accessors for the ssl types, as this would require exposing much of the internals of ssl
			//.add_property("ssl_ctx",
			//              bp::make_getter(&rsComm_t::ssl_ctx, bp::return_internal_reference<1>{}),
			//              bp::make_setter(&rsComm_t::ssl_ctx, bp::with_custodian_and_ward<1, 2>{}))
			//.add_property("ssl",
			//              bp::make_getter(&rsComm_t::ssl, bp::return_internal_reference<1>{}),
			//              bp::make_setter(&rsComm_t::ssl, bp::with_custodian_and_ward<1, 2>{}))
			.add_property("ssl_do_accept", &rsComm_t::ssl_on)
			.add_property("ssl_do_shutdown", &rsComm_t::ssl_on)
			.add_property("negotiation_results", +[](rsComm_t *s) { return array_ref<char>{s->negotiation_results}; })
			.add_property("shared_secret", +[](rsComm_t *s) { return array_ref<unsigned char>{s->shared_secret}; })
			.add_property("key_size", &rsComm_t::key_size)
			.add_property("salt_size", &rsComm_t::salt_size)
			.add_property("num_hash_rounds", &rsComm_t::num_hash_rounds)
			.add_property("encryption_algorithm", +[](rsComm_t *s) { return array_ref<char>{s->encryption_algorithm}; })
			;
		// clang-format on
	}
}
