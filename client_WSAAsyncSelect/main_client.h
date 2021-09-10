#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#pragma comment (lib, "ws2_32.lib")  //winsock library
#pragma warning(disable : 4996)
#include <CommCtrl.h>
#include <windows.h> // Need include after winsock.h
#include <stdarg.h>
#include <iostream> 
#include <cstring>
using namespace std;

#define BTN_CONNECT				1001
#define BTN_SEND				1002
#define WINDOW_SEND				1003
#define WM_SOCKET				1004



extern SOCKET g_hSockServer;
extern SOCKET g_hSockClient;
extern SOCKADDR_IN g_AddrClient;

extern HWND hEditPort, hEditIP, hEdit_View;
extern HWND hEdit_Send;
extern HWND hButton_Send, hButton_Connect;
extern HWND hwnd;

extern void AppendWindowText(HWND hwnd, LPCWSTR  lpString);
extern void MyDisplayText(HWND hwnd, UINT num, LPCWSTR lpString, ...);
string replaceAll(string in, string from, string to);