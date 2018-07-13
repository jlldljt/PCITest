#pragma once
#include "CTMC12A_Base.h"
class CTMC12A_DI :
  public CTMC12A_Base
{
public:
  CTMC12A_DI();
  ~CTMC12A_DI();
  //×Ö¶Î
private:
  WORD m_boardNo;
  WORD m_channel;

public:
  BOOL Config(tagTMC12A_Param* param);
  BOOL Start(tagTMC12A_Param* param);
  BOOL Stop();
  BOOL Read(tagTMC12A_Param* param);
};

