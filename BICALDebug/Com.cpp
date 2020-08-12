#include "StdAfx.h"
#include "Com.h"


CCom::CCom(void)
{
	m_struct_com.s_bool_schar_recv=0;

	//串口相关初始化
	stuInf.pchBufRcv=NULL;
	stuInf.pchBufWrt=NULL;
	stuInf.chRcvChar=0x01;
	stuInf.chWrtChar=0x01;
	stuInf.unLenWrt=8;
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
	if (comN<10)
	{l_cstr_name_com.Format(_T("COM%d"),comN);
	} 
	else
	{l_cstr_name_com.Format(_T("\\\\.\\COM%d"),comN);
	}
	
	//CreateFile(
	//	"\\\\.\\COM10",           // 定义串口名
	//	fdwAccess,                // 存取模式（读写）
	//	0,                               // 共享模式：必须设置为0，表示设备独占使用
	//	NULL,                        // 保密性
	//	OPEN_EXISTING,      // 必须设置为OPEN_EXISTING
	//	0,                               // 文件属性，如果是异步模式，可设置为
	//	NULL                         // 模版，串口设备必须设置为NULL
	//	);
	m_struct_com.s_hd=CreateFile(l_cstr_name_com,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);//非重叠
	if(m_struct_com.s_hd==INVALID_HANDLE_VALUE)
	{
		m_struct_com.s_error=GetLastError();
		if(m_struct_com.s_error==5)
			::AfxMessageBox(_T("没有发现串口或串口已被占用"),MB_ICONWARNING,TRUE);
		stuInf.bComOpen=0;
		return false;
		
	}
	else
	{
		SetupComm(m_struct_com.s_hd,2048,2048);//分配缓冲区
		GetCommState(m_struct_com.s_hd,&m_struct_com.s_dcb);//读取DCB结构
		m_struct_com.s_dcb.BaudRate=115200;//波特率为9600
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
	stuInf.bComOpen=0;
	return 1;
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

	//m_struct_com.s_bool_schar_recv=0;//有起始位的接收结束
	
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
	//char* l_char_buff[100];//局部变量，用于发送数据，防止writebuff被修改
	char* l_char_buff = (char*)malloc(sizeof(char)*writeLen);
	DWORD l_dword_len_read=0;//发送出去的字符长度
	memset(l_char_buff, 0, writeLen);

	//if(writeLen>98)return 3;//如果需要发送的字符数太多
	if(writeLen>5000)return 3;//如果需要发送的字符数太多
	if (strlen(writeBuff)>5000)return 3;

	for (int i=0;i<=(int)writeLen;i++)//由于strcpy碰到0即结束，这里要循环复制
	{
		l_char_buff[i]=writeBuff[i];
	}

	//strcpy(l_char_buff,writeBuff);//将缓存copy到缓存l_char_buff
 	ClearCommError(m_struct_com.s_hd,&m_struct_com.s_error,&m_struct_com.s_stat);
	WriteFile(m_struct_com.s_hd,l_char_buff,writeLen,&l_dword_len_read,NULL);//发送
	m_struct_com.s_error=GetLastError();

	if (l_dword_len_read<writeLen)//发送长度小于要发送的长度
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
//发送s_uint_len_recv+1（1位是起始位s_char_mark_writechar)个字符给下位机
//返回0，成功
//返回1，失败
UINT CCom::SendCharToComNOID(const char* comStr, UINT len)
{
	stuInf.unLenWrt = 0 == len? stuInf.unLenWrt : len+2;
	UINT l_flag=0;
	while(stuInf.pchBufWrt != NULL)
		Sleep(1);
	stuInf.pchBufWrt=new char[stuInf.unLenWrt+2];//建立动态数组，长度按需修改
	memset(stuInf.pchBufWrt, 0, stuInf.unLenWrt+2);
	//stuInf.pchBufWrt[0]=stuInf.chWrtChar;//起始位

	for (int i=0;i<stuInf.unLenWrt-2;i++)//累加校验位
	{
		stuInf.pchBufWrt[i]=comStr[i];
	}
	unsigned short crcresult = getCRC16(stuInf.pchBufWrt , stuInf.unLenWrt-2);
	char temp1 = crcresult & 0xff;
	char temp0 = (crcresult >> 8) & 0xff;
	stuInf.pchBufWrt[stuInf.unLenWrt-2]=temp1;
	stuInf.pchBufWrt[stuInf.unLenWrt-1]=temp0;
	
	stuInf.bCmdSuc=0;//成功接收标志置0

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
//返回接收长度，成功接收
//返回接收长度+1，CRC匹配失败
//返回-1，读com口出错
//返回0~接收到但长度不够
//buf 用户接受数组
//len用户接受长度
//crc 1校验通过
UINT CCom::RecvCharFromCom(char *buf,UINT len,bool *crc)
{
	while(stuInf.pchBufRcv!=NULL)
		Sleep(0);//等待缓存接收缓存被取空
	stuInf.unLenRcv = 0 == len? stuInf.unLenWrt : len;
	stuInf.pchBufRcv=new char[stuInf.unLenRcv+1];//
	UINT unRL;//读到的长度
	UINT totalLen = 0;
	UINT zeroCnt = 0;
	*crc = 0;
	while (1)
	{
		Sleep(1);
		if (!ReadCom(stuInf.pchBufRcv,stuInf.chRcvChar,1,&unRL))
		{
			//totalLen += unRL;//记录收到的长度

			if(0 == unRL)
			{
				zeroCnt++;
				if (0==totalLen && zeroCnt>3000)//大约超过500ms无数据，退出
				{
					break;
				}
				else if (0!=totalLen && zeroCnt>100)//大约超过500ms无数据，退出
				{
					break;
				}
				continue;
			}
			zeroCnt=0;
			for (UINT i=0;i<unRL;i++)//由于strcpy碰到0即结束，这里要循环复制
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
	
//返回1-6，协议命令
//返回0，无命令
UINT CCom::RS232Parse()
{

	UINT unCmd=0;

	
	switch(stuInf.pchBufRTmp[1])
	{
	case 'E'://EFG检测完成
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