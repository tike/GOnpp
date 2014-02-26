//Copyright (C)2014 tike <timflex@gmx.de>,
//                  Mateusz Czapliñski <czapkofan@gmail.com>
//Copyright (C)2003 Don HO <donho@altern.org>
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

#include "PluginDefinition.h"
#include "menuCmdID.h"
#include "ErrorMessages.h"

//
// put the headers you need here
//
#include <memory>
#include <Shlwapi.h>
#include "CmdDlg.h"
#include "goCommands/goCommand.h"
#include "goCommands/goRUN.h"
#include "AutoCompletion.h"

const TCHAR sectionName[] = TEXT("Insert Extesion");
const TCHAR keyName[] = TEXT("doCloseTag");
const TCHAR configFileName[] = TEXT("GOnpp.ini");

CmdDlg _cmdDlg;
std::auto_ptr<AutoCompletion> autocompletion;

#ifdef UNICODE 
	#define generic_itoa _itow
#else
	#define generic_itoa itoa
#endif

FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;


TCHAR iniFilePath[MAX_PATH];
bool doCloseTag = false;

#define DOCKABLE_DEMO_INDEX 4

// Maximum length of environment variables.
#define MAX_ENVIRON  32767

// $(GOROOT)\bin\go.exe
tstring GO_CMD;
BOOL GO_CMD_FOUND = FALSE;

// initialize the GO_CMD variable to hold the path for go.exe
BOOL initialize_go_cmd(){
	// allocate string to hold value obtained from environment variable
	LPTSTR raw_goroot = (LPTSTR) calloc(MAX_ENVIRON, sizeof(TCHAR));
	if (raw_goroot == NULL){
		::MessageBox(nppData._nppHandle, _T("Out of memory, fuck!"), _T("E R R O R"), MB_OK);
		return FALSE;
	}

	// allocate memory to hold string "xyz\bin\go.exe"
	// will be freed in pluginCleanUp
	LPTSTR GO_CMD_tmp = (LPTSTR) calloc(MAX_PATH, sizeof(TCHAR));
	if (GO_CMD_tmp == NULL){
		::MessageBox(nppData._nppHandle, _T("Out of memory, fuck!"), _T("E R R O R"), MB_OK);
		free(raw_goroot);
		return FALSE;
	}

	// if GOBIN is set set assume that go.exe is there
	DWORD length = ::GetEnvironmentVariable(_T("GOBIN"), raw_goroot, MAX_ENVIRON);
	if (length != 0){
		if (::PathCombine(GO_CMD_tmp, raw_goroot, _T("go.exe")) == NULL) return FALSE;
		::PathQuoteSpaces(GO_CMD_tmp);
		GO_CMD = tstring(GO_CMD_tmp);
		free(raw_goroot);
		return TRUE;
	}
	
	// if GOROOT is set assume that go.exe is there
	length = ::GetEnvironmentVariable(_T("GOROOT"), raw_goroot, MAX_ENVIRON);
	if (length != 0){
		if (::PathCombine(GO_CMD_tmp, raw_goroot, _T("bin\\go.exe")) == NULL) return FALSE;
		::PathQuoteSpaces(GO_CMD_tmp);
		GO_CMD = tstring(GO_CMD_tmp);
		free(raw_goroot);
		return TRUE;
	}

	free(raw_goroot);
	// if neither is set, pray that go.exe is somewhere on the path
	if (GetLastError() == ERROR_ENVVAR_NOT_FOUND){
		GO_CMD = tstring(_T("go.exe"));
		

		CommandExec c = CommandExec(GO_CMD, tstring());
		BOOL start = c.Start();
		BOOL end = c.Wait();
		DWORD status = c.ExitStatus();
		if ( ! start && ! end && status ){
			::MessageBox(nppData._nppHandle,
				NO_GO_EXE_ERROR, 
				_T("go configuration error"), MB_OK | MB_ICONERROR);
		}
		GO_CMD = tstring(GO_CMD_tmp);
		return TRUE;
	}

	return FALSE;
}


