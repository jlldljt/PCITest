#pragma once

#include "Inc\bdaqctrl.h"
using namespace Automation::BDaq;


typedef void(*TimeIOProc)(void *param);

TCHAR const * WCHAR_TO_TCHAR(WCHAR const * in, TCHAR * out);

WCHAR const * TCHAR_TO_WCHAR(TCHAR const * in, WCHAR* out);

typedef struct tagCtrlParam
{
  int deviceNumber;
  int moduleIndex;
  int channel;
  double param0;
  double param1;
  TimeIOProc proc;
}CtrlParam, *PCtrlParam;

class CCtrlBase
{
public:
  CCtrlBase();
  ~CCtrlBase();
  CArray<int, int&>  m_modules;//支持的模块
  CArray<int, int&>  m_channels;//支持的通道
  TimeIOProc m_callback;


  int GetModuleCount();
  int GetModule(int no);
  int GetChannelCount();
  int GetChannel(int no);
  void CheckError(ErrorCode errorCode);

  static DaqCtrlBase * Create(Scenario type) { return (DaqCtrlBase *)DaqCtrlBase_Create(type); }
  static void Dispose(DeviceCtrl* obj) { DeviceCtrl_Dispose(obj); }

public:
  virtual int GetType();
  virtual BOOL Init(int device, int module = 0)=0;
  virtual void DeInit() = 0;
  virtual BOOL Config(tagCtrlParam* param) = 0;
  virtual BOOL Start(tagCtrlParam* param) = 0;
  virtual BOOL Stop() = 0;
  virtual BOOL Read(tagCtrlParam* param) { return FALSE; }
  virtual void Clear(tagCtrlParam* param) { return; }
};

