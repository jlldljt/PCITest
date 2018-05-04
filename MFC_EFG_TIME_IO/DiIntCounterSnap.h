#pragma once

#include "TimeIOCtrl.h"
#include "BoardView.h"

#define SIN_NUM 180
#define CIRCLE_NUM 200
#define COUNTER_NUM SIN_NUM + CIRCLE_NUM
struct tagCounter {
  bool start;
  bool flag;
  int index;
  double counter[4][COUNTER_NUM];
};

class CDiIntCounterSnap
{
public:
  CDiIntCounterSnap();
  ~CDiIntCounterSnap();
private:
  //di�жϻص�����
  static void UserFuncSin(void *param);
  static void UserFuncCircle(void *param);
  //�ص��еľ������
  void DIIntSin(void);
  void DIIntCircle(void);

  int m_device;//�豸��
  CTimeIOCtrl *m_card;//����1780 card������
  CBoardView *m_viewBoard;//������ͼ����
  //����������һ֡������غ���
public:
  //ָ��4����������out3 6
  struct tagCounter m_counter;
  //����ز���
  int BindCard(int device, CTimeIOCtrl *card, CBoardView *viewBoard);
  //�����ж�ͨ��
  int StartDiIntSin(int channel);
  int StartDiIntCircle(int channel);
  //����out3 6
  int StartCaptureSin(int no1, double delay1/*out3*/, int no2, double delay2/*out6*/);
  //����һ֡��׽
  int m_out;//out3 out6����һ������������忴����
  int StartCaptureCircle(int out);
  //ֹͣ�жϲ�׽
  int m_channel;
  int StopDiInt();
};

