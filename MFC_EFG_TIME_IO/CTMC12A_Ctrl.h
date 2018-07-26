#pragma once
#include "CPCIBase.h"
#include "CTMC12A_Base.h"

//计时器通道信息
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
  //Di 8个通道信息
  TMC12A_Ctrl Di[8];//8个
  //Do 8个通道信息
  TMC12A_Ctrl Do[8];//8个
  //counter0 8个通道信息
  TMC12A_Ctrl Counter0[8];//8个
  //counter1 4个通道信息
  TMC12A_Ctrl Counter1[8];//8个

  //存放设备信息
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

  // 获取安装的设备理论上只要再AllDriverInit中调用一次就行了
  static int getDevices();
  // 获取设备的信息
  //int getDevice(int no, DevInf& devInf);

};

