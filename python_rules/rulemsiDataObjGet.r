def myTestRule(rule_args, callback):
    source_file = global_vars['*SourceFile'][1:-1]

    import os
    (coll, file) = os.path.split(source_file)

    dummy_int = {}
    dummy_int[PYTHON_MSPARAM_TYPE] = PYTHON_INT_MS_T
    
    callback.msiDataObjGet(source_file, 'localPath=./' + file + '++++forceFlag=', dummy_int)

    callback.writeLine('stdout', 'File ' + source_file + ' is retrieved from the data grid')


INPUT *SourceFile="/tempZone/home/rods/sub1/foo1"
OUTPUT ruleExecOut
