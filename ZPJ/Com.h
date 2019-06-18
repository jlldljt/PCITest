#pragma once



typedef struct ComStruct
{
  HANDLE hd;				//串口句柄
  DWORD error;				//错误信息，由GetLastError()返回结果
  DCB dcb;					//DCB结构定义
  COMMTIMEOUTS timeout;		//超时结构，超时设置通过这个结构完成
  COMSTAT stat;				//也是一个结构，作为ClearCommError(hdCom,&ComError,&ComStat)
} ComStruct;
//串口收发数据定义
typedef struct ComInfo
{
  CString str;							//	接收显示字符串
  bool bComOpen;							//	指示是否有com打开
} ComInfo;

class CCom
{
public:
  CCom(void);
  ~CCom(void);
  ComInfo stuInf;
  //基本操作
//private:
  ComStruct m_struct_com;
  BOOL OpenCom(UINT no);									//打开com
  BOOL OpenCom(CString no);
  BOOL CloseCom();											//关闭com
  int ReadCom(char* buf, UINT len);	//读com
  int WriteCom(const char* writeBuff, UINT writeLen);				//写com
  //com其他操作相关
  int SendCharToCom(const char* buf, UINT len);	//发送字符串沒有id加。主要用于初始化操作，防止id被改掉
  int SendCharToCom(char* buf, UINT len, bool crc);
  int RecvCharFromCom(char* buf, UINT len, bool* crc, UINT timeout);						//接收字符串
  void ClearCom();							//清除接收缓存
  // 枚举出pc上有效的串口号
  void EnumerateSerialPorts(CStringArray& ports);
  UINT SetCrc(char* buff, UINT len);
  UINT SetSum(char* buf, UINT len);
public:
  int AutoOpen(const char* sbuf, int slen, char* rbuf, int rlen, bool* crc = NULL, int timeout = 10);
  int SendAndRecv(const char* sbuf, int slen, char* rbuf, int rlen, bool* crc = NULL, int rTimeout = 10);
  INT32 WriteVar(UINT32 no, INT32 val);
  INT32 ReadVar(UINT32 no, INT32& val);
};

extern CCom gclsCom;