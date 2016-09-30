Building the iRODS Python Rule Engine Plugin requires version 4.2 of the iRODS software from github (http://github.com/irods/irods).

Build with 'cmake PATH_TO_SOURCE_DIRECTORY --GNinja', then run 'ninja package' to create the deb, and 'sudo dpkg -i irods_rule_engine_plugin-python-4.2.0-ubuntu14-x86_64.deb' to install the python rule engine plugin.

After installing the plugin, you need to edit /etc/hosts/server_config.json to make your iRODS install aware of the plugin. In the "rule_engines" section of the json file, you need to add a new stanza, below the stanza that begins "instance_name": "irods_rule_engine_plugin-irods_rule_language". A working stanza for the python rule engine plugin is included below:

    {
        "instance_name": "irods_rule_engine_plugin-python-instance",
        "plugin_name": "irods_rule_engine_plugin_python",
        "plugin_specific_configuration": {
        }
    } 

If you want the python rule engine to be your default rule engine, you will need to change the order of the rule_engines stanzas, so that the python stanza comes before the irods_rule_language stanza.

The core.py file in this repo contains a Python implementation of all static peps from core.re.

