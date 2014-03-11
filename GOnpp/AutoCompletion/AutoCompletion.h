#pragma once

#include <vector>
#include "NppWrapper.h"
#include "goCommands/gocode.h"

class NppData;
class SCNotification;

class AutoCompletion
{
public:
	AutoCompletion(NppWrapper);
	~AutoCompletion();

	bool process_notification(SCNotification &n);
	bool invoke_gocode();
	bool show_calltip();

protected:
	bool on_char_added(int c);

private:
	static tstring search_cmd();
	int detect_prefix_length();
	bool run_gocode(long offset, std::vector<completion> &);

	NppWrapper _npp;
	tstring _cmd;
};

