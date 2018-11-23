#include "StdAfx.h"
#include "EfgIO.h"
#include "EfgAlg.h"

CEfgIO::CEfgIO()
{
  memset(&m_configParam, 0, sizeof(m_configParam));
  memset(&m_resultParam, 0, sizeof(m_resultParam));
  CPCICtrl::AllDriverInit();
  m_on = FALSE;
  m_on_card_no = -1;
  m_on_iocard_no = -1;
}


CEfgIO::~CEfgIO()
{
}

int CEfgIO::GetPCINum()
{
  return CPCICtrl::m_num;
}

CString CEfgIO::GetPCIDesc(int cardNo)
{
  DevInf dev;
  if (0 == CPCICtrl::getDevice(cardNo, dev))
    return dev.description;
  else
    return _T("");
}

PCIType CEfgIO::GetPCIType(int cardNo)
{
  DevInf dev;
  if (0 == CPCICtrl::getDevice(cardNo, dev))
    return dev.type;
  else
    return NO_TYPE;
}

int CEfgIO::GetPCIDeviceNumber(int cardNo)
{
  DevInf dev;
  if (0 == CPCICtrl::getDevice(cardNo, dev))
    return dev.deviceNumber;
  else
    return -1;
}

void CEfgIO::InitPCI()
{
  int count = GetPCINum();
  for (int i = 0; i < count; i++) {
    AddPCI(i, GetPCIType(i));
  }
}

BOOL CEfgIO::AddPCI(int cardNo, PCIType type)
{
  if (!m_multiCardCtrl.m_card[cardNo]) {  //CPCICtrl::Delete(m_timeIOCtrl);//出错操作
    m_multiCardCtrl.m_card[cardNo] = CPCICtrl::Create(type);
  }

  if (m_multiCardCtrl.m_card[cardNo])
    return TRUE;
  else
    return FALSE;

}

CPCIBase * CEfgIO::GetPCI(int cardNo)
{
  return m_multiCardCtrl.m_card[cardNo];
}

BOOL CEfgIO::RunPCI(int cardNo)
{
  if (0 == m_multiCardCtrl.Load(cardNo))
    return TRUE;
  else
    return FALSE;
}

BOOL CEfgIO::StopPCI(int cardNo)
{
  if (0 == m_multiCardCtrl.Stop(cardNo))
    return TRUE;
  else
    return FALSE;
}
//打开efg所需要的卡
BOOL CEfgIO::CardOn()
{
  if (TRUE == m_on)
    return TRUE;

  int count = GetPCINum();
  if (!count)
    return FALSE;
  //启动tmc12a
  int index = -1;
  for (int i = 0; i < count; i++) {
    if (TMC12A == GetPCIType(i)) {//查找tmc12a板卡
      index = i;
      break;
    }
  }

  if (-1 == index) {
    return FALSE;
  }
  if (FALSE == RunPCI(index))
    return FALSE;
  //启动ac6641
  int index_io = -1;
  for (int i = 0; i < count; i++) {
    if (AC6641 == GetPCIType(i)) {//查找tmc12a板卡
      index_io = i;
      break;
    }
  }

  if (-1 == index_io) {
    return FALSE;
  }
  if (FALSE == RunPCI(index_io))
    return FALSE;

  m_on = TRUE;
  m_on_card_no = index;
  m_on_iocard_no = index_io;

  return TRUE;
}

BOOL CEfgIO::CardOff()
{

  if (FALSE == m_on)
    return TRUE;

  if (FALSE == StopPCI(m_on_card_no))
    return FALSE;
  if (FALSE == StopPCI(m_on_iocard_no))
    return FALSE;
  m_on = FALSE;
  m_on_card_no = -1;
  m_on_iocard_no = -1;
  return TRUE;
}

int CEfgIO::GetCardIndex()//获取efg板卡的序号
{
  return m_on_card_no;
}
int CEfgIO::GetIOCardIndex()//获取io板卡的序号
{
  return m_on_iocard_no;
}

