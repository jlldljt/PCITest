#pragma once
//#include "TimeIOCtrl.h"
#include "CPCICtrl.h"
#include "DiIntCounterSnap.h"
class CMultiCardCtrl
{
public:
  CMultiCardCtrl();
  ~CMultiCardCtrl();

public:
#define MAX_CARD_NUM 3
#define MAX_CHANNEL_NUM  8
  CPCIBase *m_card[MAX_CARD_NUM];//1780卡
  CDiIntCounterSnap m_diInt[MAX_CARD_NUM];
  int m_xray_cardsel;
  int m_laser_cardsel;
private:
  int LoadDI(const int no, const int cardSel, DevInf& dev);
  int LoadDO(int no, int cardSel, DevInf & dev);
  int LoadT0(const int no, const int cardSel, DevInf& dev);
  int LoadT1(const int no, const int cardSel, DevInf& dev);
public:
  int Load(const int cardSel);
  int Stop(const int cardSel);

  //void StartXrayOneshot(const int cardSel);

  //void StartLaserSin(const int cardSel);

  //void MeasureStart(double out3_delay/*out3 第几行*/, double out6_delay/*out6 第几行*/);

};

