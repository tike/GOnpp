#pragma once

#include <algorithm>
#include "PluginInterface.h"

#ifdef min
#undef min
#endif

template< unsigned int Size >
class FuncsArray {
public:
	enum { 
		N = Size
	};
	typedef FuncItem array_type[N];

	array_type items;

	FuncsArray();
	~FuncsArray();
	void set(int index, const tstring &name, PFUNCPLUGINCMD func, bool checked, UCHAR vk_keycode=0, UCHAR key_modifiers=0);
	void release_shortcut_keys();
};

template< unsigned int Size>
FuncsArray<Size>::FuncsArray()
{
	for (int i=0; i<N; i++) {
		items[i]._pShKey = 0;
		items[i]._pFunc = 0;
		items[i]._itemName[0] = '\0';
	}
}

template< unsigned int Size>
FuncsArray<Size>::~FuncsArray()
{
	release_shortcut_keys();
}

template< unsigned int Size>
void FuncsArray<Size>::release_shortcut_keys()
{
	for (int i=0; i<N; i++) {
		delete items[i]._pShKey;
	}
}

template< unsigned int Size>
void FuncsArray<Size>::set(int index, const tstring &name, PFUNCPLUGINCMD func, bool checked, UCHAR vk_keycode, UCHAR key_modifiers)
{
	if (index >= N) {
		return;
	}
	FuncItem &item = items[index];

	// shortcut key
	delete item._pShKey;
	item._pShKey = 0;
	if (vk_keycode != 0) {
		item._pShKey = new ShortcutKey;
		item._pShKey->_isAlt = (key_modifiers & MOD_ALT) != 0;
		item._pShKey->_isCtrl = (key_modifiers & MOD_CONTROL) != 0;
		item._pShKey->_isShift = (key_modifiers & MOD_SHIFT) != 0;
		item._pShKey->_key = vk_keycode;
	}

	// name
	int len = std::min(sizeof(item._itemName)/sizeof(item._itemName[0])-1, name.length());
	name.copy(item._itemName, len);
	item._itemName[len] = '\0';

	// rest of the easier stuff
	item._pFunc = func;
	item._init2Check = checked;
	//item._cmdID = ?
}
