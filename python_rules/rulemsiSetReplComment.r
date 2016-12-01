def myTestRule(rule_args, callback):
    source_file = global_vars['*SourceFile'][1:-1]
    comment = global_vars['*Comment'][1:-1]

    callback.msiSetReplComment('null', source_file, '0', comment)

    callback.writeLine('stdout', 'The comment added to file ' + source_file + ' is ' + comment)

    import os
    (coll, file) = os.path.split(source_file)

    genQueryInp = {}
    genQueryInp[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYINP_MS_T

    ret_val = callback.msiMakeGenQuery('DATA_COMMENTS', "DATA_NAME = '" + file + "' and COLL_NAME = '" + coll + "'", genQueryInp)
    genQueryInp = ret_val[PYTHON_RE_RET_OUTPUT][2]
    genQueryInp[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYINP_MS_T

    genQueryOut = {}
    genQueryOut[PYTHON_MSPARAM_TYPE] = PYTHON_GENQUERYOUT_MS_T

    ret_val = callback.msiExecGenQuery(genQueryInp, genQueryOut)
    genQueryOut = ret_val[PYTHON_RE_RET_OUTPUT][1]

    for row in range(int(genQueryOut['rowCnt'])):
        data_comment_string = 'value_' + str(row) + '_0'
        data_comment = genQueryOut[data_comment_string]
        callback.writeLine('stdout', 'The comment retrieved from iCAT is ' + data_comment)


INPUT *SourceFile="/tempZone/home/rods/sub1/foo3", *Comment="New comment"
OUTPUT ruleExecOut
