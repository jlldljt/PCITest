#include "StdAfx.h"
#include "EFGV1_Ctrl.h"
#include "EFGV1_DI.h"
#include "EFGV1_DO.h"
#include "EFGV1_MOTOR.h"



CEFGV1_Ctrl::CEFGV1_Ctrl()
{
  memset( Di, 0x00,sizeof(Di));
  memset( Do, 0x00,sizeof(Do));
  memset( Motor, 0x00,sizeof(Motor));
}


CEFGV1_Ctrl::~CEFGV1_Ctrl()
{
}

BOOL CEFGV1_Ctrl::CreateDi(int no, TimeIOType type, int device)
{
  BOOL ret = FALSE;
  ASSERT(no < DI_NUM);
  if (Di[no].ctrl)
    return FALSE;

  Di[no].type = type;
  switch (type) {
  case STATIC_DI:
    Di[no].ctrl = new CEFGV1_DI();
    if (Di[no].ctrl)
      ret = TRUE;
    break;
    break;
  default:
    break;
  }

  return ret;
}

BOOL CEFGV1_Ctrl::DeleteDi(int no)
{
  ASSERT(no < DI_NUM);
  if (!Di[no].ctrl)
    return FALSE;
  delete  Di[no].ctrl;
  Di[no].ctrl = NULL;
  return TRUE;
}

BOOL CEFGV1_Ctrl::StartDi(int no, int device, double param0, double param1, void * param2, void * param3)
{
  ASSERT(no < DI_NUM);
  ASSERT(Di[no].ctrl);
  BOOL ret = FALSE;
  tagEFGV1_Param param;
  param.boardNo = device;
  param.cardld = 0;//module 0
  param.channel = no;//channel¼´ÊÇno
  param.param0 = param0;
  param.param1 = param1;
  ret = Di[no].ctrl->Config(&param);
  /*if () {
    Motor[no].ctrl->SetParam(param);
    ret = Motor[no].ctrl->Start(&param);
  }*/
  return ret;
}

BOOL CEFGV1_Ctrl::StopDi(int no)
{
  ASSERT(no < DI_NUM);
  if (!Di[no].ctrl)
    return FALSE;

  return Di[no].ctrl->Stop();
}

BOOL CEFGV1_Ctrl::ReadDi(int no, double & param0, double & param1)
{
  ASSERT(no < DI_NUM);
  ASSERT(Di[no].ctrl);

  tagEFGV1_Param param;

  if (!Di[no].ctrl->Read(&param))
    return FALSE;

  param0 = param.param0;
  param1 = param.param1;
  return TRUE;
}

//BOOL CEFGV1_Ctrl::CreateT0(int no, TimeIOType type, int device)
//{
//  BOOL ret = FALSE;
//  ASSERT(no < MOTOR_NUM);
//  if (Motor[no].ctrl)
//    return FALSE;
//
//  Motor[no].type = type;
//  switch (type) {
//  case EFGV1_MOTOR:
//    Motor[no].ctrl = new CEFGV1_Motor();
//    if (Motor[no].ctrl)
//      ret = TRUE;
//    break;
//    break;
//  default:
//    break;
//  }
//
//  return ret;
//}
//
//BOOL CEFGV1_Ctrl::DeleteT0(int no)
//{
//  ASSERT(no < MOTOR_NUM);
//  if (!Motor[no].ctrl)
//    return FALSE;
//  delete  Motor[no].ctrl;
//  Motor[no].ctrl = NULL;
//  return TRUE;
//}
//
//BOOL CEFGV1_Ctrl::StartT0(int no, int device, double param0, double param1)
//{
//  ASSERT(no < MOTOR_NUM);
//  ASSERT(Motor[no].ctrl);
//  BOOL ret = FALSE;
//  tagEFGV1_Param param;
//  param.boardNo = device;
//  param.cardld = 0;//module 0
//  param.channel = no;//channel¼´ÊÇno
//  param.param0 = param0;
//  param.param1 = param1;
//  ret = Motor[no].ctrl->Config(&param);
//  /*if () {
//    Motor[no].ctrl->SetParam(param);
//    ret = Motor[no].ctrl->Start(&param);
//  }*/
//  return ret;
//}
//
//BOOL CEFGV1_Ctrl::StopT0(int no)
//{
//  ASSERT(no < MOTOR_NUM);
//  if (!Motor[no].ctrl)
//    return FALSE;
//
//  return Motor[no].ctrl->Stop();
//}
//
//BOOL CEFGV1_Ctrl::ReadT0(int no, double & param0, double & param1)
//{
//  ASSERT(no < MOTOR_NUM);
//  ASSERT(Motor[no].ctrl);
//
//  tagEFGV1_Param param;
//
//  if (!Motor[no].ctrl->Read(&param))
//    return FALSE;
//
//  param0 = param.param0;
//  param1 = param.param1;
//  return TRUE;
//}
//
//BOOL CEFGV1_Ctrl::WriteT0(int no, double param0, double param1)
//{
//  if (no >= MOTOR_NUM)
//    return FALSE;
//  ASSERT(Motor[no].ctrl);
//
//  tagEFGV1_Param param;
//  param.param0 = param0;
//  param.param1 = param1;
//
//  if (!Motor[no].ctrl->Write(&param))
//    return FALSE;
//
//  return TRUE;
//};

