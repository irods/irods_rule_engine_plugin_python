def get_map(rei):

    def get_map_for_user_struct(user_struct):
        return {
            'user_name': str(user_struct.userName),
            'irods_zone': str(user_struct.rodsZone),
            'user_type': str(user_struct.userType),
            'system_uid': user_struct.sysUid,
            'authentication_info': {
                'authentication_scheme': str(user_struct.authInfo.authScheme),
                'privilege_level': user_struct.authInfo.authFlag,
                'flag': user_struct.authInfo.flag,
                'ppid': user_struct.authInfo.ppid,
                'host': str(user_struct.authInfo.host),
                'authentication_string': str(user_struct.authInfo.authStr)
                },
            'info': str(user_struct.userOtherInfo.userInfo),
            'comments': str(user_struct.userOtherInfo.userComments),
            'create_time': str(user_struct.userOtherInfo.userCreate),
            'modify_time': str(user_struct.userOtherInfo.userModify)
            }

    return {
            'plugin_instance_name': str(rei.pluginInstanceName),
            'status': rei.status,
            'operation_type': rei.doinp.oprType if rei.doinp is not None else None,
            'connection': {
                'client_address': str(rei.rsComm.clientAddr),
                'connection_count': rei.rsComm.connectCnt,
                'socket': rei.rsComm.sock,
                'option': str(rei.rsComm.option),
                'status': rei.rsComm.status,
                'api_number': rei.rsComm.apiInx
                } if rei.rsComm is not None else None,
            'data_object': {
                'object_path': str(rei.doi.objPath) if rei.doi is not None else str(rei.doinp.objPath),
                'size': rei.doi.dataSize if rei.doi is not None else rei.doinp.dataSize,
                'type': str(rei.doi.dataType) if rei.doi is not None else None,
                'checksum': str(rei.doi.chksum) if rei.doi is not None else None,
                'file_path': str(rei.doi.filePath) if rei.doi is not None else None,
                'replica_number': rei.doi.replNum if rei.doi is not None else None,
                'replication_status': rei.doi.replStatus if rei.doi is not None else None,
                'write_flag': rei.doi.writeFlag if rei.doi is not None else None,
                'owner': str(rei.doi.dataOwnerName) if rei.doi is not None else None,
                'owner_zone': str(rei.doi.dataOwnerZone) if rei.doi is not None else None,
                'expiry': str(rei.doi.dataExpiry) if rei.doi is not None else None,
                'comments': str(rei.doi.dataComments) if rei.doi is not None else None,
                'create_time': str(rei.doi.dataCreate) if rei.doi is not None else None,
                'modify_time': str(rei.doi.dataModify) if rei.doi is not None else None,
                'access_time': str(rei.doi.dataAccess) if rei.doi is not None else None,
                'id': rei.doi.dataId if rei.doi is not None else None,
                'collection_id': rei.doi.collId if rei.doi is not None else None,
                'status_string': str(rei.doi.statusString) if rei.doi is not None else None,
                'destination_resource_name': str(rei.doi.destRescName) if rei.doi is not None else None,
                'backup_resource_name': str(rei.doi.backupRescName) if rei.doi is not None else None,
                'resource_name': str(rei.doi.rescName) if rei.doi is not None else None,
            } if rei.doi is not None or rei.doinp is not None else None,
            'collection': {
                'id': rei.coi.collId,
                'name': str(rei.coi.collName),
                'parent_collection_name': str(rei.coi.collParentName),
                'owner': str(rei.coi.collOwnerName),
                'expiry': str(rei.coi.collExpiry),
                'comments': str(rei.coi.collComments),
                'create_time': str(rei.coi.collCreate),
                'modify_time': str(rei.coi.collModify),
                'access_time': str(rei.coi.collAccess),
                'inheritance': str(rei.coi.collInheritance)
                } if rei.coi is not None else None,
            'client_user': get_map_for_user_struct(rei.uoic) if rei.uoic is not None else get_map_for_user_struct(rei.rsComm.clientUser) if rei.rsComm is not None and rei.rsComm.clientUser is not None else None,
            'proxy_user': get_map_for_user_struct(rei.uoip) if rei.uoip is not None else get_map_for_user_struct(rei.rsComm.proxyUser) if rei.rsComm is not None and rei.rsComm.proxyUser is not None else None,
            'other_user': get_map_for_user_struct(rei.uoio) if rei.uoio is not None else None,
            'key_value_pairs': dict((rei.condInputData.key[i], rei.condInputData.value[i]) for i in range(0, rei.condInputData.len)) if rei.condInputData is not None else None
            }

