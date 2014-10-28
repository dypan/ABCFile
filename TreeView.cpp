#include "stdafx.h"
#include "TreeView.h"
#include "Commctrl.h"

TreeView::TreeView(HWND parent, HINSTANCE hInst)
{
	handle= CreateWindowEx(0,
		WC_TREEVIEW,
		TEXT("Tree View"),
		WS_VISIBLE | WS_CHILD | TVS_FULLROWSELECT | TVS_HASBUTTONS, 
		0, 
		0,
		0, 
		0,	
		parent, 
		(HMENU)1, 
		hInst,
		NULL); 
}

HWND TreeView::getTreeViewhandler()
{
	return handle;
}

TreeView::~TreeView()
{}

void TreeView::insertItem(const wchar_t *str)
{
	TVINSERTSTRUCT insertStruct;
    insertStruct.hParent = NULL;
    insertStruct.hInsertAfter = TVI_ROOT;
    insertStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    insertStruct.item.pszText = (LPWSTR)str;
    insertStruct.item.cchTextMax = sizeof(str)/sizeof(str[0]);
	TreeView_InsertItem(handle, &insertStruct);
    //insertStruct.item.iImage = ;
    //insertStruct.item.iSelectedImage = selectedImageIndex;
}
