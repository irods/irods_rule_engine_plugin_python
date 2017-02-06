def myTestRule(rule_args, callback, rei):
    callback.delayExec('<PLUSET>30s</PLUSET><EF>24h</EF>', 'callback.msiQuota()', '')
    callback.writeLine('serverLog', 'Updated quota check')

INPUT null
OUTPUT ruleExecOut
