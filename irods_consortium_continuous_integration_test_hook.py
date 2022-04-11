from __future__ import print_function

import multiprocessing
import optparse
import os
import shutil
import sys
import tempfile

import irods_python_ci_utilities

def main():
    parser = optparse.OptionParser()
    parser.add_option('--output_root_directory')
    parser.add_option('--built_packages_root_directory')
    parser.add_option('--test', metavar='dotted name',
                      help='Space-delimited list of dotted test names.')
    parser.add_option('--skip-setup', action='store_false', dest='do_setup', default=True,
                      help='If indicated, installing packages and configuration will be skipped.')
    options, _ = parser.parse_args()

    if options.do_setup:
        built_packages_root_directory = options.built_packages_root_directory
        plugin_dir = irods_python_ci_utilities.append_os_specific_directory(built_packages_root_directory)
        irods_python_ci_utilities.install_os_packages_from_files([os.path.join(plugin_dir, entry) for entry in os.listdir(plugin_dir)])
        irods_python_ci_utilities.subprocess_get_output(['sudo', 'su', '-', 'irods', '-c', 'python3 scripts/setup_python_rule_engine_as_only_rule_engine.py'], check_rc=True)

    test_output_file = 'log/test_output.log'

    test_option = f'--run_specific_test={options.test}' if options.test else '--run_python_suite'

    try:
        irods_python_ci_utilities.subprocess_get_output(['sudo', 'su', '-', 'irods', '-c',
            f'python3 scripts/run_tests.py --xml_output {test_option} 2>&1 | tee {test_output_file}; exit $PIPESTATUS'],
            check_rc=True)

    finally:
        output_root_directory = options.output_root_directory
        if output_root_directory:
            irods_python_ci_utilities.subprocess_get_output(['sudo', 'su', '-', 'irods', '-c', 'chmod o+r -R {0}'.format(' '.join(['test-reports', 'log', test_output_file]))])
            shutil.copytree(os.path.join(os.path.expanduser('~irods'), 'test-reports'), os.path.join(output_root_directory, 'test-reports'))
            shutil.copytree(os.path.join(os.path.expanduser('~irods'), 'log'), os.path.join(output_root_directory, 'log'))
            shutil.copy(os.path.join(os.path.expanduser('~irods'), test_output_file), output_root_directory)

if __name__ == '__main__':
    main()
