// Win32Project1.cpp : 定义控制台应用程序的入口点。
//https://msdn.microsoft.com/query/dev14.query?appId=Dev14IDEF1&l=ZH-CN&k=k(IPHlpApi%2FGetAdaptersAddresses);k(GetAdaptersAddresses);k(DevLang-C%2B%2B);k(TargetOS-Windows)&rd=true

#include "stdafx.h"

#include<WinSock2.h>  
#include<WS2tcpip.h>  
#include<iostream>  
#include<IPHlpApi.h>  
using namespace std;
#pragma comment(lib, "IPHLPAPI.lib")//GetAdaptersAddresses
#pragma comment(lib, "Ws2_32.lib")//inet_ntop



#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


#include <list>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_IP "192.168.1.3"
#define DEFAULT_PORT "60000"
#define DEFAULT_PATH "NPC.bin"

//客户端官方示例
int  test2(void)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;
	char* sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Validate the parameters
	/*if (argc != 2) {
		printf("usage: %s server-name\n", argv[0]);
		return 1;
	}*/

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo("192.168.1.31", DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// Receive until the peer closes the connection
	do {

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}
// 获取网卡信息
int test1()
{
	PIP_ADAPTER_ADDRESSES pAddresses = nullptr;
	IP_ADAPTER_DNS_SERVER_ADDRESS* pDnServer = nullptr;
	ULONG outBufLen = 0;
	DWORD dwRetVal = 0;
	char buff[100];
	DWORD bufflen = 100;
	int i;

	GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen);

	pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);

	if ((dwRetVal = GetAdaptersAddresses(AF_INET, GAA_FLAG_SKIP_ANYCAST, NULL, pAddresses, &outBufLen)) == NO_ERROR) {

		while (pAddresses) {
			printf("%S, %.2x-%.2x-%.2x-%.2x-%.2x-%.2x: \n",
				pAddresses->FriendlyName,
				pAddresses->PhysicalAddress[0], pAddresses->PhysicalAddress[1],
				pAddresses->PhysicalAddress[2], pAddresses->PhysicalAddress[3],
				pAddresses->PhysicalAddress[4], pAddresses->PhysicalAddress[5]);

			PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pAddresses->FirstUnicastAddress;
			pDnServer = pAddresses->FirstDnsServerAddress;

			printf("\tAdapter name: %s\n", pAddresses->AdapterName);


			if (pDnServer)
			{
				sockaddr_in* sa_in = (sockaddr_in*)pDnServer->Address.lpSockaddr;
				printf("DNS:%s\n", inet_ntop(AF_INET, &(sa_in->sin_addr), buff, bufflen));
			}
			if (pAddresses->OperStatus == IfOperStatusUp)
			{
				printf("Status: active\n");
			}
			else
			{
				printf("Status: deactive\n");
			}


			for (i = 0; pUnicast != NULL; i++)
			{
				if (pUnicast->Address.lpSockaddr->sa_family == AF_INET)
				{
					sockaddr_in* sa_in = (sockaddr_in*)pUnicast->Address.lpSockaddr;
					printf("IPV4 Unicast Address:%s\n", inet_ntop(AF_INET, &(sa_in->sin_addr), buff, bufflen));
				}
				else if (pUnicast->Address.lpSockaddr->sa_family == AF_INET6)
				{
					sockaddr_in6* sa_in6 = (sockaddr_in6*)pUnicast->Address.lpSockaddr;
					printf("IPV6:%s\n", inet_ntop(AF_INET6, &(sa_in6->sin6_addr), buff, bufflen));
				}
				else
				{
					printf("\tUNSPEC");
				}
				pUnicast = pUnicast->Next;
			}
			printf("Number of Unicast Addresses: %d\n", i);
			pAddresses = pAddresses->Next;
		}
	}
	else {
		LPVOID lpMsgBuf;
		printf("Call to GetAdaptersAddresses failed.\n");
		if (FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwRetVal,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0,
			NULL)) {
			printf("\tError: %s", lpMsgBuf);
		}
		LocalFree(lpMsgBuf);
	}
	free(pAddresses);

	return 0;
}
/**
* @brief  计算字符串的CRC值
*
* @param  ptr     字符串的指针
*         len     字符串的长度
*
* @retval CRC值
*
* @note
*/

uint16_t getCRC16(uint8_t* ptr, uint16_t len)
{
	uint8_t i;
	uint16_t crc = 0xFFFF;
	if (len == 0) {
		len = 1;
	}
	while (len--) {
		crc ^= *ptr;
		for (i = 0; i < 8; i++) {
			if (crc & 1) {
				crc >>= 1;
				crc ^= 0xA001;
			}
			else {
				crc >>= 1;
			}
		}
		ptr++;
	}
	return(crc);
}

/**
* @brief  累次计算字符串的CRC值
*
* @param  ptr     字符串的指针
*         len     字符串的长度
*
* @retval CRC值
*
* @note
*/

uint16_t oneCRC16(uint8_t ch, uint16_t crc)
{
	uint8_t i;

	crc ^= ch;
	for (i = 0; i < 8; i++) {
		if (crc & 1) {
			crc >>= 1;
			crc ^= 0xA001;
		}
		else {
			crc >>= 1;
		}
	}

	return(crc);
}

void charTowchar(const char* chr, wchar_t* wchar, int size)
{
	MultiByteToWideChar(CP_ACP, 0, chr,
		strlen(chr) + 1, wchar, size / sizeof(wchar[0]));
}


void wcharTochar(const wchar_t* wchar, char* chr, int length)
{
	WideCharToMultiByte(CP_ACP, 0, wchar, -1,
		chr, length, NULL, NULL);
}

