# iRODS Rule Engine Plugin - Python

This C++ plugin provides the iRODS platform a rule engine that allows iRODS rules to be written in Python.

# Build

Building the iRODS Python Rule Engine Plugin requires version 4.2.1 of the iRODS software from github (http://github.com/irods/irods).

```
cd irods_rule_engine_plugin_python
mkdir build
cd build
cmake ../
make package
```

# Install

The packages produced by CMake will install the Python plugin shared object file:

`/usr/lib/irods/plugins/rule_engines/libirods_rule_engine_plugin-python.so`

# Configuration

After installing the plugin, `/etc/irods/server_config.json` needs to be configured to use the plugin.

Add a new stanza to the "rule_engines" array within `server_config.json`:

```json
{
    "instance_name": "irods_rule_engine_plugin-python-instance",
    "plugin_name": "irods_rule_engine_plugin-python",
    "plugin_specific_configuration": {}
}
```

Adding the Python Rule Engine Plugin stanza above the default "irods_rule_engine_plugin-irods_rule_language" stanza will allow any defined Python rules to take precedence over any similarly named rules in the iRODS Rule Language.

Python rules will be loaded from `core.py` (located at `/etc/irods/core.py` in a default installation).

# Other

- The example `core.py` file in this repository contains a Python implementation of all static policy enforcement points (PEPs) from a default `core.re` rulebase.

- This version of the Python Rule Engine Plugin uses the Python 2.7 interpreter.
