#include "StdAfx.h"
#include "EFGV1_Base.h"

int CEFGV1_Base::m_EFGV1Boards = 0;

CEFGV1_Base::CEFGV1_Base()
{
}


CEFGV1_Base::~CEFGV1_Base()
{
}

BOOL CEFGV1_Base::DriverInit(void)
{
  char sbuf[] = { 0x55, 'I', 'Q', 0, 0x00, 0x00 };
  int slen = sizeof(sbuf);
  char rbuf[100] = {0};
  int rlen = 100;
  bool crc = 0;
  int timeout = 10; //10*10ms

  if (-1 == m_com.AutoOpen(sbuf, slen, rbuf, rlen, &crc, timeout))
  {
    return FALSE;
  }
  else
  {
    m_EFGV1Boards = 1;
    return TRUE;
  }
}

void CEFGV1_Base::DriverClose(void)
{
}

int CEFGV1_Base::ReadDi(int no)
{
  char sbuf[] = { 0x55,'I',	'Q',no, 0x00, 0x00 };
  int slen = sizeof(sbuf);
  char rbuf[100] = { 0 };
  int rlen = 100;
  bool crc = 0;
  int rdlen = 0;

  rdlen = m_com.SendAndRecv(sbuf, slen, rbuf, rlen, &crc);

  if(rdlen >0 && crc)
  {
    return rbuf[3] == 0 ? 0 : 1;
  }
  return -1;
}

int CEFGV1_Base::ReadDo(int no)
{
  char sbuf[] = { 0x55, 'O',	'Q',no, 0x00, 0x00 };
  int slen = sizeof(sbuf);
  char rbuf[100] = { 0 };
  int rlen = 100;
  bool crc = 0;
  int rdlen = 0;

  rdlen = m_com.SendAndRecv(sbuf, slen, rbuf, rlen, &crc);

  if (rdlen > 0 && crc)
  {
    return rbuf[3]==0?0:1;
  }
  return -1;
}

int CEFGV1_Base::WriteDo(int no, int val)
{
  char sbuf[] = { 0x55, 'O',	'C',no, val?0xFF:0, 0x00, 0x00 };
  int slen = sizeof(sbuf);
  char rbuf[100] = { 0 };
  int rlen = 100;
  bool crc = 0;
  int rdlen = 0;

  rdlen = m_com.SendAndRecv(sbuf, slen, rbuf, rlen, &crc);

  if (rdlen > 0 && crc)
  {
    return 0;
  }
  return -1;
}

int CEFGV1_Base::ReadMotor(int no)
{
  char sbuf[] = { 0x55, 'M',	'Q',no, 0x00, 0x00 };
  int slen = sizeof(sbuf);
  char rbuf[100] = { 0 };
  int rlen = 100;
  bool crc = 0;
  int rdlen = 0;

  rdlen = m_com.SendAndRecv(sbuf, slen, rbuf, rlen, &crc);

  if (rdlen > 8 && crc && rbuf[3] == 0x00)
  {
    return (rbuf[4] << 24) | (rbuf[5] << 16)| (rbuf[6] << 8)| (rbuf[7]);
  }
  return -1;
}

int CEFGV1_Base::WriteMotor(int no, int val)
{
  char sbuf[] = { 0x55, 'M', 'C', no, val >> 24, val >> 16, val >> 8, val >> 0, 0x00, 0x00 };
  int slen = sizeof(sbuf);
  char rbuf[100] = { 0 };
  int rlen = 100;
  bool crc = 0;
  int rdlen = 0;

  rdlen = m_com.SendAndRecv(sbuf, slen, rbuf, rlen, &crc);

  if (rdlen > 0 && crc)
  {
    return 0;
  }
  return -1;
}
