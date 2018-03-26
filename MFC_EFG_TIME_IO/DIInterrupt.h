#pragma once
#include "CtrlBase.h"
class CDIInterrupt :
  public CCtrlBase
{
public:
  CDIInterrupt();
  ~CDIInterrupt();
  static void  BDAQCALL OnDiSnapEvent(void * sender, DiSnapEventArgs * args, void * userParam);
  //�ֶ�
private:
  InstantDiCtrl*    m_instantDiCtrl;//di interrupt

  //����
public:
  BOOL Init(int device, int module = 0);
  void DeInit();
  BOOL Config(tagCtrlParam* param);
  BOOL Start(tagCtrlParam* param);
  BOOL Stop();
};

