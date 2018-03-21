#include "StdAfx.h"
#include "CvvImage.h"

//#pragma comment(lib,"opencv_calib3d248.lib") 
//#pragma comment(lib,"opencv_contrib248.lib") 
//#pragma comment(lib,"opencv_core248.lib") 
//#pragma comment(lib,"opencv_flann248.lib") 
//#pragma comment(lib,"opencv_gpu248.lib") 
//#pragma comment(lib,"opencv_highgui248.lib") 
//#pragma comment(lib,"opencv_imgproc248.lib") 
//#pragma comment(lib,"opencv_legacy248.lib") 
//#pragma comment(lib,"opencv_ml248.lib") 
//#pragma comment(lib,"opencv_objdetect248.lib") 
//#pragma comment(lib,"opencv_ts248.lib") 
//#pragma comment(lib,"opencv_video248.lib") 
//#pragma comment(lib,"opencv_calib3d248d.lib") 
//#pragma comment(lib,"opencv_contrib248d.lib") 
//#pragma comment(lib,"opencv_core248d.lib") 
//#pragma comment(lib,"opencv_flann248d.lib") 
//#pragma comment(lib,"opencv_gpu248d.lib") 
//#pragma comment(lib,"opencv_highgui248d.lib") 
//#pragma comment(lib,"opencv_imgproc248d.lib") 
//#pragma comment(lib,"opencv_legacy248d.lib") 
//#pragma comment(lib,"opencv_ml248d.lib") 
//#pragma comment(lib,"opencv_objdetect248d.lib") 
//#pragma comment(lib,"opencv_ts248d.lib") 
//#pragma comment(lib,"opencv_video248d.lib") 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CV_INLINE RECT NormalizeRect( RECT r );
CV_INLINE RECT NormalizeRect( RECT r )
{
   int t;
   if( r.left > r.right )
   {
      t = r.left;
      r.left = r.right;
      r.right = t;
   }
   if( r.top > r.bottom )
   {
      t = r.top;
      r.top = r.bottom;
      r.bottom = t;
   }

   return r;
}
CV_INLINE CvRect RectToCvRect( RECT sr );
CV_INLINE CvRect RectToCvRect( RECT sr )
{
   sr = NormalizeRect( sr );
   return cvRect( sr.left, sr.top, sr.right - sr.left, sr.bottom - sr.top );
}
CV_INLINE RECT CvRectToRect( CvRect sr );
CV_INLINE RECT CvRectToRect( CvRect sr )
{
   RECT dr;
   dr.left = sr.x;
   dr.top = sr.y;
   dr.right = sr.x + sr.width;
   dr.bottom = sr.y + sr.height;

   return dr;
}
CV_INLINE IplROI RectToROI( RECT r );
CV_INLINE IplROI RectToROI( RECT r )
{
   IplROI roi;
   r = NormalizeRect( r );
   roi.xOffset = r.left;
   roi.yOffset = r.top;
   roi.width = r.right - r.left;
   roi.height = r.bottom - r.top;
   roi.coi = 0;

   return roi;
}
void  FillBitmapInfo( BITMAPINFO* bmi, int width, int height, int bpp, int origin )
{
   assert( bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));

   BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

   memset( bmih, 0, sizeof(*bmih));
   bmih->biSize = sizeof(BITMAPINFOHEADER);
   bmih->biWidth = width;
   bmih->biHeight = origin ? abs(height) : -abs(height);
   bmih->biPlanes = 1;
   bmih->biBitCount = (unsigned short)bpp;
   bmih->biCompression = BI_RGB;
   if( bpp == 8 )
   {
      RGBQUAD* palette = bmi->bmiColors;
      int i;
      for( i = 0; i < 256; i++ )
      {
         palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
         palette[i].rgbReserved = 0;
      }
   }
}
CvvImage::CvvImage()
{
   m_img = 0;
}
void CvvImage::Destroy()
{
   cvReleaseImage( &m_img );
}
CvvImage::~CvvImage()
{
   Destroy();
}
bool  CvvImage::Create( int w, int h, int bpp, int origin )
{
   const unsigned max_img_size = 10000;

   if( (bpp != 8 && bpp != 24 && bpp != 32) ||
      (unsigned)w >=  max_img_size || (unsigned)h >= max_img_size ||
      (origin != IPL_ORIGIN_TL && origin != IPL_ORIGIN_BL))
   {
      assert(0); // most probably, it is a programming error
      return false;
   }
   if( !m_img || Bpp() != bpp || m_img->width != w || m_img->height != h )
   {
      if( m_img && m_img->nSize == sizeof(IplImage))
         Destroy();
      /* prepare IPL header */
      m_img = cvCreateImage( cvSize( w, h ), IPL_DEPTH_8U, bpp/8 );
   }
   if( m_img )
      m_img->origin = origin == 0 ? IPL_ORIGIN_TL : IPL_ORIGIN_BL;
   return m_img != 0;
}
void  CvvImage::CopyOf( CvvImage& image, int desired_color )
{
   IplImage* img = image.GetImage();
   if( img )
   {
      CopyOf( img, desired_color );
   }
}
#define HG_IS_IMAGE(img)\
   ((img) != 0 && ((const IplImage*)(img))->nSize == sizeof(IplImage) &&\
   ((IplImage*)img)->imageData != 0)
