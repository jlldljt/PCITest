#include "serial.h"
#include <process.h>
#include <stdio.h>

// global function


serialsyn::serialsyn()
{
	_hser = INVALID_HANDLE_VALUE;
	_error = NO_ERROR;
	_dcb = { 0 };
	_timeout = { 0 };
	_stat = { 0 };
}

serialsyn::~serialsyn()
{
	CloseSerial();
}


int serialsyn::OpenSerial(char* serial, DWORD baud_rate, BYTE byte_size, BYTE parity, BYTE stop_bits)
{
	char path[32] = { 0 };
	wchar_t* wpath = NULL;

	if (strlen(serial) > 4) // com10����
	{
		strcpy_s(path, "\\\\.\\");
		strcat_s(path, serial);
		wpath = CharToWchar(path);
	}
	else
	{
		wpath = CharToWchar(serial);
	}

	_hser = CreateFile(wpath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	delete wpath;

	if (_hser == INVALID_HANDLE_VALUE)
	{
		_error = GetLastError();
		if (_error == 5)
			printf("û�з��ִ��ڻ򴮿��ѱ�ռ��");
		return -1;

	}
	else
	{
		SetupComm(_hser, 4096, 4096);//���仺����

		GetCommState(_hser, &_dcb);//��ȡDCB�ṹ
		//_dcb.DCBlength = sizeof(_dcb);
		_dcb.BaudRate = baud_rate;//������Ϊ9600
		_dcb.ByteSize = byte_size;//����λΪ8λ
		_dcb.fParity = 1;//��żУ��
		_dcb.Parity = parity;//ODDPARITY;
		_dcb.StopBits = stop_bits;//һλֹͣλ
		SetCommState(_hser, &_dcb);

		_timeout.ReadIntervalTimeout = 100;
		_timeout.ReadTotalTimeoutConstant = 0;
		_timeout.ReadTotalTimeoutMultiplier = 0;
		_timeout.WriteTotalTimeoutMultiplier = 500;
		_timeout.WriteTotalTimeoutConstant = 1000;
		SetCommTimeouts(_hser, &_timeout);
		PurgeComm(_hser, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		return 0;

	}

	return -1;
}

void serialsyn::CloseSerial(void)
{
	if (_hser != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_hser);
	}
}

int serialsyn::Write(char* buf, int buf_len)
{
	DWORD write_len = 0;//���ͳ�ȥ���ַ�����
	ClearCommError(_hser, &_error, &_stat);
	WriteFile(_hser, buf, buf_len, &write_len, NULL);//����
	_error = GetLastError();

	if (write_len != buf_len)//���ͳ���С��Ҫ���͵ĳ���
		return -1;

	return buf_len;
}
/* return the len read */
int serialsyn::Read(char* buf, int buf_len)
{
	DWORD exist_len = 0;
	DWORD read_len = 0;
	BOOL flag = 0;
	char* rbuf = buf;
	memset(rbuf, 0, buf_len);
	// if (recvLenTmp > 98)return 3;
	ClearCommError(_hser, &_error, &_stat);
	exist_len = _stat.cbInQue;
	{
		//if (exist_len > buf_len)
		//	exist_len = buf_len;
		flag = ReadFile(_hser, rbuf, buf_len, &read_len, NULL);//��ȡ�˿�ֵ
	}

	if (!flag)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			_error = GetLastError();
			if (_error == ERROR_IO_PENDING)
				return -1;
		}
	}
	else
	{
		return read_len;
	}

	return 0;
}


