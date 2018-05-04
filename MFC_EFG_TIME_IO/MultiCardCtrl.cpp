//特殊功能模块，非基础模块
//以配置文件为基础，实现多张卡同时运行


#include "StdAfx.h"
#include "MultiCardCtrl.h"
#include "ParamConfig.h"


CMultiCardCtrl::CMultiCardCtrl()
{
}


CMultiCardCtrl::~CMultiCardCtrl()
{
}

int CMultiCardCtrl::LoadDI(int no, int cardSel, DevInf& dev)
{
  ASSERT(no >= 0 && no < MAX_CHANNEL_NUM);
  CParamConfig config;
  CString str;
  str.Format(L"DI%d", no);
  CString path = L"config/" + dev.description + L".xml";
  char  ppath[50], pstr[50];
  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)path, -1, ppath, 50, 0, false);
  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)str, -1, pstr, 50, 0, false);
  if (0 == config.Create(ppath, pstr))
  {
    ConfigParam* param = config.ReadParam();
    if (!param)
      return -1;

    if (param->start && param->device == dev.deviceNumber) //启动状态
    {
      m_card[cardSel].StopDi(no);
      m_card[cardSel].DeleteDi(no);
      m_card[cardSel].CreateDi(no, (TimeIOType)param->comboData, param->device);
      m_card[cardSel].StartDi(no, param->device, param->param0, param->param1);
    }
    else {
      m_card[cardSel].StopDi(no);
      m_card[cardSel].DeleteDi(no);
    }
    delete param;
  }
  return 0;
}

int CMultiCardCtrl::LoadT0(int no, int cardSel, DevInf& dev)
{
  ASSERT(no >= 0 && no < MAX_CHANNEL_NUM);
  CParamConfig config;
  CString str;
  str.Format(L"T0%d", no);
  CString path = L"config/" + dev.description + L".xml";
  char  ppath[50], pstr[50];
  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)path, -1, ppath, 50, 0, false);
  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)str, -1, pstr, 50, 0, false);
  if (0 == config.Create(ppath, pstr))
  {
    ConfigParam* param = config.ReadParam();
    if (!param)
      return -1;

    if (param->start && param->device == dev.deviceNumber) //启动状态
    {
      m_card[cardSel].StopT0(no);
      m_card[cardSel].DeleteT0(no);
      m_card[cardSel].CreateT0(no, (TimeIOType)param->comboData, param->device);
      m_card[cardSel].StartT0(no, param->device, param->param0, param->param1);
    }
    else {
      m_card[cardSel].StopT0(no);
      m_card[cardSel].DeleteT0(no);
    }
    delete param;
  }

  return 0;
}

int CMultiCardCtrl::LoadT1(int no, int cardSel, DevInf& dev)
{
  ASSERT(no >= 0 && no < MAX_CHANNEL_NUM);
  CParamConfig config;
  CString str;
  str.Format(L"T1%d", no);
  CString path = L"config/" + dev.description + L".xml";
  char  ppath[50], pstr[50];
  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)path, -1, ppath, 50, 0, false);
  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)str, -1, pstr, 50, 0, false);
  if (0 == config.Create(ppath, pstr))
  {
    ConfigParam* param = config.ReadParam();
    if (!param)
      return -1;

    if (param->start && param->device == dev.deviceNumber) //启动状态
    {
      m_card[cardSel].StopT1(no);
      m_card[cardSel].DeleteT1(no);
      m_card[cardSel].CreateT1(no, (TimeIOType)param->comboData, param->device);
      m_card[cardSel].StartT1(no, param->device, param->param0);
    }
    else {
      m_card[cardSel].StopT1(no);
      m_card[cardSel].DeleteT1(no);
    }
    delete param;
  }
  return 0;
}

int CMultiCardCtrl::Load(const int cardSel)
{
  CParamConfig m_config;
  DevInf dev;

  if (cardSel < 0 || MAX_CARD_NUM <= cardSel)
    return -1;

  int count = m_card[cardSel].getDevices();
  int ret = m_card[cardSel].getDevice(cardSel, dev);

  for (int i = 0; i < MAX_CHANNEL_NUM; i++) {
    LoadDI(i, cardSel, dev);
    LoadT0(i, cardSel, dev);
    LoadT1(i, cardSel, dev);
  }

  return 0;
}

int CMultiCardCtrl::Stop(const int cardSel)
{
  CParamConfig m_config;
  DevInf dev;

  if (cardSel < 0 || MAX_CARD_NUM <= cardSel)
    return -1;

  for (int i = 0; i < MAX_CHANNEL_NUM; i++) {
    m_card[cardSel].StopDi(i);
    m_card[cardSel].DeleteDi(i);
    m_card[cardSel].StopT0(i);
    m_card[cardSel].DeleteT0(i);
    m_card[cardSel].StopT1(i);
    m_card[cardSel].DeleteT1(i);
  }

  return 0;
}

