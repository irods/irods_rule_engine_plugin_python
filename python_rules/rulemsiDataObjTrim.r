def myTestRule(rule_args, callback):
    source_file = global_vars['*SourceFile'][1:-1]

    dummy_int = {}
    dummy_int[PYTHON_MSPARAM_TYPE] = PYTHON_INT_MS_T

    callback.msiDataObjTrim(source_file, 'null', 'null', '1', 'null', dummy_int)

    callback.writeLine('stdout', 'The replicas of file ' + source_file + ' are deleted')

INPUT *SourceFile="/tempZone/home/rods/sub1/foo2"
OUTPUT ruleExecOut
