#include "stdafx.h"
#include "AVerCapAPI_Basic.h"

class DLLControl
{
public:
	HINSTANCE m_hdll;
	DLLControl();
	~DLLControl();
};
DLLControl::DLLControl():
m_hdll(NULL)
{
	m_hdll=LoadLibrary(_T("AVerCapAPI.dll"));
	if(m_hdll == NULL)  
	{  
		FreeLibrary(m_hdll);    
	} 
}
DLLControl::~DLLControl()
{
	HRESULT hr=FreeLibrary(m_hdll); 
}
DLLControl g_Dllcontrol;
//Device Control
typedef LONG (WINAPI *DLL_AVerGetDeviceNum)(DWORD*);
LONG WINAPI AVerGetDeviceNum(DWORD *pdwDeviceNum)
{
	DLL_AVerGetDeviceNum dll_Fun;
	dll_Fun=(DLL_AVerGetDeviceNum)GetProcAddress(g_Dllcontrol.m_hdll,"AVerGetDeviceNum");
	if(dll_Fun==NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr=dll_Fun(pdwDeviceNum);
	return hr;
}

typedef LONG (WINAPI *DLL_AVerGetDeviceType)(DWORD,DWORD*);
LONG WINAPI AVerGetDeviceType(DWORD dwDeviceIndex, DWORD *pdwDeviceType)
{
	DLL_AVerGetDeviceType dll_Fun;
	dll_Fun=(DLL_AVerGetDeviceType)GetProcAddress(g_Dllcontrol.m_hdll,"AVerGetDeviceType");
	if(dll_Fun==NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr=dll_Fun(dwDeviceIndex,pdwDeviceType);
	return hr;
}

typedef LONG (WINAPI *DLL_AVerGetDeviceName)(DWORD,LPWSTR);
LONG WINAPI AVerGetDeviceName(DWORD dwDeviceIndex, LPWSTR szDeviceName)
{
	DLL_AVerGetDeviceName dll_Fun;
	dll_Fun=(DLL_AVerGetDeviceName)GetProcAddress(g_Dllcontrol.m_hdll,"AVerGetDeviceName");
	if(dll_Fun==NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr=dll_Fun(dwDeviceIndex,szDeviceName);
	return hr;
}

typedef LONG (WINAPI *DLL_AVerGetDeviceSerialNum)(DWORD,BYTE *);
LONG WINAPI AVerGetDeviceSerialNum(DWORD dwDeviceIndex, BYTE *pbySerialNum)
{
	DLL_AVerGetDeviceSerialNum dll_Fun;
	dll_Fun=(DLL_AVerGetDeviceSerialNum)GetProcAddress(g_Dllcontrol.m_hdll,"AVerGetDeviceSerialNum");
	if(dll_Fun==NULL)
	{

		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr=dll_Fun(dwDeviceIndex,pbySerialNum);
	return hr;
}

typedef LONG (WINAPI *DLL_AVerCreateCaptureObject)(DWORD,HWND,HANDLE *);
LONG WINAPI AVerCreateCaptureObject(DWORD dwDeviceIndex, HWND hWnd, HANDLE *phCaptureObject)
{
	DLL_AVerCreateCaptureObject dll_Fun;
	dll_Fun=(DLL_AVerCreateCaptureObject)GetProcAddress(g_Dllcontrol.m_hdll,"AVerCreateCaptureObject");
	if(dll_Fun==NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr=dll_Fun(dwDeviceIndex,hWnd,phCaptureObject);
	return hr;
}

typedef LONG (WINAPI *DLL_AVerCreateCaptureObjectEx)(DWORD,DWORD,HWND,HANDLE*);
LONG WINAPI AVerCreateCaptureObjectEx(DWORD dwDeviceIndex, DWORD dwType, HWND hWnd, HANDLE *phCaptureObject)
{
	DLL_AVerCreateCaptureObjectEx dll_Fun;
	dll_Fun=(DLL_AVerCreateCaptureObjectEx)GetProcAddress(g_Dllcontrol.m_hdll,"AVerCreateCaptureObjectEx");
	if(dll_Fun==NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr=dll_Fun(dwDeviceIndex,dwType,hWnd,phCaptureObject);
	return hr;
}

typedef LONG (WINAPI *DLL_AVerDeleteCaptureObject)(HANDLE);
LONG WINAPI AVerDeleteCaptureObject(HANDLE hCaptureObject)
{
	DLL_AVerDeleteCaptureObject dll_Fun;
	dll_Fun=(DLL_AVerDeleteCaptureObject)GetProcAddress(g_Dllcontrol.m_hdll,"AVerDeleteCaptureObject");
	if(dll_Fun==NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr=dll_Fun(hCaptureObject);
	return hr;
}

typedef LONG (WINAPI *DLL_AVerGetVideoSourceSupported)(HANDLE,DWORD*,DWORD*);
LONG WINAPI AVerGetVideoSourceSupported(HANDLE hCaptureObject,DWORD *pdwSupported, DWORD *pdwNum)
{
	DLL_AVerGetVideoSourceSupported dll_Fun;
	dll_Fun=(DLL_AVerGetVideoSourceSupported)GetProcAddress(g_Dllcontrol.m_hdll,"AVerGetVideoSourceSupported");
	if(dll_Fun==NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr=dll_Fun(hCaptureObject,pdwSupported,pdwNum);
	return hr;
}

typedef LONG (WINAPI *DLL_AVerSetVideoSource)(HANDLE,DWORD);
LONG WINAPI AVerSetVideoSource(HANDLE hCaptureObject, DWORD dwVideoSource)
{
	DLL_AVerSetVideoSource dll_Fun;
	dll_Fun=(DLL_AVerSetVideoSource)GetProcAddress(g_Dllcontrol.m_hdll,"AVerSetVideoSource");
	if(dll_Fun==NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr=dll_Fun(hCaptureObject,dwVideoSource);
	return hr;
}

typedef LONG (WINAPI *DLL_AVerGetVideoSource)(HANDLE,DWORD *);
LONG WINAPI AVerGetVideoSource(HANDLE hCaptureObject, DWORD *pdwVideoSource)
{
	DLL_AVerGetVideoSource dll_Fun;
	dll_Fun=(DLL_AVerGetVideoSource)GetProcAddress(g_Dllcontrol.m_hdll,"AVerGetVideoSource");
	if(dll_Fun==NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr=dll_Fun(hCaptureObject,pdwVideoSource);
	return hr;
}

typedef LONG (WINAPI *DLL_AVerGetAudioSourceSupportedEx)(HANDLE,DWORD,DWORD *);
LONG WINAPI AVerGetAudioSourceSupportedEx(HANDLE hCaptureObject, DWORD dwVideoSource, DWORD *pdwSupported)
{
	DLL_AVerGetAudioSourceSupportedEx dll_Fun;
	dll_Fun=(DLL_AVerGetAudioSourceSupportedEx)GetProcAddress(g_Dllcontrol.m_hdll,"AVerGetAudioSourceSupportedEx");
	if(dll_Fun==NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr=dll_Fun(hCaptureObject,dwVideoSource,pdwSupported);
	return hr;
}

typedef LONG (WINAPI *DLL_AVerSetAudioSource)(HANDLE,DWORD);
LONG WINAPI AVerSetAudioSource(HANDLE hCaptureObject, DWORD dwAudioSource)
{
	DLL_AVerSetAudioSource dll_Fun;
	dll_Fun=(DLL_AVerSetAudioSource)GetProcAddress(g_Dllcontrol.m_hdll,"AVerSetAudioSource");
	if(dll_Fun==NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr=dll_Fun(hCaptureObject,dwAudioSource);
	return hr;
}
typedef LONG (WINAPI *DLL_AVerGetAudioSource)(HANDLE,DWORD *);
LONG WINAPI AVerGetAudioSource(HANDLE hCaptureObject, DWORD *pdwAudioSource)
{
	DLL_AVerGetAudioSource dll_Fun;
	dll_Fun=(DLL_AVerGetAudioSource)GetProcAddress(g_Dllcontrol.m_hdll,"AVerGetAudioSource");
	if(dll_Fun==NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr=dll_Fun(hCaptureObject,pdwAudioSource);
	return hr;
}

typedef LONG (WINAPI *DLL_AVerSetVideoFormat)(HANDLE,DWORD);
LONG WINAPI AVerSetVideoFormat(HANDLE hCaptureObject, DWORD dwVideoFormat)
{
	DLL_AVerSetVideoFormat dll_Fun;
	dll_Fun=(DLL_AVerSetVideoFormat)GetProcAddress(g_Dllcontrol.m_hdll,"AVerSetVideoFormat");
	if(dll_Fun==NULL)
	{ 
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr=dll_Fun(hCaptureObject,dwVideoFormat);
	return hr;
}

typedef LONG (WINAPI *DLL_AVerGetVideoFormat)(HANDLE,DWORD*);
LONG WINAPI AVerGetVideoFormat(HANDLE hCaptureObject, DWORD *pdwVideoFormat)
{
	DLL_AVerGetVideoFormat dll_Fun;
	dll_Fun = (DLL_AVerGetVideoFormat)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetVideoFormat");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pdwVideoFormat);
	return hr;
}
typedef LONG (WINAPI *DLL_AVerGetVideoResolutionRangeSupported)(HANDLE,DWORD,DWORD,RESOLUTION_RANGE_INFO*);
LONG WINAPI AVerGetVideoResolutionRangeSupported(HANDLE hCaptureObject, DWORD dwVideoSource,DWORD dwFormat,RESOLUTION_RANGE_INFO *ResolutionRangeInfo)
{
	DLL_AVerGetVideoResolutionRangeSupported dll_Fun;
	dll_Fun = (DLL_AVerGetVideoResolutionRangeSupported)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetVideoResolutionRangeSupported");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,dwVideoSource,dwFormat,ResolutionRangeInfo);
	return hr;
}
typedef LONG (WINAPI *DLL_AVerGetVideoResolutionSupported)(HANDLE,DWORD,DWORD,DWORD *,DWORD *);
LONG WINAPI AVerGetVideoResolutionSupported(HANDLE hCaptureObject,DWORD dwVideoSource,DWORD dwVideoFormat, DWORD *pdwSupported, DWORD *pdwNum)
{
	DLL_AVerGetVideoResolutionSupported dll_Fun;
	dll_Fun = (DLL_AVerGetVideoResolutionSupported)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetVideoResolutionSupported");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,dwVideoSource,dwVideoFormat,pdwSupported,pdwNum);
	return hr;
}
typedef LONG (WINAPI *DLL_AVerSetVideoResolutionEx)(HANDLE , VIDEO_RESOLUTION *);
LONG WINAPI AVerSetVideoResolutionEx(HANDLE hCaptureObject, VIDEO_RESOLUTION *pVideoResolution)
{
	DLL_AVerSetVideoResolutionEx dll_Fun;
	dll_Fun = (DLL_AVerSetVideoResolutionEx)GetProcAddress(g_Dllcontrol.m_hdll, "AVerSetVideoResolutionEx");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pVideoResolution);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerGetVideoResolutionEx)(HANDLE,VIDEO_RESOLUTION*);
LONG WINAPI AVerGetVideoResolutionEx(HANDLE hCaptureObject, VIDEO_RESOLUTION *pVideoResolution)
{
	DLL_AVerGetVideoResolutionEx dll_Fun;
	dll_Fun = (DLL_AVerGetVideoResolutionEx)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetVideoResolutionEx");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pVideoResolution);
	return hr;
}
typedef LONG (WINAPI *DLL_AVerGetVideoInputFrameRateRangeSupported)(HANDLE , DWORD , DWORD , DWORD , DWORD , FRAMERATE_RANGE_INFO *);
LONG WINAPI AVerGetVideoInputFrameRateRangeSupported(HANDLE hCaptureObject, DWORD dwVideoSource, DWORD dwFormat, DWORD dwWidth, DWORD dwHeight, FRAMERATE_RANGE_INFO *FrameRateRangeInfo)
{
	DLL_AVerGetVideoInputFrameRateRangeSupported dll_Fun;
	dll_Fun = (DLL_AVerGetVideoInputFrameRateRangeSupported)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetVideoInputFrameRateRangeSupported");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,dwVideoSource,dwFormat,dwWidth,dwHeight,FrameRateRangeInfo);
	return hr;
}
typedef LONG (WINAPI *DLL_AVerGetVideoInputFrameRateSupportedEx)(HANDLE , DWORD ,DWORD , DWORD , DWORD *, DWORD *);
LONG WINAPI AVerGetVideoInputFrameRateSupportedEx(HANDLE hCaptureObject, DWORD dwVideoSource,DWORD dwVideoFormat, DWORD dwVideoResolution, DWORD *pdwSupported, DWORD *pdwNum)
{
	DLL_AVerGetVideoInputFrameRateSupportedEx dll_Fun;
	dll_Fun = (DLL_AVerGetVideoInputFrameRateSupportedEx)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetVideoInputFrameRateSupportedEx");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,dwVideoSource,dwVideoFormat,dwVideoResolution,pdwSupported,pdwNum);
	return hr;
}
typedef LONG (WINAPI *DLL_AVerSetVideoInputFrameRate)(HANDLE , DWORD );
LONG WINAPI AVerSetVideoInputFrameRate(HANDLE hCaptureObject, DWORD dwFrameRate)
{
	DLL_AVerSetVideoInputFrameRate dll_Fun;
	dll_Fun = (DLL_AVerSetVideoInputFrameRate)GetProcAddress(g_Dllcontrol.m_hdll, "AVerSetVideoInputFrameRate");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,dwFrameRate);
	return hr;
}
typedef LONG (WINAPI *DLL_AVerGetVideoInputFrameRate)(HANDLE , DWORD *);
LONG WINAPI AVerGetVideoInputFrameRate(HANDLE hCaptureObject, DWORD *pdwFrameRate)
{
	DLL_AVerGetVideoInputFrameRate dll_Fun;
	dll_Fun = (DLL_AVerGetVideoInputFrameRate)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetVideoInputFrameRate");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pdwFrameRate);
	return hr;
}
typedef LONG (WINAPI *DLL_AVerGetAudioSamplingRateSupported)(HANDLE , DWORD ,DWORD *, DWORD *);
LONG WINAPI AVerGetAudioSamplingRateSupported(HANDLE hCaptureObject, DWORD dwAudioSource,DWORD *pdwSupported, DWORD *pdwNum)
{
	DLL_AVerGetAudioSamplingRateSupported dll_Fun;
	dll_Fun = (DLL_AVerGetAudioSamplingRateSupported)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetAudioSamplingRateSupported");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,dwAudioSource,pdwSupported,pdwNum);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerSetAudioSamplingRate)(HANDLE,DWORD);
LONG WINAPI AVerSetAudioSamplingRate(HANDLE hCaptureObject, DWORD dwSamplingRate)
{
	DLL_AVerSetAudioSamplingRate dll_Fun;
	dll_Fun = (DLL_AVerSetAudioSamplingRate)GetProcAddress(g_Dllcontrol.m_hdll, "AVerSetAudioSamplingRate");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,dwSamplingRate);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerGetAudioSamplingRate)(HANDLE,DWORD*);
LONG WINAPI AVerGetAudioSamplingRate(HANDLE hCaptureObject, DWORD *pdwSamplingRate)
{
	DLL_AVerGetAudioSamplingRate dll_Fun;
	dll_Fun = (DLL_AVerGetAudioSamplingRate)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetAudioSamplingRate");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pdwSamplingRate);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerStartStreaming)(HANDLE);
LONG WINAPI AVerStartStreaming(HANDLE hCaptureObject)
{
	DLL_AVerStartStreaming dll_Fun;
	dll_Fun = (DLL_AVerStartStreaming)GetProcAddress(g_Dllcontrol.m_hdll, "AVerStartStreaming");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerStopStreaming)(HANDLE);
LONG WINAPI AVerStopStreaming(HANDLE hCaptureObject)
{
	DLL_AVerStopStreaming dll_Fun;
	dll_Fun = (DLL_AVerStopStreaming)GetProcAddress(g_Dllcontrol.m_hdll, "AVerStopStreaming");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerGetAudioInfo)(HANDLE,INPUT_AUDIO_INFO*);
LONG WINAPI AVerGetAudioInfo(HANDLE hCaptureObject, INPUT_AUDIO_INFO *pAudioInfo)
{
	DLL_AVerGetAudioInfo dll_Fun;
	dll_Fun = (DLL_AVerGetAudioInfo)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetAudioInfo");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pAudioInfo);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerGetVideoInfo)(HANDLE,INPUT_VIDEO_INFO*);
LONG WINAPI AVerGetVideoInfo(HANDLE hCaptureObject, INPUT_VIDEO_INFO *pVideoInfo)
{
	DLL_AVerGetVideoInfo dll_Fun;
	dll_Fun = (DLL_AVerGetVideoInfo)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetVideoInfo");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pVideoInfo);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerGetMacroVisionMode)(HANDLE,DWORD*);
LONG WINAPI AVerGetMacroVisionMode(HANDLE hCaptureObject, DWORD *pdwMode)
{
	DLL_AVerGetMacroVisionMode dll_Fun;
	dll_Fun = (DLL_AVerGetMacroVisionMode)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetMacroVisionMode");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pdwMode);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerGetSignalPresence)(HANDLE,BOOL*);
LONG WINAPI AVerGetSignalPresence(HANDLE hCaptureObject, BOOL *pbSignalPresence)
{
	DLL_AVerGetSignalPresence dll_Fun;
	dll_Fun = (DLL_AVerGetSignalPresence)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetSignalPresence");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pbSignalPresence);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerEnumThirdPartyAudioCapSource)(HANDLE,AUDIOCAPTURESOURCE_INFO*,DWORD*);
LONG WINAPI AVerEnumThirdPartyAudioCapSource(HANDLE hCaptureObject, AUDIOCAPTURESOURCE_INFO *pAudioCapSourceInfo, DWORD *pdwNum)
{
	DLL_AVerEnumThirdPartyAudioCapSource dll_Fun;
	dll_Fun = (DLL_AVerEnumThirdPartyAudioCapSource)GetProcAddress(g_Dllcontrol.m_hdll, "AVerEnumThirdPartyAudioCapSource");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pAudioCapSourceInfo,pdwNum);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerEnumThirdPartyAudioCapSourceInputType)(HANDLE,DWORD,AUDIOCAPTURESOURCE_INPUTTYPE_INFO*,DWORD*);
LONG WINAPI AVerEnumThirdPartyAudioCapSourceInputType(HANDLE hCaptureObject, DWORD dwCapIndex, AUDIOCAPTURESOURCE_INPUTTYPE_INFO *pInputTypeInfo, DWORD *pdwNum)
{
	DLL_AVerEnumThirdPartyAudioCapSourceInputType dll_Fun;
	dll_Fun = (DLL_AVerEnumThirdPartyAudioCapSourceInputType)GetProcAddress(g_Dllcontrol.m_hdll, "AVerEnumThirdPartyAudioCapSourceInputType");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,dwCapIndex,pInputTypeInfo,pdwNum);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerEnumThirdPartyAudioCapSourceSampleFormat)(HANDLE,DWORD,AUDIOCAPTURESOURCE_FORMAT_INFO*,DWORD*);
LONG WINAPI AVerEnumThirdPartyAudioCapSourceSampleFormat(HANDLE hCaptureObject, DWORD dwCapIndex, AUDIOCAPTURESOURCE_FORMAT_INFO *pFormatInfo, DWORD *pdwNum)
{
	DLL_AVerEnumThirdPartyAudioCapSourceSampleFormat dll_Fun;
	dll_Fun = (DLL_AVerEnumThirdPartyAudioCapSourceSampleFormat)GetProcAddress(g_Dllcontrol.m_hdll, "AVerEnumThirdPartyAudioCapSourceSampleFormat");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,dwCapIndex,pFormatInfo,pdwNum);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerSetThirdPartyAudioCapSource)(HANDLE,AUDIOCAPTURESOURCE_SETTING*);
LONG WINAPI AVerSetThirdPartyAudioCapSource(HANDLE hCaptureObject, AUDIOCAPTURESOURCE_SETTING *pAudioCapSourceSetting)
{
	DLL_AVerSetThirdPartyAudioCapSource dll_Fun;
	dll_Fun = (DLL_AVerSetThirdPartyAudioCapSource)GetProcAddress(g_Dllcontrol.m_hdll, "AVerSetThirdPartyAudioCapSource");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pAudioCapSourceSetting);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerGetThirdPartyAudioCapSource)(HANDLE,AUDIOCAPTURESOURCE_SETTING*);
LONG WINAPI AVerGetThirdPartyAudioCapSource(HANDLE hCaptureObject, AUDIOCAPTURESOURCE_SETTING *pAudioCapSourceSetting)
{
	DLL_AVerGetThirdPartyAudioCapSource dll_Fun;
	dll_Fun = (DLL_AVerGetThirdPartyAudioCapSource)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetThirdPartyAudioCapSource");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pAudioCapSourceSetting);
	return hr;
};

//Preview Control
typedef LONG (WINAPI *DLL_AVerSetVideoWindowPosition)(HANDLE,RECT);
LONG WINAPI AVerSetVideoWindowPosition(HANDLE hCaptureObject, RECT rectVideoWnd)
{
	DLL_AVerSetVideoWindowPosition dll_Fun;
	dll_Fun = (DLL_AVerSetVideoWindowPosition)GetProcAddress(g_Dllcontrol.m_hdll, "AVerSetVideoWindowPosition");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,rectVideoWnd);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerRepaintVideo)(HANDLE);
LONG WINAPI AVerRepaintVideo(HANDLE hCaptureObject)
{
	DLL_AVerRepaintVideo dll_Fun;
	dll_Fun = (DLL_AVerRepaintVideo)GetProcAddress(g_Dllcontrol.m_hdll, "AVerRepaintVideo");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerSetVideoRenderer)(HANDLE,DWORD);
LONG WINAPI AVerSetVideoRenderer(HANDLE hCaptureObject, DWORD dwVideoRenderer)
{
	DLL_AVerSetVideoRenderer dll_Fun;
	dll_Fun = (DLL_AVerSetVideoRenderer)GetProcAddress(g_Dllcontrol.m_hdll, "AVerSetVideoRenderer");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,dwVideoRenderer);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerGetVideoRenderer)(HANDLE,DWORD*);
LONG WINAPI AVerGetVideoRenderer(HANDLE hCaptureObject, DWORD *pdwVideoRenderer)
{
	DLL_AVerGetVideoRenderer dll_Fun;
	dll_Fun = (DLL_AVerGetVideoRenderer)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetVideoRenderer");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pdwVideoRenderer);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerSetMaintainAspectRatioEnabled)(HANDLE,BOOL);
LONG WINAPI AVerSetMaintainAspectRatioEnabled(HANDLE hCaptureObject, BOOL bEnabled)
{
	DLL_AVerSetMaintainAspectRatioEnabled dll_Fun;
	dll_Fun = (DLL_AVerSetMaintainAspectRatioEnabled)GetProcAddress(g_Dllcontrol.m_hdll, "AVerSetMaintainAspectRatioEnabled");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,bEnabled);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerGetMaintainAspectRatioEnabled)(HANDLE,BOOL*);
LONG WINAPI AVerGetMaintainAspectRatioEnabled(HANDLE hCaptureObject, BOOL *pbEnabled)
{
	DLL_AVerGetMaintainAspectRatioEnabled dll_Fun;
	dll_Fun = (DLL_AVerGetMaintainAspectRatioEnabled)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetMaintainAspectRatioEnabled");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pbEnabled);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerGetAspectRatio)(HANDLE,DWORD*,DWORD*);
LONG WINAPI AVerGetAspectRatio(HANDLE hCaptureObject, DWORD *pdwAspectRatioX, DWORD *pdwAspectRatioY)
{
	DLL_AVerGetAspectRatio dll_Fun;
	dll_Fun = (DLL_AVerGetAspectRatio)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetAspectRatio");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pdwAspectRatioX,pdwAspectRatioY);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerSetVideoPreviewEnabled)(HANDLE,BOOL);
LONG WINAPI AVerSetVideoPreviewEnabled(HANDLE hCaptureObject, BOOL bEnabled)
{
	DLL_AVerSetVideoPreviewEnabled dll_Fun;
	dll_Fun = (DLL_AVerSetVideoPreviewEnabled)GetProcAddress(g_Dllcontrol.m_hdll, "AVerSetVideoPreviewEnabled");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,bEnabled);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerGetVideoPreviewEnabled)(HANDLE,BOOL*);
LONG WINAPI AVerGetVideoPreviewEnabled(HANDLE hCaptureObject, BOOL *pbEnabled)
{
	DLL_AVerGetVideoPreviewEnabled dll_Fun;
	dll_Fun = (DLL_AVerGetVideoPreviewEnabled)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetVideoPreviewEnabled");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pbEnabled);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerSetAudioPreviewEnabled)(HANDLE,BOOL);
LONG WINAPI AVerSetAudioPreviewEnabled(HANDLE hCaptureObject, BOOL bEnabled)
{
	DLL_AVerSetAudioPreviewEnabled dll_Fun;
	dll_Fun = (DLL_AVerSetAudioPreviewEnabled)GetProcAddress(g_Dllcontrol.m_hdll, "AVerSetAudioPreviewEnabled");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,bEnabled);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerGetAudioPreviewEnabled)(HANDLE,BOOL*);
LONG WINAPI AVerGetAudioPreviewEnabled(HANDLE hCaptureObject, BOOL *pbEnabled)
{
	DLL_AVerGetAudioPreviewEnabled dll_Fun;
	dll_Fun = (DLL_AVerGetAudioPreviewEnabled)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetAudioPreviewEnabled");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pbEnabled);
	return hr;
};


// SW Processor
typedef LONG (WINAPI *DLL_AVerSetVideoProcAmp)(HANDLE,DWORD,DWORD);
LONG WINAPI AVerSetVideoProcAmp(HANDLE hCaptureObject, DWORD dwVideoProcAmpProperty, DWORD dwPropertyValue)
{
	DLL_AVerSetVideoProcAmp dll_Fun;
	dll_Fun = (DLL_AVerSetVideoProcAmp)GetProcAddress(g_Dllcontrol.m_hdll, "AVerSetVideoProcAmp");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,dwVideoProcAmpProperty,dwPropertyValue);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerGetVideoProcAmp)(HANDLE,DWORD,DWORD*);
LONG WINAPI AVerGetVideoProcAmp(HANDLE hCaptureObject, DWORD dwVideoProcAmpProperty, DWORD *pdwPropertyValue)
{
	DLL_AVerGetVideoProcAmp dll_Fun;
	dll_Fun = (DLL_AVerGetVideoProcAmp)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetVideoProcAmp");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,dwVideoProcAmpProperty,pdwPropertyValue);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerSetVideoOutputFrameRate)(HANDLE,DWORD);
LONG WINAPI AVerSetVideoOutputFrameRate(HANDLE hCaptureObject, DWORD dwFrameRate)
{
	DLL_AVerSetVideoOutputFrameRate dll_Fun;
	dll_Fun = (DLL_AVerSetVideoOutputFrameRate)GetProcAddress(g_Dllcontrol.m_hdll, "AVerSetVideoOutputFrameRate");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,dwFrameRate);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerGetVideoOutputFrameRate)(HANDLE,DWORD*);
LONG WINAPI AVerGetVideoOutputFrameRate(HANDLE hCaptureObject, DWORD *pdwFrameRate)
{
	DLL_AVerGetVideoOutputFrameRate dll_Fun;
	dll_Fun = (DLL_AVerGetVideoOutputFrameRate)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetVideoOutputFrameRate");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pdwFrameRate);
	return hr;
};


// Capture Audio
typedef LONG (WINAPI *DLL_AVerCaptureAudioSampleStart)(HANDLE,AUDIOPRCSCALLBACK,LONGPTR);
LONG WINAPI AVerCaptureAudioSampleStart(HANDLE hCaptureObject, AUDIOPRCSCALLBACK lpCallback, LONGPTR lCallbackUserData)
{
	DLL_AVerCaptureAudioSampleStart dll_Fun;
	dll_Fun = (DLL_AVerCaptureAudioSampleStart)GetProcAddress(g_Dllcontrol.m_hdll, "AVerCaptureAudioSampleStart");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,lpCallback,lCallbackUserData);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerCaptureAudioSampleStartEx)(HANDLE,AUDIO_CAPTURE_INFO);
LONG WINAPI AVerCaptureAudioSampleStartEx(HANDLE hCaptureObject, AUDIO_CAPTURE_INFO CaptureInfo)
{
	DLL_AVerCaptureAudioSampleStartEx dll_Fun;
	dll_Fun = (DLL_AVerCaptureAudioSampleStartEx)GetProcAddress(g_Dllcontrol.m_hdll, "AVerCaptureAudioSampleStartEx");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,CaptureInfo);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerCaptureAudioSampleStop)(HANDLE);
LONG WINAPI AVerCaptureAudioSampleStop(HANDLE hCaptureObject)
{
	DLL_AVerCaptureAudioSampleStop dll_Fun;
	dll_Fun = (DLL_AVerCaptureAudioSampleStop)GetProcAddress(g_Dllcontrol.m_hdll, "AVerCaptureAudioSampleStop");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject);
	return hr;
};

// Capture Image
typedef LONG (WINAPI *DLL_AVerCaptureSingleImage)(HANDLE,CAPTURE_SINGLE_IMAGE_INFO*);
LONG WINAPI AVerCaptureSingleImage(HANDLE hCaptureObject, CAPTURE_SINGLE_IMAGE_INFO *pCaptureSingleImageInfo)
{
	DLL_AVerCaptureSingleImage dll_Fun;
	dll_Fun = (DLL_AVerCaptureSingleImage)GetProcAddress(g_Dllcontrol.m_hdll, "AVerCaptureSingleImage");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pCaptureSingleImageInfo);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerCaptureSingleImageToBuffer)(HANDLE,BYTE*,LONG*,BOOL,DWORD);
LONG WINAPI AVerCaptureSingleImageToBuffer(HANDLE hCaptureObject, BYTE *lpBmpData, LONG *plBufferSize, BOOL bOverlayMix, DWORD dwTimeout)
{
	DLL_AVerCaptureSingleImageToBuffer dll_Fun;
	dll_Fun = (DLL_AVerCaptureSingleImageToBuffer)GetProcAddress(g_Dllcontrol.m_hdll, "AVerCaptureSingleImageToBuffer");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,lpBmpData,plBufferSize,bOverlayMix,dwTimeout);
	return hr;
};

// Capture Video
typedef LONG (WINAPI *DLL_AVerCaptureVideoSequenceStart)(HANDLE,VIDEO_CAPTURE_INFO);
LONG WINAPI AVerCaptureVideoSequenceStart(HANDLE hCaptureObject, VIDEO_CAPTURE_INFO CaptureInfo)
{
	DLL_AVerCaptureVideoSequenceStart dll_Fun;
	dll_Fun = (DLL_AVerCaptureVideoSequenceStart)GetProcAddress(g_Dllcontrol.m_hdll, "AVerCaptureVideoSequenceStart");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,CaptureInfo);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerCaptureVideoSequenceStop)(HANDLE);
LONG WINAPI AVerCaptureVideoSequenceStop(HANDLE hCaptureObject)
{
	DLL_AVerCaptureVideoSequenceStop dll_Fun;
	dll_Fun = (DLL_AVerCaptureVideoSequenceStop)GetProcAddress(g_Dllcontrol.m_hdll, "AVerCaptureVideoSequenceStop");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerSetAudioRecordEnabled)(HANDLE,BOOL);
LONG WINAPI AVerSetAudioRecordEnabled(HANDLE hCaptureObject, BOOL bEnabled)
{
	DLL_AVerSetAudioRecordEnabled dll_Fun;
	dll_Fun = (DLL_AVerSetAudioRecordEnabled)GetProcAddress(g_Dllcontrol.m_hdll, "AVerSetAudioRecordEnabled");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,bEnabled);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerGetAudioRecordEnabled)(HANDLE,BOOL*);
LONG WINAPI AVerGetAudioRecordEnabled(HANDLE hCaptureObject, BOOL *pbEnabled)
{
	DLL_AVerGetAudioRecordEnabled dll_Fun;
	dll_Fun = (DLL_AVerGetAudioRecordEnabled)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetAudioRecordEnabled");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pbEnabled);
	return hr;
};

// Encoder Setting
typedef LONG (WINAPI *DLL_AVerSetMpeg2VideoEncoderInfo)(HANDLE,MPEG2_VIDEOENCODER_INFO*);
LONG WINAPI AVerSetMpeg2VideoEncoderInfo(HANDLE hCaptureObject, MPEG2_VIDEOENCODER_INFO *pVideoEncoderInfo)
{
	DLL_AVerSetMpeg2VideoEncoderInfo dll_Fun;
	dll_Fun = (DLL_AVerSetMpeg2VideoEncoderInfo)GetProcAddress(g_Dllcontrol.m_hdll, "AVerSetMpeg2VideoEncoderInfo");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pVideoEncoderInfo);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerGetMpeg2VideoEncoderInfo)(HANDLE,MPEG2_VIDEOENCODER_INFO*);
LONG WINAPI AVerGetMpeg2VideoEncoderInfo(HANDLE hCaptureObject, MPEG2_VIDEOENCODER_INFO *pVideoEncoderInfo)
{
	DLL_AVerGetMpeg2VideoEncoderInfo dll_Fun;
	dll_Fun = (DLL_AVerGetMpeg2VideoEncoderInfo)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetMpeg2VideoEncoderInfo");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pVideoEncoderInfo);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerSetMpeg2AudioEncoderInfo)(HANDLE,MPEG2_AUDIOENCODER_INFO*);
LONG WINAPI AVerSetMpeg2AudioEncoderInfo(HANDLE hCaptureObject, MPEG2_AUDIOENCODER_INFO *pAudioEncoderInfo)
{
	DLL_AVerSetMpeg2AudioEncoderInfo dll_Fun;
	dll_Fun = (DLL_AVerSetMpeg2AudioEncoderInfo)GetProcAddress(g_Dllcontrol.m_hdll, "AVerSetMpeg2AudioEncoderInfo");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pAudioEncoderInfo);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerGetMpeg2AudioEncoderInfo)(HANDLE,MPEG2_AUDIOENCODER_INFO*);
LONG WINAPI AVerGetMpeg2AudioEncoderInfo(HANDLE hCaptureObject, MPEG2_AUDIOENCODER_INFO *pAudioEncoderInfo)
{
	DLL_AVerGetMpeg2AudioEncoderInfo dll_Fun;
	dll_Fun = (DLL_AVerGetMpeg2AudioEncoderInfo)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetMpeg2AudioEncoderInfo");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pAudioEncoderInfo);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerSetMpeg4VideoEncoderInfo)(HANDLE,MPEG4_VIDEOENCODER_INFO*);
LONG WINAPI AVerSetMpeg4VideoEncoderInfo(HANDLE hCaptureObject, MPEG4_VIDEOENCODER_INFO *pVideoEncoderInfo)
{
	DLL_AVerSetMpeg4VideoEncoderInfo dll_Fun;
	dll_Fun = (DLL_AVerSetMpeg4VideoEncoderInfo)GetProcAddress(g_Dllcontrol.m_hdll, "AVerSetMpeg4VideoEncoderInfo");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pVideoEncoderInfo);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerGetMpeg4VideoEncoderInfo)(HANDLE,MPEG4_VIDEOENCODER_INFO*);
LONG WINAPI AVerGetMpeg4VideoEncoderInfo(HANDLE hCaptureObject, MPEG4_VIDEOENCODER_INFO *pVideoEncoderInfo)
{
	DLL_AVerGetMpeg4VideoEncoderInfo dll_Fun;
	dll_Fun = (DLL_AVerGetMpeg4VideoEncoderInfo)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetMpeg4VideoEncoderInfo");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pVideoEncoderInfo);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerSetMpeg4AudioEncoderInfo)(HANDLE,MPEG4_AUDIOENCODER_INFO*);
LONG WINAPI AVerSetMpeg4AudioEncoderInfo(HANDLE hCaptureObject, MPEG4_AUDIOENCODER_INFO *pAudioEncoderInfo)
{
	DLL_AVerSetMpeg4AudioEncoderInfo dll_Fun;
	dll_Fun = (DLL_AVerSetMpeg4AudioEncoderInfo)GetProcAddress(g_Dllcontrol.m_hdll, "AVerSetMpeg4AudioEncoderInfo");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pAudioEncoderInfo);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerGetMpeg4AudioEncoderInfo)(HANDLE,MPEG4_AUDIOENCODER_INFO*);
LONG WINAPI AVerGetMpeg4AudioEncoderInfo(HANDLE hCaptureObject, MPEG4_AUDIOENCODER_INFO *pAudioEncoderInfo)
{
	DLL_AVerGetMpeg4AudioEncoderInfo dll_Fun;
	dll_Fun = (DLL_AVerGetMpeg4AudioEncoderInfo)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetMpeg4AudioEncoderInfo");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pAudioEncoderInfo);
	return hr;
};

// HW Encoder Capture Card Setting 
typedef LONG (WINAPI *DLL_AVerHwSetVideoEncoderInfo)(HANDLE,HW_VIDEOENCODER_INFO*);
LONG WINAPI AVerHwSetVideoEncoderInfo(HANDLE hCaptureObject, HW_VIDEOENCODER_INFO *pVideoEncoderInfo)
{
	DLL_AVerHwSetVideoEncoderInfo dll_Fun;
	dll_Fun = (DLL_AVerHwSetVideoEncoderInfo)GetProcAddress(g_Dllcontrol.m_hdll, "AVerHwSetVideoEncoderInfo");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pVideoEncoderInfo);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerHwGetVideoEncoderInfo)(HANDLE,HW_VIDEOENCODER_INFO*);
LONG WINAPI AVerHwGetVideoEncoderInfo(HANDLE hCaptureObject, HW_VIDEOENCODER_INFO *pVideoEncoderInfo)
{
	DLL_AVerHwGetVideoEncoderInfo dll_Fun;
	dll_Fun = (DLL_AVerHwGetVideoEncoderInfo)GetProcAddress(g_Dllcontrol.m_hdll, "AVerHwGetVideoEncoderInfo");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pVideoEncoderInfo);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerHwSetAudioEncoderInfo)(HANDLE,HW_AUDIOENCODER_INFO*);
LONG WINAPI AVerHwSetAudioEncoderInfo(HANDLE hCaptureObject, HW_AUDIOENCODER_INFO *pAudioEncoderInfo)
{
	DLL_AVerHwSetAudioEncoderInfo dll_Fun;
	dll_Fun = (DLL_AVerHwSetAudioEncoderInfo)GetProcAddress(g_Dllcontrol.m_hdll, "AVerHwSetAudioEncoderInfo");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pAudioEncoderInfo);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerHwGetAudioEncoderInfo)(HANDLE,HW_AUDIOENCODER_INFO*);
