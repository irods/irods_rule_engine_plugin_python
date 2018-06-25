def main(rule_args, callback, rei):
    user = global_vars['*User'][1:-1]
    path = global_vars['*Path'][1:-1]
    acl = global_vars['*Acl'][1:-1]

    callback.msiSetACL('default', acl, user, path)

    callback.writeLine('stdout', 'Set ' + acl + ' access for ' + user + ' on file ' + path)

INPUT *User="rods", *Path="/tempZone/home/rods/sub1/foo3", *Acl="write"
OUTPUT ruleExecOut
