#pragma once
typedef struct ComStruct
{
	HANDLE s_hd;				//���ھ��
	DWORD s_error;				//������Ϣ����GetLastError()���ؽ��
	DCB s_dcb;					//DCB�ṹ����
	COMMTIMEOUTS s_timeout;		//��ʱ�ṹ����ʱ����ͨ������ṹ���
	COMSTAT s_stat;				//Ҳ��һ���ṹ����ΪClearCommError(hdCom,&ComError,&ComStat)
	bool s_bool_schar_recv;		//�Ƿ�ӵ���ʼλ����ʼ����
}ComStruct;
//�����շ����ݶ���
typedef struct ComInfo
{
	//��Ҫ��ʼ������
	char chRcvChar;							//	������ʼ�ַ�
	char chWrtChar;							//	������ʼ�ַ�
	UINT unLenRcv,unLenWrt;					//	����/���ͳ���
	bool bIsRun;							//	�Ƿ�����1�����У�ֻ���մ���ʼ�ַ��ģ�0�������У�����������ַ�
	//��������
	CString csRcv;							//	������ʾ�ַ���
	CString csWrt;							//	������ʾ�ַ���
	char *pchBufRcv,*pchBufRTmp;			//	�����ַ�˫����,tmp���ڷ����ıȽ�
	char *pchBufWrt,*pchBufWTmp;			//	�����ַ����棬��춱��^�ڎ״η��͵�tmp����
	BOOL bCmdSuc;							//	����ɹ�����(���Ѿ��ӵ��������������һ�η���ʱ��0��������ɹ�����֮����1
	char chWrtID,chRcvID;								//	���������id��ÿ����ͬ��һ�����id++
	bool bComOpen;							//	ָʾ�Ƿ���com��
}ComInfo;
class CCom
{
public:
	CCom(void);
	~CCom(void);
	ComStruct m_struct_com;
	ComInfo stuInf;
	//BOOL InitCom();
	//��������
	BOOL OpenCom(UINT comN);									//��com
	BOOL CloseCom();											//�ر�com
	UINT ReadCom(char *&readBuff,char startChar,UINT recvLen,UINT *readLen);	//��com
	UINT WriteCom(char *&writeBuff,UINT writeLen);				//дcom
	//com�����������
	UINT SendCharToComNOID(const char* comStr, UINT len);	//�����ַ����]��id�ӡ���Ҫ���ڳ�ʼ����������ֹid���ĵ�
	UINT RecvCharFromCom(char *buf,UINT len,bool *crc);						//�����ַ���
	UINT RS232Parse();							//Э�����
	BOOL ReSend();								//���·�����һ�η��͵��ַ�����ǰ��wtmp����
	void ClearCom();							//������ջ���
};
extern CCom gclsCom;
