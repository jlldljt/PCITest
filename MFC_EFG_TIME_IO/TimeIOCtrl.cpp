#include "StdAfx.h"
#include "TimeIOCtrl.h"


void CTimeIOCtrl::OnDiSnapEvent(void * sender, DiSnapEventArgs * args, void * userParam)
{
  CTimeIOCtrl * uParam = (CTimeIOCtrl *)userParam;
  //uParam->m_eventCount++;
}

CTimeIOCtrl::CTimeIOCtrl(void)
{
  memset(&m_confParam, 0, sizeof(m_confParam));
  m_oneShotCtrl = OneShotCtrl::Create();
  m_eventCounterCtrl = EventCounterCtrl::Create();
  m_pwModulatorCtrl = PwModulatorCtrl::Create();
  m_instantDiCtrl = InstantDiCtrl::Create();
  m_instantDiCtrl->addInterruptHandler(OnDiSnapEvent, this);//di中断回调
  m_selectedDeviceNumber = -1;
  devices.RemoveAll();
}


CTimeIOCtrl::~CTimeIOCtrl(void)
{
  m_oneShotCtrl->Dispose();
  m_eventCounterCtrl->Dispose();
  m_pwModulatorCtrl->Dispose();
}
int CTimeIOCtrl::CreateT0(int no, int type)
{
  return 0;
}
//返回设备数量
int CTimeIOCtrl::getDevices()
{
 

  devices.RemoveAll();
  Array<DeviceTreeNode>* sptedDevices = DeviceCtrl::getInstalledDevices();
  int count = sptedDevices->getCount();
  if (count == 0)
  {
    AfxMessageBox(_T("No device to support the currently demonstrated function!"));
    sptedDevices->Dispose();
    return -1;
  }
  else {
    for (int i = 0; i < count; ++i)
    {
      DeviceTreeNode const & node = sptedDevices->getItem(i);
      TRACE("%d, %s\n", node.DeviceNumber, node.Description);
      DevInf dev = { node.DeviceNumber, node.Description };
      devices.Add(dev);
    }
    sptedDevices->Dispose();
    return count;
  }
}

int CTimeIOCtrl::getDevice(int no, DevInf& devInf)
{
  int count = devices.GetCount();
  if (no < count) {
    devInf = devices.GetAt(no);
    return 1;
  }
  else {
    return -1;
  }
}

//参考官方例程的Counter_DelayedPulseGeneration、
//延迟脉冲的测试，行同步产生帧同步
void CTimeIOCtrl::OneShotTest()
{
  //初始化列表
  OneShotCtrl * oneShotCtrl = OneShotCtrl::Create();
  Array<DeviceTreeNode>* sptedDevices = oneShotCtrl->getSupportedDevices();
  int count = sptedDevices->getCount();
  CString des;
  for (int i = 0; i < sptedDevices->getCount(); ++i)
  {
    DeviceTreeNode const & node = sptedDevices->getItem(i);
    TRACE("%d, %s\n", node.DeviceNumber, node.Description);
    des = node.Description;
  }
  sptedDevices->Dispose();

  //oneShotCtrl->Dispose();
  //切换卡，一般用一次
  //oneShotCtrl = OneShotCtrl::Create();
  wchar_t w_des[MAX_DEVICE_DESC_LEN];
  DeviceInformation devInfo(TCHAR_TO_WCHAR((LPCTSTR)des, w_des));
  ErrorCode errorCode = oneShotCtrl->setSelectedDevice(devInfo);

  int channelCountMax = oneShotCtrl->getFeatures()->getChannelCountMax();
  Array<CounterCapability> * counterCap;
  int itemCount;
  int channel;
  for (int i = 0; i < channelCountMax; i++)
  {
    itemCount = oneShotCtrl->getFeatures()->getCapabilities()->getItem(i)->getCount();
    counterCap = oneShotCtrl->getFeatures()->getCapabilities()->getItem(i);
    for (int j = 0; j < itemCount; ++j)
    {
      if (OneShot == counterCap->getItem(j))
      {
        channel = i;
      }
    }
  }
  m_oneShotCtrl = oneShotCtrl;
  ///configdevice
  errorCode = m_oneShotCtrl->setSelectedDevice(devInfo);
  m_oneShotCtrl->getSelectedDevice(devInfo);
  errorCode = m_oneShotCtrl->setChannelCount(1);
  errorCode = m_oneShotCtrl->setChannelStart(channel);
  //start
  int m_clockCount = 312;
  MathInterval valueRange;
  m_oneShotCtrl->getFeatures()->getOsDelayCountRange(valueRange);//得到支持的delay范围
  double rangeMax = valueRange.Max;
  double rangeMin = valueRange.Min;
  errorCode = m_oneShotCtrl->setDelayCount(m_clockCount);
  int channelStart = m_oneShotCtrl->getChannelStart();
  int channelCount = m_oneShotCtrl->getChannelCount();
  for (int32 i = channelStart; i < channelStart + channelCount; i++)
  {
    errorCode = m_oneShotCtrl->getChannels()->getItem(i).setDelayCount(m_clockCount);
  }
  errorCode = m_oneShotCtrl->setEnabled(true);
  //stop
  errorCode = m_oneShotCtrl->setEnabled(FALSE);

  oneShotCtrl->Dispose();

}

