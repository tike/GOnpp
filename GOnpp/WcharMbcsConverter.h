#pragma once

#include <memory>

class WcharMbcsConverter {
public:
	
	static std::auto_ptr<wchar_t> char2wchar(const char* mbStr);
	static std::auto_ptr<char>    wchar2char(const wchar_t* wcStr);
	
	static std::auto_ptr<TCHAR>   char2tchar(const char* mbStr);
	static std::auto_ptr<char>    tchar2char(const TCHAR* tStr);


};