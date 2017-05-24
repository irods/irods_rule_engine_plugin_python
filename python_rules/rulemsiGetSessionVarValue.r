def myTestRule(rule_args, callback, rei):

    import session_vars
    import pprint

    vars = session_vars.get_map(rei)

    callback.writeLine('serverLog', 'userNameClient' + ' = ' + vars['client_user']['user_name'])
    callback.writeLine('stdout', 'Variables written to log file')


INPUT null
OUTPUT ruleExecOut
