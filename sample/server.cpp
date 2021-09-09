#include "main.h"

int iPort = DEFAULT_PORT;
BOOL bInterface = FALSE,   //Nghe tren Ip chi dinh khi TRUE
bRecvOnly = FALSE;    //Chi nhan data. ko back lai data
char szAddress[128];       //dia chi ip cua client

SOCKET sClient=0;

int co_du_lieu = 0;
char in_buffer[DEFAULT_BUFFER];
char hello[4096];
char *help = "Can I help you?";



extern HWND hEditPort, hEditIP, hEdit_Send, hEdit_View;
//Function:usage for console
void usage()
{
}

//Function validation arguments
void ValidateArgs(int argc, char **argv)
{
	int i;
	for (i = 1; i < argc; i++)
	{
		if ((argv[i][0] == '-') || (argv[i][0] == '/'))
		{
			switch (tolower(argv[i][1]))         //tolower doi chu hoa thanh chu thuong
			{
			case 'p':
				iPort = atoi(&argv[i][3]);  //ham atoi chuyen chuoi dc tro toi thanh so nguyen
				break;
			case 'i':
				bInterface = TRUE;
				if (strlen(argv[i]) > 3)
					strcpy(szAddress, &argv[i][3]);
				break;
			case 'o':
				bRecvOnly = TRUE;
				break;
			default:
				usage();
				break;
			}
		}
	}
}

//Ham ClientThread tao vong lap de Socket luon lang nghe Client
DWORD WINAPI ClientThread(LPVOID IpParam)
{
	SOCKET sock = (SOCKET)IpParam;
	char szBuff[DEFAULT_BUFFER];
	int ret;
	//time_t now = time(0);
	//char* rs = ctime(&now);
	while (1)
	{
		//Nhận dữ liệu từ Client
		ret = recv(sock, szBuff, DEFAULT_BUFFER, 0);
		if (ret == 0)
			break;
		else if (ret == SOCKET_ERROR)
		{
			//printf("Connect failed: %d\n", WSAGetLastError());
			break;
		}
		szBuff[ret] = '\0';
		//printf("\nRecv at: %s", rs);
		//printf("RECV from Client:[%d bytes]: %s", ret, szBuff);
		//printf("Can I help you?");

		//Gửi lại dữ liệu cho Client

		if (!bRecvOnly)

		{

			//printf("\nNhap tin nhan gui Client: "); fgets(hello, sizeof(hello), stdin);
			//printf("Send at: %s ", rs);
			ret = send(sock, szBuff, (int)strlen(szBuff), 0);
			if (ret == 0)
				break;
			else if (ret == SOCKET_ERROR)
			{
				//printf("Connect failed: %d \n", WSAGetLastError);
				break;
			}
		}
	}
	return 0;
}

//function: main

DWORD WINAPI creatServer(LPVOID IpParam)
{
	WSADATA  wsd;

	//HANDLE   hThread;
	//DWORD    dwThreadId;
	struct sockaddr_in serverAddr;
	//time_t now = time(0);
	//char* dt = ctime(&now);
	// Load WinSock
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		AppendWindowText(hEdit_View, L" Failed load Winsock! \r\n");
		return 1;
	}

	//Create Listening Socket
	
	g_hSockServer = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

	if (g_hSockServer == SOCKET_ERROR)
	{
		AppendWindowText(hEdit_View, L" Socket() failed: \r\n");
		return 1;
	}
	else AppendWindowText(hEdit_View, L"Server is created! \r\n");
	// Chon dia chi giao tiep va bind

	//server.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY; // htonl(INADDR_ANY);
	serverAddr.sin_port = htons(DEFAULT_PORT);
	if (bind(g_hSockServer, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		AppendWindowText(hEdit_View, L"bind( failed: %d \r\n)");
		return 1;
	} else AppendWindowText(hEdit_View, L"Server is binded. \r\n");

	listen(g_hSockServer, 8);
	WSAAsyncSelect(g_hSockServer, hwnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
	//printf("Server Listening...on IP %s and Port %d\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));
	AppendWindowText(hEdit_View, L"Server is waitting for connection... \r\n");

	WCHAR hostname[NI_MAXHOST], servInfo[NI_MAXSERV];
	GetNameInfoW((struct sockaddr *) &serverAddr, sizeof(struct sockaddr), hostname, NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICHOST);
	MyDisplayText(hEdit_View, 4, hostname, L" is waitting for connection in port ", servInfo, L"\r\n");
	

	return 0;
}