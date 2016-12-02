def myTestRule(rule_args, callback):
    in_str = global_vars['*Str'][1:-1]
    
    callback.writeLine('stdout', 'Input string is ' + in_str)

    # Replaces msiString2StrArray
    str_as_list = in_str.split('%')

    # Combined with above, replaces msiString2KeyValPair
    str_as_keyval_dict = {}
    for keyval in str_as_list:
        (key, val) = keyval.split('=', 1)
        str_as_keyval_dict[key] = val

    # Replaces writeKeyValPairs
    for key, val in str_as_keyval_dict.items():
        callback.writeLine('stdout', key + ' : ' + val)

    # Replaces msiStrArray2String
    out_str = '%'.join(str_as_list)

    callback.writeLine('stdout', 'After conversion to array and back, string is')
    callback.writeLine('stdout', out_str)

INPUT *Str="key1=value1%key2=value2%key3=value3"
OUTPUT ruleExecOut

