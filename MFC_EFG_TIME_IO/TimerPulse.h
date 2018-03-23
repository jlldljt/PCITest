#pragma once
#include "CtrlBase.h"

typedef struct tagTimerPulseParam
{
  int deviceNumber;
  int moduleIndex;
  int channel;
  double frequency;
}TimerPulseParam, *PTimerPulseParam;


class CTimerPulse :
  public CCtrlBase
{
public:
  CTimerPulse();
  ~CTimerPulse();
  //�ֶ�
private:
  TimerPulseCtrl*    m_timePulseCtrl;//time pulse
  //TimerPulseParam    m_timePulseParam;
  CArray<int, int&>  m_modules;//֧�ֵ�ģ��
  CArray<int, int&>  m_channels;//֧�ֵ�ͨ��
public:
  int GetModuleCount();
  int GetModule(int no);
  int GetChannelCount();
  int GetChannel(int no);
  //����
public:
  BOOL Init(int device, int module = 0);
  void DeInit();
  int Config(LPARAM* param);
  void OnStart(LPARAM* param);
  void OnStop();

  //
};

