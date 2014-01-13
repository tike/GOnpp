#pragma once
#include "gocommand.h"

class goFMT :
	public goCommand
{
public:
	goFMT(void);
	~goFMT(void);
	void DisplayOutput(NppData nppData);
};