LONG WINAPI AVerHwGetAudioEncoderInfo(HANDLE hCaptureObject, HW_AUDIOENCODER_INFO *pAudioEncoderInfo)
{
	DLL_AVerHwGetAudioEncoderInfo dll_Fun;
	dll_Fun = (DLL_AVerHwGetAudioEncoderInfo)GetProcAddress(g_Dllcontrol.m_hdll, "AVerHwGetAudioEncoderInfo");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pAudioEncoderInfo);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerHwCaptureStreamStart)(HANDLE,HW_STREAM_CAPTURE_INFO*);
LONG WINAPI AVerHwCaptureStreamStart(HANDLE hCaptureObject, HW_STREAM_CAPTURE_INFO *pCaptureInfo)
{
	DLL_AVerHwCaptureStreamStart dll_Fun;
	dll_Fun = (DLL_AVerHwCaptureStreamStart)GetProcAddress(g_Dllcontrol.m_hdll, "AVerHwCaptureStreamStart");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pCaptureInfo);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerHwCaptureStreamStop)(HANDLE);
LONG WINAPI AVerHwCaptureStreamStop(HANDLE hCaptureObject)
{
	DLL_AVerHwCaptureStreamStop dll_Fun;
	dll_Fun = (DLL_AVerHwCaptureStreamStop)GetProcAddress(g_Dllcontrol.m_hdll, "AVerHwCaptureStreamStop");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject);
	return hr;
};

