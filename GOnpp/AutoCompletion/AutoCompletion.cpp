#include "AutoCompletion.h"

#include <vector>
#include "PluginInterface.h"
#include "Scintilla.h"

#include "goCommands/gocode.h"
#include "CmdDlg.h"
#include "StringUtils/tokens.h"
#include "StringUtils/WcharMbcsConverter.h"

#ifdef max
#undef max
#endif

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

	bool is_go_word_char(char c)
	{
		return (c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') || (c=='_') || (c&0x80);
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
		if (is_go_word_char(c)) {
			// FIXME: what about numbers, is this OK?
			continue;
		}
		return prefix;
	}
	return prefix;
}

bool AutoCompletion::run_gocode(long offset, std::vector<completion> &completions)
{
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

        gocode g(_cmd);
        if (!g.run_for(file, offset, completions)) {
              // error, failed to run
              return false;
        }

	return true;
}

bool AutoCompletion::invoke_gocode()
{
	int offset = _npp.send_scintilla(SCI_GETCURRENTPOS);
	if (offset == -1) {
		// error failed to receive offset...
		return false;
	}

        vector<completion> completions;
	if (!run_gocode(offset, completions)) {
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

bool AutoCompletion::show_calltip()
{
	_npp.send_scintilla(SCI_CALLTIPCANCEL);

	// Invented algorithm:
	// - walk back, until an outstanding open parenthesis '(' is found
	//   - TODO: parse and skip strings
	//   - TODO: walk more than one line, but invent some reasonable stop-condition (open curly bracket '{', or close bracket on column 0?)
	// - walk over the parenthesis too, find preceding word
	//   - if not a word precedes, cancel
	// - then, if not column 0, run gocode
	// - keep only the 'func' results, and only the (first) one matching the word preceding the paren
	// - show calltip

	LRESULT pos = _npp.send_scintilla(SCI_GETCURRENTPOS);

	std::vector<char> buf(std::min<long>(pos, 1024));
	if (buf.size() == 0) {
		return true;
	}

	// retrieve some chunk of text preceding the cursor
	Sci_TextRange range;
	range.chrg.cpMax = pos;
	range.chrg.cpMin = pos - long(buf.size()-1);
	range.lpstrText = &buf[0];
	_npp.send_scintilla(SCI_GETTEXTRANGE, 0, (LPARAM)&range);

	int paren_i = 0;
	int parens = 0;
	int i=buf.size()-2;
	for (; i>=0; i--) {
		// first, try to find unmatched open paren '('
		// FIXME: don't get confused with strings, chars, comments...
		// TODO: to simplify, stop on first newline?
		if (parens >= 0) {
			switch (buf[i]) {
			case ')':
				parens++;
				break;
			case '(':
				parens--;
				paren_i = i;
				break;
			}
			continue;
		}

		if (!is_go_word_char(buf[i])) {
			continue;
		}

		i++;
		break;
	}
	if (i <= 0) {
		return true;
	}

	std::vector<completion> completions;
	if (!run_gocode(pos-long(buf.size())+long(i+1), completions)) {
		return false;
	}

	_cmdDlg.setText(_T("calltips\n"));
	for (std::vector<completion>::iterator c=completions.begin(); c!=completions.end(); ++c) {
		_cmdDlg.appendText(c->type + _T(",,") + c->name + _T(",,") + c->signature);
		if (c->type == _T("func") && c->signature.length()>4) {
			std::vector<char> sig = WcharMbcsConverter::tchar2char(c->signature.c_str() + 4);
			_npp.send_scintilla(SCI_CALLTIPSHOW, pos-long(buf.size())+long(paren_i+1), (LPARAM)&sig[0]);
		}
	}

	if (completions.empty()) {
		return true;
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
	case '(':
	case ')':
		show_calltip();
	}
	return false;
}
