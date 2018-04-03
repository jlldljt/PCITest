#include "StdAfx.h"
#include "PWModulator.h"


CPWModulator::CPWModulator()
{
  m_pwModulatorCtrl = NULL;
  m_pwModulatorCtrl = PwModulatorCtrl::Create();
}


CPWModulator::~CPWModulator()
{
  m_pwModulatorCtrl->Dispose();
}

BOOL CPWModulator::Init(int device, int module)
{
  BOOL ret = FALSE;
  PwModulatorCtrl* pwModulatorCtrl = PwModulatorCtrl::Create();
  Array<DeviceTreeNode>* supportedDevices = pwModulatorCtrl->getSupportedDevices();
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
        ErrorCode errorCode = pwModulatorCtrl->setSelectedDevice(devInfo);
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
        int channelCountMax = pwModulatorCtrl->getFeatures()->getChannelCountMax();//8
        Array<CounterCapability> * counterCap;
        int itemCount;
        m_channels.RemoveAll();
        for (int i = 0; i < channelCountMax; i++)
        {
          itemCount = pwModulatorCtrl->getFeatures()->getCapabilities()->getItem(i)->getCount();//支持类型数量
          counterCap = pwModulatorCtrl->getFeatures()->getCapabilities()->getItem(i);
          for (int j = 0; j < itemCount; ++j)
          {
            if (InstantPwmOut == counterCap->getItem(j))
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
  pwModulatorCtrl->Dispose();

  return ret;
}

void CPWModulator::DeInit()
{
  m_modules.RemoveAll();
  m_channels.RemoveAll();
}

BOOL CPWModulator::Config(tagCtrlParam * param)
{
  tagCtrlParam* tpParam = param;
  ErrorCode	errorCode;
  DeviceInformation devInfo(tpParam->deviceNumber);//设备号
  errorCode = m_pwModulatorCtrl->setSelectedDevice(devInfo);

  if (BioFailed(errorCode)) {
    return FALSE;
  }

  m_pwModulatorCtrl->getSelectedDevice(devInfo);

  TRACE(_T("PWM Output - Run( %s )"), devInfo.Description);

  errorCode = m_pwModulatorCtrl->setChannelCount(1);//通道数量，此ctrl控制从起始通道开始的1个通道

  if (BioFailed(errorCode)) {
    return FALSE;
  }

  errorCode = m_pwModulatorCtrl->setChannelStart(tpParam->channel);//起始通道

  if (BioFailed(errorCode)) {
    return FALSE;
  }
  //默认配置
  m_pwModulatorCtrl->getChannels()->getItem(tpParam->channel).setGated(false);//关闭gate
  //m_eventCounterCtrl->getChannels()->getItem(tpParam->channel).setClockPolarity(Positive);//设置极性

  return TRUE;
}
//param0 高电平宽度
//param1 低电平宽度
BOOL CPWModulator::Start(tagCtrlParam * param)
{
  tagCtrlParam* tpParam = param;
  ErrorCode errorCode;
  PulseWidth pulseWidth = { tpParam->param0, tpParam->param1 };
  
  int channelStart = m_pwModulatorCtrl->getChannelStart();
  int channelCount = m_pwModulatorCtrl->getChannelCount();
  for (int i = channelStart; i < channelCount + channelStart; i++)
  {
    errorCode = m_pwModulatorCtrl->getChannels()->getItem(i).setPulseWidth(pulseWidth);
    if (BioFailed(errorCode)) {
      return FALSE;
    }
    m_pwModulatorCtrl->getChannels()->getItem(i).getPulseWidth(pulseWidth);
  }

  errorCode = m_pwModulatorCtrl->setEnabled(true);
  if (BioFailed(errorCode)) {
    return FALSE;
  }
  tpParam->param0 = pulseWidth.HiPeriod;
  tpParam->param1 = pulseWidth.LoPeriod;

  TRACE("PWMODULATOR real hiperiod %d loperiod", tpParam->param0, tpParam->param1);
  return TRUE;
}

BOOL CPWModulator::Stop()
{
  ErrorCode errorCode = m_pwModulatorCtrl->setEnabled(false);
  if (BioFailed(errorCode)) {
    return FALSE;
  }
  return TRUE;
}
