// DlgView.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_CountCircle.h"
#include "DlgView.h"
#include "stdio.h"
#include <deque>
#define cvQueryHistValue_1D( hist, idx0 ) \
	((float)cvGetReal1D( (hist)->bins, (idx0)))
#define PI 3.1415926535897932384626433832795
#define DTORAD(A) PI*A/180.0
#define EPS 0.00001 
#define  FILTER 5
#define RADIUS 192
CvvImage m_image;
IplImage* img;//图像
IplImage* img1;//图像1
IplImage* img2;//图像2
std::deque<int> DataWhite,DataBlack;
std::deque<double> DeqApertureNum;
std::deque<CvRect>DeRect;
int gR=200;
int gFilter =5;
double gDbEffectiveCoverage = 0.95;
int gnShape = 0;
struct DetectionRegionStruct
{
	int CentreX;
	int CentreY;
	int Radius;
}DetectionRegion;
struct IdentifyAreasCircleStruct
{
	int CentreX;
	int CentreY;
	int Radius;
}IdentifyAreasCircle;
typedef struct  
{
	float x;
	float y;
}PointF;
// CDlgView

IMPLEMENT_DYNCREATE(CDlgView, CFormView)

CDlgView::CDlgView()
	: CFormView(IDD_DIALOG_MAIN)
{
  m_bmp = "";
}

CDlgView::~CDlgView()
{
}

void CDlgView::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDIT_EFFECTIVE, m_dbEffective);
  DDV_MinMaxDouble(pDX, m_dbEffective, 0.1, 1);
  DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nChipHeight);
  DDV_MinMaxInt(pDX, m_nChipHeight, 1, 800);
  DDX_Text(pDX, IDC_EDIT_RADIUS, m_nChipRadius);
  DDV_MinMaxInt(pDX, m_nChipRadius, 1, 400);
  DDX_Text(pDX, IDC_EDIT_WIDTH, m_nChipWidth);
  DDV_MinMaxInt(pDX, m_nChipWidth, 1, 800);
  DDX_Radio(pDX, IDC_RAD_SHAPE1, m_nRadShape);
  DDX_Text(pDX, IDC_EDIT_PARAM, m_fChipParam);
	DDV_MinMaxDouble(pDX, m_fChipParam, 0, 100);
}

BEGIN_MESSAGE_MAP(CDlgView, CFormView)
  ON_WM_CREATE()
  ON_WM_DESTROY()
  ON_BN_CLICKED(IDOK, &CDlgView::OnBnClickedOk)
  ON_BN_CLICKED(IDC_RAD_SHAPE1, &CDlgView::OnBnClickedRadShape)
  ON_BN_CLICKED(IDC_RAD_SHAPE2, &CDlgView::OnBnClickedRadShape)
  ON_WM_SHOWWINDOW()
  ON_BN_CLICKED(IDC_UPDATA_BTN, &CDlgView::OnBnClickedUpdataBtn)
END_MESSAGE_MAP()


// CDlgView 诊断

#ifdef _DEBUG
void CDlgView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDlgView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDlgView 消息处理程序


void CDlgView::OnDraw(CDC* pDC)
{
  // TODO: 在此添加专用代码和/或调用基类
  //if (pDC == GetDlgItem(IDC_PICTURE_SHOW)->GetDC() && m_bmp != "")
  {
    ShowLocalPicture(m_bmp, GetDlgItem(IDC_PICTURE_SHOW));
  }
}


int CDlgView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFormView::OnCreate(lpCreateStruct) == -1)
    return -1;

  // TODO:  在此添加您专用的创建代码
  m_dbEffective = 0.95;
  m_nChipHeight = 1;
  m_nChipRadius = RADIUS;//2楼180
  m_nChipWidth = 1;
  m_fChipParam = 1;
  m_nRadShape = 0;
  DetectionRegion.CentreX=RADIUS;//2楼180
  DetectionRegion.CentreY=RADIUS;//2楼180
  DetectionRegion.Radius=RADIUS;//2楼180
  m_fontResult.CreatePointFont(500, _T("黑体"), NULL);
  m_paramIniPath = "./param.ini";


  CString csTmp;
  GetPrivateProfileString(_T("识别参数"), _T("有效区域"), 0, csTmp.GetBuffer(MAX_PATH), MAX_PATH, m_paramIniPath);
  csTmp.ReleaseBuffer();
  m_dbEffective = _wtof(csTmp.GetBuffer());
  csTmp.ReleaseBuffer();
  m_nChipHeight = GetPrivateProfileInt(_T("识别参数"), _T("长"), 0, m_paramIniPath);
  m_nChipRadius = GetPrivateProfileInt(_T("识别参数"), _T("直径"), 0, m_paramIniPath);
  m_nChipWidth = GetPrivateProfileInt(_T("识别参数"), _T("宽"), 0, m_paramIniPath);
  m_nRadShape = GetPrivateProfileInt(_T("识别参数"), _T("圆形"), 0, m_paramIniPath);
  GetPrivateProfileString(_T("识别参数"), _T("比例系数"), 0, csTmp.GetBuffer(MAX_PATH), MAX_PATH, m_paramIniPath);
  csTmp.ReleaseBuffer();
  m_fChipParam = _wtof(csTmp.GetBuffer());
  csTmp.ReleaseBuffer();

 
  return 0;
}


