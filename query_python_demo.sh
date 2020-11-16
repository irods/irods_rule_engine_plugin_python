#!/bin/sh
case $1 in
  0) irule -r irods_rule_engine_plugin-irods_rule_language-instance "qc2gmain('1')" null ruleExecOut ;;
  1) irule -r irods_rule_engine_plugin-irods_rule_language-instance "qc2gmain('0')" null ruleExecOut ;;
  2) irule -r irods_rule_engine_plugin-irods_rule_language-instance "qc2dmain()" null ruleExecOut ;;
  3) irule -r irods_rule_engine_plugin-irods_rule_language-instance "qc1main()" null ruleExecOut ;;
  4) irule -r irods_rule_engine_plugin-irods_rule_language-instance "qc2main()" null ruleExecOut ;;
  5) irule -r irods_rule_engine_plugin-irods_rule_language-instance "qcmain()" null ruleExecOut ;;
  6) irule -r irods_rule_engine_plugin-irods_rule_language-instance "qmain()" null ruleExecOut ;;
  7) irule -r irods_rule_engine_plugin-irods_rule_language-instance "qqmain('2')" null ruleExecOut ;;
  8) irule -r irods_rule_engine_plugin-irods_rule_language-instance "qqmain('5')" null ruleExecOut ;;
  9) irule -r irods_rule_engine_plugin-irods_rule_language-instance "qqmain('7')" null ruleExecOut ;;
  *) exit 123;;
esac
