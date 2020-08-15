#pragma once

#include "common.h"
using namespace ns_common;

/*同步串口*/
class serialsyn
{
public:
	HANDLE _hser;
	/* 如下定义说明不支持多线程 */
	DWORD _error;
	DCB _dcb;
	COMMTIMEOUTS _timeout;
	COMSTAT _stat;
public:
	serialsyn();
	~serialsyn();
	virtual int OpenSerial(char* serial, 
		DWORD baud_rate = CBR_9600,
		BYTE byte_size = 8, 
		BYTE parity = NOPARITY, 
		BYTE stop_bits = ONESTOPBIT);
	virtual void CloseSerial(void);
	virtual int Write(char* buf, int buf_len);
	virtual int Read(char* buf, int buf_len);



};


/*异步串口*/
class serialasy : serialsyn
{
//public:
//	HANDLE _hser;
//	/* 如下定义说明不支持多线程 */
//	DWORD _error;
//	DCB _dcb;
//	COMMTIMEOUTS _timeout;
//	COMSTAT _stat;

private:
	OVERLAPPED _ov_write;//用于写入数据
	OVERLAPPED _ov_read;//用于读取数据
	OVERLAPPED _ov_wait;//用于等待数据
	HANDLE _thread;//读取线程句柄
		//读取线程
	static unsigned int __stdcall RecvThread(void*);

public:
	serialasy();
	~serialasy();
	int OpenSerial(char* serial,
		DWORD baud_rate = CBR_9600,
		BYTE byte_size = 8,
		BYTE parity = NOPARITY,
		BYTE stop_bits = ONESTOPBIT);
	void CloseSerial(void);
	int Write(char* buf, int buf_len);
	//int Write(char* buf, int buf_len);
	int Read(char* buf, int buf_len);



};