/********************************************************************/
//unsigned int __stdcall serialasy::RecvThread(void* LPParam)// ���ReadIntervalTimeout = 2
//{
//	serialasy* obj = static_cast<serialasy*>(LPParam);
//
//	DWORD Bytes = 0;
//	BOOL Status = FALSE;
//	BYTE ReadBuf[4096];
//	DWORD Error;
//	COMSTAT cs = { 0 };
//
//	while (obj->_hser != INVALID_HANDLE_VALUE)
//	{
//
//		ClearCommError(obj->_hser, &Error, &cs);
//
//		Bytes = 0;
//		obj->_ov_read.Offset = 0;
//		memset(ReadBuf, 0, sizeof(ReadBuf));
//		Status = ReadFile(obj->_hser, ReadBuf, sizeof(ReadBuf), &Bytes, &obj->_ov_read);
//		//�����Ѿ����ﻺ��������ȡ���������أ�������True�� ���򷵻�False
//		if (Status == FALSE && GetLastError() == ERROR_IO_PENDING)
//		{
//			//��������ݵ��� �� ʱ�䳬��ReadIntervalTimeout��᷵�أ������һֱ�ȴ�
//			Status = GetOverlappedResult(obj->_hser, &obj->_ov_read, &Bytes, TRUE);
//		}
//		if (FALSE != Status && Bytes > 0)
//		{
//			//cout << "Read:" << (LPCSTR)ReadBuf << "   Len:" << Bytes << endl;
//		}
//		PurgeComm(obj->_hser, PURGE_RXCLEAR | PURGE_RXABORT);
//
//	}
//
//	return 0;
//}
//
//unsigned int __stdcall serialasy::RecvThread(void* param) // ���_timeout.ReadIntervalTimeout = MAXDWORD
//{
//	serialasy* obj = static_cast<serialasy*>(param);
//
//	DWORD WaitEvent = 0, Bytes = 0;
//	BOOL Status = FALSE;
//	BYTE ReadBuf[4096];
//	DWORD Error;
//	COMSTAT cs = { 0 };
//
//	while (obj->_hser != INVALID_HANDLE_VALUE)
//	{
//		WaitEvent = 0;
//		obj->_ov_wait.Offset = 0;
//		Status = WaitCommEvent(obj->_hser, &WaitEvent, &obj->_ov_wait);
//		//WaitCommEventҲ��һ���첽���������Ҫ�ȴ�
//		if (FALSE == Status && GetLastError() == ERROR_IO_PENDING)//
//		{
//			//����������������̻߳�ͣ�ڴˣ����_hser�رջ���������False
//			Status = GetOverlappedResult(obj->_hser, &obj->_ov_wait, &Bytes, TRUE);
//		}
//		ClearCommError(obj->_hser, &Error, &cs);
//		if (TRUE == Status //�ȴ��¼��ɹ�
//			&& WaitEvent & EV_RXCHAR//�����������ݵ���
//			&& cs.cbInQue > 0)//������
//		{
//			Bytes = 0;
//			obj->_ov_read.Offset = 0;
//			memset(ReadBuf, 0, sizeof(ReadBuf));
//			//�����Ѿ����ﻺ������ReadFile���ᵱ���첽�������������ȡ������True
//			Status = ReadFile(obj->_hser, ReadBuf, sizeof(ReadBuf), &Bytes, &obj->_ov_read);
//			if (Status != FALSE)
//			{
//				//cout << "Read:" << (LPCSTR)ReadBuf << "   Len:" << Bytes << endl;
//			}
//			PurgeComm(obj->_hser, PURGE_RXCLEAR | PURGE_RXABORT);
//		}
//
//	}
//	return 0;
//}

serialasy::serialasy()
{
	//_hser = INVALID_HANDLE_VALUE;
	//_error = NO_ERROR;
	//_dcb = { 0 };
	//_timeout = { 0 };
	//_stat = { 0 };
	memset(&_ov_write, 0, sizeof(_ov_write));
	memset(&_ov_read, 0, sizeof(_ov_read));
	memset(&_ov_wait, 0, sizeof(_ov_wait));
	_thread = NULL;
}

