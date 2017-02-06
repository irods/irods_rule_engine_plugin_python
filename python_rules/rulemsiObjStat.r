def myTestRule(rule_args, callback, rei):
    source_file = global_vars['*SourceFile'][1:-1]

    import os
    (coll, file) = os.path.split(source_file)

    ret_val = callback.msiObjStat(source_file, irods_types.RodsObjStat())
    source_file_stat = ret_val[PYTHON_RE_RET_ARGUMENTS][1]

    ret_val = callback.msiObjStat(coll, irods_types.RodsObjStat())
    coll_stat = ret_val[PYTHON_RE_RET_ARGUMENTS][1]

    callback.writeLine('stdout', 'Type of object is written into a RodsObjStat_PI structure')

INPUT *SourceFile="/tempZone/home/rods/sub1/foo3"
OUTPUT ruleExecOut
