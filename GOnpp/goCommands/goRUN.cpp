#include "goRUN.h"


goRUN::goRUN(void)
	:goCommand(_T("run"), NULL)
{
}


goRUN::~goRUN(void)
{
}

BOOL goRUN::buildCommandLine(LPCTSTR go_cmd)
{
	return this->combineCommandLine(go_cmd, this->currentFile);
}

void goRUN::DisplayOutput(NppData nppData)
{
	switch(this->exitStatus){
		case 2:
			::MessageBox(nppData._nppHandle, this->stdErr, this->commandLine, MB_OK);
			break;
		case 1:
			::MessageBox(nppData._nppHandle, this->stdErr, this->commandLine, MB_OK);
			break;
		case 0:
			::MessageBox(nppData._nppHandle, this->stdOut, this->commandLine, MB_OK);
			break;
	}
}
