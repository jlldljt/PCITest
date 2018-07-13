#include "StdAfx.h"
#include "CTMC12A_DI.h"


CTMC12A_DI::CTMC12A_DI()
{
  m_boardNo = -1;
  m_channel = -1;
}


CTMC12A_DI::~CTMC12A_DI()
{
}

BOOL CTMC12A_DI::Config(tagTMC12A_Param * param)
{
  return TRUE;
}

BOOL CTMC12A_DI::Start(tagTMC12A_Param * param)
{
  ASSERT(param);
  m_channel = param->channel;
  m_boardNo = param->boardNo;
  return TRUE;
}

BOOL CTMC12A_DI::Stop()
{
  m_boardNo = -1;
  m_channel = -1;
  return TRUE;
}

BOOL CTMC12A_DI::Read(tagTMC12A_Param * param)
{
  ASSERT(param);
  if (m_channel < 0 || m_boardNo < 0)
    return FALSE;
  WORD val;
  WORD wRtn = Ixud_ReadDIBit(m_boardNo, 0, m_channel, &val);
  if (wRtn)
    return FALSE;

  param->param0 = val;
  return TRUE;
}
