def myTestRule(rule_args, callback):
    path = global_vars['*Path'][1:-1]
    acl = global_vars['*Acl'][1:-1]

    dummy_int = {}
    dummy_int[PYTHON_MSPARAM_TYPE] = PYTHON_INT_MS_T

    ret_val = callback.msiCheckAccess(path, acl, dummy_int)
    result = int(ret_val[PYTHON_RE_RET_OUTPUT][2])
    if not result:
        callback.writeLine('stdout', 'File ' + path + ' does not have access ' + acl)
    else:
        callback.writeLine('stdout', 'File ' + path + ' has access ' + acl)

INPUT *Path="/tempZone/home/rods/sub1/foo1", *Acl="own"
OUTPUT ruleExecOut
