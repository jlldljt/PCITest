#include "StdAfx.h"
#include "Com.h"

UINT8 g_npc_id = 0x01;

CCom gclsCom;

CCriticalSection ccCriticalSection;
unsigned short getCRC16(volatile char* pptr, unsigned int len)
{
  unsigned char* ptr = (unsigned char*)pptr;
  unsigned char i;
  unsigned short crc = 0xFFFF;
  if (len == 0) {
    len = 1;
  }
  while (len--) {
    crc ^= *ptr;
    for (i = 0; i < 8; i++) {
      if (crc & 1) {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else {
        crc >>= 1;
      }
    }
    ptr++;
  }
  return(crc);
}
unsigned short getSum16(volatile char* pptr, unsigned int len)
{
  unsigned short crcresult = 0;

  for (int i = 0; i < len; i++)
  {
    crcresult += pptr[i];
  }
  return(~crcresult);
}
CCom::CCom(void)
{
  //串口相关初始化

  stuInf.str = "";
  stuInf.bComOpen = false;

}


CCom::~CCom(void)
{
  CloseCom();
}

BOOL  CCom::OpenCom(UINT no)
{
  //hdCom=CreateFile("COM3",GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);//FILE_FLAG_OVERLAPPED
  CString com_name;
  com_name.Format(_T("COM%d"), no);
  return OpenCom(com_name);
  /*if (no<10)
  {
    com_name.Format(_T("COM%d"),no);
  }
  else
  {
    com_name.Format(_T("\\\\.\\COM%d"),no);
  }
  CloseCom();
  m_struct_com.hd=CreateFile(com_name,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);//非重叠
  if(m_struct_com.hd==INVALID_HANDLE_VALUE)
  {
    m_struct_com.error=GetLastError();
    //if(m_struct_com.error=5)
    //	::AfxMessageBox(_T("没有发现串口或串口已被占用"),MB_ICONWARNING,TRUE);

    return false;
  }
  else
  {
    SetupComm(m_struct_com.hd,2048,2048);//分配缓冲区
    GetCommState(m_struct_com.hd,&m_struct_com.dcb);//读取DCB结构
    m_struct_com.dcb.BaudRate=115200;//波特率为9600
    m_struct_com.dcb.ByteSize=8;//数据位为8位
    m_struct_com.dcb.fParity=1;//奇偶校验
    m_struct_com.dcb.Parity=NOPARITY;//ODDPARITY;
    m_struct_com.dcb.StopBits=0;//一位停止位
    SetCommState(m_struct_com.hd,&m_struct_com.dcb);
    m_struct_com.timeout.ReadIntervalTimeout=0;
    SetCommTimeouts(m_struct_com.hd,&m_struct_com.timeout);
    PurgeComm(m_struct_com.hd,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);

    return true;

  }*/
}


BOOL  CCom::OpenCom(CString no)
{
  //hdCom=CreateFile("COM3",GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);//FILE_FLAG_OVERLAPPED
  CString name_com;
  name_com.Format(_T("\\\\.\\%s"), no);
  CloseCom();
  m_struct_com.hd = CreateFile(name_com, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);//非重叠
  if (m_struct_com.hd == INVALID_HANDLE_VALUE)
  {
    m_struct_com.error = GetLastError();
    /*if(m_struct_com.s_error=5)
    ::AfxMessageBox(_T("没有发现串口或串口已被占用"),MB_ICONWARNING,TRUE);*/

    return false;
  }
  else
  {
    SetupComm(m_struct_com.hd, 2048, 2048);//分配缓冲区
    GetCommState(m_struct_com.hd, &m_struct_com.dcb);//读取DCB结构
    m_struct_com.dcb.BaudRate = 115200;//波特率为9600
    m_struct_com.dcb.ByteSize = 8;//数据位为8位
    m_struct_com.dcb.fParity = 1;//奇偶校验
    m_struct_com.dcb.Parity = NOPARITY;//ODDPARITY;
    m_struct_com.dcb.StopBits = 0;//一位停止位
    SetCommState(m_struct_com.hd, &m_struct_com.dcb);
    m_struct_com.timeout.ReadIntervalTimeout = 0;
    m_struct_com.timeout.ReadTotalTimeoutConstant = 10;
    SetCommTimeouts(m_struct_com.hd, &m_struct_com.timeout);
    PurgeComm(m_struct_com.hd, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

    stuInf.bComOpen = true;

    return true;

  }
}
BOOL  CCom::CloseCom()
{
  if (stuInf.bComOpen == false || m_struct_com.hd == INVALID_HANDLE_VALUE)
  {
    return 0;
  }
  CloseHandle(m_struct_com.hd);
  //m_struct_com.hd == NULL;
  stuInf.bComOpen = false;
  return 1;
}

// 读端口函数
// >0 成功 
int  CCom::ReadCom(char* buf, UINT len)
{
  DWORD inlen = 0;
  DWORD rdlen = 0;
  BOOL flag = 0;
  ClearCommError(m_struct_com.hd, &m_struct_com.error, &m_struct_com.stat);
  inlen = m_struct_com.stat.cbInQue;
  {
    if (inlen > len)
      inlen = len;
    flag = ReadFile(m_struct_com.hd, buf, len, &rdlen, NULL);//读取端口值
  }

  if (!flag)
  {
    if (GetLastError() == ERROR_IO_PENDING)
    {
      m_struct_com.error = GetLastError();
      if (m_struct_com.error == ERROR_IO_PENDING);
    }
    return -1;
  }

  return rdlen;
}


//写端口函数
//正常发送，返回>0
int  CCom::WriteCom(const char* buf, UINT len)
{
  DWORD wdlen = 0;//发送出去的字符长度
  BOOL flag = 0;
  long str;
  long Time1 = GetTickCount();

  ClearCommError(m_struct_com.hd, &m_struct_com.error, &m_struct_com.stat);
  flag = WriteFile(m_struct_com.hd, buf, len, &wdlen, NULL);//发送

  if (!flag)
  {
    if (GetLastError() == ERROR_IO_PENDING)
    {
      m_struct_com.error = GetLastError();
      if (m_struct_com.error == ERROR_IO_PENDING);
    }
    return -1;
  }


  long Time2 = GetTickCount();
  str = Time2 - Time1;

  return wdlen;
}


//返回0，成功
//返回-1，失败
int CCom::SendCharToCom(const char* buf, UINT len)
{
  //ccCriticalSection.Lock();
  int flag = 0;
  int wdlen = 0;
  ASSERT(buf && len);
  wdlen = WriteCom(buf, len);
  if (wdlen != len)//
  {
    flag = -1;
    return flag;
  }

  //CString tmp;
  //tmp = "\r\n";
  //m_info.str += tmp;
  //for (UINT j = 0; j < wdlen; j++)
  //{
  //  tmp.Format(_T("%02X"), (UCHAR)buf[j]);
  //  m_info.str += tmp;
  //}

  //ccCriticalSection.Unlock();
  return flag;
}

//返回0，成功
//返回-1，失败
int CCom::SendCharToCom(char* buf, UINT len, bool crc)
{
  if (crc)SetCrc(buf, len);//SetCrc
  return SendCharToCom(buf, len);
}

//返回-1，读com口出错
//返回0~接收到但长度不够
//buf 用户接受数组
//len用户接受长度
//crc 1校验通过
// timeout 超时多少个10ms
int CCom::RecvCharFromCom(char* buf, UINT len, bool* crc, UINT timeout)
{
  //ccCriticalSection.Lock();
  ASSERT(buf && len > 0);
  INT rdlen;//读到的长度
  //UINT totalLen = 0;
  INT timeout_cnt = 0;
  INT offset = 0;
  if (crc)
    * crc = 0;

  while (1)
  {
    //Sleep(10);
    rdlen = ReadCom(buf + offset, len - offset);
    if (rdlen >= 0)
    {
      //totalLen += unRL;//记录收到的长度

      if (0 == rdlen)
      {
        if (++timeout_cnt > timeout)//大约超过50ms无数据，退出
        {
          break;
        }
        continue;
      }
      timeout_cnt = 0;

      offset += rdlen;
      //for (UINT i=0;i<rdlen;i++)//由于strcpy碰到0即结束，这里要循环复制
      //{
      //	buf[totalLen++]=m_info.rbuf[i];
      //}
    }
    else
    {
      return -1;
    }

    if (offset == len)
    {
      break;
    }



  }
  if (offset > 2 && crc)
  {
    unsigned short crcresult = getCRC16(buf, offset - 2);
    char temp1 = crcresult & 0xff;
    char temp0 = (crcresult >> 8) & 0xff;
    if(buf[offset -2]==temp1 && buf[offset -1]==temp0)
    {
    	*crc = 1;
    }
    else
    {
      *crc = 0;
    }
   /* unsigned short crcresult = getSum16(buf + 1, offset - 2);
    if (buf[offset - 1] == (char)(crcresult & 0xff))
    {
      *crc = 1;
    }
    else
    {
      *crc = 0;
    }*/
  }


  //CString tmp;
  //tmp="\r\n";
  //stuInf.str+= tmp;
  //for (UINT j=0; j< offset; j++)
  //{
  //	tmp.Format(_T("%02X"),(UCHAR)buf[j]);
  //  stuInf.str+= tmp;
  //}

  //ccCriticalSection.Unlock();
  return offset;
}
void CCom::ClearCom()
{
  stuInf.str = "";
  PurgeComm(m_struct_com.hd, PURGE_RXCLEAR | PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR);
}


// 枚举出pc上有效的串口号
void CCom::EnumerateSerialPorts(CStringArray& ports)
{
  ports.RemoveAll();


  HKEY   hKey;

  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Hardware\\DeviceMap\\SerialComm"), NULL, KEY_READ, &hKey) == ERROR_SUCCESS)
  {
    TCHAR       szPortName[256], szComName[256];
    DWORD       dwLong, dwSize;
    int         nCount = 0;
    while (true)
    {
      dwLong = dwSize = 256;
      if (RegEnumValue(hKey, nCount, szPortName, &dwLong, NULL, NULL, (PUCHAR)szComName, &dwSize) == ERROR_NO_MORE_ITEMS)
        break;
      CString str;
      //str.Format("%d",nCount);  

      str.Format(_T("%s "), szComName);
      ports.Add(str);
      nCount++;
    }
    RegCloseKey(hKey);
  }
}

// 输入的命令最好是心跳命令
// -1失败，或者返回打开的com序号
int CCom::AutoOpen(const char* sbuf, int slen, char* rbuf, int rlen, bool* crc, int timeout)
{
  CStringArray ports;
  EnumerateSerialPorts(ports);
  int num = ports.GetCount();
  for (int i = 0; i < num; i++)
  {
    if (false == OpenCom(ports.GetAt(i)))
      continue;

    if (0 < SendAndRecv(sbuf, slen, rbuf, rlen, crc, timeout))
    {
      if (crc && 1 == *crc)
      {
        return i;
      }
    }
  }

  return -1;
}


UINT CCom::SetCrc(char* buf, UINT len)
{
  ASSERT(len > 2);
  unsigned short crcresult = getCRC16(buf, len - 2);
  buf[len - 2] = crcresult & 0xff;
  buf[len - 1] = (crcresult >> 8) & 0xff;
  return 0;
}

UINT CCom::SetSum(char* buf, UINT len)
{
  ASSERT(len > 2);

  unsigned short crcresult = getSum16(buf + 1, len - 2);

  //buf[len - 2] = crcresult & 0xff;
  buf[len - 1] = crcresult;// (~crcresult >> 8) & 0xff;
  return 0;
}

int CCom::SendAndRecv(const char* sbuf, int slen, char* rbuf, int rlen, bool* crc, int timeout)
{
  int flagr = 0, flagw = 0;
  ccCriticalSection.Lock();


  ClearCom();
  flagw = SendCharToCom((char*)sbuf, slen, crc ? true : false);
  flagr = RecvCharFromCom((char*)rbuf, rlen, crc, timeout);

  ccCriticalSection.Unlock();

  if (flagr)
    return flagr;
  else
    return flagw;
}

/////////////////////////////////////////////modbus////////////////////////////////////////////////////////////////////////////////////

void SwapByte(UINT8* rbuf, UINT8* dbuf)
{
  UINT8 tmp;
  ASSERT(rbuf && dbuf);
  tmp = *rbuf;
  *rbuf = *dbuf;
  *dbuf = tmp;
}
UINT8* Endian_TF(UINT8* buf, const UINT16 num, const UINT16 size)
{
  int i, j;
  ASSERT(!(num % size));
  ASSERT(!(size % 2));

  for (i = 0; i < num / size; i++)
  {
    for (j = 0; j < size / 2; j++)
    {
      SwapByte(buf + size * i + j, buf + size * i + size - 1 - j);
    }
  }


  return buf;
}

void WriteMultiRegisterCmd(UINT8 dev, UINT16 addr, UINT16 num,
  UINT8* regVal, UINT8** cmdBuf, UINT8* cmdLen)
{
  ASSERT(NULL != cmdBuf);
  ASSERT(NULL == *cmdBuf);

  ASSERT(NULL != cmdLen);
  ASSERT(NULL != regVal);
  ASSERT(0x7D >= num);

  *cmdLen = 7 + (num << 1)+2/*crc*/;
  //NEW08U(*cmdBuf, *cmdLen);
  *cmdBuf = new UINT8[*cmdLen];
  (*cmdBuf)[0] = dev;
  (*cmdBuf)[1] = 0x10;
  (*cmdBuf)[2] = (addr >> 8) & 0xff;
  (*cmdBuf)[3] = (addr) & 0xff;
  (*cmdBuf)[4] = (num >> 8) & 0xff;
  (*cmdBuf)[5] = (num) & 0xff;
  (*cmdBuf)[6] = num << 1;

  memcpy((*cmdBuf) + 7, Endian_TF(regVal, num << 1, 2), (*cmdBuf)[6]);
}
//no 0->1
INT32 CCom::WriteVar(UINT32 no , INT32 val) {
  UINT8* cmdBuf = NULL;
  UINT8  cmdLen = 0;
  INT32  reg = val;
  UINT8  recvBuf[30] = { 0 };
  UINT8  recvLen = 0;
  bool crc = true;

  WriteMultiRegisterCmd(g_npc_id, no<<1, 2, (UINT8*)& reg, &cmdBuf, &cmdLen);
  ASSERT(cmdBuf);
  recvLen=SendAndRecv((char*)cmdBuf, cmdLen, (char*)recvBuf, 30, &crc);

  delete cmdBuf;

  if (1 == crc && g_npc_id == recvBuf[0] && 0x80 > recvBuf[1]) {
    return 0;
  }
  else {
    return -1;
  }

}

void ReadRegisterCmd(UINT8 dev, UINT16 addr, UINT16 num,
  UINT8** cmdBuf, UINT8* cmdLen)
{
  ASSERT(NULL != cmdBuf);
  ASSERT(NULL == *cmdBuf);
  ASSERT(NULL != cmdLen);
  ASSERT(0x7D >= num);

  *cmdLen = 6 + 2/*crc*/;
  //NEW08U(*cmdBuf, *cmdLen);
  *cmdBuf = new UINT8[*cmdLen];
  (*cmdBuf)[0] = dev;
  (*cmdBuf)[1] = 0x03;
  (*cmdBuf)[2] = (addr >> 8) & 0xff;
  (*cmdBuf)[3] = (addr) & 0xff;
  (*cmdBuf)[4] = (num >> 8) & 0xff;
  (*cmdBuf)[5] = (num) & 0xff;
}

#define ENDIAN_TF16U(A)    ((((UINT16)(A) & 0xff00) >> 8) | \
                              (((UINT16)(A) & 0x00ff) << 8))

INT32 ReadRegReqToVar(UINT8* buf, UINT8 len, UINT8 reg, INT32 &var) {//获取位状态
  UINT8 fc = buf[1];
  UINT8 numCh = buf[2];
  UINT16* p_reg = (UINT16*)(buf + 3);

  INT32 num;

  if (0x03 != fc || len < numCh + 3)
    return -1;

  if (numCh == 4)
  {
    num = ((ENDIAN_TF16U(p_reg[reg + 1])) << 16) | ENDIAN_TF16U(p_reg[reg]);
  }
  else if (numCh == 2)
  {
    num = ENDIAN_TF16U(p_reg[reg]);
  }
  else // 有些设备会回复numCH = 0
  {
    return 0;
  }

  var = num;


  return 0;
}


//no 0->1
INT32 CCom::ReadVar(UINT32 no, INT32 &val) {
  UINT8* cmdBuf = NULL;
  UINT8  cmdLen = 0;
  UINT32  reg = val;
  UINT8  recvBuf[30] = { 0 };
  UINT8  recvLen = 0;
  bool crc = true;

  ReadRegisterCmd(g_npc_id, no << 1, 2,  &cmdBuf, &cmdLen);
  ASSERT(cmdBuf);
  recvLen = SendAndRecv((char*)cmdBuf, cmdLen, (char*)recvBuf, 30, &crc);

  delete cmdBuf;


  if (1 == crc && g_npc_id == recvBuf[0] && 0x03 == recvBuf[1]) {
    if(0 == ReadRegReqToVar(recvBuf, recvLen, 0, val))
      return 0;
    else
      return -1;
  }
  else {
    return -1;
  }

}