void CTimeIOCtrl::EventCounterTest()
{
  // create available device list.
  EventCounterCtrl* eventCounterCtrl = EventCounterCtrl::Create();
  Array<DeviceTreeNode>* sptedDevices = eventCounterCtrl->getSupportedDevices();
  CString des;
  if (sptedDevices->getCount() == 0)
  {
    return;
  }
  else {
    for (int i = 0; i < sptedDevices->getCount(); ++i)
    {
      DeviceTreeNode const & node = sptedDevices->getItem(i);
      TRACE("%d, %s\n", node.DeviceNumber, node.Description);
      //m_comboBox_Device.AddString(WCHAR_TO_TCHAR(node.Description, des));
      //m_comboBox_Device.SetItemData(i, node.DeviceNumber);
      des = node.Description;
    }
    sptedDevices->Dispose();
  }
  //eventCounterCtrl->Dispose();

  //切换设备
  wchar_t w_des[MAX_DEVICE_DESC_LEN];
  DeviceInformation devInfo(TCHAR_TO_WCHAR((LPCTSTR)des, w_des));
  ErrorCode errorCode = eventCounterCtrl->setSelectedDevice(devInfo);

  int channelCountMax = eventCounterCtrl->getFeatures()->getChannelCountMax();
  Array<CounterCapability> * counterCap;
  int itemCount;
  int counterChanIndex = 0;
  for (int i = 0; i < channelCountMax; i++)
  {
    itemCount = eventCounterCtrl->getFeatures()->getCapabilities()->getItem(i)->getCount();
    counterCap = eventCounterCtrl->getFeatures()->getCapabilities()->getItem(i);
    for (int j = 0; j < itemCount; ++j)
    {
      if (InstantEventCount == counterCap->getItem(j))
      {
        CString str;
        str.Format(_T("%d"), i);
        //m_comboBox_counterChan.AddString(str);
        //m_comboBox_counterChan.SetItemData(counterChanIndex++, i);
      }
    }
  }
  m_eventCounterCtrl = eventCounterCtrl;
  //start
  errorCode = m_eventCounterCtrl->setEnabled(true);
  //stop
  errorCode = m_eventCounterCtrl->setEnabled(FALSE);
  //read
  errorCode = m_eventCounterCtrl->setEnabled(TRUE);
  int32 value = 0;
  m_eventCounterCtrl->Read(1, &value);
  //config
//  DeviceInformation devInfo(m_confParam.deviceNumber);//combox GetItemData得到
  errorCode = m_eventCounterCtrl->setSelectedDevice(devInfo);
  //errorCode = m_eventCounterCtrl->LoadProfile(m_confParam.profilePath);

  m_eventCounterCtrl->getSelectedDevice(devInfo);
  errorCode = m_eventCounterCtrl->setChannelCount(1);
  errorCode = m_eventCounterCtrl->setChannelStart(m_confParam.channel);//combox GetItemData得到

  eventCounterCtrl->Dispose();
}

