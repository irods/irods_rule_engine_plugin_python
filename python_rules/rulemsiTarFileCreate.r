def myTestRule(rule_args, callback, rei):
    file = global_vars['*File'][1:-1]
    coll = global_vars['*Coll'][1:-1]
    resc = global_vars['*Resc'][1:-1]
    flag = global_vars['*Flag'][1:-1]

    callback.msiTarFileCreate(file, coll, resc, flag)

    callback.writeLine('stdout', 'Created tar file ' + file + ' for collection ' + coll + ' on resource ' + resc)

INPUT *File="/tempZone/home/rods/test/testcoll.tar", *Coll="/tempZone/home/rods/sub1", *Resc="demoResc", *Flag=""
OUTPUT ruleExecOut
