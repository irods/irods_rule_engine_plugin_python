def main(rule_args, callback, rei):
    in_str = irods_rule_vars['*StringIn'][1:-1]

    new_str = in_str[:-1]
    callback.writeLine('stdout', 'The input string is :  ' + in_str)
    callback.writeLine('stdout', 'The output string is : ' + new_str)

INPUT *StringIn="/tempZone/home/rods/sub1/foo1/"
OUTPUT ruleExecOut
