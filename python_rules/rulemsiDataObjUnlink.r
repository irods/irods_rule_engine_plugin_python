def main(rule_args, callback, rei):
    source_file = global_vars['*SourceFile'][1:-1]

    callback.msiDataObjUnlink('objPath=' + source_file + '++++forceFlag=', 0)

    callback.writeLine('stdout', 'Data object ' + source_file + ' is now removed')

INPUT *SourceFile="/tempZone/home/rods/sub1/objunlink2"
OUTPUT ruleExecOut
