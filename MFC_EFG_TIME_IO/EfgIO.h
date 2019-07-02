#pragma once

#include "MultiCardCtrl.h"

// �޸�
#include "Com.h"
#define USE_EFGV1 1
//#define USE_AC6641 1


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
    double factor_a;//�������ӣ���ӳ����ζ��ľ��� amp = atan (A * factor_a / factor_l) ��factor_l = 1�����Ϊamp = atan (A * factor_a)
    double factor_l;//�������ӣ���ӳ����⵽��ͷ�ľ���
    double offset;//�Ƕ�ƫ��
	double theta_offset;//���չ���ƫ��
	double phi_offset;//���յ���ƫ��
  }laser;
  
  struct {//X�����
    int threshold;//��ֵ
    int confirmNum;//ȷ����������ȳ�����������
    int ignore;//����ֵ�����Ե�ֵ
    double factor_h;//�������ӣ�ƽ������
    double factor_w;//��������,ƽ�����
  }xray;

  struct {
	  int degree[2];//�Ƕ�1 �� 2
	  int step[2];//��Ӧ�Ƕ�12�Ĳ���
  }u_auto;

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
  }user_config;//�û����룬ʹ��ʱ����Ҫת����
  //λ�����

  int auto_run;//�忨�Զ�����
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
  struct//��λ����
  {
    int sort_sec[MAX_SORT_NUM];     // y_off��λ�õĽǶ�,��

  }degree;
  struct//��λ�ǡ�
  {
    double D1, D2, DM, R1;              // x��������λ��
    double pluse_num,min_pluse_num,max_pluse_num;               // ת��������
	  int    pluse_cnt;
    double u_g;                      // ��ʽ���㴦��u���gֵ
    double A, w, t, k, l_num;              // ������
    double cur_phi1, cur_phi0, cur_theta1, cur_theta0, cur_equ; // ��ǰƬ
    double avg_phi1, avg_phi0, avg_theta1, avg_theta0, avg_equ;       // ƽ��
    double std_phi1, std_phi0, std_theta1, std_theta0, std_equ;       // ɢ��
    double std2_phi1, std2_phi0, std2_theta1, std2_theta0, std2_equ;   //�м�ֵ
    int cur_pos;//��ǰƬ��λ
    int cur_pos_step;//��ǰƬ��λ
    int num;//��������
	//������
    double avg_D1, avg_D2, avg_DM, avg_R1, avg_pluse_num, avg_u_g, avg_A, avg_w, avg_t, avg_k;
    double std_D1, std_D2, std_DM, std_R1, std_pluse_num, std_u_g, std_A, std_w, std_t, std_k;
    double std2_D1, std2_D2, std2_DM, std2_R1, std2_pluse_num, std2_u_g, std2_A, std2_w, std2_t, std2_k;
  //��¼����������
    int cnt_num[5];
  }measure;
}EFG_ResultParam;

//���忨�� �޸�
enum EFG_IChannel
{
  READY=0,
  LASER_CNT_GATE= 1,//��������ſ�
  XRAY_CNT_GATE= 2,//x������ſ�
  TURNTABLE_ZERO = 4,//ת����λ
};

enum EFG_OChannel
{
 // U_DIR = 0,//
  CLEAN=0,//=1,
  ALERT,
  BLOW,
  XRAY_GATE,
  X_NOZZLE,
  Y_NOZZLE,
  U_GATE,//
  U_OUTP,//
  U_OUTN,//
  XRAY_CTRL_GATE,//����
  U_EN,//
};
//�����
enum AC6641_Channel
{
  /*X_DIR=4*8,
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
  READY=6*8+7,//���Ͽ���
  U_DIR=10*8,
  U_GO,
  //U_NOZZLE=10*8+3,
  U_STATE = 11 * 8,*/
};
//��port
enum AC6641_Port
{
  /*X_STEP_LOW=0,
  X_STEP_HIGH=1,
  Y_STEP_LOW=2,
  Y_STEP_HIGH=3,
  U_STEP_LOW=8,
  U_STEP_HIGH=9,*/
};

enum EFG_T0Channel
{
  LASER_CNT_START = 4,//���������ʼchannel
  //XRAY_CNT_START = 8,//x�������ʼchannel
  X_MOTOR_CHANNEL = 0,
  Y_MOTOR_CHANNEL = 1,
  U_MOTOR_CHANNEL = 2,
};

enum EFG_T1Channel
{
  XRAY_CNT_START = 0,//x�������ʼchannel

};

//enum EFGV1_T0Channel
//{
//  X_MOTOR_CHANNEL = 0,
//  Y_MOTOR_CHANNEL = 1,
//  U_MOTOR_CHANNEL = 2,
//};

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
  void WriteT0(EFG_T0Channel channel, double param);
  // ���ӿ��޸ģ����������Ϻ��������ģ����������Ӻ�������
  char GetMotoCh(int moto_index);
  void MotoRun(int moto_index, double param);
  int MotoRun(int moto_index, int param);
  void MotoRunNoWait(int moto_index, double param);
  int MotoRunNoWait(int moto_index, int param);
  int MotoZero(int moto_index);
  void UAutoRun(double degree);
  BOOL CheckMotoEnd(int moto_index);
  void InitEfgIO(void);

  int StartMeasure(int cnt);
  BOOL CheckMeasureEnd(void);
  int GetCntDataNum(int no);// ��ȡ���м���ֵ
  int GetAllCntData(int no , char* buf, int len);// ��ȡ���м���ֵ
  int SetOut(int out3, int out6);



  CString GetMeasureType(int index);
  // ��ȡ��ǰ�Ƕȵ�����λ��
  int GetCurOffPos(int &pos_num);
  // �ж�����ǵĵ�λ -1 �� ��=��λ�� ��
  void JudegSortWhich(double sec, int & which);
  void ClearMeasureResult(void);
private:



};

