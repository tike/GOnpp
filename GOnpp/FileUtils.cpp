#include "FileUtils.h"


// checks the current files extention and compares it to ".go"
// returns TRUE on match, FALSE otherwise
BOOL current_file_is_go_file(NppData nppData){
	TCHAR ext[MAX_PATH];
	
	// Check wether current file is a .go file
	::SendMessage(nppData._nppHandle, NPPM_GETEXTPART, 0, (LPARAM)ext);
	if (_tcscmp(ext, TEXT(".go")) != 0){
		return FALSE;
	}
	return TRUE;
}

// reloads all open files
// TODO: make this check the extension and only reload .go files
BOOL reload_all_files(NppData nppData){
	int num_files = ::SendMessage(nppData._nppHandle, NPPM_GETNBOPENFILES, 0, ALL_OPEN_FILES);
		
	TCHAR **file_names = (TCHAR**) calloc(num_files, sizeof(TCHAR*));
	if (file_names == NULL) return FALSE;

	int i = 0;
	for (i=0; i<num_files; i++){
		file_names[i] = (TCHAR*) calloc(MAX_PATH, sizeof(TCHAR));
		if (file_names[i] == NULL) return FALSE;
	}

	if ( num_files != ::SendMessage(nppData._nppHandle, NPPM_GETOPENFILENAMES, (WPARAM) file_names, num_files)){
		return FALSE;
	}

		
	for ( i=0; i<num_files; i++){
		TCHAR curr_file[MAX_PATH];
		_tcsncpy(curr_file, file_names[i], MAX_PATH);
		::SendMessage(nppData._nppHandle, NPPM_RELOADFILE, FALSE, (LPARAM) curr_file);
	}
		
	for (i=0; i<num_files; i++){
		free(file_names[i]);
	}
	free(file_names);

	return TRUE;
}
