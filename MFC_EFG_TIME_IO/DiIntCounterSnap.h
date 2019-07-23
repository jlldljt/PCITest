#pragma once

//#include "TimeIOCtrl.h"
//#include "CPCICtrl.h"
#include "EfgIO.h"
#include "BoardView.h"

#define LASER_SIN_NUM 180
#define LASER_CIRCLE_NUM 200
#define XRAY_ONESHOT_NUM 30000
#define COUNTER_NUM LASER_SIN_NUM+LASER_CIRCLE_NUM+XRAY_ONESHOT_NUM // ����������Ϻ궨��
struct tagCounter {
  int start;
  bool flag;
  int index[2];
  double counter[6][COUNTER_NUM]; // ����ʱǰ4�����⣬������x��
  double tmp_counter[6][COUNTER_NUM]; // ����ʱǰ4�����⣬������x��
  double fit[2][COUNTER_NUM];
  unsigned int cnt_num;
};

enum emMotor
{
	MOTOR_STOP = 0,
	MOTOR_RUN,


};

struct tagMotor {
  double cur_step;
  double dst_step;
  double cur_acc;
  double max_acc;
  double max_speed;
  double cur_speed;
  int state;
  bool stop;
};
class CDiIntCounterSnap
{
public:
  CDiIntCounterSnap();
  ~CDiIntCounterSnap();
private:
  //di�жϻص�����
  static void UserFuncLaserSin(void *param);
  static void UserFuncLaserSin_TMC12A(void * param);
  static void UserFuncLaserCircle(void *param);
  static void UserFuncXRayOneShot(void * param);
  //�ص��еľ������
  void DIIntLaserSin(void);
  void DIIntLaserSin_TMC12A(void);
  void DIIntLaserCircle(void);

  void DIIntXRayOneShot(void);

public:
  int m_device;// �豸��
  CPCIBase *m_card;//����1780 card������
  CBoardView *m_viewBoard;//������ͼ����
  CEfgIO *m_efgio;//����efg������
  //����������һ֡������غ���
  //ָ��4����������out3 6
  struct tagCounter m_counter;
  //����ز���
  int BindCard(int device, CPCIBase *card, CBoardView *viewBoard, CEfgIO *efgio = NULL);
  //�����ж�ͨ��
  int StartDiIntLaserSin(int channel);
  int StartDiIntLaserCircle(int channel);
  int StartDiIntXRayOneShot(int channel);
  int StartXRayAndLaser(int channel);
  int StartDiIntMeasure();
  //����out3 6
  int StartCaptureSin(int no1, double delay1/*out3*/, int no2, double delay2/*out6*/);
  //����һ֡��׽
  int m_out;//out3 out6����һ������������忴����
  int StartCaptureCircle(int out);
  int StartCaptureXRayOneShot();
  int StartMeasure();
  int TestS();
  int CheckStart();
  //���ݴ���
  int LaserFit();
  int LaserFit(int n);//���ڶ�β���
  int XrayFit();
  int XrayFit(int n);
  int XrayFitTest(int n);
  //���ڶ�β���
 // int CalcResult();
  //ֹͣ�жϲ�׽
  int m_channel; // �ж�ͨ��
  int StopDiInt();
  //dianji 
  struct tagMotor m_motor_u;
  int StartURunTrd(double step, double acc, double speed);
  
};

