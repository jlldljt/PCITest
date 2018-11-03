#include "StdAfx.h"
#include "Iiic.h"



using namespace std;


WSADATA  Ws;
SOCKET ServerSocket, CientSocket;
struct sockaddr_in LocalAddr, ClientAddr;
int Ret = 0;
int AddrLen = 0;
HANDLE hThread = NULL;

FILE *ofh1 = NULL;
char yuvFilename[64];
int count = 0;
int recv_size = 0,write_size = 0;
char buffer[1024];
char *recvBuffer = (char *)malloc( sizeof(char)*2048*1280*1.5);
char *yuvBmpBuf;// = (char *)malloc( sizeof(char)*1920*1080);
int  totalLength = 2048*1280*1.5;

bool bOpen = 0,bRcvYUVOver = 0;
bool bOpenShow= 0,bOpenRtsp = 0;
TrdParam *stuTrd = new TrdParam;
///////////////////////////////////
AVFrame *pFrame, *pFrameRGB ,*pFrameShow; 
AVCodecContext *pCodecCtx; 
AVFormatContext *pFormatCtx; 
AVCodec *pCodec; 

int flag = 0;
int nShowFlag = 0;
//SaveAsBMP (pFrameRGB, pCodecCtx->width, pCodecCtx->height, i++, 24);
void SaveAsBMP (AVFrame *pFrameRGB, int width, int height, char* bmpName, int bpp) 
{ 
	/**/
	char buf[5] = {0}; 
	BITMAPFILEHEADER bmpheader; 
	BITMAPINFOHEADER bmpinfo; 
	FILE *fp; 
	static int index = 0;

	//char *filename = new char[255];
	////文件存放路径，根据自己的修改
	//sprintf_s(bmpName,255,"%s%d.bmp","D:/bmp",index);
	if ( (fp=fopen(bmpName,"wb+")) == NULL ) 
	{ 
		printf ("open file failed!\n"); 
		return; 
	} 
	index ++;
	bmpheader.bfType = 0x4d42; 
	bmpheader.bfReserved1 = 0; 
	bmpheader.bfReserved2 = 0; 
	bmpheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 
	bmpheader.bfSize = bmpheader.bfOffBits + width*height*bpp/8; 

	bmpinfo.biSize = sizeof(BITMAPINFOHEADER); 
	bmpinfo.biWidth = width; 
	bmpinfo.biHeight = height; 
	bmpinfo.biPlanes = 1; 
	bmpinfo.biBitCount = bpp; 
	bmpinfo.biCompression = BI_RGB; 
	bmpinfo.biSizeImage = (width*bpp+31)/32*4*height; 
	bmpinfo.biXPelsPerMeter = 100; 
	bmpinfo.biYPelsPerMeter = 100; 
	bmpinfo.biClrUsed = 0; 
	bmpinfo.biClrImportant = 0; 

	fwrite (&bmpheader, sizeof(bmpheader), 1, fp); 
	fwrite (&bmpinfo, sizeof(bmpinfo), 1, fp); 
	fwrite (pFrameRGB->data[0], width*height*bpp/8, 1, fp); 

	fclose(fp); 
	/***/
} 
//////////////////////////////////////////////////////////////////////////
//线程
CWinThread* gTrdIiicAc;//运行线程
UINT IiicAc(LPVOID pParam);//运行线程函数
UINT IiicAc(LPVOID pParam)
{
	int lineByte=(1920 +3)/4*4; //save 8bitcount bmp data
	yuvBmpBuf = (char *)malloc( sizeof(char)*lineByte*1080);
	memset(recvBuffer ,0,2048*1280*1.5);
	while(bOpen)
	{
		//========accept==========================================================
		printf("----------------------wait connect-------------------\n");
		int recvLength=0;
		AddrLen = sizeof(ClientAddr);
		CientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddr, &AddrLen);//触发需要原先的图像采集
		if ( CientSocket == INVALID_SOCKET )
		{
			AfxMessageBox(_T("Accept Failed\n"));
			break;
		}

		printf("recv ---------------accept   CientSocket = %d\n",CientSocket);
		//===============================================================================

		printf("begin recv -------------------------------------\n");
		while(1)
		{	
			recv_size = recv(CientSocket,buffer,1024,0);
			memcpy(&recvBuffer[recvLength],buffer,recv_size);

			if(recv_size <= 0)
			{
				printf("recv ---------------finish\n");
				break;	
			}
			recvLength += recv_size;
		}
		//memcpy(yuvBmpBuf,recvBuffer ,2048*1280);
		
		for (int i = 0 ; i<1080; i++ )
		{
			for (int j = 0 ; j< 1920 ; j++)
			{
				*(yuvBmpBuf+(1080-1-i)*lineByte+j) = *(recvBuffer+i*2048+j);
			}
		}
		bRcvYUVOver = 1;
		printf("over recv -------------------------------------\n");
		closesocket(CientSocket);
	}
	return 1;
}
CWinThread* gTrdRtsp2Bmp;//运行线程
UINT rtsp2bmp(LPVOID lpParameter);//运行线程函数
UINT rtsp2bmp(LPVOID lpParameter)
{
	//SetThreadAffinityMask(GetCurrentThread(), 0);
	unsigned int i = 0, videoStream = -1; 
	
	
	struct SwsContext *pSwsCtx; 
	AVPacket packet; 
	int frameFinished; 
	int PictureSize; 
	uint8_t *buf; 
	//LARGE_INTEGER l_lgint_start, l_lgint_end;
	//LARGE_INTEGER l_lgint_freq;
	//QueryPerformanceFrequency(&l_lgint_freq);


	printf("\n======================================\n");
	//QueryPerformanceCounter(&l_lgint_start);
	//char filepath[]="rtsp://192.168.1.107:8557/PSIA/Streaming/channels/2?videoCodecType=H.264";
	char filepath[]="rtsp://192.168.1.102:8557/PSIA/Streaming/channels/2?videoCodecType=H.264";
	av_register_all();//注册所有的文件格式和编解码器的库
	avformat_network_init();//打开流媒体初始化
	pFormatCtx = avformat_alloc_context();

	if(avformat_open_input(&pFormatCtx,filepath,NULL,NULL)!=0)
	{ 
		printf ("av open input file failed!\n"); 
		bOpenRtsp = 0;
		exit (1); 
	} 
	//获取流信息
	if ( av_find_stream_info(pFormatCtx) < 0 ) 
	{ 
		printf ("av find stream info failed!\n"); 
		bOpenRtsp = 0;
		exit (1); 
	} 
	//获取视频流
	for ( i=0; i<pFormatCtx->nb_streams; i++ ) 
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
		{ 
			videoStream = i; 
			break; 
		} 

		if (videoStream == -1) 
		{ 
			printf ("find video stream failed!\n"); 
			bOpenRtsp = 0;
			exit (1); 
		} 

		pCodecCtx = pFormatCtx->streams[videoStream]->codec; 

		pCodec = avcodec_find_decoder (pCodecCtx->codec_id); 

		if (pCodec == NULL) 
		{ 
			printf ("avcode find decoder failed!\n"); 
			bOpenRtsp = 0;
			exit (1); 
		} 
		//打开解码器
		if(avcodec_open2(pCodecCtx, pCodec,NULL)<0)
		{ 
			printf ("avcode open failed!\n"); 
			bOpenRtsp = 0;
			exit (1); 
		} 

		//为每帧图像分配内存
		pFrame = avcodec_alloc_frame(); 
		pFrameRGB = avcodec_alloc_frame(); 
		pFrameShow = avcodec_alloc_frame();
		if ( (pFrame==NULL)||(pFrameRGB==NULL)||(pFrameShow == NULL) ) 
		{ 
			printf("avcodec alloc frame failed!\n"); 
			bOpenRtsp = 0;
			exit (1); 
		} 


		PictureSize = avpicture_get_size (PIX_FMT_BGR24, pCodecCtx->width, pCodecCtx->height); 
		buf = (uint8_t*)av_malloc(PictureSize); 

		if ( buf == NULL ) 
		{ 
			printf( "av malloc failed!\n"); 
			bOpenRtsp = 0;
			exit(1); 
		} 
		avpicture_fill ( (AVPicture *)pFrameRGB, buf, PIX_FMT_BGR24, pCodecCtx->width, pCodecCtx->height); 
		avpicture_fill ( (AVPicture *)pFrameShow, buf, PIX_FMT_BGR24, pCodecCtx->width, pCodecCtx->height); 

		//设置图像转换上下文
		pSwsCtx = sws_getContext (pCodecCtx->width, 
			pCodecCtx->height, 
			pCodecCtx->pix_fmt, 
			pCodecCtx->width, 
			pCodecCtx->height, 
			PIX_FMT_BGR24, 
			SWS_BICUBIC, 
			NULL, NULL, NULL); 
		i = 0; 

		while(av_read_frame(pFormatCtx, &packet) >= 0) 
		{ 
			
			if(packet.stream_index==videoStream) 
			{ 
				avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);//解码一帧视频数据

				if(frameFinished) 
				{    
					//反转图像 ，否则生成的图像是上下调到的
					//反转的方式，将数据长度设成倒数，将指针指向数据末尾，遍历的时候往回遍历
					pFrame->data[0] += pFrame->linesize[0] * (pCodecCtx->height - 1); 
					pFrame->linesize[0] *= -1; 
					pFrame->data[1] += pFrame->linesize[1] * (pCodecCtx->height / 2 - 1); 
					pFrame->linesize[1] *= -1; 
					pFrame->data[2] += pFrame->linesize[2] * (pCodecCtx->height / 2 - 1); 
					pFrame->linesize[2] *= -1; 
					//转换图像格式，将解压出来的YUV420P的图像转换为BRG24的图像
					//sws_scale (pSwsCtx, pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize); 


					//printf("save ------------------------------ = %d\n",i);
					//getchar();
					if(flag != 0)
					{
						//QueryPerformanceCounter(&l_lgint_end);
						//double dTmpRT=double(l_lgint_end.QuadPart-l_lgint_start.QuadPart)/double(l_lgint_freq.QuadPart);
						//printf("save ------------------------------dTmpRT = %lf\n",dTmpRT);
						//QueryPerformanceCounter(&l_lgint_end);
						//double dTmpRT=double(l_lgint_end.QuadPart-l_lgint_start.QuadPart)/double(l_lgint_freq.QuadPart);	
//						printf("%lf",dTmpRT);
						//SaveAsBMP (pFrameRGB, pCodecCtx->width, pCodecCtx->height, i++, 24);
						//转换图像格式，将解压出来的YUV420P的图像转换为BRG24的图像
						sws_scale (pSwsCtx, pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize); 
						flag = 0;
					}
					while(nShowFlag == 1)
					{
						//QueryPerformanceCounter(&l_lgint_end);
						//double dTmpRT=double(l_lgint_end.QuadPart-l_lgint_start.QuadPart)/double(l_lgint_freq.QuadPart);
						//printf("save ------------------------------dTmpRT = %lf\n",dTmpRT);
						//QueryPerformanceCounter(&l_lgint_end);
						//double dTmpRT=double(l_lgint_end.QuadPart-l_lgint_start.QuadPart)/double(l_lgint_freq.QuadPart);	
						//						printf("%lf",dTmpRT);
						//SaveAsBMP (pFrameRGB, pCodecCtx->width, pCodecCtx->height, i++, 24);
						//转换图像格式，将解压出来的YUV420P的图像转换为BRG24的图像
						sws_scale (pSwsCtx, pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameShow->data, pFrameShow->linesize); 
						nShowFlag = 0;
					}
					
					//break;

				}     
			} 
			av_free_packet(&packet); 
			if (bOpenRtsp == 0)
			{
				return 0;

			}
		} 


		
		sws_freeContext (pSwsCtx); 
		av_free (pFrame); 
		av_free (pFrameRGB); 
		av_free(pFrameShow);
		avcodec_close (pCodecCtx); 
		av_close_input_file (pFormatCtx); 
		bOpenRtsp = 0;
		return 0; 
}