void  CvvImage::CopyOf( IplImage* img, int desired_color )
{
   if( HG_IS_IMAGE(img) )
   {
      int color = desired_color;
      CvSize size = cvGetSize( img );
      if( color < 0 )
         color = img->nChannels > 1;
      if( Create( size.width, size.height,
         (!color ? 1 : img->nChannels > 1 ? img->nChannels : 3)*8,
         img->origin ))
      {
         cvConvertImage( img, m_img, 0 );
      }
   }
}
bool  CvvImage::Load( const char* filename, int desired_color )
{
   IplImage* img = cvLoadImage( filename, desired_color );
   if( !img )
      return false;

   CopyOf( img, desired_color );
   cvReleaseImage( &img );

   return true;
}
bool  CvvImage::LoadRect( const char* filename,
                   int desired_color, CvRect r )
{
   if( r.width < 0 || r.height < 0 ) return false;

   IplImage* img = cvLoadImage( filename, desired_color );
   if( !img )
      return false;
   if( r.width == 0 || r.height == 0 )
   {
      r.width = img->width;
      r.height = img->height;
      r.x = r.y = 0;
   }
   if( r.x > img->width || r.y > img->height ||
      r.x + r.width < 0 || r.y + r.height < 0 )
   {
      cvReleaseImage( &img );
      return false;
   }
   /* truncate r to source image */
   if( r.x < 0 )
   {
      r.width += r.x;
      r.x = 0;
   }
   if( r.y < 0 )
   {
      r.height += r.y;
      r.y = 0;
   }
   if( r.x + r.width > img->width )
      r.width = img->width - r.x;

   if( r.y + r.height > img->height )
      r.height = img->height - r.y;
   cvSetImageROI( img, r );
   CopyOf( img, desired_color );
   cvReleaseImage( &img );
   return true;
}
bool  CvvImage::Save( const char* filename )
{
   if( !m_img )
      return false;
   cvSaveImage( filename, m_img );
   return true;
}
void  CvvImage::Show( const char* window )
{
   if( m_img )
      cvShowImage( window, m_img );
}
void  CvvImage::Show( HDC dc, int x, int y, int w, int h, int from_x, int from_y )
{
   if( m_img && m_img->depth == IPL_DEPTH_8U )
   {
      uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
      BITMAPINFO* bmi = (BITMAPINFO*)buffer;
      int bmp_w = m_img->width, bmp_h = m_img->height;
      FillBitmapInfo( bmi, bmp_w, bmp_h, Bpp(), m_img->origin );
      from_x = MIN( MAX( from_x, 0 ), bmp_w - 1 );
      from_y = MIN( MAX( from_y, 0 ), bmp_h - 1 );
      int sw = MAX( MIN( bmp_w - from_x, w ), 0 );
      int sh = MAX( MIN( bmp_h - from_y, h ), 0 );
      SetDIBitsToDevice(
         dc, x, y, sw, sh, from_x, from_y, from_y, sh,
         m_img->imageData + from_y*m_img->widthStep,
         bmi, DIB_RGB_COLORS );
   }
}
void  CvvImage::DrawToHDC( HDC hDCDst, RECT* pDstRect )
{
   if( pDstRect && m_img && m_img->depth == IPL_DEPTH_8U && m_img->imageData )
   {
      uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
      BITMAPINFO* bmi = (BITMAPINFO*)buffer;
      int bmp_w = m_img->width, bmp_h = m_img->height;
      CvRect roi = cvGetImageROI( m_img );
      CvRect dst = RectToCvRect( *pDstRect );
      if( roi.width == dst.width && roi.height == dst.height )
      {
         Show( hDCDst, dst.x, dst.y, dst.width, dst.height, roi.x, roi.y );
         return;
      }
      if( roi.width > dst.width )
      {
         SetStretchBltMode(
            hDCDst,           // handle to device context
            HALFTONE );
      }
      else
      {
         SetStretchBltMode(
            hDCDst,           // handle to device context
            COLORONCOLOR );
      }
      FillBitmapInfo( bmi, bmp_w, bmp_h, Bpp(), m_img->origin );
      ::StretchDIBits(
         hDCDst,
         dst.x, dst.y, dst.width, dst.height,
         roi.x, roi.y, roi.width, roi.height,
         m_img->imageData, bmi, DIB_RGB_COLORS, SRCCOPY );
   }
}
void  CvvImage::Fill( int color )
{
   cvSet( m_img, cvScalar(color&255,(color>>8)&255,(color>>16)&255,(color>>24)&255) );
}

//CBitmap* CvvImage::IplImage2CBitmap(void)
//{
//}


