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
	void save_all_files(void);
	void switch_to_file(tstring filename);
	void get_full_current_filename(tstring& filename);

private:
	const NppData _npp;
};

