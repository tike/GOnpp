#include "AutoCompletion.h"

#include "PluginInterface.h"
#include "Scintilla.h"

#include "CmdDlg.h"

AutoCompletion::AutoCompletion(NppWrapper npp)
	: _npp(npp)
{
	//TODO: find gocode in:
	// a) path specified in .ini file
	// b) PATH
	// c) GOPATH
	//then run `gocode -f=csv autocomplete BYTE_OFFSET < FILE_CONTENTS`
	// or `gocode -f=csv --in=FILE_PATH BYTE_OFFSET`
}


AutoCompletion::~AutoCompletion()
{
}

bool AutoCompletion::process_notification(SCNotification &n)
{
	switch (n.nmhdr.code) {
	case SCN_CHARADDED:
		return on_char_added(n.ch);
	}
	return false;
}

extern CmdDlg _cmdDlg;

bool AutoCompletion::on_char_added(int c)
{
	_cmdDlg.setText(tstring(1, c));
	switch (c) {
	case '.':
		_npp.send_scintilla(SCI_AUTOCSETSEPARATOR, (WPARAM)'\n');
		_npp.send_scintilla(SCI_AUTOCSHOW, 0, (LPARAM)"foo\nbar\nbaz");
		//_npp.send_scintilla(SCI_AUTOCSHOW, 0, (LPARAM)"foo bar baz");
	}
	return false;
}
