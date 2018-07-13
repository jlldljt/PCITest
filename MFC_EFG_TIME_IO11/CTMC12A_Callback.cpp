#include "StdAfx.h"
#include "CTMC12A_Callback.h"

void WINAPI CTMC12A_Callback::CallbackFun(DWORD param)
{
  CTMC12A_Callback * uParam = (CTMC12A_Callback *)param;
  if (uParam->m_callback) {
    uParam->m_callback(uParam->m_callbackParam);
  }
  uParam->m_cnt++;
}

CTMC12A_Callback::CTMC12A_Callback()
{
  m_boardNo = -1;
  m_channel = -1;
}


CTMC12A_Callback::~CTMC12A_Callback()
{
}

BOOL CTMC12A_Callback::Config(tagTMC12A_Param * param)
{
  return TRUE;
}

BOOL CTMC12A_Callback::Start(tagTMC12A_Param * param)
{
  ASSERT(param);

  m_callback = param->proc;
  m_callbackParam = param->procParam;
  WORD eventType = IXUD_HARDWARE_INT;
  if (param->param0)
    eventType |= IXUD_ACTIVE_HIGH;
  else
    eventType |= IXUD_ACTIVE_LOW;

  WORD wRtn = Ixud_SetEventCallback(param->boardNo, eventType, 1,NULL, CallbackFun, (DWORD)this);
  if (wRtn)
    return FALSE;
  //Install the IRQ for INT_0
  wRtn = Ixud_InstallIrq(param->boardNo, 0x1);
  if (wRtn)
    return FALSE;

  m_channel = param->channel;
  m_boardNo = param->boardNo;
  return TRUE;
}

BOOL CTMC12A_Callback::Stop()
{
  if (m_channel < 0 || m_boardNo < 0)
    return FALSE;
  // Revmove the Interrupt
  WORD wRtn = Ixud_RemoveIrq(m_boardNo);
  // Remove the Callback Event
  wRtn = Ixud_RemoveEventCallback(m_boardNo, 1);

  m_boardNo = -1;
  m_channel = -1;
  return TRUE;
}

BOOL CTMC12A_Callback::Read(tagTMC12A_Param * param)
{
  ASSERT(param);
  if (m_channel < 0 || m_boardNo < 0)
    return FALSE;
  param->param0 = m_cnt;
  param->param1 = 0;
  return TRUE;
}
