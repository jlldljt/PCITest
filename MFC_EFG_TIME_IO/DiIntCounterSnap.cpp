//���⹦��ģ�飬�ǻ���ģ��
//���ݾ����Ӳ�����ߣ�ʵ��ת��һȦ�������ߵ�һ�β�׽

#include "StdAfx.h"
#include "DiIntCounterSnap.h"


CDiIntCounterSnap::CDiIntCounterSnap()
{
  m_device = -1;
  m_card = NULL;
  m_viewBoard = NULL;
  memset(&m_counter, 0, sizeof(m_counter));
  m_channel = -1;
  // TODO:ɾ��
  for (int i = 0; i < COUNTER_NUM; i++)
  {
    m_counter.counter[0][i] = 50;
    m_counter.counter[1][i] = 100;
    m_counter.counter[2][i] = 50 + i;
    m_counter.counter[3][i] = 100 + i;
  }
}


CDiIntCounterSnap::~CDiIntCounterSnap()
{
  m_device = -1;
  m_card = NULL;
  m_viewBoard = NULL;
}

void CDiIntCounterSnap::UserFuncSin(void * param)
{
  if (!param)
    return;
  CDiIntCounterSnap* usrParam = (CDiIntCounterSnap*)param;
  usrParam->DIIntSin();
}

void CDiIntCounterSnap::UserFuncCircle(void * param)
{
  if (!param)
    return;
  CDiIntCounterSnap* usrParam = (CDiIntCounterSnap*)param;
  usrParam->DIIntCircle();
}

#define ENABLE_CNT_NUM 1

void CDiIntCounterSnap::DIIntSin(void)
{
  if (m_device < 0 || !m_card)
    return;
  if (m_counter.start) {
    memset(&m_counter, 0, sizeof(m_counter));

    for (int i = 0; i < ENABLE_CNT_NUM; i++) {
      m_card->StopT0(i);
      m_card->StartT0(i, m_device, 0, 0);//out6
    }
    m_counter.flag = 1;
  }

  if (!m_counter.flag)
    return;

  double fparam;
  for (int i = 0; i < ENABLE_CNT_NUM; i++) {
    m_card->ReadT0(i, m_counter.counter[i][m_counter.index], fparam);
  }

  if (m_counter.index++ >= SIN_NUM - 1)
  {
    m_counter.flag = 0;

    for (int i = SIN_NUM - 1; i > 0; i--) {
      for (int j = 0; j < ENABLE_CNT_NUM; j++) {
        m_counter.counter[j][i] -= m_counter.counter[j][i - 1];
      }
    }
    if(m_viewBoard)
      m_viewBoard->DrawSin();

    StopDiInt();
  }
}

void CDiIntCounterSnap::DIIntCircle(void)
{
  int delay = 0;
  if (m_device < 0 || !m_card)
    return;
  if (m_counter.start) {
    memset(&m_counter, 0, sizeof(m_counter));

    for (int i = 0; i < ENABLE_CNT_NUM; i++) {
      m_card->StopT0(i);
      m_card->StartT0(i, m_device, 0, 0);//out6
    }
    m_card->StopT0(m_out);
    m_card->StartT0(m_out, m_device, m_counter.index, 0);//ָ����һ��
    m_counter.flag = 1;
  }

  if (!m_counter.flag)
    return;

  double fparam;
  //��ȡ������
  for (int i = 0; i < ENABLE_CNT_NUM; i++) {
    m_card->ReadT0(i, m_counter.counter[i][m_counter.index], fparam);
  }
  
  if (m_counter.index++ >= CIRCLE_NUM - 1)
  {
    m_counter.flag = 0;

    for (int i = CIRCLE_NUM - 1; i > 0; i--) {
      for (int j = 0; j < ENABLE_CNT_NUM; j++) {
        m_counter.counter[j][i] -= m_counter.counter[j][i - 1];
      }
    }
    if (m_viewBoard)
      m_viewBoard->DrawCircle();

    StopDiInt();
  }
  m_card->StopT0(m_out);
  m_card->StartT0(m_out, m_device, m_counter.index, 0);//ָ����һ��
}

int CDiIntCounterSnap::BindCard(int device, CTimeIOCtrl * card, CBoardView *viewBoard)
{
  if (device < 0 || !card)
    return -1;

  m_device = device;
  m_card = card;
  m_viewBoard = viewBoard;
  return 0;
}

int CDiIntCounterSnap::StartDiIntSin(int channel)
{
  if (m_device < 0 || !m_card)
    return -1;
  if (m_card->StartDi(channel, m_device, 0, 0, UserFuncSin, this)) {
    m_channel = channel;
    return 0;
  }
  return -1;
}

int CDiIntCounterSnap::StartDiIntCircle(int channel)
{
  if (m_device < 0 || !m_card)
    return -1;
  if (m_card->StartDi(channel, m_device, 0, 0, UserFuncCircle, this)) {
    m_channel = channel;
    return 0;
  }
  return -1;
}

int CDiIntCounterSnap::StartCaptureSin(int no1, double delay1/*out3*/, int no2, double delay2/*out6*/)
{
  if (m_device < 0 || !m_card)
    return -1;
  m_card->StopT0(no1);//out3
  m_card->StopT0(no2);//out6

  m_card->StartT0(no1, m_device, delay1, 0);//out3
  m_card->StartT0(no2, m_device, delay2, 0);//out6
  m_counter.start = 1;
  return 0;
}

int CDiIntCounterSnap::StartCaptureCircle(int out)
{
  if (m_device < 0 || !m_card)
    return -1;
  m_out = out;
  m_counter.start = 1;
  return 0;
}

int CDiIntCounterSnap::StopDiInt()
{
  if (m_channel < 0 || !m_card)
    return -1;
  if (m_card->StopDi(m_channel)) {
    m_channel = -1;
    return 0;
  }
  return -1;
}
