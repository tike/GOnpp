#include "NppWrapper.h"

#include "PluginInterface.h"

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
BOOL NppWrapper::current_file_is_go_file(void){
	TCHAR ext[MAX_PATH];
	
	// Check wether current file is a .go file
	::SendMessage(_npp._nppHandle, NPPM_GETEXTPART, 0, (LPARAM)ext);
	if (_tcscmp(ext, TEXT(".go")) != 0){
		return FALSE;
	}
	return TRUE;
}

// reloads all open files
// TODO: make this check the extension and only reload .go files
BOOL NppWrapper::reload_all_files(void){
	int num_files = ::SendMessage(_npp._nppHandle, NPPM_GETNBOPENFILES, 0, ALL_OPEN_FILES);
		
	TCHAR **file_names = (TCHAR**) calloc(num_files, sizeof(TCHAR*));
	if (file_names == NULL) return FALSE;

	int i = 0;
	for (i=0; i<num_files; i++){
		file_names[i] = (TCHAR*) calloc(MAX_PATH, sizeof(TCHAR));
		if (file_names[i] == NULL){
			int j = 0;
			for (j=0; j<i; j++){
				free(file_names[j]);
			}
			free(file_names);
			return FALSE;
		}
	}
	if ( num_files != ::SendMessage(_npp._nppHandle, NPPM_GETOPENFILENAMES, (WPARAM) file_names, num_files)){
		return FALSE;
	}

		
	for ( i=0; i<num_files; i++){
		TCHAR curr_file[MAX_PATH];
		_tcsncpy(curr_file, file_names[i], MAX_PATH);
		::SendMessage(_npp._nppHandle, NPPM_RELOADFILE, FALSE, (LPARAM) curr_file);
	}
		
	for (i=0; i<num_files; i++){
		free(file_names[i]);
	}
	free(file_names);

	return TRUE;
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
