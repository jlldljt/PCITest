#pragma once

#include "TimerPulse.h"

typedef struct tagDevConfParam
{
  int deviceNumber;
  long channel;
  WCHAR profilePath[256];
}DevConfParam, *PDevConfParam;

enum TimeIOType {
  EVENT_COUNTER = 1 << 7,
  ONE_SHOT = 1 << 9,
  TIME_PULSE = 1 << 10,
  PW_MODULATOR = 1 << 12,
};
//板卡信息
typedef struct tagDevInf
{
  int deviceNumber;
  CString description;
}DevInf, *PDevInf;
//计时器通道信息
typedef struct tagTimeIOCtrl
{
  TimeIOType type;
  CCtrlBase* ctrl;
}TimeIOCtrl, *PTimeIOCtrl;

class CTimeIOCtrl
{
public:
	CTimeIOCtrl(void);
	~CTimeIOCtrl(void);

private:
  DevConfParam       m_confParam; // the device's configure information from config form.
  OneShotCtrl*       m_oneShotCtrl; // 延迟脉冲
  EventCounterCtrl*  m_eventCounterCtrl; // 计数器
  PwModulatorCtrl*   m_pwModulatorCtrl; // PWM
  InstantDiCtrl *    m_instantDiCtrl; // DI中断

  TimeIOCtrl Counter0[8];//8个
  TimeIOCtrl Counter1[4];//4个
  
public:
  int CreateT0(int no, int type);
  int m_selectedDeviceNumber;
  CArray<DevInf, DevInf&> devices;
  int getDevices();
  int getDevice(int no, DevInf& devInf);

  ///////////////////////
  void OneShotTest();
  void EventCounterTest();
  void PwModulatorTest();
  void InstantDiTest();
  // this function is used to deal with "Interrupt"Event.
  static void  BDAQCALL OnDiSnapEvent(void * sender, DiSnapEventArgs * args, void * userParam);
};

