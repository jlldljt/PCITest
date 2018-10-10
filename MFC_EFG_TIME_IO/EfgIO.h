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
  LASER_CNT_GATE= 1,//��������ſ�
  XRAY_CNT_GATE= 2,//x������ſ�
  TURNTABLE_ZERO = 4,//ת����λ
};

enum EFG_OChannel
{
  U_DIR = 0,//
  U_EN = 1,//
  XRAY_CTRL_GATE = 2,//����
  U_GATE = 3,//
  U_OUTP = 4,//
  U_OUTN = 5,//
};

enum EFG_T0Channel
{
  LASER_CNT_START = 4,//���������ʼchannel
  //XRAY_CNT_START = 8,//x�������ʼchannel
};


enum EFG_T1Channel
{
  XRAY_CNT_START = 0,//x�������ʼchannel
};

class CEfgIO
{
public:
  CEfgIO();
  ~CEfgIO();
private:
  CMultiCardCtrl m_multiCardCtrl;
  BOOL m_on;//efg�Ƿ���
  int m_on_card_no;//efg �������õİ忨���

  BOOL AddPCI(int cardNo, PCIType type);//���һ��pciָ�룬���3��,initPCI����
  BOOL RunPCI(int cardNo);//������������һ��pci��cardon����
  BOOL StopPCI(int cardNo); //ֹͣһ��pci��cardoff����
public:
  EFG_Param m_efg_param;
  int GetPCINum();//��ȡpci�忨������
  CString GetPCIDesc(int cardNo);//��ȡĳ��pci���豸����
  PCIType GetPCIType(int cardNo);//��ȡĳ��pci������
  int GetPCIDeviceNumber(int cardNo);//��ȡĳ��pci���豸���
  void InitPCI();//��ʼ��m_multiCardCtrl
  CPCIBase* GetPCI(int cardNo);//���pci��ָ��
  BOOL CardOn();//efg�忨����
  BOOL CardOff();//efg�忨ֹͣ
  int GetCardIndex();//���efg�忨
  //����ɨ��
  int GetOut3();
  int GetOut6();
  //EFG��д
  double ReadDi(EFG_IChannel channel);
  void WriteDo(EFG_OChannel channel, double param);
  double ReadT0(EFG_T0Channel channel);

};

