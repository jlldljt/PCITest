// Camera.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZPJ.h"
#include "Camera.h"
#include "afxdialogex.h"

void calparameter(double (*xy)[4],double *factor);//��������ת������
/*****vlc ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++test********/

//#define WIN32_LEAN_AND_MEAN

/*#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <windows.h>

//#define int64_t long
//#define uint32_t int

#include <vlc.h>
//#pragma comment(lib, "libvlc.lib")
//#pragma comment(lib, "libvlccore.lib")

libvlc_media_player_t * vlc_player = NULL;
BOOL gb_PlayOrNot[1]=0;*/
/*******end***++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++***/
// CCamera �Ի���

IMPLEMENT_DYNAMIC(CCamera, CDialogEx)

CCamera::CCamera(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCamera::IDD, pParent)
{
	g_dlgCamera=this;
	
}

CCamera dlgCamera;
CCamera *g_dlgCamera=NULL;

CCamera::~CCamera()
{
	mDCMem->SelectObject(mBMPOld);
	mBMPMem->DeleteObject();
	mDCMem->DeleteDC();
}

void CCamera::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_AWB, m_cmbAWB);
	DDX_Control(pDX, IDC_CMB_BACKLIGHT, m_cmbBl);
}


BEGIN_MESSAGE_MAP(CCamera, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SNAP, &CCamera::OnBnClickedBtnSnap)
	ON_BN_CLICKED(IDC_BTN_TEST, &CCamera::OnBnClickedBtnTest)
	ON_BN_CLICKED(IDC_BTN_CAMSET, &CCamera::OnBnClickedBtnCamset)
	ON_STN_CLICKED(IDC_PREVIEW, &CCamera::OnStnClickedPreview)
	ON_BN_CLICKED(IDC_BTN_CALIBRATION, &CCamera::OnBnClickedBtnCalibration)
	ON_BN_CLICKED(IDC_BTN_VIDEO, &CCamera::OnBnClickedBtnVideo)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDT_OUTPALLOWL, &CCamera::OnEnChangeEdtOutpallowl)
	ON_EN_CHANGE(IDC_EDT_DEFECTALLOWL, &CCamera::OnEnChangeEdtDefectallowl)
	ON_EN_CHANGE(IDC_EDT_THRESHOLD, &CCamera::OnEnChangeEdtThreshold)
	ON_EN_CHANGE(IDC_EDT_FTOUTPOINT, &CCamera::OnEnChangeEdtFtoutpoint)
	ON_BN_CLICKED(IDC_BTN_GETSET, &CCamera::OnBnClickedBtnGetset)
	ON_BN_CLICKED(IDC_CHK_AUTOTHRD, &CCamera::OnBnClickedChkAutothrd)
	ON_BN_CLICKED(IDC_CHK_DELNOISE, &CCamera::OnBnClickedChkDelnoise)
//	ON_BN_CLICKED(IDC_CHECK3, &CCamera::OnBnClickedCheck3)
ON_BN_CLICKED(IDC_CHK_DEBUG, &CCamera::OnBnClickedChkDebug)
ON_BN_CLICKED(IDC_BTN_SPLIT, &CCamera::OnBnClickedBtnSplit)
ON_BN_CLICKED(IDC_BTN_RECONNECT, &CCamera::OnBnClickedBtnReconnect)
ON_BN_CLICKED(IDC_BTN_PARMSEND, &CCamera::OnBnClickedBtnParmsend)
ON_BN_CLICKED(IDC_BTN_AUTO, &CCamera::OnBnClickedBtnAuto)
END_MESSAGE_MAP()


// CCamera ��Ϣ�������


