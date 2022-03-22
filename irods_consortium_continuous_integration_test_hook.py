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
    options, _ = parser.parse_args()

    output_root_directory = options.output_root_directory
    built_packages_root_directory = options.built_packages_root_directory
    plugin_dir = irods_python_ci_utilities.append_os_specific_directory(built_packages_root_directory)
    irods_python_ci_utilities.install_os_packages_from_files([os.path.join(plugin_dir, entry) for entry in os.listdir(plugin_dir)])
    irods_python_ci_utilities.subprocess_get_output(['sudo', 'su', '-', 'irods', '-c', 'python3 scripts/setup_python_rule_engine_as_only_rule_engine.py'], check_rc=True)

    test_output_file = 'log/test_output.log'
    try:
        irods_python_ci_utilities.subprocess_get_output(['sudo', 'su', '-', 'irods', '-c', 'python3 scripts/run_tests.py --xml_output --run_python_suite > {0} 2>&1'.format(test_output_file)], check_rc=True)
    finally:
        if output_root_directory:
            irods_python_ci_utilities.subprocess_get_output(['sudo', 'su', '-', 'irods', '-c', 'chmod o+r -R {0}'.format(' '.join(['test-reports', 'log', test_output_file]))])
            shutil.copytree(os.path.join(os.path.expanduser('~irods'), 'test-reports'), os.path.join(output_root_directory, 'test-reports'))
            shutil.copytree(os.path.join(os.path.expanduser('~irods'), 'log'), os.path.join(output_root_directory, 'log'))
            shutil.copy(os.path.join(os.path.expanduser('~irods'), test_output_file), output_root_directory)

if __name__ == '__main__':
    main()
