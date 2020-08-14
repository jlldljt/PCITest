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

	if (strlen(serial) > 4) // com10以上
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
			printf("没有发现串口或串口已被占用");
		return -1;

	}
	else
	{
		SetupComm(_hser, 4096, 4096);//分配缓冲区

		GetCommState(_hser, &_dcb);//读取DCB结构
		//_dcb.DCBlength = sizeof(_dcb);
		_dcb.BaudRate = baud_rate;//波特率为9600
		_dcb.ByteSize = byte_size;//数据位为8位
		_dcb.fParity = 1;//奇偶校验
		_dcb.Parity = parity;//ODDPARITY;
		_dcb.StopBits = stop_bits;//一位停止位
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
	DWORD write_len = 0;//发送出去的字符长度
	ClearCommError(_hser, &_error, &_stat);
	WriteFile(_hser, buf, buf_len, &write_len, NULL);//发送
	_error = GetLastError();

	if (write_len != buf_len)//发送长度小于要发送的长度
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
		flag = ReadFile(_hser, rbuf, buf_len, &read_len, NULL);//读取端口值
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
//unsigned int __stdcall serialasy::RecvThread(void* LPParam)// 配合ReadIntervalTimeout = 2
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
//		//数据已经到达缓存区，读取会立即返回，并返回True， 否则返回False
//		if (Status == FALSE && GetLastError() == ERROR_IO_PENDING)
//		{
//			//如果有数据到达 且 时间超过ReadIntervalTimeout则会返回，否则会一直等待
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
//unsigned int __stdcall serialasy::RecvThread(void* param) // 配合_timeout.ReadIntervalTimeout = MAXDWORD
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
//		//WaitCommEvent也是一个异步命令，所以需要等待
//		if (FALSE == Status && GetLastError() == ERROR_IO_PENDING)//
//		{
//			//如果缓存中无数据线程会停在此，如果_hser关闭会立即返回False
//			Status = GetOverlappedResult(obj->_hser, &obj->_ov_wait, &Bytes, TRUE);
//		}
//		ClearCommError(obj->_hser, &Error, &cs);
//		if (TRUE == Status //等待事件成功
//			&& WaitEvent & EV_RXCHAR//缓存中有数据到达
//			&& cs.cbInQue > 0)//有数据
//		{
//			Bytes = 0;
//			obj->_ov_read.Offset = 0;
//			memset(ReadBuf, 0, sizeof(ReadBuf));
//			//数据已经到达缓存区，ReadFile不会当成异步命令，而是立即读取并返回True
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

	if (strlen(serial) > 4) // com10以上
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
			printf("没有发现串口或串口已被占用");
		return -1;

	}
	else
	{
		SetupComm(_hser, 4096, 4096);//分配缓冲区

		GetCommState(_hser, &_dcb);//读取DCB结构
		//_dcb.DCBlength = sizeof(_dcb);
		_dcb.BaudRate = baud_rate;//波特率为9600
		_dcb.ByteSize = byte_size;//数据位为8位
		_dcb.fParity = 1;//奇偶校验
		_dcb.Parity = parity;//ODDPARITY;
		_dcb.StopBits = stop_bits;//一位停止位
		SetCommState(_hser, &_dcb);

		//读取延时设为2ms，这样ReadFile异步读取时，会返回FALSE，并且GetLastError() == ERROR_IO_PENDING。
		//如果设为MAXDWORD，ReadFile异步读取时，会返回TRUE
		//然后调用GetOverlappedResult等待数据到达，如果有数据到达且时间超过2ms则会返回，否则会一直等待。
		/*
		两字符之间最大的延时，
		当读取串口数据时，一旦两个字符传输的时间差超过该时间，读取函数将返回现有的数据。
		设置为0表示该参数不起作用。指定时间最大值（毫秒），允许接收的2个字节间有时间差。
		也就 是说，
		刚接收了一个字节后，等了ReadIntervalTimeout时间后还没有新的字节到达，就 认为本次读串口操作结束（后面的字节等下一次读取操作来处理）。
		即使你想读8个字节，但读第2个字节后，过了ReadIntervalTimeout时间后，第3个字节还没到。实际上就只读了2个字节。
		*/
		_timeout.ReadIntervalTimeout = 100;
		//_timeout.ReadIntervalTimeout = MAXDWORD;//读取无延时，因为有WaitCommEvent等待数据
		/*一次读取串口数据的固定超时。
		所以在一次读取串口的操作中，其超时为ReadTotalTimeoutMultiplier乘以读取的字节数再加上 ReadTotalTimeoutConstant。
		将ReadIntervalTimeout设置为MAXDWORD，并将ReadTotalTimeoutMultiplier 和ReadTotalTimeoutConstant设置为0，
		表示读取操作将立即返回存放在输入缓冲区的字符。
		可以理解为一个修正时间，实际上就是按ReadTotalTimeoutMultiplier计算出的超时时间再加上该时间才作为整个超时时间。*/
		_timeout.ReadTotalTimeoutConstant = 0;  //
		_timeout.ReadTotalTimeoutMultiplier = 0;//

		_timeout.WriteTotalTimeoutMultiplier = 500;
		_timeout.WriteTotalTimeoutConstant = 5000;

		PurgeComm(_hser, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

		//创建事件对象
		_ov_write.hEvent = CreateEvent(NULL, false, false, NULL);
		_ov_read.hEvent = CreateEvent(NULL, false, false, NULL);
		_ov_wait.hEvent = CreateEvent(NULL, false, false, NULL);

		SetCommMask(_hser, EV_ERR | EV_RXCHAR);//设置接受事件

		//创建读取线程
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
	//	WaitForSingleObject(_thread, 5000);//等待线程结束
	//	CloseHandle(_thread);
	//	_thread = NULL;
	//}
}

int serialasy::Write(char* buf, int buf_len)
{
	BOOL ret = FALSE;
	DWORD write_len = 0;//发送出去的字符长度
	ClearCommError(_hser, &_error, &_stat);
	ret = WriteFile(_hser, buf, buf_len, &write_len, &_ov_write);//发送

	if (FALSE == ret && GetLastError() == ERROR_IO_PENDING)//后台读取
	{
		//等待数据写入完成
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
	flag = ReadFile(_hser, buf, buf_len, &bytes, &_ov_read);//绑定io
	//数据已经到达缓存区，读取会立即返回，并返回True， 否则返回False
	if (flag == FALSE && GetLastError() == ERROR_IO_PENDING)
	{
		//如果有数据到达 且 时间超过ReadIntervalTimeout则会返回，否则会一直等待
		flag = GetOverlappedResult(_hser, &_ov_read, &bytes, TRUE);// 等待io
	}
	if (FALSE != flag && bytes > 0)
	{
		//cout << "Read:" << (LPCSTR)ReadBuf << "   Len:" << Bytes << endl;
	}
	PurgeComm(_hser, PURGE_RXCLEAR | PURGE_RXABORT);



	return bytes;
}
