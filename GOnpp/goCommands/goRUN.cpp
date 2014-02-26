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

#include "goRUN.h"


goRUN::goRUN(void)
	:goCommand(tstring(_T("run")), tstring())
{
}


goRUN::~goRUN(void)
{
}

bool goRUN::preRunCmd(void)
{
	return true;
}

bool goRUN::buildCommandLine(tstring& go_cmd)
{
	if (this->currentFile.find(_T(" "), 0) != std::string::npos){
		tstring quoted = tstring(_T("\""));
		quoted.append(this->currentFile);
		quoted.append(_T("\""));
		this->currentFile = quoted;
	}
	return this->combineCommandLine(go_cmd, this->currentFile);
}