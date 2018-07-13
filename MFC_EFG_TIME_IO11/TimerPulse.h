#pragma once
#include "CtrlBase.h"


class CTimerPulse :
  public CCtrlBase
{
public:
  CTimerPulse();
  ~CTimerPulse();
  //×Ö¶Î
private:
  TimerPulseCtrl*    m_timePulseCtrl;//time pulse
public:
  //·½·¨
public:
  BOOL Init(int device, int module = 0);
  void DeInit();
  BOOL Config(tagCtrlParam* param);
  BOOL Start(tagCtrlParam* param);
  BOOL Stop();

  //
};

