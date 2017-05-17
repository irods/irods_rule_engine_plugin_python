from __future__ import print_function

import multiprocessing
import optparse
import os
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
    irods_python_ci_utilities.install_os_packages_from_files(irods_python_ci_utilities.append_os_specific_directory(built_packages_root_directory))
    irods_python_ci_utilities.subprocess_get_output(['sudo', 'su', '-', 'irods', '-c', 'python2 scripts/setup_python_rule_engine_as_only_rule_engine.py'], check_rc=True)
    irods_python_ci_utilities.subprocess_get_output(['sudo', 'su', '-', 'irods', '-c', 'python2 scripts/run_tests.py'], check_rc=True)

    test_output_file = 'log/test_output.log'
    irods_python_ci_utilities.subprocess_get_output(['sudo', 'su', '-', 'irods', '-c', 'python2 scripts/run_tests.py --xml_output --run_python_suite > {0} 2>&1'.format(test_output_file)])

    if output_root_directory:
        shutil.copy('test-reports', output_root_directory)
        shutil.copy(test_output_file, output_root_directory)
        shutil.copytree('log', os.path.join(output_root_directory, 'log'))

if __name__ == '__main__':
    main()
