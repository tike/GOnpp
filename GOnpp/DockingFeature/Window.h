//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
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

#ifndef WINDOW_CONTROL_H
#define WINDOW_CONTROL_H

#include <tchar.h>
#include <windows.h>

class Window
{
public:
	Window()
		: _hInst(NULL)
		, _hParent(NULL)
		, _hSelf(NULL)
	{ }

	virtual ~Window()
	{ }

	virtual void init(HINSTANCE hInst, HWND parent) {
		_hInst = hInst;
		_hParent = parent;
	}

	virtual void destroy() = 0;

	virtual void display(bool show = true) const {
		::ShowWindow(_hSelf, show?SW_SHOW:SW_HIDE);
	}

protected:
	HINSTANCE _hInst;
	HWND _hParent;
	HWND _hSelf;
};

#endif //WINDOW_CONTROL_H


