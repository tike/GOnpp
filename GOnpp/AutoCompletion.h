#pragma once

#include <windows.h>

struct MessageProcessed {
	bool processed;
	LRESULT result;

	MessageProcessed()
		: processed(0)
		, result(0)
	{ }

	MessageProcessed(bool processed, LRESULT result)
		: processed(processed)
		, result(result)
	{ }

	operator bool ()
	{
		return processed;
	}
};

class AutoCompletion
{
public:
	AutoCompletion();
	~AutoCompletion();

	MessageProcessed ProcessMessage(UINT Message, WPARAM wParam, LPARAM lParam);
};

