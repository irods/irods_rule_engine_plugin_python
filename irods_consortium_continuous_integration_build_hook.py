from __future__ import print_function

import multiprocessing
import optparse
import os
import sys
import tempfile

import irods_python_ci_utilities


def install_cmake_and_add_to_front_of_path():
    irods_python_ci_utilities.install_os_packages(['irods-externals-cmake3.11.4-0'])
    os.environ['PATH'] = '/opt/irods-externals/cmake3.11.4-0/bin' + os.pathsep + os.environ['PATH']

def get_build_prerequisites_all():
    return ['irods-externals-avropre190cpp17-0',
            'irods-externals-boost1.67.0-0',
            'irods-externals-catch22.3.0-0',
            'irods-externals-clang-runtime6.0-0',
            'irods-externals-clang6.0-0',
            'irods-externals-cppzmq4.2.3-0',
            'irods-externals-libarchive3.3.2-1',
            'irods-externals-zeromq4-14.1.6-0']

def get_build_prerequisites_apt():
    return ['make', 'python-dev', 'libssl-dev', 'gcc'] + get_build_prerequisites_all()

def get_build_prerequisites_yum():
    return ['python-devel', 'openssl-devel'] + get_build_prerequisites_all()

def get_build_prerequisites():
    dispatch_map = {
        'Ubuntu': get_build_prerequisites_apt,
        'Centos': get_build_prerequisites_yum,
        'Centos linux': get_build_prerequisites_yum,
    }
    try:
        return dispatch_map[irods_python_ci_utilities.get_distribution()]()
    except KeyError:
        irods_python_ci_utilities.raise_not_implemented_for_distribution()

def install_build_prerequisites():
    if irods_python_ci_utilities.get_distribution() == 'Ubuntu': # cmake from externals requires newer libstdc++ on ub12
        if irods_python_ci_utilities.get_distribution_version_major() == '12':
            irods_python_ci_utilities.install_os_packages(['python-software-properties'])
            irods_python_ci_utilities.subprocess_get_output(['sudo', 'add-apt-repository', '-y', 'ppa:ubuntu-toolchain-r/test'], check_rc=True)
            irods_python_ci_utilities.install_os_packages(['libstdc++6'])
    irods_python_ci_utilities.install_os_packages(get_build_prerequisites())

def copy_output_packages(build_directory, output_root_directory):
    irods_python_ci_utilities.gather_files_satisfying_predicate(
        build_directory,
        irods_python_ci_utilities.append_os_specific_directory(output_root_directory),
        lambda s:s.endswith(irods_python_ci_utilities.get_package_suffix()))

def main(output_root_directory, irods_packages_root_directory):
    irods_python_ci_utilities.install_irods_core_dev_repository()
    install_cmake_and_add_to_front_of_path()
    install_build_prerequisites()
    if irods_packages_root_directory:
        irods_python_ci_utilities.install_irods_dev_and_runtime_packages(irods_packages_root_directory)
    build_directory = tempfile.mkdtemp(prefix='irods_python_rule_engine_plugin_build_directory')
    irods_python_ci_utilities.subprocess_get_output(['cmake', os.path.dirname(os.path.realpath(__file__))], check_rc=True, cwd=build_directory)
    irods_python_ci_utilities.subprocess_get_output(['make', '-j', str(multiprocessing.cpu_count()), 'package'], check_rc=True, cwd=build_directory)
    if output_root_directory:
        copy_output_packages(build_directory, output_root_directory)

if __name__ == '__main__':
    parser = optparse.OptionParser()
    parser.add_option('--output_root_directory')
    parser.add_option('--irods_packages_root_directory')
    options, _ = parser.parse_args()

    main(options.output_root_directory, options.irods_packages_root_directory)