int CEfgIO::GetOut3()
{
  if (FALSE == m_on)
    return -1;

  //double param0, param1;
  //m_multiCardCtrl.LoadParam(L"T0", OUT3_COUNTER, m_on_card_no, param0, param1);
  return m_configParam.laser.out3;
}

int CEfgIO::GetOut6()
{
  if (FALSE == m_on)
    return -1;


  return m_configParam.laser.out6;
}

double CEfgIO::ReadDi(EFG_IChannel channel)
{
  ASSERT(-1 != m_on_iocard_no);
  double fparam0, fparam1;
  GetPCI(m_on_card_no)->ReadDi(channel, fparam0, fparam1);
  return fparam0;
}

double CEfgIO::ReadDi(AC6641_Channel channel)
{
  ASSERT(-1 != m_on_iocard_no);
  int index = channel;
  int port = channel / 8;
  int bit = index % 8;
  double fparam0, fparam1;
  double val = GetPCI(m_on_iocard_no)->ReadDO(port, fparam0, fparam1);

  fparam1 = ((int)fparam0) & (0x1 << bit);

  return fparam1;
}

void CEfgIO::WriteDo(EFG_OChannel channel, double param)
{
  ASSERT(-1 != m_on_iocard_no);
  GetPCI(m_on_card_no)->WriteDO(channel, param);
}

CCriticalSection ac6641_critical;
void CEfgIO::WriteDo(AC6641_Channel channel, double param)
{
  ASSERT(-1 != m_on_iocard_no);
  int index = channel;
  int port = channel / 8;
  int bit = index % 8;
  double fparam0, fparam1;
  ac6641_critical.Lock();
  double val = GetPCI(m_on_iocard_no)->ReadDO(port, fparam0, fparam1);
  if (param > 0)
  {
    fparam1 = ((int)fparam0) | (0x1 << bit);
  }
  else
  {
    fparam1 = ((int)fparam0) & ~(0x1 << bit);
  }
  GetPCI(m_on_iocard_no)->WriteDO(port, fparam1);
  ac6641_critical.Unlock();
}

void CEfgIO::WritePort(AC6641_Port port, double param)
{
  ASSERT(-1 != m_on_iocard_no);
  ac6641_critical.Lock();
  GetPCI(m_on_iocard_no)->WriteDO(port, param);
  ac6641_critical.Unlock();
}

