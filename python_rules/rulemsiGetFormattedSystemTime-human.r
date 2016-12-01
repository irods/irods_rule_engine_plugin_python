def myTestRule(rule_args, callback):
    ret_val = callback.msiGetFormattedSystemTime('dummy_str', 'human', '%d-%02d-%02dT%02d:%02d:%02d')
    out_string = ret_val[PYTHON_RE_RET_OUTPUT][0]

    callback.writeLine('stdout', out_string)

INPUT null
OUTPUT ruleExecOut
