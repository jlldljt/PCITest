//特殊功能模块，非基础模块
//根据具体的硬件接线，实现转盘一圈正弦曲线的一次捕捉

#include "StdAfx.h"
#include "DiIntCounterSnap.h"
#include <math.h>
#include "EfgAlg.h"


CDiIntCounterSnap::CDiIntCounterSnap()
{
  m_device = -1;
  m_card = NULL;
  m_viewBoard = NULL;
  memset(&m_counter, 0, sizeof(m_counter));
  m_channel = -1;
  // TODO:删除
  for (int i = 0; i < COUNTER_NUM; i++)
  {
    m_counter.counter[0][i] = 50;
    m_counter.counter[1][i] = 100;
    m_counter.counter[2][i] = 50 + i;
    m_counter.counter[3][i] = 100 + i;
  }

  //TODO：调试用
  //for (int i = 0; i < XRAY_ONESHOT_NUM; i++)
  //{
  //  //sin后面不是角度是弧度，故要转换
  //  //我们是匀速的，每个步进弧度都一致即2pi / XRAY_ONESHOT_NUM
  //  m_counter.counter[0][i] = 110 * sin(i * (2 * PI) / XRAY_ONESHOT_NUM + PI) + 370;
  //}
  for (int i = 0; i < XRAY_ONESHOT_NUM; i++)
  {
    //x laser 
    m_counter.counter[0][i] = 100 * sin(i * (2 * PI) / XRAY_ONESHOT_NUM*4+ PI) + 300;
  }
}


CDiIntCounterSnap::~CDiIntCounterSnap()
{
  m_device = -1;
  m_card = NULL;
  m_viewBoard = NULL;
}

void CDiIntCounterSnap::UserFuncLaserSin(void * param)
{
  if (!param)
    return;
  CDiIntCounterSnap* usrParam = (CDiIntCounterSnap*)param;
  usrParam->DIIntLaserSin();
}

void CDiIntCounterSnap::UserFuncLaserCircle(void * param)
{
  if (!param)
    return;
  CDiIntCounterSnap* usrParam = (CDiIntCounterSnap*)param;
  usrParam->DIIntLaserCircle();
}

void CDiIntCounterSnap::UserFuncXRayOneShot(void * param)
{
  if (!param)
    return;
  CDiIntCounterSnap* usrParam = (CDiIntCounterSnap*)param;
  usrParam->DIIntXRayOneShot();
}

#define ENABLE_CNT_NUM 1

void CDiIntCounterSnap::DIIntLaserSin(void)
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

  if (m_counter.index++ >= LASER_SIN_NUM - 1)
  {
    m_counter.flag = 0;

    for (int i = LASER_SIN_NUM - 1; i > 0; i--) {
      for (int j = 0; j < ENABLE_CNT_NUM; j++) {
        m_counter.counter[j][i] -= m_counter.counter[j][i - 1];
      }
    }
    if(m_viewBoard)
      m_viewBoard->DrawLaserSin();

    StopDiInt();
  }
}

void CDiIntCounterSnap::DIIntLaserCircle(void)
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
    m_card->StartT0(m_out, m_device, m_counter.index, 0);//指向下一行
    m_counter.flag = 1;
  }

  if (!m_counter.flag)
    return;

  double fparam;
  //读取计数器
  for (int i = 0; i < ENABLE_CNT_NUM; i++) {
    m_card->ReadT0(i, m_counter.counter[i][m_counter.index], fparam);
  }
  
  if (m_counter.index++ >= LASER_CIRCLE_NUM - 1)
  {
    m_counter.flag = 0;

    for (int i = LASER_CIRCLE_NUM - 1; i > 0; i--) {
      for (int j = 0; j < ENABLE_CNT_NUM; j++) {
        m_counter.counter[j][i] -= m_counter.counter[j][i - 1];
      }
    }
    if (m_viewBoard)
      m_viewBoard->DrawLaserCircle();

    StopDiInt();
  }
  m_card->StopT0(m_out);
  m_card->StartT0(m_out, m_device, m_counter.index, 0);//指向下一行
}

void CDiIntCounterSnap::DIIntXRayOneShot(void)
{
  int delay = 0;
  if (m_device < 0 || !m_card)
    return;
  if (m_counter.start) {
    //memset(&m_counter, 0, sizeof(m_counter));

    for (int i = 0; i < ENABLE_CNT_NUM; i++) {
      m_card->StopT0(i);
      m_card->StartT0(i, m_device, 0, 0);//out6
    }
    m_counter.flag = 1;
  }

  if (!m_counter.flag)
    return;

  double fparam;
  //读取计数器
  for (int i = 0; i < ENABLE_CNT_NUM; i++) {
    //m_card->ReadT0(i, m_counter.counter[i][m_counter.index], fparam);
  }

  if (m_counter.index++ >= XRAY_ONESHOT_NUM - 1)
  {
    m_counter.flag = 0;

    for (int i = XRAY_ONESHOT_NUM - 1; i > 0; i--) {
      for (int j = 0; j < ENABLE_CNT_NUM; j++) {
       // m_counter.counter[j][i] -= m_counter.counter[j][i - 1];
      }
    }
    //TODO：调试拟合
    EfgAlg alg;
   // struct tagSinParam param;
   // alg.FitSinBySubstitution(m_counter.counter[0], XRAY_ONESHOT_NUM, m_counter.counter[1], param);
   // alg.FitSinByLeastSquares(m_counter.counter[0], XRAY_ONESHOT_NUM, m_counter.counter[2], param);
    alg.ExtractSpike(m_counter.counter[0], XRAY_ONESHOT_NUM, 300, 20, -1);

    if (m_viewBoard) {
      m_viewBoard->DrawXRayOneShot();
      ///TODO：调试拟合
      int num = alg.GetSpikesNumber();
      if (num < 10)
      {
        POINT point;
        for (int i = 0; i < num; i++)
        {
          alg.GetSpike(i, point);
          m_viewBoard->DrawCircle(point, 10);
        }
      }
    }

    StopDiInt();
  }
  
}



////////////////////////////////////////////////////////////////////////
int CDiIntCounterSnap::BindCard(int device, CTimeIOCtrl * card, CBoardView *viewBoard)
{
  if (device < 0 || !card)
    return -1;

  m_device = device;
  m_card = card;
  m_viewBoard = viewBoard;
  return 0;
}

int CDiIntCounterSnap::StartDiIntLaserSin(int channel)
{
  if (m_device < 0 || !m_card)
    return -1;
  if (m_card->StartDi(channel, m_device, 0, 0, UserFuncLaserSin, this)) {
    m_channel = channel;
    return 0;
  }
  return -1;
}

int CDiIntCounterSnap::StartDiIntLaserCircle(int channel)
{
  if (m_device < 0 || !m_card)
    return -1;
  if (m_card->StartDi(channel, m_device, 0, 0, UserFuncLaserCircle, this)) {
    m_channel = channel;
    return 0;
  }
  return -1;
}

int CDiIntCounterSnap::StartDiIntXRayOneShot(int channel)
{
  if (m_device < 0 || !m_card)
    return -1;
  if (m_card->StartDi(channel, m_device, 0, 0, UserFuncXRayOneShot, this)) {
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
int CDiIntCounterSnap::StartCaptureXRayOneShot()
{
  if (m_device < 0 || !m_card)
    return -1;

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
