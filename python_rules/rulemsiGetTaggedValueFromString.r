def main(rule_args, callback, rei):
    tag = irods_rule_vars['*Tag'][1:-1]
    in_str = irods_rule_vars['*Str'][1:-1]

    callback.writeLine('stdout', 'String that is tested is')
    callback.writeLine('stdout', in_str)

    ret_val = callback.msiGetTaggedValueFromString(tag, in_str, 'dummy_str')
    val = ret_val['arguments'][2]

    callback.writeLine('stdout', 'Found value is ' + val)

INPUT *Tag="Mail", *Str="<Mail>IP-address</Mail>"
OUTPUT ruleExecOut
