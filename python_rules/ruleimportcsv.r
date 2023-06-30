def main(rule_args, callback, rei):
    import csv
    callback.writeLine('stdout', 'loaded module ' + csv.__name__)

INPUT null
OUTPUT ruleExecOut
