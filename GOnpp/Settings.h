#pragma once

#include <Windows.h>
#include "PluginInterface.h"
#include "tstring.h"

class Settings
{
public:
	Settings(tstring& config_file);
	~Settings(void);

protected:
	DWORD set_value(tstring& section, tstring& key, int value);
	DWORD set_value(tstring& section, tstring& key, tstring& value);

	DWORD get_value(tstring& section, tstring& key, tstring& default_value, tstring& value);
	DWORD get_value(tstring& section, tstring& key, int default_value, int value);
	
	int _gofmt_on_save;
	int _autocompletion;

	tstring _config_file;
};

