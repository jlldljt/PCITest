#include "stdafx.h"
#include "KSJ.h"

using namespace KSJ_NAME_SPACE;
const TCHAR    *g_szDeviceType[] =
{

	_T("UC130C(MRNN)"),
	_T("UC130M(MRNN)"),
	_T("UC200C(MRNN)"),
	_T("UC320C(MRNN)"),
	_T("UC130C(MRYN)"),
	_T("UC130M(MRYN)"),
	_T("UC200C(MRYN)"),
	_T("UC320C(MRYN)"),
	_T("UC500C(MRNN)"),
	_T("UC500M(MRNN)"),
	_T("UC500C(MRYN)"),
	_T("UC500M(MRYN)"),
	_T("UC320C(OCR)"),
	_T("UC900C(MRNN)"),
	_T("UC1000C(MRNN)"),
	_T("UC900C(MRYN)"),
	_T("UC1000C(MRYN)"),
	_T("UC130C(MRYY)"),
	_T("UC130M(MRYY)"),
	_T("UD140C(SRNN)"),
	_T("UD140M(SRNN)"),
	_T("UC36C(MGNN)"),
	_T("UC36M(MGNN)"),
	_T("UC36C(MGYN)"),
	_T("UC36M(MGYN)"),
	_T("UC900C(MRYY)"),
	_T("UC1000C(MRYY)"),
	_T("UC1400C(MRYY)"),
	_T("UC36C(MGYY)"),
	_T("UC36M(MGYY)"),
	_T("UC320C(MRYY)"),
	_T("UC500C(MRYY)"),
	_T("UC500M(MRYY)"),
	_T("MiniUC130C(MRYN)"),
	_T("MiniUC130M(MRYN)"),
	_T("MiniUC320C(MRYN)"),
	_T("MUC36C(MGYY)"),
	_T("MUC36M(MGYY)"),
	_T("MUC130C(MRYY)"),
	_T("MUC130M(MRYY)"),
	_T("MUC320C(MRYY)"),
	_T("MUC500C(MRYY)"),
	_T("MUC500M(MRYY)"),
	_T("MUC900C(MRYY)"),
	_T("MUC1000C(MRYY)"),
	_T("MUC1400C(MRYY)"),
	_T("UD205C(SGYY)"),
	_T("UD205M(SGYY)"),
	_T("UD274C(SGYY)"),
	_T("UD274M(SGYY)"),
	_T("UD285C(SGYY)"),
	_T("UD285M(SGYY)"),
	_T("UNKOWN TYPE"),
	_T("UNKOWN TYPE"),
	_T("UNKOWN TYPE"),
	_T("UNKOWN TYPE"),
	_T("UNKOWN TYPE"),
	_T("UNKOWN TYPE"),
	_T("UNKOWN TYPE"),
};
CKSJ::CKSJ()
{
	m_nDeviceCount = 0;
	m_pData = nullptr;

	int nRet = KSJ_Init();
	TCHAR    szBuf[256] = { '\0' };
	KSJ_GetErrorInfo(nRet, szBuf);
}


CKSJ::~CKSJ()
{
	for (int i = 0; i < m_nDeviceCount; i++)
	{
		KSJ_PreviewStart(i, FALSE);
	}
	
	KSJ_UnInit();
	if (nullptr != m_pData) {
		delete[] m_pData;
		m_pData = nullptr;
	}
}

int CKSJ::GetKsj(KSJ_DEVICETYPE deviceType)
{
	

	int nDeviceCount = KSJ_DeviceGetCount();
	m_nDeviceCount = nDeviceCount;
	if (nDeviceCount == 0)
	{
		AfxMessageBox(_T("No Device Connected!"));

		return -1;
	}
	if (nDeviceCount >= MAX_DEVICE)    nDeviceCount = MAX_DEVICE;
	// TODO:  在此添加额外的初始化
	//int i = 0;
	//int nWidth;
	//int nHeight;
	//int nChannel;

	for (int i = 0; i<nDeviceCount; ++i)
	{
		KSJ_EmptyFrameBuffer(i);//对各个通道清空
		KSJ_DeviceGetInformation(i, &(m_DeviceInfo[i].DeviceType), &(m_DeviceInfo[i].nIndex), &(m_DeviceInfo[i].wFirmwareVersion));

		//	_stprintf( szMenuItem, _T("%s 序号(%d) 固件版本(%d.%d)"), g_szDeviceType[m_DeviceInfo[i].DeviceType], m_DeviceInfo[i].nIndex, btMajVersion, btMinVersion );
		KSJ_DEVICETYPE type = m_DeviceInfo[i].DeviceType;
		//nChannel = i;
		//if (type == deviceType)
		{
			return i;
		}
	}
	return -1;
}

