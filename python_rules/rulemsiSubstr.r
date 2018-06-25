def main(rule_args, callback, rei):
    in_str = global_vars['*StringIn'][1:-1]
    offset_str = global_vars['*Offset'][1:-1]
    length_str = global_vars['*Length'][1:-1]

    offset = int(offset_str)
    length = int(length_str)

    sub_str = in_str[offset:offset+length]

    callback.writeLine('stdout', 'The input string is: ' + in_str)
    callback.writeLine('stdout', 'The offset is ' + offset_str + ' and the length is ' + length_str)
    callback.writeLine('stdout', 'The output string is ' + sub_str)

INPUT *StringIn="/tempZone/home/rods/sub1/foo1/", *Offset="10", *Length="4"
OUTPUT ruleExecOut
