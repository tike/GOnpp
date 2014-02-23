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

#include <algorithm>
#include "goCommand.h"
#include <shlwapi.h>

#ifdef min
#undef min
#endif

goCommand::goCommand(tstring& cmd, tstring& flags)
{
	this->cmd = cmd.substr(0, cmd.size());
	this->flags = cmd.substr(0, flags.size());

/*	this->currentDir = NULL;
	this->currentFile = NULL;

	this->goPath = NULL;
	this->goPkg = NULL;

	this->commandLine = NULL;
	this->stdOut = NULL;
	this->stdErr = NULL;
*/
}


goCommand::~goCommand(void)
{
/*	if ( currentDir != NULL) free(currentDir);
	if ( currentFile != NULL) free(currentFile);

	if ( goPath != NULL) free(goPath);
	if ( goPkg != NULL) free(goPkg);

	if ( commandLine != NULL) free(commandLine);
	if ( stdOut != NULL) free(stdOut);
	if ( stdErr != NULL) free(stdErr);
*/
}

BOOL goCommand::preRunCmd(void)
{
	LPTSTR raw_gopath = (LPTSTR) calloc(MAX_ENVIRON, sizeof(TCHAR));
	if (raw_gopath == NULL){
		return FALSE;
	}

	DWORD length = GetEnvironmentVariable(_T("GOPATH"), raw_gopath, MAX_ENVIRON);
	if (length != 0){
		free(raw_gopath);
		return TRUE;
	}

	free(raw_gopath);
	return ::SetEnvironmentVariable(_T("GOPATH"), this->goPath.c_str());
}

BOOL goCommand::InitialiseCmd(tstring& go_cmd, tstring& current_file)
{
	if ( ! this->initializeFileVals(current_file)) return FALSE;
	if ( ! this->buildCommandLine(go_cmd)) return FALSE;

	return TRUE;
}

DWORD goCommand::RunCmd(void){
	if ( ! this->preRunCmd()) return FALSE;

	CommandExec exec = CommandExec(this->commandLine.c_str(), this->currentDir.c_str());
	if ( ! exec.Start()) return 200;
	if ( ! exec.Wait()) return 201;
	if ( ! exec.ReadOutput()) return 203;

	this->exitStatus = exec.ExitStatus();
	this->stdOut = exec.GetStdOut();
	this->stdErr = exec.GetStdErr();

	//delete(exec);
	return this->exitStatus;
}

tstring goCommand::GetCommand(void)
{
	return this->commandLine.substr(0, this->commandLine.size());
}

BOOL goCommand::HasStdOut(void)
{
	if (this->stdOut.size() == 0) return FALSE;
	return TRUE;
}

tstring goCommand::GetstdOut(void)
{
	return this->stdOut.substr(0, this->stdOut.size());
}

BOOL goCommand::HasStdErr(void)
{
	if (this->stdErr.size() == 0) return FALSE;
	return TRUE;
}

tstring goCommand::GetstdErr(void)
{
	return this->stdErr.substr(0, this->stdErr.size());	
}

BOOL goCommand::initializeFileVals(tstring& current_file)
{
	this->currentFile = current_file.substr(0, current_file.size());
	this->currentDir = current_file.substr(0, current_file.size());
	
	size_t last_dir = this->currentDir.rfind(TS_PATH_SEP, std::string::npos);
	if (last_dir != std::string::npos){
		this->currentDir.resize(last_dir);
	}
	return TRUE;
}

// initialize goPkg,  goPath to appropriate values
BOOL goCommand::initializeGoVals(void){
	size_t src_pos = this->currentDir.find(tstring(_T("src")), 0);
	if (src_pos == std::string::npos){
		// handle error
		return FALSE;
	}

	this->goPath = this->currentDir.substr(0, src_pos - 1);
	this->goPkg = this->currentDir.substr(src_pos + 4, std::string::npos);
	return TRUE;
}

BOOL goCommand::buildCommandLine(tstring& go_cmd){
	if( ! this->initializeGoVals()) return FALSE;

	return this->combineCommandLine(go_cmd, this->goPkg);
}

BOOL goCommand::combineCommandLine(tstring& go_cmd, tstring& pkg)
{
	this->commandLine = tstring(go_cmd);
	this->commandLine.append(_T(" "));

	if ( this->flags.size() > 0){
		this->commandLine.append(go_cmd);
		this->commandLine.append(_T(" "));
	}

	this->commandLine.append(this->cmd);
	this->commandLine.append(_T(" "));
	
	this->commandLine.append(pkg);
	return TRUE;
}