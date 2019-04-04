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

  static int m_EFGV1Boards;//��������ǰ����

  EFGV1_Param m_param;
  static BOOL  DriverInit(void);//��ʼ�����а忨
  static void  DriverClose(void);
  // ����com�����װ����֮�󼴿�ʹ�ã�������������ͬ����
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

