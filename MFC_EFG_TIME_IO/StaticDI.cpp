#include "StdAfx.h"
#include "StaticDI.h"


CStaticDI::CStaticDI()
{
  m_instantDiCtrl = NULL;
  m_instantDiCtrl = InstantDiCtrl::Create();
  channel = -1;
}


CStaticDI::~CStaticDI()
{
  m_instantDiCtrl->Dispose();
  channel = -1;
}

BOOL CStaticDI::Init(int device, int module)
{
  BOOL ret = FALSE;
  InstantDiCtrl * instantDiCtrl = InstantDiCtrl::Create();
  Array<DeviceTreeNode>* supportedDevices = instantDiCtrl->getSupportedDevices();
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
        ErrorCode errorCode = instantDiCtrl->setSelectedDevice(devInfo);//���豸
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
        int portCountMax = instantDiCtrl->getFeatures()->getPortCount();//8
        byte * portsValue = new byte[portCountMax];
        errorCode = instantDiCtrl->Read(0, portCountMax, portsValue);//��ȡ����ֵ��
        delete portsValue;
        m_channels.RemoveAll();

        for (int i = 0; i < portCountMax; i++)
        {
          m_channels.Add(i);
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
  instantDiCtrl->Dispose();

  return ret;
}

void CStaticDI::DeInit()
{
  m_modules.RemoveAll();
  m_channels.RemoveAll();
}

BOOL CStaticDI::Config(tagCtrlParam * param)
{
  ASSERT(param);
  tagCtrlParam* tpParam = param;
  ErrorCode	errorCode;
  DeviceInformation devInfo(tpParam->deviceNumber);//�豸��
  devInfo.ModuleIndex = tpParam->moduleIndex;//ģ���
  errorCode = m_instantDiCtrl->setSelectedDevice(devInfo);

  if (BioFailed(errorCode)) {
    return FALSE;
  }

  m_instantDiCtrl->getSelectedDevice(devInfo);

  TRACE(_T("Static DI - Run( %s )"), devInfo.Description);


  //Ĭ������
  return TRUE;
}

BOOL CStaticDI::Start(tagCtrlParam * param)
{
  ASSERT(param);
  tagCtrlParam* tpParam = param;
  ErrorCode errorCode;

  channel = tpParam->channel;
  return TRUE;
}

BOOL CStaticDI::Stop()
{
  channel = -1;
  return TRUE;
}

BOOL CStaticDI::Read(tagCtrlParam * param)
{
  ASSERT(param);
  if (channel < 0)
    return FALSE;
  int portCountMax = m_instantDiCtrl->getFeatures()->getPortCount();
  uint8 value;
  if (channel >= portCountMax)
    return FALSE;
  ErrorCode errorCode = m_instantDiCtrl->Read(channel, 1, &value);
  if (BioFailed(errorCode)) {
    return FALSE;
  }
  param->param0 = value;
  return TRUE;
}