def myTestRule(rule_args, callback, rei):
    source_file = global_vars['*SourceFile'][1:-1]
    dest_file = global_vars['*DestFile'][1:-1]

    callback.msiDataObjCopy(source_file, dest_file, 'forceFlag=', 0)
    callback.writeLine('stdout', 'File ' + source_file + ' copied to ' + dest_file)

INPUT *SourceFile="/tempZone/home/rods/sub1/foo1", *DestFile="/tempZone/home/rods/sub2/foo1"
OUTPUT ruleExecOut
