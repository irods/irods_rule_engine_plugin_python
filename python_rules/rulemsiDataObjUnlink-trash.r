def myTestRule(rule_args, callback):
    path = global_vars['*Path'][1:-1]

    dummy_int = {}
    dummy_int[PYTHON_MSPARAM_TYPE] = PYTHON_INT_MS_T

    callback.msiDataObjUnlink('objPath=' + path + '++++irodsAdminRmTrash=', dummy_int)

    callback.writeLine('stdout', 'Files in trash are removed')

INPUT *Path="/tempZone/trash/home/rods/sub1/objunlink1"
OUTPUT ruleExecOut
