#include "StdAfx.h"
#include "CTMC12A_DO.h"


CTMC12A_DO::CTMC12A_DO()
{
  m_boardNo = -1;
  m_channel = -1;
}


CTMC12A_DO::~CTMC12A_DO()
{
}

BOOL CTMC12A_DO::Config(tagTMC12A_Param * param)
{
  return TRUE;
}

BOOL CTMC12A_DO::Start(tagTMC12A_Param * param)
{
  ASSERT(param);
  m_channel = param->channel;
  m_boardNo = param->boardNo;
  return TRUE;
}

BOOL CTMC12A_DO::Stop()
{
  m_boardNo = -1;
  m_channel = -1;
  return TRUE;
}


BOOL CTMC12A_DO::Write(tagTMC12A_Param * param)
{
  
  ASSERT(param);
  if (m_channel < 0 || m_boardNo < 0)
    return FALSE;
  WORD val = param->param0;
  WORD wRtn = Ixud_WriteDOBit(m_boardNo, 0, m_channel, val);
  if (wRtn)
    return FALSE;

  return TRUE;
}
