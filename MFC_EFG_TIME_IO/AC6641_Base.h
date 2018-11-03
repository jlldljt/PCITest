#pragma once

#pragma comment(lib,"./AC6641/AC6641.lib")
#include"./AC6641/AC6641.h"

#include "ComDef.h"

typedef struct tagAC6641_Param
{
  DWORD boardNo;
  DWORD cardld;
  int channel;
  double param0;
  double param1;
}AC6641_Param, *PAC6641_Param;

class CAC6641_Base
{
public:
  CAC6641_Base();
  ~CAC6641_Base();

  AC6641_Param m_param;
  static HANDLE m_hDevice;//AC6641句柄
  static int m_ac6641Boards;//ac6641数量，当前无用
  static BOOL  DriverInit(void);//初始化所有板卡
  static void  DriverClose(void);

public:
  virtual BOOL Config(AC6641_Param* param) = 0;
  //virtual BOOL Start(AC6641_Param* param) = 0;
  virtual BOOL Stop() = 0;
  virtual BOOL Read(AC6641_Param* param) { return FALSE; }
  virtual BOOL Write(AC6641_Param* param) { return FALSE; }
  //virtual void Clear(AC6641_Param* param) { return; }
  //virtual BOOL SetParam(AC6641_Param param) { m_param = param; return TRUE; }
  //virtual AC6641_Param GetParam(void) { return m_param; }

};

