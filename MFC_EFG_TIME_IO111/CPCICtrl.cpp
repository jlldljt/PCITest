#include "StdAfx.h"
#include "CPCICtrl.h"
#include "CTMC12A_Ctrl.h"
#include "TimeIOCtrl.h"
#include "AC6641_Ctrl.h"

int CPCICtrl::m_num = 0;

CPCICtrl::CPCICtrl()
{
}


CPCICtrl::~CPCICtrl()
{
}

CPCIBase * CPCICtrl::Create(PCIType type)
{
  switch (type) {
  case PCI1780U:
    return new CTimeIOCtrl;
  case TMC12A:
    return new CTMC12A_Ctrl;
  case AC6641:
    return new CAC6641_Ctrl;
  default:
    return NULL;
  }
}

void CPCICtrl::Delete(CPCIBase * ctrl)
{
  if(ctrl)
    delete ctrl;
}
//TODO: ���ӿ��޸�
void CPCICtrl::AllDriverInit(void)
{
  WORD wTotalBoards = 0;
  CPCIBase::devices.RemoveAll();
  if (0 != CTMC12A_Base::DriverInit(&wTotalBoards))
    AfxMessageBox(L"tmc12a��ʼ��ʧ��");
  if (TRUE != CAC6641_Base::DriverInit())
    AfxMessageBox(L"AC6641��ʼ��ʧ��");

//  int num1 = CTimeIOCtrl::getDevices();
  int num2 = CTMC12A_Ctrl::getDevices();
  int num3 = CAC6641_Ctrl::getDevices();
  m_num = /*num1 + */num2 + num3;
}
//TODO: ���ӿ��޸�
void CPCICtrl::AllDriverClose(void)
{
  CTMC12A_Base::DriverClose();
  CAC6641_Base::DriverClose();
}

int CPCICtrl::getDevice(int no, DevInf & devInf)
{
  return CPCIBase::getDevice(no, devInf);
}
