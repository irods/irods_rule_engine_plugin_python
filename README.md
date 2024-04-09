# iRODS Rule Engine Plugin - Python

This C++ plugin provides the iRODS platform a rule engine that allows iRODS rules to be written in Python.

# Build

Building the iRODS Python Rule Engine Plugin requires version 4.2.1+ of the iRODS software from github (http://github.com/irods/irods).  The version of the `irods-dev` package will need to match the version defined in `CMakeLists.txt`.

```
cd irods_rule_engine_plugin_python
mkdir build
cd build
cmake ../
make package
```

# Installation

The packages produced by CMake will install the Python plugin shared object file:

`/usr/lib/irods/plugins/rule_engines/libirods_rule_engine_plugin-python.so`

# Configuration

After installing the plugin, `/etc/irods/server_config.json` needs to be configured to use the plugin.

To activate the plugin, add a new stanza to the "rule_engines" array within `server_config.json`:

```json
{
    "instance_name": "irods_rule_engine_plugin-python-instance",
    "plugin_name": "irods_rule_engine_plugin-python",
    "plugin_specific_configuration": {}
}
```

Adding the Python Rule Engine Plugin stanza above the default "irods_rule_engine_plugin-irods_rule_language" stanza will allow any defined Python rules to take precedence over any similarly named rules in the iRODS Rule Language. Placing the "irods_rule_engine_plugin-python-instance" stanza before the "irods_rule_engine_plugin-cpp_default_policy-instance" stanza will ensure that any default C++ policies will not cancel out any similarly named Python rules.

As soon as the stanza is inserted, the iRODS server expects a Python source code module to exist at the path `/etc/irods/core.py`. Once imported by the iRODS server and successfully compiled to a Python byte-code file bearing the ".pyc" extension, any functions in the module will be callable as rules as long as they conform to a calling convention
```
def a_python_rule( rule_args, callback, rei ):
    # ... Python code to be executed in the rule context...
```

The parameters need not be explicit or even named as they are here; they could in fact be collapsed into a parameter tuple, as in **pyfn(\*x)**.

The first argument above, `rule_args`, is a tuple containing the optional, positional parameters fed to the rule function by its caller; these same parameters may be written to, with the effect that the written values will be returned to the caller.  The `callback` object is effectively a gateway through which other rules (even those managed by other rule engine plugins) and microservices may be called. Finally, `rei` (also known as "rule execution instance") is an object carrying iRODS-related context for the current rule call, including any session variables.

When the plugin is freshly installed, a template file is located at `/etc/irods/core.py.template`. This may be copied to `/etc/irods/core.py`, or else an empty `core.py` can be created at that path if the user prefers to begin from scratch.

# Default PEPs

The example `core.py.template` file in this repository contains a Python implementation of all static policy enforcement points (PEPs), equivalent to the default `core.re` rulebase. Placing the "irods_rule_engine_plugin-python-instance" stanza before the "irods_rule_engine_plugin-cpp_default_policy-instance" stanza in `/etc/irods/server_config.json` will ensure that any default C++ policies will not cancel out any similarly named Python rules copied from the example `core.py.template` file.

# Python Version

This version of the Python Rule Engine Plugin uses the Python 2.7 interpreter.

# Remote Execution

There exists a requirement for the implementation of a different `remote` microservice call for every rule language.  Given the possibility of a namespace collision with more than one rule language being configured simultaneously, the name of the microservice to use for the python language is `py_remote()`.  As with remote execution via the native rule engine, this microservice runs the given rule text on the remote host using `exec_rule_text`.   This can be done on any iRODS host (inside or outside the local zone) where the invoking user is authenticated.

The microservice's signature is: `py_remote(hostname, hints, code, recovery)`.

Its four parameters are strings:
   - `hostname`. The name of the iRODS server where the code is to be executed.
   - `hints`.  A string containing optional XML tags. Currently scanned only for the `INST_NAME` tag, and other tags - if used - will be ignored. This
      includes <ZONE>, since the zone (whether remote or local) is inferred from the `hostname` parameter.
   - `code`. The Python source code to be executed, in the form of a complete rule, i.e.: `def main(rule_args,callback,rei): ...`
   - `recovery`.  This is currently unused.

For example:
```
def main(rule_args, callback, rei):
    rule_code = "def main(rule_args, callback, rei):\n    print('This is a test of the Python Remote Rule Execution')"
    callback.py_remote('icat.example.org', '<INST_NAME>irods_rule_engine_plugin-python-instance</INST_NAME>', rule_code, '')
INPUT null
OUTPUT ruleExecOut
```

# Admin rights required for irule with Python

Due to the Python language not being sandboxed (and running as the unix iRODS service account), `irule` does not allow rulefiles written in Python to be executed by non-`rodsadmin` user accounts.  If this is attempted, the following error message will be returned to the client:
```
Insufficient privileges to run irule in Python rule engine plugin
```

If Python rules are needed by non-`rodsadmin` users, then iRODS administrators can install those Python rules on the server side (in `core.py`, or imported by `core.py`) where they will be made available to `irule` via the `callback` mechanism (see below).


# Calling rules across language boundaries

The following example rulefiles (iRODS Rule Language and Python) demonstrate calling microservices in both directions when reacting to a simple `iput` (the `acPostProcForPut` PEP is fired in Python).

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

# Python globals and built-in modules
These built-in objects are set up at plugin initialization time and available in the python interpreter that loads `core.py` or, as the case may be, the Python rule file:
   - `irods_rule_vars` - a dictionary for accessing variables of the form `*var` from the `INPUT` line, if present.
   - `irods_types` - a module containing common struct types used for communicating with microservices.
   - `irods_errors` - a module mapping well-known iRODS error names to their corresponding integer values.
   - `global_vars` - deprecated alias for `irods_rule_vars`; only available in some contexts. Will be removed in a future release.
   
By using the `irods_errors` built-in, policy may indicate how the framework is to continue through the use of a symbolic name, rather than a cryptic number reference:
```
def pep_api_data_obj_put_pre(args, callback, rei):
  # ...local processing...
  return irods_errors.RULE_ENGINE_CONTINUE  # compare: "return 5000000"
```
Finally, the following example demonstrates the use of all three of these built-ins.
With the below rule contained anywhere in the Python rulebase:
```
def testRule(rule_args, callback, rei):
  from irods_errors import USER_FILE_DOES_NOT_EXIST
  check_err = lambda e,errcode: e.message.startswith('[iRods__Error__Code:{}]'.format(errcode))
  try:
    ret_val = callback.msiObjStat(rule_args[0], irods_types.RodsObjStat())
  except RuntimeError, e:
    if check_err(e, USER_FILE_DOES_NOT_EXIST):
      callback.writeLine('serverLog', 'ERROR in testRule: "'+rule_args[0]+'" not found')
    else:
      callback.writeLine('serverLog', 'UNKNOWN error')
    rule_args[0] = ''
  else:
    objstat_return = ret_val['arguments'][1]
    rule_args[0] = 'mtime = {!s} ; objsize = {!s} '.format(objstat_return.modifyTime, objstat_return.objSize)
```
and a rule script file (named `call_testRule.r`) which calls `testRule` with a data path argument:
```
def main(_,callback,rei):
  data_path = irods_rule_vars[ '*dataPath' ][1:-1]
  retval = callback.testRule( data_path )
  callback.writeLine( "stdout", retval['arguments'][0])
INPUT *dataPath=""
OUTPUT ruleExecOut
```
we can invoke the rule script and  point `*dataPath` at a trial data object path:
```
irule -r irods_rule_engine_plugin-python-instance -F call_testRule.r '*dataPath="/full/path/to/data/object"'
```
and thus determine a data object's existence, as well as its mtime and size, if it does exist.

# Auxiliary Python Modules

Included with the PREP (Python Rule Engine Plugin) are some other modules that provide a solid foundation of utility for writers of Python rule code.  The plugin directly loads only the module `/etc/irods/core.py`, however any import statements in that file are honored if the modules they target are in the interpreter's import path (`sys.path`).  In addition, a `rodsadmin` irods user may use `irule` to execute Python rules within `.r` files.  By default, `/etc/irods` is included in the import path, meaning that the modules discussed in this section are accessible to all other Python modules and functions (whether or not they are "rules" proper) whether they be internal to `core.py`, or otherwise loaded by the PREP.

## `session_vars.py`

This module contains a function `get_map` which may be used to extract session variables from the `rei` parameter passed to any rule function.

An example follows:

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

## Special methods

Some iRODS objects used with rules and microservices have been given utility methods.

An example is the map() method of a `PluginContext` PEP argument:
```
    def pep_resource_resolve_hierarchy_pre(rule_args, callback, rei):
        import pprint
        pprint.pprint(rule_args[1].map().items())
```

In version 4.2.11.2 of the Python plugin, `BytesBuf` has methods to set and access byte content.
These can be used to achieve true binary reads and writes, to and from data objects.

For example:
```
    def my_rule(args, callback, rei):
        buf = irods_types.BytesBuf()
        buf.set_buffer( os.urandom(256) )
        callback.msiDataObjectWrite( descriptor, buf, 0)  #-- Write the buffer.
        # ...
        buf.clear_buffer()
```
or:
```
    # ...
    retv = callback.msiDataObjectRead( descriptor, "256", irods_types.BytesBuf())
    returnbuf = retv['arguments'][2]
    out = returnbuf.get_bytes()         #--  Returns list of integer octets.
    start_byte = returnbuf.get_byte(0)  #--  Returns first octet
```



## `genquery.py`

This module offers writers of Python rules a convenient facility for querying and iterating over objects in the iRODS object catalog (ICAT).
The only declaration required within your `core.py` or other Python rule file is:
```
from genquery import *
```
Previous versions of the `genquery.py` module offered the following two styles of iterator to expose iRODS' General Query capability:

   * `row_iterator`, a generator function which yields results one row at a time:
   ```
   def my_rule(rule_args,callback,rei):
       for result in row_iterator("COLL_NAME,DATA_NAME",
                                  "DATA_NAME like '%.dat'",
                                  AS_LIST,
                                  callback):
           callback.writeLine( "serverLog", result[0] + "/" + result[1] )
   ```
   * `paged_iterator`, an iterator class that enables paging through a total of `nResults` rows from the ICAT in lists of `1 <= n <= N` at a time, with
      - `N` a specified page size in the range ``1...256`` inclusive;
      - and `n < N` only at the end of the iteration when `nResults == 0` or `nResults % N != 0`:

   ```
   def get_users_in_group(args,callback,rei):
       groupname = args[0]
       users = []
       for rows in paged_iterator("USER_NAME,USER_ZONE",
                                     "USER_GROUP_NAME = '{}'".format(groupname),
                                     AS_DICT, callback, 32 ):
           users += ["{USER_NAME}#{USER_ZONE}".format(**r) for r in rows]
       callback.writeLine('stdout',repr(users))
```

As of version 4.2.8 of iRODS, there is also a class-based iterator called `Query`, which offers an enhanced set of options and behaviors for use in ICAT searches.

Under the new class-based syntax, an iRODS general query can be as simple as in this example:

```
all_resource_names = [ r for r in Query(callback,'RESC_NAME') ]
```

the result of which will be a Python `list` of `str` values, enumerating the names of all resource nodes known to the ICAT.

`Query`'s constructor allows commonly defaulted attributes (otherwise tunable via a selection of keyword arguments, listed in the [reference](#queryref) at the end of this section) to take on reasonable values.  In particular, the *output* attribute (dictating the type of data structure used to return individual row results) will, unless specified, default to **AS_TUPLE** -- meaning that for single-column query results the iterator variable does not need to be indexed.  That is why, in our first `Query` example, the values yielded by the iteration were strings.

This allows for an admirable consistency of expression, as can be seen in the following two examples:

```
    for id in Query(callback, "DATA_ID") :
        pass # - process data id's for iterated objects
```  
and
```
    for coll,data in Query(callback,("COLL_NAME", "DATA_NAME")):
        pass # - process result data objects using the logical path: (coll + "/" + data)
```
A caveat for beginners however, when expanding on such deceptively simple syntax, is that the iterations in the above two example usages yield essentially different row types.  In the first usage, the iteration variable `id` is of type `str` (string); and in the second, a `tuple` is actually returned from each iteration and then automatically unpacked as the variables `coll` and `data`.

*If the uniformity of the result rows' data structure type is desirable for a given application of `Query(...)`, then `output=AS_LIST` or `output=AS_DICT` should be favored to the default `output=AS_TUPLE`. For more on this and other query attributes, see the [reference](#queryref) at the end of this section.*

Clones and Copies
---
The `Query` class features lazy execution, meaning that only the act of iterating on a `Query` object will actually connect to the ICAT for the purpose of a general query. This has some advantages, particularly those of cheap instantiation and duplication.

Specifically, the `copy` method can be invoked on any `Query` object to instantiate a new one, and this can be done either with or without keyword arguments.  Absent any input arguments, a call to the `copy` method simply clones the original object.

The keyword arguments to Query's `copy` method, if used, are identical to those used by the constructor, and allow modification of the corresponding attributes within the new object. In this way, a `Query` object can easily serve as a bag of *attribute* variables which can be stored, transferred, modified, and re-used.

Because iteration on a `Query` object can change its state, a best practice is to favor invoking the `copy` method wherever repeated (esp. overlapping) uses are possible for the same Query instance.  This is particularly true for a `Query` object that has been stored in a Python object or variable, so as to give it a longer lifetime than a temporary instance.

To demonstrate how this could look in practice, let's incorporate a more elaborate version of the search from our first `Query` example within the following helper function:

```
def get_resource_details(callback,*args):
    (cond_string,) = args[:1] if args else ('',)
    my_query = Query( callback,
                      columns = ['RESC_NAME'],
                      conditions = cond_string )
    n_nodes = my_query.copy().total_rows()
    if n_nodes == 0: return []

    details = my_query.copy( columns = \
                               my_query.columns + ['RESC_ID','RESC_PARENT'] )
    return [
      "Resource '{}': id = {}, parent-id = {}".format(row[0],row[1],row[2])
      for row in details
    ]
```
which could later be called thus:
```
def my_resc_summaries(_,callback,rei):
    import socket; hostname = socket.gethostname()
    from pprint import pformat
    # -- all resources listed in the ICAT --
    callback.writeLine('stdout',pformat(    get_resource_details(callback)
                                )+"\n---\n")
    # -- all resources on the local machine --
    callback.writeLine('stdout',pformat(    get_resource_details(callback,
                                            "RESC_LOC = '{}'".format(hostname))
                                )+"\n---\n")
    # -- all resources having no parent node -- (ie root resources)
    callback.writeLine('stdout', pformat(   get_resource_details(callback,
                                                                 "RESC_PARENT = ''")))
    #...
```

Auto joins
---
In iRODS's general queries, joins between related tables are automatic.  For example, queries whose column targets including both `DATA_*` and `COLL_*` fields implicitly perform "auto-joins" between the Data objects queried and their parent Collections. Metadata AVU's are also joined automatically with the corresponding type of iRODS object which they annotate:

```
q = Query( callback, columns = ['DATA_ID,META_DATA_ATTR_VALUE'], output = AS_DICT, conditions = """\
            META_DATA_ATTR_NAME = 'modify_time' and \
            META_DATA_ATTR_VALUE like '0%'
    """)
number_of_data_objects = q.total_rows() # counting only, no fetch operations
q_fetch = q.copy( q.columns + ["COLL_NAME", "order(DATA_SIZE)"] )
for row in q_fetch:
    log_record = """id = {DATA_ID} ; size = {order(DATA_SIZE)} ; path = \
                    {COLL_NAME}/{DATA_NAME}; mtime = {META_DATA_ATTR_VALUE}\
                 """.format(**row)
    callback.writeLine("serverLog",log_record)
```

Query Options
---

The `options` Query attribute is a mask of bit-field values which can be built up from constants in the `genquery.Options` namespace, although probably the only directly useful option is `NO_DISTINCT`.  This option might come into play in the rare case that we decide against iRODS' usual behavior of requiring all row results to be "distinct" (in other words, distinguishable combinations of values for the columns selected):

```
from genquery import * ; from genquery import Option
y = len([ row for row in Query(callback , 'META_DATA_ATTR_NAME',
                               """META_DATA_ATTR_NAME = 'aa' and \
                                  META_DATA_ATTR_UNITS = '' """,
                               options=Option.NO_DISTINCT) ])
```
The above example enumerates metadata AVU's (both used and unused) having the attribute name of 'aa' and no units -- and, by employing the NO_DISTINCT option, suppresses the normal de-duplicating behavior of the general query.  Without the option, any subset of row results undiscernable as being different would be collapsed into a single row result.  (Another example where this might be important would be if multiple data objects had the same values in the DATA_NAME and/or DATA_SIZE columns, without a COLL_* or DATA_ID column also being selected to establish uniqueness in an alternative way.)

Strict upward compatibility mode
---
As of iRODS 4.2.8, the `genquery` module's `row_iterator` function returns a `Query` instance instead of the generator object of previous versions. This should not affect existing Python rule code that depends on the function unless its return value is expected to follow the generator interface -- allowing, for example, iteration via Python interpreter's `next()` built-in.  The great majority of Python statements and expressions availing themselves of this simple row-iterating facility will use the more convenient and succinct form:
```
  for row in row_iterator(...)
```

But in the event of an incompatibility, users can **modify their import declarations as follows** to accommodate both new and old interfaces simultaneously:

   - Use
     ```
     from genquery import *
     from genquery import (row_generator as row_iterator)
     ```
     in place of the normal
     ```
     from genquery import *
     ```

   - or, in modules not using the wildcard import syntax, add
     ```
     from genquery import row_generator
     ```
     and then replace any problematic uses of `row_iterator` or `genquery.row_iterator` with `row_generator`.

This should solve the very rare issue of such dependencies.

<a name="queryref"></a>
`Query` object creation and attributes Reference
---
A `Query` object's *attributes* (ie "columns", "conditions", ... ) set constraints on how an iRODS general query should be carried out and how the results should be enumerated.  These *attributes* are defined by keyword arguments of the same name, detailed below.

The `Query` object constructor requires these two arguments to be first, and in the order listed:

  * *callback* (not really an attribute) can only be used in the constructor.  It is just the callback argument from the enclosing Python rule function's input argument list.
  * *columns*  is a either a string with comma-separated column names, or a `list` or `tuple` of column name. If provided among the `copy` method's keyword arguments, it must however be of type `list`. 

The following arguments are strictly optional and may be used either in `Query`'s constructor or in the `copy` method:

  * *condition*  is the "where ..." predicate traditionally used in the general query syntax, but omitting the `where` keyword. The default (an empty string) allows for opting out of the search condition, in which case all objects of the requested type, or joined list of types,  will be returned (ie. Collections for `COLL_*`, Resources for `RESC_*`)
  * *output* is one of: `AS_TUPLE`, `AS_DICT` or `AS_LIST`. These constants are defined in the genquery module, and they specify the Python data structure type to be used for returning individual row results.
    - `AS_TUPLE`, the default, lets results of a single-column query be rendered simply as one string value per row returned; or, if multiple column names are specified, as a tuple of strings, indexed by a zero-based integer column offset. (Note that if a more programmatically consistent interface is desired, ie. indexing each row result by integer offset even for the case of a single column, then `AS_LIST` should be preferred.)
    - `AS_LIST` forces a column value within each row to be indexed by its zero-based integer position among the corresponding entry in the ***columns*** attribute .
    - `AS_DICT` lets column values be indexed directly using the column name itself (a string). The
      seemingly higher convenience of this approach is, however, paid for by an increased **per-row** execution overhead.
  * *offset*: 0 by default, this `Query` attribute dictates the integer position, relative to the complete set of possible rows returned, where the enumeration of query results will start.
  * *limit*: `None` by default (ie "no limit"), this option can be an integer >= 1 if specified, and limits the returned row enumeration to the requested number of results. Often used with *offset*, as defined above.
  * *case-sensitive* is normally `True`. If it is set to `False`, the condition string will be uppercased, and the query will be executed without regard to case.  This allows for more permissive matching on the names of resources, collections, data objects, etc.
  * *options* is a bitmask of extra options, and defaults to a value of 0. `genquery.Option.NO_DISTINCT` is one such extra option, as is RETURN_TOTAL_ROW_COUNT (although in the latter case, using the `Query` object's `row_count` method should be preferred.)

## Questions and Answers

### What happened to my `print` output?

In iRODS server versions 4.3.0 and later, all standard streams are redirected to `/dev/null` in the server. This means that any data sent to `stdout` such as via a `print` statement in a rule run inside the server will be discarded.

Here is some example code which would print a message to the `rodsLog` in servers from the 4.2.x series and earlier when run with the Python Rule Engine Plugin:
```python
print('hello, server log!')
```

In order to achieve a similar effect on server versions 4.3.0 and later, you can replace such a call like this:
```python
callback.writeLine('serverLog', 'hello, server log!')
```

Note that the `writeLine` microservice can also target `stdout` in addition to `serverLog`. `callback.writeLine('stdout', ...)` will have the same effect as running `print`. The output will be discarded.

### How do I pass values back to the caller, across rule engine plugin boundaries?

This is achieved by writing a value back _(as a string)_ to the appropriate index within the `rule_args` array. The caller is required to pass one or more variables as **out** parameters.

Here's an example demonstrating how to pass a value, via the input arguments, from the PREP to the native rule engine plugin (NREP).

The following is a python function which overwrites the value of the first input argument.
```python
# file: core.py

def get_magic_number(rule_args, callback, rei):
    # Notice the integer is represented as a string.
    # Only strings can be passed across rule engine plugin boundaries.
    #
    # Take note of the index we're writing to. The index represents the
    # position of the input argument. Because this rule expects one input
    # argument, we use index 0. If there were more input arguments, we could
    # write to those as well using the index which identifies them.
    rule_args[0] = '42'
```

And here is the NREP rule which invokes the PREP rule.
```python
# file: core.re

nrep_rule()
{
    # Due to how the NREP works, we must initialize the variable to a string.
    # The value of the string isn't important.
    *magic_number = '-1';

    # Now, we invoke the Python rule in core.py. Remember, the rule takes one
    # input argument. That argument maps to index 0 within the python rule.
    get_magic_number(*magic_number);

    # Finally, the value is written to the log file.
    writeLine('serverLog', 'The magic number is [*magic_number].');
}
```

It's important that the value passed across rule engine plugin boundaries be of type string. Passing any other type will result in an error.

Upon invoking `nrep_rule`, a message very similar to the one that follows will appear in the server log file. See the **log_message** property. It should contain `[42]` instead of `[-1]`.
```js
{
  "log_category": "legacy",
  "log_level": "info",
  "log_message": "writeLine: inString = The magic number is [42].\n",
  "request_api_name": "EXEC_MY_RULE_AN",
  "request_api_number": 625,
  "request_api_version": "d",
  "request_client_user": "rods",
  "request_host": "10.15.164.3",
  "request_proxy_user": "rods",
  "request_release_version": "rods4.3.1",
  "server_host": "76f890b83c4d",
  "server_pid": 2637,
  "server_timestamp": "2024-04-10T12:43:31.381Z",
  "server_type": "agent",
  "server_zone": "tempZone"
}
```