void CCamera::OnBnClickedBtnSnap()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*CString m_bmp_file=_T(".\\ԭͼ0.bmp");
	CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
	BOOL ifFind = findini.FindFile(m_bmp_file);  
	if( !ifFind )  
		return;
	CBitmap m_bmp;//�������Ա
	BITMAP bm;//���λͼ��Ϣ�Ľṹ
	HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,m_bmp_file,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//����bitmapָ��
	m_bmp.Attach(hBitmap1);//���������cbitmap����
	m_bmp.GetBitmap(&bm);
	CWnd * pWnd = GetDlgItem(IDC_PREVIEW);
	CDC* pDC = pWnd->GetDC();
	
	CRect rect;
	GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
	CDC memDC;        //����һ���豸
	CClientDC dc1(this);      //��ȡ�ͻ�
	memDC.CreateCompatibleDC( &dc1 );
	memDC.SelectObject( m_bmp );  //Ϊ�豸ѡ�����
	pDC->SetStretchBltMode (HALFTONE);
	SetBrushOrgEx(pDC->m_hDC, 0, 0, NULL);
	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	//pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, 1000, 500, SRCCOPY);
	memDC.DeleteDC();	
	m_bmp.DeleteObject();
	DeleteObject(hBitmap1);//�ǵ�ɾ��	
	pWnd->ReleaseDC(pDC);*/
	//
	/*LARGE_INTEGER l_lgint_start, l_lgint_end;
	LARGE_INTEGER l_lgint_freq;
	QueryPerformanceFrequency(&l_lgint_freq);  
	QueryPerformanceCounter(&l_lgint_start);
	//
	if (glCamID>=0)
	{
		NET_SNAPCFG struSnapCfg;
		gstuRun.bOverCam=0;
		NET_ManualSnap(glCamID, &struSnapCfg, &gstuCamResult);
		while(!gstuRun.bOverCam)//�ȴ��ɼ�ͼ�����
				Sleep(1);
		//////////////
		//
	QueryPerformanceCounter(&l_lgint_end);
	double dTmpRT=double(l_lgint_end.QuadPart-l_lgint_start.QuadPart)/double(l_lgint_freq.QuadPart);	
	CString csTmp;
	csTmp.Format(_T("%.2f��"),dTmpRT);
	SetDlgItemText(IDC_SELECT_RESULT,csTmp);
		//
	CPicture l_class_pic;
	//char bmp_path[] ="F:\\visual studio 2010\\Projects\\BZJ\\BZJ\\123.jpg";//_T("d://tmep1.bmp")
	CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
	CString csPathTmp = gstuPathInf.csPathExe +_T("\\PIC\\ԭͼ0.jpg");
	BOOL ifFind = findini.FindFile(csPathTmp);  
	if( !ifFind )  
		return;
	CBitmap m_bmp;//�������Ա
	BITMAP bm;//���λͼ��Ϣ�Ľṹ
	//Sleep(2000);
	char chPath[MAX_PATH];
	WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,csPathTmp,-1,chPath,sizeof(chPath),NULL,NULL); 
	HBITMAP hBitmap1 = l_class_pic.LoadPicture(chPath);//(HBITMAP)LoadImage(NULL,bmp_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//����bitmapָ��
	m_bmp.Attach(hBitmap1);//���������cbitmap����
	l_class_pic.SaveBitmapToFile(&m_bmp, gstuPathInf.csPathExe+_T("\\PIC\\ԭͼ0.bmp"));
	//l_class_pic.SaveNewImage(hBitmap1, _T(".\\PIC\\ԭͼ0.bmp"), 24);
	//g_tmp_path=_T(".\\PIC\\ԭͼ0.bmp");
	m_bmp.GetBitmap(&bm);
	CWnd * pWnd = GetDlgItem(IDC_PREVIEW);
	CDC* pDC = pWnd->GetDC();
	CRect rect;
	GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
	CDC memDC;        //����һ���豸
	CClientDC dc1(this);      //��ȡ�ͻ�
	memDC.CreateCompatibleDC( &dc1 );
	memDC.SelectObject( m_bmp );  //Ϊ�豸ѡ�����
	pDC->SetStretchBltMode (HALFTONE);
	SetBrushOrgEx(pDC->m_hDC, 0, 0, NULL);
	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	memDC.DeleteDC();
	m_bmp.DeleteObject();
	//
	//	l_class_pic.SaveBmp(hBitmap1,(".\\PIC\\ԭͼ0.bmp"));

	//
	DeleteObject(hBitmap1);//�ǵ�ɾ��
	pWnd->ReleaseDC(pDC);
	l_class_pic.FreePicture();
	}
	else
	{
		AfxMessageBox(_T("���δ����"));
		return;
	}*/

	//
	//LARGE_INTEGER l_lgint_start, l_lgint_end;
	//LARGE_INTEGER l_lgint_freq;
	//QueryPerformanceFrequency(&l_lgint_freq);  
	//CWnd * pWnd = GetDlgItem(IDC_PREVIEW);
	//CDC* pDC = pWnd->GetDC();
	//CRect rect;
	//GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
	//pDC->SetStretchBltMode (HALFTONE);
	//SetBrushOrgEx(pDC->m_hDC, 0, 0, NULL);
	//
	//QueryPerformanceCounter(&l_lgint_start);
	//if(!gclsIiic.TakeBmpFromRtsp())
	//	return;
	//QueryPerformanceCounter(&l_lgint_end);
	//double dTmpRT=double(l_lgint_end.QuadPart-l_lgint_start.QuadPart)/double(l_lgint_freq.QuadPart);	
	//CString csTmp;
	//csTmp.Format(_T("%.2f��"),dTmpRT);
	//SetDlgItemText(IDC_SELECT_RESULT,csTmp);

	//BITMAPINFOHEADER RGB32BITSBITMAPINFO = {   
	//	sizeof(BITMAPINFOHEADER),    // biSize 
	//	1920,                    // biWidth; 
	//	1080,                // biHeight; 
	//	1,                            // biPlanes; 
	//	24,                            // biBitCount 
	//	BI_RGB,                        // biCompression; 
	//	(1920 * 24/8+3)/4*4 * 1080,                            // biSizeImage; 
	//	0,                            // biXPelsPerMeter; 
	//	0,                            // biYPelsPerMeter; 
	//	0,                            // biClrUsed; 
	//	0                            // biClrImportant; 
	//};
	//StretchDIBits(pDC->GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 
	//	0, 0,  1920, 1080, pFrameRGB->data[0], 
	//	(BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, SRCCOPY); 
	//
	//char l_AnsiStr[MAX_PATH];  
	//WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,gstuPathInf.csPathExe+_T("\\PIC\\ԭͼ0.bmp"),-1,l_AnsiStr,sizeof(l_AnsiStr),NULL,NULL); 

	//gclsIiic.Save(l_AnsiStr);

	//pWnd->ReleaseDC(pDC);
	///////////////////////////////////ksj///////////////////////////
	LARGE_INTEGER l_lgint_start, l_lgint_end;
	LARGE_INTEGER l_lgint_freq;
	QueryPerformanceFrequency(&l_lgint_freq);
	QueryPerformanceCounter(&l_lgint_start);
	char l_AnsiStr[MAX_PATH];
	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, gstuPathInf.csPathExe + _T("\\PIC\\ԭͼ0.bmp"), -1, l_AnsiStr, sizeof(l_AnsiStr), NULL, NULL);

	int nWidth, nHeight;
	int nBitCount;
	int nBitsPerSample;
	unsigned char *pData = gclsCamera.SnapEx(gnChannel, &nWidth, &nHeight, &nBitCount, &nBitsPerSample, l_AnsiStr, GetDlgItem(IDC_PREVIEW));
	
	QueryPerformanceCounter(&l_lgint_end);
	double dTmpRT = double(l_lgint_end.QuadPart - l_lgint_start.QuadPart) / double(l_lgint_freq.QuadPart);
	CString csTmp;
	csTmp.Format(_T("%.2f��"), dTmpRT);
	SetDlgItemText(IDC_SELECT_RESULT, csTmp);

	gstuRun.bOverRcg=1;
}


