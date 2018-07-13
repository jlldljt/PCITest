#include "StdAfx.h"
#include "CtrlBase.h"



CCtrlBase::CCtrlBase()
{
  m_callback = NULL; 
  m_callbackParam = NULL; 
}


CCtrlBase::~CCtrlBase()
{
  m_callback = NULL;
  m_callbackParam = NULL;
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
