#include "common.h"
//#include <stdlib.h>
//#include <stdio.h>
/**
* @brief  计算字符串的CRC值
*
* @param  ptr     字符串的指针
*         len     字符串的长度
*
* @retval CRC值
*
* @note
*/

uint16_t ns_common::getCRC16(uint8_t* ptr, uint16_t len)
{
	uint8_t i;
	uint16_t crc = 0xFFFF;
	if (len == 0) {
		len = 1;
	}
	while (len--) {
		crc ^= *ptr;
		for (i = 0; i < 8; i++) {
			if (crc & 1) {
				crc >>= 1;
				crc ^= 0xA001;
			}
			else {
				crc >>= 1;
			}
		}
		ptr++;
	}
	return(crc);
}

/**
* @brief  累次计算字符串的CRC值
*
* @param  ptr     字符串的指针
*         len     字符串的长度
*
* @retval CRC值
*
* @note
*/

uint16_t ns_common::oneCRC16(uint8_t ch, uint16_t crc)
{
	uint8_t i;

	crc ^= ch;
	for (i = 0; i < 8; i++) {
		if (crc & 1) {
			crc >>= 1;
			crc ^= 0xA001;
		}
		else {
			crc >>= 1;
		}
	}

	return(crc);
}

void ns_common::charTowchar(const char* chr, wchar_t* wchar, int size)
{
	MultiByteToWideChar(CP_ACP, 0, chr,
		strlen(chr) + 1, wchar, size / sizeof(wchar[0]));
}


void ns_common::wcharTochar(const wchar_t* wchar, char* chr, int length)
{
	WideCharToMultiByte(CP_ACP, 0, wchar, -1,
		chr, length, NULL, NULL);
}

wchar_t* ns_common::CharToWchar(const char* c, size_t m_encode)
{
	int len = MultiByteToWideChar(m_encode, 0, c, strlen(c), NULL, 0);
	wchar_t* m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(m_encode, 0, c, strlen(c), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}

char* ns_common::WcharToChar(const wchar_t* wp, size_t m_encode)
{
	int len = WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
	char* m_char = new char[len + 1];
	WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}