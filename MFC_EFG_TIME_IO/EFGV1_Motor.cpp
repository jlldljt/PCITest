#include "StdAfx.h"
#include "EFGV1_Motor.h"


CEFGV1_Motor::CEFGV1_Motor()
{
}


CEFGV1_Motor::~CEFGV1_Motor()
{
}

BOOL CEFGV1_Motor::Config(EFGV1_Param * param)
{
  ASSERT(param);
  m_channel = param->channel;
  m_boardNo = param->boardNo;

  return TRUE;
}

BOOL CEFGV1_Motor::Stop()
{
  m_boardNo = -1;
  m_channel = -1;
  return TRUE;
}

BOOL CEFGV1_Motor::Read(EFGV1_Param * param)
{
  ASSERT(param);

  if (m_channel == -1 || m_boardNo == -1)
    return FALSE;

  int val = ReadMotor(m_channel);

  param->param0 = val;

  return TRUE;
}

BOOL CEFGV1_Motor::Write(EFGV1_Param * param)
{
  ASSERT(param);

  if (m_channel == -1 || m_boardNo == -1)
    return FALSE;

  int val = param->param0;//µÍ8Î»ÓÐÐ§
  int rtn = WriteMotor(m_channel, val);

  if (rtn)
    return FALSE;

  return TRUE;
}
