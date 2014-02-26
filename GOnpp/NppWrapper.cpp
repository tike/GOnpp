#include "NppWrapper.h"

#include <vector>
#include "PluginInterface.h"

using namespace std;

NppWrapper::NppWrapper(const NppData npp)
	: _npp(npp)
{
}

LRESULT NppWrapper::send_scintilla(UINT msg, WPARAM wparam, LPARAM lparam)
{
	return ::SendMessage(_npp._scintillaMainHandle, msg, wparam, lparam);
}

// checks the current files extention and compares it to ".go"
// returns TRUE on match, FALSE otherwise
bool NppWrapper::current_file_is_go_file(void){
	TCHAR ext[MAX_PATH];
	
	// Check wether current file is a .go file
	::SendMessage(_npp._nppHandle, NPPM_GETEXTPART, 0, (LPARAM)ext);
	if (_tcscmp(ext, TEXT(".go")) != 0){
		return false;
	}
	return true;
}

// reloads all open files
// TODO: make this check the extension and only reload .go files
bool NppWrapper::reload_all_files(void){
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

void NppWrapper::save_all_files(void){
	::SendMessage(_npp._nppHandle, NPPM_SAVEALLFILES, 0, 0);
}

void NppWrapper::switch_to_file(tstring filename){
	::SendMessage(_npp._nppHandle, NPPM_SWITCHTOFILE, 0, (LPARAM) filename.c_str());
}

void NppWrapper::get_full_current_filename(tstring& filename){
	TCHAR full_current_file[MAX_PATH];
	::SendMessage(_npp._nppHandle, NPPM_GETFULLCURRENTPATH, MAX_PATH, (LPARAM) full_current_file);
	
	filename.clear();
	filename.insert(0, (TCHAR*) full_current_file);
	return ;
}
