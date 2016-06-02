Building the iRODS Python Rule Engine Plugin requires the newest version (XBB-2) of the iRODS software from github (http://github.com/irods/irods).

Build with 'cmake PATH_TO_SOURCE_DIRECTORY --GNinja', then run 'ninja package' to create the deb, and 'sudo dpkg -i irods-rule-engine-plugin-python-4.2.0-ubuntu14-x86_64.deb' to install the python rule engine plugin.

After installing the plugin, you need to edit /etc/hosts/server_config.json to make your iRODS install aware of the plugin. In the "rule_engines" section of the json file, you need to add a new stanza, below the stanza that begins "instance_name": "re-irods-instance". A working stanza for the python rule engine plugin is included below:

    {
        "instance_name": "rule-engine-plugin-python-instance",
        "plugin_name": "rule-engine-plugin-python",
        "plugin_specific_configuration": {
        }
    } 

Also, if you want the python rule engine to be the default rule engine, you need to change the line in that file that reads:

"default_rule_engine_instance": "re-irods-instance", 

to

"default_rule_engine_instance": "rule-engine-plugin-python-instance",

The core.py file in this repo contains a Python implementation of all static peps from core.re. The custom.re file contains a single function, getSessionVar, that must be added to your core.re file in order for core.py to function correctly.

