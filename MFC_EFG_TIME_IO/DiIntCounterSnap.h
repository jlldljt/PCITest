#pragma once

//#include "TimeIOCtrl.h"
//#include "CPCICtrl.h"
#include "EfgIO.h"
#include "BoardView.h"

#define LASER_SIN_NUM 180
#define LASER_CIRCLE_NUM 200
#define XRAY_ONESHOT_NUM 30000
#define COUNTER_NUM LASER_SIN_NUM+LASER_CIRCLE_NUM+XRAY_ONESHOT_NUM // 必须大于以上宏定义
struct tagCounter {
  int start;
  bool flag;
  int index[2];
  double counter[6][COUNTER_NUM]; // 测量时前4个激光，后两个x光
  double tmp_counter[6][COUNTER_NUM]; // 测量时前4个激光，后两个x光
  double fit[2][COUNTER_NUM];
  unsigned int cnt_num;
};

enum emMotor
{
	MOTOR_STOP = 0,
	MOTOR_RUN,


};

struct tagMotor {
  double cur_step;
  double dst_step;
  double cur_acc;
  double max_acc;
  double max_speed;
  double cur_speed;
  int state;
  bool stop;
};
class CDiIntCounterSnap
{
public:
  CDiIntCounterSnap();
  ~CDiIntCounterSnap();
private:
  //di中断回调函数
  static void UserFuncLaserSin(void *param);
  static void UserFuncLaserSin_TMC12A(void * param);
  static void UserFuncLaserCircle(void *param);
  static void UserFuncXRayOneShot(void * param);
  //回调中的具体操作
  void DIIntLaserSin(void);
  void DIIntLaserSin_TMC12A(void);
  void DIIntLaserCircle(void);

  void DIIntXRayOneShot(void);

public:
  int m_device;// 设备号
  CPCIBase *m_card;//关联1780 card控制器
  CBoardView *m_viewBoard;//关联画图界面
  CEfgIO *m_efgio;//关联efg控制器
  //计数器计数一帧数据相关函数
  //指定4计数器，记out3 6
  struct tagCounter m_counter;
  //绑定相关参数
  int BindCard(int device, CPCIBase *card, CBoardView *viewBoard, CEfgIO *efgio = NULL);
  //开启中断通道
  int StartDiIntLaserSin(int channel);
  int StartDiIntLaserCircle(int channel);
  int StartDiIntXRayOneShot(int channel);
  int StartXRayAndLaser(int channel);
  int StartDiIntMeasure();
  //开启out3 6
  int StartCaptureSin(int no1, double delay1/*out3*/, int no2, double delay2/*out6*/);
  //开启一帧捕捉
  int m_out;//out3 out6其中一组计数器，具体看接线
  int StartCaptureCircle(int out);
  int StartCaptureXRayOneShot();
  int StartMeasure();
  int TestS();
  int CheckStart();
  //数据处理
  int LaserFit();
  int LaserFit(int n);//用于多次测量
  int XrayFit();
  int XrayFit(int n);
  int XrayFitTest(int n);
  //用于多次测量
 // int CalcResult();
  //停止中断捕捉
  int m_channel; // 中断通道
  int StopDiInt();
  //dianji 
  struct tagMotor m_motor_u;
  int StartURunTrd(double step, double acc, double speed);
  
};

