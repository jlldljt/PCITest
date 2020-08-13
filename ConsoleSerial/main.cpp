
#include "serial.h"


//在线升级app版3.2.3.23开始
void update_with_serial(void)
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



int main()
{
	printf("over");
	getchar();
	return 1;
}