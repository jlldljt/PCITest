#pragma once
#include "AC6641_Base.h"
class CAC6641_DO :
  public CAC6641_Base
{
public:
  CAC6641_DO();
  ~CAC6641_DO();

private:
  int m_boardNo;
  int m_channel;

public:
  BOOL Config(AC6641_Param* param);

  BOOL Stop();

  BOOL Read(AC6641_Param * param);

  BOOL Write(AC6641_Param* param);
};

