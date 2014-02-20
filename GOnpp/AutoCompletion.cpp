#include "AutoCompletion.h"


AutoCompletion::AutoCompletion()
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

bool AutoCompletion::on_char_added(int c)
{
	return false;
}
