def main(rule_args, callback, rei):
    flags = irods_rule_vars['*Flags'][1:-1]
    length = irods_rule_vars['*Len'][1:-1]

    ret_val = callback.msiDataObjOpen(flags, 0)
    file_desc = ret_val['arguments'][1]

    ret_val = callback.msiDataObjRead(file_desc, length, irods_types.BytesBuf())
    read_buffer = ret_val['arguments'][2]

    callback.msiDataObjClose(file_desc, 0)

    callback.msiFreeBuffer(read_buffer)

    callback.writeLine('stdout', 'Freed buffer')

INPUT *Flags="objPath=/tempZone/home/rods/sub1/freebuffer", *Len="100"
OUTPUT ruleExecOut
