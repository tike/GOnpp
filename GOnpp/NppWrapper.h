#pragma once

#include "PluginInterface.h"
#include "tstring.h"

class NppWrapper
{
public:
	NppWrapper(const NppData);

	LRESULT send_scintilla(UINT msg, WPARAM, LPARAM);

	bool current_file_is_go_file(void);
	bool reload_all_files(void);
	void save_all_files(void);
	void switch_to_file(tstring filename);
	tstring get_full_current_filename();

private:
	const NppData _npp;
};

