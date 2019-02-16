#include "StdAfx.h"
#include "TXT.h"


CTXT::CTXT(void)
{
}


CTXT::~CTXT(void)
{
}

BOOL CTXT::GetYMD(char* strYMD)
{
	time_t l_time;
	l_time = time(NULL);
	struct tm *l_struct_tm;
	l_struct_tm=localtime(&l_time); 
	strftime(strYMD,20,"%Y-%m-%d",l_struct_tm);
	return 1;
}

BOOL CTXT::GetHMS(char* strHMS)
{
	time_t l_time;
	l_time = time(NULL);
	struct tm *l_struct_tm;
	l_struct_tm=localtime(&l_time); 
	strftime(strHMS,10,"%H-%M-%S",l_struct_tm);
	return 1;
}

//cfile.open������Ŀ¼����Ҫ��ǰ��Ŀ¼
BOOL CTXT::TXTAddStr(LPCTSTR pathTXT,LPCTSTR strAdd)
{
	CFile l_file;
	//CString tmp(_T(".\\check\\SortDegree.txt"));
	if(!l_file.Open(pathTXT,CFile::modeCreate | CFile::modeWrite|CFile::modeNoTruncate| CFile::shareDenyRead))
		return 0;
	char l_AnsiStr[MAX_PATH];  
	WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,strAdd,-1,l_AnsiStr,sizeof(l_AnsiStr),NULL,NULL); 
	l_file.Seek(0,CFile::end);
	l_file.Write(l_AnsiStr,CStringA(strAdd).GetLength());
	l_file.Close();
	return 1;
}
//����һ�У���������ͷ�Ƿ���Ҫ�ҵ����
BOOL CTXT::TXTReadRowStr(LPCTSTR pathTXT, CString& str, UINT row)
{
// 	CFile l_file;
// 	if(!l_file.Open(pathTXT, CFile::modeRead | CFile::shareDenyWrite))
// 		return 0;
// 	CStdioFile l_file;// ����һ��CStdioFile�����File
// 	CString l_cstr_data;//,file_temp;// ����һ��CString����Ϊһ��������
// 	//CString l_cstr_tmp;
// 	//file_temp="check\\qz_42.txt";   //�����ļ���������ļ���
// 	CString fileName(pathTXT);//CString a (_T ("aaa"));
// 	if(!l_file.Open(fileName,CFile::modeRead | CFile::shareDenyWrite))
// 	{
// 		str=NULL;
// 		return 0;
// 	}
// 	for(int j=0;j<row;j++)
// 	{
// 		l_file.ReadString(l_cstr_data);	
// 		//AfxExtractSubString( l_cstr_tmp, (LPCTSTR)l_cstr_data, 0, '-');
// 		//int tmp=_wtoi(l_cstr_tmp);
// 	}
// 	l_file.Close();
	//////////////////////////////////////////////////////////////////////////
// 	try
// 	{
// 		CFile l_file(pathTXT, CFile::modeRead | CFile::shareDenyWrite);
// 		char* l_char_buf = NULL;
// 		char l_char_tmp[256];
// 		DWORD l_dword_len = (DWORD)l_file.GetLength();
// 		l_char_buf = new char[l_dword_len + 1];
// 		memset(l_char_buf, 0, l_dword_len + 1);
// 		l_file.Read(l_char_buf, l_dword_len);
// 		l_file.Close();
// 		UINT l_uint_current_row=1;
// 		int itmp=0;
// 		for(int c=0;c<l_dword_len+1;c++)
// 		{
// 			if (l_uint_current_row==row)
// 			{
// 				l_char_tmp[itmp]=l_char_buf[c];
// 				itmp++;
// 			}
// 			if(l_char_buf[c]=='\n')
// 				if (l_char_buf[c-1]=='\r')
// 					l_uint_current_row++;
// 			if (l_uint_current_row>row)
// 			{
// 				l_char_tmp[c+1]=0;
// 				break;
// 			}
// 			
// 		}
// 		if (l_uint_current_row<row)
// 		{
// 			str="";
// 			return 0;
// 		}
// 		str=l_char_tmp;
// 		//CString str(l_char_buf);
// 		delete[] l_char_buf;
// 		l_char_buf = NULL;
// 
// 		AfxMessageBox(str);
// 	}
// 	catch (CException* e)
// 	{
// 		e->ReportError();
// 		e->Delete();
// 	}
// 	//////////////////////////////////////////////////////////////////////////
// 	return 1;
	CFile l_file_;
	if(!l_file_.Open(pathTXT, CFile::modeRead | CFile::shareDenyWrite))
		return 0;
	char* l_char_buf = NULL;
	char l_char_tmp[256]={0};
	DWORD l_dword_len = (DWORD)l_file_.GetLength();
	l_char_buf = new char[l_dword_len + 1];
	memset(l_char_buf, 0, l_dword_len + 1);
	l_file_.Read(l_char_buf, l_dword_len);
	l_file_.Close();

	BOOL rowend=0;//һ�ж�����
	UINT l_i=0;		//��buf��ȡ���ڼ����ַ�
	int itmp=0;		//���ڱ���һ���ַ���

	while(l_i<l_dword_len)
	{
		while (!rowend && l_i<l_dword_len)//���һ�ж�ȡû��ϣ�����һ�д������������������
		{
			if(l_char_buf[l_i]=='\n')
				if (l_char_buf[l_i-1]=='\r')
				{
					rowend=1;
					
				}
				l_char_tmp[itmp++]=l_char_buf[l_i++];
		}
		if (atoi(l_char_tmp)==row)//�������ǵ�ǰҪ�ҵ�
		{
			str=l_char_tmp;
			delete[] l_char_buf;
			l_char_buf = NULL;
			return 1;
		}
		else if (atoi(l_char_tmp)<(int)row )
		{
			memset(l_char_tmp,0,256);
			itmp=0;
			rowend=0;
		}
		else if (atoi(l_char_tmp)>(int)row)
		{
			delete[] l_char_buf;
			l_char_buf = NULL;
			return 0;
		}

	}
	delete[] l_char_buf;
	l_char_buf = NULL;
	return 0;
}
// 
// CFile mFile;
// CFileException fe;
// BOOL bRet = mFile.Open(_T("D:\\PPPPPPPPPPP\\temp.txt"),CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::, &fe);
// if (!bRet)
// {
// 	switch (fe.m_cause)
// 	{
// 	case CFileException::genericException:
// 		break;
// 	case CFileException::fileNotFound:
// 		break;
// 	case CFileException::badPath:
// 		MessageBox(_T("path error"));
// 		break;
// 	case CFileException::tooManyOpenFiles:
// 		break;
// 	case CFileException::accessDenied:
// 		break;
// 	case CFileException::invalidFile:
// 		break;
// 	case CFileException::removeCurrentDir:
// 		break;
// 	case CFileException::directoryFull:
// 		break;
// 	case CFileException::badSeek:
// 		break;
// 	case CFileException::hardIO:
// 		break;
// 	case CFileException::sharingViolation:
// 		break;
// 	case CFileException::lockViolation:
// 		break;
// 	case CFileException::diskFull:
// 		break;
// 	case CFileException::endOfFile:
// 		break;
// 	}
// 	return; 
// }
// mFile.Close();
//iSub��0��ʼ��0�ǵ�һ���ַ�����1�ڶ���
//����Ӣ�Ļ�ϵĳ���ò��������
//����0��û�ҵ�
//����1���ɹ�
/*BOOL CTXT::Split(CString source, CString& dest, CString division, UINT iSub)
{
	//dest="";
	int l_ipos = 0;
	int l_pre_ipos = 0;
	int l_ifind=0;
	//CString l_cstr_tmp;
	while( -1 != l_ipos )
	{
		l_pre_ipos = l_ipos;
		l_ipos = source.Find(division,(l_ipos+1));
		if (l_ifind==iSub)
		{
			break;
		}
		l_ifind++;
		//dest.Add(source.Mid(l_pre_ipos,(l_ipos-l_pre_ipos)));
	}
	if (-1 == l_ipos)
	{
		return 0;
	}
	dest=source.Mid(l_pre_ipos,(l_ipos-l_pre_ipos));
	return 1;
}*/

