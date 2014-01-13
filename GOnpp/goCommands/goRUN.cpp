#include "goRUN.h"


goRUN::goRUN(void)
	:goCommand(_T("run"), NULL, false)
{
}


goRUN::~goRUN(void)
{
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
