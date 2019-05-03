#pragma once
class CTXT
{
public:
	CTXT(void);
	~CTXT(void);
	BOOL GetYMD(char* strYMD);								//得到年-月-日
	BOOL GetHMS(char* strHMS);								//得到时-分-秒
	BOOL TXTAddStr(LPCTSTR pathTXT,LPCTSTR strAdd);			//写入txt
	BOOL TXTReadRowStr(LPCTSTR pathTXT, CString& str,UINT row);//读txt某行
	//BOOL Split(CString source, CString& dest, CString division, UINT iSub);//CString字符串截取
	BOOL Split(char* source, char* &dest, char division, UINT iSub);
	void DeleteDir(CString str,bool del);					//	删除目录
	UINT CountNum(CString szDir);							//	计算一个目录下的文件数
};

