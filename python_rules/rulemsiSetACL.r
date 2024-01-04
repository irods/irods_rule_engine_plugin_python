def main(rule_args, callback, rei):
    user = irods_rule_vars['*User'][1:-1]
    path = irods_rule_vars['*Path'][1:-1]
    acl = irods_rule_vars['*Acl'][1:-1]

    callback.msiSetACL('default', acl, user, path)

    callback.writeLine('stdout', 'Set ' + acl + ' access for ' + user + ' on file ' + path)

INPUT *User="rods", *Path="/tempZone/home/rods/sub1/foo3", *Acl="write"
OUTPUT ruleExecOut
