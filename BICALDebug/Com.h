#pragma once
typedef struct ComStruct
{
	HANDLE s_hd;				//串口句柄
	DWORD s_error;				//错误信息，由GetLastError()返回结果
	DCB s_dcb;					//DCB结构定义
	COMMTIMEOUTS s_timeout;		//超时结构，超时设置通过这个结构完成
	COMSTAT s_stat;				//也是一个结构，作为ClearCommError(hdCom,&ComError,&ComStat)
	bool s_bool_schar_recv;		//是否接到起始位，开始接收
}ComStruct;
//串口收发数据定义
typedef struct ComInfo
{
	//需要初始化数据
	char chRcvChar;							//	接收起始字符
	char chWrtChar;							//	发送起始字符
	UINT unLenRcv,unLenWrt;					//	接收/发送长度
	bool bIsRun;							//	是否运行1，运行，只接收带起始字符的，0，非运行，则接收所有字符
	//操作数据
	CString csRcv;							//	接收显示字符串
	CString csWrt;							//	发送显示字符串
	char *pchBufRcv,*pchBufRTmp;			//	接收字符双缓存,tmp用于反馈的比较
	char *pchBufWrt,*pchBufWTmp;			//	发送字符缓存，用於比較第幾次发送的tmp缓存
	BOOL bCmdSuc;							//	命令成功发送(既已经接到反馈，在命令第一次发送时置0，在命令成功接收之后置1
	char chWrtID,chRcvID;								//	串口命令发送id，每发不同的一条命令，id++
	bool bComOpen;							//	指示是否有com打开
}ComInfo;
class CCom
{
public:
	CCom(void);
	~CCom(void);
	ComStruct m_struct_com;
	ComInfo stuInf;
	//BOOL InitCom();
	//基本操作
	BOOL OpenCom(UINT comN);									//打开com
	BOOL CloseCom();											//关闭com
	UINT ReadCom(char *&readBuff,char startChar,UINT recvLen,UINT *readLen);	//读com
	UINT WriteCom(char *&writeBuff,UINT writeLen);				//写com
	//com其他操作相关
	UINT SendCharToComNOID(const char* comStr, UINT len);	//发送字符串沒有id加。主要用于初始化操作，防止id被改掉
	UINT RecvCharFromCom(char *buf,UINT len,bool *crc);						//接收字符串
	UINT RS232Parse();							//协议解析
	BOOL ReSend();								//重新发送上一次发送的字符串，前提wtmp中有
	void ClearCom();							//清除接收缓存
};
extern CCom gclsCom;
