#pragma once
#include "CtrlBase.h"
class CStaticDO :
  public CCtrlBase
{
public:
  CStaticDO();
  ~CStaticDO();

  //�ֶ�
private:
  InstantDoCtrl *      m_instantDoCtrl;
  int channel;
  //����
public:
  BOOL Init(int device, int module = 0);
  void DeInit();
  BOOL Config(tagCtrlParam* param);
  BOOL Start(tagCtrlParam* param);
  BOOL Stop();
  BOOL Read(tagCtrlParam* param);
  BOOL Write(tagCtrlParam* param);

};

