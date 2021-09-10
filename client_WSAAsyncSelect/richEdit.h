#pragma once
#include <Windows.h>
#include <richedit.h>

namespace rich_edit {
	CHARFORMAT get_char_fmt(HWND hwnd);
	void set_char_fmt(HWND hwnd, const CHARFORMAT& cf);
	void replace_sel(HWND hwnd, const wchar_t* wstr);
	void cursor_to_bottom(HWND hwnd);
	void scroll_to(HWND hwnd, DWORD pos);
	void scroll_to_bottom(HWND hwnd);
	// this function is used to output text in different color
	void append(HWND hwnd, COLORREF clr, const wchar_t* wstr);
}

