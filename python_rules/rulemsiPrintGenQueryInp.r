def myTestRule(rule_args, callback):
    select = global_vars['*Select'][1:-1]
    select_add = global_vars['*SelectAdd'][1:-1]
    fcn = global_vars['*Function'][1:-1]

    genQueryInp = {}
    genQueryInp[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYINP_MS_T

    ret_val = callback.msiMakeGenQuery(select, "COLL_NAME like '/tempZone/home/rods/%%'", genQueryInp)
    genQueryInp = ret_val[PYTHON_RE_RET_OUTPUT][2]
    genQueryInp[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYINP_MS_T

    ret_val = callback.msiAddSelectFieldToGenQuery(select_add, fcn, genQueryInp)
    genQueryInp = ret_val[PYTHON_RE_RET_OUTPUT][2]
    genQueryInp[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYINP_MS_T

    callback.msiPrintGenQueryInp('stdout', genQueryInp)

INPUT *Select="COLL_NAME", *SelectAdd="DATA_SIZE", *Function="SUM"
OUTPUT ruleExecOut
