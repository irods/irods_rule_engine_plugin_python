def main(rule_args, callback, rei):
    source_file = irods_rule_vars['*SourceFile'][1:-1]
    source_resource = irods_rule_vars['*SourceResource'][1:-1]
    dest_resource = irods_rule_vars['*DestResource'][1:-1]
    replica_number = irods_rule_vars['*ReplicaNumber'][1:-1]

    callback.msiDataObjPhymv(source_file, dest_resource, source_resource, replica_number, 'null', 0)

    callback.writeLine('stdout', 'Replica number ' + replica_number + 'of file ' + source_file + ' is moved from resource ' + source_resource + ' to resource ' + dest_resource)

INPUT *SourceFile="/tempZone/home/rods/forphymv/phymvfile", *DestResource="testallrulesResc", *SourceResource="demoResc", *ReplicaNumber="null"
OUTPUT ruleExecOut