wchar_t* CharToWchar(const char* c, size_t m_encode = CP_ACP)
{
	int len = MultiByteToWideChar(m_encode, 0, c, strlen(c), NULL, 0);
	wchar_t* m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(m_encode, 0, c, strlen(c), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}

char* WcharToChar(const wchar_t* wp, size_t m_encode = CP_ACP)
{
	int len = WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
	char* m_char = new char[len + 1];
	WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}
//在线升级老板
int  test3(void)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;
	char* sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;



	// Validate the parameters
	/*if (argc != 2) {
	printf("usage: %s server-name\n", argv[0]);
	return 1;
	}*/
	char ip[20];
	printf("输入ip\n");
	gets_s(ip, 20);
	char port[10];
	printf("输入port\n");
	gets_s(port, 10);
	int isOta = 0;
test3connect:
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(ip, port, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server %s : %s!\n", ip, port);
		WSACleanup();
		return 1;
	}
	printf("Successed to connect to server %s : %s!\n", ip, port);

	if (!isOta) {

		// send ota
		UCHAR uchBuf[20] = { 0xF3, 0x04 ,0,0 };
		uint16_t crc;
		crc = getCRC16(uchBuf, 4);
		memcpy_s(uchBuf + 4, sizeof(uchBuf), &crc, 2);

		// Send an initial buffer
		iResult = send(ConnectSocket, (char*)uchBuf, 6, 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}
		isOta = 1;

		printf("restart and reconnect\n");
		Sleep(1000);//getchar();
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
		WSACleanup();
		goto test3connect;
	}
	else
	{

	}


	//getchar();

	char path[MAX_PATH];
	printf("输入文件目录\n");
	gets_s(path, MAX_PATH);

	wchar_t* wpath = CharToWchar(path);


	/////////////////////////////////////////////////////////////////////
	HANDLE hFile = ::CreateFile(wpath,     //创建文件的名称。
		GENERIC_READ,          // 写和读文件。
		FILE_SHARE_READ,                      // 不共享读写。
		NULL,                   // 缺省安全属性。
		OPEN_EXISTING,          // 如果文件存在，也创建。
		FILE_ATTRIBUTE_NORMAL, // 一般的文件。       
		NULL);                 // 模板文件为空。
	if (hFile == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(TEXT("CreateFile fail!\r\n"));
	}

	//////////
	//发送命令
	//////////


	LARGE_INTEGER size;
	if (!::GetFileSizeEx(hFile, &size))
	{
		OutputDebugString(TEXT("获取文件失败\r\n"));;
	}

	printf("File(%s) Size: %ld\n", wpath, size);
	delete wpath;
	Sleep(500);//getchar();
	  //crc
	UCHAR uchBuf[20] = { 0xF3, 0x06, 0x6C, 0x81, 0x03, 0x00 };
	memcpy_s(uchBuf + 2, sizeof(uchBuf), &size, 4);
	uint16_t crc;
	crc = getCRC16(uchBuf, 6);
	memcpy_s(uchBuf + 6, sizeof(uchBuf), &crc, 2);

	// Send an initial buffer
	iResult = send(ConnectSocket, (char*)uchBuf, 8, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	// 等待接收初始化完成
	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0) {
		recvbuf[iResult] = 0x00;
		printf("Bytes received: %s\n", recvbuf);
	}
	else if (iResult == 0)
		printf("Connection closed\n");
	else
		printf("recv failed with error: %d\n", WSAGetLastError());

	Sleep(500); //getchar();
	  //////////
	  //发送脚本
	  //////////
	char chBuffer[4096];
	crc = 0xFFFF;
	DWORD dwReadSize = 0;
	::ZeroMemory(chBuffer, 4096);
	LONG lDistance = 0;
	while (1)
	{
		BOOL bRet = ::ReadFile(hFile, chBuffer, 0x1000, &dwReadSize, NULL);
		if (bRet && dwReadSize)
		{
			//OutputDebugString(TEXT("ReadFile 读文件成功\r\n"));
		}
		else
		{
			//获取出错码。
			DWORD dwError = GetLastError();
			//处理出错。            
			TCHAR chErrorBuf[1024];
			wsprintf(chErrorBuf, TEXT("GetLastError()=%d\r\n"), dwError);
			OutputDebugString(chErrorBuf);
			break;
		}

		//for (int i = 0; i < dwReadSize; i++) {
		//	crc = oneCRC16(chBuffer[i], crc);
		//}
		lDistance += dwReadSize;
		DWORD dwPtr = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
		if (dwPtr == INVALID_SET_FILE_POINTER &&
			GetLastError() != NO_ERROR) // Test for failure
		{
			// Obtain the error code. 
			DWORD dwError = GetLastError();

			// Deal with failure 
			// . . . 

		} // End of error handler 


		if (bRet)
		{
			//OutputDebugString(TEXT("ReadFile 读文件成功\r\n"));
			// Send an initial buffer
			iResult = send(ConnectSocket, chBuffer, dwReadSize, 0);
			if (iResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ConnectSocket);
				WSACleanup();
				return 1;
			}
		}
	}
	////////////////////////////////////////////////

	// Send an initial buffer
	/*iResult = send(ConnectSocket, (char*)&crc, 2, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}*/

	printf("Bytes Sent: %ld\n", lDistance);

	// shutdown the connection since no more data will be sent
	//iResult = shutdown(ConnectSocket, SD_SEND);
	//if (iResult == SOCKET_ERROR) {
	//	printf("shutdown failed with error: %d\n", WSAGetLastError());
	//	closesocket(ConnectSocket);
	//	WSACleanup();
	//	return 1;
	//}

	// Receive until the peer closes the connection
	//do {

	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0) {
		recvbuf[iResult] = 0x00;
		printf("Bytes received: %s\n", recvbuf);
	}
	else if (iResult == 0)
		printf("Connection closed\n");
	else
		printf("recv failed with error: %d\n", WSAGetLastError());

	//} while (iResult > 0);

	printf("CDV RESET");
	getchar();
	// cleanup
	iResult = send(ConnectSocket, "CDV RESET", 9, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	CloseHandle(hFile);
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}
//在线升级老板
int  test3_all(std::list<string>& arr)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;
	char* sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;



	// Validate the parameters
	/*if (argc != 2) {
	printf("usage: %s server-name\n", argv[0]);
	return 1;
	}*/
	char ip[20];
	//printf("输入ip\n");
	//gets_s(ip, 20);
	char port[10];
	//printf("输入port\n");
	//gets_s(port, 10);
	int isOta = 0;

	//int count = arr.size();
	memset(ip, 0, 20);
	memset(port, 0, 10);

	//getchar();
	// 获取文件
	char path[MAX_PATH];
	printf("输入文件目录\n");
	gets_s(path, MAX_PATH);

	wchar_t* wpath = CharToWchar(path);


	/////////////////////////////////////////////////////////////////////
	HANDLE hFile = ::CreateFile(wpath,     //创建文件的名称。
		GENERIC_READ,          // 写和读文件。
		FILE_SHARE_READ,                      // 不共享读写。
		NULL,                   // 缺省安全属性。
		OPEN_EXISTING,          // 如果文件存在，也创建。
		FILE_ATTRIBUTE_NORMAL, // 一般的文件。       
		NULL);                 // 模板文件为空。
	if (hFile == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(TEXT("CreateFile fail!\r\n"));
	}

	//////////
	//发送命令
	//////////


	LARGE_INTEGER size = { 0 };
	if (!::GetFileSizeEx(hFile, &size))
	{
		OutputDebugString(TEXT("获取文件失败\r\n"));;
	}
	
	printf("File(%s) Size: %ld\n", wpath, size);

	//size.QuadPart = size.QuadPart + 2;
	delete wpath;
	//Sleep(500);//getchar();
	  //crc
	//UCHAR uchBuf[20] = { 0xF3, 0x06, 0x6C, 0x81, 0x03, 0x00 };

	//UCHAR uchBuf[20] = { 0xF1, 0x10, 0x00, 0x00, 0x01, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	////uchBuf[7] = function;
	//memcpy_s(uchBuf + 8, sizeof(uchBuf), &size, 4);
	//uint16_t crc;
	//crc = getCRC16(uchBuf, 16);
	//memcpy_s(uchBuf + 16, sizeof(uchBuf), &crc, 2);
	std::list<string>::iterator it = arr.begin();

