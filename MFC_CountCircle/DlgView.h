#pragma once
#include "afxwin.h"



// CDlgView 窗体视图

class CDlgView : public CFormView
{
	DECLARE_DYNCREATE(CDlgView)

protected:
	CDlgView();           // 动态创建所使用的受保护的构造函数
	virtual ~CDlgView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAIN};
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
  CString m_bmp;
  CFont m_fontResult;
  CString m_paramIniPath;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
  virtual void OnDraw(CDC* /*pDC*/);
public:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnDestroy();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedRadShape();
  double m_dbEffective;
  int m_nChipHeight;
  int m_nChipRadius;
  int m_nChipWidth;
  int m_nRadShape;
  void MeasureAperture();
  void Mark();
  void FigureOut(IplImage *src);
  void CalculateApertureNum();
  int  getPoint(double cx,double cy,double r,double stx,double sty,double edx,double edy,CvPoint2D32f *p) ;
  int LineInterCircle(CvPoint2D32f ptStart, CvPoint2D32f ptEnd, CvPoint2D32f ptCenter, double Radius2,  CvPoint2D32f* ptInter1,  CvPoint2D32f* ptInter2) ;
  void ScanImage(void);
  void Imagecutoff(IplImage* src,IplImage* dst,int x,int y,int width,int height);
  afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
  CString m_csApertureNum;
  afx_msg void OnBnClickedUpdataBtn();
  double m_fChipParam;
};
void OutputDebugPrintf(const char * strOutputString,...);


extern CvvImage m_image;