def main(rule_args, callback, rei):
    source_obj = irods_rule_vars['*sourceObj'][1:-1]
    dest_resc = irods_rule_vars['*destResc'][1:-1]
    dest_obj = irods_rule_vars['*destObj'][1:-1]

    ret_val = callback.msiDataObjRsync(source_obj, 'IRODS_TO_IRODS', dest_resc, dest_obj, 0)
    status = ret_val['arguments'][4]

    callback.writeLine('stdout', 'Source object ' + source_obj + ' synchronized with destination object ' + dest_obj)
    callback.writeLine('stdout', 'status = ' + str(status))

INPUT *sourceObj="/tempZone/home/rods/synctest/source.txt", *destResc="demoResc", *destObj="/tempZone/home/rods/synctest/dest.txt"
OUTPUT ruleExecOut
