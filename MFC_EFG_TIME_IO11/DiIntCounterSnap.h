#pragma once

//#include "TimeIOCtrl.h"
#include "CPCICtrl.h"
#include "BoardView.h"

#define LASER_SIN_NUM 180
#define LASER_CIRCLE_NUM 200
#define XRAY_ONESHOT_NUM 300
#define COUNTER_NUM 2000 // 必须大于以上宏定义
struct tagCounter {
  bool start;
  bool flag;
  int index;
  double counter_laser[4][COUNTER_NUM];
  double counter_xray[4][COUNTER_NUM];
  double fit[2][COUNTER_NUM];
  unsigned int cnt_num;
};

class CDiIntCounterSnap
{
public:
  CDiIntCounterSnap();
  ~CDiIntCounterSnap();
private:
  //di中断回调函数
  static void UserFuncLaserSin(void *param);
  static void UserFuncLaserCircle(void *param);
  static void UserFuncXRayOneShot(void * param);
  //回调中的具体操作
  void DIIntLaserSin(void);
  void DIIntLaserCircle(void);

  void DIIntXRayOneShot(void);

  int m_device/*laser*/,m_device_add/*xray*/;//设备号
  CPCIBase *m_card;//关联1780 card控制器
  CPCIBase *m_card_add;//关联1780 card控制器
  CBoardView *m_viewBoard;//关联画图界面
  //计数器计数一帧数据相关函数
public:
  //指定4计数器，记out3 6
  struct tagCounter m_counter;
  //绑定相关参数
  int BindCard(int device, CPCIBase *card, CBoardView *viewBoard, int device_add = 0, CPCIBase *card_add = NULL);
  //开启中断通道
  int StartDiIntLaserSin(int channel);
  int StartDiIntLaserCircle(int channel);
  int StartDiIntXRayOneShot(int channel);
  int StartDiIntMeasure();
  //开启out3 6
  int StartCaptureSin(int no1, double delay1/*out3*/, int no2, double delay2/*out6*/);
  //开启一帧捕捉
  int m_out;//out3 out6其中一组计数器，具体看接线
  int StartCaptureCircle(int out);
  int StartCaptureXRayOneShot();
  //停止中断捕捉
  int m_channel, m_channel_add;
  int StopDiInt();
  int StopDiIntAdd();
};

