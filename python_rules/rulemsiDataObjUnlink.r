def myTestRule(rule_args, callback):
    source_file = global_vars['*SourceFile'][1:-1]

    dummy_int = {}
    dummy_int[PYTHON_MSPARAM_TYPE] = PYTHON_INT_MS_T

    callback.msiDataObjUnlink('objPath=' + source_file + '++++replNum=1', dummy_int)

    callback.writeLine('stdout', 'Replica number 1 of file ' + source_file + ' is removed') 

INPUT *SourceFile="/tempZone/home/rods/sub1/objunlink2"
OUTPUT ruleExecOut
