#include "AutoCompletion.h"


AutoCompletion::AutoCompletion()
{
}


AutoCompletion::~AutoCompletion()
{
}

MessageProcessed AutoCompletion::ProcessMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
	return MessageProcessed(false, 0);
}
