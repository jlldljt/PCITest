// CPicture.cpp: implementation of the CPicture class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "PreviewDialog.h"
#include "Picture.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//----------------CFileProcess class------------------------------------
//constructor
CFileProcess::CFileProcess()
{
	m_File = NULL;         //File *m_File;
}

//destructor !
CFileProcess::~CFileProcess()
{
//	this->Close();
}

//opens a file either for reading or writing
BOOL CFileProcess::Open(char * FileName , FILE_OPENMODE Open_Mode)
{

	switch (Open_Mode)
	{
		case OFM_READ:
			m_File = fopen(FileName, "rb");
			break;
		case OFM_WRITE:
			m_File = fopen(FileName, "wb");
			break;
	}

	//ASSERT(m_File != NULL);
	if(!m_File) 
		return FALSE;
	return TRUE;
}


//Reads data from the file stream.
BOOL CFileProcess::Read(VOID * zBuffer, DWORD cSize)
{
	DWORD dwFreadReslut = fread(zBuffer, sizeof(BYTE), cSize, m_File);
	if(dwFreadReslut < cSize)
		return FALSE;
	return TRUE;
}

//Writes data to the file stream.
BOOL CFileProcess::Write(VOID * zBuffer,DWORD cSize)
{
	//fwrite :write size*count byte to buffer.
	if(fwrite(zBuffer, sizeof(BYTE), cSize, m_File) < cSize)
       return FALSE;
	return TRUE;
}

//Gets the File Size
LONG CFileProcess::GetSize(VOID)
{
	long t_location =  ftell(m_File);
	//moves the file pointer to  a specified location 
	fseek(this->m_File, 0, SEEK_END);

	long f_size = ftell(this->m_File);
	fseek(this->m_File ,t_location, SEEK_SET);
	return f_size;
}

//Closes the File
VOID CFileProcess::Close(VOID)
{
	if(m_File)
	{
		fclose(this->m_File);
	    this->m_File  = NULL;
	}
	return;
}

//-----------------------
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPicture::CPicture()
{
	m_pic = NULL;
	hGlobal = NULL;

}

CPicture::~CPicture()
{
	this->FreePicture();

}



void CPicture::Render(CDC* pDC, LPRECT pDrawRect, LPRECT pSrcRect/*=NULL*/,LPCRECT prcWBounds/*=NULL*/)
{
    if (pDC == NULL || m_pPict == NULL) return ;
    
	CRect recrDest(pDrawRect);
	
	
	
    long Width  = 0;
    long Height = 0;
    m_pPict->get_Width(&Width);
    m_pPict->get_Height(&Height);
	
	
	
	CRect SrcRect(0,0,Width,Height);
	
	
	
	if(pSrcRect){
		SrcRect=*pSrcRect;
	}
	CRect DrawRect(pDrawRect);
    HRESULT hrP = NULL;
	
	
	
    hrP = m_pPict->Render(pDC->m_hDC,
		DrawRect.left,                  // Left
		DrawRect.top,                   // Top
		DrawRect.Width(), // Right
		DrawRect.Height(), // Bottom
		SrcRect.left,
		SrcRect.top,
		SrcRect.Width(),
		SrcRect.Height(),
		prcWBounds);
	
	
	
    if (SUCCEEDED(hrP)) return;
	AfxThrowMemoryException();
    return;
}
/*
OUR MAIN FUNCTION
*/
HBITMAP CPicture::LoadPicture(char *FileName)
{
	FreePicture();

//	CFileProcess cFile;  //the image file

	//if we couldn't open the image file then we should get out of here
//     if(!cFile.Open(FileName, OFM_READ)) 
// 		return NULL;

    HANDLE hFile = NULL; 
    DWORD dwFileSize,dwByteRead;
	IStream* pStream = NULL;
	
	hFile = CreateFile((CString)FileName,GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile != INVALID_HANDLE_VALUE) 
    { 
        dwFileSize = GetFileSize(hFile, NULL);//获取文件字节数 
		
        if (dwFileSize==0xFFFFFFFF)
		{
			CloseHandle(hFile); // 关闭句柄。
			return NULL; 
		}
            
    } 
    else 
    { 
		CloseHandle(hFile); // 关闭句柄。
        return NULL; 
    } 

	//分配全局存储空间 
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
    LPVOID pvData = NULL; 
	
    if (hGlobal == NULL) 
	{
		CloseHandle(hFile); // 关闭句柄。
		return NULL; 
	}
        
	
    if ((pvData = GlobalLock(hGlobal)) == NULL)//锁定分配内存块 
	{
		CloseHandle(hFile); // 关闭句柄。
		return NULL; 
	}
	
    ReadFile(hFile,pvData,dwFileSize,&dwByteRead,NULL);//把文件读入内存缓冲区 
	
    GlobalUnlock(hGlobal); 
	CloseHandle(hFile); // 关闭句柄。
    if(CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) != S_OK)
	{		
		return NULL;
	}

