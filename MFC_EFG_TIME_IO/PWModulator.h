#pragma once
#include "CtrlBase.h"
class CPWModulator :
  public CCtrlBase
{
public:
  CPWModulator();
  ~CPWModulator();

//�ֶ�
private:
  PwModulatorCtrl*    m_pwModulatorCtrl;//pwm

//����
public:
  BOOL Init(int device, int module = 0);
  void DeInit();
  BOOL Config(tagCtrlParam* param);
  BOOL Start(tagCtrlParam* param);
  BOOL Stop();

};

