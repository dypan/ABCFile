#pragma once

class ListBox
{
public:
	ListBox(HWND parent, HINSTANCE hInst);
	~ListBox();
	HWND getHandler();
	void insertString();

private:
	ListBox& operator=(const ListBox& other);
	ListBox(const ListBox& other);
	HWND handle;
};

