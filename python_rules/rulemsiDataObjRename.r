def main(rule_args, callback, rei):
    source_file = global_vars['*SourceFile'][1:-1]
    new_file_path = global_vars['*NewFilePath'][1:-1]

    callback.msiDataObjRename(source_file, new_file_path, '0', 0)

    callback.writeLine('stdout', 'The name of ' + source_file + ' is changed to ' + new_file_path)

INPUT *SourceFile="/tempZone/home/rods/sub1/foo1", *NewFilePath="/tempZone/home/rods/sub1/renamed"
OUTPUT ruleExecOut