test3_allnext:

	LONG lDistance = 0;
	DWORD dwPtr = SetFilePointer(hFile, lDistance, NULL, FILE_BEGIN);

	if (it != arr.end())
	{
		char* buf = (char*)it->c_str();
		char* idPos = strstr(buf, "ID:") + 3;
		char* ipPos = strstr(buf, "IP:") + 3;
		char* portPos = strstr(buf, "PORT:") + 5;


		if (0 == strncmp(buf, "yes", 3))
		{

			int idLen = strstr(idPos, "\r\n") - idPos;
			int ipLen = strstr(ipPos, "\r\n") - ipPos;
			int portLen = strstr(portPos, "\r\n") - portPos;


			memcpy(ip, ipPos, ipLen);
			memcpy(port, portPos, portLen);
			goto test3_allconnect;
		}
	}

	CloseHandle(hFile);
	return 0;
test3_allconnect:
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(ip, port, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server %s : %s!\n", ip, port);
		WSACleanup();
		return 1;
	}
	printf("Successed to connect to server %s : %s!\n", ip, port);

	if (!isOta) {

		// send ota
		UCHAR uchBuf[20] = { 0xF3, 0x04 ,0,0 };
		uint16_t crc;
		crc = getCRC16(uchBuf, 4);
		memcpy_s(uchBuf + 4, sizeof(uchBuf), &crc, 2);

		// Send an initial buffer
		iResult = send(ConnectSocket, (char*)uchBuf, 6, 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}
		isOta = 1;

		printf("restart and reconnect\n");
		Sleep(1000);//getchar();
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
		WSACleanup();
		goto test3_allconnect;
	}
	else
	{
		isOta = 0;
	}


	//getchar();

	Sleep(500);//getchar();
	  //crc
	UCHAR uchBuf[20] = { 0xF3, 0x06, 0x6C, 0x81, 0x03, 0x00 };
	memcpy_s(uchBuf + 2, sizeof(uchBuf), &size, 4);
	uint16_t crc;
	crc = getCRC16(uchBuf, 6);
	memcpy_s(uchBuf + 6, sizeof(uchBuf), &crc, 2);

	// Send an initial buffer
	iResult = send(ConnectSocket, (char*)uchBuf, 8, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	// 等待接收初始化完成
	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0) {
		recvbuf[iResult] = 0x00;
		printf("Bytes received: %s\n", recvbuf);
	}
	else if (iResult == 0)
		printf("Connection closed\n");
	else
		printf("recv failed with error: %d\n", WSAGetLastError());

	Sleep(500); //getchar();
	  //////////
	  //发送脚本
	  //////////
	char chBuffer[4096];
	crc = 0xFFFF;
	DWORD dwReadSize = 0;
	::ZeroMemory(chBuffer, 4096);
	/*LONG*/ lDistance = 0;
	while (1)
	{
		BOOL bRet = ::ReadFile(hFile, chBuffer, 0x1000, &dwReadSize, NULL);
		if (bRet && dwReadSize)
		{
			//OutputDebugString(TEXT("ReadFile 读文件成功\r\n"));
		}
		else
		{
			//获取出错码。
			DWORD dwError = GetLastError();
			//处理出错。            
			TCHAR chErrorBuf[1024];
			wsprintf(chErrorBuf, TEXT("GetLastError()=%d\r\n"), dwError);
			OutputDebugString(chErrorBuf);
			break;
		}

		//for (int i = 0; i < dwReadSize; i++) {
		//	crc = oneCRC16(chBuffer[i], crc);
		//}
		lDistance += dwReadSize;
		DWORD dwPtr = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
		if (dwPtr == INVALID_SET_FILE_POINTER &&
			GetLastError() != NO_ERROR) // Test for failure
		{
			// Obtain the error code. 
			DWORD dwError = GetLastError();

			// Deal with failure 
			// . . . 

		} // End of error handler 


		if (bRet)
		{
			//OutputDebugString(TEXT("ReadFile 读文件成功\r\n"));
			// Send an initial buffer
			iResult = send(ConnectSocket, chBuffer, dwReadSize, 0);
			if (iResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ConnectSocket);
				WSACleanup();
				return 1;
			}
		}
	}
	////////////////////////////////////////////////

	// Send an initial buffer
	/*iResult = send(ConnectSocket, (char*)&crc, 2, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}*/

	printf("Bytes Sent: %ld\n", lDistance);

	// shutdown the connection since no more data will be sent
	//iResult = shutdown(ConnectSocket, SD_SEND);
	//if (iResult == SOCKET_ERROR) {
	//	printf("shutdown failed with error: %d\n", WSAGetLastError());
	//	closesocket(ConnectSocket);
	//	WSACleanup();
	//	return 1;
	//}

	// Receive until the peer closes the connection
	//do {

	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0) {
		recvbuf[iResult] = 0x00;
		printf("Bytes received: %s\n", recvbuf);
	}
	else if (iResult == 0)
		printf("Connection closed\n");
	else
		printf("recv failed with error: %d\n", WSAGetLastError());

	//} while (iResult > 0);

	printf("CDV RESET\r\n");
	//getchar();
	// cleanup
	iResult = send(ConnectSocket, "CDV RESET", 9, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ConnectSocket);
	WSACleanup();
	it++; 
	goto test3_allnext;
	return 0;
}

//在线升级app版3.2.3.23开始
int  test3_2(void)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;
	char* sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;


