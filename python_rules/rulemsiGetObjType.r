def main(rule_args, callback, rei):
    source_file = irods_rule_vars['*SourceFile'][1:-1]
    resource = irods_rule_vars['*Resource'][1:-1]

    ret_val = callback.msiGetObjType(source_file, 'dummy_str')
    type_file = ret_val['arguments'][1]
    callback.writeLine('stdout', 'The type of object ' + source_file + ' is ' + type_file)

    ret_val = callback.msiGetObjType(resource, 'dummy_str')
    type_resource = ret_val['arguments'][1]
    callback.writeLine('stdout', 'The type of object ' + resource + ' is ' + type_resource)

INPUT *SourceFile="/tempZone/home/rods/sub1/foo3", *Resource="demoResc"
OUTPUT ruleExecOut
