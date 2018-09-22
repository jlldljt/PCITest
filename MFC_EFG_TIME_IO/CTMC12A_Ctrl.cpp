#include "StdAfx.h"
#include "CTMC12A_Ctrl.h"

#include "CTMC12A_DI.h"
#include "CTMC12A_Counter.h"
#include "CTMC12A_Callback.h"
#include "CTMC12A_DO.h"

CTMC12A_Ctrl::CTMC12A_Ctrl()
{
  m_type = TMC12A;

  //devices.RemoveAll();

  memset(Di, 0, sizeof(Di));
  memset(Do, 0, sizeof(Do));
  memset(Counter0, 0, sizeof(Counter0));
  memset(Counter1, 0, sizeof(Counter1));
}


CTMC12A_Ctrl::~CTMC12A_Ctrl()
{
}

BOOL CTMC12A_Ctrl::CreateDi(int no, TimeIOType type, int device)
{
  BOOL ret = FALSE;
  ASSERT(no < DI_NUM);
  if (Di[no].ctrl)
    return FALSE;

  Di[no].type = type;
  switch (type) {
  case STATIC_DI:
    Di[no].ctrl = new CTMC12A_DI();
    if(Di[no].ctrl)
      ret = TRUE;
    break;
  case INTERRUPT_DI:
    Di[no].ctrl = new CTMC12A_Callback();
    if (Di[no].ctrl)
      ret = TRUE;
    break;
  default:
    break;
  }

  return ret;
}

BOOL CTMC12A_Ctrl::DeleteDi(int no)
{
  ASSERT(no < DI_NUM);
  if (!Di[no].ctrl)
    return FALSE;
  delete  Di[no].ctrl;
  Di[no].ctrl = NULL;
  return TRUE;
}

BOOL CTMC12A_Ctrl::StartDi(int no, int device, double param0, double param1, void * param2, void * param3)
{
  ASSERT(no <DI_NUM);
  ASSERT(Di[no].ctrl);
  BOOL ret = FALSE;
  tagTMC12A_Param param;
  param.boardNo = device;
  param.cardld = 0;//module 0
  param.channel = no;//channel即是no
  param.param0 = param0;
  param.param1 = param1;
  param.proc = (TMC12A_Proc)param2;
  param.procParam = param3;
  if (Di[no].ctrl->Config(&param)) {
    Di[no].ctrl->SetParam(param);
    ret = Di[no].ctrl->Start(&param);
  }
  return ret;
}

BOOL CTMC12A_Ctrl::StopDi(int no)
{
  ASSERT(no < DI_NUM);
  if (!Di[no].ctrl)
    return FALSE;
  return Di[no].ctrl->Stop();
}

BOOL CTMC12A_Ctrl::ReadDi(int no, double & param0, double & param1)
{
  ASSERT(no < DI_NUM);
  ASSERT(Di[no].ctrl);

  tagTMC12A_Param param;

  if (!Di[no].ctrl->Read(&param))
    return FALSE;

  param0 = param.param0;
  param1 = param.param1;
  return TRUE;
}

BOOL CTMC12A_Ctrl::CreateT0(int no, TimeIOType type, int device)
{
  BOOL ret = FALSE;
  ASSERT(no < T0_NUM);
  if (Counter0[no].ctrl)
    return FALSE;

  Counter0[no].type = type;
  switch (type) {
  case TMC12_COUNTER:
    Counter0[no].ctrl = new CTMC12A_Counter();
    if (Counter0[no].ctrl)
      ret = TRUE;
    break;
    break;
  default:
    break;
  }

  return ret;
}

BOOL CTMC12A_Ctrl::DeleteT0(int no)
{
  ASSERT(no < T0_NUM);
  if (!Counter0[no].ctrl)
    return FALSE;
  delete  Counter0[no].ctrl;
  Counter0[no].ctrl = NULL;
  return TRUE;
}

