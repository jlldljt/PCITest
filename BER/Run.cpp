// Run.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BER.h"
#include "Run.h"
#include "afxdialogex.h"

int gnn=10;
int *gnnum = &gnn;
// CRun �Ի���

IMPLEMENT_DYNAMIC(CRun, CDialogEx)

	CRun::CRun(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRun::IDD, pParent)
{
	g_dlgRun=this;
	m_csvCard = NULL;
	m_csvAllCard = NULL;
}

CRun::~CRun()
{
	mDCMem->SelectObject(mBMPOld);
	mBMPMem->DeleteObject();
	mDCMem->DeleteDC();
}

CRun dlgRun;
CRun *g_dlgRun=NULL;

void CRun::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SORT, m_listSort);
	DDX_Control(pDX, IDC_COMBO_SORT1, m_cmb_sort1);
	DDX_Control(pDX, IDC_COMBO_SORT2, m_cmb_sort2);
}


BEGIN_MESSAGE_MAP(CRun, CDialogEx)
	//	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SORTSET, &CRun::OnBnClickedBtnSortset)
	ON_CBN_SELCHANGE(IDC_COMBO_SORT1, &CRun::OnCbnSelchangeComboSort1)
	ON_CBN_SELCHANGE(IDC_COMBO_SORT2, &CRun::OnCbnSelchangeComboSort2)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RAD_SQU, &CRun::OnBnClickedRadSqu)
	ON_BN_CLICKED(IDC_RAD_CIR, &CRun::OnBnClickedRadCir)
	ON_BN_CLICKED(IDC_BTN_STANDARD, &CRun::OnBnClickedBtnStandard)
	ON_STN_CLICKED(IDC_PIC_SHOW, &CRun::OnStnClickedPicShow)
	ON_BN_CLICKED(IDC_BTN_RUN, &CRun::OnBnClickedBtnRun)
	ON_BN_CLICKED(IDC_CHK_PN, &CRun::OnBnClickedChkPn)
	ON_BN_CLICKED(IDC_BTN_STAFT, &CRun::OnBnClickedBtnStaft)
	ON_BN_CLICKED(IDC_BTN_TESTSPLIT, &CRun::OnBnClickedBtnTestsplit)
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CRun::OnBnClickedBtnPause)
	ON_BN_CLICKED(IDC_BTN_PMOVE, &CRun::OnBnClickedBtnPmove)
	ON_BN_CLICKED(IDC_BTN_ENDFT, &CRun::OnBnClickedBtnEndft)
	ON_BN_CLICKED(IDC_CHK_SHAKE, &CRun::OnBnClickedChkShake)
	ON_BN_CLICKED(IDC_BTN_STDCHECK, &CRun::OnBnClickedBtnStdcheck)
	ON_BN_CLICKED(IDC_BTN_CPK, &CRun::OnBnClickedBtnCpk)
	ON_BN_CLICKED(IDC_BTN_PRINT, &CRun::OnBnClickedBtnPrint)
END_MESSAGE_MAP()


// CRun ��Ϣ�������




