#include "StdAfx.h"
#include "Com.h"

CCom m_com;

CCriticalSection ccCriticalSection;
unsigned short getCRC16(volatile char *pptr, unsigned int len)
{
  unsigned char *ptr = (unsigned char *)pptr;
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
unsigned short getSum16(volatile char *pptr, unsigned int len)
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
	//������س�ʼ��

	m_info.str="";

}


CCom::~CCom(void)
{
	CloseCom();
}

BOOL  CCom::OpenCom(UINT no)
{
	//hdCom=CreateFile("COM3",GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);//FILE_FLAG_OVERLAPPED
	CString com_name;
	com_name.Format(_T("COM%d"),no);
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
	m_base.hd=CreateFile(com_name,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);//���ص�
	if(m_base.hd==INVALID_HANDLE_VALUE)
	{
		m_base.error=GetLastError();
		//if(m_base.error=5)
		//	::AfxMessageBox(_T("û�з��ִ��ڻ򴮿��ѱ�ռ��"),MB_ICONWARNING,TRUE);

		return false;
	}
	else
	{
		SetupComm(m_base.hd,2048,2048);//���仺����
		GetCommState(m_base.hd,&m_base.dcb);//��ȡDCB�ṹ
		m_base.dcb.BaudRate=115200;//������Ϊ9600
		m_base.dcb.ByteSize=8;//����λΪ8λ
		m_base.dcb.fParity=1;//��żУ��
		m_base.dcb.Parity=NOPARITY;//ODDPARITY;
		m_base.dcb.StopBits=0;//һλֹͣλ
		SetCommState(m_base.hd,&m_base.dcb);
		m_base.timeout.ReadIntervalTimeout=0;
		SetCommTimeouts(m_base.hd,&m_base.timeout);
		PurgeComm(m_base.hd,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);

		return true;
		
	}*/
}


