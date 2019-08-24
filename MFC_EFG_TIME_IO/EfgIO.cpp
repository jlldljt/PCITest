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
  measure_status = 0;
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
//打开efg所需要的卡，修改
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
    if (EFG_V1/*TMC12A*/ == GetPCIType(i)) {//查找tmc12a板卡
      index = i;
      break;
    }
  }

  if (-1 == index) {
    return FALSE;
  }
  if (FALSE == RunPCI(index))
    return FALSE;
  //启动ac6641  efgv1不需要额外io板
 int index_io = -1;
 /*  for (int i = 0; i < count; i++) {
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
	*/
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
  /*if (FALSE == StopPCI(m_on_iocard_no))
    return FALSE;*/
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
  ASSERT(-1 != m_on_card_no);
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
  ASSERT(-1 != m_on_card_no);
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
void CEfgIO::WriteT0(EFG_T0Channel channel, double param)
{
  double fparam0= param, fparam1;
  GetPCI(m_on_card_no)->WriteT0(channel, fparam0, fparam1);
  return;
}
//0 x;1 y;2 u
#define MAX_STEP 65000
void CEfgIO::MotoRun(int moto_index, double param)
{
#ifdef USE_EFGV1
  MotoRun(moto_index,int(param));
#elif USE_AC6641
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

#endif
}

char CEfgIO::GetMotoCh(int moto_index)
{
	char moto;
	switch(moto_index)
	{
	case 0:
		moto='X';
		break;
		
	case 1:
		moto='Y';
		break;
		
	case 2:
		moto='U';
		break;
		
	default:
		moto='X';
		break;
		
	}
	return moto;
}

int CEfgIO::MotoRun(int moto_index, int param)
{
#ifdef USE_EFGV1

  char sbuf[9] = { 0x55, 'M', GetMotoCh(moto_index), 'M', param >> 24, param >> 16, param >> 8, param,0x00 };
  int slen = sizeof(sbuf);
  unsigned char rbuf[100] = { 0 };
  int rlen = 100;
  bool crc = 0;
  int rdlen = 0;

  rdlen = m_com.SendAndRecv(sbuf, slen, (char*)rbuf, rlen, &crc);

  if (rdlen > 0 && crc)
  {

    while (!CheckMotoEnd(moto_index));
    return 0;
  }


  return -1;
#endif
}

void CEfgIO::MotoRunNoWait(int moto_index, double param)
{
#ifdef USE_EFGV1
  MotoRunNoWait(moto_index, int(param));
#elif USE_AC6641

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

#endif
}

int CEfgIO::MotoRunNoWait(int moto_index, int param)
{
#ifdef USE_EFGV1
  char sbuf[9] = { 0x55, 'M', GetMotoCh(moto_index), 'M', param>>24, param >>16, param >> 8, param >> 0,0x00 };
  int slen = sizeof(sbuf);
  unsigned char rbuf[100] = { 0 };
  int rlen = 100;
  bool crc = 0;
  int rdlen = 0;

  rdlen = m_com.SendAndRecv(sbuf, slen, (char*)rbuf, rlen, &crc);

  if (rdlen > 0 && crc)
  {
    return 0;
  }

  return -1;
#endif
}

int CEfgIO::MotoZero(int moto_index)
{
#ifdef USE_EFGV1

  char sbuf[9] = { 0x55,'M', GetMotoCh(moto_index), 'Z',0x00,0x00 };
  int slen = sizeof(sbuf);
  unsigned char rbuf[100] = { 0 };
  int rlen = 100;
  bool crc = 0;
  int rdlen = 0;

  rdlen = m_com.SendAndRecv(sbuf, slen, (char*)rbuf, rlen, &crc);

  if (rdlen > 0 && crc)
  {
	  //Sleep(100);
	  while (!CheckMotoEnd(moto_index));
    return 0;
  }
 
  return -1;
#elif USE_AC6641

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

#endif

}

void CEfgIO::UAutoRun(double degree)
{
	if(USER_TO_DEG(m_configParam.u_auto.degree[1]) < USER_TO_DEG(m_configParam.u_auto.degree[0])||m_configParam.u_auto.step[1] < m_configParam.u_auto.step[0])
	{
		AfxMessageBox(_T("u范围设置不正确"));
		return;
	}
	if(degree < USER_TO_DEG(m_configParam.u_auto.degree[0])||degree > USER_TO_DEG(m_configParam.u_auto.degree[1]))
	{
		AfxMessageBox(_T("不在范围内"));
		return;
	}
	double stepofdeg = abs(m_configParam.u_auto.step[0]-m_configParam.u_auto.step[1]) /
		fabs(USER_TO_DEG(m_configParam.u_auto.degree[0])-USER_TO_DEG(m_configParam.u_auto.degree[1]));
	double steps = (degree - USER_TO_DEG(m_configParam.u_auto.degree[0]))*stepofdeg+m_configParam.u_auto.step[0];

	MotoZero(2);
	Sleep(1000);
	MotoRun(2, steps);
}

