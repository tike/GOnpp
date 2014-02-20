#include "NppWrapper.h"

#include "PluginInterface.h"

NppWrapper::NppWrapper(const NppData npp)
	: _npp(npp)
{
}

LRESULT NppWrapper::send_scintilla(UINT msg, WPARAM wparam, LPARAM lparam)
{
	return ::SendMessage(_npp._scintillaMainHandle, msg, wparam, lparam);
}