void CCamera::OnBnClickedBtnTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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
	CWnd * pWnd = GetDlgItem(IDC_PREVIEW);
	CDC* pDC = pWnd->GetDC();
	CRect rect;
	GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
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

	//pWnd = GetDlgItem(IDC_image1); //IDC_STATIC��picture control��ID.����ǵõ�picture�ľ��.
	//pDC = pWnd->GetDC();//Ȼ��õ��豸����.
	//RECT rect;//����һ��rect
	//pWnd->GetClientRect(&rect);//����picture�ؼ���������rect����.
	mDCMem->SelectObject(PenRed);//ѡ�л���.

	/*gstuRcgInfo.nPToL=2;//ֱ�ߵ��
	gstuRcgInfo.nDefectPToL=10;//ȱ�ݵ��
	gstuRcgInfo.nThreshold=30;//��ֵ*/
	//char * chPath = (LPSTR)(LPCTSTR)m_bmp_file;
	char chPath[MAX_PATH];
	//WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_bmp_file,-1,readPath,CStringA(m_bmp_file).GetLength(),NULL,NULL); 
	WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_bmp_file,-1,chPath,sizeof(chPath),NULL,NULL); 
	//
	LARGE_INTEGER l_lgint_start, l_lgint_end;
	LARGE_INTEGER l_lgint_freq;
	QueryPerformanceFrequency(&l_lgint_freq);  
	QueryPerformanceCounter(&l_lgint_start);
	//
	gclsImgRcg.RCGBMP(mDCMem,rect,chPath,gstuRcgInfo.nPToL,gstuRcgInfo.nDefectPToL,gstuRcgInfo.nThreshold,gstuRcgInfo.bDebug,gstuRcgInfo.bIsCir,gstuRcgInfo.bThrdAuto,gstuRcgInfo.bDelNoise);
	//
	QueryPerformanceCounter(&l_lgint_end);
	double dTmpRT=double(l_lgint_end.QuadPart-l_lgint_start.QuadPart)/double(l_lgint_freq.QuadPart);	
	CString csTmp;
	csTmp.Format(_T("%.2f��"),dTmpRT);
	SetDlgItemText(IDC_SELECT_RESULT,csTmp);
		//
	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);


  CBrush br;
  br.CreateStockObject(NULL_BRUSH);
  pDC->SelectObject(&br);
  pDC->Rectangle(g_npc_inf.left * rect.Width() / gclsImgRcg.g_stu_square.nBMPW
    , g_npc_inf.top * rect.Height() / gclsImgRcg.g_stu_square.nBMPH
    , g_npc_inf.right * rect.Width() / gclsImgRcg.g_stu_square.nBMPW
    , g_npc_inf.bottom * rect.Height() / gclsImgRcg.g_stu_square.nBMPH);


	pWnd->ReleaseDC(pDC);
	gstuRcgInfo.bClbPos=1;
/*	g_tmp_path=_T(".\\PIC\\temp1.bmp");
	CString l_cstr_tmp;
	l_cstr_tmp.Format(_T("%d��%d"),g_class_img_recognition.g_stu_square.crests,g_class_img_recognition.g_stu_square.sags);
	GetDlgItem(IDC_EDIT_BROKEN_RAW)->SetWindowText(l_cstr_tmp);
	l_cstr_tmp.Format(_T("%.2lf��%.2lf��%.2lf��%.2lf"),g_class_img_recognition.g_stu_square.d_len[0],g_class_img_recognition.g_stu_square.d_len[1],g_class_img_recognition.g_stu_square.d_len[2],g_class_img_recognition.g_stu_square.d_len[3]);
	GetDlgItem(IDC_EDIT_SIZE_RAW)->SetWindowText(l_cstr_tmp);*/
}


void CCamera::OnBnClickedBtnCamset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//char szLan[128] = {0};
	//char szTemp[256] = {0};  
	//char szErrorMsg[256] = {0};

	//gstuCamParam.Brightness = GetDlgItemInt(IDC_EDT_BRIGHT,0,0);
	//gstuCamParam.Contrast = GetDlgItemInt(IDC_EDT_CONTRAST,0,0);
	//gstuCamParam.Saturation = GetDlgItemInt(IDC_EDT_SATURATION,0,0);
	//gstuCamParam.Sharpness = GetDlgItemInt(IDC_EDT_SHARPNESS,0,0);
	//gstuCamParam.awb = (BYTE)m_cmbAWB.GetCurSel();
	//gstuCamParam.maxexposuretime = GetDlgItemInt(IDC_EDT_EV,0,0);
	//gstuCamParam.maxgain = GetDlgItemInt(IDC_EDT_GAIN,0,0);
	//gstuCamParam.histogram = 0;
	//gstuCamParam.vidstb1 = 0;
	//gstuCamParam.BackLight = (BYTE)m_cmbBl.GetCurSel();
	//gstuCamParam.colorkiller = 0;
	//gstuCamParam.blc = 1;//(BYTE)m_comboBlc.GetCurSel();
	//gstuCamParam.dynrange	= 0;//(BYTE)m_comboDynrange.GetCurSel();
	//gstuCamParam.exposurectrl	= 0;//(BYTE)m_comboExposureCtrl.GetCurSel();
	//gstuCamParam.lensdistortcorrection =0;// (BYTE)m_comboLenCorrect.GetCurSel();
	//gstuCamParam.tnfltctrl = 0;//(BYTE)m_comboTnfltCtrl.GetCurSel();
	//gstuCamParam.nfltctrl = 0;//(BYTE)m_comboNfltCtrl.GetCurSel();

	//if(!NET_SetConfig(glCamID, IPC_SET_GETTINGS_CAMERA_PARAM, &gstuCamParam, sizeof(gstuCamParam)))
	//{
	//	NET_GetErrorMsg(NET_GetLastError(), szErrorMsg);
	//	sprintf(szTemp, "%s %s", szLan, szErrorMsg);
	//	AfxMessageBox((CString)szTemp);
	//	return;
	//}
	gclsCamera.SetParam(gnChannel, KSJ_EXPOSURE, GetDlgItemInt(IDC_EDT_EV));//
	gclsCamera.SetParam(gnChannel, KSJ_RED, GetDlgItemInt(IDC_EDT_GAIN));//
	gclsCamera.SetParam(gnChannel, KSJ_BRIGHTNESS, GetDlgItemInt(IDC_EDT_BRIGHT));//
	gclsCamera.SetParam(gnChannel, KSJ_CONTRAST, GetDlgItemInt(IDC_EDT_CONTRAST));//
}


