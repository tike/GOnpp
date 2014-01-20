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

//this file is part of notepad++
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

//
// put the headers you need here
//
#include <Shlwapi.h>
#include "CmdDlg.h"
#include "goCommands/goCommand.h"
#include "goCommands/goRUN.h"

const TCHAR sectionName[] = TEXT("Insert Extesion");
const TCHAR keyName[] = TEXT("doCloseTag");
const TCHAR configFileName[] = TEXT("pluginDemo.ini");

CmdDlg _cmdDlg;

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
LPTSTR GO_CMD = NULL;
BOOL GO_CMD_FOUND = FALSE;

//
// Initialize your plugin data here
// It will be called while plugin loading
BOOL initialize_go_cmd(){
	LPTSTR raw_goroot = (LPTSTR) calloc(MAX_ENVIRON, sizeof(TCHAR));
	if (raw_goroot == NULL){
		::MessageBox(nppData._nppHandle, TEXT("Out of memory, fuck!"), TEXT("E R R O R"), MB_OK);
		return FALSE;
	}

	// we add one for the NULL here...
	GO_CMD = (LPTSTR) calloc(MAX_PATH, sizeof(TCHAR));
	if (GO_CMD == NULL){
		::MessageBox(nppData._nppHandle, TEXT("Out of memory, fuck!"), TEXT("E R R O R"), MB_OK);
		free(raw_goroot);
		return FALSE;
	}

	DWORD length = GetEnvironmentVariable(_TEXT("GOBIN"), raw_goroot, MAX_ENVIRON);
	if (length != 0){
		if (PathCombine(GO_CMD, raw_goroot, TEXT("go.exe")) == NULL) return FALSE;
		PathQuoteSpaces(GO_CMD);
		return TRUE;
	}
	
	length = GetEnvironmentVariable(_TEXT("GOROOT"), raw_goroot, MAX_ENVIRON);
	if (length == 0){
		if (GetLastError() == ERROR_ENVVAR_NOT_FOUND){
			::MessageBox(nppData._nppHandle,
				TEXT("GOnpp can't find neither your GOROOT nor your GOBIN environment variable!\n \
						Set it and restart notepad++ to use GOnpp."),
				TEXT("GOROOT not found!"),
				MB_OK);
		}
		free(raw_goroot);
		return FALSE;
	}
	
	if (PathCombine(GO_CMD, raw_goroot, TEXT("bin\\go.exe")) == NULL) return FALSE;
	PathQuoteSpaces(GO_CMD);
	
	free(raw_goroot);
	return TRUE;
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
	free(GO_CMD);
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


BOOL is_go_file(void){
	TCHAR ext[MAX_PATH];
	
	// Check wether current file is a .go file
	::SendMessage(nppData._nppHandle, NPPM_GETEXTPART, 0, (LPARAM)ext);
	if (_tcscmp(ext, TEXT(".go")) != 0){
		return FALSE;
	}
	return TRUE;
}

BOOL reload_all_files(void){
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

DWORD run_go_tool(goCommand *goCmd){
	if ( !GO_CMD_FOUND || !is_go_file()){
		return FALSE;
	}
	
	TCHAR path[MAX_PATH];
	::SendMessage(nppData._nppHandle, NPPM_GETFULLCURRENTPATH, MAX_PATH, (LPARAM) path);
	::SendMessage(nppData._nppHandle, NPPM_SAVEALLFILES, 0, 0);

	 CmdDlgShow();

	if ( ! goCmd->InitialiseCmd(GO_CMD, path)){
		::MessageBox(nppData._nppHandle, TEXT("failed to create commandline"), TEXT("E R R O R"), MB_OK);
		return FALSE;
	}
	_cmdDlg.setText(goCmd->GetCommand());
	goCmd->RunCmd();
	if(goCmd->HasStdOut()){
		LPTSTR stdOut = goCmd->GetstdOut();
		_cmdDlg.appendText(stdOut);
		free(stdOut);
	}

	if(goCmd->HasStdErr()){
		LPTSTR stdErr = goCmd->GetstdErr();
		_cmdDlg.appendText(stdErr);
		free(stdErr);
	}
	if (! goCmd->HasStdErr() && ! goCmd->HasStdOut()){
		_cmdDlg.display(false);
	} else {
		::SetFocus(nppData._nppHandle);
	}
	return TRUE;
}


void go_fmt(void)
{	
	goCommand* goCmd = new goCommand(_T("fmt"), NULL);

	if ( ! run_go_tool(goCmd)) return;

	if( ! goCmd->exitStatus){
		if (reload_all_files())	_cmdDlg.display(false);
	}
	delete(goCmd);
}


void go_test(void)
{
	goCommand* goCmd = new goCommand(_T("test"), NULL);
	
	if ( ! run_go_tool(goCmd)) return;

	delete(goCmd);
}



void go_install(void)
{	
	goCommand* goCmd = new goCommand(_T("install"), NULL);

	if ( ! run_go_tool(goCmd)) return;

	if (! goCmd->exitStatus){
		LPTSTR cmd = goCmd->GetCommand();
		//::MessageBox(nppData._nppHandle, cmd, _T("Build successfull"), MB_OK);
		free(cmd);
	}
	delete(goCmd);
}


void go_run(void)
{	
	goCommand* goCmd = new goRUN();

	if ( ! run_go_tool(goCmd)) return;

	delete(goCmd);
}


void CmdDlgShow()
{
	_cmdDlg.setParent(nppData._nppHandle);
	tTbData	data = {0};

	if (!_cmdDlg.isCreated())
	{
		_cmdDlg.create(&data);

		// define the default docking behaviour
		data.uMask = DWS_DF_CONT_RIGHT;

		data.pszModuleName = _cmdDlg.getPluginFileName();

		// the dlgDlg should be the index of funcItem where the current function pointer is
		// in this case is DOCKABLE_DEMO_INDEX
		data.dlgID = DOCKABLE_DEMO_INDEX;
		::SendMessage(nppData._nppHandle, NPPM_DMMREGASDCKDLG, 0, (LPARAM)&data);
	}
	_cmdDlg.display();
}

