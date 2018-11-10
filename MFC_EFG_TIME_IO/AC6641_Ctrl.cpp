#include "StdAfx.h"
#include "AC6641_Ctrl.h"

#include "AC6641_DI.h"
#include "AC6641_DO.h"

CAC6641_Ctrl::CAC6641_Ctrl()
{
  m_type = AC6641;

  memset(port, 0, sizeof(port));
}


CAC6641_Ctrl::~CAC6641_Ctrl()
{
}

BOOL CAC6641_Ctrl::CreateDO(int no, TimeIOType type, int device)
{
  BOOL ret = FALSE;
  if(no >= PORT_NUM)
	  return FALSE;
  if (port[no].ctrl)
    return FALSE;

  port[no].type = type;
  switch (type) {
  case AC6641_I:
    port[no].ctrl = new CAC6641_DI();
    if (port[no].ctrl)
      ret = TRUE;
    break;
  case AC6641_O:
    port[no].ctrl = new CAC6641_DO();
    if (port[no].ctrl)
      ret = TRUE;
    break;
  default:
    break;
  }

  return ret;
}

BOOL CAC6641_Ctrl::DeleteDO(int no)
{
  if(no >= PORT_NUM)
	  return FALSE;
  if (!port[no].ctrl)
    return FALSE;

  delete  port[no].ctrl;
  port[no].ctrl = NULL;
  return TRUE;
}

BOOL CAC6641_Ctrl::StartDO(int no, int device, double param0)
{
  if(no >= PORT_NUM)
	  return FALSE;
  ASSERT(port[no].ctrl);
  BOOL ret = FALSE;
  tagAC6641_Param param;
  param.boardNo = device;
  param.cardld = 0;//module 0
  param.channel = no;//channel¼´ÊÇno
  param.param0 = param0;
  ret = port[no].ctrl->Config(&param);
  return ret;
}

BOOL CAC6641_Ctrl::StopDO(int no)
{
  if(no >= PORT_NUM)
	  return FALSE;
  if (!port[no].ctrl)
    return FALSE;
  return port[no].ctrl->Stop();
}

BOOL CAC6641_Ctrl::ReadDO(int no, double & param0, double & param1)
{
  if(no >= PORT_NUM)
	  return FALSE;
  ASSERT(port[no].ctrl);

  tagAC6641_Param param;

  if (!port[no].ctrl->Read(&param))
    return FALSE;

  param0 = param.param0;
  param1 = param.param1;
  return TRUE;
}

BOOL CAC6641_Ctrl::WriteDO(int no, double & param0)
{
  if(no >= PORT_NUM)
	  return FALSE;
  ASSERT(port[no].ctrl);

  tagAC6641_Param param;
  param.param0 = param0;

  if (!port[no].ctrl->Write(&param))
    return FALSE;

  return TRUE;
}

int CAC6641_Ctrl::getDevices()
{
  for (int i = 0; i < CAC6641_Base::m_ac6641Boards; i++)
  {
    CString name;
    name.Format(L"AC6641%02d", i);
    DevInf dev = { AC6641, i, name };
    devices.Add(dev);
  }
  return CAC6641_Base::m_ac6641Boards;
}
