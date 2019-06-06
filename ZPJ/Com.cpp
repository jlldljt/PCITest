#include "StdAfx.h"
#include "Com.h"


CCom::CCom(void)
{
	
	stuInf.bIsRun=1;///////////////////////////
	m_struct_com.s_bool_schar_recv=0;

	//������س�ʼ��
	stuInf.pchBufRcv=NULL;
	stuInf.pchBufWrt=NULL;
	stuInf.chRcvChar=0xfc;
	stuInf.chWrtChar=0xfc;
	stuInf.unLenWrt=10;
	stuInf.unLenRcv=3;
	stuInf.pchBufRTmp=new char[stuInf.unLenRcv+1];//��destorywindows��delete
	stuInf.pchBufWTmp=new char[stuInf.unLenWrt+1];//д��tmp�����ڱȽ�
	stuInf.csRcv="";
	stuInf.csWrt="";
	stuInf.bCmdSuc=0;
	stuInf.chWrtID=0x00;
	stuInf.chRcvID=0x00;
}

CCom gclsCom;

CCom::~CCom(void)
{
	delete[] stuInf.pchBufRTmp;//��init��new
	stuInf.pchBufRTmp=NULL;
	delete[] stuInf.pchBufWTmp;//��init��new
	stuInf.pchBufWTmp=NULL;
}
BOOL  CCom::OpenCom(UINT comN)
{
	//hdCom=CreateFile("COM3",GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);//FILE_FLAG_OVERLAPPED
	CString l_cstr_name_com;
	l_cstr_name_com.Format(_T("COM%d"),comN);
	m_struct_com.s_hd=CreateFile(l_cstr_name_com,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);//���ص�
	if(m_struct_com.s_hd==INVALID_HANDLE_VALUE)
	{
		m_struct_com.s_error=GetLastError();
		if(m_struct_com.s_error=5)
			::AfxMessageBox(_T("û�з��ִ��ڻ򴮿��ѱ�ռ��"),MB_ICONWARNING,TRUE);
		stuInf.bComOpen=0;
		return false;
		
	}
	else
	{
		SetupComm(m_struct_com.s_hd,2048,2048);//���仺����
		GetCommState(m_struct_com.s_hd,&m_struct_com.s_dcb);//��ȡDCB�ṹ
		m_struct_com.s_dcb.BaudRate=57600;//������Ϊ9600
		m_struct_com.s_dcb.ByteSize=8;//����λΪ8λ
		m_struct_com.s_dcb.fParity=1;//��żУ��
		m_struct_com.s_dcb.Parity=NOPARITY;//ODDPARITY;
		m_struct_com.s_dcb.StopBits=0;//һλֹͣλ
		SetCommState(m_struct_com.s_hd,&m_struct_com.s_dcb);
		m_struct_com.s_timeout.ReadIntervalTimeout=0;
		SetCommTimeouts(m_struct_com.s_hd,&m_struct_com.s_timeout);
		PurgeComm(m_struct_com.s_hd,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
		stuInf.bComOpen=1;
		return true;
		
	}
}
BOOL  CCom::CloseCom()
{
	if(m_struct_com.s_hd==INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	CloseHandle(m_struct_com.s_hd);
	return 1;
}
// ö�ٳ�pc����Ч�Ĵ��ں�
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
//���˿ں���
//��ȡ��ʼֵ��recvLenλ������0
//��ȡ����ʼֵ������1
//û�ж�����ʼֵ������2
//�����ַ������ޣ�����3
//��ȡ���Ȳ��� ������4
//��������̬readlen��null������5
UINT  CCom::ReadCom(char *&readBuff,char startChar,UINT recvLen,UINT *readLen = NULL)
{
	UINT recvLenTmp=recvLen-1;
	DWORD l_dword_len_in=0;
	DWORD l_dword_len_read=0;
	BOOL l_bool_flag_read=0;
	char l_char_buff[100];
	memset(l_char_buff, 0, 100);
	if(recvLenTmp>98)return 3;
	ClearCommError(m_struct_com.s_hd,&m_struct_com.s_error,&m_struct_com.s_stat);
	l_dword_len_in=m_struct_com.s_stat.cbInQue;
	if(stuInf.bIsRun)
	{
		if (!m_struct_com.s_bool_schar_recv)//���û�ҵ���ʼλ������ʼλ
		{

			for (int i=0;i<(int)l_dword_len_in;i++)//����ʼλ
			{
				l_bool_flag_read=ReadFile(m_struct_com.s_hd,&l_char_buff,1,&l_dword_len_read,NULL);
				if(*l_char_buff==startChar)//
				{
					m_struct_com.s_bool_schar_recv=1;//����ʼλ�Ľ��տ�ʼ
					break;
				}
			}
		}

		if (!m_struct_com.s_bool_schar_recv)//���û���ҵ���ʼλ������
			return 2; 
	}
	ClearCommError(m_struct_com.s_hd,&m_struct_com.s_error,&m_struct_com.s_stat);
	l_dword_len_in=m_struct_com.s_stat.cbInQue;

	if(stuInf.bIsRun)
	{
		if(l_dword_len_in<recvLenTmp)//�������С�ڴ������ֽ�
			return 1;
		l_bool_flag_read=ReadFile(m_struct_com.s_hd,&l_char_buff,recvLenTmp,&l_dword_len_read,NULL);//��ȡ�˿�ֵ
		if (l_dword_len_read<recvLenTmp)
		{
			return 4;
		}
	}
	else
	{
		if(l_dword_len_in > recvLenTmp)
			l_dword_len_in = recvLenTmp;
		l_bool_flag_read=ReadFile(m_struct_com.s_hd,&l_char_buff,l_dword_len_in,&l_dword_len_read,NULL);//��ȡ�˿�ֵ
	}

	
	
	*(l_char_buff+l_dword_len_read)='\0';//���������ֹ��

	if(!l_bool_flag_read)
	{
		if(GetLastError()==ERROR_IO_PENDING)
		{
			m_struct_com.s_error=GetLastError();
			if(m_struct_com.s_error==ERROR_IO_PENDING);
		}
	}

	m_struct_com.s_bool_schar_recv=0;//����ʼλ�Ľ��ս���
	if(stuInf.bIsRun)
	{
		readBuff[0]=stuInf.chRcvChar;
		for (int i=0;i<=(int)l_dword_len_read;i++)//����strcpy����0������������Ҫѭ������
		{
			readBuff[i+1]=l_char_buff[i];
		}
	}
	else
	{
		for (int i=0;i<=(int)l_dword_len_read;i++)//����strcpy����0������������Ҫѭ������
		{
			readBuff[i]=l_char_buff[i];
		}
		if(NULL == readLen)
			return 5;
		*readLen = l_dword_len_read;
	}

	//strcpy(readBuff,l_char_buff);//������copy������tmp
	return 0;
}

//д�˿ں���
//�������ͣ�����0
//�����ַ���̫�࣬����3
//���ͳ��Ȳ���������4
UINT  CCom::WriteCom(char *&writeBuff,UINT writeLen)
{
	char l_char_buff[100];//�ֲ����������ڷ������ݣ���ֹwritebuff���޸�
	DWORD l_dword_len_read=0;//���ͳ�ȥ���ַ�����
	memset(l_char_buff, 0, 100);

	if(writeLen>98)return 3;//�����Ҫ���͵��ַ���̫��

	if (strlen(writeBuff)>98)return 3;

	for (int i=0;i<=(int)writeLen;i++)//����strcpy����0������������Ҫѭ������
	{
		l_char_buff[i]=writeBuff[i];
	}

	//strcpy(l_char_buff,writeBuff);//������copy������l_char_buff
 	ClearCommError(m_struct_com.s_hd,&m_struct_com.s_error,&m_struct_com.s_stat);
	WriteFile(m_struct_com.s_hd,&l_char_buff,writeLen,&l_dword_len_read,NULL);//����
	m_struct_com.s_error=GetLastError();

	if (l_dword_len_read<writeLen)//���ͳ���С��Ҫ���͵ĳ���
		return 4;

	return 0;
}

//����s_uint_len_recv+1��1λ����ʼλs_char_mark_writechar)���ַ�����λ��
//����0���ɹ�
//����1��ʧ��
UINT CCom::SendCharToCom(const char* comStr ,bool sucDisable=0)//��������+data
{
	UINT l_flag=0;
	while(stuInf.pchBufWrt != NULL)
		Sleep(1);
	stuInf.pchBufWrt=new char[stuInf.unLenWrt+1];//������̬���飬���Ȱ����޸�
	memset(stuInf.pchBufWrt, 0, stuInf.unLenWrt+1);
	stuInf.pchBufWrt[0]=stuInf.chWrtChar;//��ʼλ
	stuInf.pchBufWrt[1]=stuInf.chWrtID;//��1�η���id++
	stuInf.chWrtID++;
	//strcat(stuInf.pchBufWrt,comStr);//command
	if(sucDisable)
		stuInf.bCmdSuc=1;
	else
		stuInf.bCmdSuc=0;//�ɹ����ձ�־��0
	stuInf.pchBufWrt[stuInf.unLenWrt-1] = stuInf.pchBufWrt[1];
	for (int i=2;i<stuInf.unLenWrt-1;i++)//�ۼ�У��λ
	{
		stuInf.pchBufWrt[i]=comStr[i-2];
		stuInf.pchBufWrt[stuInf.unLenWrt-1]+=stuInf.pchBufWrt[i];
	}

	//stuInf.pchBufWrt[8]='\r';//����λ��������Ҫ�������޸�
	//stuInf.pchBufWrt[9]='\n';//����λ��������Ҫ�������޸�
	
	
	for (UINT i=0;i<stuInf.unLenWrt;i++)//����strcpy����0������������Ҫѭ������
	{
		stuInf.pchBufWTmp[i]=stuInf.pchBufWrt[i];
	}

	//strcpy(stuInf.pchBufWTmp,stuInf.pchBufWrt);//����֮ǰ����������жϵڼ��η���,��sysinit�г�ʼ����ע�ⳤ��;
	if(WriteCom(stuInf.pchBufWrt,stuInf.unLenWrt))//
	{
		l_flag=1;
	}
	delete[] stuInf.pchBufWrt;
	stuInf.pchBufWrt=NULL;
	return l_flag;
}
//����s_uint_len_recv+1��1λ����ʼλs_char_mark_writechar)���ַ�����λ��
//����0���ɹ�
//����1��ʧ��
UINT CCom::SendCharToComNOID(const char* comStr)
{
	UINT l_flag=0;
	while(stuInf.pchBufWrt != NULL)
		Sleep(1);
	stuInf.pchBufWrt=new char[stuInf.unLenWrt+2];//������̬���飬���Ȱ����޸�
	memset(stuInf.pchBufWrt, 0, stuInf.unLenWrt+2);
	stuInf.pchBufWrt[0]=stuInf.chWrtChar;//��ʼλ

	//strcat(stuInf.pchBufWrt,comStr);//command

	for (int i=1;i<6;i++)//�ۼ�У��λ
	{
		stuInf.pchBufWrt[i]=comStr[i-1];
		stuInf.pchBufWrt[7]+=stuInf.pchBufWrt[i];
	}

	//stuInf.pchBufWrt[8]='\r';//����λ��������Ҫ�������޸�
	//stuInf.pchBufWrt[9]='\n';//����λ��������Ҫ�������޸�
	BOOL l_unequ=0;

	for (int i=0;i<6;i++)//�ж�����һ��ָ���Ƿ����
	{
		if (stuInf.pchBufWTmp[i]!=stuInf.pchBufWrt[i])//�����
		{
			l_unequ=1;
			break;
		}
	}

	if (l_unequ)//����ȣ���һ�η���
	{ 
		//stuInf.chID++;
		stuInf.pchBufWrt[6]=stuInf.chWrtID;//��1�η���id++

		stuInf.bCmdSuc=0;//�ɹ����ձ�־��0
	}
	else//���
	{
		stuInf.pchBufWrt[6]=stuInf.chWrtID;
	}

	for (UINT i=0;i<stuInf.unLenWrt+1;i++)//����strcpy����0������������Ҫѭ������
	{
		stuInf.pchBufWTmp[i]=stuInf.pchBufWrt[i];
	}

	//strcpy(stuInf.pchBufWTmp,stuInf.pchBufWrt);//����֮ǰ����������жϵڼ��η���,��sysinit�г�ʼ����ע�ⳤ��;
	if(WriteCom(stuInf.pchBufWrt,stuInf.unLenWrt+1))//
	{
		l_flag=1;
	}
	delete[] stuInf.pchBufWrt;
	stuInf.pchBufWrt=NULL;
	return l_flag;
}
//����0���ɹ�����
//����1���ۼ�У��ʧ��
//����2����com�ڳ���
//����3��������ģʽ��������0
UINT CCom::RecvCharFromCom()
{
	while(stuInf.pchBufRcv!=NULL)
		Sleep(0);//�ȴ�������ջ��汻ȡ��
	stuInf.pchBufRcv=new char[stuInf.unLenRcv+1];//
	UINT unRL;
	if (!ReadCom(stuInf.pchBufRcv,stuInf.chRcvChar,stuInf.unLenRcv,&unRL))
	{
		if(stuInf.bIsRun)
		{
			char l_char_check=0x00;



			for (int i=1;i<stuInf.unLenRcv-1;i++)//�����ۼ�У��
			{
				l_char_check+=stuInf.pchBufRcv[i];
			}

			if (l_char_check!=stuInf.pchBufRcv[stuInf.unLenRcv-1])////�����ۼ�У������ղ���ȵ����
			{
				delete[] stuInf.pchBufRcv; 
				stuInf.pchBufRcv=NULL;
				return 1;
			}
		}
		else if(0 == unRL)
		{
			delete[] stuInf.pchBufRcv; 
			stuInf.pchBufRcv=NULL;
			return 3;
		}
		if(stuInf.bIsRun)
		{
			for (UINT i=0;i<=stuInf.unLenRcv;i++)//����strcpy����0������������Ҫѭ������
			{
				stuInf.pchBufRTmp[i]=stuInf.pchBufRcv[i];
			}
			stuInf.csRcv+=stuInf.pchBufRTmp;
			stuInf.csRcv+="\r\n";
		}
		else
			for (UINT i=0;i<=unRL;i++)//����strcpy����0������������Ҫѭ������
			{
				stuInf.pchBufRTmp[i]=stuInf.pchBufRcv[i];
			}
		//PostMessage(READ_COM,(unsigned int)stuInf.pchBufRcv,stuInf.unLenRcv);//�����Զ�����Ϣ�����̣߳�֪ͨ�����ݵ�
		
		CString l_cstr_tmp;
		l_cstr_tmp="";
		//UCHAR *buf(stuInf.pchBufRTmp);
		if(stuInf.bIsRun)
		{
			for (UINT j=0; j<stuInf.unLenRcv; j++)
			{
				l_cstr_tmp.Format(_T("%02X"),(UCHAR)stuInf.pchBufRTmp[j]);
				stuInf.csRcv += l_cstr_tmp;
			}
			stuInf.csRcv+="\r\n";
		}
		else
			for (UINT j=0; j<unRL; j++)
			{
				l_cstr_tmp.Format(_T("%02X"),(UCHAR)stuInf.pchBufRTmp[j]);
				stuInf.csRcv += l_cstr_tmp;
			}
		//stuInf.pchBufRTmp[0]='\0';
		
		//CBZJDlg::RS232Parse();
		delete[] stuInf.pchBufRcv; 
		stuInf.pchBufRcv=NULL;
		return 0;
	}
	delete[] stuInf.pchBufRcv; 
	stuInf.pchBufRcv=NULL;
	return 2;
}

//����1-6��Э������
//����0��������
UINT CCom::RS232Parse()
{
	/*if (stuInf.pchBufRTmp[0]=='C' && stuInf.pchBufRTmp[1]=='4' && stuInf.pchBufRTmp[5]==0x00)	//�ж��Ƿ�stm��ʼ��
	{
		//stuInf.bCmdSuc=1;										//�ɹ�����

		return 2;																		//�������ⲿ����Ӧ����
	}
	
	for (int i=0;i<2;i++)
	{
		if (stuInf.pchBufRTmp[i]!=stuInf.pchBufWTmp[i+1])//�ж��Ƿ��Ƿ��͵�����CX
		{
			return 1;
		}
	}*/
	UINT unCmd=0;

	/*if (stuInf.pchBufRTmp[1]==stuInf.chRcvID)	//�ж��Ƿ��Ƿ��͵�ID
	{
		return unCmd;
	}

	stuInf.chRcvID = stuInf.pchBufRTmp[1];*/
	
	switch(stuInf.pchBufRTmp[1])
	{
	case 'E'://Ҫ�����
		unCmd=1;
		break;
	case 'W'://��Ƭ����
		unCmd=2;
		break;
	case 'I'://��ʼ�����
		unCmd=3;stuInf.bCmdSuc=1;//�ɹ�����
		break;
	case '1'://1-3XY����λ��У׼
	case '2':
	case '3':
		unCmd=4;//stuInf.bCmdSuc=1;//�ɹ�����
		break;
	case 'M'://�������������
		unCmd=5;stuInf.bCmdSuc=1;//�ɹ�����
		break;
	case 'L'://λ���趨���
		unCmd=6;//stuInf.bCmdSuc=1;//�ɹ�����
		break;
	case 'O':
		unCmd=7;//�ط���λ
		break;
	/*case 'F'://EFG������
		unCmd=8;
		break;*/
	default:
		break;
	}


	//stuInf.bCmdSuc=1;//�ɹ�����
	return unCmd;
}
BOOL CCom::ReSend()
{

	if(stuInf.bCmdSuc)
		return 0;
	UINT l_flag=0;
	while(stuInf.pchBufWrt != NULL)
		Sleep(1);
	stuInf.pchBufWrt=new char[stuInf.unLenWrt+1];//������̬���飬���Ȱ����޸�
	memset(stuInf.pchBufWrt, 0, stuInf.unLenWrt+1);

	for (UINT i=0;i<stuInf.unLenWrt;i++)//����strcpy����0������������Ҫѭ������
	{
		stuInf.pchBufWrt[i]=stuInf.pchBufWTmp[i];
	}

	if(WriteCom(stuInf.pchBufWrt,stuInf.unLenWrt))//
	{
		l_flag=1;
	}

	delete[] stuInf.pchBufWrt;
	stuInf.pchBufWrt=NULL;
	return l_flag;

}