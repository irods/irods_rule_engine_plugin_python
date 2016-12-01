def myTestRule(rule_args, callback):
    select = global_vars['*Select'][1:-1]

    genQueryOut = {}
    genQueryOut[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYOUT_MS_T

    ret_val = callback.msiExecStrCondQuery(select, genQueryOut)
    genQueryOut = ret_val[PYTHON_RE_RET_OUTPUT][1]

    for row in range(int(genQueryOut['rowCnt'])):
        data_name_str = 'value_' + str(row) + '_0'
        data_name = genQueryOut[data_name_str]
        callback.writeLine('stdout', data_name_str + ' = ' + data_name)

INPUT *Select="SELECT DATA_NAME where DATA_NAME like 'rule%%'"
OUTPUT ruleExecOut
