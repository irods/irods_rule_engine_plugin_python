def main(rule_args, callback, rei):
    ret_val = callback.msiGetIcatTime('dummy_str', 'unix')
    start_time = ret_val['arguments'][0]

    callback.msiSleep('3', '')

    ret_val = callback.msiGetIcatTime('dummy_str', 'unix')
    end_time = ret_val['arguments'][0]

    callback.writeLine('stdout', 'Start time is ' + start_time)
    callback.writeLine('stdout', 'End time is ' + end_time)

    ret_val = callback.msiGetDiffTime(start_time, end_time, '', 'dummy_str')
    duration = ret_val['arguments'][3]

    callback.writeLine('stdout', 'Duration is ' + duration)

INPUT null
OUTPUT ruleExecOut
