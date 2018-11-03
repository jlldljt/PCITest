#include "StdAfx.h"
#include "EfgIO.h"


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

  double param0, param1;
  m_multiCardCtrl.LoadParam(L"T0", OUT3_COUNTER, m_on_card_no, param0, param1);
  return param1;
}

int CEfgIO::GetOut6()
{
  if (FALSE == m_on)
    return -1;

  double param0, param1;
  m_multiCardCtrl.LoadParam(L"T0", OUT6_COUNTER, m_on_card_no, param0, param1);
  return param1;
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
  double val = GetPCI(m_on_iocard_no)->ReadDO(port,fparam0,fparam1);
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
void CEfgIO::MotoRun(int moto_index, double param)
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
    Sleep(1);
    while (0 != ReadDi(X_STATE));
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
    Sleep(1);
    while (0 != ReadDi(Y_STATE));
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
    Sleep(1);
    while (0 != ReadDi(U_STATE));
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
  int sel = moto_index;
  int dst = -10000;
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
    while (0 != ReadDi(X_STATE));
    m_configParam.motor.x.pos = 0;
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
    while (0 != ReadDi(Y_STATE));
    m_configParam.motor.y.pos = 0;
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
    while (0 != ReadDi(U_STATE));
    m_configParam.motor.u.pos = 0;
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
  MotoZero(2); 
  MotoZero(1);
  MotoZero(0);

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
  MotoRun(1, m_configParam.user_config.pos.y_wait);
}
