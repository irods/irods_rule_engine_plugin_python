import itertools

__all__ = [
    "row_iterator",
    "paged_iterator",
    "AS_DICT",
    "AS_LIST",
]

MAX_SQL_ROWS = 256
AUTO_FREE_QUERIES = False

class row_return_type (object):
    def __init__(self): raise NotImplementedError

class AS_DICT (row_return_type): pass
class AS_LIST (row_return_type): pass

class bad_column_spec (RuntimeError): pass
class bad_returntype_spec (RuntimeError): pass

def irods_error_NO_ROWS_FOUND (rv):
    return (False == rv['status'] and abs(rv['code']) == 808000)

# ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
# :::::              generator-style query iterator              :::::
# ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

def row_iterator ( columns,     # comma-separated string, or list, of columns
                   conditions,  # genquery condition eg. "COLL_NAME not like '%/trash/%'"
                   row_return,  # AS_DICT or AS_LIST
                   callback,
                   auto_free = None
                   ):  # python callback object from rule argument list

    import irods_types

    if auto_free is None:
        auto_free_preferred  = AUTO_FREE_QUERIES
    else:
        auto_free_preferred  = bool(auto_free)

    if not issubclass(row_return, row_return_type):
        raise bad_returntype_spec( "row_return parameter should be AS_DICT or AS_LIST" )

    if not isinstance(columns, (list, tuple)):
        if isinstance(columns, str):
            columns = list(map(lambda obj: obj.strip(), columns.split(",")))
        else:
            raise bad_column_spec ( "Column argument '{!r}' should be column names " \
                                    "as list or comma separated string".format(columns))

    if len(columns) < 1:
        raise bad_column_spec( "Must select at least one column for the query" )

    column_indx = list(map(reversed,enumerate(columns)))
    column_lookup = dict(column_indx)

    ret_val = callback.msiMakeGenQuery(",".join(columns) , conditions , irods_types.GenQueryInp())
    genQueryInp = ret_val['arguments'][2]
    ret_val = callback.msiExecGenQuery(genQueryInp , irods_types.GenQueryOut())
    genQueryOut = ret_val['arguments'][1]
    continue_index_old = 1

    need_close = False if irods_error_NO_ROWS_FOUND(ret_val) else True

    try:

        while continue_index_old > 0:

            if irods_error_NO_ROWS_FOUND(ret_val):
                need_close = False
                break

            for j in range(genQueryOut.rowCnt):
                row_as_list = [ genQueryOut.sqlResult[i].row(j) for i in range(len(column_indx)) ]
                if row_return is AS_DICT:
                    yield { k : row_as_list[v] for k,v in column_lookup.items() }
                elif row_return is AS_LIST:
                    yield row_as_list

            continue_index_old = genQueryOut.continueInx

            if continue_index_old > 0 :

                ret_val = callback.msiGetMoreRows(genQueryInp , genQueryOut, 0)
                genQueryOut = ret_val['arguments'][1]

    finally: # for 1) GeneratorExit (on generator close())
             # or  2) normal exit

        if need_close and auto_free_preferred:
            callback.msiCloseGenQuery(genQueryInp, genQueryOut)

# ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
# ::: paged iterator returns a list of up to N rows each iteration :::
# ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

class paged_iterator (object):

    Auto_Free_Queries = AUTO_FREE_QUERIES

    def __init__(self, columns, conditions, row_return, callback,
                 N_rows_per_page = MAX_SQL_ROWS, auto_free = None
                 ):

        if auto_free is not None:
            self.Auto_Free_Queries = bool(auto_free)

        self.callback = callback
        self.set_rows_per_page ( N_rows_per_page , hard_limit_at_default = True )
        self.generator =  row_iterator (columns, conditions, row_return, callback, self.Auto_Free_Queries)

    def __iter__(self): return self
    def __next__(self): return self.next()

    def set_rows_per_page (self, rpp, hard_limit_at_default = False):

        self.rows_per_page = max (1, int(rpp))

        if hard_limit_at_default:
            self.rows_per_page = min( self.rows_per_page,  MAX_SQL_ROWS )

        return self

    def next_arbitrary_size(self):

        this_page = list(itertools.islice(self.generator, self.rows_per_page))
        if 0 == len(this_page) : raise StopIteration
        return this_page

    def next(self):

        if self.rows_per_page > MAX_SQL_ROWS:

            return self.next_arbitrary_size()

        else:

            results = list( range(self.rows_per_page) )
            j = 0

            try:
                while j < len(results):
                    results[j] = next(self.generator)
                    j += 1
            except StopIteration:
                if j == 0: raise
            except Exception as e:
                self.callback.writeLine('serverLog',
                  '*** UNEXPECTED Exception in genquery.py paged iterate [next()]')
                self.callback.writeLine('serverLog','***   {!r}'.format(e))
                raise

            # -- truncate list size to fit the generated results
            del results[j:]

            return results


def logPrinter(callback, stream):
  return lambda logMsg : callback.writeLine( stream, logMsg )

#
#  The rule code below takes (like_path_rhs , requested_rowcount) as arguments via "rule_args" param
#
#    like_path_rhs :     {collection}/{dataname_sql_pattern} eg: '/myZone/home/alice/%'
#    requested_rowcount : '<integer>' (for #rows-per-page) or '' (use generator)
#    detailed logging stream : one of [ 'serverLog', 'stdout', 'stderr', '' ]

def test_python_RE_genquery_iterators( rule_args , callback, rei ):

    cond_like_RHS = rule_args[0]
    coll_name = "/".join(cond_like_RHS.split('/')[:-1])
    data_obj_name_pattern = cond_like_RHS.split('/')[-1]

    requested_rowcount = str( rule_args[1] )

    if rule_args[2:] and rule_args[2]:
        logger = logPrinter(callback, rule_args[2])
    else:
        logger = lambda logMsg: None

    conditions = "COLL_NAME = '{0}' AND DATA_NAME like '{1}'".format(
                  coll_name , data_obj_name_pattern )
    nr = 0
    n = 0

    last_page = None

    lists_generated  = 0
    lists_remainder = 0

    if len(requested_rowcount) > 0:

        rows_per_page = int(requested_rowcount)

        logger( "#\n#\n__query: (%d) rows at a time__"%(rows_per_page,) )
        for page in paged_iterator("DATA_NAME,DATA_SIZE" , conditions, AS_DICT, callback,
                                                         N_rows_per_page = rows_per_page ):
            i = 0
            logger("__new_page_from_query__")
            for row in page:

                logger( ("n={0},i={1} ; name = {2} ; size = {3}"
                        ).format(n, i, row['DATA_NAME'], row['DATA_SIZE'] ))
                n += 1
                i += 1
                nr += 1

            lists_generated += 1
            last_page = page

    else:

        logger( "__gen_mode_return_all_rows_from_query__" )

        for row in row_iterator( "DATA_NAME,DATA_SIZE" , conditions, AS_DICT, callback ):

            logger( ("n = {0} ; name = {1} ; size = {2}"
                          ).format( n, row['DATA_NAME'], row['DATA_SIZE'] ))
            n += 1
            nr += 1

    if lists_generated > 0:
        lists_remainder = len( last_page )

    rule_args[0] = str( nr ) # -> fetched rows total
    rule_args[1] = str( lists_generated )
    rule_args[2] = str( lists_remainder )

