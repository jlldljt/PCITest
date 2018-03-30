#include "StdAfx.h"
#include "EventCounter.h"


CEventCounter::CEventCounter()
{
  m_eventCounterCtrl = NULL;
  m_eventCounterCtrl = EventCounterCtrl::Create();
}


CEventCounter::~CEventCounter()
{
  m_eventCounterCtrl->Dispose();
}

BOOL CEventCounter::Init(int device, int module)
{
  BOOL ret = FALSE;
  EventCounterCtrl* eventCounterCtrl = EventCounterCtrl::Create();
  Array<DeviceTreeNode>* supportedDevices = eventCounterCtrl->getSupportedDevices();
  if (supportedDevices != NULL)
  {
    for (int i = 0; i < supportedDevices->getCount(); ++i)
    {
      DeviceTreeNode devNote = supportedDevices->getItem(i);
      if (devNote.DeviceNumber == device)
      {
        //ģ��
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
        ErrorCode errorCode = eventCounterCtrl->setSelectedDevice(devInfo);
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

        // Set channel start combo box�Լ�֧�ֵ����ͣ�һ����8g
        int channelCountMax = eventCounterCtrl->getFeatures()->getChannelCountMax();//8
        Array<CounterCapability> * counterCap;
        int itemCount;
        m_channels.RemoveAll();
        for (int i = 0; i < channelCountMax; i++)
        {
          itemCount = eventCounterCtrl->getFeatures()->getCapabilities()->getItem(i)->getCount();//֧����������
          counterCap = eventCounterCtrl->getFeatures()->getCapabilities()->getItem(i);
          for (int j = 0; j < itemCount; ++j)
          {
            if (InstantEventCount == counterCap->getItem(j))
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
  eventCounterCtrl->Dispose();

  return ret;
}

void CEventCounter::DeInit()
{
  m_modules.RemoveAll();
  m_channels.RemoveAll();
}

BOOL CEventCounter::Config(tagCtrlParam * param)
{
  tagCtrlParam* tpParam = param;
  ErrorCode	errorCode;
  DeviceInformation devInfo(tpParam->deviceNumber);//�豸��
  errorCode = m_eventCounterCtrl->setSelectedDevice(devInfo);

  if (BioFailed(errorCode)) {
    return FALSE;
  }

  m_eventCounterCtrl->getSelectedDevice(devInfo);

  TRACE(_T("PWM Output - Run( %s )"), devInfo.Description);

  errorCode = m_eventCounterCtrl->setChannelCount(1);//ͨ����������ctrl���ƴ���ʼͨ����ʼ��1��ͨ��

  if (BioFailed(errorCode)) {
    return FALSE;
  }

  errorCode = m_eventCounterCtrl->setChannelStart(tpParam->channel);//��ʼͨ��

  if (BioFailed(errorCode)) {
    return FALSE;
  }
  //Ĭ������
  m_eventCounterCtrl->getChannels()->getItem(tpParam->channel).setGated(true);//��gate
  //m_eventCounterCtrl->getChannels()->getItem(tpParam->channel).setClockPolarity(Positive);//���ü���
  //m_eventCounterCtrl->getChannels()->getItem(tpParam->channel).setGatePolarity(Positive);

  return TRUE;
}

BOOL CEventCounter::Start(tagCtrlParam * param)
{
  tagCtrlParam* tpParam = param;
  ErrorCode errorCode;
  
  errorCode = m_eventCounterCtrl->setEnabled(true);
  if (BioFailed(errorCode)) {
    return FALSE;
  }
  return TRUE;
}

BOOL CEventCounter::Stop()
{
  ErrorCode errorCode = m_eventCounterCtrl->setEnabled(false);
  if (BioFailed(errorCode)) {
    return FALSE;
  }
  return TRUE;
}

BOOL CEventCounter::Read(tagCtrlParam * param)
{
  ASSERT(param);
  ErrorCode errorCode;
  int32 value = 0;
  if (!m_eventCounterCtrl->getEnabled()) {
    return FALSE;
  }
  errorCode = m_eventCounterCtrl->Read(value);
  if (BioFailed(errorCode)) {
    return FALSE;
  }
  param->param0 = value;
  param->param1 = 0;
  return TRUE;
}
