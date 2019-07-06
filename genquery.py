import itertools

__all__ = [
    "row_iterator",
    "paged_iterator",
    "AS_DICT",
    "AS_LIST",
]

MAX_SQL_ROWS = 256

AUTO_CLOSE_QUERIES = True
Report_Exit_To_Log = False

class row_return_type (object):
    def __init__(self): raise NotImplementedError

class AS_DICT (row_return_type): pass
class AS_LIST (row_return_type): pass

class bad_column_spec (RuntimeError): pass
class bad_returntype_spec (RuntimeError): pass

# ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
# :::::              generator-style query iterator              :::::
# :::::   --> yields one row  from query results per iteration   :::::
# ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

def row_iterator(  columns,     # comma-separated string, or list, of columns
                   conditions,  # genquery condition eg. "COLL_NAME not like '%/trash/%'"
                   row_return,  # AS_DICT or AS_LIST to specify rows as Python 'list's or 'dict's
                   callback     # fed in directly from rule call argument
                ):

    import irods_types

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

    column_indices = list(map(reversed,enumerate(columns)))
    column_lookup = dict(column_indices)

    ret_val = callback.msiMakeGenQuery(",".join(columns) , conditions , irods_types.GenQueryInp())
    genQueryInp = ret_val['arguments'][2]

    ret_val = callback.msiExecGenQuery(genQueryInp , irods_types.GenQueryOut())
    genQueryOut = ret_val['arguments'][1]
    continue_index_old = 1

    ret_val = callback.msiGetContInxFromGenQueryOut( genQueryOut, 0 )
    continue_index = ret_val['arguments'][1]

    exit_type = ''

    try:

        while continue_index_old > 0:

            for j in range(genQueryOut.rowCnt):

                row_as_list = [ genQueryOut.sqlResult[i].row(j) for i in range(len(column_indices)) ]

                if row_return is AS_DICT:
                    yield { k : row_as_list[v] for k,v in column_lookup.items() }
                elif row_return is AS_LIST:
                    yield row_as_list

            continue_index_old = continue_index

            ret_val = None  #-- in case of exception from msiGetMoreRows call
            ret_val = callback.msiGetMoreRows(genQueryInp , genQueryOut, 0)

            genQueryOut = ret_val['arguments'][1]
            continue_index = ret_val['arguments'][2]

    except GeneratorExit:
        exit_type = 'ITERATION_STOPPED_BY_CALLER'
    except Exception as e:
        if ret_val is None:
            exit_type = 'GET_ROWS_ERROR'
        else:
            exit_type = 'UNKNOWN_ERROR'
        continue_index = 0 # Unanticipated source of error, preserve exception as-is
        raise              # ('finally' clause runs, but no forced closing of query)
    else:
        exit_type = '(Normal)'
    finally:
        if Report_Exit_To_Log:
            callback.writeLine("serverLog","Python GenQuery exit type - {}".format(exit_type))
        if AUTO_CLOSE_QUERIES:
            while continue_index > 0:
                continue_index_old = continue_index
                ret_val = callback.msiGetMoreRows(genQueryInp , genQueryOut, 0) 
                genQueryOut = ret_val['arguments'][1]
                continue_index = ret_val['arguments'][2]


# ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
# :::::           Page-at-a-time query iterator                     ::::
# ::::: --> Yields one page of 0<N<=MAX_SQL_ROWS results as a list  ::::
# :::::     of rows. As with the row_iterator, each row is either a ::::
#::::::     list or dict object (dictated by the row_return param)  ::::
# ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

class paged_iterator (object):

    def __init__(self, columns, conditions, row_return, callback,
                 N_rows_per_page = MAX_SQL_ROWS):

        self.callback = callback
        self.set_rows_per_page ( N_rows_per_page , hard_limit_at_default = True )

        self.generator =  row_iterator (columns, conditions, row_return, callback )

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
#  Test rule below takes (like_path_rhs , requested_rowcount) as arguments via "rule_args" param
#
#    like_path_rhs :         {collection}/{dataname_sql_pattern} eg: '/myZone/home/alice/%'
#    requested_rowcount :    '<integer>' (for #rows-per-page) or '' (use generator)
#    detail logging stream : one of [ 'serverLog', 'stdout', 'stderr', '' ]
#

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

