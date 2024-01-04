def main(rule_args, callback, rei):
    data_object = irods_rule_vars['*dataObject'][1:-1]

    callback.writeLine('stdout', 'Object is ' + data_object)

    import os
    (coll, file) = os.path.split(data_object)

    callback.writeLine('stdout', 'Collection is ' + coll + ' and file is ' + file)

INPUT *dataObject="/tempZone/home/rods/sub1/foo1"
OUTPUT ruleExecOut
