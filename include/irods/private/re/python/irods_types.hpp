#ifndef IRODS_TYPES_HPP__
#define IRODS_TYPES_HPP__

// include this first to fix macro redef warnings
#include <pyconfig.h>

#include <map>
#include <string>
#include <vector>

#include <irods/rodsUser.h>
#include <irods/bulkDataObjPut.h>
#include <irods/miscUtil.h>
#include <irods/dataCopy.h>
#include <irods/dataObjCopy.h>
#include <irods/execCmd.h>
#include <irods/fileCreate.h>
#include <irods/fileLseek.h>
#include <irods/fileOpen.h>
#include <irods/fileRename.h>
#include <irods/rcConnect.h>
#include <irods/rodsGeneralUpdate.h>
#include <irods/icatStructs.hpp>
#include <irods/irods_hierarchy_parser.hpp>
#include <irods/irods_plugin_context.hpp>
#include <irods/modAccessControl.h>
#include <irods/msParam.h>
#include <irods/dataObjOpenAndStat.h>
#include <irods/ruleExecDel.h>
#include <irods/irods_re_structs.hpp>
#include <irods/ruleExecSubmit.h>
#include <irods/specificQuery.h>
#include <irods/structFileSync.h>
#include <irods/subStructFileRead.h>
#include <irods/irods_threads.hpp>

#include "irods/private/re/python/types/type_sequence.hpp"

#if PY_VERSION_HEX < 0x03000000
extern "C" void initirods_types();
#else
#  include <Python.h>
extern "C" PyObject* PyInit_irods_types();
#endif

void init_irods_types();

// clang-format off
using list_of_irods_types = type_sequence<
	authInfo_t,
	bulkOprInp_t,
	bytesBuf_t,
	char,
	collEnt_t,
	collInfo_t,
	collInp_t,
	collOprStat_t,
	dataCopyInp_t,
	dataObjCopyInp_t,
	dataObjInfo_t,
	dataObjInp_t,
	//dataOprInp_t,
	dataSeg_t,
	execCmdOut_t,
	execCmd_t,
	fileCreateOut_t,
	fileLseekInp_t,
	fileLseekOut_t,
	fileOpenInp_t,
	fileRenameInp_t,
	fileRenameOut_t,
	fileRestartInfo_t,
	fileRestart_t,
	float,
	genQueryInp_t,
	genQueryOut_t,
	generalUpdateInp_t,
	icatSessionStruct,
	in_addr,
	int,
	inxIvalPair_t,
	inxValPair_t,
	irods::hierarchy_parser,
	irods::plugin_context,
	keyValPair_t,
	modAccessControlInp_t,
	msParamArray_t,
	msParam_t,
	msgHeader_t,
	openStat_t,
	openedDataObjInp_t,
	operProgress_t,
	perfStat_t,
	portList_t,
	portalOprOut_t,
	portalOpr_t,
	rErrMsg_t,
	rError_t,
	rcComm_t,
	rodsDirent_t,
	rodsEnv,
	rodsHostAddr_t,
	rodsLong_t,
	rodsObjStat_t,
	rodsStat_t,
	rsComm_t,
	ruleExecDelInp_t,
	ruleExecInfo_t,
	ruleExecSubmitInp_t,
	sockaddr_in,
	specColl_t,
	specificQueryInp_t,
	sqlResult_t,
	std::map<std::string, std::string>,
	std::vector<int>,
	std::string,
	struct stat,
	struct timeval,
	structFileExtAndRegInp_t,
	structFileOprInp_t,
	subFile_t,
	subStructFileFdOprInp_t,
	transferStat_t,
	userInfo_t,
	userOtherInfo_t,
	version_t
>;
// clang-format on

#endif //IRODS_TYPES_HPP__