// Event Callback 
typedef LONG (WINAPI *DLL_AVerSetEventCallback)(HANDLE,NOTIFYEVENTCALLBACK,DWORD,LPVOID);
LONG WINAPI AVerSetEventCallback(HANDLE hCaptureObject, NOTIFYEVENTCALLBACK lpCallback, DWORD dwOptions, LPVOID lpUserData)
{
	DLL_AVerSetEventCallback dll_Fun;
	dll_Fun = (DLL_AVerSetEventCallback)GetProcAddress(g_Dllcontrol.m_hdll, "AVerSetEventCallback");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,lpCallback,dwOptions,lpUserData);
	return hr;
};

// ASI Input (ex: CD910)
typedef LONG (WINAPI *DLL_AVerTsCaptureStreamStart)(HANDLE,TS_STREAM_CAPTURE_INFO*);
LONG WINAPI AVerTsCaptureStreamStart(HANDLE hCaptureObject, TS_STREAM_CAPTURE_INFO *pCaptureInfo)
{
	DLL_AVerTsCaptureStreamStart dll_Fun;
	dll_Fun = (DLL_AVerTsCaptureStreamStart)GetProcAddress(g_Dllcontrol.m_hdll, "AVerTsCaptureStreamStart");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pCaptureInfo);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerTsCaptureStreamStop)(HANDLE);
