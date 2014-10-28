#include "stdafx.h"
#include "ListBox.h"
#include "Commctrl.h"
#include "Windowsx.h"

ListBox::ListBox(HWND parent, HINSTANCE hInst)
{
	handle= CreateWindowEx(0,WC_LISTBOX
        , NULL
        , WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL
        , 0, 0, 0, 0
        , parent, NULL, hInst, NULL);
}

HWND ListBox::getHandler()
{
	return handle;
}

ListBox::~ListBox()
{
}

void ListBox::insertString()
{
	ListBox_InsertString(handle, -1, TEXT("Hello"));
}