serialasy::~serialasy()
{
	CloseSerial();
}
// 0 success -1 failure 
int serialasy::OpenSerial(char* serial, DWORD baud_rate, BYTE byte_size, BYTE parity, BYTE stop_bits)
{
	char path[32] = { 0 };
	wchar_t* wpath = NULL;

	if (strlen(serial) > 4) // com10����
	{
		strcpy_s(path, "\\\\.\\");
		strcat_s(path, serial);
		wpath = CharToWchar(path);
	}
	else
	{
		wpath = CharToWchar(serial);
	}

	_hser = CreateFile(wpath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_ATTRIBUTE_NORMAL, NULL);

	delete wpath;

	if (_hser == INVALID_HANDLE_VALUE)
	{
		_error = GetLastError();
		if (_error == 5)
			printf("û�з��ִ��ڻ򴮿��ѱ�ռ��");
		return -1;

	}
	else
	{
		SetupComm(_hser, 4096, 4096);//���仺����

		GetCommState(_hser, &_dcb);//��ȡDCB�ṹ
		//_dcb.DCBlength = sizeof(_dcb);
		_dcb.BaudRate = baud_rate;//������Ϊ9600
		_dcb.ByteSize = byte_size;//����λΪ8λ
		_dcb.fParity = 1;//��żУ��
		_dcb.Parity = parity;//ODDPARITY;
		_dcb.StopBits = stop_bits;//һλֹͣλ
		SetCommState(_hser, &_dcb);

		//��ȡ��ʱ��Ϊ2ms������ReadFile�첽��ȡʱ���᷵��FALSE������GetLastError() == ERROR_IO_PENDING��
		//�����ΪMAXDWORD��ReadFile�첽��ȡʱ���᷵��TRUE
		//Ȼ�����GetOverlappedResult�ȴ����ݵ����������ݵ�����ʱ�䳬��2ms��᷵�أ������һֱ�ȴ���
		/*
		���ַ�֮��������ʱ��
		����ȡ��������ʱ��һ�������ַ������ʱ������ʱ�䣬��ȡ�������������е����ݡ�
		����Ϊ0��ʾ�ò����������á�ָ��ʱ�����ֵ�����룩��������յ�2���ֽڼ���ʱ��
		Ҳ�� ��˵��
		�ս�����һ���ֽں󣬵���ReadIntervalTimeoutʱ���û���µ��ֽڵ���� ��Ϊ���ζ����ڲ���������������ֽڵ���һ�ζ�ȡ������������
		��ʹ�����8���ֽڣ�������2���ֽں󣬹���ReadIntervalTimeoutʱ��󣬵�3���ֽڻ�û����ʵ���Ͼ�ֻ����2���ֽڡ�
		*/
		_timeout.ReadIntervalTimeout = 100;
		//_timeout.ReadIntervalTimeout = MAXDWORD;//��ȡ����ʱ����Ϊ��WaitCommEvent�ȴ�����
		/*һ�ζ�ȡ�������ݵĹ̶���ʱ��
		������һ�ζ�ȡ���ڵĲ����У��䳬ʱΪReadTotalTimeoutMultiplier���Զ�ȡ���ֽ����ټ��� ReadTotalTimeoutConstant��
		��ReadIntervalTimeout����ΪMAXDWORD������ReadTotalTimeoutMultiplier ��ReadTotalTimeoutConstant����Ϊ0��
		��ʾ��ȡ�������������ش�������뻺�������ַ���
		�������Ϊһ������ʱ�䣬ʵ���Ͼ��ǰ�ReadTotalTimeoutMultiplier������ĳ�ʱʱ���ټ��ϸ�ʱ�����Ϊ������ʱʱ�䡣*/
		_timeout.ReadTotalTimeoutConstant = 0;  //
		_timeout.ReadTotalTimeoutMultiplier = 0;//

		_timeout.WriteTotalTimeoutMultiplier = 500;
		_timeout.WriteTotalTimeoutConstant = 5000;

		PurgeComm(_hser, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

		//�����¼�����
		_ov_write.hEvent = CreateEvent(NULL, false, false, NULL);
		_ov_read.hEvent = CreateEvent(NULL, false, false, NULL);
		_ov_wait.hEvent = CreateEvent(NULL, false, false, NULL);

		SetCommMask(_hser, EV_ERR | EV_RXCHAR);//���ý����¼�

		//������ȡ�߳�
		//_thread = (HANDLE)_beginthreadex(NULL, 0, &serialasy::RecvThread, this, 0, NULL);


		return 0;

	}

	return -1;
}

void serialasy::CloseSerial(void)
{
	//if (INVALID_HANDLE_VALUE != _hser)
	//{
	//	CloseHandle(_hser);
	//	_hser = INVALID_HANDLE_VALUE;
	//}
	if (NULL != _ov_write.hEvent)
	{
		CloseHandle(_ov_write.hEvent);
		_ov_write.hEvent = NULL;
	}
	if (NULL != _ov_read.hEvent)
	{
		CloseHandle(_ov_read.hEvent);
		_ov_read.hEvent = NULL;
	}
	if (NULL != _ov_wait.hEvent)
	{
		CloseHandle(_ov_wait.hEvent);
		_ov_wait.hEvent = NULL;
	}
	//if (NULL != _thread)
	//{
	//	WaitForSingleObject(_thread, 5000);//�ȴ��߳̽���
	//	CloseHandle(_thread);
	//	_thread = NULL;
	//}
}

int serialasy::Write(char* buf, int buf_len)
{
	BOOL ret = FALSE;
	DWORD write_len = 0;//���ͳ�ȥ���ַ�����
	ClearCommError(_hser, &_error, &_stat);
	ret = WriteFile(_hser, buf, buf_len, &write_len, &_ov_write);//����

	if (FALSE == ret && GetLastError() == ERROR_IO_PENDING)//��̨��ȡ
	{
		//�ȴ�����д�����
		if (FALSE == ::GetOverlappedResult(_hser, &_ov_write, &write_len, TRUE))
		{
			return -1;
		}
	}

	return write_len;
}
/* return the len read */
int serialasy::Read(char* buf, int buf_len)
{
	//serialasy* obj = static_cast<serialasy*>(LPParam);

	DWORD bytes = 0;
	BOOL flag = FALSE;

	if (_hser == INVALID_HANDLE_VALUE)
		return -1;

	ClearCommError(_hser, &_error, &_stat);

	bytes = 0;
	_ov_read.Offset = 0;
	memset(buf, 0, buf_len);
	flag = ReadFile(_hser, buf, buf_len, &bytes, &_ov_read);//��io
	//�����Ѿ����ﻺ��������ȡ���������أ�������True�� ���򷵻�False
	if (flag == FALSE && GetLastError() == ERROR_IO_PENDING)
	{
		//��������ݵ��� �� ʱ�䳬��ReadIntervalTimeout��᷵�أ������һֱ�ȴ�
		flag = GetOverlappedResult(_hser, &_ov_read, &bytes, TRUE);// �ȴ�io
	}
	if (FALSE != flag && bytes > 0)
	{
		//cout << "Read:" << (LPCSTR)ReadBuf << "   Len:" << Bytes << endl;
	}
	PurgeComm(_hser, PURGE_RXCLEAR | PURGE_RXABORT);



	return bytes;
}
