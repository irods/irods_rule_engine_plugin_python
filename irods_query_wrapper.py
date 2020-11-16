import irods_query as _irods_query
import re

class IrodsQuery(object):

    class Row( object ):
        def keys(self): return self.columns
        def as_dict(self): return { k : self[k] for k in self.keys() }
        def __init__(self,values,columns):
            self.values = values
            self.columns = columns
            self.columns_dict = dict(zip(columns,values))
        def __iter__(self):
            return iter(self.values)
        def __getitem__(self,n):
            if isinstance(n,int):   return self.values[n]
            elif isinstance(n,str): return self.columns_dict[n]
            else:                   raise KeyError

    from irods_query import (query_iterator, vector_of_string)

    GENERAL  = _irods_query.query_type.GENERAL
    SPECIFIC = _irods_query.query_type.SPECIFIC

    __attributes = re.split('\s+', 'comm zone_hint query_string auto_fields query_type query_limit row_offset bind_args')

    def __init__(self, comm,             # rei.rsComm from rule parameter
                       query_string,     # "select FIELD1,FIELD2 from ... where ... "
                       auto_fields = (), # Up to n dictionary-like keys to correspond with FIELD(0..n-1)
                       zone_hint = "",
                       query_limit = 0,
                       row_offset = 0,
                       query_type = _irods_query.query_type.GENERAL, # IrodsQuery.GENERAL
                       bind_args = () ): # args for ? ; only if query_type is SPECIFIC
        self.__qi = None
        for x in self.__attributes:
            setattr(self,x,locals()[x])

    def __iter__(self):
        if self.__qi is None:
            self.__args = self.vector_of_string()
            self.__args.assign(self.bind_args)
            self.__qi = iter( self.query_iterator( self.comm,
                                                   self.query_string,
                                                   self.__args, "", 0, 0, self.query_type))
        return self

    def next(self):
        try:
            n = next( self.__qi )
        except StopIteration as e:
            self.__qi = None
            raise
        else:
            return self.Row(n, self.auto_fields)

    @property 
    def args(self): return tuple(self.__args)

# -- usage in core.py --
#
# from irods_query_wrapper import ( IrodsQuery )
# 
# def gen_q(arg,cbk,rei):
#     for (i,d,c) in IrodsQuery (rei.rsComm, "select DATA_ID,DATA_NAME,COLL_NAME"):
#         cbk.writeLine ("stdout", "id = %s : %s/%s" % (i,c,d) )
# 
# def spec_q(arg,cbk,rei):
#     for row in IrodsQuery( rei.rsComm, "listGroupsForUser",
#                            bind_args=('dan',),
#                            auto_fields=('grp_id','grp_name'), 
#                            query_type = IrodsQuery.SPECIFIC
#                          ):
#         cbk.writeLine ("stdout", "Group ID {grp_id} NAME {grp_name}".format(**row))
