#pragma once
class CTXT
{
public:
	CTXT(void);
	~CTXT(void);
	BOOL GetYMD(char* strYMD);								//�õ���-��-��
	BOOL GetHMS(char* strHMS);								//�õ�ʱ-��-��
	BOOL TXTAddStr(LPCTSTR pathTXT,LPCTSTR strAdd);			//д��txt
	BOOL TXTReadRowStr(LPCTSTR pathTXT, CString& str,UINT row);//��txtĳ��
	//BOOL Split(CString source, CString& dest, CString division, UINT iSub);//CString�ַ�����ȡ
	BOOL Split(char* source, char* &dest, char division, UINT iSub);
	void DeleteDir(CString str,bool del);					//	ɾ��Ŀ¼
	UINT CountNum(CString szDir);							//	����һ��Ŀ¼�µ��ļ���
};

