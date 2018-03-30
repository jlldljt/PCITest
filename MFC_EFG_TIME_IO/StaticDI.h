#pragma once
#include "CtrlBase.h"
class CStaticDI :
  public CCtrlBase
{
public:
  CStaticDI();
  ~CStaticDI();
  //�ֶ�
private:
  InstantDiCtrl*    m_instantDiCtrl;//di interrupt
  unsigned int m_cnt;
  //����
public:
  BOOL Init(int device, int module = 0);
  void DeInit();
  BOOL Config(tagCtrlParam* param);
  BOOL Start(tagCtrlParam* param);
  BOOL Stop();
};