BOOL CCamera::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//CWnd * pWnd = GetDlgItem(IDC_PREVIEW);
	//CDC* pDC = pWnd->GetDC();
	//mDCMem= new CDC;
	//CClientDC dc1(GetDlgItem(IDC_PREVIEW)->GetDC());      //��ȡ�ͻ�
	CWnd * pWnd = GetDlgItem(IDC_PREVIEW);
	CDC* pDC = pWnd->GetDC();
	CRect rect;
	GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
	mDCMem= new CDC;
	mBMPMem=new CBitmap;
	mDCMem->CreateCompatibleDC(pDC);
	mBMPMem->CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	mBMPOld=mDCMem->SelectObject(mBMPMem);//�������Ͼ䣬��mdcmen������Ч
	mDCMem->SetStretchBltMode (HALFTONE);
	SetBrushOrgEx(mDCMem->m_hDC, 0, 0, NULL);
	pWnd->ReleaseDC(pDC);
	//mDCMem->CreateCompatibleDC(GetDlgItem(IDC_PREVIEW)->GetDC());
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CCamera::OnStnClickedPreview()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
  //NpcParm par = { 0 };

	if (0==gclsImgRcg.g_stu_square.nN)
	{
		//return;
	}
  double xt,yt;
	POINT pt;//�����
	GetCursorPos(&pt);//ȡ�õ�ǰ����
	CRect lRect;
	GetDlgItem(IDC_PREVIEW)->GetWindowRect(&lRect);
	double temp_x=pt.x-lRect.left;
  double temp_y=pt.y-lRect.top;
	xt=/*(int)*/(temp_x*gclsImgRcg.g_stu_square.nBMPW/(lRect.Width()));//����������Ӧ��ͼ���ϵ�x��
	yt=/*(int)*/(temp_y*gclsImgRcg.g_stu_square.nBMPH/(lRect.Height()));//����������Ӧ��ͼ���ϵ�y��
	//if(!m_RadioCircle && m_RadioSquare)
	{
		//if(avg1.empty()){AfxMessageBox(_T("û�ж�λ���꣬�붨λ"));return;}

		int temp=0;
    int i;

		for(i=0;i<gclsImgRcg.g_stu_square.nN;i++)
		{

			if(sqrt(pow(double(gclsImgRcg.g_stu_square.pnZPX[i]-xt),2)+pow(double(gclsImgRcg.g_stu_square.pnZPY[i]-yt),2))<((gclsImgRcg.g_stu_square.pnWth[i])>>1)) //���λ����Ƭ�Ĵ����������С��s_sort.allowable_devision����ô����Ҫ���Ե��㲻׼ʱ���õ���
			{
				//DrawPicToHDC(line_img, IDC_PREVIEW);//��ͼƬ�ؼ�����ʾ
				CWnd * pWnd = GetDlgItem(IDC_PREVIEW);
				CDC* pDC = pWnd->GetDC();
				CRect rect;
				GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
				CPen PenRed;
				PenRed.CreatePen(PS_SOLID,1,RGB(255,0,0));//����һ֧��ɫ�Ļ���.
				pDC->SelectObject(PenRed);//ѡ�л���.
				pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);
				if(gstuRcgInfo.bIsCir)
				{
					CString csTmp;
					csTmp.Format(_T("�뾶%d"),gclsImgRcg.g_stu_square.pnWth[i]);
					GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);
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
          //
					POINT pPoint[5];
					for(int j=0;j<4;j++)
					{
						pPoint[j].x = gclsImgRcg.g_stu_square.pnPX[i][j]*rect.Width()/gclsImgRcg.g_stu_square.nBMPW;
						pPoint[j].y = gclsImgRcg.g_stu_square.pnPY[i][j]*rect.Height()/gclsImgRcg.g_stu_square.nBMPH;
					}
					pPoint[4].x = gclsImgRcg.g_stu_square.pnPX[i][0]*rect.Width()/gclsImgRcg.g_stu_square.nBMPW;
					pPoint[4].y = gclsImgRcg.g_stu_square.pnPY[i][0]*rect.Height()/gclsImgRcg.g_stu_square.nBMPH;

					pDC->Polygon(pPoint,4);
          //test
          double angle_test = gclsImgRcg.CalculateVectorAngle(pPoint[1].x-pPoint[0].x,
            pPoint[1].y-pPoint[0].y, 1, 0);
          /*int nLenNo = gclsImgRcg.g_stu_square.lenNo1PN[i];
          int nLenNoNext = (3 == nLenNo) ? 0 : (nLenNo + 1);
          */
          int nLenNo = 0;
          int nLenNoNext = (3 == nLenNo) ? 0 : (nLenNo + 1);
          pDC->Ellipse(pPoint[nLenNo].x - 2, pPoint[nLenNo].y - 2, pPoint[nLenNo].x + 2, pPoint[nLenNo].y + 2);

          pDC->Ellipse(pPoint[nLenNoNext].x - 4, pPoint[nLenNoNext].y - 4, pPoint[nLenNoNext].x + 4, pPoint[nLenNoNext].y + 4);

          //
					gclsImgRcg.stuRef.Len=gclsImgRcg.g_stu_square.pnLen[i];
					gclsImgRcg.stuRef.Wth=gclsImgRcg.g_stu_square.pnWth[i];
					gstuRcgInfo.nPN=gclsImgRcg.g_stu_square.bPN[i];

          m_par.deg0 = angle_test;
          int X = gclsImgRcg.g_stu_square.pnZPX[i];
          int Y = gclsImgRcg.g_stu_square.pnZPY[i];
          m_par.x0 = X;
          m_par.y0 = Y;
          m_par.pn0 = 0;//gstuRcgInfo.nPN;
          CString csTmp;
          csTmp.Format(_T("��%d ��%d ���� %d �Ƕ�%.1f X:%d Y:%d"), gclsImgRcg.g_stu_square.pnLen[i],gclsImgRcg.g_stu_square.pnWth[i], (int)(m_par.pn0), (angle_test), X, Y);

          //csTmp.Format(_T("��%d��%dX����%d�Ƕ�%.1f"),gclsImgRcg.g_stu_square.pnLen[i],gclsImgRcg.g_stu_square.pnWth[i],gclsImgRcg.g_stu_square.bPN[i], gclsImgRcg.g_stu_square.angreePN[i]);
          GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);
          //AfxMessageBox(csTmp);
				}

        CBrush br;
        br.CreateStockObject(NULL_BRUSH);
        pDC->SelectObject(&br);
        pDC->Rectangle(g_npc_inf.left * rect.Width() / gclsImgRcg.g_stu_square.nBMPW
          , g_npc_inf.top * rect.Height() / gclsImgRcg.g_stu_square.nBMPH
          , g_npc_inf.right * rect.Width() / gclsImgRcg.g_stu_square.nBMPW
          , g_npc_inf.bottom * rect.Height() / gclsImgRcg.g_stu_square.nBMPH);

				pWnd->ReleaseDC(pDC);
				gclsImgRcg.stuRef.Dev=gstuRcgInfo.nAllowDefect;
				gstuRcgInfo.Xxy[gstuRcgInfo.Nxy][0]=gclsImgRcg.g_stu_square.pnZPX[i]; gstuRcgInfo.Xxy[gstuRcgInfo.Nxy][1]=gclsImgRcg.g_stu_square.pnZPY[i];//xy�������һ���Ķ�������ͬλ�ã���ͬxy���Ƿ������һ���������ԣ�0,1��ͼ��x��y��2�ǹ̶�1��3�ǵ������
				gstuRcgInfo.Yxy[gstuRcgInfo.Nxy][0]=gclsImgRcg.g_stu_square.pnZPX[i]; gstuRcgInfo.Yxy[gstuRcgInfo.Nxy][1]=gclsImgRcg.g_stu_square.pnZPY[i];


        //int X = gstuRcgInfo.g_factor[0][0] * (double)gclsImgRcg.g_stu_square.pnZPX[i] + gstuRcgInfo.g_factor[0][1] * (double)gclsImgRcg.g_stu_square.pnZPY[i] + gstuRcgInfo.g_factor[0][2]+0.5;
        //int Y = gstuRcgInfo.g_factor[1][0] * (double)gclsImgRcg.g_stu_square.pnZPX[i] + gstuRcgInfo.g_factor[1][1] * (double)gclsImgRcg.g_stu_square.pnZPY[i] + gstuRcgInfo.g_factor[1][2]+0.5;
        TranNpcParam(&m_par);

        //m_clked_pos_x = par.x;
        //m_clked_pos_y = par.y;
        
        CString csTmp;
				csTmp.Format(_T("No%d ���� %d �Ƕ�%d X:%d Y:%d"),i, m_par.pn, m_par.deg, m_par.x, m_par.y);
				GetDlgItem(IDC_SELECT_XY)->SetWindowText(csTmp);
				gstuRcgInfo.bClbPos=1;
				break;
      }

		}
    //û�е㵽Ƭ��
      if(i == gclsImgRcg.g_stu_square.nN) {
      CWnd* pWnd = GetDlgItem(IDC_PREVIEW);
      CDC* pDC = pWnd->GetDC();
      CRect rect;
      GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
      CPen PenRed;
      PenRed.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));//����һ֧�Ļ���.
      pDC->SelectObject(PenRed);//ѡ�л���.
      pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);
      POINT point;//�����
      point.x = temp_x - 10 + 0.5;
      point.y = temp_y + 0.5;
      pDC->MoveTo(point);
      point.x = temp_x + 10 + 0.5;
      pDC->LineTo(point);
      //
      point.x = temp_x + 0.5;
      point.y = temp_y - 10 + 0.5;
      pDC->MoveTo(point);
      point.y = temp_y + 10 + 0.5;
      pDC->LineTo(point);

      CBrush br;
      br.CreateStockObject(NULL_BRUSH);
      pDC->SelectObject(&br);
      pDC->Rectangle(g_npc_inf.left* rect.Width() / gclsImgRcg.g_stu_square.nBMPW
        , g_npc_inf.top* rect.Height() / gclsImgRcg.g_stu_square.nBMPH
        , g_npc_inf.right* rect.Width() / gclsImgRcg.g_stu_square.nBMPW
        , g_npc_inf.bottom* rect.Height() / gclsImgRcg.g_stu_square.nBMPH);
      pWnd->ReleaseDC(pDC);
      /////////////////////
      int X = xt + 0.5;
      int Y = yt + 0.5;
      m_par.x0 = xt;
      m_par.y0 = yt;

      CString csTmp;

      csTmp.Format(_T("X:%d Y:%d"), X, Y);

      GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);

      gstuRcgInfo.Xxy[gstuRcgInfo.Nxy][0] = xt; gstuRcgInfo.Xxy[gstuRcgInfo.Nxy][1] = yt;//xy�������һ���Ķ�������ͬλ�ã���ͬxy���Ƿ������һ���������ԣ�0,1��ͼ��x��y��2�ǹ̶�1��3�ǵ������
      gstuRcgInfo.Yxy[gstuRcgInfo.Nxy][0] = xt; gstuRcgInfo.Yxy[gstuRcgInfo.Nxy][1] = yt;

      TranNpcParam(&m_par);
      //int X = gstuRcgInfo.g_factor[0][0] * xt + gstuRcgInfo.g_factor[0][1] * yt + gstuRcgInfo.g_factor[0][2] + 0.5;
      //int Y = gstuRcgInfo.g_factor[1][0] * xt + gstuRcgInfo.g_factor[1][1] * yt + gstuRcgInfo.g_factor[1][2] + 0.5;
      //m_clked_pos_x = par.x;
      //m_clked_pos_y = par.y;

      csTmp.Format(_T("X:%d Y:%d"), m_par.x, m_par.y);
      GetDlgItem(IDC_SELECT_XY)->SetWindowText(csTmp);
      }
		return;
	}
}

