#pragma once
#include "UniDAQ/UniDAQ.h"
#pragma comment(lib,"UniDAQ/UniDAQ.lib")

#include "ComDef.h"

typedef void(*TMC12A_Proc)(void *param);

typedef struct tagTMC12A_Param
{
  DWORD boardNo;
  DWORD cardld;
  int channel;
  double param0;
  double param1;
  TMC12A_Proc proc;
  void* procParam;
}TMC12A_Param, *PTMC12A_Param;

class CTMC12A_Base
{
private:

public:
  CTMC12A_Base();
  ~CTMC12A_Base();

  //CArray<int, int&>  m_modules;//支持的模块
  //CArray<int, int&>  m_channels;//支持的通道
  TMC12A_Proc m_callback;
  void *m_callbackParam;

  static WORD m_tmc12Boards;
  static WORD  DriverInit(WORD *wTotalBoards);//初始化所有板卡
  static WORD  DriverClose(void);
  //int GetModuleCount();
 // int GetModule(int no);
  //int GetChannelCount();
  //int GetChannel(int no);

public:
  //virtual int GetType();
  //virtual BOOL Init(int device, int module = 0) = 0;
  //virtual void DeInit() = 0;
  virtual BOOL Config(tagTMC12A_Param* param) = 0;
  virtual BOOL Start(tagTMC12A_Param* param) = 0;
  virtual BOOL Stop() = 0;
  virtual BOOL Read(tagTMC12A_Param* param) { return FALSE; }
  virtual BOOL Write(tagTMC12A_Param* param) { return FALSE; }
  virtual void Clear(tagTMC12A_Param* param) { return; }

};

