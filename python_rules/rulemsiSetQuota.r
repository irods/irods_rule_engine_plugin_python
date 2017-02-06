def myTestRule(rule_args, callback, rei):
    type = global_vars['*Type'][1:-1]
    name = global_vars['*Name'][1:-1]
    resource = global_vars['*Resource'][1:-1]
    value = global_vars['*Value'][1:-1]

    callback.msiSetQuota(type, name, resource, value)
    
    callback.writeLine('stdout', 'Set quota on ' + name + ' for resource ' + resource + ' to ' + value + ' bytes')

INPUT *Type="user", *Name="rods", *Resource="demoResc", *Value="1000000000"
OUTPUT ruleExecOut
