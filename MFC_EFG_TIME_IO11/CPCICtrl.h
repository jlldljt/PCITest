#pragma once
#include "CPCIBase.h"

//���ƹ�����

class CPCICtrl
{
public:
  CPCICtrl();
  ~CPCICtrl();

public:
  // ʹ��AllDriverInit֮�󷵻صİ忨����
  static int m_num; 
  // �����忨
  static CPCIBase* Create(PCIType type);
  // ɾ���忨
  static void Delete(CPCIBase *ctrl);
  // ��ʼ�����а忨
  static void AllDriverInit(void); 
  // �ر����а忨
  static void AllDriverClose(void); 
  // ��
  static int getDevice(int no, DevInf& devInf);
};

