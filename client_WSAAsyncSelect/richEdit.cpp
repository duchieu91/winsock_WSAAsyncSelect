#include "richEdit.h"

namespace rich_edit {
	CHARFORMAT get_char_fmt(HWND hwnd) {
		CHARFORMAT cf;
		SendMessage(hwnd, EM_GETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cf);
		return cf;
	}
	void set_char_fmt(HWND hwnd, const CHARFORMAT& cf) {
		SendMessage(hwnd, EM_SETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cf);
	}
	void replace_sel(HWND hwnd, const wchar_t* wstr) {
		SendMessageW(hwnd, EM_REPLACESEL, 0, (LPARAM)wstr);
	}
	void cursor_to_bottom(HWND hwnd) {
		SendMessage(hwnd, EM_SETSEL, -2, -1);
	}
	void scroll_to(HWND hwnd, DWORD pos) {
		SendMessage(hwnd, WM_VSCROLL, pos, 0);
	}
	void scroll_to_bottom(HWND hwnd) {
		scroll_to(hwnd, SB_BOTTOM);
	}
	// this function is used to output text in different color
	void append(HWND hwnd, COLORREF clr, const wchar_t* wstr) {
		cursor_to_bottom(hwnd); // move cursor to bottom

		CHARFORMAT cf = get_char_fmt(hwnd); // get default char format
		cf.cbSize = sizeof(cf);
		cf.dwMask = CFM_COLOR; // change color
		cf.dwEffects = 0; // add this line
		cf.crTextColor = clr;

		set_char_fmt(hwnd, cf); // set default char format

		replace_sel(hwnd, wstr); // code from google
		scroll_to_bottom(hwnd); // scroll to bottom
	}
}