void CRun::OnBnClickedBtnSortset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�ж�Ԥ��ֵ�ȷ����Ƿ���0~60֮��
	if(((GetDlgItemInt(IDC_EDT_MIDDEG,0,1)/10000<60?0:1)+(GetDlgItemInt(IDC_EDT_MIDDEG,0,1)/10000>=0?0:1)+
		(GetDlgItemInt(IDC_EDT_MIDDEG,0,1)%10000/100<60?0:1)+(GetDlgItemInt(IDC_EDT_MIDDEG,0,1)%10000/100>=0?0:1)+
		(GetDlgItemInt(IDC_EDT_MIDDEG,0,1)%100<60?0:1)+(GetDlgItemInt(IDC_EDT_MIDDEG,0,1)%100>=0?0:1))!=0)
	{
		AfxMessageBox(_T("��������"));
		return;
	}
	if(((GetDlgItemInt(IDC_EDT_SORTDEG,0,1)/100<60?0:1)+(GetDlgItemInt(IDC_EDT_SORTDEG,0,1)/100>=0?0:1)+
		(GetDlgItemInt(IDC_EDT_SORTDEG,0,1)%100<60?0:1)+(GetDlgItemInt(IDC_EDT_SORTDEG,0,1)%100>=0?0:1))!=0)
	{
		AfxMessageBox(_T("�ֵ�ֵ����"));
		return;
	}
	if(5!=gstuSort.sortsel2)//��������޶���׷���ж�
	{
		if(
			((GetDlgItemInt(IDC_EDT_MINDEG,0,1)/10000<60?0:1)+(GetDlgItemInt(IDC_EDT_MINDEG,0,1)/10000>=0?0:1)+
			(GetDlgItemInt(IDC_EDT_MINDEG,0,1)%10000/100<60?0:1)+(GetDlgItemInt(IDC_EDT_MINDEG,0,1)%10000/100>=0?0:1)+
			(GetDlgItemInt(IDC_EDT_MINDEG,0,1)%100<60?0:1)+(GetDlgItemInt(IDC_EDT_MINDEG,0,1)%100>=0?0:1)+
			(GetDlgItemInt(IDC_EDT_MAXDEG,0,1)/10000<60?0:1)+(GetDlgItemInt(IDC_EDT_MAXDEG,0,1)/10000>=0?0:1)+
			(GetDlgItemInt(IDC_EDT_MAXDEG,0,1)%10000/100<60?0:1)+(GetDlgItemInt(IDC_EDT_MAXDEG,0,1)%10000/100>=0?0:1)+
			(GetDlgItemInt(IDC_EDT_MAXDEG,0,1)%100<60?0:1)+(GetDlgItemInt(IDC_EDT_MAXDEG,0,1)%100>=0?0:1))
			!=0
			)
		{
			AfxMessageBox(_T("�޶�ֵ����"));
			return;
		}
	}
	//��Ч�ǲ�������
	if (((GetDlgItemInt(IDC_EDT_PHI0, 0, 1) / 10000 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_EDT_PHI0, 0, 1) / 10000 >= 0 ? 0 : 1) +
		(GetDlgItemInt(IDC_EDT_PHI0, 0, 1) % 10000 / 100 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_EDT_PHI0, 0, 1) % 10000 / 100 >= 0 ? 0 : 1) +
		(GetDlgItemInt(IDC_EDT_PHI0, 0, 1) % 100 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_EDT_PHI0, 0, 1) % 100 >= 0 ? 0 : 1)) != 0)
	{
		AfxMessageBox(_T("��Ч�ǲ���phi0����"));
		return;
	}
		//�鿴�ֵ��Ƕ��Ƿ�����нǽǶ�
	int tmpCut = USER_TO_SEC(GetDlgItemInt(IDC_EDT_CUTDEG, 0, 1));
	int tmpSort = USER_TO_SEC(GetDlgItemInt(IDC_EDT_SORTDEG, 0, 1));
	if (tmpSort>tmpCut || tmpCut%tmpSort)
	{
		AfxMessageBox(_T("�ֵ��Ƕ����нǽǶȹ�ϵ���ԣ��ֵ���С�ڵ����нǣ����н��ܱ��ֵ�������"));
		return;
	}


	//��Ԥ��ֵ���浽ȫ�ֱ���

	//�鿴�����Ƿ�ı�
	int edt_value = USER_TO_SEC(GetDlgItemInt(IDC_EDT_MIDDEG,0,1));
	//int ini_value = USER_TO_SEC(GetPrivateProfileInt(_T("�ֵ��趨"),_T("���ĽǶȣ���"),0,gstuPathInf.csPathIni)*10000+
	//	GetPrivateProfileInt(_T("�ֵ��趨"),_T("���ĽǶȣ���"),0,gstuPathInf.csPathIni)*100+
	//	GetPrivateProfileInt(_T("�ֵ��趨"),_T("���ĽǶȣ���"),0,gstuPathInf.csPathIni)); 
	int ini_value = USER_TO_SEC(GetPrivateProfileInt(_T("�Ա��趨"),_T("��һ�ζԱ����ĽǶ�"),0,gstuPathInf.csPathIni));
	
	if(edt_value != ini_value)
	{
		gstuSort.needCheck = 1;
		AfxMessageBox(_T("���ϴ����ĽǶȲ�һ�£���Ҫ�Ա�"));
	}
	else
	{
		gstuSort.needCheck = 0;
		AfxMessageBox(_T("���ϴ����ĽǶ�һ�£����öԱ�"));
	}

	int usl = edt_value-tmpSort/2+tmpCut;
	int lsl = edt_value-tmpSort/2-tmpCut;

	CString tmpStr;
	tmpStr.Format(_T("cpk usl : %d lsl : %d"),SEC_TO_USER(usl),SEC_TO_USER(lsl));
	AfxMessageBox(tmpStr);

	gstuSort.centerangle=GetDlgItemInt(IDC_EDT_MIDDEG,0,1)/10000*3600+GetDlgItemInt(IDC_EDT_MIDDEG,0,1)%10000/100*60+GetDlgItemInt(IDC_EDT_MIDDEG,0,1)%100;
	gstuSort.sortvalue=GetDlgItemInt(IDC_EDT_SORTDEG,0,1)/100*60+GetDlgItemInt(IDC_EDT_SORTDEG,0,1)%100;
	gstuSort.cutvalue=GetDlgItemInt(IDC_EDT_CUTDEG,0,1)%100;
	if(5!=gstuSort.sortsel2)//��������޶���׷���ж�
	{
		gstuSort.eleclow=GetDlgItemInt(IDC_EDT_MINDEG,0,1)/10000*3600+GetDlgItemInt(IDC_EDT_MINDEG,0,1)%10000/100*60+GetDlgItemInt(IDC_EDT_MINDEG,0,1)%100;
		gstuSort.elechigh=GetDlgItemInt(IDC_EDT_MAXDEG,0,1)/10000*3600+GetDlgItemInt(IDC_EDT_MAXDEG,0,1)%10000/100*60+GetDlgItemInt(IDC_EDT_MAXDEG,0,1)%100;
	}

	gstuSort.phi0 = GetDlgItemInt(IDC_EDT_PHI0, 0, 1) / 10000 * 3600 + GetDlgItemInt(IDC_EDT_PHI0, 0, 1) % 10000 / 100 * 60 + GetDlgItemInt(IDC_EDT_PHI0, 0, 1) % 100;
	gstuSort.factor = GetDlgItemInt(IDC_EDT_FACTOR, 0, 1);

	//���浽ini
	CString strValue = _T("");


	CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
	BOOL ifFind = findini.FindFile(gstuPathInf.csPathIni);  
	if( !ifFind )  
	{  
		AfxMessageBox(_T("�������ļ�"));
	}  

  gstuSort.needNewProcess = 0;
 
  TCHAR tmpbuff[256];
  CString tmpstr;
  GetPrivateProfileString(_T("�ֵ��趨"), _T("�ƻ���"), NULL, tmpbuff, 80, gstuPathInf.csPathIni);
  tmpstr = tmpbuff;
	GetDlgItemText(IDC_EDIT_PLANNED, strValue);
  if (tmpstr != strValue)
  {
    WritePrivateProfileString(_T("�ֵ��趨"), _T("�ƻ���"), strValue, gstuPathInf.csPathIni);
    gstuSort.needNewProcess++;
  }
  GetPrivateProfileString(_T("�ֵ��趨"), _T("���̿�"), NULL, tmpbuff, 80, gstuPathInf.csPathIni);
  tmpstr = tmpbuff;
	GetDlgItemText(IDC_EDIT_CARD, strValue);
  if (tmpstr != strValue)
  {
    WritePrivateProfileString(_T("�ֵ��趨"), _T("���̿�"), strValue, gstuPathInf.csPathIni);
    gstuSort.needNewProcess++;
  }

	strValue.Format(_T("%d"),gstuSort.sortsel1);
	WritePrivateProfileString(_T("�ֵ��趨"),_T("���ǣ����"),strValue,gstuPathInf.csPathIni); 
	if(5!=gstuSort.sortsel2)//��������޶���׷���ж�
	{
		strValue.Format(_T("%d"),gstuSort.sortsel2);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("�޶��ǣ����"),strValue,gstuPathInf.csPathIni);

		strValue.Format(_T("%d"),GetDlgItemInt(IDC_EDT_MINDEG,0,1)/10000);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("��ʼֵ����"),strValue,gstuPathInf.csPathIni);
		strValue.Format(_T("%d"),GetDlgItemInt(IDC_EDT_MINDEG,0,1)%10000/100);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("��ʼֵ����"),strValue,gstuPathInf.csPathIni);
		strValue.Format(_T("%d"),GetDlgItemInt(IDC_EDT_MINDEG,0,1)%100);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("��ʼֵ����"),strValue,gstuPathInf.csPathIni);
		strValue.Format(_T("%d"),GetDlgItemInt(IDC_EDT_MAXDEG,0,1)/10000);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("����ֵ����"),strValue,gstuPathInf.csPathIni);
		strValue.Format(_T("%d"),GetDlgItemInt(IDC_EDT_MAXDEG,0,1)%10000/100);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("����ֵ����"),strValue,gstuPathInf.csPathIni);
		strValue.Format(_T("%d"),GetDlgItemInt(IDC_EDT_MAXDEG,0,1)%100);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("����ֵ����"),strValue,gstuPathInf.csPathIni);
	}

	strValue.Format(_T("%d"),GetDlgItemInt(IDC_EDT_MIDDEG,0,1)/10000);
	WritePrivateProfileString(_T("�ֵ��趨"),_T("���ĽǶȣ���"),strValue,gstuPathInf.csPathIni);  
	strValue.Format(_T("%d"),GetDlgItemInt(IDC_EDT_MIDDEG,0,1)%10000/100);
	WritePrivateProfileString(_T("�ֵ��趨"),_T("���ĽǶȣ���"),strValue,gstuPathInf.csPathIni);
	strValue.Format(_T("%d"),GetDlgItemInt(IDC_EDT_MIDDEG,0,1)%100);
	WritePrivateProfileString(_T("�ֵ��趨"),_T("���ĽǶȣ���"),strValue,gstuPathInf.csPathIni);  

	strValue.Format(_T("%d"),GetDlgItemInt(IDC_EDT_SORTDEG,0,1)/100);
	WritePrivateProfileString(_T("�ֵ��趨"),_T("�ֵ�ֵ����"),strValue,gstuPathInf.csPathIni);
	strValue.Format(_T("%d"),GetDlgItemInt(IDC_EDT_SORTDEG,0,1)%100);
	WritePrivateProfileString(_T("�ֵ��趨"),_T("�ֵ�ֵ����"),strValue,gstuPathInf.csPathIni);
