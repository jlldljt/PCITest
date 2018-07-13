#pragma once
#include "CPCIBase.h"

//疑似工具类

class CPCICtrl
{
public:
  CPCICtrl();
  ~CPCICtrl();

public:
  // 使用AllDriverInit之后返回的板卡数量
  static int m_num; 
  // 创建板卡
  static CPCIBase* Create(PCIType type);
  // 删除板卡
  static void Delete(CPCIBase *ctrl);
  // 初始化所有板卡
  static void AllDriverInit(void); 
  // 关闭所有板卡
  static void AllDriverClose(void); 
  // 按
  static int getDevice(int no, DevInf& devInf);
};

