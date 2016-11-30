def myTestRule(rule_args, callback):
    resc = global_vars['*Resc'][1:-1]
    obj = global_vars['*Obj'][1:-1]
    oflags = global_vars['*OFlags'][1:-1]

    dummy_int = {}
    dummy_int[PYTHON_MSPARAM_TYPE] = PYTHON_INT_MS_T

    ret_val = callback.msiDataObjCreate(obj, oflags, dummy_int)
    file_desc = ret_val[PYTHON_RE_RET_OUTPUT][2]
    ret_val = callback.msiDataObjClose(file_desc, dummy_int)
    callback.writeLine('stdout', 'Created and closed file ' + obj)

INPUT *Resc="demoResc", *Obj="/tempZone/home/rods/test/foo4", *OFlags="destRescName=demoResc++++forceFlag="
OUTPUT ruleExecOut
