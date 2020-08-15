#pragma once

#include "common.h"
using namespace ns_common;

/*ͬ������*/
class serialsyn
{
public:
	HANDLE _hser;
	/* ���¶���˵����֧�ֶ��߳� */
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


/*�첽����*/
class serialasy : serialsyn
{
//public:
//	HANDLE _hser;
//	/* ���¶���˵����֧�ֶ��߳� */
//	DWORD _error;
//	DCB _dcb;
//	COMMTIMEOUTS _timeout;
//	COMSTAT _stat;

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
	//int Write(char* buf, int buf_len);
	int Read(char* buf, int buf_len);



};