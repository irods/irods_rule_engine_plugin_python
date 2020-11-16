import irods_query as _irods_query
from irods_query import (query_iterator, vector_of_string, query_type)
from irods_query_wrapper import * # --> IrodsQuery
import re,os
from pprint import pprint,pformat

#__ demo / test __

def qc2gmain(arg,cbk,rei):

    if arg[:1] == [ '0', ]:

        qGen  = "select USER_GROUP_ID,USER_GROUP_NAME where USER_NAME = 'dan' and USER_GROUP_NAME != 'rodsgroup'" 
        q_with_fields = IrodsQuery( rei.rsComm, qGen, auto_fields = ('gpid','gpnm'), query_type_ = query_type.GENERAL )
        for row in q_with_fields:
            cbk.writeLine('stderr', repr(row.as_dict()))

    elif arg[:1] == [ '1', ]:

        q = IrodsQuery( rei.rsComm, 'select COLL_NAME,DATA_NAME', query_type_ = query_type.GENERAL )
        for colldata in q:
            cbk.writeLine('stderr','{0}/{1}'.format(*colldata))

def qc2dmain(arg,cbk,rei):
    i = IrodsQuery( rei.rsComm, 'listGroupsForUser', auto_fields = ('GrpID','GrpName'), bind_args = ('dan',))
    for z in i:
        cbk.writeLine('stderr','id={GrpID}. name={GrpName}.'.format(**z))

def qc2main(arg,cbk,rei):
    i = IrodsQuery( rei.rsComm, 'listGroupsForUser', bind_args = ('dan',))
    for x,y in i:
        cbk.writeLine('stderr','{x},{y}'.format(**locals()))

def qc1main(arg,cbk,rei):
    i = IrodsQuery( rei.rsComm, 'sq1y1', bind_args = ('dan',))
    for x, in i:
        cbk.writeLine('stderr','{x}, '.format(**locals()))

def qcmain(arg,cbk,rei):
    i = IrodsQuery( rei.rsComm, 'sq2arg', bind_args = ('dan','rodsgroup'))
    for x,y in i:
        cbk.writeLine('stderr','{x}, {y}'.format(**locals()))


def ACTIVATE_VIRT_ENV(File=None):
  import sys
  ctx={'__file__':File}
  if sys.version_info >= (3,):
    exec(open(File,'r').read(),ctx)
  else:
    execfile(File,ctx)

def fff(*x):
  ACTIVATE_VIRT_ENV('/home/daniel/py2/bin/activate_this.py')
  import h5py
def ggg(*x):
  import h5py

'''
sq1y1
select group_user_id from R_USER_GROUP ug inner join R_USER_MAIN u on ug.group_user_id = u.user_id where user_type_name = 'rodsgroup' and ug.user_id = (select user_id from R_USER_MAIN where user_name = ? and user_type_name != 'rodsgroup')
----
sq1
select group_user_id, user_name from R_USER_GROUP ug inner join R_USER_MAIN u on ug.group_user_id = u.user_id where user_type_name = 'rodsgroup' and ug.user_id = (select user_id from R_USER_MAIN where user_name = 'dan' and user_type_name != 'rodsgroup')
----
sq2arg
select group_user_id, user_name from R_USER_GROUP ug inner join R_USER_MAIN u on ug.group_user_id = u.user_id where user_type_name = 'rodsgroup' and ug.user_id = (select user_id from R_USER_MAIN where user_name = ? and user_type_name != ?)
'''


def qmain(arg,cbk,rei):
    args =  vector_of_string();
    args.assign( ['dan','rodsgroup'] ) 
    qit = query_iterator( rei.rsComm, 
                          'sq2arg', 
                          args, "", 0, 0, _irods_query.query_type.SPECIFIC )
    qi = iter(qit)
    try:
      while True:
        y = next(qi)
        len(y)
        cbk.writeLine('stderr','id = {y[0]} ; name = {y[1]}'.format(**locals()) )
    except StopIteration:
      print('--stopped--')

def qqmain(arg,cbk,rei):
    q_general  = "select USER_GROUP_ID, USER_GROUP_NAME where USER_NAME = 'dan' and USER_GROUP_NAME != 'rodsgroup'" 
    q_specific = "select group_user_id, user_name from R_USER_GROUP ug inner join R_USER_MAIN u on ug.group_user_id" \
                 " = u.user_id where user_type_name = 'rodsgroup' and ug.user_id = (select "                         \
                 "user_id from R_USER_MAIN where user_name = ? and user_type_name != 'rodsgroup')"
    args =  vector_of_string();
    empty =  vector_of_string();
    args.assign( ['dan','rodsgroup'] ) 
    n_arg =  arg[0]
    if n_arg == "2":
      # -- general query
      qi = query_iterator( rei.rsComm, q_general )
    elif n_arg == "5":
      # -- specific but no args passed
      # test with: iadmin asq "<sql>" sq1 where sql like above but "?" replaced by eg "'username'"
      qi = query_iterator( rei.rsComm, "sq1", 0, 0, _irods_query.query_type.SPECIFIC )
    elif n_arg == "7":
      # test with: iadmin asq "<sql>" sq2arg where sql like above but "'rodsadmin'" replaced by "?"
      qi = query_iterator( rei.rsComm, 
                           'sq2arg', 
                           args, "", 0, 0, _irods_query.query_type.SPECIFIC )
    elif n_arg == "7g":
      qi = query_iterator( rei.rsComm, 
                           'select DATA_ID', 
                           empty, "", 0, 0, _irods_query.query_type.GENERAL )
      for _ in qi: 
        cbk.writeLine('stderr',(_)[0])
      return
    for y in qi:
      cbk.writeLine('stderr','id = {y[0]} ; name = {y[1]}'.format(**locals()) )
