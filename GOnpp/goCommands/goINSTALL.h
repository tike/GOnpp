#pragma once
#include "gocommand.h"
class goINSTALL :
	public goCommand
{
public:
	goINSTALL(void);
	~goINSTALL(void);
	void DisplayOutput(NppData nppData);
};

