//Copyright (C)2014 tike <timflex@gmx.de>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

//this file is part of notepad++
//Copyright (C)2003 Don HO ( donho@altern.org )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "CmdDlg.h"
#include "PluginDefinition.h"
#include "tstring.h"

extern NppData nppData;

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

BOOL CALLBACK CmdDlg::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		case WM_SIZE:
		{
			//::MessageBox(nppData._nppHandle, _T("foo"), _T("WM_SIZE"), MB_OK);
			switch(wParam)
			{
				case SIZE_RESTORED:
				{
					//::MessageBox(nppData._nppHandle, _T("SIZE_RESTORED"), _T("WM_SIZE"), MB_OK);
					RECT rc;
					::GetWindowRect(_hSelf, &rc);
					HWND hEdit = GetDlgItem(_hSelf, ID_DUMP);
					::MoveWindow(hEdit, 0, 0, rc.right - rc.left, rc.bottom - rc.top, TRUE);
					break;
				}
			}
			return FALSE;
		}

		default :
			return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
	}
}

void CmdDlg::reshape(int width)
{
	RECT rc;
	::GetWindowRect(_hSelf, &rc);
	::MoveWindow(_hSelf, rc.top, rc.left, width, rc.bottom - rc.top, TRUE);
}

void CmdDlg::prettify(tstring &text)
{
	try {
		const tstring search(_T("\n")), replace(_T("\r\n"));

		// http://stackoverflow.com/a/14678800/98528
		tstring::size_type pos = 0;
		while ((pos = text.find(search, pos)) != tstring::npos) {
			text.replace(pos, search.length(), replace);
			pos += replace.length();
		}
		text += replace;
	} catch (...) {
		text.clear();
	}
}

void CmdDlg::setText(const tstring &text)
{
	tstring buf(text);
	prettify(buf);
	if (buf.empty()) {
		return;
	}

	::SetDlgItemText(this->_hSelf, ID_DUMP, buf.c_str());
}

void CmdDlg::appendText(const tstring &text)
{
	tstring buf(text);
	prettify(buf);
	if (buf.empty()) {
		return;
	}

	HWND hEdit = GetDlgItem(_hSelf, ID_DUMP);
	int ndx = ::GetWindowTextLength(hEdit);
	::SendMessage (hEdit, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
	::SendMessage (hEdit, EM_REPLACESEL, 0, (LPARAM)buf.c_str());
}

void CmdDlg::show(HWND parent, int dialogID)
{
	setParent(parent);

	if (!isCreated())
	{
		tTbData	data = {0};
		create(&data);

		// define the default docking behaviour
		data.uMask = DWS_DF_CONT_RIGHT;

		data.pszModuleName = getPluginFileName();

		data.dlgID = dialogID;
		::SendMessage(parent, NPPM_DMMREGASDCKDLG, 0, (LPARAM)&data);
	}
	display();
}

