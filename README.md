# iRODS Rule Engine Plugin - Python

This C++ plugin provides the iRODS platform a rule engine that allows iRODS rules to be written in Python.

# Build

Building the iRODS Python Rule Engine Plugin requires version 4.2.1+ of the iRODS software from github (http://github.com/irods/irods).

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

Python rules will be loaded from `core.py` (located at `/etc/irods/core.py.template` in a default installation).

To activate the plugin, add a new stanza to the "rule_engines" array within `server_config.json`:

```json
{
    "instance_name": "irods_rule_engine_plugin-python-instance",
    "plugin_name": "irods_rule_engine_plugin-python",
    "plugin_specific_configuration": {}
}
```

Adding the Python Rule Engine Plugin stanza above the default "irods_rule_engine_plugin-irods_rule_language" stanza will allow any defined Python rules to take precedence over any similarly named rules in the iRODS Rule Language.

As soon as the stanza is inserted, the iRODS core expects a Python source code module to exist at the path `/etc/irods/core.py`. Once imported by the iRODS server and successfully compile to a Python byte-code file bearing the ".pyc" extension, any functions in the module will be callable as rules as long as they conform to a  calling convention
```
def a_python_rule( rule_args , callback , rei ):
  # ... Python code to be executed in the rule context...
```
although the parameters need not be explicit or even named as they are here; they could in fact be collapsed into a parameter tuple, as in **pyfn(\*x)**. The first argument above, `rule_args`, is a tuple containing the optional, positional parameters fed to the rule function by its caller; these same parameters may be written to, with the effect that the written values will be returned to the caller. The `callback` object is effectively a gateway through which other rules (even those managed by other rule engine plugins) and microservices may be called. Finally, `rei` (also known as "rule exec info") is an object carrying iRODS-related context for the current rule call, including e.g. session variables.

When the plugin is freshly installed, a file `core.py.template` will exist in the directory
`/etc/irods`. This may be copied to `/etc/irods/core.py` , or else an empty `core.py` can be created at that path if the user prefers to begin from scratch.

# Minor Notes to the Python RE Plugin

- The example `core.py.template` file in this repository contains a Python implementation of all static policy enforcement points (PEPs) from a default `core.re` rulebase.

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

# Admin rights required for irule with Python

Due to the Python language not being sandboxed (and running as the unix iRODS service account), `irule` does not allow rulefiles written in Python to be executed by non-rodsadmin user accounts.  If this is attempted, the following error message will be returned to the client:
```
Insufficient privileges to run irule in Python rule engine plugin
```

If Python rules are needed by non-rodsadmin users, then iRODS administrators can install those Python rules on the server side (in `core.py`, or imported by `core.py`) where they will be made available to `irule` via the `callback` mechanism (see below).


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

- Some python identifiers, such as `global_vars` (a lookup for accessing variables of the form `*var` from the `INPUT` line, if present) and `irods_types` (a module containing common struct types used for communicating with microservices) are automatically imported into the python interpreter that executes `core.py`.
- Some python identifiers, such as `global_vars` (a lookup for accessing variables of the form `*var` from the `INPUT` line, if present) and `irods_types` (a module containing common struct types used for communicating with microservices) are automatically imported into the python interpreter that loads `core.py`.

# Auxiliary Python modules

Included with the PREP (Python Rule Engine Plugin) are some other modules that provide a solid foundation of utility for writers of Python rule code.  The plugin directly loads only the module `/etc/irods/core.py`, however any import statements in that file are honored if the modules they target are in the interpreter's import path (`sys.path`).  In addition, `rods`  admin may use `irule` to execute Python rules within `.r` files.  By default, `/etc/irods` is included in the import path, meaning that the modules discussed in this section are accessible to all other Python modules and functions (whether or not they are "rules" proper) whether they be internal to `core.py`, or concomitantly or subsequently loaded by the PREP.

## `session_vars.py`
This module contains a function `get_map` which may be used to extract session variables from the `rei` parameter passed to any rule function. An example follows:

```
import session_vars
def get_irods_username (rule_args, callback, rei):
  username = ''
  var_map = session_vars.get_map(rei)
  user_type = rule_args[0] or 'client_user'
  userrec = var_map.get(user_type,'')
  if userrec:
    username = userrec.get('user_name','')
  rule_args [0] = username
```

## `genquery.py`
Two styles of Python iterator are offered by this module to allow a convenient way
of accessing GenQuery results from within Python rules.  One iterator allows paged
results of N rows at a time over each iteration (returning between 1 and 256
rows at a time in the form of a Python list):

```
  iter = paged_iterator (
           ["sum(DATA_SIZE)"], "DATA_NAME like 'file_%.dat'",
           False, # says we want an integer-indexed row, not key-value lookup
           callback , N_rows_per_page=1
  )
  for row in iter:  # ---> one row returned with sum of size of matching data objects
    writeLine('serverLog',
      'result = {}'.format( row[0] ))
```

while the other returns each row result via a Python generator object:

```
  for dObj in row_iterator("DATA_NAME,DATA_SIZE" , conditions, True, callback):
      callback.writeLine ("serverLog",
        "name = {0} ; size = {1}" . format( dObj['DATA_NAME'], dObj['DATA_SIZE'] )
      )
```

Whichever function we call to construct the iterator, the first four arguments to the call are the same:

  * ***columns*** - either a string with comma-separated column names, or a list of column names.
  * ***condition*** is the "select ... where ..." string traditionally used by the general query.
  * ***row_return_type***  is either `AS_DICT` or `AS_LIST`. These constants are  defined in the genquery module, and specify
    whether rows returned from the query will be represented by Python `list` or `dict` objects.
    - `AS_LIST` lets a column value be indexed from each row using its zero-based position (an integer) of
      the corresponding column name within the *columns* argument.
    - `AS_DICT` lets column values be indexed directly using the column name itself (a string). Note however, the
      seemingly higher convenience of this approach is paid for by an increased **per-row** execution overhead.
  * ***callback*** - duplicated from the callback argument fed into the enclosing Python rule call

For the paged iterator, there is a fifth argument ***N_rows_per_page***, which is hardlimited to the range [1,256].
