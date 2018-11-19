#include "StdAfx.h"
#include "TimerPulse.h"


CTimerPulse::CTimerPulse()
{
  m_timePulseCtrl = NULL;
  m_timePulseCtrl = TimerPulseCtrl::Create();
}


CTimerPulse::~CTimerPulse()
{
  m_timePulseCtrl->Dispose();
}



//根据几号设备的几号模块进行初始化
//返回FALSE，初始化失败
//初始化m_modules m_channels
BOOL CTimerPulse::Init(int device, int module)
{
  BOOL ret = FALSE;
  TimerPulseCtrl* timePulseCtrl = TimerPulseCtrl::Create();
  //timePulseCtrl = TimerPulseCtrl::Create();
  Array<DeviceTreeNode>* supportedDevices = timePulseCtrl->getSupportedDevices();
  if (supportedDevices != NULL)
  {
    for (int i = 0; i < supportedDevices->getCount(); ++i)
    {
      DeviceTreeNode devNote = supportedDevices->getItem(i);
      if (devNote.DeviceNumber == device)
      {
        //模块
        m_modules.RemoveAll();
        int index = 0;
        for (int j = 0; j < 8; ++j)
        {
          index = devNote.ModulesIndex[j];
          if (index != -1)
          {
            m_modules.Add(index);
          }
          else {
            break;
          }
        }


        DeviceInformation devInfo(device/*, devNote.Description*/, ModeWriteWithReset, module);
        //else 
        //  DeviceInformation devInfo(devNote.Description);

        ErrorCode errorCode = timePulseCtrl->setSelectedDevice(devInfo);
        if (errorCode != 0) {
          CString str;
          TCHAR des[MAX_DEVICE_DESC_LEN];
          TRACE("Error: the error code is 0x % x, The %s is busy or not exit in computer now.\n\
              Select other device please!", errorCode, WCHAR_TO_TCHAR(devNote.Description, des));
          /*str.Format(_T("Error: the error code is 0x%x, The %s is busy or not exit in computer now.\n\
Select other device please!"), errorCode, WCHAR_TO_TCHAR(devNote.Description, des));
           AfxMessageBox(str);*/

          break;
        }
        else {
          ret = TRUE;
        }


        // Set channel start combo box以及支持的类型，一般是8g
        int channelCountMax = timePulseCtrl->getFeatures()->getChannelCountMax();//8
        Array<CounterCapability> * counterCap;
        int itemCount;
        m_channels.RemoveAll();
        for (int i = 0; i < channelCountMax; i++)
        {
          itemCount = timePulseCtrl->getFeatures()->getCapabilities()->getItem(i)->getCount();//支持类型数量
          counterCap = timePulseCtrl->getFeatures()->getCapabilities()->getItem(i);
          for (int j = 0; j < itemCount; ++j)
          {
            if (TimerPulse == counterCap->getItem(j))
            {
              m_channels.Add(i);
            }
          }
        }

        break;
      }
    }
  }

  if (!ret) {
    m_modules.RemoveAll();
    m_channels.RemoveAll();
  }

  supportedDevices->Dispose();
  timePulseCtrl->Dispose();

  return ret;
}

void CTimerPulse::DeInit()
{
  m_modules.RemoveAll();
  m_channels.RemoveAll();
  //m_timePulseCtrl->Dispose();
}

BOOL CTimerPulse::Config(tagCtrlParam * param)
{
  tagCtrlParam* tpParam = param;
  ErrorCode	errorCode;
  DeviceInformation devInfo(tpParam->deviceNumber);//设备号
  devInfo.ModuleIndex = tpParam->moduleIndex;//模块号
  errorCode = m_timePulseCtrl->setSelectedDevice(devInfo);

  if (BioFailed(errorCode)) {
    return FALSE;
  }
  /*
  errorCode = m_timePulseCtrl->LoadProfile(m_confParam.profilePath);
  */
  m_timePulseCtrl->getSelectedDevice(devInfo);

  TRACE(_T("Pulse Output with Timer Interrupt - Run( %s )"), devInfo.Description);

  errorCode = m_timePulseCtrl->setChannelCount(1);//通道数量，此ctrl控制从起始通道开始的1个通道

  if (BioFailed(errorCode)) {
    return FALSE;
  }

  errorCode = m_timePulseCtrl->setChannelStart(tpParam->channel);//起始通道

  if (BioFailed(errorCode)) {
    return FALSE;
  }
  //默认配置
  m_timePulseCtrl->getChannels()->getItem(tpParam->channel).setGated(false);
  m_timePulseCtrl->getChannels()->getItem(tpParam->channel).setGatePolarity(Positive);
  m_timePulseCtrl->getChannels()->getItem(tpParam->channel).setOutSignal(ToggledFromLow);
  return TRUE;
}

BOOL CTimerPulse::Start(tagCtrlParam * param)
{
  tagCtrlParam* tpParam = param;
  ErrorCode errorCode;
  double frequency = tpParam->param0;

  int channelStart = m_timePulseCtrl->getChannelStart();
  int channelCount = m_timePulseCtrl->getChannelCount();
  for (int i = channelStart; i < channelCount + channelStart; i++)
  {
    errorCode = m_timePulseCtrl->getChannels()->getItem(i).setFrequency(frequency);
    if (BioFailed(errorCode)) {
      return FALSE;
    }
  }

  for (int j = channelStart; j < channelCount + channelStart; j++)
  {
    frequency = m_timePulseCtrl->getChannels()->getItem(j).getFrequency();
    tpParam->param0 = frequency;
  }
  errorCode = m_timePulseCtrl->setEnabled(true);
  if (BioFailed(errorCode)) {
    return FALSE;
  }

  TRACE("TIMER PULSE real frequency %d", tpParam->param0);
  return TRUE;
}

BOOL CTimerPulse::Stop()
{
  ErrorCode errorCode = m_timePulseCtrl->setEnabled(false);
  if (BioFailed(errorCode)) {
    return FALSE;
  }
  return TRUE;
}