/*
	//we must know exactly the buffer size to allocate
	//in order to read the image in it
	//so get the image file size
    long nSize = cFile.GetSize(); 
	//allocate enough memory to hold the image
	//it must be allocated using GlobalAlloc
	//otherwise it will fail...try using new or malloc and you'll see!!!
	hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);
	
	//get pointer to first byte
	void* pData = GlobalLock(hGlobal);
	
	//read the file in the prev allocated memory
	if(!cFile.Read(pData, nSize)) 
		return NULL;
	//we don't need the file any more so close it
	cFile.Close();
	GlobalUnlock(hGlobal); 

	IStream* pStream = NULL;
    //ole routines....
	//creates a stream from our handle
	//don't know why and how?
	if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) != S_OK)
		return NULL;
*/

	HRESULT hr;
	//aha..now let ole reads the image and load it for us
	//param3:Reference to the identifier of the interface 
               // describing the type of interface pointer to return
	//param4: Address of output variable that receives interface  pointer requested in riid

	hr = OleLoadPicture(pStream, dwFileSize, FALSE, IID_IPicture, (LPVOID*)&this->m_pic);
	if(hr != S_OK)
	{	
		return NULL;
	}
			
	HBITMAP hbmp = NULL;

	//return an HBITMAP to our image like the LoadImage function does
	//we might need the handle

	this->m_pic->get_Handle((OLE_HANDLE *) &hbmp);
	GlobalFree(hGlobal);
	return hbmp;
	
}


//an overloaded version of the above to draw the picture
//as well in a given DC
HBITMAP CPicture::LoadPicture(char *FileName, HDC hdc)
{
	HBITMAP hbmp = this->LoadPicture(FileName);
	this->DrawPicture(hdc, 0, 0, this->_GetWidth(), _GetHeight());
	return hbmp;
}

//an overloaded version of the above to draw the picture
//as well in a given DC , give the rect, width and height
HBITMAP CPicture::LoadPicture(char *FileName, HDC hdc, long cx, long cy)
{
	HBITMAP hbmp = this->LoadPicture(FileName);
	this->DrawPicture(hdc, 0, 0, cx,cy);
	return hbmp;
}

//returns the width of the loaded image  
DWORD CPicture::_GetWidth(VOID)
{
    
	if(! m_pic) 
		return 0;
	OLE_XSIZE_HIMETRIC pWidth;
	m_pic->get_Width(&pWidth);
	HDC tDC = ::CreateCompatibleDC(0);
	int nWidth	= MulDiv(pWidth,::GetDeviceCaps(tDC, LOGPIXELSX), HIMETRIC_INCH);
	DeleteDC(tDC);
	return (DWORD) nWidth;
}


//returns the height of the loaded image
DWORD CPicture::_GetHeight(VOID)
{
	if(! m_pic) 
		return 0;
	OLE_YSIZE_HIMETRIC pHeight;

	m_pic->get_Height(&pHeight);
	HDC tDC = ::CreateCompatibleDC(0);

	int nHeight	= MulDiv(pHeight,::GetDeviceCaps(tDC, LOGPIXELSY), HIMETRIC_INCH);
	DeleteDC(tDC);
	
	return (DWORD) nHeight;
}

//returns the current DC of the loaded image
HDC CPicture::_GetDC(VOID)
{
	if(! m_pic) 
		return NULL;
	HDC pDC ;
	m_pic->get_CurDC(&pDC);
	return  pDC;
}

//returns the Handle of the loaded image(HBITMAP)
HBITMAP CPicture::_GetHandle( VOID )
{
	if(! m_pic) 
		return NULL; 

	HBITMAP hbmp;
	m_pic->get_Handle((OLE_HANDLE *) &hbmp);
	return  hbmp;
}


