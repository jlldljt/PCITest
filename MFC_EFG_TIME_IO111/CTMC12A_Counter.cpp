#include "StdAfx.h"
#include "CTMC12A_Counter.h"


CTMC12A_Counter::CTMC12A_Counter()
{
  m_boardNo = -1;
  m_channel = -1;
}


CTMC12A_Counter::~CTMC12A_Counter()
{
}

BOOL CTMC12A_Counter::Config(tagTMC12A_Param * param)
{
  return TRUE;
}

BOOL CTMC12A_Counter::Start(tagTMC12A_Param * param)
{
  ASSERT(param); 
  //Set timer counter
  WORD wRtn = Ixud_SetCounter(param->boardNo, param->channel, param->param0, param->param1);
  if (wRtn)
    return FALSE;

  m_boardNo = param->boardNo;
  m_channel = param->channel;
  return TRUE;
}

BOOL CTMC12A_Counter::Stop()
{
  if (m_channel  == 0xFFFF  || m_boardNo == 0xFFFF )
    return FALSE;
  Ixud_DisableCounter(m_boardNo, m_channel);
  m_boardNo = -1;
  m_channel = -1;
  return TRUE;
}

BOOL CTMC12A_Counter::Read(tagTMC12A_Param * param)
{
  ASSERT(param);
  if (m_channel < 0 || m_boardNo < 0)
    return FALSE;

  DWORD val;
  WORD wRtn = Ixud_ReadCounter(m_boardNo, m_channel, &val);
  if (wRtn)
    return FALSE;

  param->param1 = val;
  return TRUE;
}
