#include "goINSTALL.h"


goINSTALL::goINSTALL(void)
	: goCommand(_T("install"), NULL)
{
}


goINSTALL::~goINSTALL(void)
{
}

void goINSTALL::DisplayOutput(NppData nppData)
{
	switch(this->exitStatus){
		case 2:
			::MessageBox(nppData._nppHandle, this->stdErr, this->commandLine, MB_OK);
			break;
		case 1:
			::MessageBox(nppData._nppHandle, this->stdErr, this->commandLine, MB_OK);
			break;
		case 0:
			::MessageBox(nppData._nppHandle, _T("build finished!"), this->commandLine, MB_OK);
			break;
	}
}
