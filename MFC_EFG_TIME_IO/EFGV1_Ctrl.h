#pragma once
#include "CPCIBase.h"
#include "EFGV1_Base.h"



typedef struct tagEFGV1_Ctrl
{
  TimeIOType type;
  CEFGV1_Base* ctrl;
}EFGV1_Ctrl, *PEFGV1_Ctrl;

class CEFGV1_Ctrl :
  public CPCIBase
{
public:
  CEFGV1_Ctrl();
  ~CEFGV1_Ctrl();
private:
#define DI_NUM  10
#define DO_NUM  10
#define MOTOR_NUM  4
  //Di 8��ͨ����Ϣ
  EFGV1_Ctrl Di[DI_NUM];//
  //Do 8��ͨ����Ϣ
  EFGV1_Ctrl Do[DO_NUM];//
  //counter0 8��ͨ����Ϣ
  EFGV1_Ctrl Motor[MOTOR_NUM];//


public:
  //Di
  BOOL CreateDi(int no, TimeIOType type, int device);
  BOOL DeleteDi(int no);
  BOOL StartDi(int no, int device, double param0, double param1, void* param2 = NULL, void* param3 = NULL);
  BOOL StopDi(int no);
  BOOL ReadDi(int no, double& param0, double &param1);
  //T0 �����Motor
  BOOL CreateT0(int no, TimeIOType type, int device);
  BOOL DeleteT0(int no);
  BOOL StartT0(int no, int device, double param0 = 0, double param1 = 0);
  BOOL StopT0(int no);
  BOOL ReadT0(int no, double& param0, double &param1);
  BOOL WriteT0(int no, double param0 = 0, double param1 = 0);
  //Do
  BOOL CreateDO(int no, TimeIOType type, int device);
  BOOL DeleteDO(int no);
  BOOL StartDO(int no, int device, double param0);
  BOOL StopDO(int no);
  BOOL ReadDO(int no, double& param0, double &param1);
  BOOL WriteDO(int no, double& param0);

  // ��ȡ��װ���豸������ֻҪ��AllDriverInit�е���һ�ξ�����
  static int getDevices();
  // ��ȡ�豸����Ϣ
  //int getDevice(int no, DevInf& devInf);

};

