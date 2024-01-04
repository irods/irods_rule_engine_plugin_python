def main(rule_args, callback, rei):
    where = irods_rule_vars['*Where'][1:-1]
    in_str = irods_rule_vars['*StringIn'][1:-1]

    callback.writeLine(where, in_str)

INPUT *Where="stdout", *StringIn="line"
OUTPUT ruleExecOut
