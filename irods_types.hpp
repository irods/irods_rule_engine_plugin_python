#ifndef IRODS_TYPES_HPP__
#define IRODS_TYPES_HPP__

#include <map>
#include <string>
#include <vector>

#include "rodsUser.h"
#include "bulkDataObjPut.h"
#include "miscUtil.h"
#include "dataCopy.h"
#include "dataObjCopy.h"
#include "execCmd.h"
#include "fileCreate.h"
#include "fileLseek.h"
#include "fileOpen.h"
#include "fileRename.h"
#include "rcConnect.h"
#include "rodsGeneralUpdate.h"
#include "icatStructs.hpp"
#include "irods_hierarchy_parser.hpp"
#include "irods_plugin_context.hpp"
#include "modAccessControl.h"
#include "msParam.h"
#include "dataObjOpenAndStat.h"
#include "ruleExecDel.h"
#include "irods_re_structs.hpp"
#include "ruleExecSubmit.h"
#include "specificQuery.h"
#include "structFileSync.h"
#include "subStructFileRead.h"

#include "type_sequence.hpp"
#include "irods_threads.hpp"

extern "C" void initirods_types();
void init_irods_types();

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
        getXmsgTicketInp_t,
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
        rcvXmsgInp_t,
        rcvXmsgOut_t,
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
        sendXmsgInp_t,
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
        version_t,
        //void*,
        xmsgTicketInfo_t>;

#endif //IRODS_TYPES_HPP__