test3connect:
	// Validate the parameters
	/*if (argc != 2) {
	printf("usage: %s server-name\n", argv[0]);
	return 1;
	}*/
	char ip[20];
	printf("输入ip\n");
	gets_s(ip, 20);
	char port[10];
	printf("输入port\n");
	gets_s(port, 10);
	int isOta = 0;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(ip, port, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server %s : %s!\n", ip, port);
		WSACleanup();
		return 1;
	}
	printf("Successed to connect to server %s : %s!\n", ip, port);

	//if (!isOta) {

	//	// send ota
	//	UCHAR uchBuf[20] = { 0xF3, 0x04 ,0,0 };
	//	UCHAR uchBuf[20] = { 0xF1, 0x10, 0x00, 0x00, 0x01, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	//	uint16_t crc;
	//	crc = getCRC16(uchBuf, 4);
	//	memcpy_s(uchBuf + 4, sizeof(uchBuf), &crc, 2);

	//	// Send an initial buffer
	//	iResult = send(ConnectSocket, (char*)uchBuf, 6, 0);
	//	if (iResult == SOCKET_ERROR) {
	//		printf("send failed with error: %d\n", WSAGetLastError());
	//		closesocket(ConnectSocket);
	//		WSACleanup();
	//		return 1;
	//	}
	//	isOta = 1;

	//	printf("restart and reconnect\n");
	//	Sleep(1000);//getchar();
	//	closesocket(ConnectSocket);
	//	ConnectSocket = INVALID_SOCKET;
	//	WSACleanup();
	//	goto test3connect;
	//}
	//else
	//{

	//}
	char function = 0;
	printf("输入功能码：\n");
	printf("0————下载脚本\n");
	printf("1————下载固件\n");
	printf("A————更新BT\n");
	printf("其他————退出\n");
	int ret = getchar();
	if (ret == '0')
		function = 0;
	else if (ret == '1')
		function = 0x20;
	else if (ret == 'A')
		function = 0xA0;
	else
		exit(0);

	ret = getchar();//接收回车

	//getchar();
	// 获取文件
	char path[MAX_PATH];
	printf("输入文件目录\n");
	gets_s(path, MAX_PATH);

	wchar_t* wpath = CharToWchar(path);


	/////////////////////////////////////////////////////////////////////
	HANDLE hFile = ::CreateFile(wpath,     //创建文件的名称。
		GENERIC_READ,          // 写和读文件。
		FILE_SHARE_READ,                      // 不共享读写。
		NULL,                   // 缺省安全属性。
		OPEN_EXISTING,          // 如果文件存在，也创建。
		FILE_ATTRIBUTE_NORMAL, // 一般的文件。       
		NULL);                 // 模板文件为空。
	if (hFile == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(TEXT("CreateFile fail!\r\n"));
	}

	//////////
	//发送命令
	//////////


	LARGE_INTEGER size = { 0 };
	if (!::GetFileSizeEx(hFile, &size))
	{
		OutputDebugString(TEXT("获取文件失败\r\n"));;
	}

	printf("File(%s) Size: %ld\n", wpath, size);

	//size.QuadPart = size.QuadPart + 2;
	delete wpath;
	Sleep(500);//getchar();
	  //crc
	//UCHAR uchBuf[20] = { 0xF3, 0x06, 0x6C, 0x81, 0x03, 0x00 };

	UCHAR uchBuf[20] = { 0xF1, 0x10, 0x00, 0x00, 0x01, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uchBuf[7] = function;
	memcpy_s(uchBuf + 8, sizeof(uchBuf), &size, 4);
	uint16_t crc;
	crc = getCRC16(uchBuf, 16);
	memcpy_s(uchBuf + 16, sizeof(uchBuf), &crc, 2);

	// Send an initial buffer
	iResult = send(ConnectSocket, (char*)uchBuf, 18, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	// 等待接收初始化完成
	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0) {
		//recvbuf[iResult] = 0x00;
		for (int i = 0; i < iResult; i++) {
			printf("%02x ", (UINT8)recvbuf[i]);
		}
		printf("\n");
	}
	else if (iResult == 0)
		printf("Connection closed\n");
	else
		printf("recv failed with error: %d\n", WSAGetLastError());

	Sleep(300); //getchar();
	  //////////
	  //发送脚本
	  //////////
	char chBuffer[4096];
	crc = 0xFFFF;
	DWORD dwReadSize = 0;
	LONG lDistance = 0;

	::ZeroMemory(chBuffer, 4096);
	//while (1)// 计算crc
	//{
	//	BOOL bRet = ::ReadFile(hFile, chBuffer, 0x1000, &dwReadSize, NULL);
	//	if (bRet && dwReadSize)
	//	{
	//		//OutputDebugString(TEXT("ReadFile 读文件成功\r\n"));
	//	}
	//	else
	//	{
	//		//获取出错码。
	//		DWORD dwError = GetLastError();
	//		//处理出错。            
	//		TCHAR chErrorBuf[1024];
	//		wsprintf(chErrorBuf, TEXT("GetLastError()=%d\r\n"), dwError);
	//		OutputDebugString(chErrorBuf);
	//		break;
	//	}

	//	for (int i = 0; i < dwReadSize; i++) {
	//		crc = oneCRC16(chBuffer[i], crc);
	//	}
	//	lDistance += dwReadSize;
	//	DWORD dwPtr = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
	//	if (dwPtr == INVALID_SET_FILE_POINTER &&
	//		GetLastError() != NO_ERROR) // Test for failure
	//	{
	//		// Obtain the error code. 
	//		DWORD dwError = GetLastError();

	//		// Deal with failure 
	//		// . . . 

	//	} // End of error handler 
	//}
	lDistance = 0;
	while (1)// 发送
	{
		BOOL bRet = ::ReadFile(hFile, chBuffer, 0x1000, &dwReadSize, NULL);
		if (bRet && dwReadSize)
		{
			//OutputDebugString(TEXT("ReadFile 读文件成功\r\n"));
		}
		else
		{
			//获取出错码。
			DWORD dwError = GetLastError();
			//处理出错。            
			TCHAR chErrorBuf[1024];
			wsprintf(chErrorBuf, TEXT("GetLastError()=%d\r\n"), dwError);
			OutputDebugString(chErrorBuf);
			break;
		}
			for (int i = 0; i < dwReadSize; i++) {
				crc = oneCRC16(chBuffer[i], crc);
			}
		lDistance += dwReadSize;
		DWORD dwPtr = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
		if (dwPtr == INVALID_SET_FILE_POINTER &&
			GetLastError() != NO_ERROR) // Test for failure
		{
			// Obtain the error code. 
			DWORD dwError = GetLastError();

			// Deal with failure 
			// . . . 

		} // End of error handler 

		Sleep(10);
		if (bRet)
		{
			//OutputDebugString(TEXT("ReadFile 读文件成功\r\n"));
			// Send an initial buffer
			iResult = send(ConnectSocket, chBuffer, dwReadSize, 0);
			if (iResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ConnectSocket);
				WSACleanup();
				printf("Bytes Sent: %ld\n", lDistance);
				return 1;
			}
		}
	}
	Sleep(10);
	// Send crc
	iResult = send(ConnectSocket, (char*)&crc, 2, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		printf("Bytes Sent: %ld\n", lDistance);
		return 1;
	}
	lDistance += 2;

	printf("CRC: %x\n", (UINT16)crc);
	////////////////////////////////////////////////

	printf("Bytes Sent: %ld\n", lDistance);

	// shutdown the connection since no more data will be sent
	//iResult = shutdown(ConnectSocket, SD_SEND);
	//if (iResult == SOCKET_ERROR) {
	//	printf("shutdown failed with error: %d\n", WSAGetLastError());
	//	closesocket(ConnectSocket);
	//	WSACleanup();
	//	return 1;
	//}

	// Receive until the peer closes the connection
	//do {

	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0) {
		/*recvbuf[iResult] = 0x00;
		printf("Bytes received: %s\n", recvbuf);*/
		for (int i = 0; i < iResult; i++) {
			printf("%02x ", (UINT8)recvbuf[i]);
		}
		printf("\n");
	}
	else if (iResult == 0)
		printf("Connection closed\n");
	else
		printf("recv failed with error: %d\n", WSAGetLastError());

	//} while (iResult > 0);

	printf("OVER");
	getchar();
	//// cleanup
	//iResult = send(ConnectSocket, "CDV RESET", 9, 0);
	//if (iResult == SOCKET_ERROR) {
	//	printf("send failed with error: %d\n", WSAGetLastError());
	//	closesocket(ConnectSocket);
	//	WSACleanup();
	//	return 1;
	//}

	CloseHandle(hFile);
	closesocket(ConnectSocket);
	WSACleanup();

	printf("继续升级？ y/n \n");
	ret = getchar();
	if (ret == 'y') {
		getchar();
		goto test3connect;
	}
	return 0;
}
//在线升级app版all
int  test3_2_all(std::list<string>& arr)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;
	char* sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	char ip[20];
	//printf("输入ip\n");
	//gets_s(ip, 20);
	char port[10];
	//printf("输入port\n");
	//gets_s(port, 10);
	int isOta = 0;

	//int count = arr.size();
	memset(ip, 0, 20);
	memset(port, 0, 10);

	char function = 0;
	printf("输入功能码：\n");
	printf("0————下载脚本\n");
	printf("1————下载固件\n");
	printf("A————更新BT\n");
	printf("其他————退出\n");
	int ret = getchar();
	if (ret == '0')
		function = 0;
	else if (ret == '1')
		function = 0x20;
	else if (ret == 'A')
		function = 0xA0;
	else
		exit(0);

	ret = getchar();//接收回车

	//getchar();
	// 获取文件
	char path[MAX_PATH];
	printf("输入文件目录\n");
	gets_s(path, MAX_PATH);

	wchar_t* wpath = CharToWchar(path);


	/////////////////////////////////////////////////////////////////////
	HANDLE hFile = ::CreateFile(wpath,     //创建文件的名称。
		GENERIC_READ,          // 写和读文件。
		FILE_SHARE_READ,                      // 不共享读写。
		NULL,                   // 缺省安全属性。
		OPEN_EXISTING,          // 如果文件存在，也创建。
		FILE_ATTRIBUTE_NORMAL, // 一般的文件。       
		NULL);                 // 模板文件为空。
	if (hFile == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(TEXT("CreateFile fail!\r\n"));
	}

	//////////
	//发送命令
	//////////


	LARGE_INTEGER size = { 0 };
	if (!::GetFileSizeEx(hFile, &size))
	{
		OutputDebugString(TEXT("获取文件失败\r\n"));;
	}

	printf("File(%s) Size: %ld\n", wpath, size);

	//size.QuadPart = size.QuadPart + 2;
	delete wpath;
	Sleep(500);//getchar();
	  //crc
	//UCHAR uchBuf[20] = { 0xF3, 0x06, 0x6C, 0x81, 0x03, 0x00 };

	UCHAR uchBuf[20] = { 0xF1, 0x10, 0x00, 0x00, 0x01, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uchBuf[7] = function;
	memcpy_s(uchBuf + 8, sizeof(uchBuf), &size, 4);
	uint16_t crc;
	crc = getCRC16(uchBuf, 16);
	memcpy_s(uchBuf + 16, sizeof(uchBuf), &crc, 2);
	std::list<string>::iterator it = arr.begin();

test3_3next:
	LONG lDistance = 0;
	DWORD dwPtr = SetFilePointer(hFile, lDistance, NULL, FILE_BEGIN);

	if (it != arr.end())
	{
		char* buf = (char*)it->c_str();
		char* idPos = strstr(buf, "ID:") + 3;
		char* ipPos = strstr(buf, "IP:") + 3;
		char* portPos = strstr(buf, "PORT:") + 5;


		if (0 == strncmp(buf, "yes", 3))
		{

			int idLen = strstr(idPos, "\r\n") - idPos;
			int ipLen = strstr(ipPos, "\r\n") - ipPos;
			int portLen = strstr(portPos, "\r\n") - portPos;


			memcpy(ip, ipPos, ipLen);
			memcpy(port, portPos, portLen);
			goto test3_3connect;
		}
	}

	CloseHandle(hFile);
	return 0;

test3_3connect:
	// Validate the parameters
	/*if (argc != 2) {
	printf("usage: %s server-name\n", argv[0]);
	return 1;
	}*/
	

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(ip, port, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			//WSACleanup();
			goto test3_2allend;
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server %s : %s!\n", ip, port);
		//WSACleanup();
		goto test3_2allend;
		return 1;
	}
	printf("Successed to connect to server %s : %s!\n", ip, port);
	

	// Send an initial buffer
	iResult = send(ConnectSocket, (char*)uchBuf, 18, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		//closesocket(ConnectSocket);
		//WSACleanup();
		goto test3_2allend;
		return 1;
	}
	// 等待接收初始化完成
	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0) {
		//recvbuf[iResult] = 0x00;
		for (int i = 0; i < iResult; i++) {
			printf("%02x ", (UINT8)recvbuf[i]);
		}
		printf("\n");
	}
	else {
		if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

		goto test3_2allend;
	}


	Sleep(300); //getchar();
	  //////////
	  //发送脚本
	  //////////
	char chBuffer[4096];
	crc = 0xFFFF;
	DWORD dwReadSize = 0;
