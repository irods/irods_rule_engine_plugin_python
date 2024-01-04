def main(rule_args, callback, rei):
    obj = irods_rule_vars['*Obj'][1:-1]
    oflags = irods_rule_vars['*OFlags'][1:-1]
    obj_b = irods_rule_vars['*ObjB'][1:-1]
    oflags_b = irods_rule_vars['*OFlagsB'][1:-1]
    offset = irods_rule_vars['*Offset'][1:-1]
    loc = irods_rule_vars['*Loc'][1:-1]
    length = irods_rule_vars['*Len'][1:-1]

    ret_val = callback.msiDataObjOpen(oflags, 0)
    file_desc = ret_val['arguments'][1]

    ret_val = callback.msiDataObjCreate(obj_b, oflags_b, 0)
    file_desc_b = ret_val['arguments'][2]

    ret_val = callback.msiDataObjLseek(file_desc, offset, loc, 0)

    ret_val = callback.msiDataObjRead(file_desc, length, irods_types.BytesBuf())
    read_buf = ret_val['arguments'][2]

    callback.msiDataObjWrite(file_desc_b, read_buf, 0)

    callback.msiDataObjClose(file_desc, 0)
    callback.msiDataObjClose(file_desc_b, 0)

    callback.writeLine('stdout', 'Open file ' + obj + ', create file ' + obj_b + ', copy ' + length + ' bytes starting at location ' + offset)

INPUT *Obj="/tempZone/home/rods/test/foo1", *OFlags="objPath=/tempZone/home/rods/test/foo1++++rescName=demoResc++++replNum=0++++openFlags=O_RDONLY", *ObjB="/tempZone/home/rods/test/foo4", *OFlagsB="destRescName=demoResc++++forceFlag=", *Offset="10", *Loc="SEEK_SET", *Len="100"
OUTPUT ruleExecOut