//���ַ��ָ����ȡ��Ҫ���ַ���
//iSub��0��ʼ��0�ǵ�һ���ַ�����1�ڶ���
//����0��û�ҵ�
//����1���ɹ�
BOOL CTXT::Split(char* source, char*& dest, char division, UINT iSub)
{
	//dest="";
	char* l_tmpchar;
// 	int l_ipos = 0;
// 	int l_pre_ipos = 0;
	int l_ifind=0;
	int l_ilen=strlen(source);
	l_tmpchar=new char[l_ilen];
	memset(l_tmpchar,0,l_ilen);
	//CString l_cstr_tmp;
	int j=0;

	for(int i=0;i<l_ilen;i++ )
	{
		if(source[i]==division) 
		{
			l_ifind++;
			continue;
		}

		if (l_ifind==iSub)
		{
			l_tmpchar[j]=source[i];
			j++;
		}
		
	}

	l_ilen=strlen(l_tmpchar);

	if (0 == l_ilen)
	{
		return 0;
	}

	memset(dest,0,l_ilen+1);

	for (int i=0;i<l_ilen;i++)
	{
		dest[i]=l_tmpchar[i];
	}

	delete[] l_tmpchar;
	l_tmpchar=NULL;
	return 1;
	
}
//del = 1,����Ŀ¼ɾ��
void  CTXT::DeleteDir(CString str,bool del)
{
	CFileFind finder; //�ļ�������
	CString strdel,strdir;//strdir:Ҫɾ����Ŀ¼��strdel��Ҫɾ�����ļ�
	strdir=str+_T("\\*.*");//ɾ���ļ��У���Ҫ����ļ���,����·��,ע���"\\"
	BOOL b_finded=(BOOL)finder.FindFile(strdir); 
	while(b_finded) 
	{ 
		b_finded=(BOOL)finder.FindNextFile(); 
		if (finder.IsDots())  continue;//�ҵ����ǵ�ǰĿ¼���ϼ�Ŀ¼������
		strdel=finder.GetFileName(); //��ȡ�ҵ����ļ���
		if(finder.IsDirectory())   //������ļ���
		{
			strdel=str + "\\" + strdel;//����·��,ע���"\\"
			DeleteDir(strdel,1); //�ݹ�ɾ���ļ���
		}
		else //�����ļ���
		{
			strdel=str + "\\" + strdel;
			if(finder.IsReadOnly())//���ֻ������
			{
				SetFileAttributes(strdel,GetFileAttributes(strdel)&(~FILE_ATTRIBUTE_READONLY));
			}
			DeleteFile(strdel); //ɾ���ļ�(API)
		}
	}
	finder.Close();
	if (del)
	{
		RemoveDirectory(str); //ɾ���ļ���(API)
	}
	
}

UINT CTXT::CountNum(CString szDir)
{
	CFileFind ff;
	szDir+=_T("\\*.*");
	UINT ncount =0;
	BOOL res = ff.FindFile(szDir);
	while(res)
	{
		res = ff.FindNextFile();
		if (ff.IsDots())
			continue;
		if(ff.IsDirectory())
			continue;
		++ncount;
		
	}
	return ncount;
}