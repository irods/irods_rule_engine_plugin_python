def myTestRule(rule_args, callback):
    a = 1

    callback.writeLine('stdout', 'Wrote an integer')
    callback.writeLine('stdout', str(a))

INPUT null
OUTPUT ruleExecOut
