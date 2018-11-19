#include "StdAfx.h"
#include "ComDef.h"


#ifndef UNICODE
TCHAR const * WCHAR_TO_TCHAR(WCHAR const * in, TCHAR * out)
{
  WideCharToMultiByte(CP_OEMCP, NULL, in, -1, out, 256, NULL, FALSE);
  return out;
}
#else
TCHAR const * WCHAR_TO_TCHAR(WCHAR const * in, TCHAR * out)
{
  wcscpy_s(out, wcslen(in) + 1, in);
  return out;
}
#endif

#ifndef UNICODE
WCHAR const * TCHAR_TO_WCHAR(TCHAR const * in, WCHAR* out)
{
  MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)in, -1, out, 256);
  return out;
}
#else
WCHAR const * TCHAR_TO_WCHAR(TCHAR const * in, WCHAR* out)
{
  wcscpy_s(out, wcslen(in) + 1, in);
  return out;
}
#endif


CHAR const * WCHAR_TO_CHAR(WCHAR const * in, CHAR * out)
{
  WideCharToMultiByte(CP_OEMCP, NULL, in, -1, out, 256, NULL, FALSE);
  return out;
}

WCHAR const * CHAR_TO_WCHAR(CHAR const * in, WCHAR* out)
{
  MultiByteToWideChar(CP_ACP, 0, (LPCSTR)in, -1, out, 256);
  return out;
}