double CEfgIO::ReadT0(EFG_T0Channel channel)
{
  double fparam0, fparam1;
  GetPCI(m_on_card_no)->ReadT0(channel, fparam0, fparam1);
  return fparam1;
}
//0 x;1 y;2 u
#define MAX_STEP 65000
void CEfgIO::MotoRun(int moto_index, double param)
{
  int sel = moto_index;
  int dst = param;
  switch (sel) {
  case 0:
    m_configParam.motor.x.dst_pos = dst;
    if (dst == 0)
      dst = -65000;
    else
      dst = m_configParam.motor.x.dst_pos - m_configParam.motor.x.pos;
    //设定方向
    if (dst > 0)
      WriteDo(X_DIR, IO_ON);
    else if (dst < 0)
      WriteDo(X_DIR, IO_OFF);
    else
      break;
    dst = abs(dst);
    while (dst > 0)
    {
      if (dst > MAX_STEP)
      {

        //设定步数
        WritePort(X_STEP_LOW, MAX_STEP);
        WritePort(X_STEP_HIGH, MAX_STEP >> 8);
      }
      else
      {

        //设定步数
        WritePort(X_STEP_LOW, dst);
        WritePort(X_STEP_HIGH, dst >> 8);
      }
      dst -= MAX_STEP;
      //下降沿
      WriteDo(X_FULL_CURRENT, IO_OFF);
      WriteDo(X_GO, IO_ON);
      Sleep(1);
      WriteDo(X_GO, IO_OFF);
      WriteDo(X_FULL_CURRENT, IO_ON);
      Sleep(1);
      while (IO_OFF != ReadDi(X_STATE));
    }
    m_configParam.motor.x.pos = m_configParam.motor.x.dst_pos;
    break;
  case 1:
    m_configParam.motor.y.dst_pos = dst;
    if (dst == 0)
      dst = -65000;
    else
      dst = m_configParam.motor.y.dst_pos - m_configParam.motor.y.pos;
    //设定方向
    if (dst > 0)
      WriteDo(Y_DIR, IO_ON);
    else if (dst < 0)
      WriteDo(Y_DIR, IO_OFF);
    else
      break;
    dst = abs(dst);
    while (dst > 0)
    {
      if (dst > MAX_STEP)
      {

        //设定步数
        WritePort(Y_STEP_LOW, MAX_STEP);
        WritePort(Y_STEP_HIGH, MAX_STEP >> 8);
      }
      else
      {

        //设定步数
        WritePort(Y_STEP_LOW, dst);
        WritePort(Y_STEP_HIGH, dst >> 8);
      }
      dst -= MAX_STEP;

      //下降沿
      WriteDo(Y_FULL_CURRENT, IO_OFF);
      WriteDo(Y_GO, IO_ON);
      Sleep(1);
      WriteDo(Y_GO, IO_OFF);
      WriteDo(Y_FULL_CURRENT, IO_ON);
      Sleep(1);
      while (IO_OFF != ReadDi(Y_STATE));
    }
    m_configParam.motor.y.pos = m_configParam.motor.y.dst_pos;
    break;
  case 2:
    m_configParam.motor.u.dst_pos = dst;
    if (dst == 0)
      dst = -65000;
    else
      dst = m_configParam.motor.u.dst_pos - m_configParam.motor.u.pos;
    //设定方向
    if (dst > 0)
      WriteDo(U_DIR, IO_ON);
    else if (dst < 0)
      WriteDo(U_DIR, IO_OFF);
    else
      break;
    dst = abs(dst);
    while (dst > 0)
    {
      if (dst > MAX_STEP)
      {

        //设定步数
        WritePort(U_STEP_LOW, MAX_STEP);
        WritePort(U_STEP_HIGH, MAX_STEP >> 8);
      }
      else
      {

        //设定步数
        WritePort(U_STEP_LOW, dst);
        WritePort(U_STEP_HIGH, dst >> 8);
      }
      dst -= MAX_STEP;

      //下降沿
      WriteDo(U_GO, IO_ON);
      Sleep(1);
      WriteDo(U_GO, IO_OFF);
      Sleep(1);
      while (IO_OFF != ReadDi(U_STATE));
    }
    m_configParam.motor.u.pos = m_configParam.motor.u.dst_pos;
    break;
  default:
    return;
  }
  Sleep(1);
}

