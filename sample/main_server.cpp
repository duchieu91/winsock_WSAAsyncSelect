
#include "main.h"



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AboutBox(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


std::string utf8_encode(const std::wstring &wstr);
std::wstring utf8_decode(const std::string &str);
void CreateForm(HWND hwnd);
void AppendWindowText(HWND hwnd, LPCWSTR  lpString);
void MyDisplayText(HWND, UINT, LPCWSTR,... );

void SocketSend(void);
//
HWND hEditPort = NULL, hEditIP = NULL, hEdit_View = NULL;
HWND hButton_Send, hButton_Create;
HWND hEdit_Send = NULL;
HWND hwnd; //main window

HFONT	g_hFont;
//
SOCKET g_hSockServer;
SOCKET g_hSockClient;
SOCKADDR_IN g_AddrClient;


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Main window";

	WNDCLASS wc = {};
	
	wc.hIcon			= LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wc.lpfnWndProc		= WindowProc;
	wc.hInstance		= hInstance;
	wc.lpszClassName	= CLASS_NAME;
	wc.lpszMenuName		= MAKEINTRESOURCE(IDR_MENU1);

	RegisterClass(&wc);

	// Create the window.
	
	hwnd = CreateWindowExW(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"SERVER TCP/IP",				// Window text
		WS_OVERLAPPEDWINDOW,            // Window style

										// Size and position
		100, 100, 600, 800,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SOCKET:
	{
		SOCKET sock = (SOCKET)wParam;
		UINT nEvent = WSAGETSELECTEVENT(lParam);
		UINT nError = WSAGETSELECTERROR(lParam);

		int iAddrSize = sizeof(g_AddrClient);
		switch (nEvent)
		{
		case FD_ACCEPT:
		{

			WCHAR clientname[NI_MAXHOST], clientInfo[NI_MAXSERV];

			g_hSockClient = accept(sock, (SOCKADDR *)&g_AddrClient, &iAddrSize);
			GetNameInfoW((struct sockaddr *) &g_AddrClient, sizeof(struct sockaddr), clientname, NI_MAXHOST, clientInfo, NI_MAXSERV, NI_NUMERICHOST);
			MyDisplayText(hEdit_View, 4, clientname, L" connected with port ", clientInfo, L"\r\n");
			WSAAsyncSelect(g_hSockClient, hwnd, WM_SOCKET, FD_READ | FD_CLOSE);
			break;
		}
		case FD_READ:
		{
			char buf[1024];
			LPCWSTR wbuf_view = NULL;
			ZeroMemory(buf, 1024);
			int ret = recv(sock, buf, 1024, 0);
			buf[ret] = '\0';
			wstring wbuf = utf8_decode(buf);
			wbuf_view = wbuf.c_str();
			MyDisplayText(hEdit_View, 3, L"Client: ", wbuf_view, L"\r\n");
			break;
		}
		case FD_CLOSE:
		{
			if (g_hSockClient)
			{
				closesocket(g_hSockClient);
				g_hSockClient = (SOCKET)NULL;
				WSACleanup();
			}
			break;
		}
		}
	}
	case WM_DESTROY:
		//PostQuitMessage(0);
		break;
	case WM_CLOSE:
	{
		if (MessageBox(hwnd, L"Really quit?", L"Confirmation", MB_ICONEXCLAMATION | MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(hwnd);
		}
		break;
	}
	case WM_PAINT:
	{
		//PAINTSTRUCT ps;
		//HDC hdc = BeginPaint(hwnd, &ps);

		//FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));
		//EndPaint(hwnd, &ps);
		break;
	}
	case WM_CREATE:
		CreateForm(hwnd);
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_HELP_EXIT:
			PostQuitMessage(0);
			break;
		case ID_HELP_ABOUT:
			DialogBoxW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, AboutBox);
			break;
		case BTN_CREATE_SERVER:
		{
			//WCHAR out[] = L"Server is listenning...";
			//SetWindowTextW(hEdit_View, L"Server is listenning...");
			//AppendWindowText(hEdit_View, L"Server is listenning...\n");
			//Sleep(1000);
			//AppendWindowText(hEdit_View, L"Connected!!!\n");
			//WCHAR pszMem[127];
			//int count = GetWindowTextW(hEdit_Send, pszMem, 127);
			//if (count > 0) AppendWindowText(hEdit_View, L"OK \n");
			//else AppendWindowText(hEdit_View, L"error \n");
			creatServer(NULL);
			/*if (EnableWindow(hButton, FALSE) == 0)
			{
				DWORD	ThreadId;
				HANDLE	hThread = CreateThread(NULL, 0, creatServer, &sClient, 0, &ThreadId);
			} else EnableWindow(hButton, FALSE);*/
			break;
		}
		case BTN_SEND:
		{
			SocketSend();
			break;
		}
		break;
		}
		break;
	}
	}//end switch
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void SocketSend(void)
{
	WCHAR wbuf[1024];
	char *buf_send = NULL;
	ZeroMemory(wbuf, sizeof(wbuf));
	
	GetWindowTextW(hEdit_Send, wbuf, 1024);
	MyDisplayText(hEdit_View, 3, L"Server: ", (LPCWSTR)&wbuf, L"\r\n");
	string buf = utf8_encode(wbuf);
	buf_send = (char *)buf.c_str();
	SetWindowTextW(hEdit_Send, L"");
	//DrawMessage(buf, FALSE);
	if (g_hSockClient)
		send(g_hSockClient, buf_send, (int)buf.length(), 0);
}