CWinThread* gTrdRtspShow;//运行线程
UINT rtspShow(LPVOID lpParameter);//运行线程函数
UINT rtspShow(LPVOID lpParameter)
{
	TrdParam* WND=(TrdParam*)lpParameter;
	BITMAPINFOHEADER RGB32BITSBITMAPINFO = {   
		sizeof(BITMAPINFOHEADER),    // biSize 
		1920,                    // biWidth; 
		1080,                // biHeight; 
		1,                            // biPlanes; 
		24,                            // biBitCount 
		BI_RGB,                        // biCompression; 
		(1920 * 24/8+3)/4*4 * 1080,                            // biSizeImage; 
		0,                            // biXPelsPerMeter; 
		0,                            // biYPelsPerMeter; 
		0,                            // biClrUsed; 
		0                            // biClrImportant; 
	};
	int i = 0;
	while(bOpenShow)
	{
		//采集图像
		nShowFlag = 1;
		i= 0;
		while(nShowFlag == 1)
		{
			i++;
			Sleep(1);
			if (i>1000)
			{
				break;
			}

		}

	
	StretchDIBits(WND->pDC->GetSafeHdc(), 0, 0, WND->rect.Width(), WND->rect.Height(), 
		0, 0,  1920, 1080, pFrameShow->data[0], 
		(BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, SRCCOPY); 
	Sleep(1);
	}
	/*while(bOpenShow)
	{
		Sleep(1);
	}
	Sleep(5000);*/
	return 0; 
}
CIiic::CIiic(void)
{
	nOpenCnt = 0;
}
CIiic gclsIiic;

CIiic::~CIiic(void)
{
	CloseIiic();
	CloseShow();
	//stuTrd->pWnd->ReleaseDC(stuTrd->pDC);
	delete[]stuTrd ;stuTrd = NULL;
}

int CIiic::ListenIiic()
{
	//Init Windows Socket
	if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
	{
		printf("Init Windows Socket Failed\n");
		getchar();
		return -1;
	}

	//Create Socket
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if ( ServerSocket == INVALID_SOCKET )
	{
		printf("Create Socket Failed\n");
		getchar();
		return -1;
	}

	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	LocalAddr.sin_port = htons(PORT);
	memset(LocalAddr.sin_zero, 0x00, 8);

	//Bind Socket
	Ret = bind(ServerSocket, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr));
	if ( Ret != 0 )
	{
		printf("Bind Socket Failed\n");
		getchar();
		return -1;
	}

	Ret = listen(ServerSocket, 10);
	if ( Ret != 0 )
	{
		printf("listen Socket Failed\n");
		getchar();
		return -1;
	}

	printf("----------------------server ok-------------------\n");

	bOpen = 1;
	gTrdIiicAc=AfxBeginThread(IiicAc, NULL , THREAD_PRIORITY_NORMAL , 0 , CREATE_SUSPENDED);
	gTrdIiicAc->m_bAutoDelete = TRUE;
	gTrdIiicAc->ResumeThread();
	return 1;
}