/*	LONG*/ lDistance = 0;

	::ZeroMemory(chBuffer, 4096);
	//while (1)// 计算crc
	//{
	//	BOOL bRet = ::ReadFile(hFile, chBuffer, 0x1000, &dwReadSize, NULL);
	//	if (bRet && dwReadSize)
	//	{
	//		//OutputDebugString(TEXT("ReadFile 读文件成功\r\n"));
	//	}
	//	else
	//	{
	//		//获取出错码。
	//		DWORD dwError = GetLastError();
	//		//处理出错。            
	//		TCHAR chErrorBuf[1024];
	//		wsprintf(chErrorBuf, TEXT("GetLastError()=%d\r\n"), dwError);
	//		OutputDebugString(chErrorBuf);
	//		break;
	//	}

	//	for (int i = 0; i < dwReadSize; i++) {
	//		crc = oneCRC16(chBuffer[i], crc);
	//	}
	//	lDistance += dwReadSize;
	//	DWORD dwPtr = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
	//	if (dwPtr == INVALID_SET_FILE_POINTER &&
	//		GetLastError() != NO_ERROR) // Test for failure
	//	{
	//		// Obtain the error code. 
	//		DWORD dwError = GetLastError();

	//		// Deal with failure 
	//		// . . . 

	//	} // End of error handler 
	//}
	lDistance = 0;
	while (1)// 发送
	{
		BOOL bRet = ::ReadFile(hFile, chBuffer, 0x1000, &dwReadSize, NULL);
		if (bRet && dwReadSize)
		{
			//OutputDebugString(TEXT("ReadFile 读文件成功\r\n"));
		}
		else
		{
			//获取出错码。
			DWORD dwError = GetLastError();
			//处理出错。            
			TCHAR chErrorBuf[1024];
			wsprintf(chErrorBuf, TEXT("GetLastError()=%d\r\n"), dwError);
			OutputDebugString(chErrorBuf);
			break;
		}
		for (int i = 0; i < dwReadSize; i++) {
			crc = oneCRC16(chBuffer[i], crc);
		}
		lDistance += dwReadSize;
		DWORD dwPtr = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
		if (dwPtr == INVALID_SET_FILE_POINTER &&
			GetLastError() != NO_ERROR) // Test for failure
		{
			// Obtain the error code. 
			DWORD dwError = GetLastError();

			// Deal with failure 
			// . . . 

		} // End of error handler 

		Sleep(10);
		if (bRet)
		{
			//OutputDebugString(TEXT("ReadFile 读文件成功\r\n"));
			// Send an initial buffer
			iResult = send(ConnectSocket, chBuffer, dwReadSize, 0);
			if (iResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				//closesocket(ConnectSocket);
				//WSACleanup();
				printf("Bytes Sent: %ld\n", lDistance);

				goto test3_2allend;
				return 1;
			}
		}
	}
	Sleep(10);
	// Send crc
	iResult = send(ConnectSocket, (char*)&crc, 2, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		/*closesocket(ConnectSocket);
		WSACleanup();*/
		printf("Bytes Sent: %ld\n", lDistance);

		goto test3_2allend;

		return 1;
	}
	lDistance += 2;

	printf("CRC: %x\n", (UINT16)crc);
	////////////////////////////////////////////////

	printf("Bytes Sent: %ld\n", lDistance);

	// shutdown the connection since no more data will be sent
	//iResult = shutdown(ConnectSocket, SD_SEND);
	//if (iResult == SOCKET_ERROR) {
	//	printf("shutdown failed with error: %d\n", WSAGetLastError());
	//	closesocket(ConnectSocket);
	//	WSACleanup();
	//	return 1;
	//}

	// Receive until the peer closes the connection
	//do {

	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0) {
		/*recvbuf[iResult] = 0x00;
		printf("Bytes received: %s\n", recvbuf);*/
		for (int i = 0; i < iResult; i++) {
			printf("%02x ", (UINT8)recvbuf[i]);
		}
		printf("\n");
	}
	else
	{
		if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

		goto test3_2allend;
	}

	//} while (iResult > 0);

	test3_2allend:
	printf("OVER\r\n");
	//getchar();
	//// cleanup
	//iResult = send(ConnectSocket, "CDV RESET", 9, 0);
	//if (iResult == SOCKET_ERROR) {
	//	printf("send failed with error: %d\n", WSAGetLastError());
	//	closesocket(ConnectSocket);
	//	WSACleanup();
	//	return 1;
	//}

	closesocket(ConnectSocket);
	WSACleanup();

	it++; 
	goto test3_3next;

	printf("继续升级？ y/n \n");
	ret = getchar();
	if (ret == 'y') {
		getchar();
		goto test3_3next;
	}
	return 0;
}

