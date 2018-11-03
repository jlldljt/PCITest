#pragma once
#include "CPCIBase.h"
#include "AC6641_Base.h"

typedef struct tagAC6641_Ctrl
{
  TimeIOType type;
  CAC6641_Base* ctrl;
}AC6641_Ctrl, *PAC6641_Ctrl;


class CAC6641_Ctrl :
  public CPCIBase
{
public:
  CAC6641_Ctrl();
  ~CAC6641_Ctrl();
private:
#define PORT_NUM  12
  //12��port

  AC6641_Ctrl port[PORT_NUM];//

public:
  //T0 counter������doͨ��������ac6641 �� port
  BOOL CreateDO(int no, TimeIOType type, int device);
  BOOL DeleteDO(int no);
  BOOL StartDO(int no, int device, double param0);
  BOOL StopDO(int no);
  BOOL ReadDO(int no, double& param0, double &param1);
  BOOL WriteDO(int no, double& param0);

  static int getDevices();//����͵����ֻ�ܻ�ȡһ��ac6641
};

