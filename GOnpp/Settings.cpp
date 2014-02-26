#include "Settings.h"

#define SETDEF_GOFMT_ON_SAVE 0
#define SETDEF_AUTO_COMPLETION 0
#define SETDEF_OPEN_ON_ERROR 0
#define SETDEF_RUN_AFTER_SUCCESSFULL_BUILD 0

Settings::Settings(tstring& config_file):
	_config_file(config_file)
{
	this->_auto_completion = SETDEF_AUTO_COMPLETION;
	this->_gofmt_on_save = SETDEF_GOFMT_ON_SAVE;
	this->_open_on_error = SETDEF_OPEN_ON_ERROR;
	this->_run_after_successfull_build = SETDEF_RUN_AFTER_SUCCESSFULL_BUILD;
}

Settings::~Settings(void)
{
}

BOOL Settings::Read(void){
	
	return TRUE;
}

BOOL Settings::Write(void){
	return TRUE;
}

void Settings::go_fmt_on_save_on(void){
	this->_gofmt_on_save = 1;
}

void Settings::go_fmt_on_save_off(void){
	this->_gofmt_on_save = 0;
}

BOOL Settings::go_fmt_is_on(void){
	return this->_gofmt_on_save?TRUE:FALSE;
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
