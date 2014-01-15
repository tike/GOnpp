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

#include "GoToLineDlg.h"
#include "PluginDefinition.h"

extern NppData nppData;

BOOL CALLBACK DemoDlg::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		case WM_COMMAND : 
		{
			switch (wParam)
			{
				case ID_DUMP:
				{
					::MessageBox(nppData._nppHandle, _T("foo"), _T("ID_DUMP"), MB_OK);
					return TRUE;
				}
			}
			return FALSE;
		}

		default :
			return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
	}
}

void DemoDlg::setText(LPTSTR text, bool addCRLF)
{
	::SetDlgItemText(this->_hSelf, ID_DUMP, text);
	if(addCRLF){
		this->appendText(TEXT("\r\n"));
	}
}

void DemoDlg::appendText(LPTSTR text)
{
	HWND hEdit = GetDlgItem(this->_hSelf, ID_DUMP);
	int ndx = GetWindowTextLength(hEdit);
	#ifdef WIN32
      SendMessage (hEdit, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
   #else
      SendMessage (hEdit, EM_SETSEL, 0, MAKELONG (ndx, ndx));
   #endif
      SendMessage (hEdit, EM_REPLACESEL, 0, (LPARAM) ((LPSTR) text));
}
