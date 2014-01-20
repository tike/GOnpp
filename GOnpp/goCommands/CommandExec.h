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


class CommandExec
{
public:
	CommandExec(LPCTSTR cmd, LPCTSTR dir);
	~CommandExec(void);

	BOOL Start(void);
	BOOL Wait(void);

	DWORD ExitStatus(void);

	BOOL ReadOutput(void);
	LPTSTR GetStdOut(void);
	LPTSTR GetStdErr(void);
	LPTSTR GetCombined(void);

protected:
	BOOL setupPipe(PHANDLE read, PHANDLE write);
	BOOL readOutput(HANDLE handle, LPTSTR* output);
	LPTSTR cmd;
	LPTSTR dir;

	SECURITY_ATTRIBUTES sa;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	DWORD exitStatus;

	LPTSTR stdOut;
	HANDLE stdOutRd;
	HANDLE stdOutWr;
	BOOL StdOutRead;
	BOOL StdOutClosed;

	HANDLE stdErrRd;
	HANDLE stdErrWr;
	LPTSTR stdErr;
	BOOL StdErrRead;
	BOOL StdErrClosed;
};

