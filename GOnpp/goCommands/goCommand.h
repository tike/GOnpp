//Copyright (C)2014 tike <timflex@gmx.de>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

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