//测试脚本接收
int  test4(void)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;
	char* sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;



	// Validate the parameters
	/*if (argc != 2) {
	printf("usage: %s server-name\n", argv[0]);
	return 1;
	}*/

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo("192.168.1.31", DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}
	/////////////////////////////////////////////////////////////////////


	//////////
	//发送命令
	//////////


	/*LARGE_INTEGER size;
	if (!::GetFileSizeEx(hFile, &size))
	{
		OutputDebugString(TEXT("获取文件失败\r\n"));;
	}*/

	//crc
	UCHAR uchBuf[20] = { 0x01, 0x10, 0x00, 0x00, 0x01, 0x41, 0x00, 0x01, };
	uint16_t crc;
	crc = getCRC16(uchBuf, 8);
	memcpy_s(uchBuf + 8, sizeof(uchBuf), &crc, sizeof(uint16_t));

	// Send an initial buffer
	iResult = send(ConnectSocket, (char*)uchBuf, 14, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0)
		printf("Bytes received: %d\n", iResult);
	else if (iResult == 0)
		printf("Connection closed\n");
	else
		printf("recv failed with error: %d\n", WSAGetLastError());
	//////////
	//发送脚本
	//////////
	int32_t size = *(int32_t*)(recvbuf + 4) + 50;

	char* rbuf = new char[size];
	crc = 0xFFFF;
	DWORD dwReadSize = 0;
	::ZeroMemory(rbuf, size);
	LONG rlen = 0;


	// Receive until the peer closes the connection
	int timeout = 3000; //3s
	int ret = setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

	do {
		iResult = recv(ConnectSocket, rbuf + rlen, size - rlen, 0);
		if (iResult > 0)
		{
			printf("Bytes received: %d\n", iResult);
			rlen += iResult;
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);
	printf("Bytes received: %d\n", rlen);
	printf("%s", rbuf);

	for (int i = 0; i < rlen; i++)
	{
		printf_s("%02x ", (unsigned char)rbuf[i]);
	}
	////////////////////////////////////////////////

	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// Receive until the peer closes the connection
	do {

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);

	// cleanup
	delete[] rbuf;
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}

//规
int  test5(void)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;
	char* sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;



	// Validate the parameters
	/*if (argc != 2) {
	printf("usage: %s server-name\n", argv[0]);
	return 1;
	}*/

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo("192.168.1.31", DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}
	/////////////////////////////////////////////////////////////////////


	//////////
	//发送命令
	//////////


	/*LARGE_INTEGER size;
	if (!::GetFileSizeEx(hFile, &size))
	{
	OutputDebugString(TEXT("获取文件失败\r\n"));;
	}*/

	//crc
	UCHAR uchBuf[20] = { 0x01, 0x10, 0x00, 0x00, 0x01, 0x41, 0x00, 0x01, };
	uint16_t crc;
	crc = getCRC16(uchBuf, 8);
	memcpy_s(uchBuf + 8, sizeof(uchBuf), &crc, sizeof(uint16_t));

	// Send an initial buffer
	iResult = send(ConnectSocket, (char*)uchBuf, 14, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0)
		printf("Bytes received: %d\n", iResult);
	else if (iResult == 0)
		printf("Connection closed\n");
	else
		printf("recv failed with error: %d\n", WSAGetLastError());
	//////////
	//发送脚本
	//////////
	int32_t size = *(int32_t*)(recvbuf + 4) + 50;

	char* rbuf = new char[size];
	crc = 0xFFFF;
	DWORD dwReadSize = 0;
	::ZeroMemory(rbuf, size);
	LONG rlen = 0;


	// Receive until the peer closes the connection
	int timeout = 3000; //3s
	int ret = setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

	do {
		iResult = recv(ConnectSocket, rbuf + rlen, size - rlen, 0);
		if (iResult > 0)
		{
			printf("Bytes received: %d\n", iResult);
			rlen += iResult;
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);
	printf("Bytes received: %d\n", rlen);
	printf("%s", rbuf);

	for (int i = 0; i < rlen; i++)
	{
		printf_s("%02x ", (unsigned char)rbuf[i]);
	}
	////////////////////////////////////////////////

	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// Receive until the peer closes the connection
	do {

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);

	// cleanup
	delete[] rbuf;
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}

