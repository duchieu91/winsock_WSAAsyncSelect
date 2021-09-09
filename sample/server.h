#pragma once
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#pragma comment (lib, "ws2_32.lib")  //winsock library
#pragma warning(disable : 4996)

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define DEFAULT_PORT 54000
#define DEFAULT_BUFFER 4096

extern SOCKET sClient;

void usage();
void ValidateArgs(int , char **);
DWORD WINAPI ClientThread(LPVOID );
DWORD WINAPI creatServer(LPVOID );