def main(rule_args, callback, rei):
    path = irods_rule_vars['*Path'][1:-1]

    callback.msiCollCreate(path, '0', 0)

    # Verify collection was created
    callback.writeLine('stdout', 'Create collection ' + path)
    callback.writeLine('stdout', 'Collection created was')

    ret_val = callback.msiExecStrCondQuery("SELECT COLL_NAME where COLL_NAME = '" + path +"'", irods_types.GenQueryOut())
    genQueryOut = ret_val['arguments'][1]

    for row in range(genQueryOut.rowCnt):
        coll_name = genQueryOut.sqlResult[0].row(row)
        coll_name_str = 'value_' + str(row) + '_0'
        callback.writeLine('stdout', coll_name_str + ' = ' + coll_name)

INPUT *Path="/tempZone/home/rods/ruletest/collCreate"
OUTPUT ruleExecOut