LONG WINAPI AVerTsCaptureStreamStop(HANDLE hCaptureObject)
{
	DLL_AVerTsCaptureStreamStop dll_Fun;
	dll_Fun = (DLL_AVerTsCaptureStreamStop)GetProcAddress(g_Dllcontrol.m_hdll, "AVerTsCaptureStreamStop");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerTsSetStreamCallback)(HANDLE,TS_STREAM_CALLBACK_INFO*);
LONG WINAPI AVerTsSetStreamCallback(HANDLE hCaptureObject, TS_STREAM_CALLBACK_INFO *pCallbackInfo)
{
	DLL_AVerTsSetStreamCallback dll_Fun;
	dll_Fun = (DLL_AVerTsSetStreamCallback)GetProcAddress(g_Dllcontrol.m_hdll, "AVerTsSetStreamCallback");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pCallbackInfo);
	return hr;
};

//SDI Audio Setting
typedef LONG (WINAPI *DLL_AVerSetEmbeddedAudioChannel)(HANDLE,DWORD);
LONG WINAPI AVerSetEmbeddedAudioChannel(HANDLE hCaptureObject, DWORD dwChannels)
{
	DLL_AVerSetEmbeddedAudioChannel dll_Fun;
	dll_Fun = (DLL_AVerSetEmbeddedAudioChannel)GetProcAddress(g_Dllcontrol.m_hdll, "AVerSetEmbeddedAudioChannel");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,dwChannels);
	return hr;
};
typedef LONG (WINAPI *DLL_AVerGetEmbeddedAudioChannel)(HANDLE,DWORD*);
LONG WINAPI AVerGetEmbeddedAudioChannel(HANDLE hCaptureObject, DWORD *pdwChannels)
{
	DLL_AVerGetEmbeddedAudioChannel dll_Fun;
	dll_Fun = (DLL_AVerGetEmbeddedAudioChannel)GetProcAddress(g_Dllcontrol.m_hdll, "AVerGetEmbeddedAudioChannel");
	if (dll_Fun == NULL)
	{
		return CAP_EC_UNKNOWN_ERROR;
	}
	LONG hr = dll_Fun(hCaptureObject,pdwChannels);
	return hr;
};

typedef LONG(WINAPI *DLL_AVerSetDeinterlaceMode)(HANDLE, DWORD);
LONG WINAPI AVerSetDeinterlaceMode(HANDLE hCaptureObject, DWORD dwMode)
{
  DLL_AVerSetDeinterlaceMode dll_Fun;
  dll_Fun = (DLL_AVerSetDeinterlaceMode)GetProcAddress(g_Dllcontrol.m_hdll, "AVerSetDeinterlaceMode");
  if (dll_Fun == NULL)
  {
    return CAP_EC_UNKNOWN_ERROR;
  }
  LONG hr = dll_Fun(hCaptureObject, dwMode);
  return hr;
};