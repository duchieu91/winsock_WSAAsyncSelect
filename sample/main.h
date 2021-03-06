#pragma once
#ifndef UNICODE
#define UNICODE
#endif 


#include "resource.h"
#include "server.h"
#include <CommCtrl.h>
#include <windows.h> // Need include after winsock.h
#include <stdarg.h>
#include <iostream> 
#include <cstring>
using namespace std;

#define BTN_CREATE_SERVER		(WM_USER + 1)
#define BTN_SEND				(WM_USER + 2)
#define WINDOW_SEND				(WM_USER + 3)
#define WM_SOCKET				(WM_USER + 4)

extern SOCKET g_hSockServer;
extern SOCKET g_hSockClient;
extern SOCKADDR_IN g_AddrClient;

extern HWND hEditPort, hEditIP, hEdit_View;
extern HWND hEdit_Send;
extern HWND hButton_Send, hButton_Create;
extern HWND hwnd;

extern void AppendWindowText(HWND hwnd, LPCWSTR  lpString);
extern void MyDisplayText(HWND hwnd, UINT num, LPCWSTR lpString, ...);
string replaceAll(string in, string from, string to);