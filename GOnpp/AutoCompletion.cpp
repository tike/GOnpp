#include "AutoCompletion.h"

#include "PluginInterface.h"
#include "Scintilla.h"

#include "CmdDlg.h"

AutoCompletion::AutoCompletion(NppWrapper npp)
	: _npp(npp)
{
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
		_npp.send_scintilla(SCI_AUTOCSHOW, 0, (LPARAM)"foo bar baz");
	}
	return false;
}
