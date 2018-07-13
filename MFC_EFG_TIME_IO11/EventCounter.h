#pragma once
#include "CtrlBase.h"
class CEventCounter :
  public CCtrlBase
{
public:
  CEventCounter();
  ~CEventCounter();

//�ֶ�
private:
  EventCounterCtrl*    m_eventCounterCtrl;//event counter

//����
public:
  BOOL Init(int device, int module = 0);
  void DeInit();
  BOOL Config(tagCtrlParam* param);
  BOOL Start(tagCtrlParam* param);
  BOOL Stop();
  BOOL Read(tagCtrlParam* param);
};

