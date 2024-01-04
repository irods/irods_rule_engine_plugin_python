def main(rule_args, callback, rei):
    in_str = irods_rule_vars['*string'][1:-1]
    separator = irods_rule_vars['*separator'][1:-1]

    callback.writeLine('stdout', 'string is ' + in_str)
    (head, tail) = in_str.rsplit(separator, 1)
    callback.writeLine('stdout', 'tail is ' + tail + ' and head is ' + head)

INPUT *string="foo.bar.baz", *separator="."
OUTPUT ruleExecOut
