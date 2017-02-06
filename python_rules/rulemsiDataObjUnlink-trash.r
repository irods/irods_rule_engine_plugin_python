def myTestRule(rule_args, callback, rei):
    path = global_vars['*Path'][1:-1]

    callback.msiDataObjUnlink('objPath=' + path + '++++irodsAdminRmTrash=', 0)

    callback.writeLine('stdout', 'Files in trash are removed')

INPUT *Path="/tempZone/trash/home/rods/sub1/objunlink1"
OUTPUT ruleExecOut