strValue.Format(_T("%d"),GetDlgItemInt(IDC_EDT_CUTDEG,0,1)%100);
	WritePrivateProfileString(_T("�ֵ��趨"),_T("�н�ֵ����"),strValue,gstuPathInf.csPathIni);

	strValue.Format(_T("%d"), GetDlgItemInt(IDC_EDT_PHI0, 0, 1) / 10000);
	WritePrivateProfileString(_T("��Ч���趨"), _T("phi0����"), strValue, gstuPathInf.csPathIni);
	strValue.Format(_T("%d"), GetDlgItemInt(IDC_EDT_PHI0, 0, 1) % 10000 / 100);
	WritePrivateProfileString(_T("��Ч���趨"), _T("phi0����"), strValue, gstuPathInf.csPathIni);
	strValue.Format(_T("%d"), GetDlgItemInt(IDC_EDT_PHI0, 0, 1) % 100);
	WritePrivateProfileString(_T("��Ч���趨"), _T("phi0����"), strValue, gstuPathInf.csPathIni);

	strValue.Format(_T("%d"), GetDlgItemInt(IDC_EDT_FACTOR, 0, 1));
	WritePrivateProfileString(_T("��Ч���趨"), _T("factor"), strValue, gstuPathInf.csPathIni);
	//��տؼ���ȫ�ֱ���
	CString sortValue,strTemps;
	for(int i=0;i<24;i++)
	{
		sortValue="";
		strTemps.Format(_T("%02d��"),(gstuSort.centerangle+gstuSort.sortvalue*(i-12))/3600);
		sortValue+=strTemps;
		strTemps.Format(_T("%02d\'"),(gstuSort.centerangle+gstuSort.sortvalue*(i-12))%3600/60);
		sortValue+=strTemps;
		strTemps.Format(_T("%02d\""),(gstuSort.centerangle+gstuSort.sortvalue*(i-12))%60);
		sortValue+=strTemps;
		m_listSort.SetItemText(i, 1, sortValue);   
		m_listSort.SetItemText(i, 2, _T("0"));   
		gstuSort.sortnum[i]=0;
	}
	m_listSort.SetItemText(24, 2, _T("0"));gstuSort.sortnum[24]=0;
	m_listSort.SetItemText(25, 2, _T("0"));gstuSort.sortnum[25]=0;
	m_listSort.SetItemText(26, 2, _T("0"));gstuSort.sortnum[26]=0;
	m_listSort.SetItemText(27, 2, _T("0"));gstuSort.sortnum[27]=0;
	m_listSort.SetItemText(28, 2, _T("0"));gstuSort.sortnum[28]=0;
	m_listSort.SetItemText(29, 2, _T("0"));gstuSort.sortnum[29]=0;
	m_listSort.SetItemText(30, 2, _T("0"));gstuSort.sortnum[30]=0;
	for(int i=0;i<5;i++)
	{
		gstuSort.sortavg[i]=0;
		gstuSort.sortstd[i]=0;
	}
	gstuSort.sortsn=0;
	gstuSort.sortsum=0;
}


void CRun::OnCbnSelchangeComboSort1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	gstuSort.sortsel1=m_cmb_sort1.GetCurSel();
}


void CRun::OnCbnSelchangeComboSort2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	gstuSort.sortsel2=m_cmb_sort2.GetCurSel();
}


void CRun::OnPaint()
{
	CRect rect,rcUpdate;
	GetUpdateRect(&rcUpdate);

	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CRect rcWindow;
	GetDlgItem(IDC_PIC_SHOW)->GetWindowRect(rcWindow);
	ScreenToClient(rcWindow);

	if (rcWindow.left<rcUpdate.right && rcWindow.right>rcUpdate.left &&
		rcWindow.top<rcUpdate.bottom && rcWindow.bottom>rcUpdate.top )
	{
		GetDlgItem(IDC_PIC_SHOW)->GetClientRect(&rect);
		GetDlgItem(IDC_PIC_SHOW)->RedrawWindow(&rect,NULL,RDW_INVALIDATE|RDW_ALLCHILDREN);//RDW_VALIDATE|RDW_UPDATENOW
	}
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	/*	CWnd * pWnd = g_dlgRun->GetDlgItem(IDC_PIC_SORT);
	CDC* pDC = pWnd->GetDC();
	CRect rect;
	GetDlgItem(IDC_PIC_SORT)->GetClientRect(&rect);
	//GetDlgItem(IDC_PIC_SORT)


	//ClientToScreen(&rect);
	GetDlgItem(IDC_SHOW_VAL)->SetWindowPos(pWnd,rect.left+14,rect.top+14,rect.right/3+14,rect.bottom/3+14,SWP_SHOWWINDOW);// MoveWindow(,1);
	pDC->RoundRect(rect.left,rect.top+rect.Height()/2,rect.right-rect.Width()/2,rect.bottom,0,0);*/
	//GetDlgItem(IDC_SHOW_VAL)->SetWindowPos(pWnd,rect.left+10,rect.top+rect.Height()/2+10,rect.right-rect.Width()/2-10,rect.bottom-10,SWP_SHOWWINDOW);// MoveWindow(,1);
	/*	CImage image;      //����һ��CImage��������͸����ͼ
	CString str=_T(".\\BER.png");      //����һ��CString�������ڴ���ļ����ַ���
	//str="onion.png";              //���ַ�����ֵΪ�ļ���

	image.Load(str);                    //��image������ͼ���ļ�

	//image.Draw(pDC->m_hDC,rect);    //����Draw����͸����ͼ
	image.TransparentBlt(pDC->m_hDC, 120, 130, 85, 113,CLR_INVALID );//����TransparentBlt����͸����ͼ
	image.Destroy();*/
	//pWnd->ReleaseDC(pDC);
} 


void CRun::OnBnClickedRadSqu()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//((CButton *)GetDlgItem(IDC_RAD_SQU))->SetCheck(TRUE);//ѡ��
	//((CButton *)GetDlgItem(IDC_RAD_CIR))->SetCheck(FALSE);//��ѡ��
	//((CButton *)GetDlgItem(IDC_RAD_CIR))->GetCheck();
	gstuRcgInfo.bIsCir=0;
}


void CRun::OnBnClickedRadCir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	gstuRcgInfo.bIsCir=1;
}


void CRun::OnBnClickedBtnStandard()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
#ifndef DEBUG_MODE
	//if (glCamID>=0)
	//{
	//	NET_SNAPCFG struSnapCfg;
	//	gstuRun.bOverCam=0;

	//	NET_ManualSnap(glCamID, &struSnapCfg, &gstuCamResult);
	//	while(!gstuRun.bOverCam)//�ȴ��ɼ�ͼ�����
	//		Sleep(1);
	if(gnChannel >=0)
	{
		char l_AnsiStr[MAX_PATH];
		WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, gstuPathInf.csPathExe + _T("\\PIC\\ԭͼ0.bmp"), -1, l_AnsiStr, sizeof(l_AnsiStr), NULL, NULL);

		int nWidth, nHeight;
		int nBitCount;
		int nBitsPerSample;
		unsigned char *pData = gclsCamera.SnapEx(gnChannel, &nWidth, &nHeight, &nBitCount, &nBitsPerSample, l_AnsiStr, NULL);

#endif
		CString m_bmp_file=gstuPathInf.csPathExe + _T("\\PIC\\ԭͼ0.bmp");
		CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
		BOOL ifFind = findini.FindFile(m_bmp_file);  
		if( !ifFind )  
			return;
		CBitmap m_bmp;//�������Ա
		BITMAP bm;//���λͼ��Ϣ�Ľṹ
		HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,m_bmp_file,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//����bitmapָ��
		m_bmp.Attach(hBitmap1);//���������cbitmap����
		m_bmp.GetBitmap(&bm);
		CWnd * pWnd = GetDlgItem(IDC_PIC_SHOW);
		CDC* pDC = pWnd->GetDC();
		CRect rect;
		GetDlgItem(IDC_PIC_SHOW)->GetClientRect(&rect);
		CDC memDC;        //����һ���豸
		CClientDC dc1(this);      //��ȡ�ͻ�
		memDC.CreateCompatibleDC( &dc1 );
		memDC.SelectObject( m_bmp );  //Ϊ�豸ѡ�����
		mDCMem->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);
		memDC.DeleteDC();
		m_bmp.DeleteObject();	
		DeleteObject(hBitmap1);//�ǵ�ɾ��	
		CPen PenRed;
		CGdiObject *o = NULL;
		PenRed.CreatePen(PS_SOLID,1,RGB(255,0,0));//����һ֧��ɫ�Ļ���.
		mDCMem->SelectObject(PenRed);//ѡ�л���.
		char chPath[MAX_PATH];
		WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_bmp_file,-1,chPath,sizeof(chPath),NULL,NULL); 
		//
		LARGE_INTEGER l_lgint_start, l_lgint_end;
		LARGE_INTEGER l_lgint_freq;
		QueryPerformanceFrequency(&l_lgint_freq);  
		QueryPerformanceCounter(&l_lgint_start);
		//
		gclsImgRcg.RCGBMP(mDCMem,rect,chPath,gstuRcgInfo.nPToL,gstuRcgInfo.nDefectPToL,gstuRcgInfo.nThreshold,0,gstuRcgInfo.bIsCir,gstuRcgInfo.bThrdAuto,gstuRcgInfo.bDelNoise);
		//
		QueryPerformanceCounter(&l_lgint_end);
		double dTmpRT=double(l_lgint_end.QuadPart-l_lgint_start.QuadPart)/double(l_lgint_freq.QuadPart);	
		CString csTmp;
		csTmp.Format(_T("%.2f��"),dTmpRT);
		SetDlgItemText(IDC_STAT_RUN,csTmp);
		//
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);
		pWnd->ReleaseDC(pDC);
