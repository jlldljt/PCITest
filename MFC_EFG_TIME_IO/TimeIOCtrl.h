#pragma once

#include "CtrlBase.h"
#include "CPCIBase.h"

//typedef struct tagDevConfParam
//{
//  int deviceNumber;
//  long channel;
//  WCHAR profilePath[256];
//}DevConfParam, *PDevConfParam;


//计时器通道信息
typedef struct tagTimeIOCtrl
{
  TimeIOType type;
  CCtrlBase* ctrl;
}TimeIOCtrl, *PTimeIOCtrl;

class CTimeIOCtrl : public CPCIBase
{
public:
	CTimeIOCtrl(void);
	~CTimeIOCtrl(void);

private:
  //DevConfParam       m_confParam; // the device's configure information from config form.
  //OneShotCtrl*       m_oneShotCtrl; // 延迟脉冲
  //EventCounterCtrl*  m_eventCounterCtrl; // 计数器
  //PwModulatorCtrl*   m_pwModulatorCtrl; // PWM
  //InstantDiCtrl *    m_instantDiCtrl; // DI中断

  //Di 8个通道信息
  TimeIOCtrl Di[8];//8个
  //Do 8个通道信息
  TimeIOCtrl Do[8];//8个
  //counter0 8个通道信息
  TimeIOCtrl Counter0[8];//8个
  //counter1 4个通道信息
  TimeIOCtrl Counter1[8];//8个
  //存放设备信息
 // int m_selectedDeviceNumber;
 // CArray<DevInf, DevInf&> devices;
public:
  //Di
  BOOL CreateDi(int no, TimeIOType type, int device);
  BOOL DeleteDi(int no);
  BOOL StartDi(int no, int device, double param0, double param1, void* param2 = NULL, void* param3 = NULL);
  BOOL StopDi(int no);
  BOOL ReadDi(int no, double& param0, double &param1);
  //T0 counter
  BOOL CreateT0(int no, TimeIOType type, int device);
  BOOL DeleteT0(int no);
  BOOL StartT0(int no, int device, double param0 = 0, double param1 = 0);
  BOOL StopT0(int no);
  BOOL ReadT0(int no, double& param0, double &param1);
  //T0 counter
  BOOL CreateT1(int no, TimeIOType type, int device);
  BOOL DeleteT1(int no);
  BOOL StartT1(int no, int device, double param0, double param1 = 0);
  BOOL StopT1(int no);
  //T0 counter
  BOOL CreateDO(int no, TimeIOType type, int device);
  BOOL DeleteDO(int no);
  BOOL StartDO(int no, int device, double param0);
  BOOL StopDO(int no);
  BOOL ReadDO(int no, double& param0, double &param1);
  BOOL WriteDO(int no, double& param0);

  // 获取安装的设备 理论上只要再AllDriverInit中调用一次就行了
  static int getDevices();
  // 获取设备的信息
  //int getDevice(int no, DevInf& devInf);
  // 获取通道信息 不需要的感觉
  int GetT0ChannelCount(int no);
  int GetT0Channel(int no, int channelIndex);

  ///////////////////////
  //void OneShotTest();
  //void EventCounterTest();
  //void PwModulatorTest();
  //void InstantDiTest();
  //// this function is used to deal with "Interrupt"Event.
  //static void  BDAQCALL OnDiSnapEvent(void * sender, DiSnapEventArgs * args, void * userParam);

};

