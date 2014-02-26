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

#include <vector>
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
}


goCommand::~goCommand()
{
}

bool goCommand::preRunCmd()
{
	std::vector<TCHAR> raw_gopath(MAX_ENVIRON);

	DWORD length = GetEnvironmentVariable(_T("GOPATH"), &raw_gopath[0], MAX_ENVIRON);
	if (length != 0) {
		return true;
	}
	
	return ::SetEnvironmentVariable(_T("GOPATH"), this->goPath.c_str()) != FALSE;
}

bool goCommand::InitialiseCmd(tstring& go_cmd, tstring& current_file)
{
	this->initializeFileVals(current_file);
	if ( ! this->buildCommandLine(go_cmd)) {
		return false;
	}

	return true;
}

DWORD goCommand::RunCmd(void){
	if ( ! this->preRunCmd()) {
		return FALSE;
	}

	CommandExec exec(this->commandLine, this->currentDir);
	if ( ! exec.Start()) {
		return 200;
	}
	if ( ! exec.Wait()) {
		return 201;
	}
	if ( ! exec.ReadOutput()) {
		return 203;
	}

	this->exitStatus = exec.ExitStatus();
	this->stdOut = exec.GetStdOut();
	this->stdErr = exec.GetStdErr();

	return this->exitStatus;
}

tstring goCommand::GetCommand(void)
{
	return this->commandLine.substr(0, this->commandLine.size());
}

bool goCommand::HasStdOut()
{
	return this->stdOut.size() != 0;
}

tstring goCommand::GetstdOut(void)
{
	return this->stdOut.substr(0, this->stdOut.size());
}

bool goCommand::HasStdErr()
{
	return this->stdErr.size() != 0;
}

tstring goCommand::GetstdErr(void)
{
	return this->stdErr.substr(0, this->stdErr.size());	
}

void goCommand::initializeFileVals(tstring& current_file)
{
	this->currentFile = current_file.substr(0, current_file.size());
	this->currentDir = current_file.substr(0, current_file.size());
	
	size_t last_dir = this->currentDir.rfind(TS_PATH_SEP, std::string::npos);
	if (last_dir != std::string::npos){
		this->currentDir.resize(last_dir);
	}
}

// initialize goPkg,  goPath to appropriate values
bool goCommand::initializeGoVals() {
	size_t src_pos = this->currentDir.find(tstring(_T("src")), 0);
	if (src_pos == std::string::npos){
		// handle error
		return false;
	}

	this->goPath = this->currentDir.substr(0, src_pos - 1);
	this->goPkg = this->currentDir.substr(src_pos + 4, std::string::npos);
	return true;
}

bool goCommand::buildCommandLine(tstring& go_cmd) {
	if( ! this->initializeGoVals()) {
		return false;
	}

	return this->combineCommandLine(go_cmd, this->goPkg);
}

bool goCommand::combineCommandLine(tstring& go_cmd, tstring& pkg)
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