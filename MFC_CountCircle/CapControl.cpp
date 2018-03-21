#include "stdafx.h"
#include "CapControl.h"

CapControl::CapControl() :m_bIsStartStreaming(0), m_bInit(0)
{
  m_capName = "";
  m_bmp = NULL;
}


CapControl::~CapControl()
{
  delete[]m_bmp;
  m_bmp = NULL;
}

bool CapControl::init(HWND hwnd)
{
	if (AVerGetDeviceNum(&m_dwDeviceNum) != CAP_EC_SUCCESS)
	{
		MessageBox(NULL, _T("请确认圆刚 SDK和卡"), _T("HDCap Demo AP"), MB_OK);
		return false;
	}
	if (m_dwDeviceNum == 0)
	{
    MessageBox(NULL, _T("没有找到圆刚卡"), _T("HDCap Demo AP"), MB_OK);
		return false;
	}

	DWORD dwDeviceIndex = 0;
	WCHAR wszDeviceName[MAX_PATH] = { 0 };
	if (AVerGetDeviceName(dwDeviceIndex, wszDeviceName) != CAP_EC_SUCCESS)
	{
		AfxMessageBox(_T("Can't get the name of device."));
		return false;
	}
	AVerGetDeviceType(dwDeviceIndex, &m_dwCurrentCardType);

	m_hwnd = hwnd;
	OnSelectCaptureDevice(dwDeviceIndex, hwnd);

	AVerSetVideoSource(m_hCaptureDevice, VIDEOSOURCE_COMPOSITE);

	AVerSetAudioSource(m_hCaptureDevice, VIDEOSOURCE_COMPOSITE);

  AVerSetDeinterlaceMode(m_hCaptureDevice, DEINTERLACE_NONE);

	VIDEO_RESOLUTION VideoResolution = { 0 };
	VideoResolution.dwVersion = 1;
	VideoResolution.dwVideoResolution = 0;
	VideoResolution.dwWidth = 0;
	VideoResolution.dwHeight = 0;
	VideoResolution.bCustom = FALSE;

	AVerSetVideoResolutionEx(m_hCaptureDevice, &VideoResolution);
  m_capName = wszDeviceName;
  m_bInit = true;
	return true;
}

void CapControl::OnSelectCaptureDevice(UINT nID, HWND hwnd)
{
	if (m_hCaptureDevice)
	{
		DeleteCaptureDevice();
	}

	LONG lRetVal;

	lRetVal = AVerCreateCaptureObjectEx(nID, DEVICETYPE_HD, hwnd, &m_hCaptureDevice);
	
	switch (lRetVal)
	{
	case CAP_EC_SUCCESS:
		break;
	case CAP_EC_DEVICE_IN_USE:
		AfxMessageBox(_T("The capture device has already been used."));
		return;
		break;
	default:
		AfxMessageBox(_T("Can't initialize the capture device."));
		return;
		break;
	}
	
	AVerGetDeviceType(nID, &m_dwCurrentCardType);
}

void CapControl::DeleteCaptureDevice()
{
	if (m_bIsStartStreaming)
	{
		AVerStopStreaming(m_hCaptureDevice);
		m_bIsStartStreaming = FALSE;
	}
	AVerDeleteCaptureObject(m_hCaptureDevice);
	m_hCaptureDevice = NULL;
  m_capName = "";
}

BOOL WINAPI CapControl::NotifyEventCallback(DWORD dwEventCode, LPVOID lpEventData, LPVOID lpUserData)
{
	switch (dwEventCode)
	{
	case EVENT_CHECKCOPP:
	{
		LONG *plErrorID = (LONG*)lpEventData;
		CString strErrorID;
		switch (*plErrorID)
		{
		case COPP_ERR_UNKNOWN:
			strErrorID = "COPP_ERR_UNKNOWN";
			break;
		case COPP_ERR_NO_COPP_HW:
			strErrorID = "COPP_ERR_NO_COPP_HW";
			break;
		case COPP_ERR_NO_MONITORS_CORRESPOND_TO_DISPLAY_DEVICE:
			strErrorID = "COPP_ERR_NO_MONITORS_CORRESPOND_TO_DISPLAY_DEVICE";
			break;
		case COPP_ERR_CERTIFICATE_CHAIN_FAILED:
			strErrorID = "COPP_ERR_CERTIFICATE_CHAIN_FAILED";
			break;
		case COPP_ERR_STATUS_LINK_LOST:
			strErrorID = "COPP_ERR_STATUS_LINK_LOST";
			break;
		case COPP_ERR_NO_HDCP_PROTECTION_TYPE:
			strErrorID = "COPP_ERR_NO_HDCP_PROTECTION_TYPE";
			break;
		case COPP_ERR_HDCP_REPEATER:
			strErrorID = "COPP_ERR_HDCP_REPEATER";
			break;
		case COPP_ERR_HDCP_PROTECTED_CONTENT:
			strErrorID = "COPP_ERR_HDCP_PROTECTED_CONTENT";
			break;
		case COPP_ERR_GET_CRL_FAILED:
			strErrorID = "COPP_ERR_GET_CRL_FAILED";
			break;
		}
		::MessageBox(NULL, strErrorID.GetBuffer(0), _T("AVerCapSDKDemoAP"), MB_OK);
	}
	break;
	default:
		return FALSE;
	}
	return TRUE;
}

