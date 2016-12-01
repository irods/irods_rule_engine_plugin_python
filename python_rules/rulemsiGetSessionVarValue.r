def myTestRule(rule_args, callback):
    session_var_name = global_vars['*A'][1:-1]

    var = session_vars[session_var_name]

    callback.writeLine('serverLog', session_var_name + ' = ' + var)
    callback.writeLine('stdout', 'Variable written to log file')


INPUT *A="userNameClient"
OUTPUT ruleExecOut
