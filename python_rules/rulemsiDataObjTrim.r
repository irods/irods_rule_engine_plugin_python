def main(rule_args, callback, rei):
    source_file = irods_rule_vars['*SourceFile'][1:-1]

    callback.msiDataObjTrim(source_file, 'null', 'null', '1', 'null', 0)

    callback.writeLine('stdout', 'The replicas of file ' + source_file + ' are deleted')

INPUT *SourceFile="/tempZone/home/rods/sub1/foo2"
OUTPUT ruleExecOut
