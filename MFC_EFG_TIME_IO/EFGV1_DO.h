#pragma once
#include "EFGV1_Base.h"
class CEFGV1_DO :
  public CEFGV1_Base
{
public:
  CEFGV1_DO();
  ~CEFGV1_DO();

private:
  int m_boardNo;
  int m_channel;

public:
  BOOL Config(EFGV1_Param* param);

  BOOL Stop();

  BOOL Read(EFGV1_Param * param);

  BOOL Write(EFGV1_Param* param);
};

