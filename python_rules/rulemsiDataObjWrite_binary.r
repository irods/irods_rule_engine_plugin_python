def main(rule_args, callback, rei):
    return data_obj_binary_write_test__104(callback)

class error_tracker:

    def __init__(self):
        self.code = 0

    def assert_(self,
                nonzero_value,
                bool_lambda = lambda:False):   # The lambda is for lazy evaluation.
                                               # `nonzero_value' should be negative and not be
                                               # the same in absolute value as any valid errno code.
        if self.code == 0:
            self.code = (0 if bool_lambda() else nonzero_value)

def data_obj_binary_write_test__104(callback):

    binary_bytes = irods_types.BytesBuf()

    buffer = bytearray((256 + 127 - i) % 256 for i in range(256))  # Create a list of [127,126,...,0,255,254,...,128]
    buffer_length = len(buffer)
    dest_file = '/tempZone/home/rods/test_buffer__104'

    binary_bytes.set_buffer(buffer)

    err = error_tracker()
    try:
        binary_bytes.get_byte(10240)
    except IndexError:
        pass               # This error is the expected result of get_byte with too large an index.
    else:
        err.assert_(-997)  # Error because an exception should have been thrown by the out-of-range access.

    ret_val = callback.msiDataObjOpen(
       ('objPath={}++++openFlags=O_RDWR''O_CREAT''O_TRUNC').format(dest_file)
       , 0)
    f = ret_val['arguments'][1]

    callback.msiDataObjWrite(f, binary_bytes, buffer_length)
    binary_bytes.clear_buffer()

    callback.msiDataObjLseek(f, "0", "SEEK_SET", 0)

    ret_val = callback.msiDataObjRead(f, str(16+buffer_length), irods_types.BytesBuf())
    read_buffer = ret_val['arguments'][2]

    err.assert_(-999, lambda: list(buffer) == read_buffer.get_bytes())
    err.assert_(-998, lambda: buffer_length == read_buffer.len)

    read_buffer.clear_buffer()

    callback.msiDataObjClose(f, 0)
    callback.msiDataObjUnlink("objPath="+dest_file,0)

    return err.code

INPUT null
OUTPUT ruleExecOut
