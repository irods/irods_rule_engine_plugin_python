def main(rule_args, callback, rei):
    import ssl
    callback.writeLine('stdout', 'loaded module ' + ssl.__name__)

INPUT null
OUTPUT ruleExecOut
