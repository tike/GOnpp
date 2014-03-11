#include "NppWrapper.h"

#include <vector>
#include "PluginInterface.h"
#include "PluginDefinition.h"

#include "Shlwapi.h"

using namespace std;

NppWrapper::NppWrapper(const NppData npp)
	: _npp(npp)
{
}

LRESULT NppWrapper::send_scintilla(UINT msg, WPARAM wparam, LPARAM lparam) const
{
	return ::SendMessage(_npp._scintillaMainHandle, msg, wparam, lparam);
}

LRESULT NppWrapper::send_npp(UINT msg, WPARAM wparam, LPARAM lparam) const
{
	return ::SendMessage(_npp._nppHandle, msg, wparam, lparam);
}

// checks the current files extention and compares it to ".go"
// returns TRUE on match, FALSE otherwise
bool NppWrapper::current_file_is_go_file() const
{
	TCHAR ext[MAX_PATH];
	send_npp(NPPM_GETEXTPART, 0, (LPARAM)ext);
	return _tcscmp(ext, _T(".go")) == 0;
}

// reloads all open files
// TODO: make this check the extension and only reload .go files
bool NppWrapper::reload_all_files() const
{
	int num_files = ::SendMessage(_npp._nppHandle, NPPM_GETNBOPENFILES, 0, ALL_OPEN_FILES);

	vector< vector<TCHAR> > file_name_bufs(num_files);
	vector< TCHAR* > file_name_ptrs(num_files);
	for (int i=0; i<num_files; i++) {
		file_name_bufs[i].resize(MAX_PATH);
		file_name_ptrs[i] = &file_name_bufs[i][0];
	}

	if ( num_files != ::SendMessage(_npp._nppHandle, NPPM_GETOPENFILENAMES, (WPARAM) &file_name_ptrs[0], num_files)){
		return false;
	}

	for (int i=0; i<num_files; i++){
		::SendMessage(_npp._nppHandle, NPPM_RELOADFILE, FALSE, (LPARAM) file_name_ptrs[i]);
	}

	return true;
}

void NppWrapper::save_all_files() const
{
	::SendMessage(_npp._nppHandle, NPPM_SAVEALLFILES, 0, 0);
}

void NppWrapper::switch_to_file(tstring filename) const
{
	::SendMessage(_npp._nppHandle, NPPM_SWITCHTOFILE, 0, (LPARAM) filename.c_str());
}

tstring NppWrapper::get_full_current_filename() const
{
	TCHAR full_current_file[MAX_PATH];
	send_npp(NPPM_GETFULLCURRENTPATH, MAX_PATH, (LPARAM) full_current_file);
	return tstring(full_current_file);
}


BOOL NppWrapper::get_config_file_name(tstring& filepath) const
{
	TCHAR iniFilePath[MAX_PATH];
	// get path of plugin configuration
	DWORD ret = ::SendMessage(_npp._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)iniFilePath);

	// if config path doesn't exist, we create it
	if (::PathFileExists(iniFilePath) == FALSE)
	{
		::CreateDirectory(iniFilePath, NULL);
	}

	// make your plugin config file full file path name
	::PathAppend(iniFilePath, NPP_PLUGIN_NAME);

	filepath.clear();
	filepath.append(iniFilePath);
	filepath.append(_T(".ini"));
	return TRUE;
}