#ifndef DEBUG_MODE
	}
	else
		g_dlgRun->SetDlgItemText(IDC_STAT_RUN,_T("���δ����"));
#endif


}


void CRun::OnStnClickedPicShow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (0==gclsImgRcg.g_stu_square.nN)
	{
		return;
	}
	int xt,yt;
	POINT pt;//�����
	GetCursorPos(&pt);//ȡ�õ�ǰ����
	CRect lRect;
	GetDlgItem(IDC_PIC_SHOW)->GetWindowRect(&lRect);
	int temp_x=pt.x-lRect.left;
	int temp_y=pt.y-lRect.top;
	xt=(int)(temp_x*gclsImgRcg.g_stu_square.nBMPW/(lRect.Width()));//����������Ӧ��ͼ���ϵ�x��
	yt=(int)(temp_y*gclsImgRcg.g_stu_square.nBMPH/(lRect.Height()));//����������Ӧ��ͼ���ϵ�y��
	//if(!m_RadioCircle && m_RadioSquare)
	int X=gstuRcgInfo.g_factor[0][0]*(double)xt+gstuRcgInfo.g_factor[0][1]*(double)yt+gstuRcgInfo.g_factor[0][2];
	int Y=gstuRcgInfo.g_factor[1][0]*(double)xt+gstuRcgInfo.g_factor[1][1]*(double)yt+gstuRcgInfo.g_factor[1][2];

	SetDlgItemInt(IDC_P_X,X,1);

	SetDlgItemInt(IDC_P_Y,Y,1);


	//{
	//if(avg1.empty()){AfxMessageBox(_T("û�ж�λ���꣬�붨λ"));return;}

	int temp=0;
	for(int i=0;i<gclsImgRcg.g_stu_square.nN;i++)
	{

		if(sqrt(pow(double(gclsImgRcg.g_stu_square.pnZPX[i]-xt),2)+pow(double(gclsImgRcg.g_stu_square.pnZPY[i]-yt),2))<((gclsImgRcg.g_stu_square.pnWth[i])>>1)) //���λ����Ƭ�Ĵ����������С��s_sort.allowable_devision����ô����Ҫ���Ե��㲻׼ʱ���õ���
		{
			//DrawPicToHDC(line_img, IDC_PREVIEW);//��ͼƬ�ؼ�����ʾ
			CWnd * pWnd = GetDlgItem(IDC_PIC_SHOW);
			CDC* pDC = pWnd->GetDC();
			CRect rect;
			GetDlgItem(IDC_PIC_SHOW)->GetClientRect(&rect);
			CPen PenRed;
			PenRed.CreatePen(PS_SOLID,1,RGB(255,0,0));//����һ֧��ɫ�Ļ���.
			pDC->SelectObject(PenRed);//ѡ�л���.
			pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);
			if(gstuRcgInfo.bIsCir)
			{
				CString csTmp;
				csTmp.Format(_T("�뾶%d"),gclsImgRcg.g_stu_square.pnWth[i]);
				//AfxMessageBox(csTmp);
				GetDlgItem(IDC_STAT_RUN)->SetWindowText(csTmp);
				pDC->Ellipse
					(gclsImgRcg.g_stu_square.pnZPX[i]*rect.Width()/gclsImgRcg.g_stu_square.nBMPW - gclsImgRcg.g_stu_square.pnWth[i]*rect.Width()/gclsImgRcg.g_stu_square.nBMPW
					,gclsImgRcg.g_stu_square.pnZPY[i]*rect.Height()/gclsImgRcg.g_stu_square.nBMPH - gclsImgRcg.g_stu_square.pnWth[i]*rect.Width()/gclsImgRcg.g_stu_square.nBMPW
					,gclsImgRcg.g_stu_square.pnZPX[i]*rect.Width()/gclsImgRcg.g_stu_square.nBMPW + gclsImgRcg.g_stu_square.pnWth[i]*rect.Width()/gclsImgRcg.g_stu_square.nBMPW
					,gclsImgRcg.g_stu_square.pnZPY[i]*rect.Height()/gclsImgRcg.g_stu_square.nBMPH + gclsImgRcg.g_stu_square.pnWth[i]*rect.Width()/gclsImgRcg.g_stu_square.nBMPW
					);
				//���浽��������
				gclsImgRcg.stuRef.Rad=gclsImgRcg.g_stu_square.pnWth[i];
			}
			else
			{
				pt.x=gclsImgRcg.g_stu_square.pnPX[i][3]*rect.Width()/gclsImgRcg.g_stu_square.nBMPW;
				pt.y=gclsImgRcg.g_stu_square.pnPY[i][3]*rect.Height()/gclsImgRcg.g_stu_square.nBMPH;
				pDC->MoveTo(pt);
				CString csTmp;
				csTmp.Format(_T("��%d��%dX����%d"),gclsImgRcg.g_stu_square.pnLen[i],gclsImgRcg.g_stu_square.pnWth[i],gclsImgRcg.g_stu_square.bPN[i]);
				GetDlgItem(IDC_STAT_RUN)->SetWindowText(csTmp);
				//AfxMessageBox(csTmp);
				for(int j=0;j<4;j++)
				{
					pDC->LineTo(gclsImgRcg.g_stu_square.pnPX[i][j]*rect.Width()/gclsImgRcg.g_stu_square.nBMPW,
						gclsImgRcg.g_stu_square.pnPY[i][j]*rect.Height()/gclsImgRcg.g_stu_square.nBMPH);
				}
				gclsImgRcg.stuRef.Len=gclsImgRcg.g_stu_square.pnLen[i];
				gclsImgRcg.stuRef.Wth=gclsImgRcg.g_stu_square.pnWth[i];
				gstuRcgInfo.nPN=gclsImgRcg.g_stu_square.bPN[i];


			}
			int status=AfxMessageBox(_T("�Ƿ�ѡ��"),MB_OKCANCEL);
			if(status==1)//��ȷ���˳�
			{
				gclsImgRcg.stuRef.Dev=gstuRcgInfo.nAllowDefect;
				gstuRun.bIsSelFt = 1;
			}

			pWnd->ReleaseDC(pDC);
			break;
		}
	}

	return;
	//}
}


