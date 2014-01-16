#include "goRUN.h"


goRUN::goRUN(void)
	:goCommand(_T("run"), NULL)
{
}


goRUN::~goRUN(void)
{
}

BOOL goRUN::preRunCmd(void)
{
	return TRUE;
}

BOOL goRUN::buildCommandLine(LPCTSTR go_cmd)
{
	return this->combineCommandLine(go_cmd, this->currentFile);
}