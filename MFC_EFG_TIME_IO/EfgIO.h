#pragma once

#include "MultiCardCtrl.h"

#define MAX_SORT_NUM 50
#define MAX_TYPE_NUM 2

#define MAX_CARD_LEN 50

#define IO_ON 1
#define IO_OFF 0

#define MOTOR_X 0
#define MOTOR_Y 1
#define MOTOR_U 2
// �󲿷����ݱ��浽ini
typedef struct  {
  
  struct {//�������
    int out3;
    int out6;
  }laser;
  
  struct {//X�����

  }xray;

  struct {  //������
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
  
  struct//�û�����
  {
    int type;//��Ƭ���͡�0:0��Ƭ��1��1��Ƭ
    int type_sort_num[MAX_TYPE_NUM];//����Ƭ���͵ĵ�λ����
    struct
    {
      int x_off;//x����λ��
      int x_wait;//x�ȴ�λ��
      int y_wait;//y�ȴ�λ��
      int u_;//uλ��
      int y_off[MAX_TYPE_NUM][MAX_SORT_NUM];//y����λ�ã������Ͳ�ͬ���ж��ֵ�λ�����MAX_SORT_NUM
      int primary_n;//����ǵ�
      int primary_p;//���Ǹ�
      int secondary_n;//�νǵ�
      int secondary_p;//�νǸ�
      int not_detected;//δ���
    }pos;
    struct{//����ͣ��ʱ��
      int x_on;//x����ʱ��
      int x_off;//x����ʱ��
      int y_on;//y����ʱ��
      int y_off;//y����ʱ��
      int blow;//����ʱ��
      int clean;//��������ʱ��
    }time;
    struct{//�������
      TCHAR card[MAX_CARD_LEN];//���̿�
      struct {//������
        int type;// ��0����1����2��Ч��3
        int center_degree;
        int step_degree;
      }primary;
      struct {
        int type;// ��0����1����2��Ч��3
        int min_degree;
        int max_degree;
      }secondary;
      struct {// ��Ч�� = ԭʼ���� + ��ԭʼ���� - ��Ч�ǵ��������/��Ч������
        int phi;//��Ч�ǵ������
        int factor;//��Ч������
      }equivalent_angle;
     
    }measure;
  }user_config;
  //λ�����

}EFG_ConfigParam;

// �󲿷����ݲ����浽ini
typedef struct {
  struct
  {
    int y_off[MAX_SORT_NUM];    //y_off��λ�õķֵ�����
    int primary_n;//����ǵ�
    int primary_p;//���Ǹ�
    int secondary_n;//�νǵ�
    int secondary_p;//�νǸ�
    int not_detected;//δ���
  }num;
  struct
  {
    int sort_sec[MAX_SORT_NUM];    //y_off��λ�õĽǶ�,��

  }degree;
  struct
  {
    double D1, D2, DM;              //x����
    double A, w, t, k;              //������
    double cur_phi, cur_phi0, cur_; //��ǰƬ
    int cur_pos;//��ǰƬ��λ
    double avg_phi, avg_phi0;       //ƽ��
    double std_phi, std_phi0;       // ɢ��
  }measure;
}EFG_ResultParam;


enum EFG_IChannel
{
  LASER_CNT_GATE= 1,//��������ſ�
  XRAY_CNT_GATE= 2,//x������ſ�
  TURNTABLE_ZERO = 4,//ת����λ
};

enum EFG_OChannel
{
 // U_DIR = 0,//
  U_EN = 1,//
  XRAY_CTRL_GATE = 2,//����
  U_GATE = 3,//
  U_OUTP = 4,//
  U_OUTN = 5,//
};
//�����
enum AC6641_Channel
{
  X_DIR=4*8,
  X_GO,
  X_NOZZLE=4*8+3,
  Y_DIR,
  Y_GO,
  Y_NOZZLE = 4 * 8 + 7,
  BLOW,
  XRAY_GATE,
  X_FULL_CURRENT,
  Y_FULL_CURRENT,
  ALERT,
  CLEAN,
  X_STATE=6*8,
  Y_STATE=6*8+4,
  U_DIR=10*8,
  U_GO,
  //U_NOZZLE=10*8+3,
  U_STATE = 11 * 8,
  READY,//���Ͽ���
};
//��port
enum AC6641_Port
{
  X_STEP_LOW=0,
  X_STEP_HIGH=1,
  Y_STEP_LOW=2,
  Y_STEP_HIGH=3,
  U_STEP_LOW=8,
  U_STEP_HIGH=9,
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
  int m_on_iocard_no;//efg �������õ�io�忨���(ac6641

  BOOL AddPCI(int cardNo, PCIType type);//���һ��pciָ�룬���3��,initPCI����
  BOOL RunPCI(int cardNo);//������������һ��pci��cardon����
  BOOL StopPCI(int cardNo); //ֹͣһ��pci��cardoff����
public:
  EFG_ConfigParam m_configParam;
  EFG_ResultParam m_resultParam;
  int GetPCINum();//��ȡpci�忨������
  CString GetPCIDesc(int cardNo);//��ȡĳ��pci���豸����
  PCIType GetPCIType(int cardNo);//��ȡĳ��pci������
  int GetPCIDeviceNumber(int cardNo);//��ȡĳ��pci���豸���
  void InitPCI();//��ʼ��m_multiCardCtrl
  CPCIBase* GetPCI(int cardNo);//���pci��ָ��
  BOOL CardOn();//efg����忨����
  BOOL CardOff();//efg����忨ֹͣ
  int GetCardIndex();
  int GetIOCardIndex();
  //���efg�忨
  //����ɨ��
  int GetOut3();
  int GetOut6();
  //��װ��д//TODO: ���ӿ��޸�
  double ReadDi(EFG_IChannel channel);
  double ReadDi(AC6641_Channel channel);
  void WriteDo(EFG_OChannel channel, double param);
  void WriteDo(AC6641_Channel channel, double param);
  void WritePort(AC6641_Port port, double param);
  double ReadT0(EFG_T0Channel channel);
  void MotoRun(int moto_index, double param);
  void MotoRunNoWait(int moto_index, double param);
  void MotoZero(int moto_index);
  BOOL CheckMotoEnd(int moto_index);
  CString GetMeasureType(int index);
  void InitEfgIO(void);
private:



};