void CDlgView::OnDestroy()
{
  CFormView::OnDestroy();

  // TODO: 在此处添加消息处理程序代码

}


void CDlgView::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
}
void CDlgView::OnBnClickedRadShape()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_nRadShape==0)//圆
	{
		GetDlgItem(IDC_STA_RADIUS)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_RADIUS)->ShowWindow(TRUE);

		GetDlgItem(IDC_STA_WIDTH)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_WIDTH)->ShowWindow(FALSE);
		GetDlgItem(IDC_STA_HEIGHT)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_HEIGHT)->ShowWindow(FALSE);
	}
	else
	{

		GetDlgItem(IDC_STA_RADIUS)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_RADIUS)->ShowWindow(FALSE);

		GetDlgItem(IDC_STA_WIDTH)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_WIDTH)->ShowWindow(TRUE);
		GetDlgItem(IDC_STA_HEIGHT)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_HEIGHT)->ShowWindow(TRUE);

	}
}
// 创建灰度图像的直方图  
CvHistogram* CreateGrayImageHist(IplImage **ppImage)  
{  
	int nHistSize = 256;  
	float fRange[] = {0, 255};  //灰度级的范围    
	float *pfRanges[] = {fRange};    
	CvHistogram *pcvHistogram = cvCreateHist(1, &nHistSize, CV_HIST_ARRAY, pfRanges);  
	cvCalcHist(ppImage, pcvHistogram);  
	return pcvHistogram;  
}  
void FillWhite(IplImage *pImage)  
{  
	cvRectangle(pImage, cvPoint(0, 0), cvPoint(pImage->width, pImage->height), CV_RGB(255, 255, 255), CV_FILLED);  
}  
// 根据直方图创建直方图图像  
IplImage* CreateHisogramImage(int nImageWidth, int nScale, int nImageHeight, CvHistogram *pcvHistogram)  
{  
	IplImage *pHistImage = cvCreateImage(cvSize(nImageWidth * nScale, nImageHeight), IPL_DEPTH_8U, 1);  
	FillWhite(pHistImage);  
	int max =0;
	int max_i=0;;
	double sum1=0,sum2=0;
	double aver1,aver2;
	//统计直方图中的最大直方块  
	float fMaxHistValue = 0;  
	cvGetMinMaxHistValue(pcvHistogram, NULL, &fMaxHistValue, NULL, NULL);  
	float HistValue[256];
	//分别将每个直方块的值绘制到图中  
	int i;  
	for(i = 0; i < nImageWidth; i++)  
	{  
		float fHistValue = cvQueryHistValue_1D(pcvHistogram, i); //像素为i的直方块大小  		
		int nRealHeight = cvRound((fHistValue / fMaxHistValue) * nImageHeight);  //要绘制的高度  
		HistValue[i] =fHistValue;
		cvRectangle(pHistImage,  
			cvPoint(i * nScale, nImageHeight - 1),  
			cvPoint((i + 1) * nScale - 1, nImageHeight - nRealHeight),  
			cvScalar(i, 0, 0, 0),   
			CV_FILLED  
			);   
		if(max<HistValue[i]&&i>0)
		{
			max=HistValue[i];
			max_i=i;
		}
		if(i<70&&i>=1)
			sum1+=HistValue[i];
		else if(i>=70)
			sum2+=HistValue[i];
		OutputDebugPrintf("[%d]=%f\n",i,HistValue[i]);
	}  
	aver1=sum1/69;
	aver2=sum2/185;
	//OutputDebugPrintf("max[%d]=%d,aver1=%f,aver2=%f\n",max_i,max,aver1,aver2);
	OutputDebugPrintf("aver1=%f,aver2=%f,max_i=%d\n",aver1,aver2,max_i);
	return pHistImage;  
}  
IplImage* NoiseCancelling(IplImage* src)
{		
	CvScalar s[9];
	double averg;
	for(int i=1;i<m_image.Height()-1;i++)
	{
		for(int j=1;j<m_image.Width()-1;j++)
		{
			s[0]=cvGet2D(src,i,j);
			s[1]=cvGet2D(src,i-1,j);s[2]=cvGet2D(src,i,j-1);s[3]=cvGet2D(src,i-1,j-1);s[4]=cvGet2D(src,i+1,j+1);
			s[5]=cvGet2D(src,i+1,j);s[6]=cvGet2D(src,i,j+1);s[7]=cvGet2D(src,i-1,j+1);s[8]=cvGet2D(src,i+1,j-1);
			averg= 0;
			averg=(s[1].val[0]+s[2].val[0]+s[3].val[0]+s[4].val[0]+s[5].val[0]+s[6].val[0]+s[7].val[0]+s[8].val[0])/8;
			if(abs(averg-s[0].val[0])>127.5)
			{
				s[0].val[0]=255-s[0].val[0];
				s[0].val[1]=255-s[0].val[1];
				s[0].val[2]=255-s[0].val[2];
			}
			cvSet2D(src,i,j,s[0]);
		}
	}
	return src;

}
//用来填充物体内细小空洞、连接邻近物体、平滑其边界的同时并不明显改变其面积。
IplImage* Closing(IplImage* src)//先膨胀后腐蚀 闭运算
{
	// TODO: 在此添加命令处理程序代码
	IplImage* dst1=cvCreateImage(cvGetSize(src), src->depth, 1);
	IplImage* dst2=cvCreateImage(cvGetSize(src), src->depth, 1);

	long time1 = GetTickCount();
	cvDilate(src,dst1,NULL,1);
	cvErode(dst1,dst2,NULL,1);
	long time2 = GetTickCount();
	OutputDebugPrintf("闭运算时间:%dms\n",time2-time1);
	return dst2;
}

