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
  //字段
private:
  TimerPulseCtrl*    m_timePulseCtrl;//time pulse
  //TimerPulseParam    m_timePulseParam;
  CArray<int, int&>  m_modules;//支持的模块
  CArray<int, int&>  m_channels;//支持的通道
public:
  int GetModuleCount();
  int GetModule(int no);
  int GetChannelCount();
  int GetChannel(int no);
  //方法
public:
  BOOL Init(int device, int module = 0);
  void DeInit();
  int Config(LPARAM* param);
  void OnStart(LPARAM* param);
  void OnStop();

  //
};

