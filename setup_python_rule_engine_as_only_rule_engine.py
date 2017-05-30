from __future__ import print_function

from irods.configuration import IrodsConfig
from irods import paths

def main():
    irods_config = IrodsConfig()
    irods_config.server_config['plugin_configuration']['rule_engines'] = [
            {
                'instance_name': 'irods_rule_engine_plugin-python-instance',
                'plugin_name': 'irods_rule_engine_plugin-python',
                'plugin_specific_configuration': {}
            }
        ]
    irods_config.commit(irods_config.server_config, irods_config.server_config_path, make_backup=True)

if __name__ == '__main__':
    main()
