#pragma once
#include "gocommand.h"
class goRUN :
	public goCommand
{
public:
	goRUN(void);
	~goRUN(void);

	BOOL buildCommandLine(LPCTSTR go_cmd);
	void DisplayOutput(NppData nppData);
};

