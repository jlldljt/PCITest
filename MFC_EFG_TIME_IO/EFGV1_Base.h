#pragma once

#include "Com.h"

typedef struct tagEFGV1_Param
{
  DWORD boardNo;
  DWORD cardld;
  int channel;
  double param0;
  double param1;
}EFGV1_Param, *PEFGV1_Param;

class CEFGV1_Base
{
public:
  CEFGV1_Base();
  ~CEFGV1_Base();

  static int m_EFGV1Boards;//数量，当前无用

  EFGV1_Param m_param;
  static BOOL  DriverInit(void);//初始化所有板卡
  static void  DriverClose(void);
  // 基础com命令封装，打开之后即可使用，所以其他类形同虚设
  int ReadDi(int no);
  int ReadDo(int no);
  int WriteDo(int no, int val);
  int ReadMotor(int no);
  int WriteMotor(int no, int val);

public:
  virtual BOOL Config(EFGV1_Param* param) = 0;
  //virtual BOOL Start(AC6641_Param* param) = 0;
  virtual BOOL Stop() = 0;
  virtual BOOL Read(EFGV1_Param* param) { return FALSE; }
  virtual BOOL Write(EFGV1_Param* param) { return FALSE; }
};

