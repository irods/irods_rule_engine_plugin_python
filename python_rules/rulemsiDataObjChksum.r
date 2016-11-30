def myTestRule(rule_args, callback):
    data_object = global_vars['*dataObject'][1:-1]
    flags = global_vars['*Flags'][1:-1]

    import os
    (coll, file) = os.path.split(data_object)
    callback.writeLine('stdout', 'Collection is ' + coll + ' and file is ' + file)

    genQueryInp = {}
    genQueryInp[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYINP_MS_T
    ret_val = callback.msiMakeGenQuery('DATA_CHECKSUM', "DATA_NAME = '" + file + "' AND COLL_NAME = '" + coll + "'", genQueryInp)
    genQueryInp = ret_val[PYTHON_RE_RET_OUTPUT][2]
    genQueryInp[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYINP_MS_T

    genQueryOut = {}
    genQueryOut[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYOUT_MS_T
    ret_val = callback.msiExecGenQuery(genQueryInp, genQueryOut)
    genQueryOut = ret_val[PYTHON_RE_RET_OUTPUT][1]

    for row in range(int(genQueryOut['rowCnt'])):
        data_chksum_str = 'value_' + str(row) + '_0'
        data_chksum = genQueryOut[data_chksum_str]
        ret_val = callback.msiDataObjChksum(data_object, flags, 'dummy_str')
        new_chksum = ret_val[PYTHON_RE_RET_OUTPUT][2]
        callback.writeLine('stdout', 'Saved checksum for file ' + file + ' is ' + data_chksum + ', new checksum is ' + new_chksum)


INPUT *dataObject="/tempZone/home/rods/sub1/foo1", *Flags="forceChksum="
OUTPUT ruleExecOut
