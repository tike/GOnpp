#pragma once

#include "tstring.h"

class Tokens {
	const tstring _src;
	const tstring _separator;
	tstring::size_type _begin, _end;

public:
	Tokens(const tstring &src, const tstring &separator)
		: _src(src), _separator(separator), _begin(0), _end(0)
	{ }

	bool next()
	{
		if (_end==tstring::npos || _separator.length()==0) {
			return false;
		}
		if (_end != 0) {
			_begin = _end + _separator.length();
		}
		_end = _src.find(_separator, _begin);
		return true;
	}

	tstring get() const
	{
		return _src.substr(_begin, _end-_begin);
	}
};

