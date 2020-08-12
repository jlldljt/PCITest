#include "StdAfx.h"
#include "Com.h"


CCom::CCom(void)
{
	m_struct_com.s_bool_schar_recv=0;

	//������س�ʼ��
	stuInf.pchBufRcv=NULL;
	stuInf.pchBufWrt=NULL;
	stuInf.chRcvChar=0x01;
	stuInf.chWrtChar=0x01;
	stuInf.unLenWrt=8;
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
	if (comN<10)
	{l_cstr_name_com.Format(_T("COM%d"),comN);
	} 
	else
	{l_cstr_name_com.Format(_T("\\\\.\\COM%d"),comN);
	}
	
	//CreateFile(
	//	"\\\\.\\COM10",           // ���崮����
	//	fdwAccess,                // ��ȡģʽ����д��
	//	0,                               // ����ģʽ����������Ϊ0����ʾ�豸��ռʹ��
	//	NULL,                        // ������
	//	OPEN_EXISTING,      // ��������ΪOPEN_EXISTING
	//	0,                               // �ļ����ԣ�������첽ģʽ��������Ϊ
	//	NULL                         // ģ�棬�����豸��������ΪNULL
	//	);
	m_struct_com.s_hd=CreateFile(l_cstr_name_com,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);//���ص�
	if(m_struct_com.s_hd==INVALID_HANDLE_VALUE)
	{
		m_struct_com.s_error=GetLastError();
		if(m_struct_com.s_error==5)
			::AfxMessageBox(_T("û�з��ִ��ڻ򴮿��ѱ�ռ��"),MB_ICONWARNING,TRUE);
		stuInf.bComOpen=0;
		return false;
		
	}
	else
	{
		SetupComm(m_struct_com.s_hd,2048,2048);//���仺����
		GetCommState(m_struct_com.s_hd,&m_struct_com.s_dcb);//��ȡDCB�ṹ
		m_struct_com.s_dcb.BaudRate=115200;//������Ϊ9600
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
	stuInf.bComOpen=0;
	return 1;
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
	UINT recvLenTmp=recvLen;
	DWORD l_dword_len_in=0;
	DWORD l_dword_len_read=0;
	BOOL l_bool_flag_read=0;
	char l_char_buff[100];
	memset(l_char_buff, 0, 100);
	if(recvLenTmp>98)return 3;
	ClearCommError(m_struct_com.s_hd,&m_struct_com.s_error,&m_struct_com.s_stat);
	l_dword_len_in=m_struct_com.s_stat.cbInQue;
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

	//m_struct_com.s_bool_schar_recv=0;//����ʼλ�Ľ��ս���
	
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
	//char* l_char_buff[100];//�ֲ����������ڷ������ݣ���ֹwritebuff���޸�
	char* l_char_buff = (char*)malloc(sizeof(char)*writeLen);
	DWORD l_dword_len_read=0;//���ͳ�ȥ���ַ�����
	memset(l_char_buff, 0, writeLen);

	//if(writeLen>98)return 3;//�����Ҫ���͵��ַ���̫��
	if(writeLen>5000)return 3;//�����Ҫ���͵��ַ���̫��
	if (strlen(writeBuff)>5000)return 3;

	for (int i=0;i<=(int)writeLen;i++)//����strcpy����0������������Ҫѭ������
	{
		l_char_buff[i]=writeBuff[i];
	}

	//strcpy(l_char_buff,writeBuff);//������copy������l_char_buff
 	ClearCommError(m_struct_com.s_hd,&m_struct_com.s_error,&m_struct_com.s_stat);
	WriteFile(m_struct_com.s_hd,l_char_buff,writeLen,&l_dword_len_read,NULL);//����
	m_struct_com.s_error=GetLastError();

	if (l_dword_len_read<writeLen)//���ͳ���С��Ҫ���͵ĳ���
		return 4;

	return 0;
}

/*unsigned short getCRC16(volatile char *ptr,UINT len) 
{ 
	unsigned char i; 
	unsigned short crc = 0xFFFF; 
	if(len==0){
		len = 1;
	} 
	while(len--){   
		crc ^= *ptr; 
		for(i=0; i<8; i++){ 
			if(crc&1){ 
				crc >>= 1;  
				crc ^= 0xA001; 
			}  
			else{
				crc >>= 1;
			} 
		}         
		ptr++; 
	} 
	return(crc); 
} */

unsigned short getCRC16(volatile char *pptr,unsigned short len) 
{ 
	unsigned char *ptr = (unsigned char *)pptr;
	unsigned char i; 
	unsigned short crc = 0xFFFF; 
	if(len==0){
		len = 1;
	} 
	while(len--){   
		crc ^= *ptr; 
		for(i=0; i<8; i++){ 
			if(crc&1){ 
				crc >>= 1;  
				crc ^= 0xA001; 
			}  
			else{
				crc >>= 1;
			} 
		}         
		ptr++; 
	} 
	return(crc); 
} 
//����s_uint_len_recv+1��1λ����ʼλs_char_mark_writechar)���ַ�����λ��
//����0���ɹ�
//����1��ʧ��
UINT CCom::SendCharToComNOID(const char* comStr, UINT len)
{
	stuInf.unLenWrt = 0 == len? stuInf.unLenWrt : len+2;
	UINT l_flag=0;
	while(stuInf.pchBufWrt != NULL)
		Sleep(1);
	stuInf.pchBufWrt=new char[stuInf.unLenWrt+2];//������̬���飬���Ȱ����޸�
	memset(stuInf.pchBufWrt, 0, stuInf.unLenWrt+2);
	//stuInf.pchBufWrt[0]=stuInf.chWrtChar;//��ʼλ

	for (int i=0;i<stuInf.unLenWrt-2;i++)//�ۼ�У��λ
	{
		stuInf.pchBufWrt[i]=comStr[i];
	}
	unsigned short crcresult = getCRC16(stuInf.pchBufWrt , stuInf.unLenWrt-2);
	char temp1 = crcresult & 0xff;
	char temp0 = (crcresult >> 8) & 0xff;
	stuInf.pchBufWrt[stuInf.unLenWrt-2]=temp1;
	stuInf.pchBufWrt[stuInf.unLenWrt-1]=temp0;
	
	stuInf.bCmdSuc=0;//�ɹ����ձ�־��0

	for (UINT i=0;i<stuInf.unLenWrt;i++)
	{
		stuInf.pchBufWTmp[i]=stuInf.pchBufWrt[i];
	}

	if(WriteCom(stuInf.pchBufWrt,stuInf.unLenWrt))//
	{
		l_flag=1;
	}
	delete[] stuInf.pchBufWrt;
	stuInf.pchBufWrt=NULL;
	return l_flag;
}
//���ؽ��ճ��ȣ��ɹ�����
//���ؽ��ճ���+1��CRCƥ��ʧ��
//����-1����com�ڳ���
//����0~���յ������Ȳ���
//buf �û���������
//len�û����ܳ���
//crc 1У��ͨ��
UINT CCom::RecvCharFromCom(char *buf,UINT len,bool *crc)
{
	while(stuInf.pchBufRcv!=NULL)
		Sleep(0);//�ȴ�������ջ��汻ȡ��
	stuInf.unLenRcv = 0 == len? stuInf.unLenWrt : len;
	stuInf.pchBufRcv=new char[stuInf.unLenRcv+1];//
	UINT unRL;//�����ĳ���
	UINT totalLen = 0;
	UINT zeroCnt = 0;
	*crc = 0;
	while (1)
	{
		Sleep(1);
		if (!ReadCom(stuInf.pchBufRcv,stuInf.chRcvChar,1,&unRL))
		{
			//totalLen += unRL;//��¼�յ��ĳ���

			if(0 == unRL)
			{
				zeroCnt++;
				if (0==totalLen && zeroCnt>3000)//��Լ����500ms�����ݣ��˳�
				{
					break;
				}
				else if (0!=totalLen && zeroCnt>100)//��Լ����500ms�����ݣ��˳�
				{
					break;
				}
				continue;
			}
			zeroCnt=0;
			for (UINT i=0;i<unRL;i++)//����strcpy����0������������Ҫѭ������
			{
				buf[totalLen++]=stuInf.pchBufRcv[i];
			}
		}
		else
		{
			delete[] stuInf.pchBufRcv; 
			stuInf.pchBufRcv=NULL;
			return -1;
		}
		
		if (totalLen == len)
		{

			
			break;
		}
		

		
	}
	if (totalLen > 2)
	{
		unsigned short crcresult = getCRC16(buf,totalLen-2);
		char temp1 = crcresult & 0xff;
		char temp0 = (crcresult >> 8) & 0xff;
		if(buf[totalLen-2]==temp1 && buf[totalLen-1]==temp0)
		{
			*crc = 1;
		}
	}
	
	if(totalLen!=0)
	{
		CString l_cstr_tmp;
		l_cstr_tmp="\r\n";
		stuInf.csRcv+= l_cstr_tmp;
		if (stuInf.csRcv.GetLength()>0xff)
		{
			stuInf.csRcv = "";
		}
		for (UINT j=0; j<totalLen; j++)
		{
			l_cstr_tmp.Format(_T("%02X"),(UCHAR)buf[j]);
			stuInf.csRcv+= l_cstr_tmp;
		}
	}
	
	delete[] stuInf.pchBufRcv; 
	stuInf.pchBufRcv=NULL;
	return totalLen;
}
void CCom::ClearCom()
{
	PurgeComm(m_struct_com.s_hd,PURGE_RXCLEAR);
}
	
//����1-6��Э������
//����0��������
UINT CCom::RS232Parse()
{

	UINT unCmd=0;

	
	switch(stuInf.pchBufRTmp[1])
	{
	case 'E'://EFG������
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
	default:
		break;
	}
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