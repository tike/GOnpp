#pragma once

class NppData;
class SCNotification;

class AutoCompletion
{
public:
	AutoCompletion(const NppData &);
	~AutoCompletion();

	bool process_notification(SCNotification &n);

protected:
	bool on_char_added(int c);

private:
	const NppData &_nppData;
};

