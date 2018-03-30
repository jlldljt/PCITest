#pragma once
#include "CtrlBase.h"
class CStaticDI :
  public CCtrlBase
{
public:
  CStaticDI();
  ~CStaticDI();
  //×Ö¶Î
private:
  InstantDiCtrl*    m_instantDiCtrl;//di interrupt
  unsigned int m_cnt;
  //·½·¨
public:
  BOOL Init(int device, int module = 0);
  void DeInit();
  BOOL Config(tagCtrlParam* param);
  BOOL Start(tagCtrlParam* param);
  BOOL Stop();
};

