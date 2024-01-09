def main(rule_args, callback, rei):
    source_file = irods_rule_vars['*SourceFile'][1:-1]
    resource = irods_rule_vars['*Resource'][1:-1]

    callback.msiDataObjRepl(source_file, 'destRescName=' + resource, 0)

    callback.writeLine('stdout', 'The file ' + source_file + ' is replicated onto resource ' + resource)

INPUT *SourceFile="/tempZone/home/rods/sub1/foo3", *Resource="testallrulesResc"
OUTPUT ruleExecOut