BOOL CTMC12A_Ctrl::StartT0(int no, int device, double param0, double param1)
{
  ASSERT(no < T0_NUM);
  ASSERT(Counter0[no].ctrl);
  BOOL ret = FALSE;
  tagTMC12A_Param param;
  param.boardNo = device;
  param.cardld = 0;//module 0
  param.channel = no;//channel即是no
  param.param0 = param0;
  param.param1 = param1;

  if (Counter0[no].ctrl->Config(&param)) {
    Counter0[no].ctrl->SetParam(param);
    ret = Counter0[no].ctrl->Start(&param);
  }
  return ret;
}

BOOL CTMC12A_Ctrl::StopT0(int no)
{
  ASSERT(no < T0_NUM);
  if (!Counter0[no].ctrl)
    return FALSE;

  return Counter0[no].ctrl->Stop();
}

BOOL CTMC12A_Ctrl::ReadT0(int no, double & param0, double & param1)
{
  ASSERT(no < T0_NUM);
  ASSERT(Counter0[no].ctrl);

  tagTMC12A_Param param;

  if (!Counter0[no].ctrl->Read(&param))
    return FALSE;

  param0 = param.param0;
  param1 = param.param1;
  return TRUE;
}

BOOL CTMC12A_Ctrl::RestartT0(int no)
{
  if (TRUE != StopT0(no))
    return FALSE;

  ASSERT(no < T0_NUM);
  ASSERT(Counter0[no].ctrl);
  BOOL ret = FALSE;
  tagTMC12A_Param param = Counter0[no].ctrl->GetParam();

  if (Counter0[no].ctrl->Config(&param)) {
    ret = Counter0[no].ctrl->Start(&param);
  }
  return ret;
}

BOOL CTMC12A_Ctrl::CreateT1(int no, TimeIOType type, int device)
{
  BOOL ret = FALSE;
  ASSERT(no < T1_NUM);
  if (Counter1[no].ctrl)
    return FALSE;

  Counter1[no].type = type;
  switch (type) {
  case TMC12_COUNTER:
    Counter1[no].ctrl = new CTMC12A_Counter();
    if (Counter1[no].ctrl)
      ret = TRUE;
    break;
    break;
  default:
    break;
  }

  return ret;
}

BOOL CTMC12A_Ctrl::DeleteT1(int no)
{
  ASSERT(no < T1_NUM);
  if (!Counter1[no].ctrl)
    return FALSE;
  delete  Counter1[no].ctrl;
  Counter1[no].ctrl = NULL;
  return TRUE;
}

BOOL CTMC12A_Ctrl::StartT1(int no, int device, double param0, double param1)
{
  ASSERT(no < T1_NUM);
  ASSERT(Counter1[no].ctrl);
  BOOL ret = FALSE;
  tagTMC12A_Param param;
  param.boardNo = device;
  param.cardld = 0;//module 0
  param.channel = no+8;//channel即是no
  param.param0 = param0;
  param.param1 = param1;

  if (Counter1[no].ctrl->Config(&param)) {
    Counter1[no].ctrl->SetParam(param);
    ret = Counter1[no].ctrl->Start(&param);
  }
  return ret;
}

BOOL CTMC12A_Ctrl::StopT1(int no)
{
  ASSERT(no < T1_NUM);
  if (!Counter1[no].ctrl)
    return FALSE;

  return Counter1[no].ctrl->Stop();
}

BOOL CTMC12A_Ctrl::ReadT1(int no, double & param0, double & param1)
{
  ASSERT(no < T1_NUM);
  ASSERT(Counter1[no].ctrl);

  tagTMC12A_Param param;

  if (!Counter1[no].ctrl->Read(&param))
    return FALSE;

  param0 = param.param0;
  param1 = param.param1;
  return TRUE;
}

