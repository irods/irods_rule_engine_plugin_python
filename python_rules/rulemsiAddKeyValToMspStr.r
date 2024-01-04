def main(rule_args, callback, rei):
    attr_name = irods_rule_vars['*AttrName'][1:-1]
    attr_value = irods_rule_vars['*AttrValue'][1:-1]

    out_str = '='.join([attr_name, attr_value])

    callback.writeLine('stdout', 'The string now contains')
    callback.writeLine('stdout', out_str)

INPUT *AttrName="destRescName", *AttrValue="demoResc"
OUTPUT ruleExecOut