#include "DlgPriview.h"
void CCamera::OnBnClickedBtnCalibration()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//gTrdClb=AfxBeginThread(CalibrationThread , NULL , THREAD_PRIORITY_NORMAL , 0 , CREATE_SUSPENDED);
	//gTrdClb->m_bAutoDelete = TRUE;
	//gTrdClb->ResumeThread();
  CDlgPriview dlg;
  dlg.DoModal();
}




void CCamera::OnBnClickedBtnVideo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
/*	NET_SDK_CLIENTINFO clientinfo;
	memset(&clientinfo, 0, sizeof(clientinfo));
	clientinfo.lChannel = 1;
	clientinfo.lLinkMode = 0;
	clientinfo.hPlayWnd = GetDlgItem(IDC_PREVIEW)->GetSafeHwnd();
	//qinye
#if 0
	NET_RealPlay(1, &clientinfo, RealDataCallBack0, NULL, TRUE);
#else 
	//test vlc ------------------------------------------------------------------------------------------------------
	if(!gb_PlayOrNot[1])
	{
		gb_PlayOrNot[1]=1;
		//RTSP	��ַ
		//char play_address[]="rtsp://218.204.223.237:554/live/1/0547424F573B085C/gsfp90ef4k0a6iap.sdp";
		char play_address[] ="rtsp://192.168.1.102:8557/PSIA/Streaming/channels/2?videoCodecType=H.264";
		libvlc_instance_t *     vlc_ins    = NULL;
		//	libvlc_media_player_t * vlc_player = NULL;
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
			vlc_player = libvlc_media_player_new(vlc_ins);
			if(vlc_player != NULL)
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
					libvlc_media_player_set_media(vlc_player, vlc_media);

					// ��Ϊ��windowsϵͳ��������Ҫ����һ��HWND����������Ϊ����,�����ֱ��ʹ�����洰��,������ǲ���
					libvlc_media_player_set_hwnd(vlc_player, clientinfo.hPlayWnd);
					// ��ʼ������Ƶ
					libvlc_media_player_play(vlc_player);
					//	Sleep(3000);
					//	libvlc_media_player_stop(vlc_player);
				}
			}
		}
	}
	else
	{
		gb_PlayOrNot[1]=0;
		libvlc_media_player_stop(vlc_player);
	}
	//test vlc end------------------------------------------------------------------------------------------------------
#endif
	//end
	SetLastError(0);
	INT N=GetLastError();*/
	if(!gb_PlayOrNot[1])
	{
		gb_PlayOrNot[1]=1;
		gclsCamera.Preview(gnChannel, GetDlgItem(IDC_PREVIEW));
		/*stuTrd->pWnd= GetDlgItem(IDC_PREVIEW);
	gclsIiic.Show(1);*/
    SetDlgItemText(IDC_BTN_VIDEO, _T("��ƵԤ����"));
	}
	else
	{
		gb_PlayOrNot[1]=0;
		//gclsIiic.Show(0);
		gclsCamera.StopPreview(gnChannel);  //ֹͣԤ��
    SetDlgItemText(IDC_BTN_VIDEO, _T("��ƵԤ��"));

	}
	

}