//接收广播和回复
int  test_udp(void)
{
	SOCKET ConnectSocket = INVALID_SOCKET;
	WSADATA wsaData;
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		return -1;
	}

	ConnectSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (ConnectSocket == INVALID_SOCKET) {
		WSACleanup();
		return -1;
	}

	const int opt = -1;
	//int nb = 0;
	//nb = setsockopt(ConnectSocket, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));
	//if (nb == -1)
	//{
	//  return -1;
	//}


	if (ConnectSocket == INVALID_SOCKET) {
		WSACleanup();
		return -1;
	}

	/////////////////////////////////////////////////////////
   // char * sbuf, int slen;
	int ret = -1;
	char rbuf[200] = { 0 };
	int rlen = 200;

	if (ConnectSocket == INVALID_SOCKET)
		return -1;

	sockaddr_in senderAddr, destAddr, sourAddr;
	/*设置源地址为本地广播地址*/
	//bzero(&sourAddr, sizeof(struct sockaddr_in));
	//sourAddr.sin_family = AF_INET;
	//sourAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	//sourAddr.sin_port = htons(10);

	destAddr.sin_family = AF_INET;
	destAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	destAddr.sin_port = htons(10);

	iResult = bind(ConnectSocket, (SOCKADDR*)&destAddr, sizeof(destAddr));
	if (iResult != 0) {
		wprintf(L"bind failed with error %d\n", WSAGetLastError());
		return 1;
	}

	int addrSize = sizeof(sourAddr);

	//iResult = sendto(ConnectSocket, sbuf, slen, 0, (SOCKADDR *)& sourAddr, sizeof(sourAddr));
	//if (iResult == SOCKET_ERROR) {
	//  return -1;
	//}

   // int timeout = 1000;
   // ret = setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
	printf("start recv\n");
	do {

		iResult = recvfrom(ConnectSocket, rbuf, rlen, 0, (SOCKADDR*)&sourAddr, &addrSize);

		int error = WSAGetLastError();
		if (iResult > 0)
		{
			printf("%s\n", rbuf);
			//rarray.Add(rbuf);

			int iResult1 = sendto(ConnectSocket, rbuf, iResult, 0, (SOCKADDR*)&sourAddr, sizeof(sourAddr));
			Sleep(2);//测试两次连续发送，终端是否正常接收
			iResult1 = sendto(ConnectSocket, rbuf, iResult, 0, (SOCKADDR*)&sourAddr, sizeof(sourAddr));
			if (iResult1 == SOCKET_ERROR) {
				return -1;
			}
		}
		else if (WSAETIMEDOUT == error)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	} while (true);

	////////////////////////////////////////////////////////
	iResult = closesocket(ConnectSocket);
	if (iResult == SOCKET_ERROR) {
		WSACleanup();
		return -1;
	}
	// Clean up and quit.
	WSACleanup();

}

//发送广播
int  test_udp2(void)
{
	SOCKET ConnectSocket = INVALID_SOCKET;
	WSADATA wsaData;
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		return -1;
	}

	ConnectSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (ConnectSocket == INVALID_SOCKET) {
		WSACleanup();
		return -1;
	}

	const int opt = -1;
	//int nb = 0;
	//nb = setsockopt(ConnectSocket, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));
	//if (nb == -1)
	//{
	//  return -1;
	//}

	int nb = 0;
	nb = setsockopt(ConnectSocket, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));
	if (nb == -1)
	{
		return -1;
	}

	if (ConnectSocket == INVALID_SOCKET) {
		WSACleanup();
		return -1;
	}

	/////////////////////////////////////////////////////////
   // char * sbuf, int slen;
	int ret = -1;
	char rbuf[200] = { 0 };
	int rlen = 200;

	sockaddr_in senderAddr, destAddr, sourAddr;
	/*设置源地址为本地广播地址*/
	sourAddr.sin_family = AF_INET;
	sourAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	sourAddr.sin_port = htons(10);

	//destAddr.sin_family = AF_INET;
	//destAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//destAddr.sin_port = htons(10);

	//iResult = bind(ConnectSocket, (SOCKADDR *)& destAddr, sizeof(destAddr));
	//if (iResult != 0) {
	//  wprintf(L"bind failed with error %d\n", WSAGetLastError());
	//  return 1;
	//}
	

	int addrSize = sizeof(sourAddr);

	iResult = sendto(ConnectSocket, "are you npc", 11, 0, (SOCKADDR*)&sourAddr, sizeof(sourAddr));
	if (iResult == SOCKET_ERROR) {
		return -1;
	}

	// int timeout = 1000;
	// ret = setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
	printf("start recv\n");
	int timeout = 500;
	ret = setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
	do {
		memset(rbuf, 0, 200);
		iResult = recvfrom(ConnectSocket, rbuf, rlen, 0, (SOCKADDR*)&destAddr, &addrSize);

		int error = WSAGetLastError();
		if (iResult > 0)
		{
			printf(rbuf);
		}
		else if (WSAETIMEDOUT == error)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	} while (true);

	////////////////////////////////////////////////////////

}


//发送广播
int  test_udp3(std::list<string> &arr)
{
	SOCKET ConnectSocket = INVALID_SOCKET;
	WSADATA wsaData;
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		return -1;
	}

	ConnectSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (ConnectSocket == INVALID_SOCKET) {
		WSACleanup();
		return -1;
	}

	const int opt = -1;
	//int nb = 0;
	//nb = setsockopt(ConnectSocket, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));
	//if (nb == -1)
	//{
	//  return -1;
	//}

	int nb = 0;
	nb = setsockopt(ConnectSocket, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));
	if (nb == -1)
	{
		return -1;
	}

	if (ConnectSocket == INVALID_SOCKET) {
		WSACleanup();
		return -1;
	}

	/////////////////////////////////////////////////////////
   // char * sbuf, int slen;
	int ret = -1;
	char rbuf[200] = { 0 };
	int rlen = 200;

	sockaddr_in senderAddr, destAddr, sourAddr;
	/*设置源地址为本地广播地址*/
	sourAddr.sin_family = AF_INET;
	sourAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	sourAddr.sin_port = htons(10);

	//destAddr.sin_family = AF_INET;
	//destAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//destAddr.sin_port = htons(10);

	//iResult = bind(ConnectSocket, (SOCKADDR *)& destAddr, sizeof(destAddr));
	//if (iResult != 0) {
	//  wprintf(L"bind failed with error %d\n", WSAGetLastError());
	//  return 1;
	//}


	int addrSize = sizeof(sourAddr);

	iResult = sendto(ConnectSocket, "are you npc", 11, 0, (SOCKADDR*)&sourAddr, sizeof(sourAddr));
	if (iResult == SOCKET_ERROR) {
		return -1;
	}

	// int timeout = 1000;
	// ret = setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
	printf("start recv\n");
	int timeout = 500;
	ret = setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
	do {
		memset(rbuf, 0, 200);
		iResult = recvfrom(ConnectSocket, rbuf, rlen, 0, (SOCKADDR*)&destAddr, &addrSize);

		int error = WSAGetLastError();
		if (iResult > 0)
		{
			arr.push_back(rbuf);
			char* tmp_buf = rbuf;

			do {
				tmp_buf = strstr(tmp_buf, "\r\n");
				if(tmp_buf)
				strncpy(tmp_buf, "  ", 2);
			} while (tmp_buf);

			printf(rbuf);
			printf("\r\n");
		}
		else if (WSAETIMEDOUT == error)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	} while (true);

	////////////////////////////////////////////////////////

}


