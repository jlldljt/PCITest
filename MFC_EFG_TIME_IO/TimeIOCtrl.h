#pragma once
#include "Inc\bdaqctrl.h"
using namespace Automation::BDaq;

#ifndef UNICODE
TCHAR const * WCHAR_TO_TCHAR(WCHAR const * in, TCHAR * out)
{
  WideCharToMultiByte(CP_OEMCP, NULL, in, -1, out, MAX_DEVICE_DESC_LEN, NULL, FALSE);
  return out;
}
#else
TCHAR const * WCHAR_TO_TCHAR(WCHAR const * in, TCHAR * out)
{
  wcscpy_s(out, wcslen(in) + 1, in);
  return out;
}
#endif

#ifndef UNICODE
WCHAR const * TCHAR_TO_WCHAR(TCHAR const * in, WCHAR* out)
{
  MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)in, -1, out, MAX_DEVICE_DESC_LEN);
  return out;
}
#else
WCHAR const * TCHAR_TO_WCHAR(TCHAR const * in, WCHAR* out)
{
  wcscpy_s(out, wcslen(in) + 1, in);
  return out;
}
#endif
typedef struct tagDevConfParam
{
  int deviceNumber;
  long channel;
  WCHAR profilePath[256];
}DevConfParam, *PDevConfParam;
class CTimeIOCtrl
{
public:
	CTimeIOCtrl(void);
	~CTimeIOCtrl(void);

private:
  DevConfParam       m_confParam; // the device's configure information from config form.
  OneShotCtrl*       m_oneShotCtrl; // ÑÓ³ÙÂö³å
  EventCounterCtrl*  m_eventCounterCtrl; // ¼ÆÊýÆ÷
  PwModulatorCtrl*     m_pwModulatorCtrl; // PWM
  InstantDiCtrl *      m_instantDiCtrl; // DIÖÐ¶Ï
public:
  void OneShotTest();
  void EventCounterTest();
  void PwModulatorTest();
  void InstantDiTest();
  // this function is used to deal with "Interrupt"Event.
  static void  BDAQCALL OnDiSnapEvent(void * sender, DiSnapEventArgs * args, void * userParam);
};

