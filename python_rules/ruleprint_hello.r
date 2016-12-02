def myTestRule(rule_args, callback):
    callback.writeLine('stdout', 'Execute command to print out hello')
    callback.print_hello()

INPUT null
OUTPUT ruleExecOut
