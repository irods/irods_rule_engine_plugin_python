#ifndef RE_PYTHON_TYPES_IRODS_RCCONNECT_HPP
#define RE_PYTHON_TYPES_IRODS_RCCONNECT_HPP

namespace irods::re::python::types
{
	void export_procState();
	void export_DataSeg();
	void export_fileRestartFlag();
	void export_fileRestartStatus();
	void export_FileRestartInfo();
	void export_FileRestart();
	void export_procLogFlag();
	void export_RcComm();
	void export_PerfStat();
	void export_RsComm();
}

#endif // RE_PYTHON_TYPES_IRODS_RCCONNECT_HPP
