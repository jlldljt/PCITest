#include "StdAfx.h"
#include "AC6641_DO.h"


CAC6641_DO::CAC6641_DO()
{
  m_boardNo = -1;
  m_channel = -1;
}


CAC6641_DO::~CAC6641_DO()
{
}

BOOL CAC6641_DO::Config(AC6641_Param * param)
{
  ASSERT(param);
  m_channel = param->channel;
  m_boardNo = param->boardNo;

  int mode = AC6641_GetIoMode(m_hDevice);
  mode = mode | (0x01 << m_channel); //对应通道设置成输出
  AC6641_SetIoMode(m_hDevice, mode, (mode>>8));


  return TRUE;
}

BOOL CAC6641_DO::Stop()
{
  m_boardNo = -1;
  m_channel = -1;
  return TRUE;
}

BOOL CAC6641_DO::Read(AC6641_Param * param)
{
  ASSERT(param);
  if (m_channel == -1 || m_boardNo == -1)
    return FALSE;

  int val = AC6641_DI(m_hDevice, m_channel);

  param->param0 = val & 0xFF;//低8位有效
  return TRUE;
}

BOOL CAC6641_DO::Write(AC6641_Param * param)
{
  ASSERT(param);
  if (m_channel == -1 || m_boardNo == -1)
    return FALSE;
  int val = param->param0;//低8位有效
  int rtn = AC6641_DO(m_hDevice, m_channel, val);
  if (rtn)
    return FALSE;

  return TRUE;
}
