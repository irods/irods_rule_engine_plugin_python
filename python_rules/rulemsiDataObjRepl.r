def myTestRule(rule_args, callback):
    source_file = global_vars['*SourceFile'][1:-1]
    resource = global_vars['*Resource'][1:-1]

    dummy_int = {}
    dummy_int[PYTHON_MSPARAM_TYPE] = PYTHON_INT_MS_T

    callback.msiDataObjRepl(source_file, 'destRescName=' + resource, dummy_int)

    callback.writeLine('stdout', 'The file ' + source_file + ' is replicated onto resource ' + resource)

INPUT *SourceFile="/tempZone/home/rods/sub1/foo3", *Resource="testallrulesResc"
OUTPUT ruleExecOut
