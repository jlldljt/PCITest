#pragma once

#include "TimerPulse.h"
#include "PWModulator.h"
#include "EventCounter.h"

//typedef struct tagDevConfParam
//{
//  int deviceNumber;
//  long channel;
//  WCHAR profilePath[256];
//}DevConfParam, *PDevConfParam;

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
  //DevConfParam       m_confParam; // the device's configure information from config form.
  //OneShotCtrl*       m_oneShotCtrl; // �ӳ�����
  //EventCounterCtrl*  m_eventCounterCtrl; // ������
  //PwModulatorCtrl*   m_pwModulatorCtrl; // PWM
  //InstantDiCtrl *    m_instantDiCtrl; // DI�ж�

  //Di 8��ͨ����Ϣ
  TimeIOCtrl Di[8];//8��
  //Do 8��ͨ����Ϣ
  TimeIOCtrl Do[8];//8��
  //counter0 8��ͨ����Ϣ
  TimeIOCtrl Counter0[8];//8��
  //counter1 4��ͨ����Ϣ
  TimeIOCtrl Counter1[4];//4��
  //����豸��Ϣ
  int m_selectedDeviceNumber;
  CArray<DevInf, DevInf&> devices;
public:
  //Di
  BOOL CreateDi(int no, TimeIOType type, int device);
  BOOL DeleteDi(int no);
  BOOL StartDi(int no, int device, double param0);
  BOOL StopDi(int no);
  //T0 counter
  BOOL CreateT0(int no, TimeIOType type, int device);
  BOOL DeleteT0(int no);
  BOOL StartT0(int no, int device, double param0 = 0, double param1 = 0);
  BOOL StopT0(int no);
  BOOL ReadT0(int no, double& param0, double &param1);
  //T0 counter
  BOOL CreateT1(int no, TimeIOType type, int device);
  BOOL DeleteT1(int no);
  BOOL StartT1(int no, int device, double param0);
  BOOL StopT1(int no);

  // ��ȡ��װ���豸
  int getDevices();
  // ��ȡ�豸����Ϣ
  int getDevice(int no, DevInf& devInf);
  // ��ȡͨ����Ϣ ����Ҫ�ĸо�
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

