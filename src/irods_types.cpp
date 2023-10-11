// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

// include boost/any.hpp before system irods includes to silence boost-caused deprecation warnings
#include <boost/version.hpp>
#if BOOST_VERSION < 108100
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#  include <boost/any.hpp>
#  pragma GCC diagnostic pop
#endif

#include "irods/private/re/python/irods_types.hpp"
#include "irods/private/re/python/types/standard/arrays.hpp"
#include "irods/private/re/python/types/standard/containers.hpp"
#include "irods/private/re/python/types/standard/pointers.hpp"
#include "irods/private/re/python/types/system/netinet/in.hpp"
#include "irods/private/re/python/types/system/sys/stat.hpp"
#include "irods/private/re/python/types/system/sys/time.hpp"
#include "irods/private/re/python/types/irods/bulkDataObjPut.hpp"
#include "irods/private/re/python/types/irods/check_auth_credentials.hpp"
#include "irods/private/re/python/types/irods/dataCopy.hpp"
#include "irods/private/re/python/types/irods/dataObjCopy.hpp"
#include "irods/private/re/python/types/irods/dataObjInpOut.hpp"
#include "irods/private/re/python/types/irods/dataObjOpenAndStat.hpp"
#include "irods/private/re/python/types/irods/execCmd.hpp"
#include "irods/private/re/python/types/irods/fileCreate.hpp"
#include "irods/private/re/python/types/irods/fileLseek.hpp"
#include "irods/private/re/python/types/irods/fileOpen.hpp"
#include "irods/private/re/python/types/irods/fileRename.hpp"
#include "irods/private/re/python/types/irods/getRodsEnv.hpp"
#include "irods/private/re/python/types/irods/guiProgressCallback.hpp"
#include "irods/private/re/python/types/irods/icatStructs.hpp"
#include "irods/private/re/python/types/irods/irods_hierarchy_parser.hpp"
#include "irods/private/re/python/types/irods/irods_plugin_context.hpp"
#include "irods/private/re/python/types/irods/irods_re_structs.hpp"
#include "irods/private/re/python/types/irods/irods_threads.hpp"
#include "irods/private/re/python/types/irods/miscUtil.hpp"
#include "irods/private/re/python/types/irods/msParam.hpp"
#include "irods/private/re/python/types/irods/modAccessControl.hpp"
#include "irods/private/re/python/types/irods/objInfo.hpp"
#include "irods/private/re/python/types/irods/objStat.hpp"
#include "irods/private/re/python/types/irods/rcConnect.hpp"
#include "irods/private/re/python/types/irods/rodsDef.hpp"
#include "irods/private/re/python/types/irods/rodsError.hpp"
#include "irods/private/re/python/types/irods/rodsGeneralUpdate.hpp"
#include "irods/private/re/python/types/irods/rodsGenQuery.hpp"
#include "irods/private/re/python/types/irods/rodsType.hpp"
#include "irods/private/re/python/types/irods/rodsUser.hpp"
#include "irods/private/re/python/types/irods/ruleExecSubmit.hpp"
#include "irods/private/re/python/types/irods/ruleExecDel.hpp"
#include "irods/private/re/python/types/irods/specificQuery.hpp"
#include "irods/private/re/python/types/irods/structFileExtAndReg.hpp"
#include "irods/private/re/python/types/irods/structFileSync.hpp"
#include "irods/private/re/python/types/irods/subStructFileRead.hpp"

#include <patchlevel.h>
#pragma GCC diagnostic push
#if PY_VERSION_HEX < 0x030400A2
#  pragma GCC diagnostic ignored "-Wregister"
#endif
#if PY_VERSION_HEX >= 0x03090000 && BOOST_VERSION < 107500
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <boost/python/module.hpp>
#pragma GCC diagnostic pop

namespace bp = boost::python;
namespace py_types = irods::re::python::types;

namespace
{

	BOOST_PYTHON_MODULE(irods_types)
	{
		py_types::export_specCollClass();
		py_types::export_structFileType();

		py_types::export_objType();

		py_types::export_procState();
		py_types::export_procLogFlag();

		py_types::export_irodsProt();

		py_types::export_fileRestartFlag();
		py_types::export_fileRestartStatus();

		py_types::export_int_array();

		py_types::export_SqlResult_array();

		py_types::export_RErrMsg_pointer_array();

		py_types::export_MsParam_pointer_array();

		py_types::export_char_array();
		py_types::export_unsigned_char_array();
		py_types::export_c_string_array();
		py_types::export_c_string();

		py_types::export_SpecColl();

		py_types::export_RodsObjStat();

		py_types::export_KeyValPair();
		py_types::export_InxIvalPair();
		py_types::export_InxValPair();

		py_types::export_GenQueryInp();
		py_types::export_SqlResult();
		py_types::export_GenQueryOut();

		py_types::export_CollInfo();
		py_types::export_DataObjInfo();

		py_types::export_DataObjInp();

		py_types::export_MsParam();
		py_types::export_MsParamArray();

		py_types::export_ThreadContext();

		py_types::export_PerfStat();

		py_types::export_PortList();
		//py_types::export_DataOprInp();
		py_types::export_PortalOpr();

		py_types::export_RErrMsg();
		py_types::export_RError();

		py_types::export_Version();

		py_types::export_In_Addr();
		py_types::export_Sockaddr_In();

		py_types::export_AuthInfo();
		py_types::export_UserOtherInfo();
		py_types::export_UserInfo();

		py_types::export_RodsEnv();

		py_types::export_RsComm();

		py_types::export_OperProgress();

		py_types::export_TransferStat();

		py_types::export_DataSeg();
		py_types::export_FileRestartInfo();
		py_types::export_FileRestart();
		py_types::export_RcComm();

		py_types::export_RuleExecInfo();

		py_types::export_PluginContext();

		py_types::export_HierarchyParser();

		//py_types::export_VoidPointer();

		py_types::export_Stat();

		py_types::export_BytesBuf();

		py_types::export_RuleExecSubmitInp();

		py_types::export_RodsDirent();

		py_types::export_StringToStringMap();
		py_types::export_IntVector();

		py_types::export_SpecificQueryInp();

		py_types::export_IcatSessionStruct();

		py_types::export_GeneralUpdateInp();

		py_types::export_Timeval();

		py_types::export_MsgHeader();

		py_types::export_BulkOprInp();

		py_types::export_CollInp();

		py_types::export_CollOprStat();

		py_types::export_DataCopyInp();

		py_types::export_DataOprInp();
		py_types::export_PortalOprOut();
		py_types::export_OpenedDataObjInp();

		py_types::export_DataObjCopyInp();

		py_types::export_OpenStat();

		py_types::export_ExecCmd();
		py_types::export_ExecCmdOut();

		py_types::export_FileOpenInp();

		py_types::export_FileCreateOut();

		py_types::export_FileRenameInp();
		py_types::export_FileRenameOut();

		py_types::export_RodsStat();

		py_types::export_RodsHostAddr();

		py_types::export_ModAccessControlInp();

		py_types::export_StructFileExtAndRegInp();

		py_types::export_CollEnt();

		py_types::export_RuleExecDelInp();

		py_types::export_StructFileOprInp();

		py_types::export_SubStructFileFdOprInp();

		py_types::export_SubFile();

		py_types::export_FileLseekInp_t();
		py_types::export_FileLseekOut_t();

		py_types::export_CheckAuthCredentialsInput();
	}
} //namespace