BOOL CRun::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CWnd * pWnd = GetDlgItem(IDC_PIC_SHOW);
	CDC* pDC = pWnd->GetDC();
	CRect rect;
	GetDlgItem(IDC_PIC_SHOW)->GetClientRect(&rect);
	mDCMem= new CDC;
	mBMPMem=new CBitmap;
	mDCMem->CreateCompatibleDC(pDC);
	mBMPMem->CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	mBMPOld=mDCMem->SelectObject(mBMPMem);//�������Ͼ䣬��mdcmen������Ч
	mDCMem->SetStretchBltMode (HALFTONE);
	SetBrushOrgEx(mDCMem->m_hDC, 0, 0, NULL);
	pWnd->ReleaseDC(pDC);

	gTrdCam = NULL;


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CRun::OnBnClickedBtnRun()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!gstuRun.bIsSelFt)
	{
		AfxMessageBox(_T("û��ѡ������Ƭ"));
		return;
	}
	int status;
	switch (gstuRun.unStatRun)
	{
	case 0://���ֹͣ״̬-������
		if(!g_dlgDevice->EasyPosSet())
			break;
		g_dlgDevice->EFGStaSel(char(gstuRun.nEfgStaSel));
		CamView();
		//��λ��������ʼ
		if (FALSE == RunInit())
			return;

		//stm��ʼ
		gstuRun.chStmCmd &= ~(1<<2);
		g_dlgDevice->StmSE(1);
		SetDlgItemText(IDC_STAT_RUN,_T("�ȴ���λ����ʼ�����"));
		gstuRun.bRunStatChg = 1; //ָʾontime��ʱ����
		GetDlgItem(IDC_BTN_RUN)->EnableWindow(0);
		// #ifndef DEBUG_COM
		// 		while(! (gstuRun.chStmCmd & (1<<2)))//�ȴ���λ����ʼ�����
		// 		{
		// 			Sleep(1);
		// 		}
		// #endif
		// 		SetDlgItemText(IDC_STAT_RUN,_T("��λ����ʼ�����"));
		// 		gstuRun.unStatRun=2;
		break;
		//case 1:
	case 2://�������״̬-���˳�
		status=AfxMessageBox(_T("ȷ�����˳�\r\nȡ������ͣ"),MB_OKCANCEL);
		if(status==1)//��ȷ���˳�
		{
			CamView();
			gstuRun.bTrdExit=1;//ָʾ�߳�ֹͣ
			gstuRun.bExit=1;//ָʾϵͳ��ʱ����ֹͣ����
			gstuRun.chStmCmd &= ~(1<<2);
			g_dlgDevice->StmSE(0);
			SetDlgItemText(IDC_STAT_RUN,_T("�ȴ���λ��ֹͣ"));
			gstuRun.bRunStatChg = 1;//ָʾontime��ʱ����
			GetDlgItem(IDC_BTN_RUN)->EnableWindow(0);
			// #ifndef DEBUG_COM
			// 			while(! (gstuRun.chStmCmd & (1<<3)))
			// 			{
			// 				Sleep(1);
			// 			}
			// #endif
			// 			SetDlgItemText(IDC_STAT_RUN,_T("��λ��ֹͣ"));
		}
		else
		{

		}

		break;
	default:
		break;
	}
	Sleep(200);
}

