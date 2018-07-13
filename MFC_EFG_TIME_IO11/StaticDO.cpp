#include "StdAfx.h"
#include "StaticDO.h"


CStaticDO::CStaticDO()
{
  m_instantDoCtrl = NULL;
  m_instantDoCtrl = InstantDoCtrl::Create();
  channel = -1;
}


CStaticDO::~CStaticDO()
{
  m_instantDoCtrl->Dispose();
  channel = -1;
}

BOOL CStaticDO::Init(int device, int module)
{
  BOOL ret = FALSE;
  InstantDoCtrl * instantDoCtrl = InstantDoCtrl::Create();
  Array<DeviceTreeNode>* supportedDevices = instantDoCtrl->getSupportedDevices();
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
        ErrorCode errorCode = instantDoCtrl->setSelectedDevice(devInfo);//绑定设备
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
        int portCountMax = instantDoCtrl->getFeatures()->getPortCount();//8
        byte * portsValue = new byte[portCountMax];
        errorCode = instantDoCtrl->Read(0, portCountMax, portsValue);//读取所有值？
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
  instantDoCtrl->Dispose();

  return ret;
}

void CStaticDO::DeInit()
{
  m_modules.RemoveAll();
  m_channels.RemoveAll();
}

BOOL CStaticDO::Config(tagCtrlParam * param)
{
  tagCtrlParam* tpParam = param;
  ErrorCode	errorCode;
  DeviceInformation devInfo(tpParam->deviceNumber);//设备号
  devInfo.ModuleIndex = tpParam->moduleIndex;//模块号
  errorCode = m_instantDoCtrl->setSelectedDevice(devInfo);

  if (BioFailed(errorCode)) {
    return FALSE;
  }

  m_instantDoCtrl->getSelectedDevice(devInfo);

  TRACE(_T("Static DO - Run( %s )"), devInfo.Description);

  //默认配置
  return TRUE;
}

BOOL CStaticDO::Start(tagCtrlParam * param)
{
  tagCtrlParam* tpParam = param;
  ErrorCode errorCode;

  channel = tpParam->channel;

  return TRUE;
}

BOOL CStaticDO::Stop()
{
  channel = -1;
  return TRUE;
}

BOOL CStaticDO::Read(tagCtrlParam * param)
{
  if(channel < 0)
    return FALSE;
  int portCountMax = m_instantDoCtrl->getFeatures()->getPortCount();
  uint8 value;
  if (channel >= portCountMax)
    return FALSE;
  ErrorCode errorCode = m_instantDoCtrl->Read(channel, 1, &value);
  if (BioFailed(errorCode)) {
    return FALSE;
  }
  param->param0 = value;
  return TRUE;
}

BOOL CStaticDO::Write(tagCtrlParam * param)
{
  if (channel < 0)
    return FALSE;
  int portCountMax = m_instantDoCtrl->getFeatures()->getPortCount();
  uint8 value = param->param0;
  if (channel >= portCountMax)
    return FALSE;

  ErrorCode errorCode = m_instantDoCtrl->Write(channel, 1, &value);
  if (BioFailed(errorCode)) {
    return FALSE;
  }
  return TRUE;
}
