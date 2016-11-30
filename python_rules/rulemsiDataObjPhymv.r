def myTestRule(rule_args, callback):
    source_file = global_vars['*SourceFile'][1:-1]
    source_resource = global_vars['*SourceResource'][1:-1]
    dest_resource = global_vars['*DestResource'][1:-1]
    replica_number = global_vars['*ReplicaNumber'][1:-1]

    dummy_int = {}
    dummy_int[PYTHON_MSPARAM_TYPE] = PYTHON_INT_MS_T

    callback.msiDataObjPhymv(source_file, dest_resource, source_resource, replica_number, 'null', dummy_int)

    callback.writeLine('stdout', 'Replica number ' + replica_number + 'of file ' + source_file + ' is moved from resource ' + source_resource + ' to resource ' + dest_resource)

INPUT *SourceFile="/tempZone/home/rods/forphymv/phymvfile", *DestResource="testallrulesResc", *SourceResource="demoResc", *ReplicaNumber="0"
OUTPUT ruleExecOut
