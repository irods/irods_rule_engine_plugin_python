def main(rule_args, callback, rei):
    dest = irods_rule_vars['*dest'][1:-1]
    src = irods_rule_vars['*source'][1:-1]

    callback.writeLine('stdout', 'dest is ' + dest)
    callback.writeLine('stdout', 'source is ' + src)
    dest += src
    callback.writeLine('stdout', 'dest is now ' + dest)

INPUT *dest="foo", *source="bar"
OUTPUT ruleExecOut
