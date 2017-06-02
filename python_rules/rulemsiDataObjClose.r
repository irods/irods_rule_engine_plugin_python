def myTestRule(rule_args, callback, rei):
    resc = global_vars['*Resc'][1:-1]
    obj = global_vars['*Obj'][1:-1]
    oflags = global_vars['*OFlags'][1:-1]

    ret_val = callback.msiDataObjCreate(obj, oflags, 0)
    file_desc = ret_val['arguments'][2]
    callback.writeLine('stdout', 'Created and closed file ' + obj)
    callback.msiDataObjClose(file_desc, 0)
    
INPUT *Resc="demoResc", *Obj="/tempZone/home/rods/test/foo4", *OFlags="destRescName=demoResc++++forceFlag="
OUTPUT ruleExecOut
