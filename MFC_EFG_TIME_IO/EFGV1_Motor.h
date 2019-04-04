#pragma once
#include "EFGV1_Base.h"
class CEFGV1_Motor :
  public CEFGV1_Base
{
public:
  CEFGV1_Motor();
  ~CEFGV1_Motor();

private:
  int m_boardNo;
  int m_channel;

public:
  BOOL Config(EFGV1_Param* param);

  BOOL Stop();

  BOOL Read(EFGV1_Param * param);

  BOOL Write(EFGV1_Param* param);
};

