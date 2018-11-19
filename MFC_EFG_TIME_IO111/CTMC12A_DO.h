#pragma once
#include "CTMC12A_Base.h"
class CTMC12A_DO :
  public CTMC12A_Base
{
public:
  CTMC12A_DO();
  ~CTMC12A_DO();

private:
  WORD m_boardNo;
  WORD m_channel;

public:
  BOOL Config(tagTMC12A_Param* param);
  BOOL Start(tagTMC12A_Param* param);
  BOOL Stop();
  BOOL Write(tagTMC12A_Param* param);
};

