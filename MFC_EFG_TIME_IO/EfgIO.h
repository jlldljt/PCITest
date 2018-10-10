#pragma once

#include "MultiCardCtrl.h"

typedef struct  {
  struct {
    int out3;
    int out6;
  }laser;
  struct {

  }xray;
  struct {
    struct {
      int pos;
      int dst_pos;
      float max_freq;
      float min_freq;
      float flexible;
    }x;
    struct {
      int pos;
      int dst_pos;
      float max_freq;
      float min_freq;
      float flexible;
    }y;
    struct {
      int pos;
      int dst_pos;
      float max_freq;
      float min_freq;
      float flexible;
    }u;
  }motor;

}EFG_Param;

enum EFG_IChannel
{
  LASER_CNT_GATE= 1,//激光计数门控
  XRAY_CNT_GATE= 2,//x光计数门控
  TURNTABLE_ZERO = 4,//转盘零位
};

enum EFG_OChannel
{
  U_DIR = 0,//
  U_EN = 1,//
  XRAY_CTRL_GATE = 2,//光门
  U_GATE = 3,//
  U_OUTP = 4,//
  U_OUTN = 5,//
};

enum EFG_T0Channel
{
  LASER_CNT_START = 4,//激光计数起始channel
  //XRAY_CNT_START = 8,//x光计数起始channel
};


enum EFG_T1Channel
{
  XRAY_CNT_START = 0,//x光计数起始channel
};

class CEfgIO
{
public:
  CEfgIO();
  ~CEfgIO();
private:
  CMultiCardCtrl m_multiCardCtrl;
  BOOL m_on;//efg是否开启
  int m_on_card_no;//efg 开启所用的板卡序号

  BOOL AddPCI(int cardNo, PCIType type);//添加一个pci指针，最多3个,initPCI调用
  BOOL RunPCI(int cardNo);//按照配置启动一块pci，cardon调用
  BOOL StopPCI(int cardNo); //停止一块pci，cardoff调用
public:
  EFG_Param m_efg_param;
  int GetPCINum();//获取pci板卡的数量
  CString GetPCIDesc(int cardNo);//获取某块pci的设备描述
  PCIType GetPCIType(int cardNo);//获取某块pci的类型
  int GetPCIDeviceNumber(int cardNo);//获取某块pci的设备序号
  void InitPCI();//初始化m_multiCardCtrl
  CPCIBase* GetPCI(int cardNo);//获得pci的指针
  BOOL CardOn();//efg板卡启动
  BOOL CardOff();//efg板卡停止
  int GetCardIndex();//获得efg板卡
  //激光扫描
  int GetOut3();
  int GetOut6();
  //EFG读写
  double ReadDi(EFG_IChannel channel);
  void WriteDo(EFG_OChannel channel, double param);
  double ReadT0(EFG_T0Channel channel);

};

