def myTestRule(rule_args, callback):
    select = global_vars['*Select'][1:-1]
    select_add = global_vars['*SelectAdd'][1:-1]
    fcn = global_vars['*Function'][1:-1]

    genQueryInp = {}
    genQueryInp[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYINP_MS_T

    # Initial select is on COLL_NAME
    ret_val = callback.msiMakeGenQuery(select, "COLL_NAME like '/tempZone/home/rods/%%'", genQueryInp)
    genQueryInp = ret_val[PYTHON_RE_RET_OUTPUT][2]
    genQueryInp[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYINP_MS_T

    # Add select on sum(DATA_SIZE)
    ret_val = callback.msiAddSelectFieldToGenQuery(select_add, fcn, genQueryInp)
    genQueryInp = ret_val[PYTHON_RE_RET_OUTPUT][2]
    genQueryInp[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYINP_MS_T

    genQueryOut = {}
    genQueryOut[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYOUT_MS_T
    ret_val = callback.msiExecGenQuery(genQueryInp, genQueryOut)
    genQueryOut = ret_val[PYTHON_RE_RET_OUTPUT][1]

    for row in range(int(genQueryOut['rowCnt'])):
        coll_str = 'value_' + str(row) + '_0'
        size_str = 'value_' + str(row) + '_1'
        coll = genQueryOut[coll_str]
        size = genQueryOut[size_str]
        callback.writeLine('stdout', 'For collection ' + coll + ', the size of the files is ' + size)

INPUT *Select="COLL_NAME", *SelectAdd="DATA_SIZE", *Function="SUM"
OUTPUT ruleExecOut
