#include "StdAfx.h"
#include "OneShot.h"


COneShot::COneShot(void)
{
  m_oneShotCtrl = NULL;
  m_oneShotCtrl = OneShotCtrl::Create();
}


COneShot::~COneShot(void)
{
  m_oneShotCtrl->Dispose();
}

BOOL COneShot::Init(int device, int module)
{
	BOOL ret = FALSE;
  OneShotCtrl * oneShotCtrl = OneShotCtrl::Create();
  Array<DeviceTreeNode>* supportedDevices = oneShotCtrl->getSupportedDevices();
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

        DeviceInformation devInfo(device, ModeWriteWithReset);
        ErrorCode errorCode = oneShotCtrl->setSelectedDevice(devInfo);
        if (errorCode != 0) {
          CString str;
          TCHAR des[MAX_DEVICE_DESC_LEN];
          TRACE("Error: the error code is 0x % x, The %s is busy or not exit in computer now.\n\
              Select other device please!", errorCode, WCHAR_TO_TCHAR(devNote.Description, des));
          break;
        }
        else {
          ret = TRUE;
        }

        // Set channel start combo box以及支持的类型，一般是8g
        int channelCountMax = oneShotCtrl->getFeatures()->getChannelCountMax();//8
        Array<CounterCapability> * counterCap;
        int itemCount;
        m_channels.RemoveAll();
        for (int i = 0; i < channelCountMax; i++)
        {
          itemCount = oneShotCtrl->getFeatures()->getCapabilities()->getItem(i)->getCount();//支持类型数量
          counterCap = oneShotCtrl->getFeatures()->getCapabilities()->getItem(i);
          for (int j = 0; j < itemCount; ++j)
          {
            if (OneShot == counterCap->getItem(j))
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
  oneShotCtrl->Dispose();

  return ret;
}

void COneShot::DeInit()
{
  m_modules.RemoveAll();
  m_channels.RemoveAll();
}

BOOL COneShot::Config(tagCtrlParam* param)
{
  tagCtrlParam* tpParam = param;
  ErrorCode	errorCode;
  DeviceInformation devInfo(tpParam->deviceNumber);//设备号
  errorCode = m_oneShotCtrl->setSelectedDevice(devInfo);

  if (BioFailed(errorCode)) {
    return FALSE;
  }

  m_oneShotCtrl->getSelectedDevice(devInfo);

  TRACE(_T("One Shot - Run( %s )"), devInfo.Description);

  errorCode = m_oneShotCtrl->setChannelCount(1);//通道数量，此ctrl控制从起始通道开始的1个通道

  if (BioFailed(errorCode)) {
    return FALSE;
  }

  errorCode = m_oneShotCtrl->setChannelStart(tpParam->channel);//起始通道

  if (BioFailed(errorCode)) {
    return FALSE;
  }
  //默认配置
  SignalDrop signalClock = m_oneShotCtrl->getChannels()->getItem(tpParam->channel).getClockSource();
  m_oneShotCtrl->getChannels()->getItem(tpParam->channel).setClockPolarity(Positive);
  SignalDrop signalGate = m_oneShotCtrl->getChannels()->getItem(tpParam->channel).getGateSource();
  m_oneShotCtrl->getChannels()->getItem(tpParam->channel).setGatePolarity(Positive);
  if(tpParam->param1)
    m_oneShotCtrl->getChannels()->getItem(tpParam->channel).setOutSignal(PositivePulse);
  else
    m_oneShotCtrl->getChannels()->getItem(tpParam->channel).setOutSignal(NegativePulse);

  return TRUE;
}

BOOL COneShot::Start(tagCtrlParam* param)
{
  tagCtrlParam* tpParam = param;
  ErrorCode errorCode;
  MathInterval valueRange;
  m_oneShotCtrl->getFeatures()->getOsDelayCountRange(valueRange);
  double rangeMax = valueRange.Max;
  double rangeMin = valueRange.Min;

  if (tpParam->param0 > rangeMax || tpParam->param0 < rangeMin)
  {
    CString str(L"DelayCount is out of range:");
    CString str1;
    str1.Format(_T("%2.0lf - %2.0lf"),rangeMin,rangeMax);
    CString error = str + str1;
    AfxMessageBox(error);
    return FALSE;
  }
  errorCode = m_oneShotCtrl->setDelayCount(tpParam->param0);
  if (BioFailed(errorCode)) {
    return FALSE;
  }
  int channelStart = m_oneShotCtrl->getChannelStart();
  int channelCount = m_oneShotCtrl->getChannelCount();
  for (int32 i = channelStart; i < channelStart + channelCount; i++)
  {
    errorCode = m_oneShotCtrl->getChannels()->getItem(i).setDelayCount(tpParam->param0);
    if (BioFailed(errorCode)) {
      return FALSE;
    }
  }
  errorCode = m_oneShotCtrl->setEnabled(true);
  if (BioFailed(errorCode)) {
    return FALSE;
  }
  return TRUE;
}

BOOL COneShot::Stop()
{
  ErrorCode errorCode = m_oneShotCtrl->setEnabled(false);
  if (BioFailed(errorCode)) {
    return FALSE;
  }
  return TRUE;
}