BOOL  CCom::OpenCom(CString no)
{
	//hdCom=CreateFile("COM3",GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);//FILE_FLAG_OVERLAPPED
	CString name_com;
	name_com.Format(_T("\\\\.\\%s"),no);
	CloseCom();
	m_base.hd=CreateFile(name_com,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);//���ص�
	if(m_base.hd==INVALID_HANDLE_VALUE)
	{
		m_base.error=GetLastError();
		/*if(m_struct_com.s_error=5)
		::AfxMessageBox(_T("û�з��ִ��ڻ򴮿��ѱ�ռ��"),MB_ICONWARNING,TRUE);*/

		return false;
	}
	else
	{
		SetupComm(m_base.hd,2048,2048);//���仺����
		GetCommState(m_base.hd,&m_base.dcb);//��ȡDCB�ṹ
		m_base.dcb.BaudRate=115200;//������Ϊ9600
		m_base.dcb.ByteSize=8;//����λΪ8λ
		m_base.dcb.fParity=1;//��żУ��
		m_base.dcb.Parity=NOPARITY;//ODDPARITY;
		m_base.dcb.StopBits=0;//һλֹͣλ
		SetCommState(m_base.hd,&m_base.dcb);
		m_base.timeout.ReadIntervalTimeout=0;
		SetCommTimeouts(m_base.hd,&m_base.timeout);
		PurgeComm(m_base.hd,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);

		return true;

	}
}
BOOL  CCom::CloseCom()
{
	if(m_base.hd==INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	CloseHandle(m_base.hd);

	return 1;
}

// ���˿ں���
// >0 �ɹ� 
int  CCom::ReadCom(char *buf,UINT len)
{
  DWORD inlen = 0;
	DWORD rdlen=0;
  BOOL flag = 0;
	ClearCommError(m_base.hd,&m_base.error,&m_base.stat);
  inlen =m_base.stat.cbInQue;
	{
		if(inlen > len)
      inlen = len;
    flag =ReadFile(m_base.hd, buf,len,&rdlen,NULL);//��ȡ�˿�ֵ
	}

	if(!flag)
	{
		if(GetLastError()==ERROR_IO_PENDING)
		{
			m_base.error=GetLastError();
			if(m_base.error==ERROR_IO_PENDING);
		}
    return -1;
	}

	return rdlen;
}


//д�˿ں���
//�������ͣ�����>0
int  CCom::WriteCom(const char *buf,UINT len)
{
	DWORD wdlen=0;//���ͳ�ȥ���ַ�����
  BOOL flag = 0;
	long str;
	long Time1 = GetTickCount();

 	ClearCommError(m_base.hd,&m_base.error,&m_base.stat);
	WriteFile(m_base.hd, buf,len,&wdlen,NULL);//����

  if (!flag)
  {
    if (GetLastError() == ERROR_IO_PENDING)
    {
      m_base.error = GetLastError();
      if (m_base.error == ERROR_IO_PENDING);
    }
    return -1;
  }


	long Time2 = GetTickCount();
	str=Time2-Time1;

	return wdlen;
}


//����0���ɹ�
//����-1��ʧ��
int CCom::SendCharToCom(const char* buf, UINT len)
{
	//ccCriticalSection.Lock();
	int flag=0;
  int wdlen = 0;
  ASSERT(buf && len);
  wdlen = WriteCom(buf, len);
	if(wdlen !=len)//
	{
		flag=-1;
	}

  CString tmp;
  tmp = "\r\n";
  m_info.str += tmp;
  for (UINT j = 0; j < wdlen; j++)
  {
    tmp.Format(_T("%02X"), (UCHAR)buf[j]);
    m_info.str += tmp;
  }

	//ccCriticalSection.Unlock();
	return flag;
}

//����0���ɹ�
//����-1��ʧ��
int CCom::SendCharToCom(char* buf, UINT len, bool crc)
{
  if(crc)SetSum(buf, len);//SetCrc
  return SendCharToCom(buf, len);
}

//����-1����com�ڳ���
//����0~���յ������Ȳ���
//buf �û���������
//len�û����ܳ���
//crc 1У��ͨ��
// timeout ��ʱ���ٸ�10ms
int CCom::RecvCharFromCom(char *buf,UINT len,bool *crc,UINT timeout)
{
	//ccCriticalSection.Lock();
	UINT rdlen;//�����ĳ���
	//UINT totalLen = 0;
	UINT timeout_cnt = 0;
  UINT offset = 0;
  if (crc)
	  *crc = 0;

	while (1)
	{
		Sleep(10);
    rdlen = ReadCom(buf+ offset, len- offset);
		if (rdlen>=0)
		{
			//totalLen += unRL;//��¼�յ��ĳ���

			if(0 == rdlen)
			{
				if (++timeout_cnt> timeout)//��Լ����50ms�����ݣ��˳�
				{
					break;
				}
				continue;
			}
			timeout_cnt=0;

      offset += rdlen;
			//for (UINT i=0;i<rdlen;i++)//����strcpy����0������������Ҫѭ������
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
		unsigned short crcresult = getSum16(buf+1, offset -2);
		//char temp1 = crcresult & 0xff;
		//char temp0 = (crcresult >> 8) & 0xff;
		//if(buf[offset -2]==temp1 && buf[offset -1]==temp0)
		//{
		//	*crc = 1;
		//}
    if(buf[offset -1]== crcresult)
    {
    	*crc = 1;
    }
	}
	

	CString tmp;
	tmp="\r\n";
	m_info.str+= tmp;
	for (UINT j=0; j< offset; j++)
	{
		tmp.Format(_T("%02X"),(UCHAR)buf[j]);
		m_info.str+= tmp;
	}

	//ccCriticalSection.Unlock();
	return offset;
}
void CCom::ClearCom()
{
  m_info.str = "";
	PurgeComm(m_base.hd,PURGE_RXCLEAR|PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR);
}


// ö�ٳ�pc����Ч�Ĵ��ں�
void CCom::EnumerateSerialPorts( CStringArray& ports )
{
	ports.RemoveAll();


	HKEY   hKey;  

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Hardware\\DeviceMap\\SerialComm"), NULL, KEY_READ, &hKey)==ERROR_SUCCESS)  
	{  
		TCHAR       szPortName[256], szComName[256];  
		DWORD       dwLong, dwSize;  
		int         nCount  = 0;  
		while(true)  
		{  
			dwLong  = dwSize    = 256;  
			if(RegEnumValue(hKey, nCount, szPortName, &dwLong, NULL, NULL, (PUCHAR)szComName, &dwSize)==ERROR_NO_MORE_ITEMS)  
				break;  
			CString str;  
			//str.Format("%d",nCount);  
			
			str.Format(_T("%s "),szComName);  
			ports.Add(str);  
			nCount++;  
		}  
		RegCloseKey(hKey);
	} 
}

// ����������������������
// -1ʧ�ܣ����߷��ش򿪵�com���
int CCom::AutoOpen(const char* sbuf, int slen, char* rbuf, int rlen, bool *crc, int timeout)
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


UINT CCom::SetCrc(char * buf, UINT len)
{
  ASSERT(len > 2);
	unsigned short crcresult = getCRC16(buf, len-2);
	buf[len - 2] = crcresult & 0xff;
	buf[len - 1] = (crcresult >> 8) & 0xff;
	return 0;
}

UINT CCom::SetSum(char * buf, UINT len)
{
  ASSERT(len > 2);

  unsigned short crcresult = getSum16(buf+1,len-2);

  //buf[len - 2] = crcresult & 0xff;
  buf[len - 1] = crcresult;// (~crcresult >> 8) & 0xff;
  return 0;
}

int CCom::SendAndRecv(const char* sbuf, int slen, char* rbuf, int rlen, bool *crc, int timeout)
{
  int flagr = 0, flagw = 0;
  ccCriticalSection.Lock();
  
   
  ClearCom();
  flagw = SendCharToCom((char*)sbuf, slen, crc ? true : false);
  flagr = RecvCharFromCom((char*)rbuf, rlen, crc, timeout);
    
  ccCriticalSection.Unlock();

  if (flagw)
    return flagw;
  else
    return flagr;
}