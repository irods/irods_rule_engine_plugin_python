import datetime

def acPostProcForPut(rule_args, callback):
    with open('/tmp/from_core_py.txt', 'a') as f:
        f.write(str(datetime.datetime.now()))
        f.write('\n')
        for arg in (rule_args + [callback]):
            f.write('\t')
            f.write(str(arg))
            f.write(' : ')
            f.write(str(type(arg)))
            f.write('\n')

    session_vars = [
        'userNameClient', 'dataSize',
    ]

    callback.writeLine('serverLog', 'XXXXXXXXXXXXXX Printed to server log from python')
    for s in session_vars:
        v = callback.getSessionVar(s, 'dummy')[1]
        callback.writeLine('serverLog', s + ' :: ' + v)
