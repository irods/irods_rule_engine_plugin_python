def main(rule_args, callback, rei):
    cpu_weight = irods_rule_vars['*Cpuw'][1:-1]
    mem_weight = irods_rule_vars['*Memw'][1:-1]
    swap_weight = irods_rule_vars['*Swapw'][1:-1]
    run_queue_weight = irods_rule_vars['*Runw'][1:-1]
    disk_weight = irods_rule_vars['*Diskw'][1:-1]
    network_in_weight = irods_rule_vars['*Netinw'][1:-1]
    network_out_weight = irods_rule_vars['*Netow'][1:-1]

    callback.msiDigestMonStat(cpu_weight, mem_weight, swap_weight, run_queue_weight, disk_weight, network_in_weight, network_out_weight)

    callback.writeLine('stdout', 'CPU weight is ' + cpu_weight + ', Memory weight is ' + mem_weight + ', Swap weight is ' + swap_weight + ', Run queue weight is ' + run_queue_weight)
    callback.writeLine('stdout', 'Disk weight is ' + disk_weight + ', Network transfer in weight is ' + network_in_weight + ', Network transfer out weight is ' + network_out_weight)

    ret_val = callback.msiExecStrCondQuery('SELECT SLD_RESC_NAME, SLD_LOAD_FACTOR', irods_types.GenQueryOut())
    genQueryOut = ret_val['arguments'][1]

    for row in range(genQueryOut.rowCnt):
        resc_name = genQueryOut.sqlResult[0].row(row)
        load_factor = genQueryOut.sqlResult[0].row(row)
        callback.writeLine('stdout', resc_name + ' = ' + load_factor)

INPUT *Cpuw="1", *Memw="1", *Swapw="0", *Runw="0", *Diskw="0", *Netinw="1", *Netow="1"
OUTPUT ruleExecOut