void CDlgView::MeasureAperture()
{
	//UpdateData(TRUE);
	gnShape = m_nRadShape;
	if (m_nRadShape==0)//圆
	{

    IdentifyAreasCircle.Radius = (double)(m_nChipRadius) * m_fChipParam;
	}
	else
	{
		return;
	}
	gDbEffectiveCoverage = m_dbEffective;

	img = m_image.GetImage();	
	if (!(img->width==RADIUS*2&&img->height==RADIUS*2))
	{
		
		img1 = cvCreateImage(cvSize(RADIUS*2,RADIUS*2),img->depth,img->nChannels);
		Imagecutoff(img,img1,86,45,RADIUS*2,RADIUS*2);//175,53
		m_image.CopyOf(img1);
		cvReleaseImage(&img1);
	}

	FigureOut(m_image.GetImage());
	img = m_image.GetImage();
	cvSmooth( img, img, CV_GAUSSIAN, 5, 5); 

	if (img->nChannels == 1)
	{
		img1 = cvCloneImage (img);
	}
	else
	{
		img1 = cvCreateImage(cvGetSize(img),img->depth,1);
		cvCvtColor (img, img1, CV_RGB2GRAY); //RGB转灰度
	}

	cvThreshold(img1,img1,100,255.0,CV_THRESH_BINARY);
	img1 = NoiseCancelling(img1);
	img1 = NoiseCancelling(img1);

	m_image.CopyOf(img1);
	cvReleaseImage(&img1);
	Mark();	
	ScanImage();
  CString prevApertureNum;
  GetDlgItemText(IDC_STA_NUMBER_OF_APERTURE, prevApertureNum);
  if(m_csApertureNum != prevApertureNum)
	  GetDlgItem(IDC_STA_NUMBER_OF_APERTURE)->SetWindowText(m_csApertureNum);
}
void CDlgView::Imagecutoff(IplImage* src,IplImage* dst,int x,int y,int width,int height)
{
	
	cvSetImageROI(src,cvRect(x,y,width,height));  
/*	dst = cvCreateImage(cvSize(width,height),  
		IPL_DEPTH_8U,  
		src->nChannels); */ 
	cvCopy(src,dst,0);  
	cvResetImageROI(src);  
}
void CDlgView::CalculateApertureNum()
{
	double DbApertureNum;
	int nBlackAperture=0,nWhiteAperture=0,npos=0;
	/*double max=0;
	for (int i=0;i<DataBlack.size();i++)
	{
		if (DataBlack.at(i)>max)
		{
			max = DataBlack.at(i);
			npos = i;
		}
	}
	if (npos>DataBlack.size()/2)
	{
		nBlackAperture = npos+1;
	}
	else
	{
		nBlackAperture = DataBlack.size()-npos;
	}
	for (int i=0;i<DataBlack.size();i++)
	{
		if (DataBlack.at(i)>max*0.9)
		{
			nBlackAperture--;
			break;
		}
	}
	max=0;
	npos=0;
	for (int i=0;i<DataWhite.size();i++)
	{
		if (DataWhite.at(i)>max)
		{
			max = DataWhite.at(i);
			npos = i;
		}
	}
	if (npos>DataWhite.size()/2)
	{
		nWhiteAperture = npos+1;
	}
	else
	{
		nWhiteAperture = DataWhite.size()-npos;
	}
	for (int i=0;i<DataWhite.size();i++)
	{
		if (DataWhite.at(i)>max*0.9)
		{
			nWhiteAperture--;
			break;
		}
	}*/
	double sum=0,aver=0;
	for (int i=0;i<DataBlack.size();i++)
	{
		sum+=DataBlack.at(i);		
	}
	aver = sum/DataBlack.size();
	double FilterCoef;
	FilterCoef = DataBlack.size()*0.005+0.1;
	FilterCoef = FilterCoef>0.2?0.2:FilterCoef;
	for (int i=0;i<DataBlack.size();i++)
	{
		if (DataBlack.at(i)>=aver*FilterCoef||(i==DataBlack.size()-1)&&DataBlack.at(i)>2)
		{
			nBlackAperture++;
		}			
	}

	sum=0,aver=0;
	for (int i=0;i<DataWhite.size();i++)
	{
		sum+=DataWhite.at(i);		
	}
	aver = sum/DataWhite.size();
	FilterCoef = DataWhite.size()*0.005+0.1;
	FilterCoef = FilterCoef>0.2?0.2:FilterCoef;
	for (int i=0;i<DataWhite.size();i++)
	{
		if (DataWhite.at(i)>=aver*FilterCoef||(i==DataWhite.size()-1)&&DataWhite.at(i)>2)
		{
			nWhiteAperture++;
		}			
	}
	DbApertureNum =(double)(nWhiteAperture+nBlackAperture)/2;
	DeqApertureNum.push_back(DbApertureNum);
}
void CDlgView::ScanImage(void)
{
	DeqApertureNum.clear();
	img = m_image.GetImage();
	img2 = cvCreateImage(cvGetSize(img),img->depth,3);
	img1 = cvCreateImage(cvGetSize(img),img->depth,1);
	cvCvtColor(img,img2,CV_GRAY2RGB) ;
	if (img->nChannels == 1)
	{
		//img1 = cvCloneImage (img);
		cvCopy(img,img1);
	}
	else
	{
		cvCvtColor (img, img1, CV_RGB2GRAY); //RGB转灰度
	}
//	img1 = cvCloneImage(img); 	

	//1.确定玻璃边界圆，获取圆心坐标
	//2.按圆心进行各方向扫描

	
	//IdentifyAreasCircle.CentreX=DetectionRegion.CentreX;
	//IdentifyAreasCircle.CentreY=DetectionRegion.CentreY;
	//IdentifyAreasCircle.Radius=DetectionRegion.Radius*gDbEffectiveCoverage;

	int sx=0,sy=0,ex=0,ey=0;
	if (DeRect.size()>0)
	{
		sx = DeRect.at(0).x+DeRect.at(0).width/2;
		sy =DeRect.at(0).y+DeRect.at(0).height/2;
	}
	else
	{
		sx = IdentifyAreasCircle.CentreX;
		sy = IdentifyAreasCircle.CentreY;
	}
	double  DbApertureNum=0;

	int cnt=0;
	int Color1 = 0;
	int nWhiteFilter=0,nBlackFilter=0;
	int XLimit = DetectionRegion.CentreX+DetectionRegion.Radius;
	int YLimit = DetectionRegion.CentreY+DetectionRegion.Radius;
	for (int angle=0;angle<360;angle+=5)
	{		
		DataWhite.clear();
		DataBlack.clear();
		//gR = DetectionRegion.Radius*gDbEffectiveCoverage;
		gR = IdentifyAreasCircle.Radius*gDbEffectiveCoverage;
		int nR;
    CvPoint2D32f Inter1;
		/*int bRet =LineInterCircle(cvPointTo32f(cvPoint(sx,sy)),cvPointTo32f(cvPoint(sx+(int)(gR*cos(DTORAD(angle))),sy-(int)(gR*sin(DTORAD(angle))))),
		cvPointTo32f(cvPoint(IdentifyAreasCircle.CentreX,IdentifyAreasCircle.CentreX)),IdentifyAreasCircle.Radius,&Inter1,&Inter2);*/
		int bRet = getPoint((double)IdentifyAreasCircle.CentreX,(double)IdentifyAreasCircle.CentreY,(double)IdentifyAreasCircle.Radius*gDbEffectiveCoverage,
			(double)sx,(double)sy,
			(double)(sx+(int)(gR*cos(DTORAD(angle)))),(double)(sy-(int)(gR*sin(DTORAD(angle)))),
			&Inter1);
		if (bRet==1)
		{
			nR =  (int)sqrt((long double)((Inter1.x-sx)*(Inter1.x-sx)+(Inter1.y-sy)*(Inter1.y-sy)));
			cvLine(img2,cvPoint(sx,sy),cvPointFrom32f(Inter1),CV_RGB( 0,255,0));
		}
		else
		{
			nR = gR;
			cvLine(img2,cvPoint(sx,sy),cvPoint(sx+(int)(gR*cos(DTORAD(angle))),sy-(int)(gR*sin(DTORAD(angle)))),CV_RGB( 0,255,0));
		}
		//cvLine(img2,cvPoint(sx,sy),cvPoint(sx+(int)(gR*cos(DTORAD(angle))),sy-(int)(gR*sin(DTORAD(angle)))),CV_RGB( 0,255,0));
		for (int i=0;i<nR;i++)
		{		
			int CurX = sx+i*cos(DTORAD(angle));
			int CurY = sy-i*sin(DTORAD(angle));			
			if (CurX<0||CurX>=XLimit||CurY<0||CurY>=YLimit)
			{
				break;
			}
			if (i ==0)
			{
				Color1=CV_IMAGE_ELEM(img1,byte,sy,sx);
				cnt++;
			}
			else
			{
				int p[FILTER];
        int row=0,col=0;
				for(int j=0;j<FILTER;j++)
				{
          row = sy - (int)((i + j - FILTER / 2)*sin(DTORAD(angle)));
          col = sx + (int)((i + j - FILTER / 2)*cos(DTORAD(angle)));
          row = row >= 0 ? row : 0;
          col = col >= 0 ? col : 0;
					p[j]=CV_IMAGE_ELEM(img1,byte, row, col);
				}
				CvPoint CenPoint;
				CenPoint.x = sx+(int)(i*cos(DTORAD(angle)));
				CenPoint.y = sy-(int)(i*sin(DTORAD(angle)));
				//cvCircle(img2,CenPoint,1,CV_RGB( 0,255,0),1);
				double pSum=0;
				for (int k=0;k<FILTER;k++)
				{
					pSum+=p[k];
				}
				pSum/=255.0;
				if(pSum<=FILTER*0.4)
				{			
					if(Color1>=1)
					{				
						if (!(DataWhite.size()==0&&cnt == 1))
						{
							DataWhite.push_back(cnt);
						}				
						cnt = 0;
					}

					cnt++;								
					Color1 = 0;
				}
				else if(pSum>=FILTER*0.6)
				{				
					if(Color1==0)
					{
						if (!(DataBlack.size()==0&&cnt == 1))
						{
							DataBlack.push_back(cnt);
						}						
						cnt = 0;				
					}
					cnt++;
					Color1 = 1;
				}
				if (i==gR-1)
				{
					if(Color1==0&&cnt>2)
					{						
						DataBlack.push_back(cnt);										
					}
					else if(Color1>=1&&cnt>2)
					{
						DataWhite.push_back(cnt);
					}
				}
			}
		}		
		CalculateApertureNum();
	}

	//m_image.Load("./cap3.bmp");
	//img = m_image.GetImage();
	//if (!(img->width==360&&img->height==360))
	//{
	//	
	//	img=Imagecutoff(img,175,53,360,360);
	//}
	//cvCircle(img,cvPoint(IdentifyAreasCircle.CentreX,IdentifyAreasCircle.CentreY),IdentifyAreasCircle.Radius,CV_RGB(0,255,0),1);
	//cvShowImage("a",img);
	cvCircle(img2,cvPoint(IdentifyAreasCircle.CentreX,IdentifyAreasCircle.CentreY),IdentifyAreasCircle.Radius,CV_RGB(0,255,0),1);
	m_image.CopyOf(img2);
	cvReleaseImage(&img1);
	cvReleaseImage(&img2);
	//cvShowImage("a",img2);
	double DbMax = 1000;
	int nMaxNum=0;//连续最大值个数
	double DbMax2 = 0;
	int j=0;
	while (nMaxNum<4&&j<10)
	{
		nMaxNum = 0;
		DbMax2 = 0;
		for(int i=0;i<DeqApertureNum.size();i++)
		{
			if (DeqApertureNum.at(i)>DbMax2&&DeqApertureNum.at(i)<DbMax)
			{
				if (DeqApertureNum.at(i)>DbMax2)
				{
					DbMax2 =DeqApertureNum.at(i); 			
				}	
			}	
		}
		DbMax = DbMax2;
		for (int i=0;i<DeqApertureNum.size();i++)
		{
			if (DeqApertureNum.at(i)==DbMax)
			{
				nMaxNum++;
				if (nMaxNum>4)
				{
					break;
				}
				
			}
			else
				nMaxNum=0;
		}
		j++;
	}
	
	
	m_csApertureNum.Format(_T("%.1f"),DbMax);
	//AfxMessageBox(csApertureNum);
}
/// <summary>  
/// 线段与圆的交点  
/// </summary>  
/// <param name="ptStart">线段起点</param>  
/// <param name="ptEnd">线段终点</param>  
/// <param name="ptCenter">圆心坐标</param>  
/// <param name="Radius2">圆半径平方</param>  
/// <param name="ptInter1">交点1(若不存在返回65536)</param>  
/// <param name="ptInter2">交点2(若不存在返回65536)</param>  
int CDlgView::LineInterCircle(CvPoint2D32f ptStart, CvPoint2D32f ptEnd, CvPoint2D32f ptCenter, double Radius2,  CvPoint2D32f* ptInter1,  CvPoint2D32f* ptInter2)  
{ 
	/*ptInter1->x = ptInter2->x = 65536.0f;  
	ptInter1->y = ptInter2->y = 65536.0f;  

	float fDis = (float)sqrt((long double)((ptEnd.x - ptStart.x) * (ptEnd.x - ptStart.x) + (ptEnd.y - ptStart.y) * (ptEnd.y - ptStart.y)));  
	PointF d;  
	d.x = (ptEnd.x - ptStart.x) / fDis;  
	d.y = (ptEnd.y - ptStart.y) / fDis;  
	PointF E;
	E.x = ptCenter.x - ptStart.x;  
	E.y = ptCenter.y - ptStart.y;  
	float a = E.x * d.x + E.y * d.y;  
	float a2 = a * a;  
	float e2 = E.x * E.x + E.y * E.y;  
	if ((Radius2 - e2 + a2) < 0)  
	{  
		return false;  
	}  
	else  
	{  
		float f = (float)sqrt((long double)(Radius2 - e2 + a2));  
		float t = a - f;  
		if (((t - 0.0) > -EPS) && (t - fDis) < EPS)  
		{  
			ptInter1->x = ptStart.x + t * d.x;  
			ptInter1->y = ptStart.y + t * d.y;  
		}  
		t = a + f;  
		if (((t - 0.0) > -EPS) && (t - fDis) < EPS)  
		{  
			ptInter2->x = ptStart.x + t * d.x;  
			ptInter2->y = ptStart.y + t * d.y;  
		}  
		return true;  
	}  */
	int ret=0;
	ptInter1->x = ptInter2->x = 65536.0f;  
	ptInter1->y = ptInter2->y = 65536.0f;  

	float fDis = sqrt((long double)((ptEnd.x - ptStart.x) * (ptEnd.x - ptStart.x) + (ptEnd.y - ptStart.y) * (ptEnd.y - ptStart.y)));  

	CvPoint2D32f d;  
	d.x = (ptEnd.x - ptStart.x) / fDis;  
	d.y = (ptEnd.y - ptStart.y) / fDis;  

	CvPoint2D32f E;  
	E.x = ptCenter.x - ptStart.x;  
	E.y = ptCenter.y - ptStart.y;  

	float a = E.x * d.x + E.y * d.y;  
	float a2 = a * a;  

	float e2 = E.x * E.x + E.y * E.y;  

	float r2 = Radius2 * Radius2;  

	if ((r2 - e2 + a2) < 0)  
	{  
		return 0;  
	}  
	else  
	{  
		float f = sqrt(r2 - e2 + a2);  

		float t = a - f;  

		if( ((t - 0.0) > - EPS) && (t - fDis) < EPS)  
		{  
			ptInter1->x = ptStart.x + t * d.x;  
			ptInter1->y = ptStart.y + t * d.y;  
		}         

		t = a + f;  

		if( ((t - 0.0) > - EPS) && (t - fDis) < EPS)  
		{  
			ptInter2->x = ptStart.x + t * d.x;  
			ptInter2->y = ptStart.y + t * d.y;  
		}  
		if (ptInter1->x<1000&&ptInter1->y<1000&&ptInter1->x>=0&&ptInter1->y>0)
		{
			ret = 1;  
		}
		if (ptInter2->x<1000&&ptInter2->y<1000&&ptInter2->x>=0&&ptInter2->y>0)
		{
			if (ret==1)
			{
				ret = 3; 
			}
			else
			{
				ret = 2; 
			}
		}
		return ret;
	}  
}  

