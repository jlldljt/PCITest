#include "serial.h"

// global function
wchar_t* CharToWchar(const char* c, size_t m_encode = CP_ACP)
{
	int len = MultiByteToWideChar(m_encode, 0, c, strlen(c), NULL, 0);
	wchar_t* m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(m_encode, 0, c, strlen(c), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}

serial::serial()
{
	_hser = INVALID_HANDLE_VALUE;
	_error = NO_ERROR;
}

serial::~serial()
{
}

int serial::OpenSerial(char* serial)
{
	char path[32] = { 0 };
	wchar_t* wpath = NULL;

	if (strlen(serial) > 4) // com10以上
	{
		strcpy(path, "\\\\.\\");
		strcat(path, serial);
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
			printf("没有发现串口或串口已被占用");
		return false;

	}
	else
	{
		SetupComm(_hser, 2048, 2048);//分配缓冲区
		GetCommState(_hser, &_dcb);//读取DCB结构
		_dcb.BaudRate = 115200;//波特率为9600
		_dcb.ByteSize = 8;//数据位为8位
		_dcb.fParity = 1;//奇偶校验
		_dcb.Parity = NOPARITY;//ODDPARITY;
		_dcb.StopBits = 0;//一位停止位
		SetCommState(_hser, &_dcb);
		_timeout.ReadIntervalTimeout = 0;
		SetCommTimeouts(_hser, &_timeout);
		PurgeComm(_hser, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		return true;

	}

	return 0;
}

void serial::CloseSerial(void)
{
	if (_hser != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_hser);
	}
}

int serial::Write(char* buf, int buf_len)
{
	DWORD write_len = 0;//发送出去的字符长度
	ClearCommError(_hser, &_error, &_stat);
	WriteFile(_hser, buf, buf_len, &write_len, NULL);//发送
	_error = GetLastError();

	if (write_len != buf_len)//发送长度小于要发送的长度
		return -1;

	return 0;
}
/* return the len read */
int serial::Read(char* buf, int buf_len)
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
		if (exist_len > buf_len)
			exist_len = buf_len;
		flag = ReadFile(_hser, &rbuf, exist_len, &read_len, NULL);//读取端口值
	}

	if (!flag)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			_error = GetLastError();
			if (_error == ERROR_IO_PENDING);
		}
	}
	else
	{
		return read_len;
	}

	return 0;
}
