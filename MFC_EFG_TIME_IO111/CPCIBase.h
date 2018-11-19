#pragma once

#define OUT3_COUNTER 2
#define OUT6_COUNTER 3
//TODO: ���ӿ��޸�
enum PCIType {
  PCI1780U = 0,
  TMC12A,
  AC6641,
  NO_TYPE,
};
//TODO: ���ӿ��޸�
enum TimeIOType {
  STATIC_DI = 0,//SceInstantDi,
  INTERRUPT_DI,
  EVENT_COUNTER, //SceEventCounter,
  ONE_SHOT, //SceOneShot,
  TIME_PULSE, //SceTimerPulse,
  PW_MODULATOR, //ScePwModulator,
  STATIC_DO,
  TMC12_COUNTER,
  AC6641_I,
  AC6641_O,
};
//�忨��Ϣ
typedef struct tagDevInf
{
  PCIType type;
  int deviceNumber;
  CString description;
}DevInf, *PDevInf;

class CPCIBase
{
public:
  CPCIBase();
  ~CPCIBase();

public:
  PCIType m_type;

  // ȫ�ְ忨��Ϣ
  static CArray<DevInf, DevInf&> devices;
  // ��ȡ�豸����Ϣ
  static int getDevice(int no, DevInf& devInf);
public:
  //Di
  virtual BOOL CreateDi(int no, TimeIOType type, int device) { return FALSE; };
  virtual BOOL DeleteDi(int no) { return FALSE; };
  virtual BOOL StartDi(int no, int device, double param0, double param1, void* param2 = NULL, void* param3 = NULL) { return FALSE; };
  virtual BOOL StopDi(int no) { return FALSE; };
  virtual BOOL ReadDi(int no, double& param0, double &param1) { return FALSE; };
  //T0 counter
  virtual BOOL CreateT0(int no, TimeIOType type, int device) { return FALSE; };
  virtual BOOL DeleteT0(int no) { return FALSE; };
  virtual BOOL StartT0(int no, int device, double param0 = 0, double param1 = 0) { return FALSE; };
  virtual BOOL StopT0(int no) { return FALSE; };
  virtual BOOL ReadT0(int no, double& param0, double &param1) { return FALSE; };
  virtual BOOL RestartT0(int no) { return FALSE; };
  //T1 counter
  virtual BOOL CreateT1(int no, TimeIOType type, int device) { return FALSE; };
  virtual BOOL DeleteT1(int no) { return FALSE; };
  virtual BOOL StartT1(int no, int device, double param0, double param1 = 0) { return FALSE; };
  virtual BOOL StopT1(int no) { return FALSE; };
  virtual BOOL ReadT1(int no, double& param0, double &param1) { return FALSE; };
  virtual BOOL RestartT1(int no) { return FALSE; };
  //T0 counter
  virtual BOOL CreateDO(int no, TimeIOType type, int device) { return FALSE; };
  virtual BOOL DeleteDO(int no) { return FALSE; };
  virtual BOOL StartDO(int no, int device, double param0) { return FALSE; };
  virtual BOOL StopDO(int no) { return FALSE; };
  virtual BOOL ReadDO(int no, double& param0, double &param1) { return FALSE; };
  virtual BOOL WriteDO(int no, double& param0) { return FALSE; };
  // ��ȡ��װ���豸 ÿ�ְ忨����һ��
  virtual int getDevices() { return FALSE; };
  // ��ȡͨ����Ϣ ����Ҫ�ĸо�
  virtual int GetT0ChannelCount(int no) { return FALSE; };
  virtual int GetT0Channel(int no, int channelIndex) { return FALSE; };
};