void CTimeIOCtrl::PwModulatorTest()
{
  // create available device list.
  PwModulatorCtrl* pwModulatorCtrl = PwModulatorCtrl::Create();
  Array<DeviceTreeNode>* sptedDevices = pwModulatorCtrl->getSupportedDevices();
  CString des;
  if (sptedDevices->getCount() == 0)
  {
    return;
  }
  else {
    for (int i = 0; i < sptedDevices->getCount(); ++i)
    {
      DeviceTreeNode const & node = sptedDevices->getItem(i);
      TRACE("%d, %s\n", node.DeviceNumber, node.Description);
      des = node.Description;
      //TCHAR des[MAX_DEVICE_DESC_LEN];
      //m_comboBox_Device.AddString(WCHAR_TO_TCHAR(node.Description, des));
      //m_comboBox_Device.SetItemData(i, node.DeviceNumber);
    }
    sptedDevices->Dispose();
  }
  //init combo
  wchar_t w_des[MAX_DEVICE_DESC_LEN];
  DeviceInformation devInfo(TCHAR_TO_WCHAR((LPCTSTR)des, w_des));
  ErrorCode errorCode = pwModulatorCtrl->setSelectedDevice(devInfo);
  
  int channelCountMax = pwModulatorCtrl->getFeatures()->getChannelCountMax();
  Array<CounterCapability> * counterCap;
  int itemCount;
  int counterChanIndex = 0;
  for (int i = 0; i < channelCountMax; i++)
  {
    itemCount = pwModulatorCtrl->getFeatures()->getCapabilities()->getItem(i)->getCount();
    counterCap = pwModulatorCtrl->getFeatures()->getCapabilities()->getItem(i);
    for (int j = 0; j < itemCount; ++j)
    {
      if (InstantPwmOut == counterCap->getItem(j))
      {
        CString str;
        str.Format(_T("%d"), i);
        //m_comboBox_counterChan.AddString(str);
        //m_comboBox_counterChan.SetItemData(counterChanIndex++, i);
      }
    }
  }
  //start
//  ErrorCode errorCode;
  PulseWidth pulseWidth = { 0.08,0.02 };
  for (int i = m_pwModulatorCtrl->getChannelStart(); i < m_pwModulatorCtrl->getChannelStart() + m_pwModulatorCtrl->getChannelCount(); i++)
  {
    errorCode = m_pwModulatorCtrl->getChannels()->getItem(i).setPulseWidth(pulseWidth);
  }

  errorCode = m_pwModulatorCtrl->setEnabled(true);

  //stop
//  ErrorCode errorCode;
  errorCode = m_pwModulatorCtrl->setEnabled(false);
  //config
//  ErrorCode	errorCode;
  //DeviceInformation devInfo(m_confParam.deviceNumber);
  errorCode = m_pwModulatorCtrl->setSelectedDevice(devInfo);
  errorCode = m_pwModulatorCtrl->LoadProfile(m_confParam.profilePath);

  m_pwModulatorCtrl->getSelectedDevice(devInfo);
  errorCode = m_pwModulatorCtrl->setChannelCount(1);
  errorCode = m_pwModulatorCtrl->setChannelStart(m_confParam.channel);

  pwModulatorCtrl->Dispose();
  
}

int m_picBoxStatus[32];
int m_portNumList[256];
int m_enabledChannel[16];
byte *m_diIntChannl;

