#include "goFMT.h"


goFMT::goFMT(void)
	:goCommand(_T("fmt"), NULL, true)
{
	
}


goFMT::~goFMT(void)
{
}

void goFMT::DisplayOutput(NppData nppData)
{
	switch(this->exitStatus){
		case 2:
			::MessageBox(nppData._nppHandle, this->stdErr, this->commandLine, MB_OK);
			break;
		case 1:
			::MessageBox(nppData._nppHandle, this->stdOut, this->commandLine, MB_OK);
		case 0:
			::SendMessage(nppData._nppHandle, NPPM_RELOADFILE, FALSE, (LPARAM)(TCHAR*) this->currentFile);
			break;
	}
}