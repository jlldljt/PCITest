
#include "serial.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define DEFAULT_BUFLEN 1024

//��������app��3.2.3.23��ʼ
void update_with_serial(void)
{
	char sendbuf[] = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	serialasy serial;

test3connect:
	// Validate the parameters
	/*if (argc != 2) {
	printf("usage: %s server-name\n", argv[0]);
	return 1;
	}*/
	char com[20];
	printf("���봮�ڣ����硰COM1��\n");
	gets_s(com, 20);
	int isOta = 0;

	// Initialize serial
	if (serial.OpenSerial(com, 115200))
		exit(0);

	printf("Successed to connect to %s!\n", com);

	char function = 0;
	printf("���빦���룺\n");
	printf("0�����������ؽű�\n");
	printf("1�����������ع̼�\n");
	printf("A������������BT\n");
	printf("C������������COP\n");
	printf("�������������˳�\n");
	int ret = getchar();
	if (ret == '0')
		function = 0;
	else if (ret == '1')
		function = 0x20;
	else if (ret == 'A')
		function = 0xA0;
	else if (ret == 'C')
		function = 0xC0;
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
	iResult = serial.Write((char*)uchBuf, 18);

	if (iResult != 18)
	{
		printf("send failed\n");
		return;
	}

	// �ȴ����ճ�ʼ�����
	iResult = serial.Read(recvbuf, recvbuflen);

	if (iResult > 0) {
		//recvbuf[iResult] = 0x00;
		for (int i = 0; i < iResult; i++) {
			printf("%02x ", (UINT8)recvbuf[i]);
		}
		printf("\n");
	}
	else if (iResult == 0)
		printf("recv nothing\n");
	else
		printf("recv failed\n");

	Sleep(300); //getchar();
	  //////////
	  //���ͽű�
	  //////////
	char chBuffer[4096];
	crc = 0xFFFF;
	DWORD dwReadSize = 0;
	LONG lDistance = 0;

	::ZeroMemory(chBuffer, 4096);
	
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
			// Send
			iResult = serial.Write(chBuffer, dwReadSize);

			if (iResult != dwReadSize)
			{
				printf("send failed\n");
				printf("Bytes Sent: %ld\n", lDistance);
				return;
			}
		}
	}
	Sleep(10);
	// Send crc
	// Send
	iResult = serial.Write((char*)&crc, 2);

	if (iResult != 2)
	{
		printf("send crc failed\n");
		printf("Bytes Sent: %ld\n", lDistance);
		return;
	}

	lDistance += 2;

	printf("CRC: %x\n", (UINT16)crc);
	printf("Bytes Sent: %ld\n", lDistance);

	////////////////////////////////////////////////

	iResult = serial.Read(recvbuf, recvbuflen);

	if (iResult > 0) {
		//recvbuf[iResult] = 0x00;
		for (int i = 0; i < iResult; i++) {
			printf("%02x ", (UINT8)recvbuf[i]);
		}
		printf("\n");
	}
	else if (iResult == 0)
		printf("recv nothing\n");
	else
		printf("recv failed\n");

	printf("OVER");
	getchar();

	CloseHandle(hFile);
	serial.CloseSerial();

	printf("���������� y/n \n");
	ret = getchar();
	if (ret == 'y') {
		getchar();
		goto test3connect;
	}
	return;
}


void NPC_INF(void)
{
	char sendbuf[] = "NPC INF";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	serialasy serial;

	char com[20];
	printf("���봮�ڣ����硰COM1��\n");
	gets_s(com, 20);
	int isOta = 0;

	// Initialize serial
	if (serial.OpenSerial(com, 115200))
		exit(0);

	printf("Successed to connect to %s!\n", com);

	//int ret = getchar();

	// Send an initial buffer
	iResult = serial.Write(sendbuf, strlen(sendbuf));

	if (iResult != strlen(sendbuf))
	{
		printf("send failed\n");
		return;
	}
	printf("send NPC INF\n");
	// �ȴ����ճ�ʼ�����
	iResult = serial.Read(recvbuf, recvbuflen);

	if (iResult > 0) {
		//recvbuf[iResult] = 0x00;
		//for (int i = 0; i < iResult; i++) {
		//	printf("%02x ", (UINT8)recvbuf[i]);
		//}
		printf(recvbuf);
		printf("\n");
	}
	else if (iResult == 0)
		printf("recv nothing\n");
	else
		printf("recv failed\n");

}

int main()
{
	//NPC_INF();// update_with_serial();
	update_with_serial();
	printf("over");
	getchar();
	return 1;
}