def generateBagIt(rule_args, callback):
    bagIt_data = global_vars['*BAGITDATA'][1:-1]
    new_bagIt_root = global_vars['*NEWBAGITROOT'][1:-1]

    print_size = 0
    print_unit = ''

    dummy_int = {}
    dummy_int[PYTHON_MSPARAM_TYPE] = PYTHON_INT_MS_T

    ret_val = {}

    # Create NEWBAGITROOT collection
    callback.msiCollCreate(new_bagIt_root, '1', dummy_int)
    offset = len(new_bagIt_root) + 1

    # Write bagit.txt to NEWBAGITROOT/bagit.txt
    callback.writeLine('stdout', 'BagIt-Version: 0.96')
    callback.writeLine('stdout', 'Tag-File-Character-Encoding: UTF-8')
    ret_val = callback.msiDataObjCreate(new_bagIt_root + '/bagit.txt', 'null', dummy_int)
    fd = ret_val[PYTHON_RE_RET_OUTPUT][2]
    callback.msiDataObjWrite(fd, 'stdout', dummy_int)
    callback.msiDataObjClose(fd, dummy_int)
    callback.msiFreeBuffer('stdout')

    # Rsyncs existing *BAGITDATA to NEWBAGITROOT/data
    new_bagIt_data = new_bagIt_root + '/data'
    callback.msiCollRsync(bagIt_data, new_bagIt_data, 'null', 'IRODS_TO_IRODS', dummy_int)

    # Generates payload manifest file of NEWBAGITROOT/data
    continue_index_old = 1
    genQueryInp = {}
    genQueryInp[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYINP_MS_T
    condition = "COLL_NAME like '" + new_bagIt_data + "%%'"
    ret_val = callback.msiMakeGenQuery('DATA_ID, DATA_NAME, COLL_NAME', condition, genQueryInp)
    genQueryInp = ret_val[PYTHON_RE_RET_OUTPUT][2]
    genQueryInp[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYINP_MS_T
    
    genQueryOut = {}
    genQueryOut[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYOUT_MS_T
    ret_val = callback.msiExecGenQuery(genQueryInp, genQueryOut)
    genQueryOut = ret_val[PYTHON_RE_RET_OUTPUT][1]

    while continue_index_old > 0:
        for row in range(int(genQueryOut['rowCnt'])):
            data_name_str = 'value_' + str(row) + '_1'
            coll_name_str = 'value_' + str(row) + '_2'
            data = genQueryOut[data_name_str]
            coll = genQueryOut[coll_name_str]
            full_path = coll + '/' + data
            relative_path = full_path[offset:]
            ret_val = callback.msiDataObjChksum(full_path, 'forceChksum=', 'dummy_str')
            chksum = ret_val[PYTHON_RE_RET_OUTPUT][2]
            callback.writeLine('stdout', relative_path + '   ' + chksum)
        continue_index_old = int(genQueryOut['continueInx'])
        if continue_index_old > 0:
            genQueryOut[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYOUT_MS_T
            ret_val = callback.msiGetMoreRows(genQueryInp, genQueryOut, dummy_int)
            genQueryOut = ret_val[PYTHON_RE_RET_OUTPUT][1]

    # Write payload manifest to NEWBAGITROOT/manifest-md5.txt
    ret_val = callback.msiDataObjCreate(new_bagIt_root + '/manifest-md5.txt', 'null', dummy_int)
    fd = ret_val[PYTHON_RE_RET_OUTPUT][2]
    callback.msiDataObjWrite(fd, 'stdout', dummy_int)
    callback.msiDataObjClose(fd, dummy_int)
    callback.msiFreeBuffer('stdout')

    # Write tagmanifest file to NEWBAGITROOT/tagmanifest-md5.txt 
    ret_val = callback.msiDataObjChksum(new_bagIt_root + '/bagit.txt', 'forceChksum', 'dummy_str')
    chksum = ret_val[PYTHON_RE_RET_OUTPUT][2]
    callback.writeLine('stdout', 'bagit.txt   ' + chksum)
    ret_val = callback.msiDataObjChksum(new_bagIt_root + '/manifest-md5.txt', 'forceChksum', 'dummy_str')
    chksum = ret_val[PYTHON_RE_RET_OUTPUT][2]
    callback.writeLine('stdout', 'manifest-md5.txt   ' + chksum)
    ret_val = callback.msiDataObjCreate(new_bagIt_root + '/tagmanifest-md5.txt', 'null', dummy_int)
    fd = ret_val[PYTHON_RE_RET_OUTPUT][2]
    callback.msiDataObjWrite(fd, 'stdout', dummy_int)
    callback.msiDataObjClose(fd, dummy_int)
    callback.msiFreeBuffer('stdout')

    # Create tarfile of new bag for faster download
    tar_file_path = new_bagIt_root + '.tar'
    callback.msiTarFileCreate(tar_file_path, new_bagIt_root, 'null', 'null')

    # Get filesize of new tarfile
    callback.writeLine('serverLog', 'XXXXX computing filesize of new tar file')
    
    import os
    (coll, tar_file_name) = os.path.split(tar_file_path)
    callback.writeLine('serverLog', 'XXXXX coll = ' + coll + ' tar_file_name = ' + tar_file_name)
#    ret_val = callback.msiSplitPath(new_bagIt_root + '.tar', 'dummy_str', 'dummy_str')
#    coll = ret_val[PYTHON_RE_RET_OUTPUT][1]
#    tar_file_name = ret_val[PYTHON_RE_RET_OUTPUT][2]

    genQueryInp[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYINP_MS_T
    condition = "COLL_NAME like '" + coll + "%%' and DATA_NAME = '" + tar_file_name + "'"
    ret_val = callback.msiMakeGenQuery('DATA_SIZE', condition, genQueryInp)
    genQueryInp = ret_val[PYTHON_RE_RET_OUTPUT][2]
    genQueryInp[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYINP_MS_T
    
    genQueryOut = {}
    genQueryOut[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYOUT_MS_T
    ret_val = callback.msiExecGenQuery(genQueryInp, genQueryOut)
    genQueryOut = ret_val[PYTHON_RE_RET_OUTPUT][1]

#    query_str = "COLL_NAME like '" + coll + "%%' and DATA_NAME = '" + tar_file_name + "'"
#    callback.writeLine('serverLog', 'XXXXX ' + query_str)
#    ret_val = callback.msiMakeQuery("DATA_SIZE", "COLL_NAME like '" + coll + "%%' and DATA_NAME = '" + tar_file_name + "'", 'dummy_str')
#    query_str = ret_val[PYTHON_RE_RET_OUTPUT][1]
#    genQueryOut[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYOUT_MS_T
#    ret_val = callback.msiExecStrCondQuery(query_str, genQueryOut)
#    genQueryOut = ret_val[PYTHON_RE_RET_OUTPUT][1]
    for row in range(int(genQueryOut['rowCnt'])):
        data_size_str = 'value_' + str(row) + '_0'
        data_size = int(genQueryOut[data_size_str])
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
    callback.writeLine('serverLog', 'XXXXX filesize done')

    # Output report and suggested download procedures
    callback.writeLine('stdout', '\nYour BagIt bag has been created and tarred on the iRODS server:')
    callback.writeLine('stdout', '  ' + new_bagIt_root + '.tar - ' + str(print_size) + ' ' + print_unit)
    callback.writeLine('stdout', '\nTo copy it to your local computer, use:')
    callback.writeLine('stdout', '  iget -Pf ' + new_bagIt_root + '.tar ' + tar_file_name + '\n')

    # Write to rodsLog
#    callback.msiWriteRodsLog('BagIt bag created: ' + new_bagIt_root + ' <- ' + bagIt_data, dummy_int)
    callback.writeLine('serverLog', 'BagIt bag created: ' + new_bagIt_root + ' <- ' + bagIt_data)

INPUT *BAGITDATA="/tempZone/home/rods/sub1", *NEWBAGITROOT="/tempZone/home/rods/bagit"
OUTPUT ruleExecOut
