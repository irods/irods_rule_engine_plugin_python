def main(rule_args, callback, rei):
    source_file = global_vars['*SourceFile'][1:-1]
    comment = global_vars['*Comment'][1:-1]

    callback.msiSetReplComment('null', source_file, '0', comment)

    callback.writeLine('stdout', 'The comment added to file ' + source_file + ' is ' + comment)

    import os
    (coll, file) = os.path.split(source_file)

    ret_val = callback.msiMakeGenQuery('DATA_COMMENTS', "DATA_NAME = '" + file + "' and COLL_NAME = '" + coll + "'", irods_types.GenQueryInp())
    genQueryInp = ret_val['arguments'][2]

    ret_val = callback.msiExecGenQuery(genQueryInp, irods_types.GenQueryOut())
    genQueryOut = ret_val['arguments'][1]

    for row in range(genQueryOut.rowCnt):
        data_comment = genQueryOut.sqlResult[0].row(row)
        callback.writeLine('stdout', 'The comment retrieved from iCAT is ' + data_comment)


INPUT *SourceFile="/tempZone/home/rods/sub1/foo3", *Comment="New comment"
OUTPUT ruleExecOut