/**
 * Created by apple on 2017/1/15.
 */
//求直线与圆的交点
/*函数参数说明:
    cx:圆X轴坐标
    cy:圆y轴坐标
    r:圆半径
 stx:起点直线的X轴坐标
 sty:起点直线的轴坐标
 edx:终点直线的X轴坐标
 edy:终点直线的Y轴坐标

 返回值:
*/
int  CDlgView::getPoint(double cx,double cy,double r,double stx,double sty,double edx,double edy,CvPoint2D32f *p) 
{
	if(edx == stx)
	{
		if (edy<sty)
		{
			p->x =edx;
			//p->y =cy-(int)r; 
			p->y =cy-(int)sqrt(r*r-(stx-cx)*(stx-cx));
		}
		else
		{
			p->x =edx;
			//p->y =cy+(int)r;
			p->y =cy+(int)sqrt(r*r-(stx-cx)*(stx-cx));
		}
		
		return 1;
	}
   // 求直线
    double k = (edy - sty) / (edx - stx);
    double b = edy - k*edx;
    //列方程
    double x1,y1,x2,y2;
    double c = cx*cx + (b - cy)*(b- cy) -r*r;
    double a = (1 + k*k);
    double b1 = (2*cx - 2*k*(b - cy));

    double  tmp = sqrt((long double)(b1*b1 - 4*a*c));
    x1 = ( b1 + tmp )/(2*a);
    y1 = k*x1 + b;
    x2 = ( b1 - tmp)/(2*a);
    y2 = k*x2 + b;
//判断求出的点是否在圆上
    double res = (x1 -cx)*(x1 -cx) + (y1 - cy)*(y1 -cy);
    //if( res <= r*r+1&&res>= r*r-1)
	if(edx>stx)
    {
        p->x = x1;
        p->y = y1;
    }    
	else
    {
        p->x = x2;
        p->y = y2;
    }
	return 1;
}
void CDlgView::FigureOut(IplImage *src)//确定外轮廓
{
	if (gnShape==0)//圆
	{
		CvMat* circles;
		IplImage *dst = cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);


		if (src->nChannels == 1)
		{
			//dst = cvCloneImage (src);
			cvCopy(src,dst,0);
		}
		else
		{
			cvCvtColor (src, dst, CV_RGB2GRAY); //转为单通道
		}
		CvMemStorage* storage=cvCreateMemStorage(0);
		CvSeq* circle = 0;
		cvSmooth( dst, dst, CV_GAUSSIAN, 5, 5 ); //看了很多事例，要降噪处理，但测试的结果是
		//找圆会有一定偏差，说明用这个要因图而异，噪声高的图才要用
		circle = cvHoughCircles( //cvHoughCircles函数需要估计每一个像素梯度的方向，
			//因此会在内部自动调用cvSobel,而二值边缘图像的处理是比较难的
			dst,
			storage,
			CV_HOUGH_GRADIENT,
			1.0, //1 累加器图像的分辨率,增大则分辨率变小
			2, //18 很重要的一个参数，告诉两个圆之间的距离的最小距离，如果已知一副图像，可以先行计
			//算出符合自己需要的两个圆之间的最小距离。
			100, //100 canny算法的阈值上限，下限为一半（即100以上为边缘点，50以下抛弃，中间视是否相连而
			//定）
			IdentifyAreasCircle.Radius/6, //20 决定成圆的多寡 ，一个圆上的像素超过这个阈值，则成圆，否则丢弃
			IdentifyAreasCircle.Radius-IdentifyAreasCircle.Radius/100-1,//最小圆半径，这个可以通过图片确定你需要的圆的区间范围
			IdentifyAreasCircle.Radius+IdentifyAreasCircle.Radius/100+1 //最大圆半径
			);
		//圆圈

		std::deque<double> dOverlappixels;

		float* p = ( float* )cvGetSeqElem( circle, 0 );
		if (circle->total>0)
		{
			IdentifyAreasCircle.CentreX = p[0];
			IdentifyAreasCircle.CentreY = p[1];
		}
		//cvClearMemStorage(storage);
		cvReleaseMemStorage(&storage);
		cvReleaseImage(&dst);
	}
};


