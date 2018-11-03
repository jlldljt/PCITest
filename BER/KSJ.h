#pragma once



#pragma comment(lib,"KSJApi.lib\\KSJApi.lib")
#include "KSJApi.Inc\KSJApi.h"

#define MAX_DEVICE    32

namespace KSJ_NAME_SPACE {

	

	

	class CKSJ
	{
	public:
		CKSJ();
		~CKSJ();

		struct DEVICEINFO
		{
			KSJ_DEVICETYPE		DeviceType;
			int					nIndex;
			WORD				wFirmwareVersion;
		};

		
		DEVICEINFO    m_DeviceInfo[MAX_DEVICE];//已连接的相机的信息
		int m_nDeviceCount;//已连接的相机数
		unsigned char *m_pData;

	public:
		int GetKsj(KSJ_DEVICETYPE deviceType);
		CString GetType(int nChannel);
		int GetParamRange(int nChannel, KSJ_PARAM Param, int *pnMinValue, int *pnMaxValue);
		unsigned char *Snap(int nChannel, int *pnWidth, int *pnHeight, int *pnBitCount, int *nBitsPerSample);
		unsigned char *SnapEx(int nChannel, int *pnWidth, int *pnHeight, int *pnBitCount, int *nBitsPerSample, const char *pszFileName, CWnd *pWnd);
		int SetParam(int nChannel, KSJ_PARAM Param, int nValue);
		int GetParam(int nChannel, KSJ_PARAM Param, int *pnValue);
		int GetFrameRate(int nChannel, float *fFrameRate);
		int SaveToBmp(unsigned char *pData, int nWidth, int nHeight, int nBitCount, const TCHAR *pszFileName);
		int Preview(int nChannel, CWnd *pWnd);
		int StopPreview(int nChannel);

	private:
		int  DrawBMP(CWnd * pWnd, const char *fileName);

	};

}

