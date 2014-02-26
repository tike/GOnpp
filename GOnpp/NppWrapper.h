#pragma once

#include "PluginInterface.h"

class NppWrapper
{
public:
	NppWrapper(const NppData);

	LRESULT send_scintilla(UINT msg, WPARAM, LPARAM=0);

private:
	const NppData _npp;
};