LRESULT CALLBACK AboutBox(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hwnd, IDOK);
			break;
		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			break;
		}
		break;
	default:
		return 0;
	}
	return 1;
}
void CreateForm(HWND hwnd)
{
	g_hFont = CreateFontW(18, 0, GM_COMPATIBLE, 0, FW_MEDIUM, 0, 0, 0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		FF_DONTCARE,
		L"Helvetica");

	CreateWindowW(L"Static", L"Server's IP:", WS_VISIBLE | WS_CHILD | SS_LEFT | SS_CENTERIMAGE, 20, 20, 100, 20, hwnd, NULL, NULL, NULL);
	hEditIP = CreateWindowW(L"Edit", L"ANYIP", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE, 120, 20, 200, 20, hwnd, NULL, NULL, NULL);
	CreateWindowW(L"Static", L"Server's Port:", WS_VISIBLE | WS_CHILD | SS_LEFT | SS_CENTERIMAGE, 20, 57, 100, 20, hwnd, NULL, NULL, NULL);
	hEditPort = CreateWindowW(L"Edit", L"54000", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 120, 57, 100, 20, hwnd, NULL, NULL, NULL);
	hButton_Create = CreateWindowW(L"Button", L"CREATE SERVER", WS_VISIBLE | WS_CHILD | SS_CENTER, 50, 100, 150, 35, hwnd, (HMENU)BTN_CREATE_SERVER, NULL, NULL);
	
	//creat view receive text
	CreateWindowW(L"Static", L"Received messages:", WS_VISIBLE | WS_CHILD | SS_LEFT | SS_CENTERIMAGE, 20, 150, 200, 20, hwnd, NULL, NULL, NULL);
	hEdit_View = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VSCROLL, 20, 190, 500, 300, hwnd, NULL, NULL, NULL);

	CreateWindowW(L"Static", L"Send messages:", WS_VISIBLE | WS_CHILD | SS_LEFT | SS_CENTERIMAGE, 20, 500, 220, 20, hwnd, NULL, NULL, NULL);
	hEdit_Send=CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VSCROLL, 20, 550, 300, 150, hwnd, (HMENU)WINDOW_SEND, NULL, NULL);
	hEdit_Send = GetDlgItem(hwnd, WINDOW_SEND);
	hButton_Send = CreateWindowW(L"Button", L"SEND", WS_VISIBLE | WS_CHILD | SS_CENTER, 350, 600, 150, 35, hwnd, (HMENU)BTN_SEND, NULL, NULL);

	//change font 
	SendMessage(hEdit_Send, WM_SETFONT, (WPARAM)g_hFont, 0);
	SendMessage(hButton_Send, WM_SETFONT, (WPARAM)g_hFont, 0);

	SendMessage(hEdit_View, WM_SETFONT, (WPARAM)g_hFont, 0);
	SendMessage(hButton_Create, WM_SETFONT, (WPARAM)g_hFont, 0);
}
void AppendWindowText(HWND hwnd, LPCWSTR  lpString)
{

	HDC hdc = GetDC(hwnd);
	//SetDCPenColor(hdc, RGB(0, 0, 255)); //set to blue
	SetTextColor(hdc, RGB(0, 0, 255));
	int iLength = GetWindowTextLength(hwnd);
	SendMessage(hwnd, EM_SETSEL, iLength, iLength);
	SendMessage(hwnd, EM_REPLACESEL, 0, (LPARAM)lpString);
	SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, (LPARAM)NULL);

}

void MyDisplayText(HWND hwnd, UINT num, LPCWSTR lpString, ...)
{
	//
	va_list valist;
	LPCWSTR temp;
	//WCHAR out[100];
	int len = 0;

	/* initialize valist for num number of arguments */
	va_start(valist, num);

	/* access all the arguments assigned to valist */
	for (UINT i = 0; i < num; i++) 
	{
		temp = va_arg(valist, LPCWSTR);
		AppendWindowText(hwnd, temp);

	}
	/* clean memory reserved for valist */
	va_end(valist);

	//AppendWindowText(hwnd, out);
}
//replace 
string replaceAll(string in, string from, string to)
{
	string tmp = in;

	if (from.empty())
	{
		return in;
	}

	size_t start_pos = 0;

	// tmp.find() fails to match on "\0"
	while ((start_pos = tmp.find(from, start_pos)) != std::string::npos)
	{
		tmp.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}

	return tmp;
}

// Convert wtring to string
std::string utf8_encode(const std::wstring &wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string &str)
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}