BOOL CEfgIO::CheckMotoEnd(int moto_index)
{
#ifdef USE_EFGV1
  char sbuf[9] = { 0x55, 'M',GetMotoCh(moto_index),	'Q', 0x00, 0x00 };
  int slen = sizeof(sbuf);
  unsigned char rbuf[100] = { 0 };
  int rlen = 100;
  bool crc = 0;
  int rdlen = 0;

  rdlen = m_com.SendAndRecv(sbuf, slen, (char*)rbuf, rlen, &crc);

  if (rdlen > 8 && crc && rbuf[4]==0x00)
  {
    return TRUE;
  }
  return FALSE;

#elif USE_AC6641

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

#endif
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
// 修改
void CEfgIO::InitEfgIO(void)
{
  //MotoZero(2); 
  MotoZero(0);
  MotoZero(1);

  //WriteDo(X_DIR, IO_OFF);
  //WriteDo(X_GO, IO_OFF);
  WriteDo(X_NOZZLE, IO_OFF);
  //WriteDo(Y_DIR, IO_OFF);
  //WriteDo(Y_GO, IO_OFF);
  WriteDo(Y_NOZZLE, IO_OFF);
  WriteDo(BLOW, IO_OFF);
  WriteDo(XRAY_GATE, IO_OFF);
  //WriteDo(X_FULL_CURRENT, IO_OFF);
  //WriteDo(Y_FULL_CURRENT, IO_OFF);
  WriteDo(ALERT, IO_OFF);
  WriteDo(CLEAN, IO_OFF);
  //WriteDo(U_DIR, IO_OFF);
 // WriteDo(U_GO, IO_OFF);

  WriteDo(Y_NOZZLE, IO_ON);
  MotoRun(0, m_configParam.user_config.pos.x_wait);
  MotoRun(1, m_configParam.user_config.pos.not_detected);
  WriteDo(Y_NOZZLE, IO_OFF);
  Sleep(m_configParam.user_config.time.y_off);//延时
  MotoRun(1, m_configParam.user_config.pos.y_wait);

}
int CEfgIO::StartMeasure(int cnt)
{
  char sbuf[9] = { 0x55, 'E', 'S', 0, 0x00, 0x00 };
  int slen = sizeof(sbuf);
  unsigned char rbuf[100] = { 0 };
  int rlen = 100;
  bool crc = 0;
  int rdlen = 0;
  measure_status = 0;
  char status;

  rdlen = m_com.SendAndRecv(sbuf, slen, (char*)rbuf, rlen, &crc);

  if (rdlen > 0 && crc)
  {
	  ////
	 // int wait = 0;
	 // while(1 != (status=GetMeasureStatus()) && wait++ < 80)//等待直到1或者Xs，大约7s
	 // {
		////Sleep(10);
	 // };

	 // if(wait >= 80)//超时
		//  return -1;
	  if(1 != (status=GetMeasureStatus()))//等待直到1或者Xs，大约7s
	  {
		return -1;
	  };
	  /////
	  measure_status = status;//2
    return 0;
  }

  return -1;
}

int CEfgIO::StopMeasure()
{
  char sbuf[9] = { 0x55, 'E', 'E', 0, 0x00, 0x00 };
  int slen = sizeof(sbuf);
  unsigned char rbuf[100] = { 0 };
  int rlen = 100;
  bool crc = 0;
  int rdlen = 0;
  char status;

  rdlen = m_com.SendAndRecv(sbuf, slen, (char*)rbuf, rlen, &crc);

  if (rdlen > 0 && crc)
  {
	  measure_status = 0;//2
    return 0;
  }

  return -1;
}

BOOL CEfgIO::CheckMeasureEnd(void)
{
  char sbuf[9] = { 0x55,'E', 'Q', 0x00, 0x00 };
  int slen = sizeof(sbuf);
  unsigned char rbuf[100] = { 0 };
  int rlen = 100;
  bool crc = 0;
  int rdlen = 0;

  rdlen = m_com.SendAndRecv(sbuf, slen, (char*)rbuf, rlen, &crc);

  if (rdlen > 2 && crc && rbuf[3] == 0)
  {
    return TRUE;
  }

  return FALSE;
}
//返回测量到第几圈
char CEfgIO::GetMeasureStatus(void)
{
  char sbuf[9] = { 0x55,'E', 'Q', 0x00, 0x00 };
  int slen = sizeof(sbuf);
  unsigned char rbuf[100] = { 0 };
  int rlen = 100;
  bool crc = 0;
  int rdlen = 0;

  rdlen = m_com.SendAndRecv(sbuf, slen, (char*)rbuf, rlen, &crc);

  if (rdlen > 2 && crc)
  {
		return rbuf[3];
    
  }

  return -1;
}
//返回已经测量好的圈
char CEfgIO::CheckChangedStatus(void)
{
  ////

	  int wait = 0;
	  char pre;
	  do//等待直到2或者3s
	  {
		  pre=GetMeasureStatus();

		  switch(pre){
		  case -1:
			  return -1;
		  case 0x0E://error
		  case 0x00:
			  StartMeasure(1);//重新启动
			  return -1;
		  default:
			  if(measure_status!=pre)
			  {
				  measure_status = pre;
				  return pre-1 == 0?3:pre-1;
			  }
		  }

		//Sleep(10);
	  }while(wait++<80);

	  if(wait >= 80)//超时
		  return -1;
	  /////

  return -1;
}
// no 0 - 4
// result in m_resultParam.measure.cnt_num[no]
// return num>=0 success
// return -1 failure
int CEfgIO::GetCntDataNum(int no)
{
  char sbuf[9] = { 0x55, 'C',	'N', no, 0x00 , 0x00 };
  int slen = sizeof(sbuf);
  unsigned char rbuf[100] = { 0 };
  int rlen = 100;
  bool crc = 0;
  int rdlen = 0;

  rdlen = m_com.SendAndRecv(sbuf, slen, (char*)rbuf, rlen, &crc,1);

  if (rdlen > 5 && crc)
  {
    m_resultParam.measure.cnt_num[no] = (rbuf[4] << 8) | (rbuf[5]);
    return m_resultParam.measure.cnt_num[no];
  }
  return -1;
}

// no 0 - 4
// result in m_resultParam.measure.cnt_num[no]
// return num>=0 success
// return -1 failure
int CEfgIO::GetCntDataNum(int no, int shift)
{
  char sbuf[9] = { 0x55, 'C',	'N', shift, 0x00 , 0x00 };
  int slen = sizeof(sbuf);
  unsigned char rbuf[100] = { 0 };
  int rlen = 100;
  bool crc = 0;
  int rdlen = 0;

  rdlen = m_com.SendAndRecv(sbuf, slen, (char*)rbuf, rlen, &crc,1);

  if (rdlen > 5 && crc)
  {
    m_resultParam.measure.cnt_num[no] = (rbuf[4] << 8) | (rbuf[5]);
    return m_resultParam.measure.cnt_num[no];
  }
  return -1;
}

// no 0 - 4, buf the place of save , len the length of buf
// return recvd len success
// return -1 failure
int CEfgIO::GetAllCntData(int no, char* buf, int len)
{
  char sbuf[9] = { 0x55, 'C',	'D', no, 0x00 , 0x00 };
  int slen = sizeof(sbuf);
  unsigned char rbuf[100] = { 0 };
  int rlen = 100;
  bool crc = 0;
  int rdlen = 0;

  rdlen = m_com.SendAndRecv(sbuf, slen, buf, len, &crc, 1/*100ms*/);

  if (rdlen > 5 && crc)
  {
	  //if(buf[0] != 0x55)
		 // buf[0] = 0x54;

    return rdlen;
  }
  //ASSERT(0);
  return -1;
}
int CEfgIO::SetOut(int out3, int out6)
{
  char sbuf[9] = { 0x55, 'F', 'S',0x00,out3, out3 >> 8,out6,  out6 >> 8,0x00 };
  int slen = sizeof(sbuf);
  unsigned char rbuf[100] = { 0 };
  int rlen = 100;
  bool crc = 0;
  int rdlen = 0;

  rdlen = m_com.SendAndRecv(sbuf, slen, (char*)rbuf, rlen, &crc);

  if (rdlen > 0 && crc)
  {
    return 0;
  }

  return -1;
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

//返回档位电机数，pos_第几档，超过档位，取未检出
int CEfgIO::GetYOffPos(int pos)
{
  int sort_type = m_configParam.user_config.type;//晶片类型
  ASSERT(sort_type < MAX_TYPE_NUM);
  int sort_num = m_configParam.user_config.type_sort_num[sort_type];//档位数量

  if(pos >= sort_num)
	  return m_configParam.user_config.pos.not_detected;
  

  return m_configParam.user_config.pos.y_off[sort_type][pos];
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