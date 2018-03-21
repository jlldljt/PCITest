#pragma once

#include "AVerCapAPI_Basic.h"

class CapControl
{
public:
	CapControl();
	~CapControl();
	BOOL   m_bIsStartStreaming;
  BOOL   m_bInit;
  BYTE* m_bmp;
  LONG m_bmpSize;
  CString m_capName;
public:
	bool init(HWND hwnd);

public:
	static BOOL WINAPI NotifyEventCallback(DWORD dwEventCode, LPVOID lpEventData, LPVOID lpUserData);

	DWORD  m_dwDeviceNum;
	DWORD  m_dwCurrentCardType;
	void OnSelectCaptureDevice(UINT nID, HWND hwnd);
	void DeleteCaptureDevice();
	void Startstreaming();
	void Stopstreaming();

	void CaptureSingle(CString csFilePath);

  void RePaint(void);
  void SetVideoWindowPosition(RECT rc);
  BOOL SaveImageData(CString sFile);
  void CaptureSingleBuffer();
private:
	HWND m_hwnd;
	
	BOOL   m_bHadSetVideoRenderer;
	HANDLE m_hCaptureDevice;
	CAPTURE_SINGLE_IMAGE_INFO m_CaptureSingleInfo;
};

