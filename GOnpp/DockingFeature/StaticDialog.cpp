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

#include "StaticDialog.h"

HGLOBAL StaticDialog::makeRTLResource(int dialogID, DLGTEMPLATE **ppMyDlgTemplate)
{
	// Get Dlg Template resource
	HRSRC hDialogRC = ::FindResource(_hInst, MAKEINTRESOURCE(dialogID), RT_DIALOG);
	HGLOBAL hDlgTemplate = ::LoadResource(_hInst, hDialogRC);
	DLGTEMPLATE *pDlgTemplate = (DLGTEMPLATE *)::LockResource(hDlgTemplate);
	
	// Duplicate Dlg Template resource
	unsigned long sizeDlg = ::SizeofResource(_hInst, hDialogRC);
	HGLOBAL hMyDlgTemplate = ::GlobalAlloc(GPTR, sizeDlg);
	*ppMyDlgTemplate = (DLGTEMPLATE *)::GlobalLock(hMyDlgTemplate);

	::memcpy(*ppMyDlgTemplate, pDlgTemplate, sizeDlg);
	
	DLGTEMPLATEEX *pMyDlgTemplateEx = (DLGTEMPLATEEX *)*ppMyDlgTemplate;
	if (pMyDlgTemplateEx->signature == 0xFFFF) {
		pMyDlgTemplateEx->exStyle |= WS_EX_LAYOUTRTL;
	} else {
		(*ppMyDlgTemplate)->dwExtendedStyle |= WS_EX_LAYOUTRTL;
	}

	return hMyDlgTemplate;
}

void StaticDialog::create(int dialogID, bool isRTL)
{
	if (isRTL) {
		DLGTEMPLATE *pMyDlgTemplate = NULL;
		HGLOBAL hMyDlgTemplate = makeRTLResource(dialogID, &pMyDlgTemplate);
		_hSelf = ::CreateDialogIndirectParam(_hInst, pMyDlgTemplate, _hParent, (DLGPROC)dlgProc, (LPARAM)this);
		::GlobalFree(hMyDlgTemplate);
	} else {
		_hSelf = ::CreateDialogParam(_hInst, MAKEINTRESOURCE(dialogID), _hParent, (DLGPROC)dlgProc, (LPARAM)this);
	}

	if (!_hSelf) {
		return;
	}

	::SendMessage(_hParent, NPPM_MODELESSDIALOG, MODELESSDIALOGADD, (WPARAM)_hSelf);
}

BOOL CALLBACK StaticDialog::dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message) {
	case WM_INITDIALOG :
	{
		StaticDialog *pStaticDlg = (StaticDialog *)lParam;
		pStaticDlg->_hSelf = hwnd;
		::SetWindowLongPtr(hwnd, GWL_USERDATA, (long)lParam);
		::GetWindowRect(hwnd, &(pStaticDlg->_rc));
		pStaticDlg->run_dlgProc(message, wParam, lParam);
		return TRUE;
	}

	default :
	{
		StaticDialog *pStaticDlg = (StaticDialog *)::GetWindowLongPtr(hwnd, GWL_USERDATA);
		if (!pStaticDlg) {
			return FALSE;
		}
		return pStaticDlg->run_dlgProc(message, wParam, lParam);
	}
	}
}
