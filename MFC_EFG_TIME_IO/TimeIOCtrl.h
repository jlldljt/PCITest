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
//�忨��Ϣ
typedef struct tagDevInf
{
  int deviceNumber;
  CString description;
}DevInf, *PDevInf;
//��ʱ��ͨ����Ϣ
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
  OneShotCtrl*       m_oneShotCtrl; // �ӳ�����
  EventCounterCtrl*  m_eventCounterCtrl; // ������
  PwModulatorCtrl*   m_pwModulatorCtrl; // PWM
  InstantDiCtrl *    m_instantDiCtrl; // DI�ж�

  TimeIOCtrl Counter0[8];//8��
  TimeIOCtrl Counter1[4];//4��
  
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

