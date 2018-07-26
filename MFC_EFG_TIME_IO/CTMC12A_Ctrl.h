#pragma once
#include "CPCIBase.h"
#include "CTMC12A_Base.h"

//��ʱ��ͨ����Ϣ
typedef struct tagTMC12A_Ctrl
{
  TimeIOType type;
  CTMC12A_Base* ctrl;
}TMC12A_Ctrl, *PTMC12A_Ctrl;


class CTMC12A_Ctrl :
  public CPCIBase
{
public:
  CTMC12A_Ctrl();
  ~CTMC12A_Ctrl();
private:
  //Di 8��ͨ����Ϣ
  TMC12A_Ctrl Di[8];//8��
  //Do 8��ͨ����Ϣ
  TMC12A_Ctrl Do[8];//8��
  //counter0 8��ͨ����Ϣ
  TMC12A_Ctrl Counter0[8];//8��
  //counter1 4��ͨ����Ϣ
  TMC12A_Ctrl Counter1[8];//8��

  //����豸��Ϣ
 // WORD m_totalBoards;
  
public:
  //Di
  BOOL CreateDi(int no, TimeIOType type, int device);
  BOOL DeleteDi(int no);
  BOOL StartDi(int no, int device, double param0, double param1, void* param2 = NULL, void* param3 = NULL);
  BOOL StopDi(int no);
  BOOL ReadDi(int no, double& param0, double &param1);
  //T0 counter
  BOOL CreateT0(int no, TimeIOType type, int device);
  BOOL DeleteT0(int no);
  BOOL StartT0(int no, int device, double param0 = 0, double param1 = 0);
  BOOL StopT0(int no);
  BOOL ReadT0(int no, double& param0, double &param1);
  BOOL RestartT0(int no);
  //T0 counter
  BOOL CreateT1(int no, TimeIOType type, int device);
  BOOL DeleteT1(int no);
  BOOL StartT1(int no, int device, double param0, double param1 = 0);
  BOOL StopT1(int no);
  BOOL ReadT1(int no, double& param0, double &param1);
  BOOL RestartT1(int no);
  //T0 counter
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

