#pragma once

class TreeView
{
public:
	TreeView(HWND parent, HINSTANCE hInst);
	~TreeView();
	HWND getTreeViewhandler();
	void insertItem(const wchar_t *str);

private:
	TreeView& operator=(const TreeView& other);
	TreeView(TreeView& other);
	HWND handle;
};
