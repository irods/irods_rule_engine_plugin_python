def main(rule_args, callback, rei):
    import ctypes
    callback.writeLine('stdout', 'loaded module ' + ctypes.__name__)

INPUT null
OUTPUT ruleExecOut
