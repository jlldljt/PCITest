#include "StdAfx.h"
#include "AC6641_DI.h"


CAC6641_DI::CAC6641_DI()
{
  m_boardNo = -1;
  m_channel = -1;
}


CAC6641_DI::~CAC6641_DI()
{
}

BOOL CAC6641_DI::Config(AC6641_Param * param)
{
  ASSERT(param);
  m_channel = param->channel;
  m_boardNo = param->boardNo;

  int mode = AC6641_GetIoMode(m_hDevice);
  mode = mode & ~(0x01 << m_channel); //对应通道设置成输入
  AC6641_SetIoMode(m_hDevice, mode, (mode >> 8));

  return TRUE;
}

BOOL CAC6641_DI::Stop()
{
  m_boardNo = -1;
  m_channel = -1;
  return TRUE;
}



BOOL CAC6641_DI::Read(AC6641_Param * param)
{
  ASSERT(param);
  if (m_channel == -1 || m_boardNo == -1)
    return FALSE;
  int val = AC6641_DI(m_hDevice, m_channel);
  
  param->param0 = val & 0xFF;//低8位有效
  return TRUE;
}