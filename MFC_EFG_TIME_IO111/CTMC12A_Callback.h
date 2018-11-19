#pragma once
#include "CTMC12A_Base.h"
class CTMC12A_Callback :
  public CTMC12A_Base
{
public:
  CTMC12A_Callback();
  ~CTMC12A_Callback();
  static void WINAPI CTMC12A_Callback::CallbackFun(DWORD param);
private:
  WORD m_boardNo;
  WORD m_channel;

  unsigned int m_cnt;
public:
  BOOL Config(tagTMC12A_Param* param);
  BOOL Start(tagTMC12A_Param* param);
  BOOL Stop();
  BOOL Read(tagTMC12A_Param* param);
};

