#pragma once

#include "Scintilla.h"

class AutoCompletion
{
public:
	AutoCompletion();
	~AutoCompletion();

	bool process_notification(SCNotification &n);

protected:
	bool on_char_added(int c);
};

