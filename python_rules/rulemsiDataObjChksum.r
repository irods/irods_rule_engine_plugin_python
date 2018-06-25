def main(rule_args, callback, rei):
    data_object = global_vars['*dataObject'][1:-1]
    flags = global_vars['*Flags'][1:-1]

    import os
    (coll, file) = os.path.split(data_object)
    callback.writeLine('stdout', 'Collection is ' + coll + ' and file is ' + file)

    ret_val = callback.msiMakeGenQuery('DATA_CHECKSUM', "DATA_NAME = '" + file + "' AND COLL_NAME = '" + coll + "'", irods_types.GenQueryInp())
    genQueryInp = ret_val['arguments'][2]

    ret_val = callback.msiExecGenQuery(genQueryInp, irods_types.GenQueryOut())
    genQueryOut = ret_val['arguments'][1]

    for row in range(genQueryOut.rowCnt):
        data_chksum = genQueryOut.sqlResult[0].row(row)
        ret_val = callback.msiDataObjChksum(data_object, flags, 'dummy_str')
        new_chksum = ret_val['arguments'][2]
        callback.writeLine('stdout', 'Saved checksum for file ' + file + ' is ' + data_chksum + ', new checksum is ' + new_chksum)


INPUT *dataObject="/tempZone/home/rods/sub1/foo1", *Flags="forceChksum="
OUTPUT ruleExecOut
