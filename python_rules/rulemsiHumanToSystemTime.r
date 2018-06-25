def main(rule_args, callback, rei):
    ret_val = callback.msiGetSystemTime('dummy_str', 'human')
    date = ret_val['arguments'][0]

    ret_val = callback.msiHumanToSystemTime(date, 'dummy_str')
    time = ret_val['arguments'][1]

    callback.writeLine('stdout', 'Input date is ' + date)
    callback.writeLine('stdout', 'Time in unix seconds is ' + time)

INPUT null
OUTPUT ruleExecOut