//Draws the image in a specified DC..with given dimensions
//specify -1 for width and height if you like to draw
//with original dimensions
BOOL CPicture::DrawPicture(HDC hdc,long x,long y,long cx,long cy)
{
	if(!m_pic) 
	{
		return FALSE;
	}	

	LONG pHeight = 0;
	LONG pWidth = 0;
	if(cx == -1) 
		cx = this->_GetWidth();
	if(cy == -1) 
		cy = this->_GetHeight();
	m_pic->get_Width(&pWidth);
    m_pic->get_Height(&pHeight);
	if (m_pic->Render(hdc, x, y, cx, cy, 0, pHeight, pWidth, -pHeight, NULL) != S_OK)
	{
		return FALSE ;
	}
	return TRUE;
}

//Cleans up
VOID CPicture::FreePicture(VOID)
{
	if(m_pic)
	{
		m_pic->Release();
		m_pic = NULL;
	}

	if(hGlobal)
	{
		GlobalFree(hGlobal);
		hGlobal = NULL;
	}
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
BOOL CPicture::SaveNewImage(HBITMAP hbitmap, LPCTSTR filename, int nColor)//ncolor设成8会出错，可能是8位需要颜色表，但下面没创建合理的颜色表导致图像一片灰
{
	BITMAP Bitmap;
	HDC         hDC;                         
	DWORD     dwPaletteSize=0,dwBmBitsSize,dwDIBSize,   dwWritten;     
	BITMAPFILEHEADER       bmfHdr;                   
	BITMAPINFOHEADER       bi;                           
	LPBITMAPINFOHEADER   lpbi;           
	HANDLE   fh,   hDib,   hPal,hOldPal=NULL;  
	if   (nColor<=   8)   
		dwPaletteSize   =   (1<<nColor)   *sizeof(RGBQUAD);   
	GetObject(hbitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = nColor;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0; 
	dwBmBitsSize   =   ((Bitmap.bmWidth  *   nColor+31)/32*4)*Bitmap.bmHeight;  
	//为位图分配内存
	hDib     =   GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));   
	lpbi   =   (LPBITMAPINFOHEADER)GlobalLock(hDib);   
	*lpbi   =   bi; 
	//设置调色板
	hPal   =   GetStockObject(DEFAULT_PALETTE);       
	if   (hPal)   
	{   
		hDC     =   ::GetDC(NULL);   
		hOldPal   =   SelectPalette(hDC,   (HPALETTE)hPal,   FALSE);   
		RealizePalette(hDC);   
	}  
	// 获取该调色板下新的像素值
	GetDIBits(hDC,   hbitmap,   0,   Bitmap.bmHeight, 
		(LPSTR)lpbi   +   sizeof(BITMAPINFOHEADER)+dwPaletteSize,   
		(BITMAPINFO   *)lpbi,   DIB_RGB_COLORS);   
	if   (hOldPal)   
	{   
		SelectPalette(hDC,   (HPALETTE)hOldPal,   TRUE);   
		RealizePalette(hDC);   
		::ReleaseDC(NULL,hDC);   
	}  
	fh   =   CreateFile(filename,   GENERIC_WRITE,     
		0,//not   be   shared   
		NULL,   //cannot   be   inherited   
		CREATE_ALWAYS,   
		FILE_ATTRIBUTE_NORMAL   |   FILE_FLAG_SEQUENTIAL_SCAN,     
		NULL);  
	if   (fh   ==   INVALID_HANDLE_VALUE)   
		return   FALSE;   
	//设置位图文件头
	bmfHdr.bfType   =   0x4D42;     //   "BM"   
	dwDIBSize           =   sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize;       
	bmfHdr.bfSize   =   dwDIBSize;   
	bmfHdr.bfReserved1   =   0;   
	bmfHdr.bfReserved2   =   0;   
	bmfHdr.bfOffBits   =   (DWORD)sizeof(BITMAPFILEHEADER)     
		+   (DWORD)sizeof(BITMAPINFOHEADER)+   dwPaletteSize;  
	//write   file   header   
	WriteFile(fh,   (LPSTR)&bmfHdr,   sizeof(BITMAPFILEHEADER),   &dwWritten,   NULL);  
	//write   bmp   data   
	WriteFile(fh,   (LPSTR)lpbi,   dwDIBSize,   &dwWritten,   NULL);  
	GlobalUnlock(hDib);   
	GlobalFree(hDib);   
	CloseHandle(fh);   
	DeleteObject(hbitmap);    
	return   TRUE;  
}
//////////////////////////////////////////////////////////////////////////
BOOL CPicture::SaveBmp(HBITMAP hBitmap, const char *FileName)   
{   
	HDC hDC;   
	//当前分辨率下每象素所占字节数   
	int iBits;   
	//位图中每象素所占字节数   
	WORD wBitCount;   
	//定义调色板大小， 位图中像素字节大小 ，位图文件大小 ， 写入文件字节数   
	DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0;   
	//位图属性结构   
	BITMAP Bitmap;   
	//位图文件头结构   
	BITMAPFILEHEADER bmfHdr;   
	//位图信息头结构   
	BITMAPINFOHEADER bi;   
	//指向位图信息头结构   
	LPBITMAPINFOHEADER lpbi;   
	//定义文件，分配内存句柄，调色板句柄   
	HANDLE fh, hDib, hPal,hOldPal=NULL;  
	//计算位图文件每个像素所占字节数   
	hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);   
	iBits = GetDeviceCaps(hDC, BITSPIXEL) *GetDeviceCaps(hDC, PLANES);   
	DeleteDC(hDC);   
	if (iBits <= 1) 
	{
		wBitCount = 1; 
	}
	else if (iBits <= 4)   
	{
		wBitCount = 4;     
	}
	else if (iBits <= 8)  
	{
		wBitCount = 8;  
	}
	else 
	{
		wBitCount = 24;
	}
	wBitCount = 8;  
	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);   
	bi.biSize = sizeof(BITMAPINFOHEADER);   
	bi.biWidth =  Bitmap.bmWidth;   
	bi.biHeight = Bitmap.bmHeight;   
	bi.biPlanes = 1;   
	bi.biBitCount = wBitCount;   
	bi.biCompression =  BI_RGB;   
	bi.biSizeImage  =  0;   
	bi.biXPelsPerMeter = 0;   
	bi.biYPelsPerMeter   =   0;   
	bi.biClrImportant = 0;   
	bi.biClrUsed   = 0;  
	dwBmBitsSize  = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;  
	//为位图内容分配内存   
	hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));   
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);   
	*lpbi = bi;  
	// 处理调色板   
	hPal = GetStockObject(DEFAULT_PALETTE);   
	if (hPal)   
	{   
		hDC = ::GetDC(NULL);   
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);   
		RealizePalette(hDC);   
	}  
	// 获取该调色板下新的像素值   
	GetDIBits(hDC, 
		hBitmap, 
		0,   
		(UINT)Bitmap.bmHeight,
		(LPSTR)lpbi+sizeof(BITMAPINFOHEADER)+dwPaletteSize,
		(BITMAPINFO *)lpbi, 
		DIB_RGB_COLORS);  
	//恢复调色板   
	if (hOldPal)   
	{   
		::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);   
		RealizePalette(hDC);   
		::ReleaseDC(NULL, hDC);   
	}  
	//创建位图文件   
	fh  =  CreateFile((LPCWSTR)FileName,
		GENERIC_WRITE,
		0, 
		NULL,    
		CREATE_ALWAYS,   
		FILE_ATTRIBUTE_NORMAL,
		NULL);/* FILE_FLAG_SEQUENTIAL_SCAN, NULL);  */    
	if (fh == INVALID_HANDLE_VALUE) 
	{
		return FALSE;  
	}
	// 设置位图文件头   
	bmfHdr.bfType = 0x4D42; // "BM"   
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;   
	bmfHdr.bfSize = dwDIBSize;   
	bmfHdr.bfReserved1 = 0;   
	bmfHdr.bfReserved2 = 0;   
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;   
	// 写入位图文件头   
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);   
	// 写入位图文件其余内容   
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	//清除   
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	return TRUE;
}    

