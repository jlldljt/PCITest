#include "StdAfx.h"
#include "Com.h"


CCom::CCom(void)
{
	
	stuInf.bIsRun=1;///////////////////////////
	m_struct_com.s_bool_schar_recv=0;

	//串口相关初始化
	stuInf.pchBufRcv=NULL;
	stuInf.pchBufWrt=NULL;
	stuInf.chRcvChar=0xfc;
	stuInf.chWrtChar=0xfc;
	stuInf.unLenWrt=10;
	stuInf.unLenRcv=3;
	stuInf.pchBufRTmp=new char[stuInf.unLenRcv+1];//在destorywindows中delete
	stuInf.pchBufWTmp=new char[stuInf.unLenWrt+1];//写入tmp，用于比较
	stuInf.csRcv="";
	stuInf.csWrt="";
	stuInf.bCmdSuc=0;
	stuInf.chWrtID=0x00;
	stuInf.chRcvID=0x00;
}

CCom gclsCom;

CCom::~CCom(void)
{
	delete[] stuInf.pchBufRTmp;//在init中new
	stuInf.pchBufRTmp=NULL;
	delete[] stuInf.pchBufWTmp;//在init中new
	stuInf.pchBufWTmp=NULL;
}
BOOL  CCom::OpenCom(UINT comN)
{
	//hdCom=CreateFile("COM3",GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);//FILE_FLAG_OVERLAPPED
	CString l_cstr_name_com;
	l_cstr_name_com.Format(_T("COM%d"),comN);
	m_struct_com.s_hd=CreateFile(l_cstr_name_com,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);//非重叠
	if(m_struct_com.s_hd==INVALID_HANDLE_VALUE)
	{
		m_struct_com.s_error=GetLastError();
		if(m_struct_com.s_error=5)
			::AfxMessageBox(_T("没有发现串口或串口已被占用"),MB_ICONWARNING,TRUE);
		stuInf.bComOpen=0;
		return false;
		
	}
	else
	{
		SetupComm(m_struct_com.s_hd,2048,2048);//分配缓冲区
		GetCommState(m_struct_com.s_hd,&m_struct_com.s_dcb);//读取DCB结构
		m_struct_com.s_dcb.BaudRate=57600;//波特率为9600
		m_struct_com.s_dcb.ByteSize=8;//数据位为8位
		m_struct_com.s_dcb.fParity=1;//奇偶校验
		m_struct_com.s_dcb.Parity=NOPARITY;//ODDPARITY;
		m_struct_com.s_dcb.StopBits=0;//一位停止位
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
//读端口函数
//读取初始值后recvLen位，返回0
//读取到初始值，返回1
//没有读到初始值，返回2
//接收字符数超限，返回3
//读取长度不到 ，返回4
//当非运行态readlen是null，返回5
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
		if (!m_struct_com.s_bool_schar_recv)//如果没找到起始位，找起始位
		{

			for (int i=0;i<(int)l_dword_len_in;i++)//找起始位
			{
				l_bool_flag_read=ReadFile(m_struct_com.s_hd,&l_char_buff,1,&l_dword_len_read,NULL);
				if(*l_char_buff==startChar)//
				{
					m_struct_com.s_bool_schar_recv=1;//有起始位的接收开始
					break;
				}
			}
		}

		if (!m_struct_com.s_bool_schar_recv)//如果没有找到起始位，返回
			return 2; 
	}
	ClearCommError(m_struct_com.s_hd,&m_struct_com.s_error,&m_struct_com.s_stat);
	l_dword_len_in=m_struct_com.s_stat.cbInQue;

	if(stuInf.bIsRun)
	{
		if(l_dword_len_in<recvLenTmp)//如果缓存小于待接收字节
			return 1;
		l_bool_flag_read=ReadFile(m_struct_com.s_hd,&l_char_buff,recvLenTmp,&l_dword_len_read,NULL);//读取端口值
		if (l_dword_len_read<recvLenTmp)
		{
			return 4;
		}
	}
	else
	{
		if(l_dword_len_in > recvLenTmp)
			l_dword_len_in = recvLenTmp;
		l_bool_flag_read=ReadFile(m_struct_com.s_hd,&l_char_buff,l_dword_len_in,&l_dword_len_read,NULL);//读取端口值
	}

	
	
	*(l_char_buff+l_dword_len_read)='\0';//最后增加终止符

	if(!l_bool_flag_read)
	{
		if(GetLastError()==ERROR_IO_PENDING)
		{
			m_struct_com.s_error=GetLastError();
			if(m_struct_com.s_error==ERROR_IO_PENDING);
		}
	}

	m_struct_com.s_bool_schar_recv=0;//有起始位的接收结束
	if(stuInf.bIsRun)
	{
		readBuff[0]=stuInf.chRcvChar;
		for (int i=0;i<=(int)l_dword_len_read;i++)//由于strcpy碰到0即结束，这里要循环复制
		{
			readBuff[i+1]=l_char_buff[i];
		}
	}
	else
	{
		for (int i=0;i<=(int)l_dword_len_read;i++)//由于strcpy碰到0即结束，这里要循环复制
		{
			readBuff[i]=l_char_buff[i];
		}
		if(NULL == readLen)
			return 5;
		*readLen = l_dword_len_read;
	}

	//strcpy(readBuff,l_char_buff);//将缓存copy到缓存tmp
	return 0;
}

