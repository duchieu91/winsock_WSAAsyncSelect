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

#define BTN_CREATE_SERVER		1
#define BTN_SEND				2
#define WINDOW_SEND				3
#define WM_SOCKET				4

extern SOCKET g_hSockServer;
extern SOCKET g_hSockClient;
extern SOCKADDR_IN g_AddrClient;

extern HWND hEditPort, hEditIP, hEdit_View;
extern HWND hEdit_Send;

extern HWND hwnd;

extern void AppendWindowText(HWND hwnd, LPCWSTR  lpString);
extern void MyDisplayText(HWND hwnd, UINT num, LPCWSTR lpString, ...);
string replaceAll(string in, string from, string to);