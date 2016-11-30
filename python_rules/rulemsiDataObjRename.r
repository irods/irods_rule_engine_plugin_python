def myTestRule(rule_args, callback):
    source_file = global_vars['*SourceFile'][1:-1]
    new_file_path = global_vars['*NewFilePath'][1:-1]

    dummy_int = {}
    dummy_int[PYTHON_MSPARAM_TYPE] = PYTHON_INT_MS_T

    callback.msiDataObjRename(source_file, new_file_path, '0', dummy_int)

    callback.writeLine('stdout', 'The name of ' + source_file + ' is changed to ' + new_file_path)

INPUT *SourceFile="/tempZone/home/rods/sub1/foo1", *NewFilePath="/tempZone/home/rods/sub1/renamed"
OUTPUT ruleExecOut
