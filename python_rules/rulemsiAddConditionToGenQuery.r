def myTestRule(rule_args, callback, rei):
    select = global_vars['*Select'][1:-1]
    attribute = global_vars['*Attribute'][1:-1]
    operator = global_vars['*Operator'][1:-1]
    value = global_vars['*Value'][1:-1]

    ret_val = {}

    # Initial condition for query corresponds to "COLL_NAME like '/tempZone/home/rods/%%'"
    ret_val = callback.msiMakeGenQuery(select, "COLL_NAME like '/tempZone/home/rods/%%'", irods_types.GenQueryInp())
    genQueryInp = ret_val[PYTHON_RE_RET_ARGUMENTS][2]

    # Add condition to query "DATA_NAME like rule%%"
    ret_val = callback.msiAddConditionToGenQuery(attribute, operator, value, genQueryInp)
    genQueryInp = ret_val[PYTHON_RE_RET_ARGUMENTS][3]

    ret_val = callback.msiExecGenQuery(genQueryInp, irods_types.GenQueryOut())
    genQueryOut = ret_val[PYTHON_RE_RET_ARGUMENTS][1]

    for row in range(genQueryOut.rowCnt):
        data = genQueryOut.sqlResult[0].row(row)
        coll = genQueryOut.sqlResult[1].row(row)
        callback.writeLine('stdout', coll + '/' + data)

INPUT *Select="DATA_NAME, COLL_NAME", *Attribute="DATA_NAME", *Operator=" like ", *Value="rule%%"
OUTPUT ruleExecOut
