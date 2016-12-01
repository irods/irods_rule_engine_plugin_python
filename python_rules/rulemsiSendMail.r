def myTestRule(rule_args, callback):
    address = global_vars['*Address'][1:-1]
    subject = global_vars['*Subject'][1:-1]
    body = global_vars['*Body'][1:-1]

    callback.msiSendMail(address, subject, body)
    callback.writeLine('stdout', 'Sent e-mail to ' + address + ' about ' + subject)

INPUT *Address="tgr@irods.org", *Subject="Test message", *Body="Testing the msiSendMail microservice"
OUTPUT ruleExecOut
