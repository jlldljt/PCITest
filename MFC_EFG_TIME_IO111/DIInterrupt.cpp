#include "StdAfx.h"
#include "DIInterrupt.h"

void CDIInterrupt::OnDiSnapEvent(void * sender, DiSnapEventArgs * args, void * userParam)
{
  CDIInterrupt * uParam = (CDIInterrupt *)userParam;
  if (uParam->m_callback) {
    uParam->m_callback(uParam->m_callbackParam);
  }
  uParam->m_cnt++;
}

CDIInterrupt::CDIInterrupt()
{
  m_instantDiCtrl = NULL;
  m_instantDiCtrl = InstantDiCtrl::Create();
  m_instantDiCtrl->addInterruptHandler(OnDiSnapEvent, this);
}


CDIInterrupt::~CDIInterrupt()
{
  m_instantDiCtrl->Dispose();
}

BOOL CDIInterrupt::Init(int device, int module)
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
        ErrorCode errorCode = instantDiCtrl->setSelectedDevice(devInfo);//绑定设备
        if (errorCode != 0) {
          CString str;
          TCHAR des[MAX_DEVICE_DESC_LEN];
          TRACE("Error: the error code is 0x % x, The %s is busy or not exit in computer now.\n\
              Select other device please!", errorCode, WCHAR_TO_TCHAR(devNote.Description, des));
          break;
        }

        Array<DiintChannel>* interruptChans = instantDiCtrl->getDiintChannels();//查看此设备是否支持int
        if (interruptChans == NULL)
        {
          break;
        }
          
        ret = TRUE;

        int portsCount = instantDiCtrl->getPortCount();
       
        int INTChanCount = interruptChans->getCount();
        for (i = 0; i < INTChanCount; ++i)
        {
          int chan = interruptChans->getItem(i).getChannel();
          m_channels.Add(chan);
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

void CDIInterrupt::DeInit()
{
  m_callback = NULL;
  m_modules.RemoveAll();
  m_channels.RemoveAll();
}

BOOL CDIInterrupt::Config(tagCtrlParam * param)
{
  tagCtrlParam* tpParam = param;
  //ASSERT(tpParam->proc);
  ErrorCode	errorCode;
  DeviceInformation devInfo(tpParam->deviceNumber);//设备号
  errorCode = m_instantDiCtrl->setSelectedDevice(devInfo);

  if (BioFailed(errorCode)) {
    return FALSE;
  }

  m_instantDiCtrl->getSelectedDevice(devInfo);

  TRACE(_T("DI Interrupt - Run( %s )"), devInfo.Description);

  Array<DiintChannel>* interruptChans = m_instantDiCtrl->getDiintChannels();// 得到设备的int通道集合
  int intPortCount = interruptChans->getCount();// 得到数量
  for (int j = 0; j < intPortCount; ++j)
  {
    if (tpParam->channel == interruptChans->getItem(j).getChannel())// 判断是否是设置的通道
    {
      errorCode = interruptChans->getItem(j).setEnabled(true);
      if (BioFailed(errorCode)) {
        return FALSE;
      }
      m_callback = tpParam->proc;
      m_callbackParam = tpParam->procParam;

      //默认配置
      m_instantDiCtrl->getDiintChannels()->getItem(tpParam->channel).setGated(false);
      m_instantDiCtrl->getDiintChannels()->getItem(tpParam->channel).setTrigEdge(RisingEdge);
      //

      return TRUE;
      //break;
    }
  }
  // TODO：成功时不会执行到
  return FALSE;
}

BOOL CDIInterrupt::Start(tagCtrlParam * param)
{
  ErrorCode errorCode = m_instantDiCtrl->SnapStart();
  if (BioFailed(errorCode)) {
    return FALSE;
  }
  m_cnt = 0;
  return TRUE;
}

BOOL CDIInterrupt::Stop()
{
  ErrorCode errorCode = m_instantDiCtrl->SnapStop();
  if (BioFailed(errorCode)) {
    return FALSE;
  }
  return TRUE;
}


BOOL CDIInterrupt::Read(tagCtrlParam* param)
{
  ASSERT(param);
  ErrorCode errorCode;
  int32 value = 0;
  param->param0 = m_cnt;
  param->param1 = 0;
  return TRUE;
}