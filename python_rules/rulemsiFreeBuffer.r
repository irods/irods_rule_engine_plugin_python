def myTestRule(rule_args, callback):
    flags = global_vars['*Flags'][1:-1]
    length = global_vars['*Len'][1:-1]

    dummy_int = {}
    dummy_int[PYTHON_MSPARAM_TYPE] = PYTHON_INT_MS_T

    ret_val = callback.msiDataObjOpen(flags, dummy_int)
    file_desc = ret_val[PYTHON_RE_RET_OUTPUT][1]

    dummy_buf_len = {}
    dummy_buf_len[PYTHON_MSPARAM_TYPE] = PYTHON_BUF_LEN_MS_T

    ret_val = callback.msiDataObjRead(file_desc, length, dummy_buf_len)
    read_buffer = ret_val[PYTHON_RE_RET_OUTPUT][2]

    callback.msiDataObjClose(file_desc, dummy_int)

    callback.msiFreeBuffer(read_buffer)

    callback.writeLine('stdout', 'Freed buffer')

INPUT *Flags="objPath=/tempZone/home/rods/sub1/freebuffer", *Len="100"
OUTPUT ruleExecOut
