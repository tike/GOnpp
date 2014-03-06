#pragma once

#include "PluginInterface.h"
#include "tstring.h"

class NppWrapper
{
public:
	NppWrapper(const NppData);

	LRESULT send_scintilla(UINT msg, WPARAM, LPARAM=0);

	bool current_file_is_go_file(void);
	bool reload_all_files(void);
	void save_all_files(void);
	void switch_to_file(tstring filename);
	tstring get_full_current_filename();

        int get_current_offset(void);

	// config file operations
	BOOL get_config_file_name(tstring& filepath);

private:
	const NppData _npp;
};

