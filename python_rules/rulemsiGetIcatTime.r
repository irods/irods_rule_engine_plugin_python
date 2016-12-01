def myTestRule(rule_args, callback):
    ret_val = callback.msiGetIcatTime('dummy_str', 'unix')
    time_1 = ret_val[PYTHON_RE_RET_OUTPUT][0]

    ret_val = callback.msiGetIcatTime('dummy_str', 'human')
    time_2 = ret_val[PYTHON_RE_RET_OUTPUT][0]

    callback.writeLine('stdout', 'Time in seconds is ' + time_1)
    callback.writeLine('stdout', 'Time human readable is ' + time_2)

INPUT null
OUTPUT ruleExecOut
