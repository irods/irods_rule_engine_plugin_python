def myTestRule(rule_args, callback, rei):
    source_file = global_vars['*SourceFile'][1:-1]

    callback.msiDataObjUnlink('objPath=' + source_file + '++++replNum=1', 0)

    callback.writeLine('stdout', 'Replica number 1 of file ' + source_file + ' is removed') 

INPUT *SourceFile="/tempZone/home/rods/sub1/objunlink2"
OUTPUT ruleExecOut
