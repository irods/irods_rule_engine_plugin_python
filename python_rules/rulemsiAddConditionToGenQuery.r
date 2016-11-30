def myTestRule(rule_args, callback):
    select = global_vars['*Select'][1:-1]
    attribute = global_vars['*Attribute'][1:-1]
    operator = global_vars['*Operator'][1:-1]
    value = global_vars['*Value'][1:-1]

    ret_val = {}

    genQueryInp = {}
    genQueryInp[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYINP_MS_T

    # Initial condition for query corresponds to "COLL_NAME like '/tempZone/home/rods/%%'"
    ret_val = callback.msiMakeGenQuery(select, "COLL_NAME like '/tempZone/home/rods/%%'", genQueryInp)
    genQueryInp = ret_val[PYTHON_RE_RET_OUTPUT][2]
    genQueryInp[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYINP_MS_T

    # Add condition to query "DATA_NAME like rule%%"
    ret_val = callback.msiAddConditionToGenQuery(attribute, operator, value, genQueryInp)
    genQueryInp = ret_val[PYTHON_RE_RET_OUTPUT][3]
    genQueryInp[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYINP_MS_T

    genQueryOut = {}
    genQueryOut[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYOUT_MS_T
    ret_val = callback.msiExecGenQuery(genQueryInp, genQueryOut)
    genQueryOut = ret_val[PYTHON_RE_RET_OUTPUT][1]

    for row in range(int(genQueryOut['rowCnt'])):
        data_name_str = 'value_' + str(row) + '_0'
        coll_name_str = 'value_' + str(row) + '_1'
        data = genQueryOut[data_name_str]
        coll = genQueryOut[coll_name_str]
        callback.writeLine('stdout', coll + '/' + data)

INPUT *Select="DATA_NAME, COLL_NAME", *Attribute="DATA_NAME", *Operator=" like ", *Value="rule%%"
OUTPUT ruleExecOut