void CEfgIO::MotoRunNoWait(int moto_index, double param)
{
  int sel = moto_index;
  int dst = param;
  switch (sel) {
  case 0:
    m_configParam.motor.x.dst_pos = dst;
    dst = m_configParam.motor.x.dst_pos - m_configParam.motor.x.pos;
    //设定方向
    if (dst > 0)
      WriteDo(X_DIR, IO_ON);
    else
      WriteDo(X_DIR, IO_OFF);
    dst = abs(dst);
    //设定步数
    WritePort(X_STEP_LOW, dst);
    WritePort(X_STEP_HIGH, dst >> 8);
    //下降沿
    WriteDo(X_FULL_CURRENT, IO_OFF);
    WriteDo(X_GO, IO_ON);
    Sleep(1);
    WriteDo(X_GO, IO_OFF);
    WriteDo(X_FULL_CURRENT, IO_ON);
    m_configParam.motor.x.pos = m_configParam.motor.x.dst_pos;
    break;
  case 1:
    m_configParam.motor.y.dst_pos = dst;
    dst = m_configParam.motor.y.dst_pos - m_configParam.motor.y.pos;
    //设定方向
    if (dst > 0)
      WriteDo(Y_DIR, IO_ON);
    else
      WriteDo(Y_DIR, IO_OFF);
    dst = abs(dst);
    //设定步数
    WritePort(Y_STEP_LOW, dst);
    WritePort(Y_STEP_HIGH, dst >> 8);
    //下降沿
    WriteDo(Y_FULL_CURRENT, IO_OFF);
    WriteDo(Y_GO, IO_ON);
    Sleep(1);
    WriteDo(Y_GO, IO_OFF);
    WriteDo(Y_FULL_CURRENT, IO_ON);
    m_configParam.motor.y.pos = m_configParam.motor.y.dst_pos;
    break;
  case 2:
    m_configParam.motor.u.dst_pos = dst;
    dst = m_configParam.motor.u.dst_pos - m_configParam.motor.u.pos;
    //设定方向
    if (dst > 0)
      WriteDo(U_DIR, IO_ON);
    else
      WriteDo(U_DIR, IO_OFF);
    dst = abs(dst);
    //设定步数
    WritePort(U_STEP_LOW, dst);
    WritePort(U_STEP_HIGH, dst >> 8);
    //下降沿
    WriteDo(U_GO, IO_ON);
    Sleep(1);
    WriteDo(U_GO, IO_OFF);
    m_configParam.motor.u.pos = m_configParam.motor.u.dst_pos;
    break;
  default:
    return;
  }

}

void CEfgIO::MotoZero(int moto_index)
{
  MotoRun(moto_index, 0);
  return;
  int sel = moto_index;
  int dst = -65000;

  switch (sel) {
  case 0:
    m_configParam.motor.x.dst_pos = dst;
    dst = m_configParam.motor.x.dst_pos - m_configParam.motor.x.pos;
    //设定方向
    if (dst > 0)
      WriteDo(X_DIR, IO_ON);
    else
      WriteDo(X_DIR, IO_OFF);
    dst = abs(dst);
    //设定步数
    WritePort(X_STEP_LOW, dst);
    WritePort(X_STEP_HIGH, dst >> 8);
    //下降沿
    WriteDo(X_FULL_CURRENT, IO_OFF);
    WriteDo(X_GO, IO_ON);
    Sleep(1);
    WriteDo(X_GO, IO_OFF);
    WriteDo(X_FULL_CURRENT, IO_ON);
    Sleep(1);
    while (IO_OFF != ReadDi(X_STATE));
    m_configParam.motor.x.pos = 0;
    m_configParam.motor.x.dst_pos = 0;
    break;
  case 1:
    m_configParam.motor.y.dst_pos = dst;
    dst = m_configParam.motor.y.dst_pos - m_configParam.motor.y.pos;
    //设定方向
    if (dst > 0)
      WriteDo(Y_DIR, IO_ON);
    else
      WriteDo(Y_DIR, IO_OFF);
    dst = abs(dst);
    //设定步数
    WritePort(Y_STEP_LOW, dst);
    WritePort(Y_STEP_HIGH, dst >> 8);
    //下降沿
    WriteDo(Y_FULL_CURRENT, IO_OFF);
    WriteDo(Y_GO, IO_ON);
    Sleep(1);
    WriteDo(Y_GO, IO_OFF);
    WriteDo(Y_FULL_CURRENT, IO_ON);
    Sleep(1);
    while (IO_OFF != ReadDi(Y_STATE));
    m_configParam.motor.y.pos = 0;
    m_configParam.motor.y.dst_pos = 0;
    break;
  case 2:
    m_configParam.motor.u.dst_pos = dst;
    dst = m_configParam.motor.u.dst_pos - m_configParam.motor.u.pos;
    //设定方向
    if (dst > 0)
      WriteDo(U_DIR, IO_ON);
    else
      WriteDo(U_DIR, IO_OFF);
    dst = abs(dst);
    //设定步数
    WritePort(U_STEP_LOW, dst);
    WritePort(U_STEP_HIGH, dst >> 8);
    //下降沿
    WriteDo(U_GO, IO_ON);
    Sleep(1);
    WriteDo(U_GO, IO_OFF);
    Sleep(1);
    while (IO_OFF != ReadDi(U_STATE));
    m_configParam.motor.u.pos = 0;
    m_configParam.motor.u.dst_pos = 0;
    break;
  default:
    return;
  }

}

