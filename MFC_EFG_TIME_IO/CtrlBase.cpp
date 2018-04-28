#include "StdAfx.h"
#include "CtrlBase.h"


#ifndef UNICODE
TCHAR const * WCHAR_TO_TCHAR(WCHAR const * in, TCHAR * out)
{
  WideCharToMultiByte(CP_OEMCP, NULL, in, -1, out, MAX_DEVICE_DESC_LEN, NULL, FALSE);
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
  MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)in, -1, out, MAX_DEVICE_DESC_LEN);
  return out;
}
#else
WCHAR const * TCHAR_TO_WCHAR(TCHAR const * in, WCHAR* out)
{
  wcscpy_s(out, wcslen(in) + 1, in);
  return out;
}
#endif

CCtrlBase::CCtrlBase()
{
  m_callback = NULL;
}


CCtrlBase::~CCtrlBase()
{
  m_callback = NULL;
}

int CCtrlBase::GetType()
{
  return SceTimerPulse;
}

void CCtrlBase::CheckError(ErrorCode errorCode)
{
  if (BioFailed(errorCode))
  {
    TRACE(_T("Some errors happened, the error code is: 0x%X !\n"), errorCode);
    //AfxMessageBox(str);
  }
}

int CCtrlBase::GetModuleCount()
{
  return m_modules.GetCount();
}

int CCtrlBase::GetModule(int no)
{
  return m_modules.GetAt(no);
}

int CCtrlBase::GetChannelCount()
{
  return m_channels.GetCount();
}

int CCtrlBase::GetChannel(int no)
{
  return m_channels.GetAt(no);
}
