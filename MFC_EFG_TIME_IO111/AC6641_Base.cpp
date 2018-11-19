#include "StdAfx.h"
#include "AC6641_Base.h"

HANDLE CAC6641_Base::m_hDevice = NULL;//AC6641¾ä±ú
int CAC6641_Base::m_ac6641Boards = 0;
CAC6641_Base::CAC6641_Base()
{
  
}


CAC6641_Base::~CAC6641_Base()
{
}

BOOL CAC6641_Base::DriverInit(void)
{
  m_hDevice = AC6641_OpenDevice(0);
  if (m_hDevice == INVALID_HANDLE_VALUE)
  {
    return FALSE;
  }
  //int sn = AC6641_SN(m_hDevice);
  //if (6641 != AC6641_SN(m_hDevice))
  //{
  //  AC6641_CloseDevice(m_hDevice);
  //  return FALSE;
  //}
  m_ac6641Boards = 1;
  return TRUE;
}

void CAC6641_Base::DriverClose(void)
{
  m_ac6641Boards = 0;
  AC6641_CloseDevice(m_hDevice); //¹Ø±Õ²Ù×÷¾ä±ú
}
