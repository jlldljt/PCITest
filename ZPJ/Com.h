#pragma once



typedef struct ComStruct
{
  HANDLE hd;				//���ھ��
  DWORD error;				//������Ϣ����GetLastError()���ؽ��
  DCB dcb;					//DCB�ṹ����
  COMMTIMEOUTS timeout;		//��ʱ�ṹ����ʱ����ͨ������ṹ���
  COMSTAT stat;				//Ҳ��һ���ṹ����ΪClearCommError(hdCom,&ComError,&ComStat)
} ComStruct;
//�����շ����ݶ���
typedef struct ComInfo
{
  CString str;							//	������ʾ�ַ���
  bool bComOpen;							//	ָʾ�Ƿ���com��
} ComInfo;

class CCom
{
public:
  CCom(void);
  ~CCom(void);
  ComInfo stuInf;
  //��������
//private:
  ComStruct m_struct_com;
  BOOL OpenCom(UINT no);									//��com
  BOOL OpenCom(CString no);
  BOOL CloseCom();											//�ر�com
  int ReadCom(char* buf, UINT len);	//��com
  int WriteCom(const char* writeBuff, UINT writeLen);				//дcom
  //com�����������
  int SendCharToCom(const char* buf, UINT len);	//�����ַ����]��id�ӡ���Ҫ���ڳ�ʼ����������ֹid���ĵ�
  int SendCharToCom(char* buf, UINT len, bool crc);
  int RecvCharFromCom(char* buf, UINT len, bool* crc, UINT timeout);						//�����ַ���
  void ClearCom();							//������ջ���
  // ö�ٳ�pc����Ч�Ĵ��ں�
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