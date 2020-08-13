#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

class serial
{
public:
	HANDLE _hser;
	DWORD _error;
	DCB _dcb;
	COMMTIMEOUTS _timeout;
	COMSTAT _stat;
public:
	serial();
	~serial();
	int OpenSerial(char* serial);
	void CloseSerial(void);
	int Write(char* buf, int buf_len);
	int Read(char* buf, int buf_len);



};