CString KSJ_NAME_SPACE::CKSJ::GetType(int nChannel)
{
	if (nChannel >= m_nDeviceCount)
		return NULL;
	return CString(g_szDeviceType[m_DeviceInfo[nChannel].DeviceType]);
}

int KSJ_NAME_SPACE::CKSJ::GetParamRange(int nChannel, KSJ_PARAM Param, int * pnMinValue, int * pnMaxValue)
{
	return KSJ_GetParamRange(nChannel, Param, pnMinValue, pnMaxValue);
}

unsigned char * KSJ_NAME_SPACE::CKSJ::Snap(int nChannel, int * pnWidth, int * pnHeight, int * pnBitCount, int * nBitsPerSample)
{
	if(nChannel >= m_nDeviceCount)
		return nullptr;
	KSJ_CaptureGetSizeExEx(nChannel, pnWidth, pnHeight, pnBitCount, nBitsPerSample);

	if (8 != *pnBitCount || 8 != *nBitsPerSample)
		return nullptr;

	if (nullptr != m_pData) {
		delete[] m_pData;
		m_pData = nullptr;
	}

	m_pData = new unsigned char[*pnWidth * *pnHeight];

	//KSJ_BAYERMODE bayerMode;
	//KSJ_BayerGetDefaultMode(m_nChannel, &bayerMode);//KSJ_RGGB_BGR24
	//KSJ_BayerSetMode(m_nChannel, KSJ_RGGB_BGR24);//KSJ_RGGB_BGR24
	//KSJ_CaptureRgbDataEx(m_nChannel, pData, &nWidth, &nHeight, &nBitCount);

	KSJ_CaptureRawData(nChannel, m_pData);
	return m_pData;
}

unsigned char * KSJ_NAME_SPACE::CKSJ::SnapEx(int nChannel, int * pnWidth, int * pnHeight, int * pnBitCount, int * nBitsPerSample, const char * pszFileName, CWnd * pWnd)
{
	if (nChannel >= m_nDeviceCount)
		return nullptr;
	KSJ_CaptureGetSizeExEx(nChannel, pnWidth, pnHeight, pnBitCount, nBitsPerSample);

	if (8 != *pnBitCount || 8 != *nBitsPerSample)
		return nullptr;

	if (nullptr != m_pData) {
		delete[] m_pData;
		m_pData = nullptr;
	}

	m_pData = new unsigned char[*pnWidth * *pnHeight];

	KSJ_CaptureRawData(nChannel, m_pData);
	if (nullptr != pszFileName)
	{
		int ret = SaveToBmp(m_pData, *pnWidth, *pnHeight, *pnBitCount, (TCHAR*)pszFileName);

		if (NULL != pWnd)
		{
			DrawBMP(pWnd, pszFileName);
		}
	}

	return m_pData;
}

int KSJ_NAME_SPACE::CKSJ::SetParam(int nChannel, KSJ_PARAM Param, int nValue)
{
	return KSJ_SetParam(nChannel, Param, nValue);
}

int KSJ_NAME_SPACE::CKSJ::GetParam(int nChannel, KSJ_PARAM Param, int * pnValue)
{
	return KSJ_GetParam(nChannel, Param, pnValue);
}

int KSJ_NAME_SPACE::CKSJ::GetFrameRate(int nChannel, float * fFrameRate)
{
	return KSJ_PreviewGetFrameRate(nChannel, fFrameRate);
}

int KSJ_NAME_SPACE::CKSJ::SaveToBmp(unsigned char * pData, int nWidth, int nHeight, int nBitCount, const TCHAR * pszFileName)
{
	return KSJ_HelperSaveToBmp(pData, nWidth, nHeight, nBitCount, pszFileName);
}

