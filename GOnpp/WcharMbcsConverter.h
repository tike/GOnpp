#pragma once

#include <vector>

class WcharMbcsConverter {
public:
	
	static std::vector<wchar_t> char2wchar(const char* mbStr);
	static std::vector<char>    wchar2char(const wchar_t* wcStr);
	
	static std::vector<TCHAR>   char2tchar(const char* mbStr);
	static std::vector<char>    tchar2char(const TCHAR* tStr);


};