BOOL CRun::RunInit()
{
	gstuRefresh.alarm = 0;
	gclsCom.stuInf.bIsRun=1;
	gclsImgRcg.g_stu_square.bJudgeFeature=1;
	memset(gclsCom.stuInf.pchBufWTmp, 0, gclsCom.stuInf.unLenWrt+1);
	memset(gclsCom.stuInf.pchBufRTmp, 0, gclsCom.stuInf.unLenRcv+1);
	gstuRun.bTrdExit=0;									
	gstuRun.unTrdExitNum=0;
	gstuRun. bTrdPause=0;								
	gstuRun. unStatRun=0;								
	gstuRun. bOverRcg = 1;
	gstuRun. bOverCam = 1;
	//memset(gstuRun. chStmCmd,0,10);	
	gstuRun. chStmCmd = 0;
	gstuRun. unCntResend=0;								
	gstuRun. dRT=0;									
	gstuRun. bExit=0;			

	memset(gstuRun.unSort,0,3);

	int status = 0;

	CString planned,process;

	GetDlgItemText(IDC_EDIT_PLANNED, planned);

	if ("" == planned)
	{
		AfxMessageBox(_T("�ƻ��ţ�"));

		return FALSE;
	}


	GetDlgItemText(IDC_EDIT_CARD, process);

	if ("" == process)
	{
		AfxMessageBox(_T("���̿��ţ�"));

		return FALSE;
	}
	// �Ա�׼
	if(gstuSort.needCheck)
	{
		CDlgCheckStd dlg;
		int ret = dlg.DoModal();
		if(ret != 101)
		{
			if(1==AfxMessageBox(_T("�Ա�׼δͨ����ȷ���˳����¶Ա꣬ȡ��ǿ��ִ��"),MB_OKCANCEL))
				return FALSE;
		}
		gstuSort.needCheck=0;
		CString str;
		GetDlgItemText(IDC_EDT_MIDDEG,str);
		WritePrivateProfileString(_T("�Ա��趨"),_T("��һ�ζԱ����ĽǶ�"),str,gstuPathInf.csPathIni);  

	}

	/*	if(g_hDevice==INVALID_HANDLE_VALUE)
	{
	AfxMessageBox(_T("ac6641 ��ʧ��"));
	return;
	}*/
	//���̿���¼��by mmy 171115
	TRY
	{
		if (!m_csvAllCard)
			m_csvAllCard = new CCSVFile(gstuPathInf.csPathExe + _T("\\data\\���̿�.csv"), CCSVFile::modeWrite);

	}
	CATCH(CFileException, e)
	{
		CString str;
		e->GetErrorMessage(str.GetBufferSetLength(256), 256);
		str.ReleaseBufferSetLength(256);
		AfxMessageBox(str);
		return FALSE;
	}
	END_CATCH

	//CString planned,process;

	//GetDlgItemText(IDC_EDIT_PLANNED, planned);

	//if ("" == planned)
	//{
	//	AfxMessageBox(_T("�ƻ��ţ�"));

	//	return FALSE;
	//}


	//GetDlgItemText(IDC_EDIT_CARD, process);

	//if ("" == process)
	//{
	//	//ȥ����� by mmy 181017
	//	//status = AfxMessageBox(_T("�Ƿ��죿"), MB_OKCANCEL);
	//	//if (status == 2)//ȡ��
	//	//{
	//	//  return FALSE;
	//	//}
	//	//else {}
	//}
	//else
	{
		// ����cpk�ƻ��š����̿�, 
    // 190327 �����ظ�����
    // 190727 �Զ��Ӻ�׺����ֹcpk���治ͳһ
    if (!g_pCpk->OpenProcessCard(planned, process, TRUE)) {}
		//{
		//	AfxMessageBox(_T("�������̿��д��󣬼���Ƿ��ظ������̿�"));
		//	return FALSE;
		//}
		//else
		//{
			//GetDlgItemText(IDC_EDIT_CARD, process);
			CProcessCard processCard;
			processCard.no = process;
			CString axis;
			GetDlgItemText(IDC_COMBO_SORT1, axis);
			processCard.axis = axis;
			processCard.center = gstuSort.centerangle;
			
			int tmpCut = USER_TO_SEC(GetDlgItemInt(IDC_EDT_CUTDEG, 0, 1));
			int tmpSort = USER_TO_SEC(GetDlgItemInt(IDC_EDT_SORTDEG, 0, 1));

			processCard.usl = processCard.center-tmpSort/2+tmpCut;
			processCard.lsl = processCard.center-tmpSort/2-tmpCut;

			CString tmpStr;
			tmpStr.Format(_T("usl : %d lsl : %d"),SEC_TO_USER(processCard.usl),SEC_TO_USER(processCard.lsl));
			AfxMessageBox(tmpStr);


			g_pCpk->AddToPlannedCsv(processCard);
		//	// g_pCpk->AddCpkToPlannedCsv(processCard);
		//}

		TRY
		{
			m_csvCard = new CCSVFile(gstuPathInf.csPathExe + _T("\\data\\") + process + _T(".csv"), CCSVFile::modeWrite);

		}
		CATCH(CFileException, e)
		{
			CString str;
			e->GetErrorMessage(str.GetBufferSetLength(256), 256);
			str.ReleaseBufferSetLength(256);
			AfxMessageBox(str);
			return FALSE;
		}
		END_CATCH
	}


	//CCSVFile csvCard(gstuPathInf.csPathExe + _T("\\data\\���̿�.csv"), CCSVFile::modeWrite);
	CStringArray arr;
	CTime time;
	time = CTime::GetCurrentTime();

	arr.Add(time.Format("%Y/%m/%d"));
	arr.Add(time.Format("%H:%M:%S"));
	arr.Add(process);
	m_csvAllCard->WriteData(arr);
	arr.RemoveAll();

	if (m_csvCard) {
		//CCSVFile csv(gstuPathInf.csPathExe + _T("\\data\\") + card + _T(".csv"), CCSVFile::modeWrite);

		/*if (0 == csv.GetLength())
		{
		arr.Add(_T("���"));
		arr.Add(_T("����Ƕ�"));
		arr.Add(_T("����Ƕ�"));
		arr.Add(_T("��Ч�Ƕ�"));
		arr.Add(_T("ԭʼ����"));
		arr.Add(_T("ԭʼ����"));
		arr.Add(_T("��λ"));
		csv.WriteData(arr);
		}

		arr.RemoveAll();
		*/
		//�ָ�
		CString str;
		//
		arr.Add(_T("��ʼ"));
		arr.Add(time.Format("%Y/%m/%d"));
		arr.Add(time.Format("%H:%M:%S"));
		m_csvCard->WriteData(arr);
		arr.RemoveAll();
		//
		arr.Add(_T("�ֵ�Ԥ��"));
		GetDlgItemText(IDC_COMBO_SORT1, str);
		if ("" == str)
		{
			AfxMessageBox(_T("����ȷ����"));
			return FALSE;
		}
		arr.Add(str);
		GetDlgItemText(IDC_EDT_MIDDEG, str);
		if ("" == str)
		{
			AfxMessageBox(_T("����ȷ����"));
			return FALSE;
		}
		arr.Add(str);
		GetDlgItemText(IDC_EDT_SORTDEG, str);
		if ("" == str)
		{
			AfxMessageBox(_T("����ȷ����"));
			return FALSE;
		}
		arr.Add(str);
		GetDlgItemText(IDC_COMBO_SORT2, str);
		if ("" == str)
		{
			AfxMessageBox(_T("����ȷ����"));
			return FALSE;
		}
		arr.Add(str);
		GetDlgItemText(IDC_EDT_MINDEG, str);
		if ("" == str)
		{
			AfxMessageBox(_T("����ȷ����"));
			return FALSE;
		}
		arr.Add(str);
		GetDlgItemText(IDC_EDT_MAXDEG, str);
		if ("" == str)
		{
			AfxMessageBox(_T("����ȷ����"));
			return FALSE;
		}
		arr.Add(str);
		GetDlgItemText(IDC_EDT_PHI0, str);
		if ("" == str)
		{
			AfxMessageBox(_T("����ȷ����"));
			return FALSE;
		}
		arr.Add(str);
		GetDlgItemText(IDC_EDT_FACTOR, str);
		if ("" == str)
		{
			AfxMessageBox(_T("����ȷ����"));
			return FALSE;
		}
		arr.Add(str);
		/* if (g_sort.islongsqu)
		{
		arr.Add(_T("����Ƭ"));
		}
		else
		{
		arr.Add(_T("��Ƭ"));
		}*/
		/*str.Format(_T("%d"), g_sort.islongsqu);
		arr.Add(_T("����Ƭ"));
		arr.Add(str);*/
		m_csvCard->WriteData(arr);
		arr.RemoveAll();
		//
		arr.Add(_T("���"));
		arr.Add(_T("����Ƕ�"));
		arr.Add(_T("����Ƕ�"));
		arr.Add(_T("��Ч�Ƕ�"));
		arr.Add(_T("ԭʼ����"));
		arr.Add(_T("ԭʼ����"));
		arr.Add(_T("��λ"));
		m_csvCard->WriteData(arr);
		arr.RemoveAll();

	}


	if (0==gstuRun.runCnt)
	{
		gstuRun.runCnt++;
	}
	else
	{
		int status=AfxMessageBox(_T("�̳�ǰһ�Σ�"),MB_OKCANCEL);
		if(status==1)//��ȷ���˳�
		{
			return TRUE;
		}
	}


	//����Ŀ¼
	gclsTxt.GetYMD(gstuRun. chTmYMD);				
	gclsTxt.GetHMS(gstuRun.chTmHMS);
	char l_char_tmp[256];
	BOOL l_bool_tmp=0;
	CFileFind l_find;
	sprintf(l_char_tmp,"\\Data\\%s\\*.*",gstuRun. chTmYMD);
	CString csTmpPath = gstuPathInf.csPathExe+l_char_tmp;
	l_bool_tmp=l_find.FindFile(csTmpPath) ;
	sprintf(l_char_tmp,"\\Data\\%s",gstuRun. chTmYMD);
	csTmpPath = gstuPathInf.csPathExe + (CString)l_char_tmp;
	//g_struct_into_txt.s_cstr_path_good=l_char_tmp;

	if (!l_bool_tmp)
	{
		if(!CreateDirectory(csTmpPath, NULL))
		{
			AfxMessageBox(_T("����Ŀ¼����ʧ�ܣ�"));
		}
	}
	gstuPathInf.csPathTxt= csTmpPath + _T("\\");
	CString csTmp;
	//gstuPathInf.csPathTxt +="\\";
	GetDlgItem(IDC_EDT_TXTNM)->GetWindowText(csTmp);
	if (csTmp!="")
	{
		gstuPathInf.csPathTxt +=csTmp+_T("_");
	}

	gstuRun.unCntStart=gclsTxt.CountNum(csTmpPath)+1;
	sprintf(l_char_tmp,"%s_%d.txt",gstuRun.chTmHMS,gstuRun.unCntStart);
	csTmp=l_char_tmp;
	gstuPathInf.csPathTxt+=csTmp;
	CFile g_file(gstuPathInf.csPathTxt,CFile::modeCreate | CFile::modeWrite|CFile::modeNoTruncate| CFile::shareDenyRead);
	g_file.Close();


	////////////////////////////////////////////////
	//������ʼ��
	gstuTrdStat._N=0;
	gstuTrdStat._RUN=0;
	gstuTrdStat._SHOW=0;
	gstuTrdStat.CHK_STOP=0;
	gstuTrdStat.DSP_STOP=0;
	gstuTrdStat.SCR_STOP=0;
	gstuTrdStat.SOT_STOP=0;
	gstuTrdStat.SYS_PAUSE=0;
	gstuTrdStat.TAK_STOP=0;
	gstuTrdStat._DEBUGEFG=0;//����
	gstuSort.sortsn=0;
	gstuSort.sortsum=0;//1~24����Ƭ��
	gstuRun.unSort[2]=29;
	gstuRun.unSort[1]=29;
	gstuRun.unSort[0]=29;
	for(int i=0;i<30;i++)//ÿ���ѷֵ�Ƭ������
	{
		gstuSort.sortnum[i]=0;
		gstuSort.alertnum[i]=1;
	}
	for(int i=0;i<5;i++)//5���ƽ�������ʼ��
	{
		gstuSort.sortavg[i]=0;
		gstuSort.sortstd[i]=0;
		gstuSort.std2[i]=0;
	}
	//
	for(int i=0;i<30;i++)
	{
		m_listSort.SetItemText(i, 2, _T("0"));   
	}


	CString _degree;
	_degree=_T("���\t����\t����\t��Ч��\tԭ����\tԭ����\t��λ\r\n");
	gclsTxt.TXTAddStr(gstuPathInf.csPathTxt,_degree);
	/////////////////////////////////////////////////
	return TRUE;
}


void CRun::OnBnClickedChkPn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_PN);
	int nStat = pChk->GetCheck();
	if (nStat)
	{
		gstuRcgInfo.bChkPN=1;
	} 
	else
	{
		gstuRcgInfo.bChkPN=0;
	}
}


void CRun::OnBnClickedBtnStaft()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*if(1)
	{
	for(int i = 0;i<5;i++)
	{
	g_dlgDevice->EFGCtrl(true);
	Sleep(100);
	}
	}*/
	g_dlgDevice->EFGCut(false);
	//MyMsgBoxReadIo("��ʱ");
	/*
	gTrdCam=AfxBeginThread(CameraThreadTest , &gnn , THREAD_PRIORITY_NORMAL , 0 , CREATE_SUSPENDED);
	gTrdCam->m_bAutoDelete = TRUE;
	gTrdCam->ResumeThread();*/
}