//CBitmap * CvvImage::IplImage2CBitmap(IplImage * pImage)
//{
//	  if( pImage && pImage->depth == IPL_DEPTH_8U )
//    {
//        HDC hDC=GetDC()->GetSafeHdc();
//       uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
//       BITMAPINFO* bmi = (BITMAPINFO*)buffer;
//       int bmp_w = pImage->width, bmp_h = pImage->height;
//       FillBitmapInfo( bmi, bmp_w, bmp_h, pImage->depth*pImage->nChannels, pImage->origin );
//     
//       char *pBits=NULL;
//       HBITMAP hBitmap=CreateDIBSection(hDC,bmi,DIB_RGB_COLORS,(void**)&pBits,NULL,0);
//       memcpy(pBits,pImage->imageData,pImage->imageSize);
//       CBitmap *pBitmap=new CBitmap;
//       pBitmap->Attach(hBitmap);
//
//       return pBitmap;
//    }
//    else
//       return NULL;
//}


void CvvImage::FillBitmapInfo(BITMAPINFO * bmi, int width, int height, int bpp, int origin)
{
	assert( bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));

    BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

    memset( bmih, 0, sizeof(*bmih));
    bmih->biSize = sizeof(BITMAPINFOHEADER);
    bmih->biWidth = width;
    bmih->biHeight = origin ? abs(height) : -abs(height);
    bmih->biPlanes = 1;
    bmih->biBitCount = (unsigned short)bpp;
    bmih->biCompression = BI_RGB;

    if( bpp == 8 )
    {
       RGBQUAD* palette = bmi->bmiColors;
       int i;
       for( i = 0; i < 256; i++ )
       {
        palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
        palette[i].rgbReserved = 0;
       }
    }
}


IplImage * CvvImage::CBitmap2IplImage(CBitmap * pBitmap)
{
	DIBSECTION ds;
     pBitmap->GetObject(sizeof(ds),&ds);
     IplImage *pImage=cvCreateImage(cvSize(ds.dsBm.bmWidth,ds.dsBm.bmHeight),8,ds.dsBmih.biBitCount/8);
     memcpy(pImage->imageData,ds.dsBm.bmBits,pImage->imageSize);
     return pImage;
}


IplImage* CvvImage::hBitmap2Ipl(HBITMAP hBmp)
{
	BITMAP bmp; 
 ::GetObject(hBmp,sizeof(BITMAP),&bmp);//hBmp-->bmp
 int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel/8 ;
 int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U; 
 IplImage* img = cvCreateImage(cvSize(bmp.bmWidth,bmp.bmHeight),depth,nChannels); //cvCreateImageHeader
 //pBuffer = (char*)malloc(bmp.bmHeight*bmp.bmWidth*nChannels*sizeof(char));
    memcpy(img->imageData,(char*)(bmp.bmBits),bmp.bmHeight*bmp.bmWidth*nChannels);
 IplImage *dst = cvCreateImage(cvGetSize(img),img->depth,3);
 cvCvtColor(img,dst,CV_BGRA2BGR);
 cvReleaseImage(&img);
 return dst;
}


HBITMAP CvvImage::IplImage2hBitmap(IplImage* pImg)
{
	BYTE tmp[sizeof(BITMAPINFO)+1024];
 BITMAPINFO *bmi = (BITMAPINFO*)tmp;
 HBITMAP hBmp;
 int i;
 memset(bmi,0,sizeof(BITMAPINFO));
 bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
 bmi->bmiHeader.biWidth = pImg->width;
 bmi->bmiHeader.biHeight = pImg->height; 
 bmi->bmiHeader.biPlanes = 1;
 bmi->bmiHeader.biBitCount = pImg->nChannels * pImg->depth;
 bmi->bmiHeader.biCompression = BI_RGB;
 bmi->bmiHeader.biSizeImage = 0;//if biCompression is BI_RGB,this can be 0
 bmi->bmiHeader.biClrImportant =0 ;
 
 switch(pImg->nChannels * pImg->depth) 
 { 
 case 8 : 
  for(i=0 ; i < 256 ; i++){ 
   bmi->bmiColors[i].rgbBlue = i;
   bmi->bmiColors[i].rgbGreen= i;
   bmi->bmiColors[i].rgbRed= i;
  }
  break;
 case 32:
 case 24: 
  ((DWORD*) bmi->bmiColors)[0] = 0x00FF0000;
  ((DWORD*) bmi->bmiColors)[1] = 0x0000FF00;
  ((DWORD*) bmi->bmiColors)[2] = 0x000000FF;
  break; 
 } 
 hBmp = ::CreateDIBSection(NULL,bmi,DIB_RGB_COLORS,NULL,0,0);
 
 SetDIBits(NULL,hBmp,0,pImg->height,pImg->imageData,bmi,DIB_RGB_COLORS);
 
 return hBmp;
}


//CBitmap CvvImage::HBITMAP2CBitmap(HBITMAP hbitmap)
//{
//}


//HBITMAP CvvImage::CBitmap2HBITMAP(CBitmap bitmap)
//{
//
//}