//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule)
{
	// Initialize dockable demo dialog
	_cmdDlg.init((HINSTANCE)hModule, NULL);
	GO_CMD_FOUND = initialize_go_cmd();
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
	::WritePrivateProfileString(sectionName, keyName, doCloseTag?TEXT("1"):TEXT("0"), iniFilePath);
	_cmdDlg.destroy();
	//free(GO_CMD);
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{
	//
	// Firstly we get the parameters from your plugin config file (if any)
	//

	// get path of plugin configuration
	::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)iniFilePath);

	// if config path doesn't exist, we create it
	if (PathFileExists(iniFilePath) == FALSE)
	{
		::CreateDirectory(iniFilePath, NULL);
	}

	// make your plugin config file full file path name
	PathAppend(iniFilePath, configFileName);

	// get the parameter value from plugin config
	doCloseTag = (::GetPrivateProfileInt(sectionName, keyName, 0, iniFilePath) != 0);

	ShortcutKey *fmtKey = new ShortcutKey;
	fmtKey->_isAlt = true;
	fmtKey->_isCtrl = false;
	fmtKey->_isShift = false;
	fmtKey->_key = 0x46; //VK_F
    setCommand(0, TEXT("go fmt"), go_fmt, fmtKey, false);

	ShortcutKey *testKey = new ShortcutKey;
	testKey->_isAlt = true;
	testKey->_isCtrl = false;
	testKey->_isShift = false;
	testKey->_key = 0x54; //VK_T

    setCommand(1, TEXT("go test"), go_test, testKey, false);

	ShortcutKey *installKey = new ShortcutKey;
	installKey->_isAlt = true;
	installKey->_isCtrl = false;
	installKey->_isShift = false;
	installKey->_key = 0x49; //VK_I

	setCommand(2, TEXT("go install"), go_install, installKey, false);

	ShortcutKey *runKey = new ShortcutKey;
	runKey->_isAlt = true;
	runKey->_isCtrl = false;
	runKey->_isShift = false;
	runKey->_key = 0x52; //VK_R

	setCommand(3, TEXT("go run"), go_run, runKey, false);

	setCommand(DOCKABLE_DEMO_INDEX, TEXT("show output window"), CmdDlgShow, NULL, false);
}


//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
	delete funcItem[0]._pShKey;
	delete funcItem[1]._pShKey;
	delete funcItem[2]._pShKey;
	delete funcItem[3]._pShKey;
}

//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}




// To keep the goCmd class clean of notepad++ specific commands, 
// notepad++ specific preparations (saving open files, etc) are done here
// 
DWORD run_go_tool(goCommand &goCmd){
	NppWrapper npp(nppData);
	if ( !GO_CMD_FOUND || !npp.current_file_is_go_file()){
		return FALSE;
	}
	npp.save_all_files();

	tstring &full_current_file = npp.get_full_current_filename();

	CmdDlgShow();

	if ( ! goCmd.InitialiseCmd(GO_CMD, full_current_file)){
		_cmdDlg.setText(goCmd.GetCommand());
		::MessageBox(nppData._nppHandle, TEXT("failed to create commandline"), TEXT("E R R O R"), MB_OK);
		return FALSE;
	}
	_cmdDlg.setText(goCmd.GetCommand());
	goCmd.RunCmd();
	if(goCmd.HasStdOut()){
		tstring stdOut = goCmd.GetstdOut();
		_cmdDlg.appendText(stdOut);
		//free(stdOut);
	}

	if(goCmd.HasStdErr()){
		tstring stdErr = goCmd.GetstdErr();
		_cmdDlg.appendText(stdErr);
		// free(stdErr);
	}
	if (! goCmd.HasStdErr() && ! goCmd.HasStdOut()){
		_cmdDlg.display(false);
	} 
	npp.switch_to_file(full_current_file);
	
	return TRUE;
}


void go_fmt(void)
{	
	tstring cmd = tstring(_T("fmt"));
	goCommand goCmd(cmd, tstring());

	if ( ! run_go_tool(goCmd)) return;

	if( ! goCmd.exitStatus){
		NppWrapper npp(nppData);
		if (npp.reload_all_files())	_cmdDlg.display(false);
	}
}


void go_test(void)
{
	tstring cmd = tstring(_T("test"));
	goCommand goCmd(cmd, tstring());
	if ( ! run_go_tool(goCmd)) return;
}



void go_install(void)
{	
	tstring cmd = tstring(_T("install"));
	goCommand goCmd(cmd, tstring());

	if ( ! run_go_tool(goCmd)) return;

	/* if (! goCmd.exitStatus){
		tstring cmd = goCmd.GetCommand();
		::MessageBox(nppData._nppHandle, cmd, _T("Build successfull"), MB_OK);
	}
	*/
}


void go_run(void)
{	
	goRUN goCmd;
	if ( ! run_go_tool(goCmd)) return;
}

void CmdDlgShow()
{
	// the dlgDlg should be the index of funcItem where the current function pointer is
	// in this case is DOCKABLE_DEMO_INDEX
	_cmdDlg.show(nppData._nppHandle, DOCKABLE_DEMO_INDEX);
}
