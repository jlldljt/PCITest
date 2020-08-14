#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include <stdint.h>

namespace ns_common
{
	class common
	{
	};

	uint16_t getCRC16(uint8_t* ptr, uint16_t len);

	uint16_t oneCRC16(uint8_t ch, uint16_t crc);

	void charTowchar(const char* chr, wchar_t* wchar, int size);

	void wcharTochar(const wchar_t* wchar, char* chr, int length);

	wchar_t* CharToWchar(const char* c, size_t m_encode = CP_ACP);

	char* WcharToChar(const wchar_t* wp, size_t m_encode);
}