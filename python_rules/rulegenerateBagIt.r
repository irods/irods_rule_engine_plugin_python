def main(rule_args, callback, rei):
    bagIt_data = irods_rule_vars['*BAGITDATA'][1:-1]
    new_bagIt_root = irods_rule_vars['*NEWBAGITROOT'][1:-1]

    print_size = 0
    print_unit = ''

    ret_val = {}

    # Create NEWBAGITROOT collection
    callback.msiCollCreate(new_bagIt_root, '1', 0)
    offset = len(new_bagIt_root) + 1

    # Write bagit.txt to NEWBAGITROOT/bagit.txt
    callback.writeLine('stdout', 'BagIt-Version: 0.96')
    callback.writeLine('stdout', 'Tag-File-Character-Encoding: UTF-8')
    ret_val = callback.msiDataObjCreate(new_bagIt_root + '/bagit.txt', 'null', 0)
    fd = ret_val['arguments'][2]
    callback.msiDataObjWrite(fd, 'stdout', 0)
    callback.msiDataObjClose(fd, 0)
    callback.msiFreeBuffer('stdout')

    # Rsyncs existing *BAGITDATA to NEWBAGITROOT/data
    new_bagIt_data = new_bagIt_root + '/data'
    callback.msiCollRsync(bagIt_data, new_bagIt_data, 'null', 'IRODS_TO_IRODS', 0)

    # Generates payload manifest file of NEWBAGITROOT/data
    continue_index_old = 1
    condition = "COLL_NAME like '" + new_bagIt_data + "%%'"
    ret_val = callback.msiMakeGenQuery('DATA_ID, DATA_NAME, COLL_NAME', condition, irods_types.GenQueryInp())
    genQueryInp = ret_val['arguments'][2]
    ret_val = callback.msiExecGenQuery(genQueryInp, irods_types.GenQueryOut())
    genQueryOut = ret_val['arguments'][1]

    while continue_index_old > 0:
        for row in range(genQueryOut.rowCnt):
            data = genQueryOut.sqlResult[1].row(row)
            coll = genQueryOut.sqlResult[2].row(row)
            full_path = coll + '/' + data
            relative_path = full_path[offset:]
            ret_val = callback.msiDataObjChksum(full_path, 'forceChksum=', 'dummy_str')
            chksum = ret_val['arguments'][2]
            callback.writeLine('stdout', relative_path + '   ' + chksum)
        continue_index_old = genQueryOut.continueInx
        if continue_index_old > 0:
            ret_val = callback.msiGetMoreRows(genQueryInp, genQueryOut, 0)
            genQueryOut = ret_val['arguments'][1]

    # Write payload manifest to NEWBAGITROOT/manifest-md5.txt
    ret_val = callback.msiDataObjCreate(new_bagIt_root + '/manifest-md5.txt', 'null', 0)
    fd = ret_val['arguments'][2]
    callback.msiDataObjWrite(fd, 'stdout', 0)
    callback.msiDataObjClose(fd, 0)
    callback.msiFreeBuffer('stdout')

    # Write tagmanifest file to NEWBAGITROOT/tagmanifest-md5.txt 
    ret_val = callback.msiDataObjChksum(new_bagIt_root + '/bagit.txt', 'forceChksum', 'dummy_str')
    chksum = ret_val['arguments'][2]
    callback.writeLine('stdout', 'bagit.txt   ' + chksum)
    ret_val = callback.msiDataObjChksum(new_bagIt_root + '/manifest-md5.txt', 'forceChksum', 'dummy_str')
    chksum = ret_val['arguments'][2]
    callback.writeLine('stdout', 'manifest-md5.txt   ' + chksum)
    ret_val = callback.msiDataObjCreate(new_bagIt_root + '/tagmanifest-md5.txt', 'null', 0)
    fd = ret_val['arguments'][2]
    callback.msiDataObjWrite(fd, 'stdout', 0)
    callback.msiDataObjClose(fd, 0)
    callback.msiFreeBuffer('stdout')

    # Create tarfile of new bag for faster download
    tar_file_path = new_bagIt_root + '.tar'
    callback.msiTarFileCreate(tar_file_path, new_bagIt_root, 'null', 'null')

    # Get filesize of new tarfile
    import os
    (coll, tar_file_name) = os.path.split(tar_file_path)

    condition = "COLL_NAME like '" + coll + "%%' and DATA_NAME = '" + tar_file_name + "'"
    ret_val = callback.msiMakeGenQuery('DATA_SIZE', condition, irods_types.GenQueryInp())
    genQueryInp = ret_val['arguments'][2]
    ret_val = callback.msiExecGenQuery(genQueryInp, irods_types.GenQueryOut())
    genQueryOut = ret_val['arguments'][1]

    for row in range(genQueryOut.rowCnt):
        data_size = int(genQueryOut.sqlResult[0].row(row))
        if data_size > 1048576:
            print_size = data_size / 1048576
            print_unit = 'MB'
        else:
            if data_size > 1024:
                print_size = data_size / 1024
                print_unit = 'KB'
            else:
                print_size = data_size
                print_unit = 'B'

    # Output report and suggested download procedures
    callback.writeLine('stdout', '\nYour BagIt bag has been created and tarred on the iRODS server:')
    callback.writeLine('stdout', '  ' + new_bagIt_root + '.tar - ' + str(print_size) + ' ' + print_unit)
    callback.writeLine('stdout', '\nTo copy it to your local computer, use:')
    callback.writeLine('stdout', '  iget -Pf ' + new_bagIt_root + '.tar ' + tar_file_name + '\n')

    # Write to rodsLog
    callback.writeLine('serverLog', 'BagIt bag created: ' + new_bagIt_root + ' <- ' + bagIt_data)

INPUT *BAGITDATA="/tempZone/home/rods/sub1", *NEWBAGITROOT="/tempZone/home/rods/bagit"
OUTPUT ruleExecOut
