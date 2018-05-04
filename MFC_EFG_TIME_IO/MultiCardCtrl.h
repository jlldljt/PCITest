#pragma once
#include "TimeIOCtrl.h"
class CMultiCardCtrl
{
public:
  CMultiCardCtrl();
  ~CMultiCardCtrl();

public:
#define MAX_CARD_NUM 2
#define MAX_CHANNEL_NUM  8
  CTimeIOCtrl m_card[MAX_CARD_NUM];//1780¿¨

private:
  int LoadDI(const int no, const int cardSel, DevInf& dev);
  int LoadT0(const int no, const int cardSel, DevInf& dev);
  int LoadT1(const int no, const int cardSel, DevInf& dev);
public:
  int Load(const int cardSel);
  int Stop(const int cardSel);

};

