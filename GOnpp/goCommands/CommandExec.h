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

