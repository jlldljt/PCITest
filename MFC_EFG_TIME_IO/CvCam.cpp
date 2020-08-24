#include "StdAfx.h"
#include "CvCam.h"

CCvCam::CCvCam(void)
{
}

CCvCam::~CCvCam()
{
}

int CCvCam::init(HWND wnd)
{

  if (!m_cvCap.isOpened())
  {
    m_cvCap.open(0);//m_cvCap = 0; // 打开摄像头


    if (!m_cvCap.isOpened())
    {
      return -1;
    }
    //opencv 可以   4-3 8-6  16-12  32-24  320-240  640-480  960-720 1280-960

    m_cvCap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);

    m_cvCap.set(CV_CAP_PROP_FRAME_HEIGHT, 1024);
    double width = m_cvCap.get(CV_CAP_PROP_FRAME_WIDTH);

    double height = m_cvCap.get(CV_CAP_PROP_FRAME_HEIGHT);

    if (wnd) {
      m_hWND = wnd;
      // 获取设备上下文的句柄
      m_hDC = GetDC(wnd);

      // 获取绘制区域
      GetClientRect(wnd, &m_rect);
    }
  }
  return 0;
}

void CCvCam::unInit()
{
  if (m_cvCap.isOpened())
  {
    m_cvCap.release();
    m_cvCap = -1;

    if (m_hDC)
    {
      ReleaseDC(m_hWND, m_hDC);
      m_hDC = NULL;
    }
    m_hWND = NULL;
  }
}

int CCvCam::drawPicToHDC(IplImage* pImg)
{
  if (!pImg || !m_hWND)
  {
    OutputDebugString(_T("pImg is NULL!\n"));
    return -1;
  }

  m_bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  m_bmpInfo.bmiHeader.biHeight = pImg->height;
  m_bmpInfo.bmiHeader.biWidth = pImg->width;
  m_bmpInfo.bmiHeader.biPlanes = 1;
  m_bmpInfo.bmiHeader.biBitCount = 24;
  m_bmpInfo.bmiHeader.biCompression = BI_RGB;
  m_bmpInfo.bmiHeader.biSizeImage = pImg->imageSize;
  m_bmpInfo.bmiHeader.biXPelsPerMeter = 0;
  m_bmpInfo.bmiHeader.biYPelsPerMeter = 0;
  m_bmpInfo.bmiHeader.biClrUsed = 0;
  m_bmpInfo.bmiHeader.biClrImportant = 0;

  // 该函数可以设置指定输出设备环境中的位图拉伸模式
  SetStretchBltMode(m_hDC, COLORONCOLOR);

  ::StretchDIBits(
    m_hDC,
    // 默认绘制原点为左上角，X方向向右为正，Y方向向下为正
    // 加上负号表明X或者Y方向取反
    //m_rect.Width(), m_rect.Height(), -m_rect.Width(), -m_rect.Height(),
    0, m_rect.Height(),m_rect.Width(), -m_rect.Height(), //-m_rect.Width(), -m_rect.Height(),
    0, 0, m_bmpInfo.bmiHeader.biWidth, m_bmpInfo.bmiHeader.biHeight,
    pImg->imageData, (PBITMAPINFO)& m_bmpInfo, DIB_RGB_COLORS, SRCCOPY);

  return 0;
}
int CCvCam::savePic(const char* path,IplImage* pImg)
{
  if (!pImg || !path)
  {
    OutputDebugString(_T("pImg is NULL!\n"));
    return -1;
  }

  cvSaveImage(path, pImg);

  return 0;
}

int CCvCam::snap(void)
{
  if (!m_cvCap.isOpened())
  {
    return -1;
  }

  cv::Mat frame;
  m_cvCap >> frame;

  if (!frame.empty())
  {
    IplImage img = IplImage(frame);
    drawPicToHDC(&img);

   // savePic("img.bmp", &img);
  }
  else
  {
    OutputDebugString(_T("queryFrame is Null!\n"));
  }

  return 0;
}
//
int CCvCam::captureBmp(const char* path)
{
  if (!m_cvCap.isOpened())
  {
    return -1;
  }

  cv::Mat frame;
  m_cvCap >> frame;

  if (!frame.empty())
  {
    IplImage img = IplImage(frame);
   // drawPicToHDC(&img);

    savePic(path, &img);
    return 0;
  }
  else
  {
    OutputDebugString(_T("queryFrame is Null!\n"));
    return -1;
  }

}

