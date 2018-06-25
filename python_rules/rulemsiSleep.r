def main(rule_args, callback, rei):
    sec = global_vars['*Sec'][1:-1]
    microsec = global_vars['*MicroSec'][1:-1]

    import time
    callback.writeLine('stdout', time.ctime())
    # callback.msiSleep(sec, microsec)
    float_sec = float(sec) + float(microsec)/1000000
    time.sleep(float_sec)
    callback.writeLine('stdout', time.ctime())

INPUT *Sec="3", *MicroSec="123456"
OUTPUT ruleExecOut
