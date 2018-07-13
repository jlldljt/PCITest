#pragma once
#include "ctrlbase.h"
class COneShot :
	public CCtrlBase
{
public:
	COneShot(void);
	~COneShot(void);
//×Ö¶Î
private:
  OneShotCtrl*    m_oneShotCtrl;

//·½·¨
public:
  BOOL Init(int device, int module = 0);
  void DeInit();
  BOOL Config(tagCtrlParam* param);
  BOOL Start(tagCtrlParam* param);
  BOOL Stop();
};

