import datetime
import json

def pythonRuleEnginePluginTest(rule_args, callback):
    with open('/tmp/from_core_py.txt', 'a') as f:
        f.write(str(datetime.datetime.now()))
        f.write('\n')
        c = 0
        for arg in rule_args:
            f.write('\t')
            f.write(str(c))
            f.write(' : ')
            f.write(str(arg))
            f.write('\n')
            c = c +1
    callback.writeLine('serverLog', 'Printed to server log from python rule engine')

def acPreConnect(rule_args, callback):
    rule_args[0] = 'CS_NEG_DONT_CARE'

def acCreateUser(rule_args, callback):
    ret = callback.msiCreateUser()

    if not ret[PYTHON_RE_RET_STATUS]:
        callback.msiRollback()
        return ret

    ret = acCreateDefaultCollections(rule_args, callback)

    if not ret[PYTHON_RE_RET_STATUS]:
        callback.msiRollback()
        return ret

    ret = callback.msiAddUserToGroup('public')

    if not ret[PYTHON_RE_RET_STATUS]:
        callback.msiRollback()
        return ret

    callback.msiCommit()

def acCreateDefaultCollections(rule_args, callback):
    ret = acCreateUserZoneCollections(rule_args, callback)
    return ret

def acCreateUserZoneCollections(rule_args, callback):
    tmpStr = ''
    ret = callback.getSessionVar('rodsZoneProxy', tmpStr)
    rodsZoneProxy = str(ret[PYTHON_RE_RET_OUTPUT][1])
    ret = callback.getSessionVar('otherUserName', tmpStr)
    otherUserName = str(ret[PYTHON_RE_RET_OUTPUT][1])
    homeStr = '/' + rodsZoneProxy + '/home'
    trashStr = '/' + rodsZoneProxy + '/trash/home'

    homeDict = []
    homeDict.append(homeStr)
    homeDict.append(otherUserName)

    trashDict = []
    trashDict.append(trashStr)
    trashDict.append(otherUserName)
    
    ret = acCreateCollByAdmin(homeDict, callback)
    ret = acCreateCollByAdmin(trashDict, callback)
    return ret

def acCreateCollByAdmin(rule_args, callback):
    ret = callback.msiCreateCollByAdmin(rule_args[0], rule_args[1])
    return ret

def acDeleteUser(rule_args, callback):
    ret = callback.acDeleteDefaultCollections()

    if not ret[PYTHON_RE_RET_STATUS]:
        callback.msiRollback()
        return ret

    ret = callback.msiDeleteUser()
    
    if not ret[PYTHON_RE_RET_STATUS]:
        callback.msiRollback()
        return ret

    callback.msiCommit()

def acDeleteDefaultCollections(rule_args, callback):
    ret = acDeleteUserZoneCollections(rule_args, callback)
    return ret

def acDeleteUserZoneCollections(rule_args, callback):
    tmpStr = ''
    ret = callback.getSessionVar('rodsZoneProxy', tmpStr)
    rodsZoneProxy = ret[PYTHON_RE_RET_OUTPUT][1]
    ret = callback.getSessionVar('otherUserName', tmpStr)
    otherUserName = ret[PYTHON_RE_RET_OUTPUT][1]
    homeStr = '/' + rodsZoneProxy + '/home'
    trashStr = '/' + rodsZoneProxy + '/trash/home'
    
    homeDict = []
    homeDict.append(homeStr)
    homeDict.append(otherUserName)

    trashDict = []
    trashDict.append(trashStr)
    trashDict.append(otherUserName)

    ret = acDeleteCollByAdminIfPresent(homeDict, callback)
    ret = acDeleteCollByAdminIfPresent(trashDict, callback)
    return ret

def acDeleteCollByAdminIfPresent(rule_args, callback):
    ret = callback.msiDeleteCollByAdmin(rule_args[0], rule_args[1])

    if not ret[PYTHON_RE_RET_STATUS]:
        if not ret[PYTHON_RE_RET_CODE] == -808000:
            callback.failmsg(ret[PYTHON_RE_RET_CODE], 'error deleting collection')
    return ret

def acDeleteCollByAdmin(rule_args, callback):
    callback.msiDeleteCollByAdmin(rule_args[0], rule_args[1])

def acRenameLocalZone(rule_args, callback):
    coll_name = '/' + rule_args[0]

    ret = callback.msiRenameCollection(coll_name, rule_args[1])

    if not ret[PYTHON_RE_RET_STATUS]:
        callback.msiRollback()
        return ret

    ret = callback.msiRenameLocalZone(rule_args[0], rule_args[1])

    if not ret[PYTHON_RE_RET_STATUS]:
        callback.msiRollback()
        return ret

    callback.msiCommit()

def acGetUserByDN(rule_args, callback):
    pass

def acAclPolicy(rule_args, callback):
    callback.msiAclPolicy('STRICT')

def acTicketPolicy(rule_args, callback):
    pass

def acCheckPasswordStrength(rule_args, callback):
    pass