//写端口函数
//正常发送，返回0
//发送字符数太多，返回3
//发送长度不到，返回4
UINT  CCom::WriteCom(char *&writeBuff,UINT writeLen)
{
	char l_char_buff[100];//局部变量，用于发送数据，防止writebuff被修改
	DWORD l_dword_len_read=0;//发送出去的字符长度
	memset(l_char_buff, 0, 100);

	if(writeLen>98)return 3;//如果需要发送的字符数太多

	if (strlen(writeBuff)>98)return 3;

	for (int i=0;i<=(int)writeLen;i++)//由于strcpy碰到0即结束，这里要循环复制
	{
		l_char_buff[i]=writeBuff[i];
	}

	//strcpy(l_char_buff,writeBuff);//将缓存copy到缓存l_char_buff
 	ClearCommError(m_struct_com.s_hd,&m_struct_com.s_error,&m_struct_com.s_stat);
	WriteFile(m_struct_com.s_hd,&l_char_buff,writeLen,&l_dword_len_read,NULL);//发送
	m_struct_com.s_error=GetLastError();

	if (l_dword_len_read<writeLen)//发送长度小于要发送的长度
		return 4;

	return 0;
}

//发送s_uint_len_recv+1（1位是起始位s_char_mark_writechar)个字符给下位机
//返回0，成功
//返回1，失败
UINT CCom::SendCharToCom(const char* comStr ,bool sucDisable=0)//输入命令+data
{
	UINT l_flag=0;
	while(stuInf.pchBufWrt != NULL)
		Sleep(1);
	stuInf.pchBufWrt=new char[stuInf.unLenWrt+1];//建立动态数组，长度按需修改
	memset(stuInf.pchBufWrt, 0, stuInf.unLenWrt+1);
	stuInf.pchBufWrt[0]=stuInf.chWrtChar;//起始位
	stuInf.pchBufWrt[1]=stuInf.chWrtID;//第1次发送id++
	stuInf.chWrtID++;
	//strcat(stuInf.pchBufWrt,comStr);//command
	if(sucDisable)
		stuInf.bCmdSuc=1;
	else
		stuInf.bCmdSuc=0;//成功接收标志置0
	stuInf.pchBufWrt[stuInf.unLenWrt-1] = stuInf.pchBufWrt[1];
	for (int i=2;i<stuInf.unLenWrt-1;i++)//累加校验位
	{
		stuInf.pchBufWrt[i]=comStr[i-2];
		stuInf.pchBufWrt[stuInf.unLenWrt-1]+=stuInf.pchBufWrt[i];
	}

	//stuInf.pchBufWrt[8]='\r';//因下位机程序需要，按需修改
	//stuInf.pchBufWrt[9]='\n';//因下位机程序需要，按需修改
	
	
	for (UINT i=0;i<stuInf.unLenWrt;i++)//由于strcpy碰到0即结束，这里要循环复制
	{
		stuInf.pchBufWTmp[i]=stuInf.pchBufWrt[i];
	}

	//strcpy(stuInf.pchBufWTmp,stuInf.pchBufWrt);//保存之前的命令，用于判断第几次发送,在sysinit中初始化，注意长度;
	if(WriteCom(stuInf.pchBufWrt,stuInf.unLenWrt))//
	{
		l_flag=1;
	}
	delete[] stuInf.pchBufWrt;
	stuInf.pchBufWrt=NULL;
	return l_flag;
}
//发送s_uint_len_recv+1（1位是起始位s_char_mark_writechar)个字符给下位机
//返回0，成功
//返回1，失败
UINT CCom::SendCharToComNOID(const char* comStr)
{
	UINT l_flag=0;
	while(stuInf.pchBufWrt != NULL)
		Sleep(1);
	stuInf.pchBufWrt=new char[stuInf.unLenWrt+2];//建立动态数组，长度按需修改
	memset(stuInf.pchBufWrt, 0, stuInf.unLenWrt+2);
	stuInf.pchBufWrt[0]=stuInf.chWrtChar;//起始位

	//strcat(stuInf.pchBufWrt,comStr);//command

	for (int i=1;i<6;i++)//累加校验位
	{
		stuInf.pchBufWrt[i]=comStr[i-1];
		stuInf.pchBufWrt[7]+=stuInf.pchBufWrt[i];
	}

	//stuInf.pchBufWrt[8]='\r';//因下位机程序需要，按需修改
	//stuInf.pchBufWrt[9]='\n';//因下位机程序需要，按需修改
	BOOL l_unequ=0;

	for (int i=0;i<6;i++)//判断与上一条指令是否相等
	{
		if (stuInf.pchBufWTmp[i]!=stuInf.pchBufWrt[i])//不相等
		{
			l_unequ=1;
			break;
		}
	}

	if (l_unequ)//不相等，第一次发送
	{ 
		//stuInf.chID++;
		stuInf.pchBufWrt[6]=stuInf.chWrtID;//第1次发送id++

		stuInf.bCmdSuc=0;//成功接收标志置0
	}
	else//相等
	{
		stuInf.pchBufWrt[6]=stuInf.chWrtID;
	}

	for (UINT i=0;i<stuInf.unLenWrt+1;i++)//由于strcpy碰到0即结束，这里要循环复制
	{
		stuInf.pchBufWTmp[i]=stuInf.pchBufWrt[i];
	}

	//strcpy(stuInf.pchBufWTmp,stuInf.pchBufWrt);//保存之前的命令，用于判断第几次发送,在sysinit中初始化，注意长度;
	if(WriteCom(stuInf.pchBufWrt,stuInf.unLenWrt+1))//
	{
		l_flag=1;
	}
	delete[] stuInf.pchBufWrt;
	stuInf.pchBufWrt=NULL;
	return l_flag;
}
//返回0，成功接收
//返回1，累加校验失败
//返回2，读com口出错
//返回3，非运行模式读到长度0
UINT CCom::RecvCharFromCom()
{
	while(stuInf.pchBufRcv!=NULL)
		Sleep(0);//等待缓存接收缓存被取空
	stuInf.pchBufRcv=new char[stuInf.unLenRcv+1];//
	UINT unRL;
	if (!ReadCom(stuInf.pchBufRcv,stuInf.chRcvChar,stuInf.unLenRcv,&unRL))
	{
		if(stuInf.bIsRun)
		{
			char l_char_check=0x00;



			for (int i=1;i<stuInf.unLenRcv-1;i++)//反馈累加校验
			{
				l_char_check+=stuInf.pchBufRcv[i];
			}

			if (l_char_check!=stuInf.pchBufRcv[stuInf.unLenRcv-1])////反馈累加校验与接收不相等的情况
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
			for (UINT i=0;i<=stuInf.unLenRcv;i++)//由于strcpy碰到0即结束，这里要循环复制
			{
				stuInf.pchBufRTmp[i]=stuInf.pchBufRcv[i];
			}
			stuInf.csRcv+=stuInf.pchBufRTmp;
			stuInf.csRcv+="\r\n";
		}
		else
			for (UINT i=0;i<=unRL;i++)//由于strcpy碰到0即结束，这里要循环复制
			{
				stuInf.pchBufRTmp[i]=stuInf.pchBufRcv[i];
			}
		//PostMessage(READ_COM,(unsigned int)stuInf.pchBufRcv,stuInf.unLenRcv);//发送自定义消息给主线程，通知有数据到
		
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

//返回1-6，协议命令
//返回0，无命令
UINT CCom::RS232Parse()
{
	/*if (stuInf.pchBufRTmp[0]=='C' && stuInf.pchBufRTmp[1]=='4' && stuInf.pchBufRTmp[5]==0x00)	//判断是否stm初始化
	{
		//stuInf.bCmdSuc=1;										//成功接收

		return 2;																		//反馈给外部做相应处理
	}
	
	for (int i=0;i<2;i++)
	{
		if (stuInf.pchBufRTmp[i]!=stuInf.pchBufWTmp[i+1])//判断是否是发送的命令CX
		{
			return 1;
		}
	}*/
	UINT unCmd=0;

	/*if (stuInf.pchBufRTmp[1]==stuInf.chRcvID)	//判断是否是发送的ID
	{
		return unCmd;
	}

	stuInf.chRcvID = stuInf.pchBufRTmp[1];*/
	
	switch(stuInf.pchBufRTmp[1])
	{
	case 'E'://要求读档
		unCmd=1;
		break;
	case 'W'://无片坐标
		unCmd=2;
		break;
	case 'I'://初始化完成
		unCmd=3;stuInf.bCmdSuc=1;//成功接收
		break;
	case '1'://1-3XY联动位置校准
	case '2':
	case '3':
		unCmd=4;//stuInf.bCmdSuc=1;//成功接收
		break;
	case 'M'://备料盘运行完成
		unCmd=5;stuInf.bCmdSuc=1;//成功接收
		break;
	case 'L'://位置设定完毕
		unCmd=6;//stuInf.bCmdSuc=1;//成功接收
		break;
	case 'O':
		unCmd=7;//重发档位
		break;
	/*case 'F'://EFG检测完成
		unCmd=8;
		break;*/
	default:
		break;
	}


	//stuInf.bCmdSuc=1;//成功接收
	return unCmd;
}
BOOL CCom::ReSend()
{

	if(stuInf.bCmdSuc)
		return 0;
	UINT l_flag=0;
	while(stuInf.pchBufWrt != NULL)
		Sleep(1);
	stuInf.pchBufWrt=new char[stuInf.unLenWrt+1];//建立动态数组，长度按需修改
	memset(stuInf.pchBufWrt, 0, stuInf.unLenWrt+1);

	for (UINT i=0;i<stuInf.unLenWrt;i++)//由于strcpy碰到0即结束，这里要循环复制
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