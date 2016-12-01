def myTestRule(rule_args, callback):
    coll = global_vars['*Coll'][1:-1]
    resc = global_vars['*Resc'][1:-1]

    dummy_int = {}
    dummy_int[PYTHON_MSPARAM_TYPE] = PYTHON_INT_MS_T

    ret_val = callback.msiPhyBundleColl(coll, resc, dummy_int)

    callback.writeLine('stdout', 'Create tar file of collection ' + coll + ' on resource ' + resc)

INPUT *Coll="/tempZone/home/rods/test", *Resc="testallrulesResc"
OUTPUT ruleExecOut
