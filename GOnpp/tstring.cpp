#include "tstring.h"


void quote_if_spaces(tstring& path){
	int has_space = path.find(tstring(_T(" ")), 0);
	if (has_space == std::string::npos) return;

	path = _T("\"") + path + _T("\"");
	return;
}