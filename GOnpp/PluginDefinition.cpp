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
#include "AutoCompletion/AutoCompletion.h"
#include "Settings.h"
#include "FuncsArray.h"

std::auto_ptr<Settings> settings;
CmdDlg _cmdDlg;
std::auto_ptr<AutoCompletion> autocompletion;
FuncsArray<7> funcs_array;


#ifdef UNICODE 
	#define generic_itoa _itow
#else
	#define generic_itoa itoa
#endif

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;


#define DOCKABLE_DEMO_INDEX 6

// Maximum length of environment variables.
#define MAX_ENVIRON  32767

// $(GOROOT)\bin\go.exe
tstring GO_CMD;
bool GO_CMD_FOUND = false;

// initialize the GO_CMD variable to hold the path for go.exe
bool initialize_go_cmd(){
	// allocate string to hold value obtained from environment variable
	LPTSTR raw_goroot = (LPTSTR) calloc(MAX_ENVIRON, sizeof(TCHAR));
	if (raw_goroot == NULL){
		::MessageBox(nppData._nppHandle, OUT_OF_MEMORY, MSG_ERROR_HEAD, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// allocate memory to hold string "xyz\bin\go.exe"
	// will be freed in pluginCleanUp
	LPTSTR GO_CMD_tmp = (LPTSTR) calloc(MAX_PATH, sizeof(TCHAR));
	if (GO_CMD_tmp == NULL){
		::MessageBox(nppData._nppHandle, OUT_OF_MEMORY, MSG_ERROR_HEAD, MB_OK | MB_ICONERROR);
		free(raw_goroot);
		return false;
	}

	// if GOBIN is set set assume that go.exe is there
	DWORD length = ::GetEnvironmentVariable(_T("GOBIN"), raw_goroot, MAX_ENVIRON);
	if (length != 0){
		if (::PathCombine(GO_CMD_tmp, raw_goroot, _T("go.exe")) == NULL) {
			return false;
		}
		::PathQuoteSpaces(GO_CMD_tmp);
		GO_CMD = tstring(GO_CMD_tmp);
		free(raw_goroot);
		return true;
	}
	
	// if GOROOT is set assume that go.exe is there
	length = ::GetEnvironmentVariable(_T("GOROOT"), raw_goroot, MAX_ENVIRON);
	if (length != 0){
		if (::PathCombine(GO_CMD_tmp, raw_goroot, _T("bin\\go.exe")) == NULL) {
			return false;
		}
		::PathQuoteSpaces(GO_CMD_tmp);
		GO_CMD = tstring(GO_CMD_tmp);
		free(raw_goroot);
		return true;
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
				CONFIGURATION_ERROR, MB_OK | MB_ICONERROR);
		}
		GO_CMD = tstring(GO_CMD_tmp);
		return true;
	}

	return false;
}


//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule){
	// Initialize dockable demo dialog
	_cmdDlg.init((HINSTANCE)hModule, NULL);
	GO_CMD_FOUND = initialize_go_cmd();
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
	if (! settings->Write()){
		::MessageBox(nppData._nppHandle, CONFIGFILE_ERROR, CONFIGURATION_ERROR, MB_OK | MB_ICONERROR);
	}
	autocompletion.reset();
	_cmdDlg.destroy();
	//free(GO_CMD);
}

namespace {

void go_code()
{
	if (!autocompletion.get()) {
		return;
	}
	if (!NppWrapper(nppData).current_file_is_go_file()) {
		return;
	}
	autocompletion->invoke_gocode();
}

void go_calltip()
{
	if (!autocompletion.get()) {
		return;
	}
	if (!NppWrapper(nppData).current_file_is_go_file()) {
		return;
	}
	autocompletion->show_calltip();
}

} // anonymous namespace

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{	
	funcs_array.set(0, _T("go fmt"), go_fmt, false, 'F', MOD_ALT);
	funcs_array.set(1, _T("go test"), go_test, false, 'T', MOD_ALT);
	funcs_array.set(2, _T("go install"), go_install, false, 'I', MOD_ALT);
	funcs_array.set(3, _T("go run"), go_run, false, 'R', MOD_ALT);
	funcs_array.set(4, _T("gocode complete"), go_code, false, 'O', MOD_ALT);
	funcs_array.set(5, _T("gocode calltip"), go_calltip, false, 'P', MOD_ALT);
	funcs_array.set(DOCKABLE_DEMO_INDEX, _T("show output window"), CmdDlgShow, false);

	NppWrapper npp = NppWrapper(nppData);
	tstring settings_filename;
	if (! npp.get_config_file_name(settings_filename)){
		::MessageBox(nppData._nppHandle, CONFIGFILE_ERROR, CONFIGURATION_ERROR, MB_OK | MB_ICONERROR);
	}
	settings.reset( new Settings(settings_filename));
	if (! settings->Read()){
		::MessageBox(nppData._nppHandle, CONFIGFILE_ERROR, CONFIGURATION_ERROR, MB_OK | MB_ICONERROR);
	}
}


//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
	funcs_array.release_shortcut_keys();
	settings->Write();
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
		::MessageBox(nppData._nppHandle, CMD_CREATION_FAILED, MSG_ERROR_HEAD, MB_OK | MB_ICONERROR);
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

	if ( ! run_go_tool(goCmd)) {
		return;
	}

	if( ! goCmd.exitStatus){
		NppWrapper npp(nppData);
		if (npp.reload_all_files()) {
			_cmdDlg.display(false);
		}
	}
}


void go_test(void)
{
	tstring cmd = tstring(_T("test"));
	goCommand goCmd(cmd, tstring());
	if ( ! run_go_tool(goCmd)) {
		return;
	}
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
