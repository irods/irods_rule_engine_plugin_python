def myTestRule(rule_args, callback):
    source_file = global_vars['*SourceFile'][1:-1]

    import os
    (coll, file) = os.path.split(source_file)

    dummy_rodsObjStat = {}
    dummy_rodsObjStat[PYTHON_MSPARAM_TYPE] = PYTHON_RODSOBJSTAT_MS_T

    ret_val = callback.msiObjStat(source_file, dummy_rodsObjStat)
    source_file_stat = ret_val[PYTHON_RE_RET_OUTPUT][1]

    ret_val = callback.msiObjStat(coll, dummy_rodsObjStat)
    coll_stat = ret_val[PYTHON_RE_RET_OUTPUT][1]

    callback.writeLine('stdout', 'Type of object is written into a RodsObjStat_PI structure')

INPUT *SourceFile="/tempZone/home/rods/sub1/foo3"
OUTPUT ruleExecOut