BOOL CEFGV1_Ctrl::CreateDO(int no, TimeIOType type, int device)
{
  BOOL ret = FALSE;
  ASSERT(no < DO_NUM);
  if (Do[no].ctrl)
    return FALSE;

  Do[no].type = type;
  switch (type) {
  case STATIC_DO:
    Do[no].ctrl = new CEFGV1_DO();
    if (Do[no].ctrl)
      ret = TRUE;
    break;
    break;
  default:
    break;
  }

  return ret;
}

BOOL CEFGV1_Ctrl::DeleteDO(int no)
{
  ASSERT(no < DO_NUM);
  if (!Do[no].ctrl)
    return FALSE;
  delete  Do[no].ctrl;
  Do[no].ctrl = NULL;
  return TRUE;
}

BOOL CEFGV1_Ctrl::StartDO(int no, int device, double param0)
{
  ASSERT(no < DO_NUM);
  ASSERT(Do[no].ctrl);
  BOOL ret = FALSE;
  tagEFGV1_Param param;
  param.boardNo = device;
  param.cardld = 0;//module 0
  param.channel = no;//channel¼´ÊÇno
  param.param0 = param0;
  ret = Do[no].ctrl->Config(&param);
  /*if () {
    Motor[no].ctrl->SetParam(param);
    ret = Motor[no].ctrl->Start(&param);
  }*/
  return ret;
}

BOOL CEFGV1_Ctrl::StopDO(int no)
{
  ASSERT(no < DO_NUM);
  if (!Do[no].ctrl)
    return FALSE;

  return Do[no].ctrl->Stop();
}

BOOL CEFGV1_Ctrl::ReadDO(int no, double & param0, double & param1)
{
  ASSERT(no < DO_NUM);
  ASSERT(Do[no].ctrl);

  tagEFGV1_Param param;

  if (!Do[no].ctrl->Read(&param))
    return FALSE;

  param0 = param.param0;
  param1 = param.param1;
  return TRUE;
}

BOOL CEFGV1_Ctrl::WriteDO(int no, double & param0)
{
  if (no >= DO_NUM)
    return FALSE;
  ASSERT(Do[no].ctrl);

  tagEFGV1_Param param;
  param.param0 = param0;

  if (!Do[no].ctrl->Write(&param))
    return FALSE;

  return TRUE;
}

int CEFGV1_Ctrl::getDevices()
{
  for (int i = 0; i < CEFGV1_Base::m_EFGV1Boards; i++)
  {
    CString name;
    name.Format(L"EFGV1%02d", i);
    DevInf dev = { EFG_V1, i, name };
    devices.Add(dev);
  }
  return CEFGV1_Base::m_EFGV1Boards;
}
