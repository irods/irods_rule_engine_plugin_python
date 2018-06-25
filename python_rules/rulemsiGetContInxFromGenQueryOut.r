def main(rule_args, callback, rei):
    coll = global_vars['*Coll'][1:-1]
    condition = global_vars['*Condition'][1:-1]

    continue_index_old = 1
    count = 0
    size = 0

    ret_val = callback.msiMakeGenQuery('DATA_ID, DATA_SIZE', condition, irods_types.GenQueryInp())
    genQueryInp = ret_val['arguments'][2]

    ret_val = callback.msiExecGenQuery(genQueryInp, irods_types.GenQueryOut())
    genQueryOut = ret_val['arguments'][1]

    while continue_index_old > 0:
        for row in range(genQueryOut.rowCnt):
            data_size_str = 'value_' + str(row) + '_1'
            size += int(genQueryOut.sqlResult[0].row(row))
            count += 1
        continue_index_old = genQueryOut.continueInx
        if continue_index_old > 0:
            ret_val = callback.msiGetMoreRows(genQueryInp, genQueryOut, 0)
            genQueryOut = ret_val['arguments'][1]

    callback.writeLine('stdout', 'Number of files in ' + coll + ' is ' + str(count) + ' and total size is ' + str(size))

INPUT *Coll="/tempZone/home/rods/%%", *Condition="COLL_NAME like '/tempZone/home/rods/%%'"
OUTPUT ruleExecOut
