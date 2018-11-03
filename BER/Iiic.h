#pragma once 
#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#ifdef __cplusplus
}
#endif

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"avcodec.lib")	
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"avdevice.lib")
#pragma comment(lib,"avfilter.lib")
#pragma comment(lib,"postproc.lib")
#pragma comment(lib,"swresample.lib")
#pragma comment(lib,"swscale.lib")
#pragma comment(lib,"SDL.lib")

#ifndef _WINGDI_ 
#define _WINGDI_
typedef struct tagBITMAPFILEHEADER{ 
	WORD    bfType;
	DWORD   bfSize; 
	WORD    bfReserved1; 
	WORD    bfReserved2; 
	DWORD   bfOffBits; 
}BITMAPFILEHEADER, FAR *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER; 

typedef struct tagBITMAPINFOHEADER{ 
	DWORD      biSize; 
	LONG       biWidth; 
	LONG       biHeight; 
	WORD       biPlanes; 
	WORD       biBitCount; 
	DWORD      biCompression; 
	DWORD      biSizeImage; 
	LONG       biXPelsPerMeter; 
	LONG       biYPelsPerMeter; 
	DWORD      biClrUsed; 
	DWORD      biClrImportant; 
} BITMAPINFOHEADER, FAR *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER; 

#endif 

#define  PORT 8017
#define  IP_ADDRESS "192.168.1.118"

class CIiic
{
public:
	CIiic(void);
	~CIiic(void);
	int ListenIiic();
	int CloseIiic();
	int nOpenCnt;
	int OpenRtsp();
	int TakeBmpFromRtsp();
	void Save (char* bmpName) ;

	int Show(bool open);
	int CloseShow();
};
struct TrdParam
{ 
	CWnd *pWnd;
	CDC *pDC;
	CRect rect;
};
extern char *recvBuffer;
extern char *yuvBmpBuf;
extern int  totalLength;
extern CIiic gclsIiic;
extern bool bRcvYUVOver;
extern AVFrame *pFrame, *pFrameRGB; 
extern TrdParam *stuTrd;