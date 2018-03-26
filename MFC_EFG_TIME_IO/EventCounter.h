#pragma once
#include "CtrlBase.h"
class CEventCounter :
  public CCtrlBase
{
public:
  CEventCounter();
  ~CEventCounter();

//×Ö¶Î
private:
  EventCounterCtrl*    m_eventCounterCtrl;//event counter

//·½·¨
public:
  BOOL Init(int device, int module = 0);
  void DeInit();
  BOOL Config(tagCtrlParam* param);
  BOOL Start(tagCtrlParam* param);
  BOOL Stop();
  BOOL Read(tagCtrlParam* param);
};

