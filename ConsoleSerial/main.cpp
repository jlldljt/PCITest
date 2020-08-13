
#include "serial.h"


//��������app��3.2.3.23��ʼ
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
	printf("����ip\n");
	gets_s(ip, 20);
	char port[10];
	printf("����port\n");
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
	printf("���빦���룺\n");
	printf("0�����������ؽű�\n");
	printf("1�����������ع̼�\n");
	printf("A������������BT\n");
	printf("�������������˳�\n");
	int ret = getchar();
	if (ret == '0')
		function = 0;
	else if (ret == '1')
		function = 0x20;
	else if (ret == 'A')
		function = 0xA0;
	else
		exit(0);

	ret = getchar();//���ջس�

	//getchar();
	// ��ȡ�ļ�
	char path[MAX_PATH];
	printf("�����ļ�Ŀ¼\n");
	gets_s(path, MAX_PATH);

	wchar_t* wpath = CharToWchar(path);


	/////////////////////////////////////////////////////////////////////
	HANDLE hFile = ::CreateFile(wpath,     //�����ļ������ơ�
		GENERIC_READ,          // д�Ͷ��ļ���
		FILE_SHARE_READ,                      // �������д��
		NULL,                   // ȱʡ��ȫ���ԡ�
		OPEN_EXISTING,          // ����ļ����ڣ�Ҳ������
		FILE_ATTRIBUTE_NORMAL, // һ����ļ���       
		NULL);                 // ģ���ļ�Ϊ�ա�
	if (hFile == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(TEXT("CreateFile fail!\r\n"));
	}

	//////////
	//��������
	//////////


	LARGE_INTEGER size = { 0 };
	if (!::GetFileSizeEx(hFile, &size))
	{
		OutputDebugString(TEXT("��ȡ�ļ�ʧ��\r\n"));;
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
	// �ȴ����ճ�ʼ�����
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
	  //���ͽű�
	  //////////
	char chBuffer[4096];
	crc = 0xFFFF;
	DWORD dwReadSize = 0;
	LONG lDistance = 0;

	::ZeroMemory(chBuffer, 4096);
	//while (1)// ����crc
	//{
	//	BOOL bRet = ::ReadFile(hFile, chBuffer, 0x1000, &dwReadSize, NULL);
	//	if (bRet && dwReadSize)
	//	{
	//		//OutputDebugString(TEXT("ReadFile ���ļ��ɹ�\r\n"));
	//	}
	//	else
	//	{
	//		//��ȡ�����롣
	//		DWORD dwError = GetLastError();
	//		//�������            
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
	while (1)// ����
	{
		BOOL bRet = ::ReadFile(hFile, chBuffer, 0x1000, &dwReadSize, NULL);
		if (bRet && dwReadSize)
		{
			//OutputDebugString(TEXT("ReadFile ���ļ��ɹ�\r\n"));
		}
		else
		{
			//��ȡ�����롣
			DWORD dwError = GetLastError();
			//�������            
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
			//OutputDebugString(TEXT("ReadFile ���ļ��ɹ�\r\n"));
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

	printf("���������� y/n \n");
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