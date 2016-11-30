def myTestRule(rule_args, callback):
    src_coll = global_vars['*srcColl'][1:-1]
    dest_coll = global_vars['*destColl'][1:-1]
    resource = global_vars['*Resource'][1:-1]

    dummy_int = {}
    dummy_int[PYTHON_MSPARAM_TYPE] = PYTHON_INT_MS_T

    callback.msiCollRsync(src_coll, dest_coll, resource, 'IRODS_TO_IRODS', dummy_int)

    callback.writeLine('stdout', 'Synchronized collection ' + src_coll + ' with collection ' + dest_coll)

INPUT *srcColl="/tempZone/home/rods/sub1", *destColl="/tempZone/home/rods/sub2", *Resource="demoResc"
OUTPUT ruleExecOut
