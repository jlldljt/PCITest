#include "StdAfx.h"
#include "CPCICtrl.h"
#include "CTMC12A_Ctrl.h"
#include "TimeIOCtrl.h"
#include "AC6641_Ctrl.h"
#include "EFGV1_Ctrl.h"


int CPCICtrl::m_num = 0;

CPCICtrl::CPCICtrl()
{
}


CPCICtrl::~CPCICtrl()
{
}
// 增加卡修改
CPCIBase * CPCICtrl::Create(PCIType type)
{
  switch (type) {
  //case PCI1780U:
   // return new CTimeIOCtrl;
  //case TMC12A:
  //  return new CTMC12A_Ctrl;
  //case AC6641:
  //  return new CAC6641_Ctrl;
  case EFG_V1:
    return new CEFGV1_Ctrl;
  default:
    return NULL;
  }
}

void CPCICtrl::Delete(CPCIBase * ctrl)
{
  if(ctrl)
    delete ctrl;
}
//TODO: 增加卡修改
void CPCICtrl::AllDriverInit(void)
{
  WORD wTotalBoards = 0;
  CPCIBase::devices.RemoveAll();
  /*if (0 != CTMC12A_Base::DriverInit(&wTotalBoards))
    AfxMessageBox(L"tmc12a初始化失败");
  if (TRUE != CAC6641_Base::DriverInit())
    AfxMessageBox(L"AC6641初始化失败");*/
  if (TRUE != CEFGV1_Base::DriverInit())
    AfxMessageBox(L"EFGV1初始化失败");

//  int num1 = CTimeIOCtrl::getDevices();
/*  int num2 = CTMC12A_Ctrl::getDevices();
  int num3 = CAC6641_Ctrl::getDevices();*/
  int num4 = CEFGV1_Ctrl::getDevices();
  m_num = /*num1 + *//*num2 + num3*/num4;
}
//TODO: 增加卡修改
void CPCICtrl::AllDriverClose(void)
{
  /*CTMC12A_Base::DriverClose();
  CAC6641_Base::DriverClose();*/
  CEFGV1_Base::DriverClose();
}

int CPCICtrl::getDevice(int no, DevInf & devInf)
{
  return CPCIBase::getDevice(no, devInf);
}