BOOL CRun::CamView()
{
	/*if (glCamID < 0)
	{
	return 0;
	}
	NET_SDK_CLIENTINFO clientinfo;
	memset(&clientinfo, 0, sizeof(clientinfo));
	clientinfo.lChannel = 1;
	clientinfo.lLinkMode = 0;
	clientinfo.hPlayWnd = GetDlgItem(IDC_PIC_SHOW)->GetSafeHwnd();
	//qinye
	#if 0
	NET_RealPlay(1, &clientinfo, RealDataCallBack0, NULL, TRUE);
	#else 
	//test vlc ------------------------------------------------------------------------------------------------------
	if(!gb_PlayOrNot[0])
	{
	gb_PlayOrNot[0]=1;
	//RTSP	��ַ
	//char play_address[]="rtsp://218.204.223.237:554/live/1/0547424F573B085C/gsfp90ef4k0a6iap.sdp";
	char play_address[] ="rtsp://192.168.1.102:8557/PSIA/Streaming/channels/2?videoCodecType=H.264";
	libvlc_instance_t *     vlc_ins    = NULL;
	//	libvlc_media_player_t * vlc_player_run = NULL;
	libvlc_media_t *        vlc_media  = NULL;
	const char * vlc_args[] = 
	{
	"-I",
	"dummy",
	"--verbose=2",
	"--aspect-ratio=16:9",
	//"--align=4",
	//"--width=500",
	//"--height=1600",
	"--zoom=0.5",
	//"--vout",
	//"--sout=#duplicate{dst=display,dst=std{accs=file,mux=ts,dst=D:\\1.h264}}"
	//"--sout-transcode-croptop=10"
	}; 


	// ����һ��VLCʵ��
	vlc_ins = libvlc_new(sizeof(vlc_args)/sizeof(vlc_args[0]), vlc_args);
	if(vlc_ins != NULL)
	{
	// ����һ��VLC������
	vlc_player_run = libvlc_media_player_new(vlc_ins);
	if(vlc_player_run != NULL)
	{
	// ͨ���ļ�·������һ��ý��ʵ��,�������ҵĲ����ļ�
	vlc_media = libvlc_media_new_path(vlc_ins, play_address);
	if(vlc_media != NULL)
	{
	// ����ý��ʵ��
	libvlc_media_parse(vlc_media);
	// ��ȡý���ļ��Ĳ��ų���,  ���� ms
	libvlc_time_t duration = libvlc_media_get_duration(vlc_media);

	// �˴��ǻ�ȡý������������Ƶ����Ƶ���Լ��������͵Ĺ����Ϣ
	libvlc_media_track_info_t *media_tracks = NULL;
	int trackCount = libvlc_media_get_tracks_info(vlc_media, &media_tracks);
	// �������ͷ��ڴ棬���Ҳ��Ե�ʱ��������⣬��û��ϸ�о���Ϊ��
	// free(media_tracks);  // crash?

	// �Ѵ򿪵�ý���ļ����ø�������
	libvlc_media_player_set_media(vlc_player_run, vlc_media);

	// ��Ϊ��windowsϵͳ��������Ҫ����һ��HWND����������Ϊ����,�����ֱ��ʹ�����洰��,������ǲ���
	libvlc_media_player_set_hwnd(vlc_player_run, clientinfo.hPlayWnd);
	// ��ʼ������Ƶ
	libvlc_media_player_play(vlc_player_run);
	//	Sleep(3000);
	//	libvlc_media_player_stop(vlc_player_run);
	}
	}
	}
	}
	else
	{
	gb_PlayOrNot[0]=0;
	libvlc_media_player_stop(vlc_player_run);
	}
	//test vlc end------------------------------------------------------------------------------------------------------
	#endif
	//end
	SetLastError(0);
	INT N=GetLastError();*/
	/*if(!gb_PlayOrNot[0])
	{
	gb_PlayOrNot[0]=1;
	stuTrd->pWnd= GetDlgItem(IDC_PIC_SHOW);
	if(!gclsIiic.Show(1))
	AfxMessageBox(_T("��Ƶ����ʧ��"));
	}
	else
	{
	gb_PlayOrNot[0]=0;
	gclsIiic.Show(0);
	}*/

	if (!gb_PlayOrNot[0])
	{
		gb_PlayOrNot[0] = 1;
		gclsCamera.Preview(gnChannel, GetDlgItem(IDC_PIC_SHOW));
		/*stuTrd->pWnd= GetDlgItem(IDC_PREVIEW);
		gclsIiic.Show(1);*/
	}
	else
	{
		gb_PlayOrNot[0] = 0;
		//gclsIiic.Show(0);
		gclsCamera.StopPreview(gnChannel);  //ֹͣԤ��
	}

	return 1;
}

//BOOL CRun::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: �ڴ����ר�ô����/����û���
//	return CDialogEx::PreTranslateMessage(pMsg);
//}


void CRun::OnBnClickedBtnTestsplit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

#ifndef DEBUG_MODE
	//if (glCamID>=0)
	//{
	//	NET_SNAPCFG struSnapCfg;
	//	gstuRun.bOverCam=0;

	//	NET_ManualSnap(glCamID, &struSnapCfg, &gstuCamResult);
	//	while(!gstuRun.bOverCam)//�ȴ��ɼ�ͼ�����
	//		Sleep(1);
	if (gnChannel >= 0)
	{
		char l_AnsiStr[MAX_PATH];
		WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, gstuPathInf.csPathExe + _T("\\PIC\\ԭͼ0.bmp"), -1, l_AnsiStr, sizeof(l_AnsiStr), NULL, NULL);

		int nWidth, nHeight;
		int nBitCount;
		int nBitsPerSample;
		unsigned char *pData = gclsCamera.SnapEx(gnChannel, &nWidth, &nHeight, &nBitCount, &nBitsPerSample, l_AnsiStr, NULL);

#endif
		CString m_bmp_file=gstuPathInf.csPathExe + _T("\\PIC\\ԭͼ0.bmp");
		CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
		BOOL ifFind = findini.FindFile(m_bmp_file);  
		if( !ifFind )  
			return;
		CBitmap m_bmp;//�������Ա
		BITMAP bm;//���λͼ��Ϣ�Ľṹ
		HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,m_bmp_file,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//����bitmapָ��
		m_bmp.Attach(hBitmap1);//���������cbitmap����
		m_bmp.GetBitmap(&bm);
		CWnd * pWnd = GetDlgItem(IDC_PIC_SHOW);
		CDC* pDC = pWnd->GetDC();
		CRect rect;
		GetDlgItem(IDC_PIC_SHOW)->GetClientRect(&rect);
		CDC memDC;        //����һ���豸
		CClientDC dc1(this);      //��ȡ�ͻ�
		memDC.CreateCompatibleDC( &dc1 );
		memDC.SelectObject( m_bmp );  //Ϊ�豸ѡ�����
		mDCMem->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);
		memDC.DeleteDC();
		m_bmp.DeleteObject();	
		DeleteObject(hBitmap1);//�ǵ�ɾ��	
		CPen PenRed;
		CGdiObject *o = NULL;
		PenRed.CreatePen(PS_SOLID,1,RGB(255,0,0));//����һ֧��ɫ�Ļ���.
		mDCMem->SelectObject(PenRed);//ѡ�л���.
		char chPath[MAX_PATH];
		WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_bmp_file,-1,chPath,sizeof(chPath),NULL,NULL); 
		//
		LARGE_INTEGER l_lgint_start, l_lgint_end;
		LARGE_INTEGER l_lgint_freq;
		QueryPerformanceFrequency(&l_lgint_freq);  
		QueryPerformanceCounter(&l_lgint_start);
		//
		gclsImgRcg.RCGBMPSPLIT(mDCMem,rect,chPath,gstuRcgInfo.nPToL,gstuRcgInfo.nDefectPToL,gstuRcgInfo.nThreshold,0,gstuRcgInfo.bIsCir,gstuRcgInfo.bThrdAuto,gstuRcgInfo.bDelNoise);
		//
		QueryPerformanceCounter(&l_lgint_end);
		double dTmpRT=double(l_lgint_end.QuadPart-l_lgint_start.QuadPart)/double(l_lgint_freq.QuadPart);	
		CString csTmp;
		csTmp.Format(_T("%.2f��"),dTmpRT);
		SetDlgItemText(IDC_STAT_RUN,csTmp);
		//
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);
		pWnd->ReleaseDC(pDC);
