//特殊功能模块，非基础模块
//以配置文件为基础，实现多张卡同时运行


#include "StdAfx.h"
#include "MultiCardCtrl.h"
#include "ParamConfig.h"


CMultiCardCtrl::CMultiCardCtrl()
{
  m_xray_cardsel = -1;
  m_laser_cardsel = -1;
  for (int i = 0; i < MAX_CARD_NUM; i++) {
    m_card[i] = NULL;// CPCICtrl::Create(PCI1780U);// new CTimeIOCtrl;
  }
}


CMultiCardCtrl::~CMultiCardCtrl()
{
  for (int i = 0; i < MAX_CARD_NUM; i++) {
    CPCICtrl::Delete(m_card[i]);
  }
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
      m_card[cardSel]->StopDi(no);
      m_card[cardSel]->DeleteDi(no);
      if(TRUE==m_card[cardSel]->CreateDi(no, (TimeIOType)param->comboData, param->device))
      m_card[cardSel]->StartDi(no, param->device, param->param0, param->param1);
    }
    else {
      m_card[cardSel]->StopDi(no);
      m_card[cardSel]->DeleteDi(no);
    }
    delete param;
  }
  return 0;
}

int CMultiCardCtrl::LoadDO(int no, int cardSel, DevInf& dev)
{
  ASSERT(no >= 0 && no < MAX_CHANNEL_NUM);
  CParamConfig config;
  CString str;
  str.Format(L"DO%d", no);
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
      m_card[cardSel]->StopDO(no);
      m_card[cardSel]->DeleteDO(no);
      if(TRUE==m_card[cardSel]->CreateDO(no, (TimeIOType)param->comboData, param->device))
      m_card[cardSel]->StartDO(no, param->device, param->param0);
    }
    else {
      m_card[cardSel]->StopDO(no);
      m_card[cardSel]->DeleteDO(no);
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
      m_card[cardSel]->StopT0(no);
      m_card[cardSel]->DeleteT0(no);
      if(TRUE==m_card[cardSel]->CreateT0(no, (TimeIOType)param->comboData, param->device))
      m_card[cardSel]->StartT0(no, param->device, param->param0, param->param1);
    }
    else {
      m_card[cardSel]->StopT0(no);
      m_card[cardSel]->DeleteT0(no);
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
      m_card[cardSel]->StopT1(no);
      m_card[cardSel]->DeleteT1(no);
      if(TRUE==m_card[cardSel]->CreateT1(no, (TimeIOType)param->comboData, param->device))
      m_card[cardSel]->StartT1(no, param->device, param->param0, param->param1);
    }
    else {
      m_card[cardSel]->StopT1(no);
      m_card[cardSel]->DeleteT1(no);
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

  int count = CPCICtrl::m_num;// m_card[cardSel]->getDevices();
  int ret = /*m_card[cardSel]->*/CPCICtrl::getDevice(cardSel, dev);

  if (!m_card[cardSel]) {
   // CPCICtrl::Delete(m_card[cardSel]);
    m_card[cardSel] = CPCICtrl::Create(dev.type);
  }

  for (int i = 0; i < MAX_CHANNEL_NUM; i++) {
    LoadDI(i, cardSel, dev);
    LoadDO(i, cardSel, dev);
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
    m_card[cardSel]->StopDi(i);
    m_card[cardSel]->DeleteDi(i);
	m_card[cardSel]->StopDO(i);
    m_card[cardSel]->DeleteDO(i);
    m_card[cardSel]->StopT0(i);
    m_card[cardSel]->DeleteT0(i);
    m_card[cardSel]->StopT1(i);
    m_card[cardSel]->DeleteT1(i);
  }

  return 0;
}
/////////////////////////////////////////////////////////////
/////////////////////测量所需要的函数////////////////////////

