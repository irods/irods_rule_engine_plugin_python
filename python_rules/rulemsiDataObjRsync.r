def rulemsiDataObjRsync(rule_args, callback):
    source_obj = global_vars['*sourceObj'][1:-1]
    dest_resc = global_vars['*destResc'][1:-1]
    dest_obj = global_vars['*destObj'][1:-1]

    dummy_int = {}
    dummy_int[PYTHON_MSPARAM_TYPE] = PYTHON_INT_MS_T

    ret_val = callback.msiDataObjRsync(source_obj, 'IRODS_TO_IRODS', dest_resc, dest_obj, dummy_int)
    status = ret_val[PYTHON_RE_RET_OUTPUT][4]

    callback.writeLine('stdout', 'Source object ' + source_obj + ' synchronized with destination object ' + dest_obj)
    callback.writeLine('stdout', 'status = ' + status)

INPUT *sourceObj="/tempZone/home/rods/synctest/source.txt", *destResc="demoResc", *destObj="/tempZone/home/rods/synctest/dest.txt"
OUTPUT ruleExecOut