def acSetRescSchemeForCreate(rule_args, callback):
    server_config_file = open('/etc/irods/server_config.json')
    server_config_str = server_config_file.read()
    server_config_dict = json.loads(server_config_str)
    resc_name = 'demoResc'
    if server_config_dict.has_key('default_resource_name'):
        rescName = server_config_dict['default_resource_name']
    callback.msiSetDefaultResc(resc_name, 'null')
    pass

def acSetRescSchemeForRepl(rule_args, callback):
    server_config_file = open('/etc/irods/server_config.json')
    server_config_str = server_config_file.read()
    server_config_dict = json.loads(server_config_str)
    resc_name = 'demoResc'
    if server_config_dict.has_key('default_resource_name'):
        rescName = server_config_dict['default_resource_name']
    callback.msiSetDefaultResc(resc_name, 'null')
    pass

def acPreprocForDataObjOpen(rule_args, callback):
    pass

def acSetMultiReplPerResc(rule_args, callback):
    pass

def acPostProcForPut(rule_args, callback):
    pythonRuleEnginePluginTest(rule_args, callback)
    pass

def acPostProcForCopy(rule_args, callback):
    pass

def acPostProcForFilePathReg(rule_args, callback):
    pass

def acPostProcForCreate(rule_args, callback):
    pass

def acPostProcForOpen(rule_args, callback):
    pass

def acPostProcForPhymv(rule_args, callback):
    pass

def acPostProcForRepl(rule_args, callback):
    pass

def acSetNumThreads(rule_args, callback):
    callback.msiSetNumThreads('default', '64', 'default')

def acDataDeletePolicy(rule_args, callback):
    pass

def acPostProcForDelete(rule_args, callback):
    pass

def acSetChkFilePathPerm(rule_args, callback):
    callback.msiSetChkFilePathPerm('disallowPathReg')

def acTrashPolicy(rule_args, callback):
    pass

def acSetPublicUserPolicy(rule_args, callback):
    pass

def acChkHostAccessControl(rule_args, callback):
    pass

def acSetVaultPathPolicy(rule_args, callback):
    callback.msiSetGraftPathScheme('no', '1')

def acSetReServerNumProc(rule_args, callback):
    callback.msiSetReServerNumProc('default')

def acPreprocForCollCreate(rule_args, callback):
    pass

def acPostProcForCollCreate(rule_args, callback):
    pass

def acPreprocForRmColl(rule_args, callback):
    pass

def acPostProcForRmColl(rule_args, callback):
    pass

def acPreProcForModifyUser(rule_args, callback):
    pass

def acPostProcForModifyUser(rule_args, callback):
    pass

def acPreProcForModifyAVUMetadata(rule_args, callback):
    pass

def acPostProcForModifyAVUMetadata(rule_args, callback):
    pass

def acPreProcForCreateUser(rule_args, callback):
    pass

def acPostProcForCreateUser(rule_args, callback):
    pass

def acPreProcForDeleteUser(rule_args, callback):
    pass

def acPostProcForDeleteUser(rule_args, callback):
    pass

def acPreProcForCreateResource(rule_args, callback):
    pass

def acPostProcForCreateResource(rule_args, callback):
    pass

def acPreProcForCreateToken(rule_args, callback):
    pass

def acPostProcForCreateToken(rule_args, callback):
    pass

def acPreProcForModifyUserGroup(rule_args, callback):
    pass

def acPostProcForModifyUserGroup(rule_args, callback):
    pass

def acPreProcForDeleteResource(rule_args, callback):
    pass

def acPostProcForDeleteResource(rule_args, callback):
    pass

def acPreProcForDeleteToken(rule_args, callback):
    pass

def acPostProcForDeleteToken(rule_args, callback):
    pass

def acPreProcForModifyResource(rule_args, callback):
    pass

def acPostProcForModifyResource(rule_args, callback):
    pass

def acPreProcForModifyCollMeta(rule_args, callback):
    pass

def acPostProcForModifyCollMeta(rule_args, callback):
    pass

def acPreProcForModifyDataObjMeta(rule_args, callback):
    pass

def acPostProcForModifyDataObjMeta(rule_args, callback):
    pass

def acPreProcForModifyAccessControl(rule_args, callback):
    pass

def acPostProcForModifyAccessControl(rule_args, callback):
    pass

def acPreProcForObjRename(rule_args, callback):
    pass

def acPostProcForObjRename(rule_args, callback):
    pass

def acPreProcForGenQuery(rule_args, callback):
    pass

def acPostProcForGenQuery(rule_args, callback):
    pass

def acRescQuotaPolicy(rule_args, callback):
    callback.msiSetRescQuotaPolicy('off')

def acBulkPutPostProcPolicy(rule_args, callback):
    callback.msiSetBulkPutPostProcPolicy('off')

def acPostProcForTarFileReg(rule_args, callback):
    pass

def acPostProcForDataObjWrite(rule_args, callback):
    pass

def acPostProcForDataObjRead(rule_args, callback):
    pass

def acPreProcForExecCmd(rule_args, callback):
    pass

def acPreProcForServerPortal(rule_args, callback):
    pass

def acPostProcForServerPortal(rule_args, callback):
    pass