// http://blog.sina.com.cn/s/blog_6ee382440100moeu.html
// http://apps.hi.baidu.com/share/detail/32878068
// bitmap 位图CBitmap对象指针
// lpFileName 为位图文件名  
bool CPicture::SaveBitmapToFile(CBitmap* bitmap, LPCWSTR lpFileName)
{            
	HBITMAP hBitmap;	// 为刚才的屏幕位图句柄
	HDC hDC; //设备描述表  
	int iBits; //当前显示分辨率下每个像素所占字节数  
	WORD wBitCount; //位图中每个像素所占字节数	
	DWORD dwPaletteSize = 0, //定义调色板大小
		dwBmBitsSize,  //位图中像素字节大小   
		dwDIBSize,	 //位图文件大小
		dwWritten;  //写入文件字节数
	BITMAP Bitmap; //位图属性结构  
	BITMAPFILEHEADER bmfHdr; //位图文件头结构  
	BITMAPINFOHEADER bi; //位图信息头结构    
	LPBITMAPINFOHEADER lpbi; //指向位图信息头结构  
	HANDLE fh,	 //定义文件
		hDib,	 //分配内存句柄
		hPal,	//调色板句柄
		hOldPal = NULL;
	
	//计算位图文件每个像素所占字节数  
	hBitmap = (HBITMAP)*bitmap;  
	hDC = CreateDC(_T("DISPLAY"),NULL,NULL,NULL);  
	iBits =	GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);  
	DeleteDC(hDC);
	
	if (iBits <= 1)  
		wBitCount =	1;	
	else if	(iBits <= 4)  
		wBitCount =	4;	
	else if (iBits <= 8)  
		wBitCount =	8;	
	else if	(iBits <= 24)  
		wBitCount =	24;  
	else if (iBits <= 32)
		wBitCount = 32;
	wBitCount =	24; //mmy
	//计算调色板大小  
	if (wBitCount <= 8)  
		dwPaletteSize =	(1 << wBitCount) * sizeof (RGBQUAD);
	
	//设置位图信息头结构  
	GetObject(hBitmap, sizeof (BITMAP), (LPSTR)&Bitmap);
	bi.biSize = sizeof (BITMAPINFOHEADER);  
	bi.biWidth = Bitmap.bmWidth;  
	bi.biHeight = Bitmap.bmHeight;  
	bi.biPlanes = 1;	
	bi.biBitCount =	wBitCount;	
	bi.biCompression = BI_RGB;  
	bi.biSizeImage = 0;  
	bi.biXPelsPerMeter = 0;  
	bi.biYPelsPerMeter = 0;  
	bi.biClrUsed = 0;  
	bi.biClrImportant = 0;  
	
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount+31) / 32) * 4 * Bitmap.bmHeight;  
	
	//为位图内容分配内存  
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof (BITMAPINFOHEADER));  
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);  
	*lpbi =	bi;
	
	// 处理调色板 	   
	hPal = GetStockObject(DEFAULT_PALETTE);  
	if (hPal)  
	{  
		hDC = ::GetDC(NULL);	
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);	
		RealizePalette(hDC);
	}  
	
	//	 获取该调色板下新的像素值  
	GetDIBits(hDC, hBitmap,	0, (UINT) Bitmap.bmHeight,	
		(LPSTR)lpbi + sizeof (BITMAPINFOHEADER)	+ dwPaletteSize,
		(LPBITMAPINFO)lpbi,	DIB_RGB_COLORS);  
	
	//恢复调色板		
	if (hOldPal)	
	{  
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);  
		RealizePalette(hDC);  
		::ReleaseDC(NULL, hDC);
	}  
	
	//创建位图文件			
	fh = CreateFile(lpFileName, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL |	FILE_FLAG_SEQUENTIAL_SCAN, NULL);  
	
	if (fh == INVALID_HANDLE_VALUE)
		return FALSE;  
	
	//	 设置位图文件头  
	bmfHdr.bfType =	0x4D42; 	//	 "BM"  
	dwDIBSize = sizeof (BITMAPFILEHEADER)	  
		+ sizeof (BITMAPINFOHEADER)  
		+ dwPaletteSize	+ dwBmBitsSize;	   
	bmfHdr.bfSize =	dwDIBSize;	
	bmfHdr.bfReserved1 = 0;  
	bmfHdr.bfReserved2 = 0;  
	bmfHdr.bfOffBits = (DWORD)sizeof (BITMAPFILEHEADER)	  
		+ (DWORD)sizeof (BITMAPINFOHEADER)  
		+ dwPaletteSize;	
	
	//	 写入位图文件头  
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten,	NULL);	
	
	//	 写入位图文件其余内容  
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize,
		&dwWritten, NULL);
	
	//清除		  
	GlobalUnlock(hDib);  
	GlobalFree(hDib);  
	CloseHandle(fh);  

	return TRUE;	
}

