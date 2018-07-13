#pragma once
#include "ctrlbase.h"
class COneShot :
	public CCtrlBase
{
public:
	COneShot(void);
	~COneShot(void);
//�ֶ�
private:
  OneShotCtrl*    m_oneShotCtrl;

//����
public:
  BOOL Init(int device, int module = 0);
  void DeInit();
  BOOL Config(tagCtrlParam* param);
  BOOL Start(tagCtrlParam* param);
  BOOL Stop();
};

