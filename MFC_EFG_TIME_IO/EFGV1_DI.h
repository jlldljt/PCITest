#pragma once
#include "EFGV1_Base.h"
class CEFGV1_DI :
  public CEFGV1_Base
{
public:
  CEFGV1_DI();
  ~CEFGV1_DI();

private:
  int m_boardNo;
  int m_channel;

public:
  BOOL Config(EFGV1_Param* param);

  BOOL Stop();

  BOOL Read(EFGV1_Param * param);


};

