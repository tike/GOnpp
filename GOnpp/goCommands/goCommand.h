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
#include "tstring.h"

#define MAX_ENVIRON  32767

class goCommand
{
public:
	goCommand(tstring& cmd, tstring& flags);
	virtual ~goCommand();

	bool InitialiseCmd(tstring& go_cmd, tstring& current_file);
	DWORD RunCmd(void);
	tstring GetCommand(void);

	DWORD exitStatus;

	bool HasStdOut();
	tstring GetstdOut(void);

	bool HasStdErr();
	tstring GetstdErr(void);

protected:
	// configuration Variables
	tstring cmd;
	tstring flags;

	// pkg related runvars
	tstring currentFile;
	tstring currentDir;
	tstring goPath;
	tstring goPkg;

	// execution related
	tstring commandLine;
	tstring stdOut;
	tstring stdErr;


	// path operations
	bool initializeGoVals();
	void initializeFileVals(tstring& current_file);

	virtual bool buildCommandLine(tstring& go_cmd);
	bool combineCommandLine(tstring& go_cmd, tstring& pkg);

	virtual bool preRunCmd(void);
};

