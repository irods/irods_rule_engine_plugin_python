def myTestRule(rule_args, callback):
    dest_file = global_vars['*DestFile'][1:-1]
    dest_resource = global_vars['*DestResource'][1:-1]
    local_file = global_vars['*LocalFile'][1:-1]

    dummy_int = {}
    dummy_int[PYTHON_MSPARAM_TYPE] = PYTHON_INT_MS_T

    callback.msiDataObjPut(dest_file, dest_resource, 'localPath=' + local_file + '++++forceFlag=', dummy_int)

    callback.writeLine('stdout', 'File ' + local_file + ' is written to the data grid as '+ dest_file)


INPUT *DestFile="/tempZone/home/rods/sub1/foo1", *DestResource="demoResc", *LocalFile="foo1"
OUTPUT ruleExecOut
