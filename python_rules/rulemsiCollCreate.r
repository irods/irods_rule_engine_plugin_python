def myTestRule(rule_args, callback):
    path = global_vars['*Path'][1:-1]

    dummy_int = {}
    dummy_int[PYTHON_MSPARAM_TYPE] = PYTHON_INT_MS_T

    callback.msiCollCreate(path, '0', dummy_int)

    # Verify collection was created
    callback.writeLine('stdout', 'Create collection ' + path)
    callback.writeLine('stdout', 'Collection created was')

    genQueryOut = {}
    genQueryOut[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYOUT_MS_T
    ret_val = callback.msiExecStrCondQuery("SELECT COLL_NAME where COLL_NAME = '" + path +"'", genQueryOut)
    genQueryOut = ret_val[PYTHON_RE_RET_OUTPUT][1]

    for row in range(int(genQueryOut['rowCnt'])):
        coll_name_str = 'value_' + str(row) + '_0'
        coll_name = genQueryOut[coll_name_str]
        callback.writeLine('stdout', coll_name_str + ' = ' + coll_name)

INPUT *Path="/tempZone/home/rods/ruletest/collCreate"
OUTPUT ruleExecOut
