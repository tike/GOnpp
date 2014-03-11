#pragma once

#include "NppWrapper.h"

class NppData;
class SCNotification;

class AutoCompletion
{
public:
	AutoCompletion(NppWrapper);
	~AutoCompletion();

	bool process_notification(SCNotification &n);
	bool invoke_gocode();

protected:
	bool on_char_added(int c);

private:
	static tstring search_cmd();
	int detect_prefix_length();

	NppWrapper _npp;
	tstring _cmd;
};

