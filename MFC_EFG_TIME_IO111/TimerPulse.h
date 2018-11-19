#pragma once
#include "CtrlBase.h"


class CTimerPulse :
  public CCtrlBase
{
public:
  CTimerPulse();
  ~CTimerPulse();
  //�ֶ�
private:
  TimerPulseCtrl*    m_timePulseCtrl;//time pulse
public:
  //����
public:
  BOOL Init(int device, int module = 0);
  void DeInit();
  BOOL Config(tagCtrlParam* param);
  BOOL Start(tagCtrlParam* param);
  BOOL Stop();

  //
};

