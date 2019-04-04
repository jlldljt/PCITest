#include "StdAfx.h"
#include "EFGV1_DI.h"


CEFGV1_DI::CEFGV1_DI()
{
}


CEFGV1_DI::~CEFGV1_DI()
{
}

BOOL CEFGV1_DI::Config(EFGV1_Param * param)
{
  ASSERT(param);
  m_channel = param->channel;
  m_boardNo = param->boardNo;

  return TRUE;
}

BOOL CEFGV1_DI::Stop()
{
  m_boardNo = -1;
  m_channel = -1;
  return TRUE;
}

BOOL CEFGV1_DI::Read(EFGV1_Param * param)
{
  ASSERT(param);

  if (m_channel == -1 || m_boardNo == -1)
    return FALSE;

  int val = ReadDi(m_channel);

  param->param0 = val;

  return TRUE;
}