void CCamera::OnPaint()
{
	CRect rect,rcUpdate;
	GetUpdateRect(&rcUpdate);
	
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CRect rcWindow;
	GetDlgItem(IDC_PREVIEW)->GetWindowRect(rcWindow);
	ScreenToClient(rcWindow);
	
	if (rcWindow.left<rcUpdate.right && rcWindow.right>rcUpdate.left &&
		rcWindow.top<rcUpdate.bottom && rcWindow.bottom>rcUpdate.top )
	{
		GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
		GetDlgItem(IDC_PREVIEW)->RedrawWindow(&rect,NULL,RDW_INVALIDATE|RDW_ALLCHILDREN);//RDW_VALIDATE|RDW_UPDATENOW
	}
}


void CCamera::OnEnChangeEdtOutpallowl()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValue = GetDlgItemInt(IDC_EDT_OUTPALLOWL,&bFlag,0);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error number"));
		return;
	}

	CString strValue = _T("");
	CFileFind findini;
	BOOL ifFind = findini.FindFile(gstuPathInf.csPathIni);  

	if( !ifFind )  
	{  
		AfxMessageBox(_T("�������ļ�"));
		return;
	}  

	strValue.Format(_T("%d"),nValue);
	WritePrivateProfileString(_T("ʶ�����"),_T("ֱ�ߵ��"),strValue,gstuPathInf.csPathIni);
	gstuRcgInfo.nPToL = nValue;
}


void CCamera::OnEnChangeEdtDefectallowl()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValue = GetDlgItemInt(IDC_EDT_DEFECTALLOWL,&bFlag,0);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error number"));
		return;
	}

	CString strValue = _T("");
	CFileFind findini;
	BOOL ifFind = findini.FindFile(gstuPathInf.csPathIni);  

	if( !ifFind )  
	{  
		AfxMessageBox(_T("�������ļ�"));
		return;
	}  

	strValue.Format(_T("%d"),nValue);
	WritePrivateProfileString(_T("ʶ�����"),_T("ȱ�ݵ��"),strValue,gstuPathInf.csPathIni);
	gstuRcgInfo.nDefectPToL = nValue;
}


void CCamera::OnEnChangeEdtThreshold()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValue = GetDlgItemInt(IDC_EDT_THRESHOLD,&bFlag,0);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error number"));
		return;
	}

	CString strValue = _T("");
	CFileFind findini;
	BOOL ifFind = findini.FindFile(gstuPathInf.csPathIni);  

	if( !ifFind )  
	{  
		AfxMessageBox(_T("�������ļ�"));
		return;
	}  

	strValue.Format(_T("%d"),nValue);
	WritePrivateProfileString(_T("ʶ�����"),_T("��ֵ"),strValue,gstuPathInf.csPathIni);
	gstuRcgInfo.nThreshold = nValue;
}


void CCamera::OnEnChangeEdtFtoutpoint()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValue = GetDlgItemInt(IDC_EDT_FTOUTPOINT,&bFlag,0);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error number"));
		return;
	}

	CString strValue = _T("");
	CFileFind findini;
	BOOL ifFind = findini.FindFile(gstuPathInf.csPathIni);  

	if( !ifFind )  
	{  
		AfxMessageBox(_T("�������ļ�"));
		return;
	}  

	strValue.Format(_T("%d"),nValue);
	WritePrivateProfileString(_T("ʶ�����"),_T("�����������"),strValue,gstuPathInf.csPathIni);
	gstuRcgInfo.nAllowDefect = nValue;
	gclsImgRcg.stuRef.Dev=gstuRcgInfo.nAllowDefect;
}


