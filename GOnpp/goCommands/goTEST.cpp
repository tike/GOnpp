#include "goTEST.h"


goTEST::goTEST(void)
	:goCommand(_T("test"), NULL)
{
}


goTEST::~goTEST(void)
{
}

void goTEST::DisplayOutput(NppData nppData)
{
	switch(this->exitStatus){
		case 2:
			::MessageBox(nppData._nppHandle, this->stdErr, this->commandLine, MB_OK);
			break;
		case 1:
		case 0:
			::MessageBox(nppData._nppHandle, this->stdOut, this->cmd, MB_OK);
			break;
	}
}


