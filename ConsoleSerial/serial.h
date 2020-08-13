#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
//#include <stdlib.h>
#include <stdio.h>
//#include <stdint.h>

/*ͬ������*/
class serialsyn
{
public:
	HANDLE _hser;
	DWORD _error;
	DCB _dcb;
	COMMTIMEOUTS _timeout;
	COMSTAT _stat;
public:
	serialsyn();
	~serialsyn();
	int OpenSerial(char* serial, 
		DWORD baud_rate = CBR_9600,
		BYTE byte_size = 8, 
		BYTE parity = NOPARITY, 
		BYTE stop_bits = ONESTOPBIT);
	void CloseSerial(void);
	int Write(char* buf, int buf_len);
	int Read(char* buf, int buf_len);



};


/*�첽����*/
class serialasy
{
public:
	HANDLE _hser;
	DWORD _error;
	DCB _dcb;
	COMMTIMEOUTS _timeout;
	COMSTAT _stat;

private:
	OVERLAPPED _ov_write;//����д������
	OVERLAPPED _ov_read;//���ڶ�ȡ����
	OVERLAPPED _ov_wait;//���ڵȴ�����
	HANDLE _thread;//��ȡ�߳̾��
		//��ȡ�߳�
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
	int Read(char* buf, int buf_len);



};