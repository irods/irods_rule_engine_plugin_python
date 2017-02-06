def myTestRule(rule_args, callback, rei):
    where = global_vars['*Where'][1:-1]
    in_str = global_vars['*StringIn'][1:-1]

    callback.writeLine(where, in_str + ' cheese')

INPUT *Where="stdout", *StringIn="string"
OUTPUT ruleExecOut
