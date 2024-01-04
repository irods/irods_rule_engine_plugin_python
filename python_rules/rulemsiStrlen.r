def main(rule_args, callback, rei):
    in_str = irods_rule_vars['*StringIn'][1:-1]

    length = len(in_str)

    callback.writeLine('stdout', 'The string: ' + in_str + ' has length ' + str(length))

INPUT *StringIn="/tempZone/home/rods/sub1/foo1"
OUTPUT ruleExecOut