void CapControl::Startstreaming()
{
  if (!m_bInit) {
    AfxMessageBox(_T("采集卡未初始化！"));
    return;
  }

	if (!m_bHadSetVideoRenderer)
	{
		AVerSetVideoRenderer(m_hCaptureDevice, VIDEORENDERER_EVR);
	}
	AVerSetEventCallback(m_hCaptureDevice, NotifyEventCallback, 0, this);
	if (m_hwnd)
	{
		RECT rectClient;
		GetClientRect(m_hwnd, &rectClient);
		AVerSetVideoWindowPosition(m_hCaptureDevice, rectClient);
	}
	if (AVerStartStreaming(m_hCaptureDevice) != CAP_EC_SUCCESS)
	{
		AfxMessageBox(_T("Can't start streaming."));
		return;
	}

	m_bIsStartStreaming = TRUE;
}
void CapControl::Stopstreaming()
{
  if (!m_bIsStartStreaming) {
    AfxMessageBox(_T("视频未开启！"));
    return;
  }
	AVerStopStreaming(m_hCaptureDevice);
	m_bIsStartStreaming = FALSE;
	AVerSetEventCallback(m_hCaptureDevice, NULL, 0, this);
	//InvalidateRect(m_hwnd, NULL, TRUE);
}
//bmp
void CapControl::CaptureSingle(CString csFilePath)
{
	//WCHAR wszFilePath[MAX_PATH] = { 0 };
  if (!m_bIsStartStreaming) {
    AfxMessageBox(_T("视频未开启！"));
    return;
  }
	HANDLE hTempFile = CreateFile(csFilePath, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_FLAG_DELETE_ON_CLOSE, NULL);
	if (hTempFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("Invalid path."));
		return;
	}
	CloseHandle(hTempFile);

	m_CaptureSingleInfo.dwVersion = 1;
	m_CaptureSingleInfo.bOverlayMix = FALSE;
	m_CaptureSingleInfo.lpFileName = (LPWSTR)(LPCTSTR)csFilePath;
	m_CaptureSingleInfo.dwImageType = IMAGETYPE_BMP;
  //m_CaptureSingleInfo.rcCapRect = { 0,0,0,0 };
  m_CaptureSingleInfo.rcCapRect.bottom = 0;
   m_CaptureSingleInfo.rcCapRect.left =0 ;
   m_CaptureSingleInfo.rcCapRect.right= 0;
    m_CaptureSingleInfo.rcCapRect.top= 0;
	LONG lRetVal = AVerCaptureSingleImage(m_hCaptureDevice, &m_CaptureSingleInfo);
	if (lRetVal != 0)
	{
		if (lRetVal == CAP_EC_HDCP_PROTECTED_CONTENT)
		{
			AfxMessageBox(_T("The program content is protected!"));
			return;
		}
		AfxMessageBox(_T("The width and height of capture area are not a multiple of 2 or out of range."));
		return;
	}
  return;
}

void CapControl::RePaint(void)
{
  if (m_bIsStartStreaming)
    LONG lRetVal = AVerRepaintVideo(m_hCaptureDevice);
}

void CapControl::SetVideoWindowPosition(RECT rc)
{
  if (m_bIsStartStreaming)
    LONG lRetVal = AVerSetVideoWindowPosition(m_hCaptureDevice, rc);
}



BOOL CapControl::SaveImageData(CString sFile)
{
  if (!m_bmp || !m_bmpSize)
  {
    return FALSE;
  }
  CString sPathName;
  sPathName = sFile;
  CFile file;
  CFileException e;
  if (file.Open(sPathName, CFile::modeCreate | CFile::modeWrite, &e))
  {
    file.Write(m_bmp, m_bmpSize);//把pDib写入到文件中，长度为dwDibSize
    file.Close();
  }
  else
  {
    return FALSE;
  }
  return TRUE;
}

void CapControl::CaptureSingleBuffer()
{

  LONG lRetVal = AVerCaptureSingleImageToBuffer(m_hCaptureDevice, NULL, &m_bmpSize, 0, 100);
  if (m_bmpSize<0)
  {
	  return;
  }
  if (m_bmp)
  {
    delete[]m_bmp;
    m_bmp = NULL;
  }

  m_bmp = new BYTE[m_bmpSize];
  lRetVal = AVerCaptureSingleImageToBuffer(m_hCaptureDevice, m_bmp, &m_bmpSize, 0, 100);

  if (lRetVal != CAP_EC_SUCCESS)
  {
    if (lRetVal == CAP_EC_NOT_ENOUGH_MEMORY)
    {
      AfxMessageBox(_T("not enough memory"));
      return;
    }
    AfxMessageBox(_T("capture failure"));
    return;
  }
}