void CDlgView::Mark()	//连通区标记
{

	img = m_image.GetImage();

	IplImage* dst = NULL;
	//cvCvtColor(img,dst,CV_BGR2GRAY);
	if (img->nChannels == 1)
	{
		dst = cvCloneImage (img);
	}
	else
	{
		cvCvtColor (img, dst, CV_BGR2GRAY);
	}
	img1 = cvCreateImage(cvGetSize(img),img->depth,3);
	CvMemStorage* storage = cvCreateMemStorage(0);  
	CvSeq* contour = 0;
	int contour_num = cvFindContours(dst, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE  );
	int i;
	//CvScalar color[2]={(255,0,255),(0,255,255),(255,255,0)};
	int wai = 0;    
	int nei = 0;    
	CvSeq *pConInner = NULL;
	DeRect.clear();
	std::deque<double> deArea;
	for (; contour != NULL; contour = contour->h_next)     
	{     
		wai++;    
		double tmparea = fabs(cvContourArea(contour));  	
		if (tmparea < PI*IdentifyAreasCircle.Radius*IdentifyAreasCircle.Radius/10)  //10000
		{  
			
			//if (tmparea <150)
			//{
			//
			//	// PI*IdentifyAreasCircle.Radius*IdentifyAreasCircle.Radius
			//	cvDrawContours(img, contour, CV_RGB( 0, 0,0), CV_RGB( 0,0,0), -1, CV_FILLED, 0); 
			//}
			//删除面积小于设定值的轮廓  
			cvSeqRemove(contour, 0);   
		}  
		else
		{			
			CvRect aRect = cvBoundingRect(contour, 0);  
			DeRect.push_back(aRect);
			deArea.push_back(tmparea);
			//cvZero( img1 );
			CvScalar color = CV_RGB( 255, 0,0);
			cvDrawContours(img1, contour, CV_RGB( 255, 0,0), CV_RGB( 0,255,0), -1, CV_FILLED, 0); 
			CString Title;
			Title.Format(_T("%d+%d:Area=%f"),wai,nei,tmparea);
			//cvRectangle(img1,cvPoint(aRect.x,aRect.y),cvPoint(aRect.x+aRect.width,aRect.y+aRect.height), CV_RGB( 0,255,0),1);
			//cvShowImage(Title,img1);
		}
	}   
	cvClearMemStorage(storage);
	cvReleaseMemStorage(&storage);	
	cvReleaseImage(&img1);
	cvReleaseImage(&dst);
	m_image.CopyOf(img);


	//if (deArea.size()>0)
	//{
	//	std::deque<double> deAreaDiffer;
	//	double Areas ;
	//	if (gnShape==0)//圆
	//	{
	//		Areas = IdentifyAreasCircle.Radius*IdentifyAreasCircle.Radius*PI;
	//	}
	//	else
	//	{
	//		Areas = IdentifyAreasCircle.Radius*IdentifyAreasCircle.Radius*PI;
	//	}
	//	double Areatmp;
	//	for (int i=0;i<deArea.size();i++)
	//	{
	//		if (gnShape==0)//圆
	//		{
	//			Areatmp = abs(deArea.at(i)-Areas);
	//			deAreaDiffer.push_back(Areatmp);
	//		}
	//	}
	//	double min = 1000000;
	//	int nNum=0;
	//	for (int i=0;i<deAreaDiffer.size();i++)
	//	{
	//		if (min>deAreaDiffer.at(i))
	//		{
	//			min = deAreaDiffer.at(i);
	//			nNum = i;
	//		}
	//	}
	//	if (gnShape==0)//圆
	//	{
	//		IdentifyAreasCircle.CentreX = DeRect.at(nNum).x+DeRect.at(nNum).width/2;
	//		IdentifyAreasCircle.CentreY = DeRect.at(nNum).y+DeRect.at(nNum).height/2;
	//	}
	//}
	//else
	//{
	//	IdentifyAreasCircle.CentreX = DetectionRegion.CentreX;
	//	IdentifyAreasCircle.CentreY = DetectionRegion.CentreY;
	//}



}
void OutputDebugPrintf(const char * strOutputString,...)
{
	char strBuffer[4096]={0};
	va_list vlArgs;
	va_start(vlArgs,strOutputString);
	_vsnprintf(strBuffer,sizeof(strBuffer)-1,strOutputString,vlArgs);
	//vsprintf(strBuffer,strOutputString,vlArgs);
	va_end(vlArgs);
	OutputDebugStringA(strBuffer);
}