BOOL CTMC12A_Ctrl::RestartT1(int no)
{
  if (TRUE != StopT1(no))
    return FALSE;

  ASSERT(no < T1_NUM);
  ASSERT(Counter1[no].ctrl);
  BOOL ret = FALSE;
  tagTMC12A_Param param = Counter1[no].ctrl->GetParam();

  if (Counter1[no].ctrl->Config(&param)) {
    ret = Counter1[no].ctrl->Start(&param);
  }
  return ret;
}

BOOL CTMC12A_Ctrl::CreateDO(int no, TimeIOType type, int device)
{
  BOOL ret = FALSE;
  ASSERT(no < DO_NUM);
  if (Do[no].ctrl)
    return FALSE;

  Do[no].type = type;
  switch (type) {
  case STATIC_DO:
    Do[no].ctrl = new CTMC12A_DO();
    if (Do[no].ctrl)
      ret = TRUE;
    break;
  default:
    break;
  }

  return ret;
}

BOOL CTMC12A_Ctrl::DeleteDO(int no)
{
  ASSERT(no < DO_NUM);
  if (!Do[no].ctrl)
    return FALSE;

  delete  Do[no].ctrl;
  Do[no].ctrl = NULL;
  return TRUE;
}

BOOL CTMC12A_Ctrl::StartDO(int no, int device, double param0)
{
  ASSERT(no < DO_NUM);
  ASSERT(Do[no].ctrl);
  BOOL ret = FALSE;
  tagTMC12A_Param param;
  param.boardNo = device;
  param.cardld = 0;//module 0
  param.channel = no;//channel即是no
  param.param0 = param0;
  if (Do[no].ctrl->Config(&param)) {
    Do[no].ctrl->SetParam(param);
    ret = Do[no].ctrl->Start(&param);
  }
  return ret;
}

BOOL CTMC12A_Ctrl::StopDO(int no)
{
  ASSERT(no < DO_NUM);
  if (!Do[no].ctrl)
    return FALSE;
  return Do[no].ctrl->Stop();
}

BOOL CTMC12A_Ctrl::ReadDO(int no, double & param0, double & param1)
{
  ASSERT(no < DO_NUM);
  ASSERT(Do[no].ctrl);

  tagTMC12A_Param param;

  if (!Do[no].ctrl->Read(&param))
    return FALSE;

  param0 = param.param0;
  param1 = param.param1;
  return TRUE;
}

BOOL CTMC12A_Ctrl::WriteDO(int no, double & param0)
{
  ASSERT(no < DO_NUM);
  ASSERT(Do[no].ctrl);

  tagTMC12A_Param param;
  param.param0 = param0;

  if (!Do[no].ctrl->Write(&param))
    return FALSE;

  return TRUE;
}

int CTMC12A_Ctrl::getDevices()
{
  WORD wRtn;
  WORD wBoardNo;
  IXUD_DEVICE_INFO sDevInfo;
  IXUD_CARD_INFO sCardInfo;
  char szModelName[20] = "Unknow Device";
  //Get Card Information
  for (WORD wBoardIndex = 0; wBoardIndex<CTMC12A_Base::m_tmc12Boards; wBoardIndex++)
  {
    wRtn = Ixud_GetCardInfo(wBoardIndex, &sDevInfo, &sCardInfo, szModelName);

    //m_IDC_COMBO_SelectBoard.AddString(szModelName);
    WCHAR des[256] = { 0 };
    CString name = CHAR_TO_WCHAR(szModelName, des);
    name.Format(L"TMC12A%d", sCardInfo.CardID);
    name += CHAR_TO_WCHAR(szModelName, des);
    DevInf dev = { TMC12A, wBoardIndex, name };
    devices.Add(dev);
  }
  return CTMC12A_Base::m_tmc12Boards;
}

//int CTMC12A_Ctrl::getDevice(int no, DevInf & devInf)
//{
//  ASSERT(no >= 0);
//  int count = devices.GetCount();
//  if (no < count) {
//    devInf = devices.GetAt(no);
//    return 1;
//  }
//  else {
//    return -1;
//  }
//}
