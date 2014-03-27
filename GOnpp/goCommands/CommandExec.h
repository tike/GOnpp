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
#include "StringUtils/tstring.h"


class CommandExec
{
public:
	CommandExec(const tstring& cmd, const tstring& dir);
	~CommandExec(void);

	bool Start(void);
	bool Wait(DWORD millis = INFINITE);

	DWORD ExitStatus(void);

	bool ReadOutput(void);
	tstring GetStdOut(void);
	tstring GetStdErr(void);
	tstring GetCombined(void);

protected:
	bool setupPipe(PHANDLE read, PHANDLE write);
	bool readOutput(HANDLE handle, tstring& output);
	tstring cmd;
	tstring dir;

	SECURITY_ATTRIBUTES sa;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	DWORD exitStatus;

	tstring stdOut;
	HANDLE stdOutRd;
	HANDLE stdOutWr;
	bool StdOutRead;
	bool StdOutClosed;

	HANDLE stdErrRd;
	HANDLE stdErrWr;
	tstring stdErr;
	bool StdErrRead;
	bool StdErrClosed;
};

