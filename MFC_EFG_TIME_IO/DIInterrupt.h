#pragma once
#include "CtrlBase.h"
class CDIInterrupt :
  public CCtrlBase
{
public:
  CDIInterrupt();
  ~CDIInterrupt();
  static void  BDAQCALL OnDiSnapEvent(void * sender, DiSnapEventArgs * args, void * userParam);
  //×Ö¶Î
private:
  InstantDiCtrl*    m_instantDiCtrl;//di interrupt

  //·½·¨
public:
  BOOL Init(int device, int module = 0);
  void DeInit();
  BOOL Config(tagCtrlParam* param);
  BOOL Start(tagCtrlParam* param);
  BOOL Stop();
};

