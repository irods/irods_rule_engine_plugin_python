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

# Remote Execution

There exists a requirement for the implementation of a different ```remote``` microservice call for every rule language.  Given the possibility of a namespace collision with more than one rule language being configured simultaneously, the name of the microservice to use for the python language is ```py_remote()```.
For example:
```
def main(rule_args, callback, rei):
    rule_code = "def main(rule_args, callback, rei):\n    print('This is a test of the Python Remote Rule Execution')"
    callback.py_remote('icat.example.org', '', rule_code, '')
INPUT null
OUTPUT ruleExecOut
```

# Calling rules across language boundaries

The following example rulefiles (iRODS Rule Language and Python) demonstrate calling microservices in both directions when reacting to a simple `iput` (the acPostProcForPut rule is fired in Python).

The Python `acPostProcForPut` rule calls `irods_rule_language_printme` with a single string parameter via the `callback` mechanism.

The iRODS Rule Language rule calls `python_printme` with a single string parameter (without using the `callback` mechanism).

core.py:
```python
def acPostProcForPut(rule_args, callback, rei):
    callback.writeLine('serverLog', 'PYTHON - acPostProcForPut() begin')
    callback.irods_rule_language_printme('apples')
    callback.writeLine('serverLog', 'PYTHON - acPostProcForPut() end')

def python_printme(rule_args, callback, rei):
    callback.writeLine('serverLog', 'PYTHON - python_printme() begin')
    callback.writeLine('serverLog', 'python_printme [' + rule_args[0] + ']')
    callback.writeLine('serverLog', 'PYTHON - python_printme() end')
```

example.re:
```
irods_rule_language_printme(*thestring){
    writeLine("serverLog","irods_rule_language_printme [" ++ *thestring ++ "]")
    python_printme('bananas');
}
```

Put a file:
```
$ iput foo
```

The resulting rodsLog will have the following output:
```
Aug  8 20:57:43 pid:23802 NOTICE: writeLine: inString = PYTHON - acPostProcForPut() begin
Aug  8 20:57:43 pid:23802 NOTICE: writeLine: inString = irods_rule_language_printme [apples]
Aug  8 20:57:43 pid:23802 NOTICE: writeLine: inString = PYTHON - python_printme() begin
Aug  8 20:57:43 pid:23802 NOTICE: writeLine: inString = python_printme [bananas]
Aug  8 20:57:43 pid:23802 NOTICE: writeLine: inString = PYTHON - python_printme() end
Aug  8 20:57:43 pid:23802 NOTICE: writeLine: inString = PYTHON - acPostProcForPut() end
```