BOOL CEfgIO::CheckMotoEnd(int moto_index)
{
  int sel = moto_index;
  double val;
  switch (sel) {
  case 0:
    val = ReadDi(X_STATE);
    break;
  case 1:
    val = ReadDi(Y_STATE);
    break;
  case 2:
    val = ReadDi(U_STATE);
    break;
  default:
    return TRUE;
  }
  return val ? TRUE : FALSE;
}

CString CEfgIO::GetMeasureType(int index)
{
  switch (index)
  {
  case 1:
    return _T("光轴");
  case 2:
    return _T("电轴");
  case 3:
    return _T("等效角");
  default:
    return _T("无");
  }
}

void CEfgIO::InitEfgIO(void)
{
  //MotoZero(2); 
  MotoZero(0);
  MotoZero(1);

  WriteDo(X_DIR, IO_OFF);
  WriteDo(X_GO, IO_OFF);
  WriteDo(X_NOZZLE, IO_OFF);
  WriteDo(Y_DIR, IO_OFF);
  WriteDo(Y_GO, IO_OFF);
  WriteDo(Y_NOZZLE, IO_OFF);
  WriteDo(BLOW, IO_OFF);
  WriteDo(XRAY_GATE, IO_OFF);
  WriteDo(X_FULL_CURRENT, IO_OFF);
  WriteDo(Y_FULL_CURRENT, IO_OFF);
  WriteDo(ALERT, IO_OFF);
  WriteDo(CLEAN, IO_OFF);
  WriteDo(U_DIR, IO_OFF);
  WriteDo(U_GO, IO_OFF);

  WriteDo(Y_NOZZLE, IO_ON);
  MotoRun(0, m_configParam.user_config.pos.x_wait);
  MotoRun(1, m_configParam.user_config.pos.not_detected);
  WriteDo(Y_NOZZLE, IO_OFF);
  Sleep(m_configParam.user_config.time.y_off);//延时
  MotoRun(1, m_configParam.user_config.pos.y_wait);

}
//返回档位，pos_num返回该档位的计数值，pos_step返回该档位的位置
int CEfgIO::GetCurOffPos(int &pos_num)
{
  double primary_degree = 0;
  double secondary_degree = 0;
  int pos = -1;
  pos_num = -1;
  int sort_type = m_configParam.user_config.type;//晶片类型
  ASSERT(sort_type < MAX_TYPE_NUM);
  int sort_num = m_configParam.user_config.type_sort_num[sort_type];//档位数量

  switch (m_configParam.user_config.measure.primary.type)//判断主要测量类型
  {
  case 1:
    primary_degree = m_resultParam.measure.cur_theta1;
    break;
  case 2:
    primary_degree = m_resultParam.measure.cur_phi1;
    break;
  case 3:
    primary_degree = m_resultParam.measure.cur_equ;
    break;
  default://同未检出
    m_resultParam.measure.cur_pos = sort_num + 4;
    m_resultParam.num.not_detected++;
    pos = m_resultParam.measure.cur_pos;
    pos_num = m_resultParam.num.not_detected;
    m_resultParam.measure.cur_pos_step = m_configParam.user_config.pos.not_detected;
    return pos;
  }

  switch (m_configParam.user_config.measure.secondary.type)//判断主要测量类型
  {
  case 1:
    secondary_degree = m_resultParam.measure.cur_theta1;
    break;
  case 2:
    secondary_degree = m_resultParam.measure.cur_phi1;
    break;
  case 3:
    secondary_degree = m_resultParam.measure.cur_equ;
    break;
  default:
    break;
  }

  // 未检出
  if (0 == primary_degree /*|| 0 == secondary_degree*/)
  {
    m_resultParam.measure.cur_pos = sort_num + 4;
    m_resultParam.num.not_detected++;
    pos = m_resultParam.measure.cur_pos;
    pos_num = m_resultParam.num.not_detected;
    m_resultParam.measure.cur_pos_step = m_configParam.user_config.pos.not_detected;
    return pos;
  }
  if (m_configParam.user_config.measure.secondary.type != 0)
  {
    // 超上限
    if (secondary_degree > USER_TO_DEG(m_configParam.user_config.measure.secondary.max_degree))
    {
      m_resultParam.measure.cur_pos = sort_num + 3;
      m_resultParam.num.secondary_p++;
      pos = m_resultParam.measure.cur_pos;
      pos_num = m_resultParam.num.secondary_p;
      m_resultParam.measure.cur_pos_step = m_configParam.user_config.pos.secondary_p;
      return pos;
    }
    // 超下限
    if (secondary_degree < USER_TO_DEG(m_configParam.user_config.measure.secondary.min_degree))
    {
      m_resultParam.measure.cur_pos = sort_num + 2;
      m_resultParam.num.secondary_n++;
      pos = m_resultParam.measure.cur_pos;
      pos_num = m_resultParam.num.secondary_n;
      m_resultParam.measure.cur_pos_step = m_configParam.user_config.pos.secondary_n;
      return pos;
    }
  }
  // 判断在那个档
  JudegSortWhich(DEG_TO_SEC(primary_degree), m_resultParam.measure.cur_pos);

  if (m_resultParam.measure.cur_pos == -1)
  {
    m_resultParam.measure.cur_pos = sort_num;
    m_resultParam.num.primary_n++;
    pos = m_resultParam.measure.cur_pos;
    pos_num = m_resultParam.num.primary_n;
    m_resultParam.measure.cur_pos_step = m_configParam.user_config.pos.primary_n;
    return pos;
  }

  if (m_resultParam.measure.cur_pos == sort_num)
  {
    m_resultParam.measure.cur_pos = sort_num + 1;
    m_resultParam.num.primary_p++;
    pos = m_resultParam.measure.cur_pos;
    pos_num = m_resultParam.num.primary_p;
    m_resultParam.measure.cur_pos_step = m_configParam.user_config.pos.primary_p;
    return pos;
  }
  m_resultParam.num.y_off[m_resultParam.measure.cur_pos]++;
  pos = m_resultParam.measure.cur_pos;
  pos_num = m_resultParam.num.y_off[m_resultParam.measure.cur_pos];
  m_resultParam.measure.cur_pos_step = m_configParam.user_config.pos.y_off[sort_type][pos];
  return pos;
}

// sec 待判断角度
//which 结果 档位值
void CEfgIO::JudegSortWhich(double sec, int &which)
{
  int sort_type = m_configParam.user_config.type;//晶片类型
  ASSERT(sort_type < MAX_TYPE_NUM);
  int sort_num = m_configParam.user_config.type_sort_num[sort_type];//档位数量

  int *sort_sec = m_resultParam.degree.sort_sec;//各档位中间值
  int step_sec = m_configParam.user_config.measure.primary.step_degree;

  double d_sec = double(step_sec) / 2;
  if (sec < sort_sec[0] - d_sec)
  {
    which = -1;
    return;
  }

  //if (sec > sort_sec[sort_num - 1] + d_sec)
  //{
  //  which = sort_num;
  //  return;
  //}

  int i;
  for (i = 0; i < sort_num; i++)
  {
    if (sec < sort_sec[i] + d_sec)
      break;
  }

  which = i;

}

void CEfgIO::ClearMeasureResult(void)
{
  memset(&(m_resultParam.measure), 0, sizeof(m_resultParam.measure));
}