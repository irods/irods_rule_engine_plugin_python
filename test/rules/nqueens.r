def nqueens(rule_args, callback):
    queens = add_queen([], 8)
    print_board(queens, 8, callback)

class Bail(Exception):
    pass

def add_queen(queens, n):
    for i in range(n):
        test_queens = queens + [i]
        try:
            validate_queens(test_queens)
            if len(test_queens) == n:
                return test_queens
            else:
                return add_queen(test_queens, n)
        except Bail:
            pass
    raise Bail

def validate_queens(queens):
    left = right = col = queens[-1]
    for r in reversed(queens[:-1]):
        left, right = left-1, right+1
        if r in (left, col, right):
            raise Bail

def print_board(queens, n, callback):
    out_str = ''
    for row in range(n):
        for col in range(n):
            if col == queens[row]:
                out_str += '1 '
            else:
                out_str += '0 '
        out_str += '\n'
    callback.writeLine('serverLog', out_str)
    callback.writeLine('stdout', out_str)

INPUT null
OUTPUT ruleExecOut