int CIiic::CloseIiic()
{
	bOpen = 0;
	bOpenRtsp = 0;
	return 1;
}

int CIiic::OpenRtsp()
{
	if(0 == bOpenRtsp)
	{
	bOpenRtsp = 1;
	gTrdRtsp2Bmp=AfxBeginThread(rtsp2bmp, NULL , THREAD_PRIORITY_NORMAL , 0 , CREATE_SUSPENDED);
	nOpenCnt ++;
	if (gTrdRtsp2Bmp == NULL)
	{
		return 0;
	}
	gTrdRtsp2Bmp->m_bAutoDelete = TRUE;
	gTrdRtsp2Bmp->ResumeThread();
	}
	return 1;
}

int CIiic::TakeBmpFromRtsp()
{
	if (0 == bOpenRtsp)
	{
		OpenRtsp();
		//return 0;
	}
	flag = 1;
	int i= 0;
	while(flag == 1)
	{
		i++;
		Sleep(1);
		if (i>2000)
		{
			return 0;
		}
		
	}
	return 1;
}

void CIiic::Save (char* bmpName) 
{
	SaveAsBMP (pFrameRGB, pCodecCtx->width, pCodecCtx->height, bmpName, 24);
}

int CIiic::Show(bool open)
{
	if(open)
	{
	if (bOpenShow == 1)
	{
		
		return 2;
	}
	if (0 == bOpenRtsp)
	{
		OpenRtsp();
		return 0;
	}
	stuTrd->pDC = stuTrd->pWnd->GetDC();
	stuTrd->pWnd->GetClientRect(&stuTrd->rect);
	stuTrd->pDC->SetStretchBltMode (HALFTONE);
	
	SetBrushOrgEx(stuTrd->pDC->m_hDC, 0, 0, NULL);
	if(stuTrd->pDC == NULL)
		return 0;
	bOpenShow=1;
	gTrdRtspShow=AfxBeginThread(rtspShow, stuTrd , THREAD_PRIORITY_NORMAL , 0 , CREATE_SUSPENDED);
	if (gTrdRtspShow == NULL)
	{
		return 0;
	}
	gTrdRtspShow->m_bAutoDelete = FALSE;
	gTrdRtspShow->ResumeThread();
	return 1;
	}
	else
	{
		CloseShow();//bOpenShow = 0;
		return 1;
	}
}
int CIiic::CloseShow()
{
	flag=0;
	bOpenShow = 0;
	//nShowFlag = 0;
	DWORD dwCode = 0;
	if (NULL == gTrdRtspShow)
	{
		return 1;
	}
	dwCode = WaitForSingleObject(gTrdRtspShow->m_hThread, INFINITE); 
	
	/*if ( dwCode == WAIT_TIMEOUT )
	{                    
	}
	else
	{
		do 
		{
			Sleep(1);
			GetExitCodeThread(gTrdRtspShow->m_hThread , &dwCode);

		} while (dwCode == STILL_ACTIVE);
	}*/
	
	gTrdRtspShow->Delete();
	gTrdRtspShow = NULL;
	Sleep(500);
	return 1;
}