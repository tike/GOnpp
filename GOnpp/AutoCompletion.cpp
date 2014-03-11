#include "AutoCompletion.h"

#include <vector>
#include "PluginInterface.h"
#include "Scintilla.h"

#include "goCommands/gocode.h"
#include "CmdDlg.h"

namespace {
	tstring tgetenv(const tstring &name)
	{
		std::vector<TCHAR> buf(32767);
		DWORD length = ::GetEnvironmentVariable(_T("PATH"), &buf[0], buf.size());
		if (length == 0) {
			return tstring();
		}
		return tstring(&buf[0]);
	}

	class Tokens {
		const tstring &_src;
		const tstring &_separator;
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

	bool regular_file_exists(const tstring &path)
	{
		DWORD attrs = ::GetFileAttributes(path.c_str());
		return attrs != INVALID_FILE_ATTRIBUTES &&
			!(attrs & FILE_ATTRIBUTE_DIRECTORY);
	}

	bool find_file_in_path(const tstring &relative, const tstring &search_roots, tstring &result)
	{
		for (Tokens t(search_roots, _T(";")); t.next(); ) {
			std::vector<TCHAR> buf(MAX_PATH);
			LPTSTR ok = ::PathCombine(&buf[0], t.get().c_str(), _T("gocode.exe"));
			if (!ok) {
				continue;
			}
			if (!regular_file_exists(&buf[0])) {
				continue;
			}
			result.assign(&buf[0]);
			return true;
		}
		return false;
	}

} // anonymous namespace

AutoCompletion::AutoCompletion(NppWrapper npp)
	: _npp(npp)
{
	//TODO: search also in path provided in .ini file

	if (_cmd.length() == 0) {
		find_file_in_path(_T("gocode.exe"), tgetenv(_T("PATH")), _cmd);
	}
	if (_cmd.length() == 0) {
		find_file_in_path(_T("bin\\gocode.exe"), tgetenv(_T("GOPATH")), _cmd);
	}
}


AutoCompletion::~AutoCompletion()
{
}

bool AutoCompletion::process_notification(SCNotification &n)
{
	//TODO:
	// run `gocode -f=csv autocomplete BYTE_OFFSET < FILE_CONTENTS`
	//  or `gocode -f=csv --in=FILE_PATH BYTE_OFFSET`

	switch (n.nmhdr.code) {
	case SCN_CHARADDED:
		return on_char_added(n.ch);
	}
	return false;
}

bool AutoCompletion::invoke_gocode()
{
	int offset = _npp.send_scintilla(SCI_GETCURRENTPOS);
	if (offset == -1) {
		// error failed to receive offset...
		return false;
	}
        tstring file = _npp.get_full_current_filename();
        gocode goc;
        goc.init(file, offset);
        if (! goc.Run()){
              // error failed to run;
              return false;
        }

        vector<completion> completions;
        if ( ! goc.getCompletions(completions)){
                // error failed to receive completions
                return false;
        }

        
        return true;
}

extern CmdDlg _cmdDlg;

bool AutoCompletion::on_char_added(int c)
{
	_cmdDlg.setText(tstring(1, c));
	switch (c) {
	case '.':
		invoke_gocode();
		_npp.send_scintilla(SCI_AUTOCSETSEPARATOR, (WPARAM)'\n');
		_npp.send_scintilla(SCI_AUTOCSHOW, 0, (LPARAM)"foo\nbar\nbaz");
		//_npp.send_scintilla(SCI_AUTOCSHOW, 0, (LPARAM)"foo bar baz");
	}
	return false;
}
