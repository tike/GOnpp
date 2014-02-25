#include "Settings.h"

Settings::Settings(tstring& config_file):
	_config_file(config_file)
{
}

Settings::~Settings(void)
{
}

DWORD Settings::set_value(tstring& section, tstring& key, int value){
	return ::WritePrivateProfileString(section.c_str(), key.c_str(), value?_T("1"):_T("0"), this->_config_file.c_str());
}

DWORD Settings::set_value(tstring& section, tstring& key, tstring& value){
	return ::WritePrivateProfileString(section.c_str(), key.c_str(), value.c_str(), this->_config_file.c_str());
}

DWORD Settings::get_value(tstring& section, tstring& key, tstring& default_value, tstring& value){
	LPTSTR returned_value = (LPTSTR) calloc(MAX_PATH, sizeof(TCHAR));
	if (returned_value == NULL) return 0;

	DWORD ret = ::GetPrivateProfileString(section.c_str(), key.c_str(), default_value.c_str(), returned_value, MAX_PATH, this->_config_file.c_str());
	value.clear();
	value.append(returned_value, MAX_PATH);
	return 1;
}

DWORD Settings::get_value(tstring& section, tstring& key, int default_value, int value){
	return ::GetPrivateProfileInt(section.c_str(), key.c_str(), default_value, this->_config_file.c_str());
}
