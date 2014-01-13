#pragma once
#include "gocommand.h"
class goRUN :
	public goCommand
{
public:
	goRUN(void);
	~goRUN(void);
	void DisplayOutput(NppData nppData);
};

