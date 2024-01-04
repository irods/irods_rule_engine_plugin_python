def main(rule_args, callback, rei):
    coll = irods_rule_vars['*Coll'][1:-1]
    flag = irods_rule_vars['*Flag'][1:-1]

    callback.msiRmColl(coll, flag, 0)
    
    callback.writeLine('stdout', 'Removed collection ' + coll)

INPUT *Coll="/tempZone/home/rods/ruletest/subforrmcoll", *Flag="forceFlag="
OUTPUT ruleExecOut
