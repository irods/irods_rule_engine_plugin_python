def main(rule_args, callback, rei):
    select = global_vars['*Select'][1:-1]

    ret_val = callback.msiExecStrCondQuery(select, irods_types.GenQueryOut())
    genQueryOut = ret_val['arguments'][1]

    for row in range(genQueryOut.rowCnt):
        data_name = genQueryOut.sqlResult[0].row(row)
        data_name_str = 'value_' + str(row) + '_0'
        callback.writeLine('stdout', data_name_str + ' = ' + data_name)

INPUT *Select="SELECT DATA_NAME where DATA_NAME like 'rule%%'"
OUTPUT ruleExecOut
