#ifndef FILEUTILS_H_INCLUDED
#define FILEUTILS_H_INCLUDED

#ifdef UNICODE
	#include <tchar.h>
#endif
#include <windows.h>
#include "PluginInterface.h"
#include <Shlwapi.h>
#include "menuCmdID.h"

BOOL current_file_is_go_file(NppData nppData);
BOOL reload_all_files(NppData nppData);


#endif // Header guard