def myTestRule(rule_args, callback, rei):
    ret_val = callback.msiGetFormattedSystemTime('dummy_str', 'human', '%d-%02d-%02dT%02d:%02d:%02d')
    out_string = ret_val['arguments'][0]

    callback.writeLine('stdout', out_string)

INPUT null
OUTPUT ruleExecOut
