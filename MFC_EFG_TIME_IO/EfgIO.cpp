#include "StdAfx.h"
#include "EfgIO.h"


CEfgIO::CEfgIO()
{
  memset(&m_efg_param, 0, sizeof(m_efg_param));
  CPCICtrl::AllDriverInit();
  m_on = FALSE;
  m_on_card_no = -1;
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
  if (!m_multiCardCtrl.m_card[cardNo]) {  //CPCICtrl::Delete(m_timeIOCtrl);//³ö´í²Ù×÷
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

BOOL CEfgIO::CardOn()
{
  if (TRUE == m_on)
    return TRUE;

  int count = GetPCINum();
  if (!count)
    return FALSE;

  int index = -1;
  for (int i = 0; i < count; i++) {
    if (TMC12A == GetPCIType(i)) {//²éÕÒtmc12a°å¿¨
      index = i;
      break;
    }
  }

  if (-1 == index) {
    return FALSE;
  }

  if (FALSE == RunPCI(index))
    return FALSE;

  m_on = TRUE;
  m_on_card_no = index;
  return TRUE;
}

BOOL CEfgIO::CardOff()
{

  if (FALSE == m_on)
    return TRUE;

  if (FALSE == StopPCI(m_on_card_no))
    return FALSE;

  m_on = FALSE;
  m_on_card_no = -1;
  return TRUE;
}

int CEfgIO::GetCardIndex()
{
  return m_on_card_no;
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
  double fparam0, fparam1;
  GetPCI(m_on_card_no)->ReadDi(channel, fparam0, fparam1);
  return fparam0;
}

void CEfgIO::WriteDo(EFG_OChannel channel, double param)
{
  GetPCI(m_on_card_no)->WriteDO(channel, param);
}

double CEfgIO::ReadT0(EFG_T0Channel channel)
{
  double fparam0, fparam1;
  GetPCI(m_on_card_no)->ReadT0(channel, fparam0, fparam1);
  return fparam1;
}
