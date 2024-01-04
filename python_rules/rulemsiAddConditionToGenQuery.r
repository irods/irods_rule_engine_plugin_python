def main(rule_args, callback, rei):
    select = irods_rule_vars['*Select'][1:-1]
    attribute = irods_rule_vars['*Attribute'][1:-1]
    operator = irods_rule_vars['*Operator'][1:-1]
    value = irods_rule_vars['*Value'][1:-1]

    ret_val = {}

    # Initial condition for query corresponds to "COLL_NAME like '/tempZone/home/rods/%%'"
    ret_val = callback.msiMakeGenQuery(select, "COLL_NAME like '/tempZone/home/rods/%%'", irods_types.GenQueryInp())
    genQueryInp = ret_val['arguments'][2]

    # Add condition to query "DATA_NAME like rule%%"
    ret_val = callback.msiAddConditionToGenQuery(attribute, operator, value, genQueryInp)
    genQueryInp = ret_val['arguments'][3]

    ret_val = callback.msiExecGenQuery(genQueryInp, irods_types.GenQueryOut())
    genQueryOut = ret_val['arguments'][1]

    for row in range(genQueryOut.rowCnt):
        data = genQueryOut.sqlResult[0].row(row)
        coll = genQueryOut.sqlResult[1].row(row)
        callback.writeLine('stdout', coll + '/' + data)

INPUT *Select="DATA_NAME, COLL_NAME", *Attribute="DATA_NAME", *Operator=" like ", *Value="rule%%"
OUTPUT ruleExecOut