int m_enabledCount = 0;
int m_portNum0 = -1;
int m_diINTPortCount = 0;
ErrorCode SetINTStatus(InstantDiCtrl * instantDiCtrl)
{
  ErrorCode errorCode = Success;
   m_enabledCount = 0;
   m_portNum0 = -1;
   m_diINTPortCount = 0;
  int i = 0;
  for (int k = 0; k < 256; k++)
  {
    m_portNumList[k] = -1;
    if (k < 32)
    {
      m_picBoxStatus[k] = -1;
    }
    if (k < 16)
    {
      m_enabledChannel[k] = -1;
    }
  }

  int portsCount = instantDiCtrl->getPortCount();
  Array<DiintChannel>* interruptChans = instantDiCtrl->getDiintChannels();
  if (m_diIntChannl != NULL)
  {
    delete[]m_diIntChannl;
    m_diIntChannl = NULL;
  }

  int INTChanCount = interruptChans->getCount();
  m_diIntChannl = new byte[portsCount * 8];
  memset(m_diIntChannl, 0, portsCount * 8);

  for (i = 0; i < INTChanCount; ++i)
  {
    m_diIntChannl[interruptChans->getItem(i).getChannel()] = 1;
  }

  bool isINTPort = false;
  for (i = 0; i < portsCount; i++)
  {
    isINTPort = false;
    for (int j = 0; j<8; j++)
    {
      if (1 == m_diIntChannl[i * 8 + j])
      {
        isINTPort = true;
        break;
      }
    }
    if (true == isINTPort)
    {
      m_portNumList[m_diINTPortCount] = i;
      m_diINTPortCount++;
    }
  }

  int portCount = 0;
  if (m_diINTPortCount < 5)
  {
    portCount = m_diINTPortCount;
  }
  else
  {
    if (m_diINTPortCount >5 || 5 == m_diINTPortCount)
    {
      portCount = 4;
    }
  }

  interruptChans->Dispose();
  return  errorCode;
}

void CTimeIOCtrl::InstantDiTest()
{
  // create available device list.
  InstantDiCtrl * instantDiCtrl = InstantDiCtrl::Create();
  Array<DeviceTreeNode>* sptedDevices = instantDiCtrl->getSupportedDevices();

  if (sptedDevices->getCount() == 0)
  {
    return;
  }
  else {
    int deviceCount = 0;
    for (int i = 0; i < sptedDevices->getCount(); ++i)
    {

      DeviceTreeNode const & node = sptedDevices->getItem(i);
      TRACE("%d, %s\n", node.DeviceNumber, node.Description);

      DeviceInformation devInfo(node.DeviceNumber, ModeRead);
      instantDiCtrl->setSelectedDevice(devInfo);
      Array<DiintChannel>* INTChannels = instantDiCtrl->getDiintChannels();
      if (INTChannels == NULL)
      {
        continue;
      }
      //TCHAR des[MAX_DEVICE_DESC_LEN];
      //m_comboBox_Device.AddString(WCHAR_TO_TCHAR(node.Description, des));
      //m_comboBox_Device.SetItemData(deviceCount++, node.DeviceNumber);
    }
    sptedDevices->Dispose();
  }
  instantDiCtrl->Dispose();
  //切换设备
//  InstantDiCtrl * instantDiCtrl = InstantDiCtrl::Create();
  CString des;

  wchar_t w_des[MAX_DEVICE_DESC_LEN];
  DeviceInformation devInfo(TCHAR_TO_WCHAR((LPCTSTR)des, w_des));
  ErrorCode errorCode = instantDiCtrl->setSelectedDevice(devInfo);
 
  if (errorCode != 0) {
    CString str;
    str.Format(_T("Error: the error code is 0x%x, The %s is busy or not exit in computer now.\n\
  Select other device please!"), errorCode, des);
    return;
  }
  SetINTStatus(instantDiCtrl);
  //start
  m_instantDiCtrl->SnapStart();
  //stop
  errorCode = m_instantDiCtrl->SnapStop();
  //config
//  ErrorCode	errorCode;
//  DeviceInformation devInfo(m_confParam.deviceNumber);
  errorCode = m_instantDiCtrl->setSelectedDevice(devInfo);
  errorCode = m_instantDiCtrl->LoadProfile(m_confParam.profilePath);
  m_instantDiCtrl->getSelectedDevice(devInfo);
  CString str;
//  TCHAR des[MAX_DEVICE_DESC_LEN];
//  str.Format(_T("DI Interrupt - Run( %s )"), WCHAR_TO_TCHAR((LPCWSTR)devInfo.Description, des));
  Array<DiintChannel>* interruptChans = m_instantDiCtrl->getDiintChannels();
  int intPortCount = interruptChans->getCount();
  for (int i = 0; i < m_enabledCount; ++i)
  {
    for (int j = 0; j < intPortCount; ++j)
    {
      if (m_enabledChannel[i] == interruptChans->getItem(j).getChannel())
      {
        errorCode = interruptChans->getItem(j).setEnabled(true);
        TRACE("enable: %d, %d\n", i, j);
        break;
      }
    }
  }

  instantDiCtrl->Dispose();
}
