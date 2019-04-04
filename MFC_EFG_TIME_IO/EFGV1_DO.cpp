#include "StdAfx.h"
#include "EFGV1_DO.h"


CEFGV1_DO::CEFGV1_DO()
{
}


CEFGV1_DO::~CEFGV1_DO()
{
}

BOOL CEFGV1_DO::Config(EFGV1_Param * param)
{
  ASSERT(param);
  m_channel = param->channel;
  m_boardNo = param->boardNo;

  return TRUE;
}

BOOL CEFGV1_DO::Stop()
{
  m_boardNo = -1;
  m_channel = -1;
  return TRUE;
}

BOOL CEFGV1_DO::Read(EFGV1_Param * param)
{
  ASSERT(param);

  if (m_channel == -1 || m_boardNo == -1)
    return FALSE;

  int val = ReadDo(m_channel);

  param->param0 = val;

  return TRUE;
}

BOOL CEFGV1_DO::Write(EFGV1_Param * param)
{
  ASSERT(param);

  if (m_channel == -1 || m_boardNo == -1)
    return FALSE;

  int val = param->param0;//µÍ8Î»ÓÐÐ§
  int rtn = WriteDo(m_channel, val);

  if (rtn)
    return FALSE;

  return TRUE;
}
