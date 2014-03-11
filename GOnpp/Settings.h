#pragma once

#include <Windows.h>
#include "PluginInterface.h"
#include "StringUtils/tstring.h"

class Settings
{
public:
	Settings(tstring& config_file);
	~Settings(void);

	BOOL Read(void);
	BOOL Write(void);

	void go_fmt_on_save_on(void);
	void go_fmt_on_save_off(void);
	BOOL go_fmt_is_on(void);

protected:
	DWORD set_value(tstring& section, tstring& key, int value);
	DWORD set_value(tstring& section, tstring& key, tstring& value);

	DWORD get_value(tstring& section, tstring& key, tstring& default_value, tstring& value);
	DWORD get_value(tstring& section, tstring& key, int default_value, int value);
	
	int _gofmt_on_save;
	int _auto_completion;
	int _open_on_error;
	int _run_after_successfull_build;

	tstring _config_file;
};

