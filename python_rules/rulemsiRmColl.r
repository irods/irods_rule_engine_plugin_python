def myTestRule(rule_args, callback):
    coll = global_vars['*Coll'][1:-1]
    flag = global_vars['*Flag'][1:-1]

    dummy_int = {}
    dummy_int[PYTHON_MSPARAM_TYPE] = PYTHON_INT_MS_T

    callback.msiRmColl(coll, flag, dummy_int)
    
    callback.writeLine('stdout', 'Removed collection ' + coll)

INPUT *Coll="/tempZone/home/rods/ruletest/subforrmcoll", *Flag="forceFlag="
OUTPUT ruleExecOut
