#pragma once

#include <tchar.h>
#include <windows.h>
#include "PluginInterface.h"
#include "CommandExec.h"

class goCommand
{
public:
	goCommand(LPCTSTR cmd, LPCTSTR flags);
	~goCommand(void);

	DWORD RunCmd(LPCTSTR go_cmd, LPTSTR current_file);
	LPTSTR GetCommand(void);

	DWORD exitStatus;
	LPTSTR GetstdOut(void);
	LPTSTR GetstdErr(void);

	virtual void DisplayOutput(NppData nppData);

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

	virtual BOOL preRunCmd(LPCTSTR go_cmd, LPTSTR current_file);
};