#include "Win32Socket.h"
//发送广播，多网卡
int  test_udp4(std::list<string>& arr)
{
	
	UDPSender sender1;
	if (sender1.SetRecvAddr("192.168.1.255", 10) == true)
	{
		cout << "BroadCasting 192.168.23 ...";
		sender1.Send("are you npc", sizeof("are you npc"));
	}
	else
		return -1;

	// int timeout = 1000;
	// ret = setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
	printf("start recv\n");
	int ret = -1;
	char rbuf[200] = { 0 };
	int rlen = 200;
	int timeout = 500;
	int iResult;
	sockaddr_in senderAddr, destAddr, sourAddr;
	int addrSize = sizeof(sourAddr);
	ret = setsockopt(sender1.m_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
	do {
		memset(rbuf, 0, 200);
		iResult = recvfrom(sender1.m_sock, rbuf, rlen, 0, (SOCKADDR*)&destAddr, &addrSize);

		int error = WSAGetLastError();
		if (iResult > 0)
		{
			arr.push_back(rbuf);
			char* tmp_buf = rbuf;

			do {
				tmp_buf = strstr(tmp_buf, "\r\n");
				if (tmp_buf)
					strncpy(tmp_buf, "  ", 2);
			} while (tmp_buf);

			printf(rbuf);
			printf("\r\n");
		}
		else if (WSAETIMEDOUT == error)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	} while (true);

	////////////////////////////////////////////////////////

}


//模拟集中控制
union BINHEX {
	struct
	{
		unsigned char low : 4;
		unsigned char high : 4;
	}hex;
	unsigned char ch;
};

char* Bin2Hex(char* bin, int &len)
{
	union BINHEX *hexs;
	int bin_len = strlen(bin);
	char *ptr = bin;
	int cnt = 0;
	char ref = 0;

	hexs = new BINHEX[bin_len+2];

	do
	{
		if (!(*ptr >= '0' && *ptr <= '9' || *ptr >= 'A' && *ptr <= 'F' || *ptr >= 'a' && *ptr <= 'f'))
			continue;

		if (*ptr >= '0' && *ptr <= '9')
			ref = '0';
		else if (*ptr >= 'A' && *ptr <= 'F')
			ref = 'A' - 10;
		else if (*ptr >= 'a' && *ptr <= 'f')
			ref = 'a' - 10;

		switch (cnt & 1)
		{
		case 0:
			hexs[cnt/2].hex.high = *ptr - ref;
			break;
		case 1:
			hexs[cnt/2].hex.low = *ptr - ref;
			break;
		default:
			break;
		}
		cnt++;
	} while (*ptr++ != '\0');

	if (cnt & 1)
	{
		len = 0;
		return NULL;
	}
	else
	{
		len = cnt/2;
		return (char*)hexs;
	}
}

//在线升级app版3.2.3.23开始，模拟集中控制在线查询，循环
int  test_net(void)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;
	char* sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;


	// 获取文件
	char str[MAX_PATH];
	printf("输入字符串\n");
	gets_s(str, MAX_PATH);
	int len;
	char* hex = Bin2Hex(str, len);
	//wchar_t* wpath = CharToWchar(path);
	if (!hex || !len)
		return -1;

	uint16_t crc;
	crc = getCRC16((UINT8*)hex, len);
	memcpy_s(hex + len, 2, &crc, 2);

	// 准备ip
	char ip[20];
	printf("输入ip\n");
	gets_s(ip, 20);
	char port[10];
	printf("输入port\n");
	gets_s(port, 10);
	int isOta = 0;
	printf("输入超时时间ms\n");
	int timeout;
	scanf("%d", &timeout);
	printf("输入间隔时间ms\n");
	int timewait;
	scanf("%d", &timewait);
connect:
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(ip, port, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);

		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server %s : %s!\n", ip, port);
		WSACleanup();
		return 1;
	}
	printf("Successed to connect to server %s : %s!\n", ip, port);

	// ready to send
	//发送时限
	//struct timeval timeout;
	//timeout.tv_sec = 3;
	//timeout.tv_usec = 0;

	//socklen_t len = sizeof(timeout);

	//int timeout = 2000;
	int ret = setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(int));

	// Send an initial buffer
	iResult = send(ConnectSocket, hex,len+2, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	// 等待接收初始化完成
	while (1)
	{
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			for (int i = 0; i < iResult; i++)
			{
				printf("%02x ", (UINT8)recvbuf[i]);
			}
			printf("\n");
		}
		else if (iResult == 0)
		{
			printf("Connection closed\n");
			break;
		}
		else
		{
			//printf("recv failed with error: %d\n", WSAGetLastError());
			break;
		}
	}

	closesocket(ConnectSocket);
	WSACleanup();
	Sleep(timewait);
	goto connect;
	
	return 0;
}


//在线升级管理
void update(void)
{
	test_udp2();
	printf("请确认npc列表中有升级的npc y/n \n");
	int ret = getchar();
	if (ret != 'y')
		return;
	ret = getchar();//接收回车
	test3_2();// test3();
}

//在线升级管理
void update_all(void)
{
	std::list<string> arr;
	//test_udp3(arr);
	test_udp4(arr);
	printf("请确认npc列表中的都需要升级 y/n \n");
	int ret = getchar();
	if (ret != 'y')
		return;
	ret = getchar();//接收回车
	test3_2_all(arr);//test3_all(arr);//  test3();
}
#include "exp.h"
//#include "vcwnd.h"

int main()
{
	////多网卡
	//UDPSender sender1, sender2;
	//if (sender1.SetRecvAddr("192.168.1.255", 9999) == true)
	//{
	//	cout << "BroadCasting 192.168.23 ...";
	//	sender1.Send("are you npc", sizeof("are you npc"));
	//}
	///*if (sender2.SetRecvAddr("10.20.30.255", 9998) == true)
	//{
	//	cout << "BroadCasting 10.20.30 ...";
	//		sender2.Send("hello", sizeof("hello"));
	//}*/
	//return 0;
	//test_net();
	//test3();
	update_all();

	printf("over");
	getchar();
	return 1;
}
#undef main
#include "WIN32WND.h"