int KSJ_NAME_SPACE::CKSJ::Preview(int nChannel, CWnd *pWnd)
{
	if (nChannel >= m_nDeviceCount)
		return -1;
	int nWidth, nHeight;
	RECT    rectPreview;
	//	KSJ_PreviewSetCallbackEx(m_nChannel, PREVIEWCALLBACKEX, NULL);
	pWnd->GetClientRect(&rectPreview);
	KSJ_PreviewSetMode(nChannel, PM_RAWDATA);
	KSJ_PreviewGetSize(nChannel, &nWidth, &nHeight);
	KSJ_PreviewSetFieldOfView(nChannel, 0, 0, nWidth, nHeight, KSJ_SKIPNONE, KSJ_SKIPNONE);
	KSJ_PreviewSetPos(nChannel, pWnd->m_hWnd, 0, 0, rectPreview.right, rectPreview.bottom);
	KSJ_PreviewStart(nChannel, true);  //相机1 启动预览
	return 0;
}

int KSJ_NAME_SPACE::CKSJ::StopPreview(int nChannel)
{
	if (nChannel >= m_nDeviceCount)
		return -1;
	//	KSJ_PreviewSetCallbackEx(m_nChannel, NULL, NULL);
	KSJ_PreviewStart(nChannel, false);  //停止预览
	return 0;
}

int KSJ_NAME_SPACE::CKSJ::DrawBMP(CWnd * pWnd, const char * fileName)
{
	CFileFind findini;   //查找是否存在jpg文件
	CString csPathTmp(fileName);
	BOOL ifFind = findini.FindFile(csPathTmp);
	if (!ifFind)
		return 0;
	CBitmap m_bmp;//创建类成员
	BITMAP bm;//存放位图信息的结构
	char chPath[MAX_PATH];
	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, csPathTmp, -1, chPath, sizeof(chPath), NULL, NULL);
	HBITMAP hBitmap1 = (HBITMAP)LoadImageA(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);//创建bitmap指针
	m_bmp.Attach(hBitmap1);
	m_bmp.GetBitmap(&bm);
	CDC* pDC = pWnd->GetDC();
	CRect rect;
	pWnd->GetClientRect(&rect);
	CDC memDC, tmpDC;        //定义一个设备
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(m_bmp);  //为设备选择对象
	pDC->SetStretchBltMode(HALFTONE);
	SetBrushOrgEx(pDC->m_hDC, 0, 0, NULL);
	CBitmap MemBitmap;
	tmpDC.CreateCompatibleDC(pDC);
	MemBitmap.CreateCompatibleBitmap(pDC, rect.right, rect.bottom);
	tmpDC.SelectObject(&MemBitmap);
	tmpDC.SetStretchBltMode(HALFTONE);
	SetBrushOrgEx(tmpDC.m_hDC, 0, 0, NULL);
	if (rect.Width() / rect.Height() > bm.bmWidth / bm.bmHeight)
	{
		tmpDC.FillRect(&rect, &CBrush(RGB(0, 0, 0)));
		tmpDC.StretchBlt((rect.Width() - rect.Height()*bm.bmWidth / bm.bmHeight) / 2, 0, rect.Height()*bm.bmWidth / bm.bmHeight, rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	}
	else
	{
		tmpDC.FillRect(&rect, &CBrush(RGB(0, 0, 0)));
		tmpDC.StretchBlt(0, (rect.Height() - rect.Width()*bm.bmHeight / bm.bmWidth) / 2, rect.Width(), rect.Width()*bm.bmHeight / bm.bmWidth, &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	}
	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &tmpDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
	//pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	MemBitmap.DeleteObject();
	tmpDC.DeleteDC();
	memDC.DeleteDC();
	m_bmp.DeleteObject();
	DeleteObject(hBitmap1);//记得删除
						   //pWnd->ReleaseDC(pDC);
						   //FreePicture();
	return 1;
}

//图像上下反转
//输入pBmpBuf，图像宽度和高度
//输出：pBmpBuf_temp
void read_reversal(unsigned char * pBmpBuf, unsigned char * &pBmpBuf_temp, int bmpWidth, int bmpHeight, int biBitCount)
{


	int temp_rgb = 0;
	int i, j;
	int lineByte = (bmpWidth*biBitCount / 8 + 3) / 4 * 4;

	for (i = 0; i <= bmpHeight - 1; i++)
	{
		for (j = 0; j <= bmpWidth - 1; j++)
		{
			//for(k=0;k<3;k++)//每像素RGB三个分量分别置0才变成黑色      
			//	*(pBmpBuf+i*lineByte+j*3+k)=0;

			temp_rgb = *(pBmpBuf + i*bmpWidth + j);
			*(pBmpBuf_temp + (bmpHeight - 1 - i)*bmpWidth + j) = temp_rgb;
		}
	}

}