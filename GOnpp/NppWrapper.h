#pragma once

#include "PluginInterface.h"
#include "StringUtils/tstring.h"

class NppWrapper
{
public:
	NppWrapper(const NppData);

	LRESULT send_scintilla(UINT msg, WPARAM=0, LPARAM=0) const;
	LRESULT send_npp(UINT msg, WPARAM=0, LPARAM=0) const;

	bool current_file_is_go_file() const;
	bool reload_all_files() const;
	void save_all_files() const;
	void switch_to_file(tstring filename) const;
	tstring get_full_current_filename() const;

	// config file operations
	BOOL get_config_file_name(tstring& filepath) const;

private:
	const NppData _npp;
};