#ifndef DEBUG_MODE
	}
	else
		g_dlgRun->SetDlgItemText(IDC_STAT_RUN,_T("���δ����"));
#endif
}


BOOL CRun::PreTranslateMessage(MSG* pMsg)
{
  // TODO: �ڴ����ר�ô����/����û���
  if (pMsg->message == WM_KEYDOWN)
    //if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
    {
      //�ж��Ƿ��¼���Enter��
      switch (pMsg->wParam)
      {
      case VK_RETURN:
        //g_dlgRun->OnBnClickedBtnRun();
        break;
      case VK_SPACE:
        //g_dlgRun->OnBnClickedBtnStaft();
        g_dlgDevice->EFGCtrl(true);
        break;
      case VK_ESCAPE:
         // OnCancel();
          return TRUE;
      case 'X':
        if (GetKeyState(VK_CONTROL) < 0)
          g_dlgRun->OnBnClickedBtnStandard();
      default:
        break;
      }
      //return TRUE;
      //Sleep(100);
    }
  return CDialogEx::PreTranslateMessage(pMsg);
}


void CRun::OnBnClickedBtnPause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int status;
	gstuRun.chStmCmd &= ~(1<<2);

	switch (gstuRun.unStatRun)
	{
	case 1://�����ͣ״̬-������
		gstuRun.bTrdPause=0;
		gstuRun.unStatRun=2;
		g_dlgDevice->StmPause(0);
		g_dlgRun->GetDlgItem(IDC_BTN_RUN)->EnableWindow(1);
		// 		g_dlgRun->GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(0);
		// 		gstuRun.bRunStatChg = 1; //ָʾontime��ʱ����
		break;
	case 2://�������״̬-����ͣ
		gstuRun.bTrdPause=1;
		gstuRun.unStatRun=1;
		g_dlgDevice->StmPause(1);
		g_dlgRun->GetDlgItem(IDC_BTN_RUN)->EnableWindow(0);
		// 		g_dlgRun->GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(0);
		// 		gstuRun.bRunStatChg = 1; //ָʾontime��ʱ����
		break;
	default:
		break;
	}
	Sleep(2000);
}

void CRun::OnBnClickedBtnPmove()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValueX = GetDlgItemInt(IDC_P_X,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error X number"));
		return;
	}
	int nValueY = GetDlgItemInt(IDC_P_Y,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error Y number"));
		return;
	}
	g_dlgDevice->XYMove(nValueX,nValueY);
}

void CRun::OnBnClickedBtnEndft()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_dlgDevice->EFGCut(true);
}


void CRun::OnBnClickedChkShake()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_SHAKE);
	int nStat = pChk->GetCheck();
	if (nStat)
	{
		gstuRun.closeShake = 1;
	}
	else
	{
		gstuRun.closeShake = 0;
	}
}


void CRun::OnBnClickedBtnStdcheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgCheckStd dlg;
	int ret = dlg.DoModal();
	if(ret != 101)
	{
		AfxMessageBox(_T("�Ա�׼δͨ��"));
		return;
	}
	gstuSort.needCheck=0;
	CString str;
	GetDlgItemText(IDC_EDT_MIDDEG,str);
	WritePrivateProfileString(_T("�Ա��趨"),_T("��һ�ζԱ����ĽǶ�"),str,gstuPathInf.csPathIni); 

	return;
}


void CRun::OnBnClickedBtnCpk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CDlgCpk dlg(g_pCpk);
	dlg.DoModal();

	return;
}

#include "TrendChart.h"

void CRun::OnBnClickedBtnPrint()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	 // CTrendChart trend;

  //trend.Show();

  //return;

  int count = this->m_listSort.GetItemCount(), page, row = 0;

  int i, j;

  page = count / 40 + 1;

  CPrintDialog print(false);

  if (print.DoModal() == IDOK)
  {
    CDC printed;

    printed.Attach(print.GetPrinterDC());

    DOCINFO pdoc;

    memset(&pdoc, 0, sizeof(pdoc));

    pdoc.cbSize = sizeof(pdoc);

    pdoc.lpszDocName = L"pdoc";

    pdoc.lpszDatatype = NULL;

    pdoc.fwType = NULL;

    pdoc.lpszOutput = NULL;

    if (printed.StartDoc(&pdoc) >= 0)
    {
      LOGFONT logfont;

      memset(&logfont, 0, sizeof(LOGFONT));

      logfont.lfHeight = 75;

      CFont font;

      CFont *oldfont = NULL;

      if (font.CreateFontIndirect(&logfont))

        oldfont = (CFont*)printed.SelectObject(&font);
      
      CRect rectPrint(0, 0,

        printed.GetDeviceCaps(HORZRES),

        printed.GetDeviceCaps(VERTRES));

      printed.DPtoLP(&rectPrint);

      CSize sizeText(75, 75);

      sizeText = printed.GetTextExtent(_T("00"), 2);

      int xMax = rectPrint.Width();

      int xStep = rectPrint.Width() / 12;

      int yMax = rectPrint.Height() / 2;

      int yStep = rectPrint.Height() / 2 / ((count + 1) / 2 + 5);

      int xl = xStep;

      int xr = rectPrint.Width() / 2 + xStep;

      for (j = 1; j <= page; j++)
      {
        printed.StartPage();

        CString pageHead, pageBottom;

        pageHead.Format(_T("�������"));

        printed.TextOut(xStep * 5, yStep * 1, pageHead); //��ӡҳü

        CString title;//���ñ�����

        title.Format(_T("���     �Ƕ�     ����"));

        printed.TextOut(xl, yStep * 2, title); //��ӡ����

        printed.TextOut(xr, yStep * 2, title); //��ӡ����

        CString stt;

        stt.Format(_T("________________________________"));

        printed.TextOut(xl, yStep * 2 + sizeText.cy, stt); //��ӡstt

        printed.TextOut(xr, yStep * 2 + sizeText.cy, stt); //��ӡstt

        for (i = 1; (i < 40 / 2) && (row < count / 2); i++) //��ӡlist
        {
          CString record(_T(""));

          record += this->m_listSort.GetItemText(row, 0) + L"     ";

          record += this->m_listSort.GetItemText(row, 1) + L"     ";

          record += this->m_listSort.GetItemText(row, 2);

          printed.TextOut(xl, yStep * (2 + i), record);

          printed.TextOut(xl, yStep * (2 + i) + sizeText.cy, stt);

          row++;
        }
        for (i = 1; (i < 40 / 2) && (row < count); i++)
        {
          CString record(_T(""));

          record += this->m_listSort.GetItemText(row, 0) + L"     ";

          record += this->m_listSort.GetItemText(row, 1) + L"     ";

          record += this->m_listSort.GetItemText(row, 2);

          printed.TextOut(xr, yStep * (2 + i), record);

          printed.TextOut(xr, yStep * (2 + i) + sizeText.cy, stt);

          row++;
        }
        pageBottom.Format(_T("��%dҳ   ��%dҳ"), page, j);

        printed.TextOut(xStep * 5, yMax - yStep, pageBottom);// ��ӡҳ��

        printed.EndPage();//��ҳ����
      }

      if (oldfont != NULL)

        printed.SelectObject(oldfont);

      font.DeleteObject();

      printed.EndDoc();
    }
    printed.Detach();

    printed.DeleteDC();
  }

}
