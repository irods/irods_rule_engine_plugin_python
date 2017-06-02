def myTestRule(rule_args, callback, rei):
    select = global_vars['*Select'][1:-1]
    select_add = global_vars['*SelectAdd'][1:-1]
    fcn = global_vars['*Function'][1:-1]

    ret_val = callback.msiMakeGenQuery(select, "COLL_NAME like '/tempZone/home/rods/%%'", irods_types.GenQueryInp())
    genQueryInp = ret_val['arguments'][2]

    ret_val = callback.msiAddSelectFieldToGenQuery(select_add, fcn, genQueryInp)
    genQueryInp = ret_val['arguments'][2]

    callback.msiPrintGenQueryInp('stdout', genQueryInp)

INPUT *Select="COLL_NAME", *SelectAdd="DATA_SIZE", *Function="SUM"
OUTPUT ruleExecOut
