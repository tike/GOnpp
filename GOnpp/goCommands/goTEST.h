#pragma once
#include "gocommand.h"
class goTEST :
	public goCommand
{
public:
	goTEST(void);
	~goTEST(void);
	void goTEST::DisplayOutput(NppData nppData);
};

