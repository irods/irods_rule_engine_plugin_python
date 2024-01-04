def main(rule_args, callback, rei):
    source_file = irods_rule_vars['*SourceFile'][1:-1]

    import os
    (coll, file) = os.path.split(source_file)

    callback.msiDataObjGet(source_file, 'localPath=./' + file + '++++forceFlag=', 0)

    callback.writeLine('stdout', 'File ' + source_file + ' is retrieved from the data grid')


INPUT *SourceFile="/tempZone/home/rods/sub1/foo1"
OUTPUT ruleExecOut
