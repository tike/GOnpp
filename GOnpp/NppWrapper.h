#pragma once

#include "PluginInterface.h"
#include "tstring.h"

class NppWrapper
{
public:
	NppWrapper(const NppData);

	LRESULT send_scintilla(UINT msg, WPARAM, LPARAM);

	BOOL current_file_is_go_file(void);
	BOOL reload_all_files(void);

private:
	const NppData _npp;
};

