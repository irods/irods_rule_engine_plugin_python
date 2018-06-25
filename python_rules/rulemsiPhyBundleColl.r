def main(rule_args, callback, rei):
    coll = global_vars['*Coll'][1:-1]
    resc = global_vars['*Resc'][1:-1]

    ret_val = callback.msiPhyBundleColl(coll, resc, 0)

    callback.writeLine('stdout', 'Create tar file of collection ' + coll + ' on resource ' + resc)

INPUT *Coll="/tempZone/home/rods/test", *Resc="testallrulesResc"
OUTPUT ruleExecOut
