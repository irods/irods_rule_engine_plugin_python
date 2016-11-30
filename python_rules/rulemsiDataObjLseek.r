def myTestRule(rule_args, callback):
    obj = global_vars['*Obj'][1:-1]
    oflags = global_vars['*OFlags'][1:-1]
    obj_b = global_vars['*ObjB'][1:-1]
    oflags_b = global_vars['*OFlagsB'][1:-1]
    offset = global_vars['*Offset'][1:-1]
    loc = global_vars['*Loc'][1:-1]
    length = global_vars['*Len'][1:-1]

    dummy_int = {}
    dummy_int[PYTHON_MSPARAM_TYPE] = PYTHON_INT_MS_T

    ret_val = callback.msiDataObjOpen(oflags, dummy_int)
    file_desc = ret_val[PYTHON_RE_RET_OUTPUT][1]

    ret_val = callback.msiDataObjCreate(obj_b, oflags_b, dummy_int)
    file_desc_b = ret_val[PYTHON_RE_RET_OUTPUT][2]

    dummy_buf_len = {}
    dummy_buf_len[PYTHON_MSPARAM_TYPE] = PYTHON_BUF_LEN_MS_T
    ret_val = callback.msiDataObjRead(file_desc, length, dummy_buf_len)
    read_buf = ret_val[PYTHON_RE_RET_OUTPUT][2]

    callback.msiDataObjWrite(file_desc_b, read_buf, dummy_int)

    callback.msiDataObjClose(file_desc, dummy_int)
    callback.msiDataObjClose(file_desc_b, dummy_int)

    callback.writeLine('stdout', 'Open file ' + obj + ', create file ' + obj_b + ', copy ' + length + ' bytes starting at location ' + offset)

INPUT *Obj="/tempZone/home/rods/test/foo1", *OFlags="objPath=/tempZone/home/rods/test/foo1++++rescName=demoResc++++replNum=0++++openFlags=O_RDONLY", *ObjB="/tempZone/home/rods/test/foo4", *OFlagsB="destRescName=demoResc++++forceFlag=", *Offset="10", *Loc="SEEK_SET", *Len="100"
OUTPUT ruleExecOut
