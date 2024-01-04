def main(rule_args, callback, rei):
    type = irods_rule_vars['*Type'][1:-1]
    name = irods_rule_vars['*Name'][1:-1]
    resource = irods_rule_vars['*Resource'][1:-1]
    value = irods_rule_vars['*Value'][1:-1]

    callback.msiSetQuota(type, name, resource, value)
    
    callback.writeLine('stdout', 'Set quota on ' + name + ' for resource ' + resource + ' to ' + value + ' bytes')

INPUT *Type="user", *Name="rods", *Resource="demoResc", *Value="0"
OUTPUT ruleExecOut
