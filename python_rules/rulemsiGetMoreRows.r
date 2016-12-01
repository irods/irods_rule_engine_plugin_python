def myTestRule(rule_args, callback):
    coll = global_vars['*Coll'][1:-1]
    condition = global_vars['*Condition'][1:-1]

    continue_index_old = 1
    count = 0
    size = 0

    dummy_int = {}
    dummy_int[PYTHON_MSPARAM_TYPE] = PYTHON_INT_MS_T

    genQueryInp = {}
    genQueryInp[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYINP_MS_T

    ret_val = callback.msiMakeGenQuery('DATA_ID, DATA_SIZE', condition, genQueryInp)
    genQueryInp = ret_val[PYTHON_RE_RET_OUTPUT][2]
    genQueryInp[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYINP_MS_T

    genQueryOut = {}
    genQueryOut[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYOUT_MS_T

    ret_val = callback.msiExecGenQuery(genQueryInp, genQueryOut)
    genQueryOut = ret_val[PYTHON_RE_RET_OUTPUT][1]

    while continue_index_old > 0:
        for row in range(int(genQueryOut['rowCnt'])):
            data_size_str = 'value_' + str(row) + '_1'
            size += int(genQueryOut[data_size_str])
            count += 1
        continue_index_old = int(genQueryOut['continueInx'])
        if continue_index_old > 0:
            genQueryOut[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYOUT_MS_T
            ret_val = callback.msiGetMoreRows(genQueryInp, genQueryOut, dummy_int)
            genQueryOut = ret_val[PYTHON_RE_RET_OUTPUT][1]

    callback.writeLine('stdout', 'Number of files in ' + coll + ' is ' + str(count) + ' and total size is ' + str(size))

INPUT *Coll="/tempZone/home/rods/%%", *Condition="COLL_NAME like '/tempZone/home/rods/%%'"
OUTPUT ruleExecOut
