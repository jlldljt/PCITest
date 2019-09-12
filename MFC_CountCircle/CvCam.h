#pragma once


#include "opencv2\opencv.hpp"
#include "opencv\cv.h"

class CCvCam
{
public:
  cv::VideoCapture    m_cvCap;
  BITMAPINFO          m_bmpInfo;
  //CDC                 m_memDC;
  HWND                m_hWND;
  HDC                 m_hDC;
  //CBitmap             m_bmp;
  CRect               m_rect;

public:
  CCvCam(void);   // 标准构造函数
  virtual ~CCvCam();

  void init(HWND wnd);
  void unInit();
  int drawPicToHDC(IplImage* pImg);
  int savePic(const char* path, IplImage* pImg);
  void snap(void);
  void captureBmp(const char* path);

};

