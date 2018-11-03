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
        dwFileSize = GetFileSize(hFile, NULL);//��ȡ�ļ��ֽ��� 
		
        if (dwFileSize==0xFFFFFFFF)
		{
			CloseHandle(hFile); // �رվ����
			return NULL; 
		}
            
    } 
    else 
    { 
		CloseHandle(hFile); // �رվ����
        return NULL; 
    } 

	//����ȫ�ִ洢�ռ� 
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
    LPVOID pvData = NULL; 
	
    if (hGlobal == NULL) 
	{
		CloseHandle(hFile); // �رվ����
		return NULL; 
	}
        
	
    if ((pvData = GlobalLock(hGlobal)) == NULL)//���������ڴ�� 
	{
		CloseHandle(hFile); // �رվ����
		return NULL; 
	}
	
    ReadFile(hFile,pvData,dwFileSize,&dwByteRead,NULL);//���ļ������ڴ滺���� 
	
    GlobalUnlock(hGlobal); 
	CloseHandle(hFile); // �رվ����
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
BOOL CPicture::SaveNewImage(HBITMAP hbitmap, LPCTSTR filename, int nColor)//ncolor���8�����������8λ��Ҫ��ɫ��������û�����������ɫ����ͼ��һƬ��
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
	//Ϊλͼ�����ڴ�
	hDib     =   GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));   
	lpbi   =   (LPBITMAPINFOHEADER)GlobalLock(hDib);   
	*lpbi   =   bi; 
	//���õ�ɫ��
	hPal   =   GetStockObject(DEFAULT_PALETTE);       
	if   (hPal)   
	{   
		hDC     =   ::GetDC(NULL);   
		hOldPal   =   SelectPalette(hDC,   (HPALETTE)hPal,   FALSE);   
		RealizePalette(hDC);   
	}  
	// ��ȡ�õ�ɫ�����µ�����ֵ
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
	//����λͼ�ļ�ͷ
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
	//��ǰ�ֱ�����ÿ������ռ�ֽ���   
	int iBits;   
	//λͼ��ÿ������ռ�ֽ���   
	WORD wBitCount;   
	//�����ɫ���С�� λͼ�������ֽڴ�С ��λͼ�ļ���С �� д���ļ��ֽ���   
	DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0;   
	//λͼ���Խṹ   
	BITMAP Bitmap;   
	//λͼ�ļ�ͷ�ṹ   
	BITMAPFILEHEADER bmfHdr;   
	//λͼ��Ϣͷ�ṹ   
	BITMAPINFOHEADER bi;   
	//ָ��λͼ��Ϣͷ�ṹ   
	LPBITMAPINFOHEADER lpbi;   
	//�����ļ��������ڴ�������ɫ����   
	HANDLE fh, hDib, hPal,hOldPal=NULL;  
	//����λͼ�ļ�ÿ��������ռ�ֽ���   
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
	//Ϊλͼ���ݷ����ڴ�   
	hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));   
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);   
	*lpbi = bi;  
	// �����ɫ��   
	hPal = GetStockObject(DEFAULT_PALETTE);   
	if (hPal)   
	{   
		hDC = ::GetDC(NULL);   
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);   
		RealizePalette(hDC);   
	}  
	// ��ȡ�õ�ɫ�����µ�����ֵ   
	GetDIBits(hDC, 
		hBitmap, 
		0,   
		(UINT)Bitmap.bmHeight,
		(LPSTR)lpbi+sizeof(BITMAPINFOHEADER)+dwPaletteSize,
		(BITMAPINFO *)lpbi, 
		DIB_RGB_COLORS);  
	//�ָ���ɫ��   
	if (hOldPal)   
	{   
		::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);   
		RealizePalette(hDC);   
		::ReleaseDC(NULL, hDC);   
	}  
	//����λͼ�ļ�   
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
	// ����λͼ�ļ�ͷ   
	bmfHdr.bfType = 0x4D42; // "BM"   
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;   
	bmfHdr.bfSize = dwDIBSize;   
	bmfHdr.bfReserved1 = 0;   
	bmfHdr.bfReserved2 = 0;   
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;   
	// д��λͼ�ļ�ͷ   
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);   
	// д��λͼ�ļ���������   
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	//���   
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	return TRUE;
}    

// http://blog.sina.com.cn/s/blog_6ee382440100moeu.html
// http://apps.hi.baidu.com/share/detail/32878068
// bitmap λͼCBitmap����ָ��
// lpFileName Ϊλͼ�ļ���  
bool CPicture::SaveBitmapToFile(CBitmap* bitmap, LPCWSTR lpFileName)
{            
	HBITMAP hBitmap;	// Ϊ�ղŵ���Ļλͼ���
	HDC hDC; //�豸������  
	int iBits; //��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���  
	WORD wBitCount; //λͼ��ÿ��������ռ�ֽ���	
	DWORD dwPaletteSize = 0, //�����ɫ���С
		dwBmBitsSize,  //λͼ�������ֽڴ�С   
		dwDIBSize,	 //λͼ�ļ���С
		dwWritten;  //д���ļ��ֽ���
	BITMAP Bitmap; //λͼ���Խṹ  
	BITMAPFILEHEADER bmfHdr; //λͼ�ļ�ͷ�ṹ  
	BITMAPINFOHEADER bi; //λͼ��Ϣͷ�ṹ    
	LPBITMAPINFOHEADER lpbi; //ָ��λͼ��Ϣͷ�ṹ  
	HANDLE fh,	 //�����ļ�
		hDib,	 //�����ڴ���
		hPal,	//��ɫ����
		hOldPal = NULL;
	
	//����λͼ�ļ�ÿ��������ռ�ֽ���  
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
	//�����ɫ���С  
	if (wBitCount <= 8)  
		dwPaletteSize =	(1 << wBitCount) * sizeof (RGBQUAD);
	
	//����λͼ��Ϣͷ�ṹ  
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
	
	//Ϊλͼ���ݷ����ڴ�  
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof (BITMAPINFOHEADER));  
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);  
	*lpbi =	bi;
	
	// �����ɫ�� 	   
	hPal = GetStockObject(DEFAULT_PALETTE);  
	if (hPal)  
	{  
		hDC = ::GetDC(NULL);	
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);	
		RealizePalette(hDC);
	}  
	
	//	 ��ȡ�õ�ɫ�����µ�����ֵ  
	GetDIBits(hDC, hBitmap,	0, (UINT) Bitmap.bmHeight,	
		(LPSTR)lpbi + sizeof (BITMAPINFOHEADER)	+ dwPaletteSize,
		(LPBITMAPINFO)lpbi,	DIB_RGB_COLORS);  
	
	//�ָ���ɫ��		
	if (hOldPal)	
	{  
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);  
		RealizePalette(hDC);  
		::ReleaseDC(NULL, hDC);
	}  
	
	//����λͼ�ļ�			
	fh = CreateFile(lpFileName, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL |	FILE_FLAG_SEQUENTIAL_SCAN, NULL);  
	
	if (fh == INVALID_HANDLE_VALUE)
		return FALSE;  
	
	//	 ����λͼ�ļ�ͷ  
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
	
	//	 д��λͼ�ļ�ͷ  
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten,	NULL);	
	
	//	 д��λͼ�ļ���������  
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize,
		&dwWritten, NULL);
	
	//���		  
	GlobalUnlock(hDib);  
	GlobalFree(hDib);  
	CloseHandle(fh);  

	return TRUE;	
}

