def main(rule_args, callback, rei):
    resc = irods_rule_vars['*Resc'][1:-1]
    obj = irods_rule_vars['*Obj'][1:-1]
    oflags = irods_rule_vars['*OFlags'][1:-1]

    ret_val = callback.msiDataObjCreate(obj, oflags, 0)
    file_desc = ret_val['arguments'][2]
    ret_val = callback.msiDataObjClose(file_desc, 0)
    callback.writeLine('stdout', 'Created and closed file ' + obj)

INPUT *Resc="demoResc", *Obj="/tempZone/home/rods/test/foo4", *OFlags="destRescName=demoResc++++forceFlag="
OUTPUT ruleExecOut
