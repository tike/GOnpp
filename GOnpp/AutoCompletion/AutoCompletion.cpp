#include "AutoCompletion.h"

#include <vector>
#include "PluginInterface.h"
#include "Scintilla.h"

#include "goCommands/gocode.h"
#include "CmdDlg.h"
#include "StringUtils/tokens.h"
#include "StringUtils/WcharMbcsConverter.h"

namespace {
	tstring tgetenv(const tstring &name)
	{
		std::vector<TCHAR> buf(32767);
		DWORD length = ::GetEnvironmentVariable(name.c_str(), &buf[0], buf.size());
		if (length == 0) {
			return tstring();
		}
		return tstring(&buf[0]);
	}

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
			LPTSTR ok = ::PathCombine(&buf[0], t.get().c_str(), relative.c_str());
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
	, _cmd(search_cmd())
{
}


AutoCompletion::~AutoCompletion()
{
}

tstring AutoCompletion::search_cmd()
{
	//TODO: search also in path provided in .ini file
	tstring cmd;
	if (cmd.length() == 0) {
		find_file_in_path(_T("gocode.exe"), tgetenv(_T("PATH")), cmd);
	}
	if (cmd.length() == 0) {
		find_file_in_path(_T("bin\\gocode.exe"), tgetenv(_T("GOPATH")), cmd);
	}
	return cmd;
}

bool AutoCompletion::process_notification(SCNotification &n)
{
	if (!_npp.current_file_is_go_file()) {
		return false;
	}

	switch (n.nmhdr.code) {
	case SCN_CHARADDED:
		return on_char_added(n.ch);
	}
	return false;
}

// find out length of word before cursor
int AutoCompletion::detect_prefix_length()
{
	LRESULT line_length = _npp.send_scintilla(SCI_GETCURLINE, 0, 0);
	if (line_length > 4000) { // attempt at Defensive Coding
		return 0;
	}
	std::vector<char> buf(line_length);
	LRESULT caret = _npp.send_scintilla(SCI_GETCURLINE, buf.size(), (LPARAM)&buf[0]);
	// FIXME: verify that we're in CODEPAGE utf-8, otherwise `return 0;`
	int prefix=0;
	caret--;  // we're interested only in characters to the left of the cursor
	for (; caret>=0; caret--, prefix++) {
		// FIXME: verify that `caret` points to what I think
		char c = buf[caret];
		if ((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') || (c=='_') || (c&0x80)) {
			// FIXME: what about numbers, is this OK?
			continue;
		}
		return prefix;
	}
	return prefix;
}

bool AutoCompletion::invoke_gocode()
{
	int offset = _npp.send_scintilla(SCI_GETCURRENTPOS);
	if (offset == -1) {
		// error failed to receive offset...
		return false;
	}

	//TODO: instead of saving current file, pipe its contents into gocode.exe's stdin:
	//      `gocode -f=csv autocomplete BYTE_OFFSET < FILE_CONTENTS`
	_npp.send_npp(NPPM_SAVECURRENTFILE);

        tstring file = _npp.get_full_current_filename();

	if (_cmd.empty()) {
		_cmd = search_cmd();
	}
	if (_cmd.empty()) {
		//FIXME: return an error message and show it in the end
		return false;
	}

        vector<completion> completions;
        gocode goc(_cmd);
        if (!goc.run_for(file, offset, completions)) {
              // error, failed to run
              return false;
        }

	tstring buf;
	for (vector<completion>::iterator c=completions.begin(); c!=completions.end(); ++c) {
		if (!buf.empty()) {
			buf.append(_T("\n"));
		}
		buf.append(c->name);
	}

	int prefix_length = detect_prefix_length();

	//unsigned int codepage = _npp.send_scintilla(SCI_GETCODEPAGE);
	_npp.send_scintilla(SCI_AUTOCSETSEPARATOR, (WPARAM)'\n');
	std::vector<char> utf8buf = WcharMbcsConverter::tchar2char(buf.c_str());
	_npp.send_scintilla(SCI_AUTOCSHOW, prefix_length, (LPARAM)&utf8buf[0]);


        return true;
}

extern CmdDlg _cmdDlg;

bool AutoCompletion::on_char_added(int c)
{
	_cmdDlg.setText(tstring(1, c));
	switch (c) {
	case '.':
		invoke_gocode();
	}
	return false;
}