void CCamera::OnBnClickedBtnGetset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//char szLan[128] = {0};
	//char szTemp[256] = {0};  
	//char szErrorMsg[256] = {0};
	//DWORD dwReturn;
	//if(!NET_GetConfig(glCamID,IPC_GET_SETTINGS_CAMERA_PARAM,&gstuCamParam, sizeof(gstuCamParam),&dwReturn))
	//{
	//	NET_GetErrorMsg(NET_GetLastError(), szErrorMsg);
	//	sprintf(szTemp, "%s %s", szLan, szErrorMsg);
	//	AfxMessageBox((CString)szTemp);
	//	return;
	//}
	////
	//g_dlgCamera->SetDlgItemInt(IDC_EDT_EV,gstuCamParam.maxexposuretime);
	//g_dlgCamera->SetDlgItemInt(IDC_EDT_GAIN,gstuCamParam.maxgain);
	//g_dlgCamera->SetDlgItemInt(IDC_EDT_BRIGHT,gstuCamParam.Brightness);
	//g_dlgCamera->SetDlgItemInt(IDC_EDT_CONTRAST,gstuCamParam.Contrast);
	//g_dlgCamera->SetDlgItemInt(IDC_EDT_SATURATION,gstuCamParam.Saturation);
	//g_dlgCamera->SetDlgItemInt(IDC_EDT_SHARPNESS,gstuCamParam.Sharpness);
	//g_dlgCamera->m_cmbAWB.SetCurSel(gstuCamParam.awb);
	//g_dlgCamera->m_cmbBl.SetCurSel(gstuCamParam.BackLight);
	int    val;
	CString s1;

	gclsCamera.GetParam(gnChannel, KSJ_EXPOSURE, &val);
	s1.Format(_T("%d"), val);
	((CEdit*)GetDlgItem(IDC_EDT_EV))->SetWindowText(s1);


	gclsCamera.GetParam(gnChannel, KSJ_RED, &val);
	s1.Format(_T("%d"), val);
	((CEdit*)GetDlgItem(IDC_EDT_GAIN))->SetWindowText(s1);


	gclsCamera.GetParam(gnChannel, KSJ_BRIGHTNESS, &val);
	s1.Format(_T("%d"), val);
	((CEdit*)GetDlgItem(IDC_EDT_BRIGHT))->SetWindowText(s1);


	gclsCamera.GetParam(gnChannel, KSJ_CONTRAST, &val);
	s1.Format(_T("%d"), val);
	((CEdit*)GetDlgItem(IDC_EDT_CONTRAST))->SetWindowText(s1);

	return;
}


void CCamera::OnBnClickedChkAutothrd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_AUTOTHRD);
	int nStat = pChk->GetCheck();
	if (nStat)
	{
		gstuRcgInfo.bThrdAuto = 1;
	} 
	else
	{
		gstuRcgInfo.bThrdAuto = 0;
	}
}


void CCamera::OnBnClickedChkDelnoise()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_DELNOISE);
	int nStat = pChk->GetCheck();
	if (nStat)
	{
		gstuRcgInfo.bDelNoise = 1;
	} 
	else
	{
		gstuRcgInfo.bDelNoise = 0;
	}
}


//void CCamera::OnBnClickedCheck3()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//}


void CCamera::OnBnClickedChkDebug()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_DEBUG);
	int nStat = pChk->GetCheck();
	if (nStat)
	{
		gstuRcgInfo.bDebug = 1;
	} 
	else
	{
		gstuRcgInfo.bDebug = 0;
	}
}

#include "DlgPriview1.h"
void CCamera::OnBnClickedBtnSplit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

  CDlgPriview1 dlg;
  dlg.DoModal();


	//
	//CString m_bmp_file=gstuPathInf.csPathExe + _T("\\PIC\\ԭͼ0.bmp");
	//
	//CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
	//BOOL ifFind = findini.FindFile(m_bmp_file);  
	//if( !ifFind )  
	//	return;
	//CBitmap m_bmp;//�������Ա
	//BITMAP bm;//���λͼ��Ϣ�Ľṹ
	//HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,m_bmp_file,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//����bitmapָ��
	//m_bmp.Attach(hBitmap1);//���������cbitmap����
	//m_bmp.GetBitmap(&bm);
	//CWnd * pWnd = GetDlgItem(IDC_PREVIEW);
	//CDC* pDC = pWnd->GetDC();
	//CRect rect;
	//GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
	//CDC memDC;        //����һ���豸
	//CClientDC dc1(this);      //��ȡ�ͻ�
	//memDC.CreateCompatibleDC( &dc1 );
	//memDC.SelectObject( m_bmp );  //Ϊ�豸ѡ�����

	//mDCMem->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	//pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);
	//memDC.DeleteDC();
	//m_bmp.DeleteObject();	
	//DeleteObject(hBitmap1);//�ǵ�ɾ��	
	//
	//CPen PenRed;
	//CGdiObject *o = NULL;
	//PenRed.CreatePen(PS_SOLID,1,RGB(255,0,0));//����һ֧��ɫ�Ļ���.
	////pWnd = GetDlgItem(IDC_image1); //IDC_STATIC��picture control��ID.����ǵõ�picture�ľ��.
	////pDC = pWnd->GetDC();//Ȼ��õ��豸����.
	////RECT rect;//����һ��rect
	////pWnd->GetClientRect(&rect);//����picture�ؼ���������rect����.
	//mDCMem->SelectObject(PenRed);//ѡ�л���.

	///*gstuRcgInfo.nPToL=2;//ֱ�ߵ��
	//gstuRcgInfo.nDefectPToL=10;//ȱ�ݵ��
	//gstuRcgInfo.nThreshold=30;//��ֵ*/
	////char * chPath = (LPSTR)(LPCTSTR)m_bmp_file;
	//char chPath[MAX_PATH];
	////WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_bmp_file,-1,readPath,CStringA(m_bmp_file).GetLength(),NULL,NULL); 
	//WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_bmp_file,-1,chPath,sizeof(chPath),NULL,NULL); 
	////
	//LARGE_INTEGER l_lgint_start, l_lgint_end;
	//LARGE_INTEGER l_lgint_freq;
	//QueryPerformanceFrequency(&l_lgint_freq);  
	//QueryPerformanceCounter(&l_lgint_start);
	////
	//gclsImgRcg.RCGBMPSPLIT(mDCMem,rect,chPath,gstuRcgInfo.nPToL,gstuRcgInfo.nDefectPToL,gstuRcgInfo.nThreshold,gstuRcgInfo.bDebug,gstuRcgInfo.bIsCir,gstuRcgInfo.bThrdAuto,gstuRcgInfo.bDelNoise);
	////
	//QueryPerformanceCounter(&l_lgint_end);
	//double dTmpRT=double(l_lgint_end.QuadPart-l_lgint_start.QuadPart)/double(l_lgint_freq.QuadPart);	
	//CString csTmp;
	//csTmp.Format(_T("%.2f��"),dTmpRT);
	//SetDlgItemText(IDC_SELECT_RESULT,csTmp);
	//	//
	//pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);
	//pWnd->ReleaseDC(pDC);
	//gstuRcgInfo.bClbPos=1;
}


void CCamera::OnBnClickedBtnReconnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//IIICStartConnect();201608
	gnChannel = gclsCamera.GetKsj(KSJ_UC500M_MRYY);
	if (0 > gnChannel)
		AfxMessageBox(_T("KSJ_UC500M_MRYY �����ʧ��	,�����Ƿ����Ӹ����"));
	
}
//
//void CCamera::IIICStartConnect()
//{
//	//
//	char sUserName[32];
//	char sPassword[16];
//	char sAddressIp[16];
//	DWORD dwPort;
//	
//	//
//
//	char szLan[128] = {0};
//	char szTemp[256] = {0};
//	CString csTemp;
//
//	if (glCamID >= 0)
//	{
//		MessageBox(_T("�����Ѿ�����!"));
//		return;
//	}
//	//test
//	strncpy(sUserName, "admin", 32);
//	strncpy(sPassword, "9999", 16);
//	strncpy(sAddressIp, "192.168.1.102", 16);
//	dwPort = 8000;
//	//end
//	DWORD dwReturn;
//	NET_GetConfig(1,IPC_GET_NETWORK_PARAM,&gstuCamNetCfg, sizeof(gstuCamNetCfg),&dwReturn);
//	//sprintf(sUserName,"%s.%s.%s.%s",gstuCamNetCfg.ip.S_un.S_addr[1]);
//
//	NET_SDK_DEVICEINFO m_struDeviceInfo;
//	memset(&m_struDeviceInfo, 0, sizeof(m_struDeviceInfo));
//
//	//NET_SetMessageCallBack(MessageCallback0,NULL);
//
//	//	NET_DVR_SetLogToFile(3, NULL, FALSE);
//	//	NET_SetLogToFile(1,);//SDK��־����δʵ��
//
//
//	NET_SetConnectTime(3000,3);
//	NET_SetReconnect(3000,TRUE);
//	glCamID = NET_Login(sAddressIp, (WORD)dwPort, sUserName, sPassword, NULL);
//
//
//
//	if (glCamID < 0)
//		AfxMessageBox(_T("����IIIC���ʧ�ܣ�"));
//	else if(gclsIiic.OpenRtsp()!=1)
//		AfxMessageBox(_T("RTSP FAIL"));
//	/*int connectCNT= 0;
//	while(glCamID < 0 )
//	{
//		glCamID = NET_Login(sAddressIp, (WORD)dwPort, sUserName, sPassword, NULL);
//		connectCNT ++;
//		if (connectCNT >10000)
//		{
//			AfxMessageBox(_T("����IIIC���ʧ�ܣ�"));
//			OnClose();
//			break;
//		}
//	}*/
//
//
//}


void CCamera::OnBnClickedBtnParmsend()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������

  //m_clked_pos_x = X;
  //m_clked_pos_y = Y;m_par
  if (0 == g_dlgDevice->ParamMove(m_par.x, m_par.y, m_par.deg, m_par.pn))
  {
    AfxMessageBox(_T("���ͳɹ�"));
  }
  else
  {
    AfxMessageBox(_T("status is not ready"));
  }

}


void CCamera::OnBnClickedBtnAuto()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������

  CString str;
  GetDlgItemText(IDC_BTN_AUTO, str);
  ((CButton*)GetDlgItem(IDC_BTN_AUTO))->EnableWindow(FALSE);
  if ("�Զ�" == str)
  {
    if (!gb_PlayOrNot[1])
    {
      gb_PlayOrNot[1] = 1;
      gclsCamera.Preview(gnChannel, GetDlgItem(IDC_PREVIEW));
    }

    gstuRun.unStatRun = RUN_STAT_RUN;
    gTrdAuto = AfxBeginThread(Thread_Auto, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
    gTrdAuto->m_bAutoDelete = TRUE;
    gTrdAuto->ResumeThread();
    SetDlgItemText(IDC_BTN_AUTO, _T("�Զ���"));
    //

    GetDlgItem(IDC_BTN_VIDEO)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_SNAP)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_TEST)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_PARMSEND)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_CALIBRATION)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_RECONNECT)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_SPLIT)->EnableWindow(FALSE);
    //
  }
  else
  {
    if (gb_PlayOrNot[1])
    {
      gb_PlayOrNot[1] = 0;
      gclsCamera.StopPreview(gnChannel);  //ֹͣԤ��
    }
    gstuRun.unStatRun = RUN_STAT_STOP;
    SetDlgItemText(IDC_BTN_AUTO, _T("�Զ�"));

    //

    GetDlgItem(IDC_BTN_VIDEO)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_SNAP)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_PARMSEND)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_CALIBRATION)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_RECONNECT)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_SPLIT)->EnableWindow(TRUE);

    //

  }
}