void CDlgView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFormView::OnShowWindow(bShow, nStatus);
	//OnBnClickedRadShape();
	GetDlgItem(IDC_STA_RADIUS)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDIT_RADIUS)->ShowWindow(TRUE);

	GetDlgItem(IDC_STA_WIDTH)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_WIDTH)->ShowWindow(FALSE);
	GetDlgItem(IDC_STA_HEIGHT)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_HEIGHT)->ShowWindow(FALSE);
	// TODO: Add your message handler code here
}


void CDlgView::OnBnClickedUpdataBtn()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

  CString csTmp;
  csTmp.Format(_T("%f"), m_dbEffective);
  WritePrivateProfileString(_T("识别参数"), _T("有效区域"), csTmp, m_paramIniPath);
  csTmp.Format(_T("%d"), m_nChipHeight);
  WritePrivateProfileString(_T("识别参数"), _T("长"), csTmp, m_paramIniPath);
  csTmp.Format(_T("%d"), m_nChipRadius);
  WritePrivateProfileString(_T("识别参数"), _T("直径"), csTmp, m_paramIniPath);
  csTmp.Format(_T("%d"), m_nChipWidth);
  WritePrivateProfileString(_T("识别参数"), _T("宽"), csTmp, m_paramIniPath);
  csTmp.Format(_T("%d"), m_nRadShape);
  WritePrivateProfileString(_T("识别参数"), _T("圆形"), csTmp, m_paramIniPath);
  csTmp.Format(_T("%f"), m_fChipParam);
  WritePrivateProfileString(_T("识别参数"), _T("比例系数"), csTmp, m_paramIniPath);
}

