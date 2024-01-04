def main(rule_args, callback, rei):
    address = irods_rule_vars['*Address'][1:-1]
    subject = irods_rule_vars['*Subject'][1:-1]
    body = irods_rule_vars['*Body'][1:-1]

    callback.msiSendMail(address, subject, body)
    callback.writeLine('stdout', 'Sent e-mail to ' + address + ' about ' + subject)

INPUT *Address="tgr@irods.org", *Subject="Test message", *Body="Testing the msiSendMail microservice"
OUTPUT ruleExecOut
