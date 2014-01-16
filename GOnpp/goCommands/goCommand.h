#pragma once

#include <tchar.h>
#include <windows.h>
#include "PluginInterface.h"
#include "CommandExec.h"

#define MAX_ENVIRON  32767

class goCommand
{
public:
	goCommand(LPCTSTR cmd, LPCTSTR flags);
	~goCommand(void);

	BOOL InitialiseCmd(LPCTSTR go_cmd, LPTSTR current_file);
	DWORD RunCmd(void);
	LPTSTR GetCommand(void);

	DWORD exitStatus;

	BOOL HasStdOut(void);
	LPTSTR GetstdOut(void);

	BOOL HasStdErr(void);
	LPTSTR GetstdErr(void);

protected:
	// configuration Variables
	LPCTSTR cmd;
	LPCTSTR flags;

	// pkg related runvars
	LPTSTR currentFile;
	LPTSTR currentDir;
	LPTSTR goPath;
	LPTSTR goPkg;

	// execution related
	LPTSTR commandLine;
	LPTSTR stdOut;
	LPTSTR stdErr;


	// path operations
	BOOL initializeGoVals(void);
	BOOL initializeFileVals(LPTSTR current_file);

	virtual BOOL buildCommandLine(LPCTSTR go_cmd);
	BOOL combineCommandLine(LPCTSTR go_cmd, LPTSTR pkg);

	virtual BOOL preRunCmd(void);
};