//// 开启卡的功能
//void CMultiCardCtrl::StartXrayOneshot(const int cardSel)
//{
//  DevInf dev;
//  m_xray_cardsel = cardSel;
//  int count = CPCICtrl::m_num;// m_card[cardSel]->getDevices();
//  int ret = /*m_card[cardSel]->*/CPCICtrl::getDevice(cardSel, dev);
//  if (1 != ret)
//    return;
//  // 开启一次捕捉
//  m_diInt[cardSel].BindCard(dev.deviceNumber, m_card[cardSel], NULL);
//  m_diInt[cardSel].StartDiIntXRayOneShot(0);
//}
//// 开启卡的功能
//void CMultiCardCtrl::StartLaserSin(const int cardSel)
//{
//  DevInf dev;
//  m_laser_cardsel = cardSel;
//  int count = CPCICtrl::m_num;// m_card[cardSel]->getDevices();
//  int ret = /*m_card[cardSel]->*/CPCICtrl::getDevice(cardSel, dev);
//  if (1 != ret)
//    return;
//  // 开启一次捕捉
//  m_diInt[cardSel].BindCard(dev.deviceNumber, m_card[cardSel], NULL);
//  m_diInt[cardSel].StartDiIntLaserSin(0);
//}
//
//// 开启一次测量 具体CDiIntCounterSnap负责
//void CMultiCardCtrl::MeasureStart(double out3_delay/*out3 第几行*/, double out6_delay/*out6 第几行*/)
//{
//  if (m_xray_cardsel == -1 || m_laser_cardsel == -1)
//    return;
//#ifdef __DEBUG__
//  m_diInt[m_laser_cardsel].StartCaptureSin(0, out3_delay, 0, out6_delay);// TODO：应该时5和4
//#else
//  m_diInt[m_laser_cardsel].StartCaptureSin(OUT3_COUNTER, out3_delay, OUT6_COUNTER, out6_delay);// TODO：应该时5和4
//#endif
//  m_diInt[m_xray_cardsel].StartCaptureXRayOneShot();
//}


int CMultiCardCtrl::LoadParam(CString type,int no, const int cardSel, double& param0,double& param1)
{
  ASSERT(no >= 0 && no < MAX_CHANNEL_NUM);
  ASSERT(type == L"T0" || type == L"T1" || type == L"DO" || type == L"DI");
  ASSERT(cardSel >= 0 || MAX_CARD_NUM > cardSel);

  DevInf dev;
  int count = CPCICtrl::m_num;// m_card[cardSel]->getDevices();
  int ret = /*m_card[cardSel]->*/CPCICtrl::getDevice(cardSel, dev);

  //if (!m_card[cardSel]) {
  //  // CPCICtrl::Delete(m_card[cardSel]);
  //  m_card[cardSel] = CPCICtrl::Create(dev.type);
  //}

  CParamConfig config;
  CString str;
  str.Format(L"%d", no);
  str = type + str;
  CString path = L"config/" + dev.description + L".xml";
  char  ppath[50], pstr[50];
  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)path, -1, ppath, 50, 0, false);
  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)str, -1, pstr, 50, 0, false);
  if (0 == config.Create(ppath, pstr))
  {
    ConfigParam* param = config.ReadParam();
    if (!param)
      return -1;

    param0 = param->param0;
    param1 = param->param1;

    delete param;
  }
  return 0;

}

int CMultiCardCtrl::SaveParam(CString type, int no, const int cardSel,double param0, double param1)
{
  ASSERT(no >= 0 && no < MAX_CHANNEL_NUM);
  ASSERT(type == L"T0" || type == L"T1" || type == L"DO" || type == L"DI");
  ASSERT(cardSel >= 0 || MAX_CARD_NUM > cardSel);

  DevInf dev;
  int count = CPCICtrl::m_num;// m_card[cardSel]->getDevices();
  int ret = /*m_card[cardSel]->*/CPCICtrl::getDevice(cardSel, dev);

  //if (!m_card[cardSel]) {
  //  // CPCICtrl::Delete(m_card[cardSel]);
  //  m_card[cardSel] = CPCICtrl::Create(dev.type);
  //}

  CParamConfig config;
  CString str;
  str.Format(L"%d", no);
  str = type + str;
  CString path = L"config/" + dev.description + L".xml";
  char  ppath[50], pstr[50];
  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)path, -1, ppath, 50, 0, false);
  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)str, -1, pstr, 50, 0, false);

  ConfigParam* param = NULL;
  if (0 == config.Create(ppath, pstr))
  {
    param = config.ReadParam();
    if (!param)
      return -1;
    param0 = param->param0;
    param1 = param->param1;
    param->param0 = param0;
    param->param1 = param1;
    config.SaveParam(*param);
    delete param;
  }

}