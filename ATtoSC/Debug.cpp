// Debug.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "ATtoSC.h"
#include "Debug.h"
#include "afxdialogex.h"
#include "ATtoSCDlg.h"

extern sortchip g_sort;
extern bool debugupdate;
extern threadstatus g_status;
extern HANDLE g_hDevice;
extern int degree[30];
extern int *g_num;
extern int g_height;    
extern int g_width;
extern int g_widthBytes;
extern BYTE *g_lpGray;
extern void MotorCtrl(int n,bool zf,int steps);//����1��2�ŵ��
extern void CylinderCtrlDbg(bool stat);
extern void TubeCtrl(int n,bool stat);//����1��2�Ż�е�۵����쿪��
extern void SortBlock(int n);//�ֵ�
extern void VibrateBlock();//������
extern void ReturnZero(int n);//����1��2�Ż�е�ۻ���λ
extern void BlowBlock();//����
extern void StartFTLong(bool se);
extern int head[10][8],height_num,width_num;//ͼƬ��־����
//px1000 ��2 46  c199�� 1 41
#define HEAD_H_OFFSET 2
#define HEAD_W_OFFSET 46
extern RGBCAPTUREBUFFER pCaptureBuffer;
extern RGBBITMAP       RGBBitmap1;     //��һ���ɼ��¼�λͼ��Ϣ
extern TPicRegion pDest;
extern double m_VGAthreshold;
int listsortsel;//��ǵ��е�list�еı��
bool DebugShow;//ʵʱ��ʾ��־
bool testStart;//efg�ź��շ���־
bool dynamicDetect;//��̬����̱߳�־
unsigned int dynamicNum;//��̬������
CDebug g_csDebug;
CDebug *gp_csDebug = NULL;
//CString pathSaveR1=(_T(".\\check\\R1Number.txt"));_0610
UINT DebugDisplay(LPVOID pParam)//��ʾ
{
	while(1)
	{
		if(!debugupdate)
			return 0;
		::PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_DISPLAY,0,NULL);//������ʾ��Ϣ��������
		Sleep(100);
	}
	return 0;
}
UINT TestStart(LPVOID pParam)//efg�ź��շ�����
{
	while(testStart)
	{

		StartFT();//����ʼ����ź�
		while(1)//����
		{
			if(AC6641_DI(g_hDevice,6) & 0x02)//���ߵ�ƽ
				if(AC6641_DI(g_hDevice,6) & 0x02)
				{
					while(1)
					{
						if(!(AC6641_DI(g_hDevice,6) & 0x02))//����ʱ
							if(!(AC6641_DI(g_hDevice,6) & 0x02))
								break;
						Sleep(1);
					}
					break;
				}
				Sleep(1);
		}
	}
	return 0;
}
UINT DynamicDetect(LPVOID pParam)//��̬���
{
	CStdioFile file;
	file.Open(pathSaveR1,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
	CString _degree,_tmp;
	int nNum = 0;
	ReturnZero(1);//����
	ReturnZero(2);//����
	Sleep(500);
	MotorCtrl(2,1,g_status.Ywait);//�ƶ����ȴ�λ��
	MotorCtrl(1,1,g_status.Xwait);//�ƶ����ȴ���Ƭλ��
	StartFT();//����ʼ����ź�
	Sleep(4000);
	while(dynamicDetect && nNum++<dynamicNum)//-->0)
	{
		/*ReturnZero(2);//����
		Sleep(500);
		TubeCtrl(2,1);//����
		Sleep(500);
		TubeCtrl(2,0);//����
		Sleep(500);
		MotorCtrl(2,1,g_status.Ywait);//�ƶ����ȴ�λ��

		StartFT();//����ʼ����ź�*/

		MotorCtrl(1,1,g_status.Xcheck-g_status.Xwait);//�ƶ�����Ƭλ��
		Sleep(500);
		TubeCtrl(1,1);//����
		Sleep(g_tim.onX);
		TubeCtrl(1,0);//����

		Sleep(g_tim.offX);

		ReturnZero(1);//����

		Sleep(300);
		StartFT();//����ʼ����ź�
		MotorCtrl(1,1,g_status.Xwait);//�ƶ����ȴ���Ƭλ��

		while(dynamicDetect && 1)//����
		{
			if(AC6641_DI(g_hDevice,6) & 0x02)//���ߵ�ƽ
			{
				Sleep(50);
				if(AC6641_DI(g_hDevice,6) & 0x02)
				{
					Sleep(50);
					while(dynamicDetect &&1)
					{
						if(!(AC6641_DI(g_hDevice,6) & 0x02))//����ʱ
						{
							Sleep(25);
							if(!(AC6641_DI(g_hDevice,6) & 0x02))
								break;
						}
						Sleep(1);
					}
					break;
				}

			}
			Sleep(1);
		}
		///
		DWORD width = RGBBitmap1.bmiHeader.biWidth;
		DWORD height = -RGBBitmap1.bmiHeader.biHeight;

		pDest.pdata = new TARGB32[800*600];//b,g,r,a  4��8λ���һ������
		pDest.height = height;
		pDest.width  = width;
		pDest.byte_width = width << 2;

		g_height=height;
		g_width=width;
		g_lpGray=new BYTE[g_height*g_width];

		int iW=width, iH=height;
		DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
		//SaveImageToFile(_T("screen_photo\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//����ɼ�ͼ��

		DECODE_RGB_TO_GRAY((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);//ת�Ҷ�:����lpVoidBuffer�����pDest

		//SaveImageToFile(_T("screen_photo\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//����Ҷ�ͼ��
		DECODE_THRESHOLD(pDest,pDest,g_lpGray,m_VGAthreshold);//ת��ֵ����pDest������0-255�Ķ�ֵ��ͼƬ��lpGray�����0-1�Ķ�ֵ������

		//SaveImageToFile(_T("screen_photo\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//�����ֵ��ͼ��
		delete [] pDest.pdata;
		pDest.pdata=NULL;
		LoadMark(g_path + _T("\\check\\qz_42.txt"));

		FindOrigin(g_lpGray);
		if(height_num>100 || width_num>200|| (height_num==0&&width_num==0))
		{ 
			//AfxMessageBox(_T("û���ҵ�ͼ����,�������趨��ֵ"));
			dynamicNum++;
			delete []g_lpGray;
			g_lpGray=NULL;
			continue;
		}
		g_num=new int[10*10*8];
		GetNumber();
		ScanDegree(g_lpGray,g_path + _T("\\check\\position.txt"));//���degree[]
		_degree.Format(_T("%d\t%d\t%d\t%d\t%d\t%d"),nNum,degree[0]*100000+degree[1]*10000+degree[2]*1000+degree[3]*100+degree[4]*10+degree[5],
			degree[6]*100000+degree[7]*10000+degree[8]*1000+degree[9]*100+degree[10]*10+degree[11],
			degree[12]*100000+degree[13]*10000+degree[14]*1000+degree[15]*100+degree[16]*10+degree[17],
			degree[18]*100000+degree[19]*10000+degree[20]*1000+degree[21]*100+degree[22]*10+degree[23],
			degree[24]*100000+degree[25]*10000+degree[26]*1000+degree[27]*100+degree[28]*10+degree[29]);

		ScanDegreeDynamic(g_lpGray,g_path + _T("\\check\\positionD1.txt"));//���degree[]
		_tmp.Format(_T("\tD1:%.3f"),degree[0]*100+degree[1]*10+degree[2]+degree[3]*0.1+degree[4]*0.01+degree[5]*0.001);
		_degree +=_tmp;
		ScanDegreeDynamic(g_lpGray,g_path + _T("\\check\\positionD2.txt"));//���degree[]
		_tmp.Format(_T("\tD2:%.3f"),degree[0]*100+degree[1]*10+degree[2]+degree[3]*0.1+degree[4]*0.01+degree[5]*0.001);
		_degree +=_tmp;
		ScanDegreeDynamic(g_lpGray,g_path + _T("\\check\\positionDM.txt"));//���degree[]
		_tmp.Format(_T("\tDM:%.3f"),degree[0]*100+degree[1]*10+degree[2]+degree[3]*0.1+degree[4]*0.01+degree[5]*0.001);
		_degree +=_tmp;
		ScanDegreeDynamic(g_lpGray,g_path + _T("\\check\\positionR1.txt"));//���degree[]
		_tmp.Format(_T("\tR1:%.3f"),degree[0]*100+degree[1]*10+degree[2]+degree[3]*0.1+degree[4]*0.01+degree[5]*0.001);
		_degree +=_tmp;
		ScanDegreeDynamic(g_lpGray,g_path + _T("\\check\\positionAL.txt"));//���degree[]
		_tmp.Format(_T("\tAL:%.3f\r\n"),degree[0]*100+degree[1]*10+degree[2]+degree[3]*0.1+degree[4]*0.01+degree[5]*0.001);
		_degree +=_tmp;

		delete []g_num;
		delete []g_lpGray;
		g_lpGray=NULL;
		g_num=NULL;
		///
		//����Ƕȵ��ļ�
		char szANSIString[MAX_PATH];  
		WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,_degree,-1,szANSIString,sizeof(szANSIString),NULL,NULL); 
		file.Seek(0,CFile::end);
		//myFile.Write(szANSIString,_sort.GetLength()*2-98);
		file.Write(szANSIString,CStringA(_degree).GetLength());

		//	file.Seek(0,CFile::end);
		//file.WriteString( _degree );
		_tmp.Format(_T("%d"),nNum);
		gp_csDebug->SetDlgItemText(IDC_STATIC_DYM,_tmp);
	}
	dynamicDetect=0;
	file.Close();
	AfxMessageBox(_T("��̬������"));
	gp_csDebug->SetDlgItemText(IDC_DYNAMIC_DETECT,_T("��ʼ���"));
	return 0;
}
void LoadMark(CString file_temp)
{
	CStdioFile File2;// ����һ��CStdioFile�����File
	CString FileData2;//,file_temp;// ����һ��CString����Ϊһ��������
	//file_temp="check\\qz_42.txt";   //�����ļ���������ļ���
	CString fileName(file_temp);//CString a (_T ("aaa"));
	File2.Open(fileName,CFile::modeRead);
	for(int j=0;j<10;j++)
	{
		File2.ReadString(FileData2);
		FileData2.MakeReverse();
		int tmp=_wtoi(FileData2);
		for(int k=0;k<8;k++)
		{
			head[j][k]=tmp%10;
			tmp/=10;
		}
	}
	File2.Close();
}
void FindOrigin(BYTE* lpGray)
{
	bool first=false;
	char tmep_num=0;
	height_num=0;
	width_num=0;
	CString strTmp;
	for(int i=0;i<g_height-11;i++)
	{	
		for(int j=0;j<g_width-9;j++)
		{
			for(int x=0;x<10;x++)
				for(int y=0;y<8;y++)
				{
					if(lpGray[(i+x)*g_width+(j+y)]==head[x][y]) tmep_num++; 
				}
				if(tmep_num>75)//�ж�ͼƬ���
				{
					first=true;
					height_num=i;
					width_num=j;
					strTmp.Format(_T("����У���ֵ��%d��%d ",i,j));
					//AfxMessageBox(strTmp);
					break;
				}
				else tmep_num=0;

		}
		if(first) break;
	}

}

void FindOrigin2(BYTE* lpGray)
{
#define white 1//��ʾǰ��
#define black 0//��ʾ����
  bool first = false;
 // char tmep_num = 0;
  height_num = 0;
  width_num = 0;

  CString strTmp;
  for (int i = 0; i<g_height - 11; i++)
  {
    for (int j = 0; j<g_width - 9; j++)
    {
      int different_rows = 0;
      for (int x = 0; x<10; x++)//10��
      {
        
        int dest_templet_have_white_color = 0;//ͬ����ɫ���֣�ȥ���˴����䣬������ĵ�һ��������
        int dest_color_updown = 0;//dest�����¼
        int templet_color_updown = 0;//templet�����¼
        int dest_temp_color = black;//��¼ǰһ��
        int templet_temp_color = black;//��¼ǰһ��

        for (int y = 0; y<8; y++)//8��

        {
          int templet = head[x][y];
          int dest = lpGray[(i + x)*g_width + (j + y)];
          if (white == dest && 7 != y)
          {
            dest_temp_color = white;
          }
          else if (dest_temp_color == white)//һ�β���
          {
            dest_color_updown++;
            dest_temp_color = black;
          }

          if (white == templet && 7 != y)
          {
            templet_temp_color = white;
          }
          else if (templet_temp_color == white)//һ�β���
          {
            templet_color_updown++;
            templet_temp_color = black;
          }

          if (white == dest && white == templet)
          {
            dest_templet_have_white_color = 1;
          }

          if (7 == y)
          {
            if (dest_temp_color == white)//һ�β���
            {
              dest_color_updown++;
              dest_temp_color = black;
            }
            if (templet_temp_color == white)//һ�β���
            {
              templet_color_updown++;
              templet_temp_color = black;
            }
          }


          if (1 == dest_color_updown && 1 == templet_color_updown)//����һ�����䶼����
          {
            if (!dest_templet_have_white_color)//�����һ����û���غ�����
            {
              different_rows++;
              break;
            }
            else
            {
              dest_color_updown = 0;
              templet_color_updown = 0;
              dest_templet_have_white_color = 0;
            }
          }
          else if (1 < dest_color_updown || 1 < templet_color_updown)//�����쳣
          {
            different_rows++;
            break;
          }
          else if (7 == y && dest_color_updown != templet_color_updown)
          {
            different_rows++;
            break;
          }

        }//for (int l = 0; l < 8; l++)//8��
      }//for (int k = 0; k < 10; k++)//10��

      if (different_rows == 0)//�ж�ͼƬ���
      {
        first = true;
        height_num = i- HEAD_H_OFFSET;
        width_num = j- HEAD_W_OFFSET;
        strTmp.Format(_T("����У���ֵ��%d��%d ", i, j));
        //AfxMessageBox(strTmp);
        break;
      }
     // else tmep_num = 0;

    }
    if (first) break;
  }
#undef white;
#undef black;
}

void FindOriginEX(BYTE* lpGray)
{
	bool first=false;
	char w_num=0,b_num=0;
	height_num=0;
	width_num=0;
	CString strTmp;
	for(int i=0;i<g_height-11;i++)
	{	
		for(int j=0;j<g_width-9;j++)
		{
			for(int x=0;x<10;x++)
				for(int y=0;y<8;y++)
				{
					if(0==lpGray[(i+x)*g_width+(j+y)]&&1==head[x][y]) w_num++; 
					if(1==lpGray[(i+x)*g_width+(j+y)]&&0==head[x][y]) 
						if(y<7)
							if(1==head[x][y+1])
								;
							else
								b_num++; 
						else
							b_num++;
				}
				if(0==w_num&&b_num<3)//�ж�ͼƬ���
				{
					first=true;
					height_num=i;
					width_num=j;
					strTmp.Format(_T("����У���ֵ��%d��%d ",i,j));
					//AfxMessageBox(strTmp);
					break;
				}
				else 
				{
					w_num=0;
					b_num=0;
				}

		}
		if(first) break;
	}

}
//��ͼƬ��ɨ��Ƕ�
void GetNumber()
{

	CStdioFile File2;// ����һ��CStdioFile�����File
	CString FileData2,file_temp;// ����һ��CString����Ϊһ��������
	for(int p=0;p<10;p++)
	{
		file_temp.Format(_T("\\check\\num_%d.txt"),p); 
		file_temp =g_path + file_temp;
		File2.Open(file_temp,CFile::modeRead);
		for(int j=0;j<10;j++)
		{
			File2.ReadString(FileData2);
			FileData2.MakeReverse();
			int tmp=_wtoi(FileData2);
			for(int k=0;k<8;k++)
			{
				g_num[p*80+j*8+k]=tmp%10;
				tmp/=10;
			}
		}
		File2.Close();
	}
}

void ScanDegree(BYTE * lpGray,CString file_temp)
{
	CStdioFile File2;// ����һ��CStdioFile�����File
	CString FileData2;//,file_temp;// ����һ��CString����Ϊһ��������
	CString fileName_position(file_temp);//
	File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;

	int t=0;
	int nocheckout=0;
	char tmep_num=0;
	for(int m=0;m<30;m++,t++)//�ܹ�30����
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10��8��
		{
			int j=x;
			{
				for(int p=0;p<10;p++)//10����
				{
					for(int k=0;k<10;k++)//10��
						for(int l=0;l<8;l++)//8��
						{
							if(lpGray[(i+k+(height_num-2))*g_width+(j+l+(width_num-46))]==g_num[p*80+k*8+l]) 
								tmep_num++; 
						}
						if(tmep_num>75)
						{
							degree[t]=p;
							break;
						}
						else tmep_num=0;
						if(p==9&&(t==0||t==6||t==12||t==18||t==24))
						{
							nocheckout++;
							degree[t]=0;
							break;
						}
						else if(p==9)
						{
							degree[0]=-1;
						}
				}
			}
		}

	}//return degree[30];
	if(nocheckout>5 ) //δ�ҵ�ƥ������
		{
			degree[0]=-1;
			//break;
		}
}

//ƥ��gnum�е�����
int MatchNumber(int x, int y, BYTE* gray, int front_color, int back_color)
{
  int ret = -1;
  int min_different_rows = 1000;//�����ϵ�����
  int min_different_p = 0;
  for (int p = 0; p<10; p++)//10����
  {
    int different_rows = 0;
    for (int k = 0; k < 10; k++)//10��
    {
      int dest_templet_have_front_color = 0;//ͬ����ɫ���֣�ȥ���˴����䣬������ĵ�һ��������
      int dest_color_updown = 0;//dest�����¼
      int templet_color_updown = 0;//templet�����¼
      int dest_temp_color = back_color;//��¼ǰһ��
      int templet_temp_color = back_color;//��¼ǰһ��

      for (int l = 0; l < 8; l++)//8��

      {
        int templet = g_num[p * 80 + k * 8 + l];
        int dest = gray[(y + k + height_num)*g_width + (x + l + width_num)];
       
        if (front_color == dest)
        {
          dest_temp_color = front_color;
        }
        else if (dest_temp_color == front_color)//һ�β���
        {
          dest_color_updown++;
          dest_temp_color = back_color;
        }

        if (front_color == templet)
        {
          templet_temp_color = front_color;
        }
        else if (templet_temp_color == front_color)//һ�β���
        {
          templet_color_updown++;
          templet_temp_color = back_color;
        }

        if (front_color == dest && front_color == templet)
        {
          dest_templet_have_front_color = 1;
        }

        if (7 == l)
        {
          if (dest_temp_color == front_color)//һ�β���
          {
            dest_color_updown++;
            dest_temp_color = back_color;
          }
          if (templet_temp_color == front_color)//һ�β���
          {
            templet_color_updown++;
            templet_temp_color = back_color;
          }
        }

        if (1 == dest_color_updown && 1 == templet_color_updown)//����һ�����䶼����
        {
          if (!dest_templet_have_front_color)//�����һ����û���غ�����
          {
            different_rows++;
            break;
          }
          else
          {
            dest_color_updown = 0;
            templet_color_updown = 0;
            dest_templet_have_front_color = 0;
          }
        }
        else if (1 < dest_color_updown || 1 < templet_color_updown)//�����쳣
        {
          different_rows++;
          break;
        }
        else if (7 == l && dest_color_updown != templet_color_updown)
        {
          different_rows++;
          break;
        }


      }//for (int l = 0; l < 8; l++)//8��
    }//for (int k = 0; k < 10; k++)//10��

    if (different_rows < min_different_rows)
    {
      min_different_rows = different_rows;
      min_different_p = p;
    }

  }//for (int p = 0; p<10; p++)//10����
  if (min_different_rows<3)
  {
    ret = min_different_p;
  }

  return ret;
}
//20171102 mmy
//�Ƚ�30��������Ŀ�����ĸ�ģ���е��������������غ�,�ж�ÿ�е�����

void ScanDegree2(BYTE * lpGray, CString file_temp)
{
#define white 1
#define black 0
  CStdioFile File2;// ����һ��CStdioFile�����File
  CString FileData2;//,file_temp;// ����һ��CString����Ϊһ��������
  CString fileName_position(file_temp);//
  File2.Open(fileName_position, CFile::modeRead);
  int x, y;
  //CString str;
  int min_different_rows = 1000;//�����ϵ�����
  int min_different_p = 0;
  int t = 0;
  int nocheckout = 0;
  // int tmep_num = 0;

  for (int m = 0; m<30; m++, t++)//�ܹ�30����
  {
    min_different_rows = 1000;
    File2.ReadString(FileData2);
    x = _wtoi(FileData2);
    File2.ReadString(FileData2);
    y = _wtoi(FileData2);
    //  tmep_num = 0;
    int i = y;//10��8��
    {
      int j = x;
      {
        min_different_p = MatchNumber(j, i, lpGray, white, black);
        //for (int p = 0; p<10; p++)//10����
        //{
        //  int different_rows = 0;
        //  for (int k = 0; k < 10; k++)//10��
        //  {
        //    int dest_templet_have_white_color = 0;//ͬ����ɫ���֣�ȥ���˴����䣬������ĵ�һ��������
        //    int dest_color_updown = 0;//dest�����¼
        //    int templet_color_updown = 0;//templet�����¼
        //    int dest_temp_color = black;//��¼ǰһ��
        //    int templet_temp_color = black;//��¼ǰһ��

        //    for (int l = 0; l < 8; l++)//8��

        //    {
        //      int templet = g_num[p * 80 + k * 8 + l];
        //      int dest = lpGray[(i + k/* + (height_num - 2)*/)*g_width + (j + l/* + (width_num - 46)*/)];
        //      /*if (lpGray[(i + k + (height_num - 2))*g_width + (j + l + (width_num - 46))] == g_num[p * 80 + k * 8 + l])
        //      tmep_num++;*/
        //      if (white == dest)
        //      {
        //        dest_temp_color = white;
        //      }
        //      else if (dest_temp_color == white)//һ�β���
        //      {
        //        dest_color_updown++;
        //        dest_temp_color = black;
        //      }

        //      if (white == templet)
        //      {
        //        templet_temp_color = white;
        //      }
        //      else if (templet_temp_color == white)//һ�β���
        //      {
        //        templet_color_updown++;
        //        templet_temp_color = black;
        //      }

        //      if (white == dest && white == templet)
        //      {
        //        dest_templet_have_white_color = 1;
        //      }

        //      if (7 == l)
        //      {
        //        if (dest_temp_color == white)//һ�β���
        //        {
        //          dest_color_updown++;
        //          dest_temp_color = black;
        //        }
        //        if (templet_temp_color == white)//һ�β���
        //        {
        //          templet_color_updown++;
        //          templet_temp_color = black;
        //        }
        //      }

        //      if (1 == dest_color_updown && 1 == templet_color_updown)//����һ�����䶼����
        //      {
        //        if (!dest_templet_have_white_color)//�����һ����û���غ�����
        //        {
        //          different_rows++;
        //          break;
        //        }
        //        else
        //        {
        //          dest_color_updown = 0;
        //          templet_color_updown = 0;
        //          dest_templet_have_white_color = 0;
        //        }
        //      }
        //      else if (1 < dest_color_updown || 1 < templet_color_updown)//�����쳣
        //      {
        //        different_rows++;
        //        break;
        //      }
        //      else if (7 == l && dest_color_updown != templet_color_updown)
        //      {
        //        different_rows++;
        //        break;
        //      }


        //    }//for (int l = 0; l < 8; l++)//8��
        //  }//for (int k = 0; k < 10; k++)//10��

        //  if (different_rows < min_different_rows)
        //  {
        //    min_different_rows = different_rows;
        //    min_different_p = p;
        //  }
        //}//for (int p = 0; p<10; p++)//10����

         /////////////////////
        //if (min_different_rows<3)
        //{
        //  degree[t] = min_different_p;
        //  continue;
        //}

        if (min_different_p != -1)
        {
          degree[t] = min_different_p;
          continue;
        }

        //else tmep_num = 0;
        if (t == 0 || t == 6 || t == 12 || t == 18 || t == 24)
        {
          nocheckout++;
          degree[t] = 0;
          continue;
        }
        else
        {
          degree[0] = -1;
        }
      }
    }

  }//return degree[30];
  if (nocheckout>5) //δ�ҵ�ƥ������
  {
    degree[0] = -1;
    //break;
  }
#undef white
#undef black
}
void ScanDegreeEX(BYTE * lpGray,CString file_temp)
{
	CStdioFile File2;// ����һ��CStdioFile�����File
	CString FileData2;//,file_temp;// ����һ��CString����Ϊһ��������
	CString fileName_position(file_temp);//
	File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;

	int t=0;
	int nocheckout=0;
	char tmep_num=0;
	for(int m=0;m<30;m++,t++)//�ܹ�30����
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10��8��
		{
			int j=x;
			{
				for(int p=0;p<10;p++)//10����
				{
					for(int k=0;k<10;k++)//10��
						for(int l=0;l<8;l++)//8��
						{
							if(0==lpGray[(i+k+(height_num-2))*g_width+(j+l+(width_num-46))]&&1==g_num[p*80+k*8+l]) 
								tmep_num++; 
						}
						if(0==tmep_num)
						{
							degree[t]=p;
							break;
						}
						else tmep_num=0;
						if(p==9&&(t==0||t==6||t==12||t==18||t==24))
						{
							nocheckout++;
							degree[t]=0;
							break;
						}
						else if(p==9)
						{
							degree[0]=-1;
						}
				}
			}
		}

	}//return degree[30];
	if(nocheckout>5 ) //δ�ҵ�ƥ������
	{
		degree[0]=-1;
		//break;
	}
}
void ScanDegreeDynamic2(BYTE * lpGray,CString file_temp)
{
	CStdioFile File2;// ����һ��CStdioFile�����File
	CString FileData2;//,file_temp;// ����һ��CString����Ϊһ��������
	CString fileName_position(file_temp);//
	BOOL open = File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;
	
	int t=0;
	char tmep_num=0;
	for(int m=0;m<6;m++,t++)//�ܹ�6����
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10��8��
		{
		int j=x;
			{
        int tmep_num = MatchNumber(j, i, lpGray, 1, 0);

        if (tmep_num != -1)
        {
          degree[t] = tmep_num;
          continue;
        }
        else
        {
          degree[t] = 0;
          continue;
        }
			}
		}
		//if(degree[0]==-1) break;
	}//return degree[30];
}

void ScanDegreeDynamic(BYTE * lpGray, CString file_temp)
{
	CStdioFile File2;// ����һ��CStdioFile�����File
	CString FileData2;//,file_temp;// ����һ��CString����Ϊһ��������
	CString fileName_position(file_temp);//
	BOOL open = File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;

	int t=0;
	char tmep_num=0;
	for(int m=0;m<6;m++,t++)//�ܹ�6����
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10��8��
		{
			int j=x;
			{
				for(int p=0;p<10;p++)//10����
				{
					for(int k=0;k<10;k++)//10��
						for(int l=0;l<8;l++)//8��
						{
							if(lpGray[(i+k+(height_num-2))*g_width+(j+l+(width_num-46))]==g_num[p*80+k*8+l]) 
								tmep_num++; 
						}
						if(tmep_num>75)
						{
							degree[t]=p;
							break;
						}
						else tmep_num=0;
						if(p==9)
						{
							degree[t]=0;
							break;
						}
				}
			}
		}
		//if(degree[0]==-1) break;
	}//return degree[30];
}
bool ScanDegreeEFGM(BYTE * lpGray,CString file_temp)
{
	while(1)
	{
		DWORD width = RGBBitmap1.bmiHeader.biWidth;
		DWORD height = -RGBBitmap1.bmiHeader.biHeight;

		pDest.pdata = new TARGB32[800*600];//b,g,r,a  4��8λ���һ������
		pDest.height = height;
		pDest.width  = width;
		pDest.byte_width = width << 2;

		g_height=height;
		g_width=width;
		lpGray=new BYTE[g_height*g_width];

		int iW=width, iH=height;
		//DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
		//SaveImageToFile(_T("screen_photo\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//����ɼ�ͼ��

		DECODE_RGB_TO_GRAY((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);//ת�Ҷ�:����lpVoidBuffer�����pDest

		//SaveImageToFile(_T("screen_photo\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//����Ҷ�ͼ��
		DECODE_THRESHOLD(pDest,pDest,lpGray,m_VGAthreshold);//ת��ֵ����pDest������0-255�Ķ�ֵ��ͼƬ��lpGray�����0-1�Ķ�ֵ������

		//SaveImageToFile(_T("screen_photo\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//�����ֵ��ͼ��
		delete [] pDest.pdata;
		pDest.pdata=NULL;
		LoadMark(g_path + _T("\\check\\qz_42.txt"));

		FindOrigin(lpGray);
		if(height_num>100 || width_num>200|| (height_num==0&&width_num==0))
		{ 

			//AfxMessageBox(_T("û���ҵ�ͼ����,�������趨��ֵ"));
		}
		else
			break;
	}
	g_num=new int[10*10*8];
	GetNumber();

	//////////////////////////////////////////////////
	CStdioFile File2;// ����һ��CStdioFile�����File
	CString FileData2;//,file_temp;// ����һ��CString����Ϊһ��������
	CString fileName_position(file_temp);//
	File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;
	int M[5]={0};
	int t=0;
	char tmep_num=0;
	for(int m=0;m<5;m++,t++)//�ܹ�5����
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10��8��
		{
			int j=x;
			{
				for(int p=0;p<10;p++)//10����
				{
					for(int k=0;k<10;k++)//10��
						for(int l=0;l<8;l++)//8��
						{
							if(lpGray[(i+k+(height_num-2))*g_width+(j+l+(width_num-46))]!=g_num[p*80+k*8+l]) 
								tmep_num++; 
						}
						if(tmep_num>=75)
						{
							M[t]=p;
							break;
						}
						else tmep_num=0;
						if(p==9)
						{
							M[t]=0;
							break;
						}
				}
			}
		}
		//if(degree[0]==-1) break;
	}//return degree[30];
	delete []g_num;
	delete []lpGray;
	lpGray=NULL;
	g_num=NULL;
	if(0!=M[0]*10000+M[1]*1000+M[2]*100+M[3]*10+M[4])//�ж�Mֵ
		return 1;
	else
		return 0;
}
bool ScanDegreeEFGMEX(BYTE * lpGray,CString file_temp)
{
	while(1)
	{
		DWORD width = RGBBitmap1.bmiHeader.biWidth;
		DWORD height = -RGBBitmap1.bmiHeader.biHeight;

		pDest.pdata = new TARGB32[800*600];//b,g,r,a  4��8λ���һ������
		pDest.height = height;
		pDest.width  = width;
		pDest.byte_width = width << 2;

		g_height=height;
		g_width=width;
		lpGray=new BYTE[g_height*g_width];

		int iW=width, iH=height;
		//DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
		//SaveImageToFile(_T("screen_photo\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//����ɼ�ͼ��

		DECODE_RGB_TO_GRAY((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);//ת�Ҷ�:����lpVoidBuffer�����pDest

		//SaveImageToFile(_T("screen_photo\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//����Ҷ�ͼ��
		DECODE_THRESHOLD(pDest,pDest,lpGray,m_VGAthreshold);//ת��ֵ����pDest������0-255�Ķ�ֵ��ͼƬ��lpGray�����0-1�Ķ�ֵ������

		//SaveImageToFile(_T("screen_photo\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//�����ֵ��ͼ��
		delete [] pDest.pdata;
		pDest.pdata=NULL;
		LoadMark(g_path + _T("\\check\\qz_42.txt"));

		FindOrigin(lpGray);
		if(height_num>100 || width_num>200|| (height_num==0&&width_num==0))
		{ 

			//AfxMessageBox(_T("û���ҵ�ͼ����,�������趨��ֵ"));
		}
		else
			break;
	}
	g_num=new int[10*10*8];
	GetNumber();

	//////////////////////////////////////////////////
	CStdioFile File2;// ����һ��CStdioFile�����File
	CString FileData2;//,file_temp;// ����һ��CString����Ϊһ��������
	CString fileName_position(file_temp);//
	File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;
	int M[5]={0};
	int t=0;
	char tmep_num=0;
	for(int m=0;m<5;m++,t++)//�ܹ�5����
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10��8��
		{
			int j=x;
			{
				for(int p=0;p<10;p++)//10����
				{
					for(int k=0;k<10;k++)//10��
						for(int l=0;l<8;l++)//8��
						{
							if(1==lpGray[(i+k+(height_num-2))*g_width+(j+l+(width_num-46))]&&1==g_num[p*80+k*8+l]) 
								tmep_num++; 
						}
						if(0==tmep_num)
						{
							M[t]=p;
							break;
						}
						else tmep_num=0;
						if(p==9)
						{
							M[t]=0;
							break;
						}
				}
			}
		}
		//if(degree[0]==-1) break;
	}//return degree[30];
	delete []g_num;
	delete []lpGray;
	lpGray=NULL;
	g_num=NULL;
	if(14742==M[0]*10000+M[1]*1000+M[2]*100+M[3]*10+M[4] || 14993==M[0]*10000+M[1]*1000+M[2]*100+M[3]*10+M[4])//�ж�Mֵ
		return 1;
	else
		return 0;
}

//ret 0 success
//ret !0 failure
int CalcEquAngle()
{
  for (int i = 0; i < 30; i++) {
    if (degree[i] > 60 || degree[i] < 0)
      return -1;
  }

  int angle[5] = { 
    (degree[0] * 10 + degree[1]) * 3600 + (degree[2] * 10 + degree[3]) * 60 + (degree[4] * 10 + degree[5]),//����
    (degree[6] * 10 + degree[7]) * 3600 + (degree[8] * 10 + degree[9]) * 60 + (degree[10] * 10 + degree[11]),//����
    (degree[12] * 10 + degree[13]) * 3600 + (degree[14] * 10 + degree[15]) * 60 + (degree[16] * 10 + degree[17]),//��Ч��
    (degree[18] * 10 + degree[19]) * 3600 + (degree[20] * 10 + degree[21]) * 60 + (degree[22] * 10 + degree[23]),//ԭʼ����
    (degree[24] * 10 + degree[25]) * 3600 + (degree[26] * 10 + degree[27]) * 60 + (degree[28] * 10 + degree[29]) 
  };//ԭʼ���᣻����ⶨ���ĽǶȣ�ת������

  int t0 = (degree[0] * 10 + degree[1]) * 3600 + (degree[2] * 10 + degree[3]) * 60 + (degree[4] * 10 + degree[5]) + g_sort.R1t[g_sort.R1Num];//����//_0614
  int e0 = (degree[6] * 10 + degree[7]) * 3600 + (degree[8] * 10 + degree[9]) * 60 + (degree[10] * 10 + degree[11]) + g_sort.R1e[g_sort.R1Num];//����
  int equal_angle;
    
  // ��Ч�� = ԭʼ���� + ��ԭʼ���� - ��Ч�ǵ��������/��Ч������
  if (g_sort.ek) {
    equal_angle = t0 + double((e0 - g_sort.et0)* 1000) / double(g_sort.ek) + 0.5 ;
  }
  else {
    equal_angle = 0;
  }

  degree[12] = equal_angle / 3600 / 10;
  degree[13] = equal_angle / 3600 % 10;
  degree[14] = equal_angle % 3600 / 60 / 10;
  degree[15] = equal_angle % 3600 / 60 % 10;
  degree[16] = equal_angle % 60 / 10;
  degree[17] = equal_angle % 60 % 10;

  return 0;
}
// CDebug �Ի���

IMPLEMENT_DYNAMIC(CDebug, CDialogEx)

	CDebug::CDebug(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDebug::IDD, pParent)
	, m_tip_x(FALSE)
	, m_tip_y(FALSE)
	, m_tip_blow(FALSE)
	//, m_shock(FALSE)
	, m_alert(FALSE)
{
	gp_csDebug = this;
}

CDebug::~CDebug()
{
}

void CDebug::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MOTOR_TURN, m_motorturn);
	DDX_Control(pDX, IDC_LIST1, m_debuglist);
	DDX_Check(pDX, IDC_TIP_X, m_tip_x);
	//  DDX_Control(pDX, IDC_TIP_Y, m_tip_y);
	DDX_Check(pDX, IDC_TIP_Y, m_tip_y);
	DDX_Check(pDX, IDC_TIP_BLOW, m_tip_blow);
	//  DDX_Check(pDX, IDC_SHOCK, m_shock);
	DDX_Control(pDX, IDC_PREVIEW, m_preview);
	DDX_Check(pDX, IDC_ALERT, m_alert);
}


BEGIN_MESSAGE_MAP(CDebug, CDialogEx)
	ON_BN_CLICKED(IDC_MOTOR1_RUN, &CDebug::OnBnClickedMotor1Run)
	ON_BN_CLICKED(IDC_AC_READ, &CDebug::OnBnClickedAcRead)
	ON_BN_CLICKED(IDC_AC_WRITE, &CDebug::OnBnClickedAcWrite)
	ON_BN_CLICKED(IDC_SCRREAD, &CDebug::OnBnClickedScrread)
	ON_BN_CLICKED(IDC_VGA_INIT, &CDebug::OnBnClickedVgaInit)
	ON_BN_CLICKED(IDC_LOAD_PIC, &CDebug::OnBnClickedLoadPic)
	ON_BN_CLICKED(IDC_COMPARE, &CDebug::OnBnClickedCompare)
	ON_BN_CLICKED(IDC_STOP_VGA, &CDebug::OnBnClickedStopVga)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CDebug::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_SORT_MOVE, &CDebug::OnBnClickedSortMove)
	ON_BN_CLICKED(IDC_SORT_TEST, &CDebug::OnBnClickedSortTest)
	ON_BN_CLICKED(IDC_SORT_SET, &CDebug::OnBnClickedSortSet)
	ON_BN_CLICKED(IDC_MOTOR2_RUN, &CDebug::OnBnClickedMotor2Run)
	ON_BN_CLICKED(IDC_TIP_X, &CDebug::OnBnClickedTipX)
	ON_BN_CLICKED(IDC_TIP_Y, &CDebug::OnBnClickedTipY)
	ON_BN_CLICKED(IDC_TIP_BLOW, &CDebug::OnBnClickedTipBlow)
	ON_BN_CLICKED(IDC_Y_ZERO, &CDebug::OnBnClickedYZero)
	ON_BN_CLICKED(IDC_X_ZERO, &CDebug::OnBnClickedXZero)
	ON_BN_CLICKED(IDC_BUTTON1, &CDebug::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_TEST, &CDebug::OnBnClickedTest)
	ON_BN_CLICKED(IDC_DYNAMIC_DETECT, &CDebug::OnBnClickedDynamicDetect)
	ON_BN_CLICKED(IDC_PRINT, &CDebug::OnBnClickedPrint)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_ALERT, &CDebug::OnBnClickedAlert)
	ON_EN_CHANGE(IDC_EDIT1, &CDebug::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_CHK_DEBUG, &CDebug::OnBnClickedChkDebug)
	ON_BN_CLICKED(IDC_CHK_TBLOW, &CDebug::OnBnClickedChkTblow)
	ON_BN_CLICKED(IDC_EASY_SET, &CDebug::OnBnClickedEasySet)
	ON_BN_CLICKED(IDC_MOTOR3_RUN, &CDebug::OnBnClickedMotor3Run)
	ON_BN_CLICKED(IDC_Z_ZERO, &CDebug::OnBnClickedZZero)
	ON_BN_CLICKED(IDC_TIP_Z, &CDebug::OnBnClickedTipZ)
	ON_BN_CLICKED(IDC_CHK_CY, &CDebug::OnBnClickedChkCy)
	ON_BN_CLICKED(IDC_CHK_EFG, &CDebug::OnBnClickedChkEfg)
END_MESSAGE_MAP()

static int PixelFormatToBytesPerPixel[] =
{
	0  /* RGBPIXELFORMAT_UNKNOWN */,
	4  /* RGBPIXELFORMAT_555 */,
	4  /* RGBPIXELFORMAT_565 */,
	8  /* RGBPIXELFORMAT_888 */,
};

void CALLBACK callbackNotify(PVOID NotifyArg,   UINT   message,   WPARAM   wParam,   LPARAM   lParam)
{
	CDebug* psamp = (CDebug*)NotifyArg;
	switch(message)
	{
	case RGBWM_FRAMECAPTURED:
		{
			psamp->OnMyMessage_Sta1(wParam, lParam);//�������Ѿ��ɼ���һ֡ͼ��
			break;
		}
	case RGBWM_MODECHANGED:
		{
			psamp->OnMyMessage_modechange(wParam, lParam);//���ı�ɼ�ģʽ
			break;
		}
	case RGBWM_NOSIGNAL:
		{
			psamp->OnMyMessage_nosignal(wParam, lParam);//û�ź�
			break;
		}
	default:
		{
			break;
		}
	}
	//	::PostMessage(psamp->m_hWnd, message, wParam, lParam);	
}
//���浱ǰ��ʾ�������е�ͼ����Ϣ add wygwl 2010-7-16
BOOL SaveImageToFile(CString sFile, int iW, int iH, BYTE * pInfo) 
{

#define WIDTHBYTES(bits) ((((bits) + 31) >> 5) << 2)
	int nBitCount = 32;
	int nLineBytes = WIDTHBYTES(iW * nBitCount);
	int dwDibSize = sizeof(BITMAPINFOHEADER) + nLineBytes*iH;//ͼ��ͷ��ͼ������

	CString sPathName;
	unsigned char * pDib= NULL;
	{
		sPathName  = sFile;
		pDib = new unsigned char[dwDibSize];
		if (pDib == NULL) 
			return 0;
		memset(pDib, 0, dwDibSize);//��pDibָ�����������dwDibSize�ֽ�
		unsigned char *pDibBits = pDib + sizeof(BITMAPINFOHEADER) ;
		BITMAPINFOHEADER*	pBIH = (BITMAPINFOHEADER*)pDib;
		pBIH->biSize		= sizeof(BITMAPINFOHEADER);
		pBIH->biWidth		= iW;
		pBIH->biHeight		= iH;
		pBIH->biPlanes		= 1;
		pBIH->biBitCount	= nBitCount;
		pBIH->biCompression	= BI_RGB;
		pBIH->biSizeImage	= nLineBytes*iH;
		pBIH->biClrUsed		= 0;

		//		memcpy( pDibBits, , nLineBytes*iH);
		pInfo += nLineBytes*(iH-1);
		for ( int i = 0; i < iH; i++) // ���·���
		{
			memcpy( pDibBits + i*nLineBytes, pInfo-i*nLineBytes, nLineBytes);
		}
	}

	//save bmp
	{
		CFile file;
		CFileException e;
		if (file.Open(sPathName, CFile::modeCreate | CFile::modeWrite, &e))
		{
			BITMAPFILEHEADER bf;
			memset(&bf, 0, sizeof(bf));
			bf.bfType = 'MB';
			bf.bfSize = sizeof(bf) + dwDibSize;
			bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) ;

			file.Write(&bf, sizeof(bf));
			file.Write(pDib, dwDibSize);//��pDibд�뵽�ļ��У�����ΪdwDibSize
			file.Close();
		}
		else
		{
			return FALSE;
		}
		delete [] pDib;
		pDib = NULL;
	}
	return TRUE;
}
// http://blog.sina.com.cn/s/blog_6ee382440100moeu.html
// http://apps.hi.baidu.com/share/detail/32878068
// bitmap λͼCBitmap����ָ��
// lpFileName Ϊλͼ�ļ���  
bool SaveBitmapToFile(CBitmap* bitmap, LPCWSTR lpFileName)
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

/******************************************************************************/
/*
*����SDK��������ϢRGBWM_FRAMECAPTURED
*��ϢRGBWM_FRAMECAPTURED:������ǰ�ɼ�������׼��������������һ֡ͼ�����ݵȴ�������
*/
void CDebug::OnMyMessage_Sta1(WPARAM wParam, LPARAM lParam)
{

	if ((NULL == hRGBCapture) || (0 == hRGBCapture))
	{
		return;
	}
	// 	if (NULL == m_hCapEvent)
	// 	{
	// 		return;
	// 	}

	RGBERROR error;
	if (!signalflag)
	{
		signalflag = TRUE;
		SetWindowText(_T(""));
	}
	pCaptureBuffer.bufferflags = TRUE;

	//////////////////////////////////////////////////////////////////////////

	//if ( g_bSaveImage ) // ����ͼ�񵽴���
	//{
	//	g_bSaveImage = 0;
	//	DWORD width = RGBBitmap1.bmiHeader.biWidth;
	//	DWORD height = -RGBBitmap1.bmiHeader.biHeight;

	//	//---memset(pData,0,1600*1200*4);
	//	TPicRegion pDest;
	//	pDest.pdata = (TARGB32*)pData;
	//	pDest.height = height;
	//	pDest.width  = width;
	//	pDest.byte_width = width << 2;
	//	if (1 < datamode)
	//	{
	//		DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
	//	}
	//	else if (2 > datamode)
	//	{
	//		DECODE_UYVY_TO_RGB32((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
	//	}

	//	int iW=width, iH=height;
	//	SaveImageToFile(g_strFile, iW, iH, (BYTE *)pData);
	//}


	//////////////////////////////////////////////////////////////////////////


	//SetEvent(m_hPThreadEvents[0]);
	//Sleep(1);
	//emp_flag = (1==emp_flag) ? 0 : 1;	
	//while (pCaptureBuffer.bufferflags)
	//{
	//	//AfxMessageBox(_T("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~BUFFER IS NOT EMPTY!!! PAINT NOT FINISHED!!!!"));
	//	Sleep(1);
	//}

	error = RGBCaptureFrameBufferReady (hRGBCapture,pCaptureBuffer.Index,bufferSize);
	if (error != RGBERROR_NO_ERROR)
	{
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  ��⵱ǰVGA�ɼ��������Ƿ���������ʧ�ܣ��򿪵��豸������û���ҵ���Ӳ���豸�����\n"));//���ʧ�ܣ��򿪵��豸������û���ҵ���Ӳ���豸�����
				break;
			}

		case RGBERROR_INVALID_INDEX:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  ��⵱ǰVGA�ɼ��������Ƿ������û���ҵ��豸����\n"));//û���ҵ�����ָ��
				break;
			}

		case RGBERROR_BUFFER_TOO_SMALL:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  ��⵱ǰVGA�ɼ��������Ƿ����������̫С\n"));//û���ҵ�����ָ��
				break;
			}

		default:
			{
				break;
			}
		}
		return;
		//         char text[80];
		//         wsprintf(text, "RGBCaptureFrameBufferReady returned: %s", RGBErrorToString(error));
		//         ::MessageBox(m_hWnd, text, NULL, MB_OK | MB_ICONERROR);
		//AfxMessageBox("RGBCaptureFrameBufferReady returned: %s", RGBErrorToString(error) );
	}  
}
/*****************************************************************************/
/*
*�������ط����ı�ʱSDK��������ϢRGBWM_MODECHANGED
*�������òɼ�����
*/
void CDebug::OnMyMessage_modechange(WPARAM wParam, LPARAM lParam)
{
	unsigned long *pbufferSize = 0L;	
	RGBCAPTUREPARMS CaptureParms;
	if (!SetCapture(m_hWnd, &CaptureParms, &hRGBCapture, pCaptureBuffer, &bufferSize, &RGBBitmap1))  //���òɼ�����
	{
		AfxMessageBox(_T("SetCapture(),���òɼ�����ʧ��"));
		return;
	}
}
/*****************************************************************************/
/*
*���òɼ�����
*/
BOOL CDebug::SetCapture(HWND hWnd
	, RGBCAPTUREPARMS  *pCaptureParms
	, HRGBCAPTURE      *phRGBCapture
	, RGBCAPTUREBUFFER pCaptureBuffer
	, unsigned long    *pBufferSize
	, RGBBITMAP       *pRGBBitmap)
{
	RGBERROR error;
	memset(pCaptureParms, 0, sizeof (*pCaptureParms));

	//////////////////////////////////////////////////////////////////////////
	/*��⵱ǰ�����VGA��Ƶ�źŷֱ���*/
	error = RGBCaptureDetectVideoMode(*phRGBCapture, 0, 0, &pCaptureParms->VideoTimings, FALSE);
	if (RGBERROR_NO_ERROR != error)
	{
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureDetectVideoMode(), ��⵱ǰ�����VGA��Ƶ�źŷֱ��ʣ����ʧ�ܣ��򿪵��豸������û���ҵ���Ӳ���豸�����\n"));//���ʧ�ܣ��򿪵��豸������û���ҵ���Ӳ���豸�����
				break;
			}

		case RGBERROR_INVALID_POINTER:
			{
				AfxMessageBox(_T("RGBCaptureDetectVideoMode(), ��⵱ǰ�����VGA��Ƶ�źŷֱ��ʣ�û���ҵ�����ָ��\n"));//û���ҵ�����ָ��
				break;
			}

		default:
			{
				break;
			}
		}
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	/*���ô��ڱ���*/
	//char szWndTitle[120];
	//wsprintf(szWndTitle, "VGA�ɼ����� ");
	//::SetWindowText(::GetParent(this->m_hWnd), szWndTitle);

	//////////////////////////////////////////////////////////////////////////
	/* ����λͼ����Ϊ����ɼ�������׼�� */

	pRGBBitmap->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pRGBBitmap->bmiHeader.biPlanes = 1;
	pRGBBitmap->bmiHeader.biCompression = BI_BITFIELDS;
	pRGBBitmap->bmiHeader.biSizeImage = 0;
	pRGBBitmap->bmiHeader.biXPelsPerMeter = 3000;
	pRGBBitmap->bmiHeader.biYPelsPerMeter = 3000;
	pRGBBitmap->bmiHeader.biClrUsed = 0;
	pRGBBitmap->bmiHeader.biClrImportant = 0;
	pRGBBitmap->bmiHeader.biWidth = pCaptureParms->VideoTimings.HorAddrTime;
	pRGBBitmap->bmiHeader.biHeight = - (int)pCaptureParms->VideoTimings.VerAddrTime;

	//WaitForSingleObject(m_hDDrawEvent,INFINITE);
	//ResetEvent(m_hDDrawEvent);
	//if (DD_OK !=ddrawRGB.DDRelease())
	//{
	//	AfxMessageBox(_T("CRGBSAMPDlg::SetCapture DDRelease���ɹ�!!! \n"));
	//	//return FALSE;
	//}
	//if (DD_OK !=ddrawRGB.DDInit(hWnd,pCaptureParms->VideoTimings.HorAddrTime,pCaptureParms->VideoTimings.VerAddrTime))
	//{
	//	AfxMessageBox(_T("CRGBSAMPDlg::SetCapture DDInit��ͼ��ʼ�����ɹ�!!! \n"));
	//	//return FALSE;
	//}	
	//SetEvent(m_hDDrawEvent);

	//////////////////////////////////////////////////////////////////////////
	/*�˴���⻺���Ƿ�������ɹ�������ɼ��¼�*/
	error = RGBCaptureFrameBufferReady (hRGBCapture,
		pCaptureBuffer.Index,
		*pBufferSize);
	if (error != RGBERROR_NO_ERROR)
	{
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  ��⵱ǰVGA�ɼ��������Ƿ���������ʧ�ܣ��򿪵��豸������û���ҵ���Ӳ���豸�����\n"));//���ʧ�ܣ��򿪵��豸������û���ҵ���Ӳ���豸�����
				break;
			}

		case RGBERROR_INVALID_INDEX:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  ��⵱ǰVGA�ɼ��������Ƿ������û���ҵ��豸����\n"));//û���ҵ�����ָ��
				break;
			}

		case RGBERROR_BUFFER_TOO_SMALL:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  ��⵱ǰVGA�ɼ��������Ƿ����������̫С\n"));//û���ҵ�����ָ��
				break;
			}

		default:
			{
				break;
			}
		}

		// 		char text[80];
		// 		wsprintf(text, "RGBCaptureFrameBufferReady returned: %s", RGBErrorToString(error));
		// 		::MessageBox(NULL, text, NULL, MB_OK | MB_ICONERROR);
		//AfxMessageBox("RGBCaptureFrameBufferReady returned: %s", RGBErrorToString(error) );

		RGBCaptureClose(*phRGBCapture);
		*phRGBCapture = 0;

		if (!FreeBuffers(*phRGBCapture, hWnd, &pCaptureBuffer))
		{
			AfxMessageBox(_T("FreeBuffers(),�ͷŻ���ʧ�ܣ�"));
			return FALSE;
		}
		return FALSE;
	}
	return TRUE;
}
/*****************************************************************************/
/*
*����SDK�ɼ�������ȷ������ʱ��������ϢRGBWM_NOSIGNAL
*/
void CDebug::OnMyMessage_nosignal(WPARAM wParam, LPARAM lParam)
{
	CString strInfo;
	switch(wParam)
	{
	case NOSIGNAL_NO_RESOLUTION:
		{
			strInfo = _T("�� VGA �� ��(����ķֱ��� !!!)");
			break;
		}
	case NOSIGNAL_DDRCHECK_ERROR:
		{
			strInfo = _T("�� VGA �� ��(DDR �Լ���� !!!)");
			break;
		}
	case NOSIGNAL_H_OVERFLOW:
		{
			strInfo = _T("�� VGA �� ��(ˮƽƫ������� !!!)");
			break;
		}
	case NOSIGNAL_V_OVERFLOW:
		{
			strInfo = _T("�� VGA �� ��(��ֱƫ������� !!!)");
			break;
		}
	default:
		{
			break;
		}
	}
	signalflag = FALSE;

	//this->RedrawWindow();
	//this->m_static.ShowWindow(SW_SHOW);
	//this->m_static.SetWindowText(strInfo);
	//this->SetWindowText(strInfo);
}
/******************************************************************************/ 
/*
*�ͷŻ�����
*/
BOOL CDebug::FreeBuffers(HRGBCAPTURE hRGBCapture, HWND hWnd, RGBCAPTUREBUFFER *pCaptureBuffer)
{
	RGBERROR error;
	HDC hDC = ::GetDC(hWnd);

	if (bUsingAccBuffer1 && (hRGBCapture == hRGBCapture))
	{
		bUsingAccBuffer1 = false;  //�޸ı�־�����Ƿ�ʹ�õĲ���Ϊfalse
	}


	::ReleaseDC(hWnd, hDC);

	if(pCaptureBuffer->LpVoidBuffer)
	{
		//////////////////////////////////////////////////////////////////////////
		/*ɾ����ǰ��VGA�ɼ���������ʹ�ö����е�����*/
		error = RGBCaptureReleaseBuffer(hRGBCapture, pCaptureBuffer->Index);
		if (RGBERROR_NO_ERROR != error)
		{
			switch(error)
			{
			case RGBERROR_INVALID_HRGBCAPTURE:
				{
					AfxMessageBox(_T("RGBCaptureReleaseBuffer(),   ɾ����ǰ��VGA�ɼ���������ʹ�ö����е�������ע��ʧ�ܣ��򿪵��豸������û���ҵ���Ӳ���豸�����\n"));//���ʧ�ܣ��򿪵��豸������û���ҵ���Ӳ���豸�����
					break;
				}
			case RGBERROR_INVALID_POINTER:
				{
					AfxMessageBox(_T("RGBCaptureReleaseBuffer(),    ɾ����ǰ��VGA�ɼ���������ʹ�ö����е�������û���ҵ�ָ��ǰ����洢����ָ��\n"));//û���ҵ�ָ��ǰ����洢����ָ��
					break;
				}
			default:
				{
					break;
				}
			}
			return FALSE;	
		}
		//////////////////////////////////////////////////////////////////////////
		/*�ͷŻ�����*/
		GlobalFree(pCaptureBuffer->LpVoidBuffer); 
		pCaptureBuffer->LpVoidBuffer = NULL;
	}

	//delete pCaptureBuffer;
	return TRUE;
}
/*****************************************************************************/ 
/*
*��������λ��
*/
int CDebug::CountBits(unsigned long  ulValue, int start, int end)
{
	int count = 0, i;
	unsigned long ulMask;

	for (i = start, ulMask = 1 << start; i <= end; i++, ulMask <<= 1)
	{
		if (ulMask & ulValue)
		{
			count++;
		}
	}
	return count;
}
/*****************************************************************************/ 
/*
*��⵱ǰ����ģʽ
*/
unsigned short CDebug::DetectPixelFormat( HWND hWnd, DWORD *pColourMask )
	/*
	* Summary: Detects the pixel format of the graphics card frame buffer.
	*
	* Purpose: 
	*          pColourMask points to an array of three DWORDs. Each DWORD is
	*             loaded with a bit mask representing the bits for red, green
	*             and blue respectively.
	*
	* Return:  One of the following values:
	*             RGBPIXELFORMAT_555,
	*             RGBPIXELFORMAT_565,
	*             RGBPIXELFORMAT_888,
	*             RGBPIXELFORMAT_UNKNOWN.
	*/
{
	HDC            hDC, hDCCompatible;
	HBITMAP        hBitmap, hBitmapOld;
	DWORD          dwPixel, dwMask;
	int            nBytes, end, red, i, green, blue;
	unsigned short format;

	hDC = ::GetDC(hWnd);
	hDCCompatible = CreateCompatibleDC(hDC);

	hBitmap = CreateCompatibleBitmap(hDC, 1, 1);
	hBitmapOld = (HBITMAP)SelectObject(hDCCompatible, hBitmap);

	SetPixel(hDCCompatible, 0, 0, RGB (255, 0, 0));
	nBytes = GetBitmapBits(hBitmap, sizeof (dwPixel), &dwPixel);
	end = (nBytes << 3) - 1;
	red = CountBits(dwPixel, 0, end);

	dwMask = 0;
	for (i = 0; i < nBytes; i++)
	{
		dwMask |= (0xff << ( i << 3 ));
	}
	pColourMask[0] = dwPixel & dwMask;

	SetPixel(hDCCompatible, 0, 0, RGB (0, 255, 0));
	GetBitmapBits(hBitmap, sizeof(dwPixel ), &dwPixel);
	green = CountBits(dwPixel, 0, end);
	pColourMask[1] = dwPixel & dwMask;

	SetPixel(hDCCompatible, 0, 0, RGB (0, 0, 255));
	GetBitmapBits(hBitmap, sizeof (dwPixel), &dwPixel);
	blue = CountBits(dwPixel, 0, end);
	pColourMask[2] = dwPixel & dwMask;

	SelectObject(hDCCompatible, hBitmapOld);
	DeleteDC(hDCCompatible);
	DeleteObject(hBitmap);

	::ReleaseDC(hWnd, hDC);

	if ((green == 5) && (red == 5) && (blue == 5))
	{
		format = RGBPIXELFORMAT_555;
	}
	else if ((green == 6) && (red == 5) && (blue == 5))
	{
		format = RGBPIXELFORMAT_565;
	}
	else if ((green == 8) && (red == 8) && (blue == 8))
	{
		format = RGBPIXELFORMAT_888;
	}
	else
	{
		format = RGBPIXELFORMAT_UNKNOWN;
	}
	return format;
}

/*****************************************************************************/
/*
*����ָ����С���仺��
*/
BOOL CDebug::AllocateBuffers(HRGBCAPTURE hRGBCapture, 
	RGBCAPTUREBUFFER *pCaptureBuffer, 
	HWND hWnd, 
	UINT BufferSize)
{
	//////////////////////////////////////////////////////////////////////////
	/*���仺��*/
	pCaptureBuffer->LpVoidBuffer = GlobalAlloc(GMEM_FIXED, BufferSize);
	if (pCaptureBuffer->LpVoidBuffer == NULL)
	{
		return FALSE;
	}
	AfxMessageBox(_T("%d"),GlobalSize(pCaptureBuffer->LpVoidBuffer));
	//////////////////////////////////////////////////////////////////////////
	/*ע�ᵱǰ��VGA�ɼ��¼�*/
	RGBERROR error;
	error = RGBCaptureUseMemoryBuffer(hRGBCapture, pCaptureBuffer->LpVoidBuffer, BufferSize, &pCaptureBuffer->Index);
	if (RGBERROR_NO_ERROR != error)
	{
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureUseMemoryBuffer(),  ע�ᵱǰ��VGA�ɼ��¼���ע��ʧ�ܣ��򿪵��豸������û���ҵ���Ӳ���豸�����\n"));//���ʧ�ܣ��򿪵��豸������û���ҵ���Ӳ���豸�����
				break;
			}

		case RGBERROR_INVALID_POINTER:
			{
				AfxMessageBox(_T("RGBCaptureUseMemoryBuffer(),  ע�ᵱǰ��VGA�ɼ��¼���û���ҵ�ָ��ǰ����洢����ָ��\n"));//û���ҵ�ָ��ǰ����洢����ָ��
				break;
			}

		case RGBERROR_INVALID_INDEX:
			{
				AfxMessageBox(_T("RGBCaptureUseMemoryBuffer(),  ע�ᵱǰ��VGA�ɼ��¼���û���ҵ���Ӧ��ǰ����洢��������\n"));//û���ҵ���Ӧ��ǰ����洢��������
				break;
			}

		default:
			{
				break; 
			}
		}
		return FALSE;
	}
	if ((bUsingAccBuffer1 == FALSE) && (hRGBCapture == hRGBCapture))
	{
		bUsingAccBuffer1 = true;  //�޸ı�־����ʹ�õĲ���ΪTRUE
	}

	return TRUE;
}
// CDebug ��Ϣ�������
BOOL CDebug::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//debugȫ�ֱ�����ʼ��
	debugupdate=0;
	testStart=0;
	dynamicDetect=0;
	//itemnum���
	int itemnum = g_sort.itemnum;
	CString itemstr = g_sort.itemstr;
	/*if (g_sort.islongsqu)
	{
	itemnum = 12;
	itemstr = _T("����Ƭ�ֵ�����");
	}
	else
	{
	itemnum = 24;
	itemstr = _T("�ֵ�����");
	}*/
	//ini�ļ���ʼ�����ȡ

	CString strFilePath,strValue;
	int iniint=0;
	GetModuleFileName(NULL,strFilePath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
	strFilePath.ReleaseBuffer(); 
	int pos = strFilePath.ReverseFind('\\'); 
	strFilePath = strFilePath.Left(pos)+_T("\\SortSet.ini"); 

	///////////////
	CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
	BOOL ifFind = findini.FindFile(strFilePath);  
	if( ifFind )  
	{
		for(int i=0;i<itemnum+6;i++)
		{
			strValue.Format(_T("%d��"),i+1);
			if(3 == g_status._MOTOR_NUM)
				g_status.Zsort[i]=GetPrivateProfileInt(itemstr,strValue,0,strFilePath);  //д��ȫ�ֱ���
			else
				g_status.Ysort[i]=GetPrivateProfileInt(itemstr,strValue,0,strFilePath);  //д��ȫ�ֱ���

		}
		strValue.Format(_T("%d��"),itemnum+7);
		g_status.Xwait=GetPrivateProfileInt(itemstr,strValue,0,strFilePath);  //д��ȫ�ֱ���
		strValue.Format(_T("%d��"),itemnum+8);
		g_status.Xcheck=GetPrivateProfileInt(itemstr,strValue,0,strFilePath);  //д��ȫ�ֱ���
		strValue.Format(_T("%d��"),itemnum+9);
		g_status.Ywait=GetPrivateProfileInt(itemstr,strValue,0,strFilePath);  //д��ȫ�ֱ���
		strValue.Format(_T("%d��"),itemnum+10);
		g_status.Ypass=GetPrivateProfileInt(itemstr,strValue,0,strFilePath);  //д��ȫ�ֱ���

	}
	//��Ͽ��ʼ��
	m_motorturn.AddString(_T("��ת"));
	m_motorturn.AddString(_T("��ת"));
	m_motorturn.SetCurSel(0);
	//��ʼ��Debugҳlistcontrol�ؼ�
	CRect listRect;

	m_debuglist.GetClientRect(&listRect);
	m_debuglist.SetExtendedStyle(m_debuglist.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_debuglist.InsertColumn(0, _T("��λ"), LVCFMT_CENTER, listRect.Width()/8*2, 0);   
	m_debuglist.InsertColumn(1, _T("����"), LVCFMT_CENTER, listRect.Width()/8*2, 1);   
	m_debuglist.InsertColumn(2, _T("˵��"), LVCFMT_CENTER, listRect.Width()/8*4, 2);

	for(int i=0;i<itemnum+6;i++)
	{
		strValue="";
		strValue.Format(_T("%d"),i+1);//_0611_ԭi+1
		m_debuglist.InsertItem(i, strValue);   
		if(3 == g_status._MOTOR_NUM)
		{
			strValue.Format(_T("%d"),g_status.Zsort[i]);//��
		}
		else
		{
			strValue.Format(_T("%d"),g_status.Ysort[i]);//��
		}
		m_debuglist.SetItemText(i, 1, strValue);   
		strValue.Format(_T("��%02d���ĵ������"),i+1);//��//_0611_ԭi+1
		m_debuglist.SetItemText(i, 2, strValue);   
	}
	strValue.Format(_T("%d"),itemnum+7);m_debuglist.InsertItem(itemnum+6, strValue); strValue.Format(_T("%d"),g_status.Xwait);m_debuglist.SetItemText(itemnum+6, 1, strValue);m_debuglist.SetItemText(itemnum+6, 2, _T("X��ȴ�λ��"));   
	strValue.Format(_T("%d"),itemnum+8);m_debuglist.InsertItem(itemnum+7, strValue); strValue.Format(_T("%d"),g_status.Xcheck);m_debuglist.SetItemText(itemnum+7, 1, strValue);m_debuglist.SetItemText(itemnum+7, 2, _T("X���Ƭλ��"));   
	strValue.Format(_T("%d"),itemnum+9);m_debuglist.InsertItem(itemnum+8, strValue); strValue.Format(_T("%d"),g_status.Ywait);m_debuglist.SetItemText(itemnum+8, 1, strValue);m_debuglist.SetItemText(itemnum+8, 2, _T("Y��ȴ�λ��"));   
	if(3 == g_status._MOTOR_NUM)
	{strValue.Format(_T("%d"),itemnum+10);m_debuglist.InsertItem(itemnum+9, strValue); strValue.Format(_T("%d"),g_status.Ypass);m_debuglist.SetItemText(itemnum+9, 1, strValue);m_debuglist.SetItemText(itemnum+9, 2, _T("Y���Ƭλ��"));   
	}


	// TODO:  ��ʼ��
	signalflag = FALSE;
	//pData = new LPVOID[1600*1200*4];
	datamode = 2;
	bufferSize = 0L;         //��1���ɼ�����Ӧ�Ļ����С
	m_VGAthreshold=110;
	SetDlgItemInt(IDC_EDIT1,m_VGAthreshold,0);
	/*EnableToolTips(TRUE);   //trueΪ����ʾ���ܣ�false Ϊ�ر���ʾ����
	m_tooltip.Create(this);//����
	m_tooltip.SetTipTextColor(RGB(0,0,255));//�趨������ɫ
	m_tooltip.SetDelayTime(150);//�趨����ͣ��ʱ��
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_LOAD),"����ͼƬ�󣬽���ʵʱ��⡣");
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_COMPARE),"һ����ͼƬ�������ʼʵʱ��⡣");
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_STOP_VGA),"ֹͣ�ɼ�����Ҫ���³�ʼ��vga������ٴβɼ���");
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_TEST),"���Ա༭���ڵ�ͼƬ�ļ����ɸ��Ĳ���ͼƬ���µ�����ԡ�");
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_BMP_PATH),"����ͼƬ·��");
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_VGA_THRASHOLD),"���ͼƬʱʹ�õ���ֵ");*/

	// TODO: VGA��ʼ��
	RGBERROR error;
	m_iIndex=0;
	hRGBCapture = 0;
	BufferSize=1024*768*3;
	RGBCAPTUREPARMS CaptureParms;  //��ŵ�ǰ�ɼ����Ĳ���
	error = RGBCaptureInitialise(&m_ulNumofcard);//��ʼ��
	if (RGBERROR_NO_ERROR != error)
	{
		//��ʼ��ʧ�ܣ�����ֵ��Ϊ��RGBERROR_HARDWARE_NOT_FOUND
		if (RGBERROR_HARDWARE_NOT_FOUND == error)
		{
			//û���ҵ�Ӳ���豸,��ӡ��־
			AfxMessageBox(_T("RGBCaptureInitialise() ��ʼ����û���ҵ�Ӳ���豸\n"));
			//break;
			//return(FALSE);
		}
		return 0;
	}
	//�򿪲ɼ���
	error = RGBCaptureOpen(m_iIndex,&hRGBCapture);
	if (RGBERROR_NO_ERROR != error)     //���豸ʧ�ܣ����ؽ�����£�
	{	
		switch (error)
		{
		case RGBERROR_HARDWARE_NOT_FOUND :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸��û���ҵ�Ӳ���豸\n"));//û���ҵ��豸
				break;
			}
		case RGBERROR_INVALID_POINTER :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸��û���ҵ�ָ���Ӳ���豸��ָ��\n"));//û���ҵ��豸
				break;
			}
		case RGBERROR_INVALID_INDEX :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸��û���ҵ�Ӳ���豸������\n"));//û���ҵ��豸
				break;
			}
		case RGBERROR_DEVICE_IN_USE :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸��Ӳ���豸���ڱ�ʹ��\n"));//Ӳ���豸���ڱ�ʹ��
				break;
			}
		case RGBERROR_UNABLE_TO_LOAD_DRIVER :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸�����ܼ�������\n"));//��Ӳ��������
				break;
			}
		case RGBERROR_INVALID_DEVICE :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸��Ӳ��������\n"));//��Ӳ��������
				break;
			}
		default:
			{
				break;
			}
		}
		return 0;
	}
	pCaptureParms.Input=0;//VGA����
	pCaptureParms.datamode=2;//0,uyvy,2,RGB��ʽ
	pCaptureParms.BuffersNumber=0;// ��֡��
	pCaptureParms.graytest=0;
	pCaptureParms.FramespSec=50;// 50֡
	pCaptureParms.Size=sizeof(pCaptureParms);// �ṹ����
	pCaptureParms.Flags= //RGBCAPTURE_PARM_PIXELFORMAT
		RGBCAPTURE_PARM_NOTIFY_ARG
		| RGBCAPTURE_PARM_NOTIFY_FN
		// | RGBCAPTURE_PARM_HWND
		| RGBCAPTURE_PARM_DATAMODE
		| RGBCAPTURE_PARM_GRAYTEST
		| RGBCAPTURE_PARM_FRAMESPSEC
		| RGBCAPTURE_PARM_INPUT
		| RGBCAPTURE_PARM_BUFFERSNUM
		| RGBCAPTURE_PARM_NOTIFY_FLAGS
		| RGBCAPTURE_PARM_VDIF_DESCRIPTION;    //���õĲ����У���ǰ����ģʽ�������ھ����֪ͨ��Ϣģʽ
	//pixelFormat = DetectPixelFormat(m_hWnd,(DWORD *)&pRGBBitmap->colorMark);
	//pCaptureParms.PixelFormat = pixelFormat;
	//pCaptureParms.PixelFormat=RGBPIXELFORMAT_888;
	pCaptureParms.NotifyArg=this;
	pCaptureParms.NotifyFn=callbackNotify;
	pCaptureParms.NotifyFlags = RGBNOTIFY_NO_SIGNAL | RGBNOTIFY_MODE_CHANGE ;
	//pCaptureParms.HWnd=m_hWnd;
	//���ò���
	error = RGBCaptureSetParameters(hRGBCapture, &pCaptureParms,RGBCAPTURE_FLAG_CURRENT|RGBCAPTURE_FLAG_REAL_WORLD);
	if (RGBERROR_NO_ERROR != error)
	{
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ���������û���ҵ�Ӳ���豸\n"));//û���ҵ�Ӳ���豸
				break;
			}

		case RGBERROR_INVALID_POINTER:
			{  
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ���������û���ҵ�����ָ��\n"));//û���ҵ�����ָ��
				break;
			}

		case RGBERROR_INVALID_SIZE:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�������������ָ���С����\n"));//����ָ���С����
				break;
			}

		case RGBERROR_INVALID_FLAGS:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵı�־λ����\n"));//��֧�ֵı�־λ����
				break;
			}

		case RGBERROR_INVALID_FORMAT:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵ������źŸ�ʽ\n"));//��֧�ֵ������źŸ�ʽ
				break;
			}

		case RGBERROR_INVALID_BLACKLEVEL:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵĺ�ɫ��\n"));//��֧�ֵĺ�ɫ��
				break;
			}

		case RGBERROR_INVALID_PIXEL_FORMAT:
			{   
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵ�����ģʽ\n"));//��֧�ֵ�����ģʽ
				break;
			}

		case RGBERROR_INVALID_HWND:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ������������ǵ�ǰ���ھ��\n"));//���ǵ�ǰ���ھ��
				break;
			}

		case RGBERROR_INVALID_SYNCEDGE:
			{   
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵ�ͬ����\n"));//��֧�ֵ�ͬ����
				break;
			}

		case RGBERROR_HSCALED_OUT_OF_RANGE:
			{  
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵ����ű���\n"));//��֧�ֵ����ű���
				break;
			}

		default:
			{
				break;
			}
		}
		return 0;
	}
	/*��ʼ�ɼ��¼�*/
	error = RGBCaptureEnable(hRGBCapture, TRUE);
	if (RGBERROR_NO_ERROR != error)     //���豸ʧ�ܣ����ؽ�����£�
	{	
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() �����ɼ������򿪶�����û���ҵ���Ӳ���豸���\n"));//�򿪶�����û���ҵ���Ӳ���豸���
				break;
			}

		case RGBERROR_CAPTURE_OUTSTANDING:
			{
				AfxMessageBox(_T("RGBCaptureEnable() �����ɼ�����Ӳ���豸û�д�\n"));//Ӳ���豸û�д�
				break;
			}

		case RGBERROR_THREAD_FAILURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() �����ɼ����������߳�ʧ��\n"));//�����߳�ʧ��
				break;
			}

		default:
			{
				break;
			}
		}
		return 0;
	}
	bufferSize =1600 * 1200 * PixelFormatToBytesPerPixel[RGBPIXELFORMAT_888];
	if (!AllocateBuffers(hRGBCapture, &pCaptureBuffer, m_hWnd, bufferSize))
	{
		return 0;
	} 
	//////////////////////////////////////////////////////////////////////////
	/*���òɼ�����*/
	if (!SetCapture(m_hWnd, &CaptureParms, &hRGBCapture, pCaptureBuffer, &bufferSize, &RGBBitmap1)) 
	{
		AfxMessageBox(_T("SetCapture(),���òɼ�����ʧ��"));
		return 0;
	}//��ʼ�����
	UpdateData(1);//�����ֵ
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDebug::OnBnClickedMotor1Run()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(g_hDevice==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("ac6641 ��ʧ��"));
		return;
	}
	int tmp=GetDlgItemInt(IDC_MOTOR_NUM,0,1);//����ת��
	if(tmp>65535||tmp<0)
	{
		AfxMessageBox(_T("����ת������ȷ�򳬳���Χ"));
		return;
	}
	GetDlgItem(IDC_MOTOR1_RUN)->EnableWindow(0);
	int ioData=tmp%256;
	AC6641_DO(g_hDevice,0,ioData);//���Ͷ�ʱ��λ
	//AC6641_DO(g_hDevice,2,ioData);//����Y��ʱ��λ
	ioData=tmp/256,
		AC6641_DO(g_hDevice,1,ioData);//���Ͷ�ʱ��λ
	//AC6641_DO(g_hDevice,3,ioData);//���Ͷ�ʱ��λ
	ioData=AC6641_DI(g_hDevice,5) & 0xfb;//52�͵�ƽ��ȫ����
	AC6641_DO(g_hDevice,5,ioData);
	ioData=AC6641_DI(g_hDevice,4) | 0x02;//��p41�øߣ�Ϊ�½�����׼����
	//ioData=AC6641_DI(g_hDevice,4) | 0x20;//��p41�øߣ�Ϊ�½�����׼����
	tmp=m_motorturn.GetCurSel();//�������ת
	0==tmp?(ioData=ioData & 0xfe):(ioData=ioData | 0x01);
	//0==tmp?(ioData=ioData & 0xef):(ioData=ioData | 0x10);
	AC6641_DO(g_hDevice,4,ioData);//����ת
	Sleep(1);
	ioData=ioData & 0xfd;
	//ioData=ioData & 0xdf;
	AC6641_DO(g_hDevice,4,ioData);//�½���
	Sleep(1);
	ioData=ioData | 0x02;
	//ioData=ioData | 0x20;
	AC6641_DO(g_hDevice,4,ioData);//
	for(int i = 0; i<1000; i++)
	{
		if(!(AC6641_DI(g_hDevice,6) & 0x01))
			break;//���ߵ�ƽʱ
		if(i == 998)
		{
			AfxMessageBox(_T("error"));
			break;
		}
		Sleep(1);
	}
	//while(AC6641_DI(g_hDevice,6) & 0x10);//���ߵ�ƽʱ
	ioData=AC6641_DI(g_hDevice,5) | 0x04;//52�ߵ�ƽ�������
	AC6641_DO(g_hDevice,5,ioData);
	GetDlgItem(IDC_MOTOR1_RUN)->EnableWindow(1);
}




void CDebug::OnBnClickedAcRead()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AC6641_SetIoMode(g_hDevice,0x0,0x0);//��ģʽ����Ӧ7-0��11-8��,=0Ϊ����
	int ioData;
	CString pM,tmp;
	ioData=AC6641_DI(g_hDevice,0);
	/*for(int i=0;i<8;i++)
	{
	tmp.Format(_T("%d"),ioData%2);
	pM=tmp+pM;
	ioData=ioData/2;
	}*/
	pM.Format(_T("%d"),ioData);
	SetDlgItemTextW(IDC_P0,pM);
	ioData=AC6641_DI(g_hDevice,1);
	pM.Format(_T("%d"),ioData);
	SetDlgItemTextW(IDC_P1,pM);
	ioData=AC6641_DI(g_hDevice,2);
	pM.Format(_T("%d"),ioData);
	SetDlgItemTextW(IDC_P2,pM);
	ioData=AC6641_DI(g_hDevice,3);
	pM.Format(_T("%d"),ioData);
	SetDlgItemTextW(IDC_P3,pM);
	ioData=AC6641_DI(g_hDevice,4);
	pM.Format(_T("%d"),ioData);
	SetDlgItemTextW(IDC_P4,pM);
	ioData=AC6641_DI(g_hDevice,5);
	pM.Format(_T("%d"),ioData);
	SetDlgItemTextW(IDC_P5,pM);
	ioData=AC6641_DI(g_hDevice,6);
	pM.Format(_T("%d"),ioData);
	SetDlgItemTextW(IDC_P6,pM);
	ioData=AC6641_DI(g_hDevice,7);
	pM.Format(_T("%d"),ioData);
	SetDlgItemTextW(IDC_P7,pM);
}


void CDebug::OnBnClickedAcWrite()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AC6641_SetIoMode(g_hDevice,0xff,0xf);//��ģʽ����Ӧ7-0��11-8��,=1Ϊ���
	int ioData=GetDlgItemInt(IDC_P0,0,1);
	AC6641_DO(g_hDevice,0,ioData);
	ioData=GetDlgItemInt(IDC_P1,0,1);
	AC6641_DO(g_hDevice,1,ioData);
	ioData=GetDlgItemInt(IDC_P2,0,1);
	AC6641_DO(g_hDevice,2,ioData);
	ioData=GetDlgItemInt(IDC_P3,0,1);
	AC6641_DO(g_hDevice,3,ioData);
	ioData=GetDlgItemInt(IDC_P4,0,1);
	AC6641_DO(g_hDevice,4,ioData);
	ioData=GetDlgItemInt(IDC_P5,0,1);
	AC6641_DO(g_hDevice,5,ioData);
	ioData=GetDlgItemInt(IDC_P6,0,1);
	AC6641_DO(g_hDevice,6,ioData);
	ioData=GetDlgItemInt(IDC_P7,0,1);
	AC6641_DO(g_hDevice,7,ioData);
}

void CreateGGray(CString path, CWnd * pWnd)
{
  CBitmap m_bmp;//�������Ա

  BITMAP bm;//���λͼ��Ϣ�Ľṹ
  CString bmp_path = path;
  HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL, bmp_path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);//����bitmapָ��
                                                                                             //m_bmp.LoadBitmap(bmp_path);
  if (hBitmap1 == NULL)
  {
    DeleteObject(hBitmap1);//�ǵ�ɾ��
    return;
  }
  m_bmp.Attach(hBitmap1);//���������cbitmap����
  m_bmp.GetBitmap(&bm);//

  g_height = bm.bmHeight;//ͼ��Ŀ��    
  g_width = bm.bmWidth;
  g_widthBytes = bm.bmWidthBytes;//ͼ��һ��������ռ���ֽ���
  int size = g_height*g_widthBytes;
  BYTE *lpBits = new BYTE[size];
  m_bmp.GetBitmapBits(size, lpBits);//�õ�RGB����

  if (g_lpGray)
  {
    delete[]g_lpGray;
    g_lpGray = NULL;
  }

  /*BYTE *lpGray*/g_lpGray = new BYTE[g_height*g_width];
  int R, G, B;
  for (int i = 0; i<g_height; i++)
    for (int j = 0; j<g_width; j++)
    {
      B = lpBits[i*g_widthBytes + j * 4];
      G = lpBits[i*g_widthBytes + j * 4 + 1];
      R = lpBits[i*g_widthBytes + j * 4 + 2];
      // lpGray[i*g_width+j]=(R*0.3+G*0.59+B*0.11)>m_VGAthreshold ? 1 : 0;///(B+G+R)/3;//ת�Ҷ�
      g_lpGray[i*g_width + j] = ((B + G + R) / 3)>m_VGAthreshold ? 1 : 0;///(B+G+R)/3;//ת�Ҷ�
    }

  if (pWnd)
  {
    CDC* pDC = pWnd->GetDC();
    CRect rect;
    pWnd->GetClientRect(&rect);

    CDC memDC;        //����һ���豸
    memDC.CreateCompatibleDC(pDC);
    memDC.SelectObject(m_bmp);  //Ϊ�豸ѡ�����//��������Ҫʹ�õ�m_bmp.GetBitmapBits�����ﲻ��ѡ�룬��Ȼ���ò�������
    pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
    
    memDC.DeleteDC();

    pWnd->ReleaseDC(pDC);
  }

  DeleteObject(hBitmap1);//�ǵ�ɾ��	
}


//���뱾��ͼ�񣬾����Ҷ�ת������ֵ��ת����Ȼ���������ƥ�䣬ʶ������
void CDebug::OnBnClickedScrread()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CBitmap m_bmp;//�������Ա

	BITMAP bm;//���λͼ��Ϣ�Ľṹ
	CString bmp_path =g_path + _T("\\screen_photo\\yuanshi.bmp");//_T("d://tmep1.bmp")
	HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,bmp_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//����bitmapָ��
	//m_bmp.LoadBitmap(bmp_path);
	if (hBitmap1 == NULL)
	{
		DeleteObject(hBitmap1);//�ǵ�ɾ��
		return;
	}
	m_bmp.Attach(hBitmap1);//���������cbitmap����
	m_bmp.GetBitmap(&bm);//


	//
	g_height = bm.bmHeight;//ͼ��Ŀ��    
	g_width = bm.bmWidth;
	g_widthBytes=bm.bmWidthBytes;//ͼ��һ��������ռ���ֽ���
	int size=g_height*g_widthBytes; 
	BYTE *lpBits=new BYTE[size]; 
	m_bmp.GetBitmapBits(size,lpBits);//�õ�RGB����
	BYTE *lpGray=new BYTE[g_height*g_width];
	int R, G, B;
	for(int i=0;i<g_height;i++)
		for(int j=0;j<g_width;j++)
		{
		  B=lpBits[i*g_widthBytes+j*4];
		  G=lpBits[i*g_widthBytes+j*4+1];
		  R=lpBits[i*g_widthBytes+j*4+2];
		 // lpGray[i*g_width+j]=(R*0.3+G*0.59+B*0.11)>m_VGAthreshold ? 1 : 0;///(B+G+R)/3;//ת�Ҷ�
          lpGray[i*g_width + j] = ((B + G + R) / 3)>m_VGAthreshold ? 1 : 0;///(B+G+R)/3;//ת�Ҷ�
		}
	//for(int i=0;i<g_height;i++)
	//	for(int j=0;j<g_width;j++)
	//	{
	//		if(lpGray[i*g_width+j]> m_VGAthreshold )//��ֵ��
	//		{
	//			lpGray[i*g_width+j]=1;
	//			/*lpGray[i*widthBytes+j*4+1]=1;
	//			lpGray[i*widthBytes+j*4+2]=1;*/
	//		}
	//		else
	//		{
	//			lpGray[i*g_width+j]=0;
	//			/*lpGray[i*widthBytes+j*4+1]=0;
	//			lpGray[i*widthBytes+j*4+2]=0;*/
	//		}
	//	}
			LoadMark(g_path + _T("\\check\\qz_42.txt"));
			FindOrigin2(lpGray);
			g_num=new int[10*10*8];
			GetNumber();
			ScanDegree2(lpGray,g_path + _T("\\check\\position.txt"));//���degree[]

      CalcEquAngle(); // DATE:180421

			CString _degree,_tmp;
			_degree.Format(_T("����Ƕȣ�%d\r\n����Ƕȣ�%d\r\n��Ч�Ƕȣ�%d\r\nԭʼ���᣺%d\r\nԭʼ���᣺%d"),degree[0]*100000+degree[1]*10000+degree[2]*1000+degree[3]*100+degree[4]*10+degree[5],
				degree[6]*100000+degree[7]*10000+degree[8]*1000+degree[9]*100+degree[10]*10+degree[11],
				degree[12]*100000+degree[13]*10000+degree[14]*1000+degree[15]*100+degree[16]*10+degree[17],
				degree[18]*100000+degree[19]*10000+degree[20]*1000+degree[21]*100+degree[22]*10+degree[23],
				degree[24]*100000+degree[25]*10000+degree[26]*1000+degree[27]*100+degree[28]*10+degree[29]);

			ScanDegreeDynamic2(lpGray,g_path + _T("\\check\\positionD1.txt"));//���degree[]
			_tmp.Format(_T("\r\nD1:%.3f"),degree[0]*100+degree[1]*10+degree[2]+degree[3]*0.1+degree[4]*0.01+degree[5]*0.001);
			_degree +=_tmp;
			ScanDegreeDynamic2(lpGray,g_path + _T("\\check\\positionD2.txt"));//���degree[]
			_tmp.Format(_T("\r\nD2:%.3f"),degree[0]*100+degree[1]*10+degree[2]+degree[3]*0.1+degree[4]*0.01+degree[5]*0.001);
			_degree +=_tmp;
			ScanDegreeDynamic2(lpGray,g_path + _T("\\check\\positionDM.txt"));//���degree[]
			_tmp.Format(_T("\r\nDM:%.3f"),degree[0]*100+degree[1]*10+degree[2]+degree[3]*0.1+degree[4]*0.01+degree[5]*0.001);
			_degree +=_tmp;
			ScanDegreeDynamic2(lpGray,g_path + _T("\\check\\positionR1.txt"));//���degree[]
			_tmp.Format(_T("\r\nR1:%.3f"),degree[0]*100+degree[1]*10+degree[2]+degree[3]*0.1+degree[4]*0.01+degree[5]*0.001);
			_degree +=_tmp;
			ScanDegreeDynamic2(lpGray,g_path + _T("\\check\\positionAL.txt"));//���degree[]
			_tmp.Format(_T("\r\nAL:%.3f"),degree[0]*100+degree[1]*10+degree[2]+degree[3]*0.1+degree[4]*0.01+degree[5]*0.001);
			_degree +=_tmp;

			//
			CWnd * pWnd = GetDlgItem(IDC_PREVIEW);
			CDC* pDC = pWnd->GetDC();
			CRect rect;
			GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
			//

			CDC memDC;        //����һ���豸
			CClientDC dc(this);      //��ȡ�ͻ�
			memDC.CreateCompatibleDC( &dc );
			memDC.SelectObject( m_bmp );  //Ϊ�豸ѡ�����//��������Ҫʹ�õ�m_bmp.GetBitmapBits�����ﲻ��ѡ�룬��Ȼ���ò�������
			pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			DeleteObject(hBitmap1);//�ǵ�ɾ��	
			memDC.DeleteDC();	

			pWnd->ReleaseDC(pDC);


			GetDlgItem(IDC_SHOW_DEG)->SetWindowText(_degree);
			UpdateData(0);
			DeleteObject(hBitmap1);//�ǵ�ɾ��	
			delete []lpBits;
			delete []lpGray;
			delete []g_num;
			g_num=NULL;
			lpBits=NULL;
			lpGray=NULL;

}


void CDebug::OnBnClickedVgaInit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: VGA��ʼ��
	/////////////
	GetDlgItem(IDC_VGA_INIT)->EnableWindow(0);
	GetDlgItem(IDC_LOAD_PIC)->EnableWindow(1);
	//GetDlgItem(IDC_BUTTON_COMPARE)->EnableWindow(0);
	GetDlgItem(IDC_STOP_VGA)->EnableWindow(1);
	/////////////
	RGBERROR error;
	m_iIndex=0;
	hRGBCapture = 0;
	BufferSize=1024*768*3;
	RGBCAPTUREPARMS CaptureParms;  //��ŵ�ǰ�ɼ����Ĳ���
	error = RGBCaptureInitialise(&m_ulNumofcard);//��ʼ��
	if (RGBERROR_NO_ERROR != error)
	{
		//��ʼ��ʧ�ܣ�����ֵ��Ϊ��RGBERROR_HARDWARE_NOT_FOUND
		if (RGBERROR_HARDWARE_NOT_FOUND == error)
		{
			//û���ҵ�Ӳ���豸,��ӡ��־
			AfxMessageBox(_T("RGBCaptureInitialise() ��ʼ����û���ҵ�Ӳ���豸\n"));
			//break;
			//return(FALSE);
		}
		return;
	}
	//�򿪲ɼ���
	error = RGBCaptureOpen(m_iIndex,&hRGBCapture);
	if (RGBERROR_NO_ERROR != error)     //���豸ʧ�ܣ����ؽ�����£�
	{	
		switch (error)
		{
		case RGBERROR_HARDWARE_NOT_FOUND :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸��û���ҵ�Ӳ���豸\n"));//û���ҵ��豸
				break;
			}
		case RGBERROR_INVALID_POINTER :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸��û���ҵ�ָ���Ӳ���豸��ָ��\n"));//û���ҵ��豸
				break;
			}
		case RGBERROR_INVALID_INDEX :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸��û���ҵ�Ӳ���豸������\n"));//û���ҵ��豸
				break;
			}
		case RGBERROR_DEVICE_IN_USE :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸��Ӳ���豸���ڱ�ʹ��\n"));//Ӳ���豸���ڱ�ʹ��
				break;
			}
		case RGBERROR_UNABLE_TO_LOAD_DRIVER :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸�����ܼ�������\n"));//��Ӳ��������
				break;
			}
		case RGBERROR_INVALID_DEVICE :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸��Ӳ��������\n"));//��Ӳ��������
				break;
			}
		default:
			{
				break;
			}
		}
		return;
	}
	pCaptureParms.Input=0;//VGA����
	pCaptureParms.datamode=2;//0,uyvy,2,RGB��ʽ
	pCaptureParms.BuffersNumber=0;// ��֡��
	pCaptureParms.graytest=0;
	pCaptureParms.FramespSec=50;// 50֡
	pCaptureParms.Size=sizeof(pCaptureParms);// �ṹ����
	pCaptureParms.Flags= //RGBCAPTURE_PARM_PIXELFORMAT
		RGBCAPTURE_PARM_NOTIFY_ARG
		| RGBCAPTURE_PARM_NOTIFY_FN
		// | RGBCAPTURE_PARM_HWND
		| RGBCAPTURE_PARM_DATAMODE
		| RGBCAPTURE_PARM_GRAYTEST
		| RGBCAPTURE_PARM_FRAMESPSEC
		| RGBCAPTURE_PARM_INPUT
		| RGBCAPTURE_PARM_BUFFERSNUM
		| RGBCAPTURE_PARM_NOTIFY_FLAGS
		| RGBCAPTURE_PARM_VDIF_DESCRIPTION;    //���õĲ����У���ǰ����ģʽ�������ھ����֪ͨ��Ϣģʽ
	//pixelFormat = DetectPixelFormat(m_hWnd,(DWORD *)&pRGBBitmap->colorMark);
	//pCaptureParms.PixelFormat = pixelFormat;
	//pCaptureParms.PixelFormat=RGBPIXELFORMAT_888;
	pCaptureParms.NotifyArg=this;
	pCaptureParms.NotifyFn=callbackNotify;
	pCaptureParms.NotifyFlags = RGBNOTIFY_NO_SIGNAL | RGBNOTIFY_MODE_CHANGE ;
	//pCaptureParms.HWnd=m_hWnd;
	//���ò���
	error = RGBCaptureSetParameters(hRGBCapture, &pCaptureParms,RGBCAPTURE_FLAG_CURRENT|RGBCAPTURE_FLAG_REAL_WORLD);
	if (RGBERROR_NO_ERROR != error)
	{
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ���������û���ҵ�Ӳ���豸\n"));//û���ҵ�Ӳ���豸
				break;
			}

		case RGBERROR_INVALID_POINTER:
			{  
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ���������û���ҵ�����ָ��\n"));//û���ҵ�����ָ��
				break;
			}

		case RGBERROR_INVALID_SIZE:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�������������ָ���С����\n"));//����ָ���С����
				break;
			}

		case RGBERROR_INVALID_FLAGS:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵı�־λ����\n"));//��֧�ֵı�־λ����
				break;
			}

		case RGBERROR_INVALID_FORMAT:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵ������źŸ�ʽ\n"));//��֧�ֵ������źŸ�ʽ
				break;
			}

		case RGBERROR_INVALID_BLACKLEVEL:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵĺ�ɫ��\n"));//��֧�ֵĺ�ɫ��
				break;
			}

		case RGBERROR_INVALID_PIXEL_FORMAT:
			{   
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵ�����ģʽ\n"));//��֧�ֵ�����ģʽ
				break;
			}

		case RGBERROR_INVALID_HWND:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ������������ǵ�ǰ���ھ��\n"));//���ǵ�ǰ���ھ��
				break;
			}

		case RGBERROR_INVALID_SYNCEDGE:
			{   
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵ�ͬ����\n"));//��֧�ֵ�ͬ����
				break;
			}

		case RGBERROR_HSCALED_OUT_OF_RANGE:
			{  
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵ����ű���\n"));//��֧�ֵ����ű���
				break;
			}

		default:
			{
				break;
			}
		}
		return;
	}
	/*��ʼ�ɼ��¼�*/
	error = RGBCaptureEnable(hRGBCapture, TRUE);
	if (RGBERROR_NO_ERROR != error)     //���豸ʧ�ܣ����ؽ�����£�
	{	
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() �����ɼ������򿪶�����û���ҵ���Ӳ���豸���\n"));//�򿪶�����û���ҵ���Ӳ���豸���
				break;
			}

		case RGBERROR_CAPTURE_OUTSTANDING:
			{
				AfxMessageBox(_T("RGBCaptureEnable() �����ɼ�����Ӳ���豸û�д�\n"));//Ӳ���豸û�д�
				break;
			}

		case RGBERROR_THREAD_FAILURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() �����ɼ����������߳�ʧ��\n"));//�����߳�ʧ��
				break;
			}

		default:
			{
				break;
			}
		}
		return;
	}
	bufferSize =1600 * 1200 * PixelFormatToBytesPerPixel[RGBPIXELFORMAT_888];
	if (!AllocateBuffers(hRGBCapture, &pCaptureBuffer, m_hWnd, bufferSize))
	{
		return ;
	} 
	//////////////////////////////////////////////////////////////////////////
	/*���òɼ�����*/
	if (!SetCapture(m_hWnd, &CaptureParms, &hRGBCapture, pCaptureBuffer, &bufferSize, &RGBBitmap1)) 
	{
		AfxMessageBox(_T("SetCapture(),���òɼ�����ʧ��"));
		return;
	}
	//��ʼ�����
}


void CDebug::OnBnClickedLoadPic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: ����ͼƬ
	/////////////
	//GetDlgItem(IDC_BUTTON_VGAINITIALISE)->EnableWindow(0);
	GetDlgItem(IDC_LOAD_PIC)->EnableWindow(1);
	GetDlgItem(IDC_COMPARE)->EnableWindow(1);
	//GetDlgItem(IDC_BUTTON_STOP_VGA)->EnableWindow(1);
	/////////////
	UpdateData(1);//�����ֵ
	//char * readPath;//
	//int bmpWidth,bmpHeight,biBitCount;
	//unsigned char * pBmpBuf=NULL;
	//readPath=bmp_path.GetBuffer(bmp_path.GetLength());//�»�ȡ���ڲ��ַ���������ָ��
	//int suc=readBmp(readPath,pBmpBuf,bmpWidth,bmpHeight,biBitCount); 

	DWORD width = RGBBitmap1.bmiHeader.biWidth;
	DWORD height = -RGBBitmap1.bmiHeader.biHeight;

	pDest.pdata = new TARGB32[800*600];//b,g,r,a  4��8λ���һ������
	pDest.height = height;
	pDest.width  = width;
	pDest.byte_width = width << 2;

	g_height=height;
	g_width=width;
	g_lpGray=new BYTE[g_height*g_width];

	int iW=width, iH=height;
	DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
	SaveImageToFile(g_path + _T("\\screen_photo\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//����ɼ�ͼ��

	DECODE_RGB_TO_GRAY((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);//ת�Ҷ�:����lpVoidBuffer�����pDest

	SaveImageToFile(g_path + _T("\\screen_photo\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//����Ҷ�ͼ��
	DECODE_THRESHOLD(pDest,pDest,g_lpGray,m_VGAthreshold);//ת��ֵ����pDest������0-255�Ķ�ֵ��ͼƬ��lpGray�����0-1�Ķ�ֵ������
	DECODE_THRESHOLD2(pDest,pDest,m_VGAthreshold);//���ضԻ�
	SaveImageToFile(g_path + _T("\\screen_photo\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//�����ֵ��ͼ��


	delete [] pDest.pdata;
	pDest.pdata=NULL;
	///////////////////////
	CBitmap m_bmp;//�������Ա
	BITMAP bm;//���λͼ��Ϣ�Ľṹ
	CString bmp_path =g_path + _T("\\screen_photo\\erzhihu.bmp");//_T("d://tmep1.bmp")
	HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,bmp_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//����bitmapָ��
	if (hBitmap1 == NULL)
	{
		DeleteObject(hBitmap1);//�ǵ�ɾ��
		return;
	}
	m_bmp.Attach(hBitmap1);//���������cbitmap����
	m_bmp.GetBitmap(&bm);
	//m_bmp.LoadBitmapA(bmp_path);

	CWnd * pWnd = GetDlgItem(IDC_PREVIEW);
	CDC* pDC = pWnd->GetDC();
	CRect rect;
	GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
	//

	CDC memDC;        //����һ���豸
	CClientDC dc(this);      //��ȡ�ͻ�
	memDC.CreateCompatibleDC( &dc );
	memDC.SelectObject( m_bmp );  //Ϊ�豸ѡ�����
	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	DeleteObject(hBitmap1);//�ǵ�ɾ��	
	memDC.DeleteDC();	

	pWnd->ReleaseDC(pDC);
	//pDC->DeleteDC();
	//GetDlgItem(IDC_PRINT)->EnableWindow(1);
}


void CDebug::OnBnClickedCompare()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �Ƚ� 
	/*
	��˵�����Ļ��ʱ��õ��Ļ����е���������ԭʼqz_42���ܶ�λ���ˣ����Կ��Ե����޸�������ݣ�
	����ģ�Ϳ��Թ۲챣��������erzhihua.bmpͼƬ
	*/
	LoadMark(g_path + _T("\\check\\qz_42.txt"));
	/*
	�ҵ�ͼƬ�ı�����
	*/
	FindOrigin2(g_lpGray);
	if(height_num>100 || width_num>100|| (height_num<0&&width_num<0))
	{ 
		AfxMessageBox(_T("û���ҵ�ͼ����,�������趨��ֵ"));
		delete []g_lpGray;
		g_lpGray=NULL;
		GetDlgItem(IDC_COMPARE)->EnableWindow(0);
		return;
	}
	g_num=new int[10*10*8];
	GetNumber();
	ScanDegree2(g_lpGray,g_path + _T("\\check\\position.txt"));//���degree[]
	CString _degree;
	_degree.Format(_T("����Ƕȣ�%d\r\n����Ƕȣ�%d\r\n��Ч�Ƕȣ�%d\r\nԭʼ���᣺%d\r\nԭʼ���᣺%d"),degree[0]*100000+degree[1]*10000+degree[2]*1000+degree[3]*100+degree[4]*10+degree[5],
		degree[6]*100000+degree[7]*10000+degree[8]*1000+degree[9]*100+degree[10]*10+degree[11],
		degree[12]*100000+degree[13]*10000+degree[14]*1000+degree[15]*100+degree[16]*10+degree[17],
		degree[18]*100000+degree[19]*10000+degree[20]*1000+degree[21]*100+degree[22]*10+degree[23],
		degree[24]*100000+degree[25]*10000+degree[26]*1000+degree[27]*100+degree[28]*10+degree[29]);

	GetDlgItem(IDC_SHOW_DEG)->SetWindowText(_degree);
	delete []g_num;
	delete []g_lpGray;
	g_lpGray=NULL;
	g_num=NULL;
	GetDlgItem(IDC_COMPARE)->EnableWindow(0);
}


void CDebug::OnBnClickedStopVga()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: ֹͣ�ɼ�
	/////////////
	GetDlgItem(IDC_VGA_INIT)->EnableWindow(1);
	GetDlgItem(IDC_LOAD_PIC)->EnableWindow(0);
	GetDlgItem(IDC_COMPARE)->EnableWindow(0);
	GetDlgItem(IDC_STOP_VGA)->EnableWindow(0);
	/////////////
	//stopVGA(m_hWnd, &hRGBCapture1, pCaptureBuffer1);
	RGBERROR error;	
	error = RGBCaptureEnable(hRGBCapture, FALSE);
	if (RGBERROR_NO_ERROR != error)     //���豸ʧ�ܣ����ؽ�����£�
	{	
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() �����ɼ������򿪶�����û���ҵ���Ӳ���豸���\n"));//�򿪶�����û���ҵ���Ӳ���豸���
				break;
			}

		case RGBERROR_CAPTURE_OUTSTANDING:
			{
				AfxMessageBox(_T("RGBCaptureEnable() �����ɼ�����Ӳ���豸û�д�\n"));//Ӳ���豸û�д�
				break;
			}

		case RGBERROR_THREAD_FAILURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() �����ɼ����������߳�ʧ��\n"));//�����߳�ʧ��
				break;
			}

		default:
			{
				break;
			}
		}
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	/*�ͷŻ���*/
	if (NULL != &pCaptureBuffer)
	{
		if (!FreeBuffers(hRGBCapture, m_hWnd, &pCaptureBuffer))
		{
			AfxMessageBox(_T("FreeBuffers(),�ͷŻ���ʧ�ܣ�"));
			return;
		}
	}
	/*�رղɼ���*/
	RGBCaptureClose(hRGBCapture);
	hRGBCapture = 0;
}


void CDebug::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	listsortsel=pNMLV->iItem;
	*pResult = 0;
}


void CDebug::OnBnClickedSortMove()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ReturnZero(1);//����
	ReturnZero(2);//����
	if(3 == g_status._MOTOR_NUM)
		ReturnZero(3);//����
	Sleep(500);
	if(3 == g_status._MOTOR_NUM)
	{
		if(listsortsel<g_sort.itemnum + 6)//Y
			MotorCtrl(3,1,g_status.Zsort[listsortsel]);//ת������λ
		else if(listsortsel==g_sort.itemnum+8)//Y
			MotorCtrl(2,1,g_status.Ywait);//�ƶ����ȴ�λ��
		else if(listsortsel==g_sort.itemnum+9)//Y
			MotorCtrl(2,1,g_status.Ypass);//�ƶ�����Ƭλ��
		else if(listsortsel==g_sort.itemnum+6)//X
		{
			MotorCtrl(2,1,g_status.Ywait);//�ƶ����ȴ�λ��
			MotorCtrl(1,1,g_status.Xwait);//�ƶ����ȴ���Ƭλ��
		}
		else if(listsortsel==g_sort.itemnum+7)//X
		{
			MotorCtrl(2,1,g_status.Ywait);//�ƶ����ȴ�λ��
			MotorCtrl(1,1,g_status.Xcheck);//�ƶ�����Ƭλ��
		}
	}
	else
	{
		if(listsortsel<g_sort.itemnum + 6)//Y
			MotorCtrl(2,1,g_status.Ysort[listsortsel]);//ת������λ
		else if(listsortsel==g_sort.itemnum+8)//Y
			MotorCtrl(2,1,g_status.Ywait);//�ƶ����ȴ�λ��
		else if(listsortsel==g_sort.itemnum+6)//X
		{
			MotorCtrl(2,1,g_status.Ywait);//�ƶ����ȴ�λ��
			MotorCtrl(1,1,g_status.Xwait);//�ƶ����ȴ���Ƭλ��
		}
		else if(listsortsel==g_sort.itemnum+7)//X
		{
			MotorCtrl(2,1,g_status.Ywait);//�ƶ����ȴ�λ��
			MotorCtrl(1,1,g_status.Xcheck);//�ƶ�����Ƭλ��
		}
	}
}


void CDebug::OnBnClickedSortTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	unsigned int plusenum = GetDlgItemInt(IDC_PLUSE_NUM,0,0);
	//ReturnZero(1);//����
	//ReturnZero(2);//����
	if(3 == g_status._MOTOR_NUM)
	{
		if(listsortsel<g_sort.itemnum+6)//Z
		{
			ReturnZero(3);//����
			Sleep(500);
			MotorCtrl(3,1,plusenum);//ת������λ
		}
		else if(listsortsel==g_sort.itemnum+8)//Y
		{
			ReturnZero(2);//����
			Sleep(500);
			MotorCtrl(2,1,plusenum);//�ƶ����ȴ�λ��
		}
		else if(listsortsel==g_sort.itemnum+9)//Y
		{
			ReturnZero(2);//����
			Sleep(500);
			MotorCtrl(2,1,plusenum);//�ƶ�����Ƭλ��
		}
		else if(listsortsel==g_sort.itemnum+6)//X
		{
			ReturnZero(1);//����
			Sleep(500);
			MotorCtrl(1,1,plusenum);//�ƶ����ȴ���Ƭλ��
		}
		else if(listsortsel==g_sort.itemnum+7)//X
		{
			ReturnZero(1);//����
			Sleep(500);
			MotorCtrl(1,1,plusenum);//�ƶ�����Ƭλ��
		}
	}
	else
	{
		if(listsortsel<g_sort.itemnum+6)//Y
		{
			ReturnZero(2);//����
			Sleep(500);
			MotorCtrl(2,1,plusenum);//ת������λ
		}
		else if(listsortsel==g_sort.itemnum+8)//Y
		{
			ReturnZero(2);//����
			Sleep(500);
			MotorCtrl(2,1,plusenum);//�ƶ����ȴ�λ��
		}
		else if(listsortsel==g_sort.itemnum+6)//X
		{
			ReturnZero(1);//����
			Sleep(500);
			MotorCtrl(1,1,plusenum);//�ƶ����ȴ���Ƭλ��
		}
		else if(listsortsel==g_sort.itemnum+7)//X
		{
			ReturnZero(1);//����
			Sleep(500);
			MotorCtrl(1,1,plusenum);//�ƶ�����Ƭλ��
		}
	}
}


void CDebug::OnBnClickedSortSet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	unsigned int plusenum = GetDlgItemInt(IDC_PLUSE_NUM,0,0);
	CString str1,str2;
	CString strFilePath;
	/*GetModuleFileName(NULL,strFilePath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
	strFilePath.ReleaseBuffer(); 
	int pos = strFilePath.ReverseFind('\\'); */
	strFilePath = g_path + _T("\\SortSet.ini"); 
	CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
	BOOL ifFind = findini.FindFile(strFilePath);  
	if( ifFind )  
	{
		if(3 == g_status._MOTOR_NUM)
		{
			if(listsortsel<g_sort.itemnum+6)//Y
				g_status.Zsort[listsortsel]=plusenum;
			else if(listsortsel==g_sort.itemnum+8)//X
				g_status.Ywait=plusenum;
			else if(listsortsel==g_sort.itemnum+9)//X
				g_status.Ypass=plusenum;
			else if(listsortsel==g_sort.itemnum+6)//X
				g_status.Xwait=plusenum;
			else if(listsortsel==g_sort.itemnum+7)//X
				g_status.Xcheck=plusenum;
			str2.Format(_T("%d"),plusenum);
			for(int i=0;i<g_sort.itemnum+10;i++)
			{
				str1.Format(_T("%d��"),listsortsel+1);

				WritePrivateProfileString(g_sort.itemstr,str1,str2,strFilePath);  
			}
			m_debuglist.SetItemText(listsortsel, 1, str2);  
		}
		else
		{
			if(listsortsel<g_sort.itemnum+6)//Y
				g_status.Ysort[listsortsel]=plusenum;
			else if(listsortsel==g_sort.itemnum+8)//X
				g_status.Ywait=plusenum;
			else if(listsortsel==g_sort.itemnum+6)//X
				g_status.Xwait=plusenum;
			else if(listsortsel==g_sort.itemnum+7)//X
				g_status.Xcheck=plusenum;
			str2.Format(_T("%d"),plusenum);
			for(int i=0;i<g_sort.itemnum+9;i++)
			{
				str1.Format(_T("%d��"),listsortsel+1);

				WritePrivateProfileString(g_sort.itemstr,str1,str2,strFilePath);  
			}
			m_debuglist.SetItemText(listsortsel, 1, str2);  
		} 
	}
}


void CDebug::OnBnClickedMotor2Run()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(g_hDevice==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("ac6641 ��ʧ��"));
		return;
	}
	int tmp=GetDlgItemInt(IDC_MOTOR_NUM,0,1);//����ת��
	if(tmp>65535||tmp<0)
	{
		AfxMessageBox(_T("����ת������ȷ�򳬳���Χ"));
		return;
	}
	GetDlgItem(IDC_MOTOR2_RUN)->EnableWindow(0);
	int ioData=tmp%256;
	//AC6641_DO(g_hDevice,0,ioData);//���Ͷ�ʱ��λ
	AC6641_DO(g_hDevice,2,ioData);//����Y��ʱ��λ
	ioData=tmp/256,
		//AC6641_DO(g_hDevice,1,ioData);//���Ͷ�ʱ��λ
		AC6641_DO(g_hDevice,3,ioData);//���Ͷ�ʱ��λ
	ioData=AC6641_DI(g_hDevice,5) & 0xf7;//53�͵�ƽ��ȫ����
	AC6641_DO(g_hDevice,5,ioData);
	//ioData=AC6641_DI(g_hDevice,4) | 0x02;//��p41�øߣ�Ϊ�½�����׼����
	ioData=AC6641_DI(g_hDevice,4) | 0x20;//��p41�øߣ�Ϊ�½�����׼����
	tmp=m_motorturn.GetCurSel();//�������ת
	//0==tmp?(ioData=ioData & 0xfe):(ioData=ioData | 0x01);
	0==tmp?(ioData=ioData & 0xef):(ioData=ioData | 0x10);
	AC6641_DO(g_hDevice,4,ioData);//����ת
	Sleep(1);
	//ioData=ioData & 0xfd;
	ioData=ioData & 0xdf;
	AC6641_DO(g_hDevice,4,ioData);//�½���
	Sleep(1);
	//ioData=ioData | 0x02;
	ioData=ioData | 0x20;
	AC6641_DO(g_hDevice,4,ioData);//
	//while(AC6641_DI(g_hDevice,6) & 0x01);//���ߵ�ƽʱ
	//while(AC6641_DI(g_hDevice,6) & 0x10);//���ߵ�ƽʱ
	for(int i = 0; i<1000; i++)
	{
		if(!(AC6641_DI(g_hDevice,6) & 0x10))
			break;//���ߵ�ƽʱ
		if(i == 998)
		{
			AfxMessageBox(_T("error"));
			break;
		}
		Sleep(1);
	}
	ioData=AC6641_DI(g_hDevice,5) | 0x08;//53�ߵ�ƽ�������
	AC6641_DO(g_hDevice,5,ioData);
	GetDlgItem(IDC_MOTOR2_RUN)->EnableWindow(1);
}


void CDebug::OnBnClickedTipX()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(1);//���ؼ���ֵ�������
	__int32 ioData;
	if(m_tip_x)
	{
		ioData=AC6641_DI(g_hDevice,4) | 0x08;// 
		AC6641_DO(g_hDevice,4,ioData);
	}
	else
	{
		ioData=AC6641_DI(g_hDevice,4) & 0xf7;// 
		AC6641_DO(g_hDevice,4,ioData);
	}

}


void CDebug::OnBnClickedTipY()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(1);//���ؼ���ֵ�������
	__int32 ioData;
	if(m_tip_y)
	{
		ioData=AC6641_DI(g_hDevice,4) | 0x80;// 
		AC6641_DO(g_hDevice,4,ioData);
	}
	else
	{
		ioData=AC6641_DI(g_hDevice,4) & 0x7f;// 
		AC6641_DO(g_hDevice,4,ioData);
	}
}


void CDebug::OnBnClickedTipBlow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(1);//���ؼ���ֵ�������
	__int32 ioData;
	if(m_tip_blow)
	{
		ioData=AC6641_DI(g_hDevice,5) | 0x01;// 
		AC6641_DO(g_hDevice,5,ioData);
	}
	else
	{
		ioData=AC6641_DI(g_hDevice,5) & 0xfe;// 
		AC6641_DO(g_hDevice,5,ioData);
	}
}


void CDebug::OnBnClickedYZero()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(g_hDevice==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("ac6641 ��ʧ��"));
		return;
	}
	int tmp=10000;//����ת��
	if(tmp>65535||tmp<0)
	{
		AfxMessageBox(_T("����ת������ȷ�򳬳���Χ"));
		return;
	}
	GetDlgItem(IDC_Y_ZERO)->EnableWindow(0);
	int ioData=tmp%256;
	//AC6641_DO(g_hDevice,0,ioData);//���Ͷ�ʱ��λ
	AC6641_DO(g_hDevice,2,ioData);//����Y��ʱ��λ
	ioData=tmp/256,
		//AC6641_DO(g_hDevice,1,ioData);//���Ͷ�ʱ��λ
		AC6641_DO(g_hDevice,3,ioData);//���Ͷ�ʱ��λ
	ioData=AC6641_DI(g_hDevice,5) & 0xf7;//53�͵�ƽ��ȫ����
	AC6641_DO(g_hDevice,5,ioData);
	//ioData=AC6641_DI(g_hDevice,4) | 0x02;//��p41�øߣ�Ϊ�½�����׼����
	ioData=AC6641_DI(g_hDevice,4) | 0x20;//��p41�øߣ�Ϊ�½�����׼����
	tmp=0;//�������ת
	//0==tmp?(ioData=ioData & 0xfe):(ioData=ioData | 0x01);
	0==tmp?(ioData=ioData & 0xef):(ioData=ioData | 0x10);
	AC6641_DO(g_hDevice,4,ioData);//����ת
	Sleep(1);
	//ioData=ioData & 0xfd;
	ioData=ioData & 0xdf;
	AC6641_DO(g_hDevice,4,ioData);//�½���
	Sleep(1);
	//ioData=ioData | 0x02;
	ioData=ioData | 0x20;
	AC6641_DO(g_hDevice,4,ioData);//
	//while(AC6641_DI(g_hDevice,6) & 0x01);//���ߵ�ƽʱ
	//while(AC6641_DI(g_hDevice,6) & 0x10);//���ߵ�ƽʱ
	for(int i = 0; i<1000; i++)
	{
		if(!(AC6641_DI(g_hDevice,6) & 0x10))
			break;//���ߵ�ƽʱ
		if(i == 998)
		{
			AfxMessageBox(_T("error"));
			break;
		}
		Sleep(1);
	}
	ioData=AC6641_DI(g_hDevice,5) | 0x08;//53�ߵ�ƽ�������
	AC6641_DO(g_hDevice,5,ioData);
	GetDlgItem(IDC_Y_ZERO)->EnableWindow(1);
}


void CDebug::OnBnClickedXZero()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(g_hDevice==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("ac6641 ��ʧ��"));
		return;
	}
	int tmp=10000;//����ת��
	if(tmp>65535||tmp<0)
	{
		AfxMessageBox(_T("����ת������ȷ�򳬳���Χ"));
		return;
	}
	GetDlgItem(IDC_X_ZERO)->EnableWindow(0);
	int ioData=tmp%256;
	AC6641_DO(g_hDevice,0,ioData);//���Ͷ�ʱ��λ
	ioData=tmp/256,
		AC6641_DO(g_hDevice,1,ioData);//���Ͷ�ʱ��λ
	ioData=AC6641_DI(g_hDevice,5) & 0xfb;//52�͵�ƽ��ȫ����
	AC6641_DO(g_hDevice,5,ioData);
	ioData=AC6641_DI(g_hDevice,4) | 0x02;//��p41�øߣ�Ϊ�½�����׼����
	tmp=0;//�������ת
	0==tmp?(ioData=ioData & 0xfe):(ioData=ioData | 0x01);
	AC6641_DO(g_hDevice,4,ioData);//����ת
	Sleep(1);
	ioData=ioData & 0xfd;
	AC6641_DO(g_hDevice,4,ioData);//�½���
	Sleep(1);
	ioData=ioData | 0x02;
	AC6641_DO(g_hDevice,4,ioData);//
	//while(AC6641_DI(g_hDevice,6) & 0x01);//���ߵ�ƽʱ
	for(int i = 0; i<1000; i++)
	{
		if(!(AC6641_DI(g_hDevice,6) & 0x01))
			break;//���ߵ�ƽʱ
		if(i == 998)
		{
			AfxMessageBox(_T("error"));
			break;
		}
		Sleep(1);
	}
	ioData=AC6641_DI(g_hDevice,5) | 0x04;//52�ߵ�ƽ�������
	AC6641_DO(g_hDevice,5,ioData);
	GetDlgItem(IDC_X_ZERO)->EnableWindow(1);
}


void CDebug::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_BUTTON1)->EnableWindow(0);
	if(debugupdate)
	{
		debugupdate=0;
		GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("ʵʱ"));
		Sleep(1000);
	}
	else
	{
		debugupdate=1;
		GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("ֹͣ"));
		p_show=AfxBeginThread(DebugDisplay,NULL,THREAD_PRIORITY_NORMAL,	0,CREATE_SUSPENDED);
		p_show->ResumeThread();


	}
	GetDlgItem(IDC_BUTTON1)->EnableWindow(1);
}


void CDebug::OnBnClickedTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_TEST)->EnableWindow(0);
	if(!testStart)
	{
		GetDlgItem(IDC_TEST)->SetWindowText(_T("ֹͣ"));
		testStart=1;
		p_test=AfxBeginThread(TestStart,NULL,THREAD_PRIORITY_NORMAL,	0,CREATE_SUSPENDED);
		p_test->ResumeThread();

	}
	else
	{
		GetDlgItem(IDC_TEST)->SetWindowText(_T("����"));
		testStart=0;
		Sleep(1000);
	}
	GetDlgItem(IDC_TEST)->EnableWindow(1);
}


void CDebug::OnBnClickedDynamicDetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_DYNAMIC_DETECT)->EnableWindow(0);
	if(!dynamicDetect)
	{
		dynamicNum=GetDlgItemInt(IDC_DETECT_NUM,0,0);
		if(0==dynamicNum)
			dynamicNum=30;
		CStdioFile file;
		file.Open(pathSaveR1,CFile::modeCreate|CFile::modeWrite);
		CString _degree;
		_degree.Format(_T("%d��\r\n���\t����\t����\t��Ч��\tԭʼ��\tԭʼ��\tD1\tD2\tDM\tR1\tAL\r\n"),dynamicNum);
		char szANSIString[MAX_PATH];  
		WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,_degree,-1,szANSIString,sizeof(szANSIString),NULL,NULL); 
		file.Seek(0,CFile::end);
		//myFile.Write(szANSIString,_sort.GetLength()*2-98);
		file.Write(szANSIString,CStringA(_degree).GetLength());

		//file.WriteString( _degree );
		file.Close();
		GetDlgItem(IDC_DYNAMIC_DETECT)->SetWindowText(_T("ֹͣ���"));
		dynamicDetect=1;
		p_dynamic=AfxBeginThread(DynamicDetect,NULL,THREAD_PRIORITY_NORMAL,	0,CREATE_SUSPENDED);
		p_dynamic->ResumeThread();
	}
	else
	{
		/*if(dynamicNum>0)
		{
		CString str;
		str.Format(_T("��ʣ %d��"),dynamicNum);
		int status=AfxMessageBox(str,MB_OKCANCEL);
		if(status==2)//��ȷ������
		{
		GetDlgItem(IDC_DYNAMIC_DETECT)->EnableWindow(1);
		return;
		}
		}*/
		//GetDlgItem(IDC_DYNAMIC_DETECT)->SetWindowText(_T("��̬���"));
		dynamicDetect=0;
		Sleep(1000);
		//SetDlgItemInt(IDC_DETECT_NUM,dynamicNum,0);
	}
	GetDlgItem(IDC_DYNAMIC_DETECT)->EnableWindow(1);
}


void CDebug::OnBnClickedPrint()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	CPrintDialog dlg(FALSE,PD_PAGENUMS);     //�����ӡ��
	//dlg.GetDefaults();
	if (dlg.DoModal() == IDCANCEL)             //��ȡ�û��Դ�ӡ��������
		return;
	HDC hdcPrinter = dlg.GetPrinterDC();       //��ô�ӡ���ľ��
	if (hdcPrinter == NULL)
	{
		MessageBox(_T("Buy a printer!"));
	}
	else
	{
		//GetDlgItem(IDC_PRINT)->EnableWindow(0);
		CDC dcPrinter;                            //����һ���豸
		dcPrinter.Attach(hdcPrinter);             //�Ѵ�ӡ����������豸
		DOCINFO docinfo;                          //��ӡ������
		memset(&docinfo, 0, sizeof(docinfo));
		docinfo.cbSize = sizeof(docinfo);
		docinfo.lpszDocName = _T("SC-EFG");
		CPrintInfo info;
		info.m_rectDraw.SetRect(0,0, 
			dcPrinter.GetDeviceCaps(HORZRES), 
			dcPrinter.GetDeviceCaps(VERTRES));//���÷�Χ
		if (dcPrinter.StartDoc(&docinfo) < 0)
		{
			MessageBox(_T("Printer wouldn't initalize"));
		}
		else
		{
			// start a page
			if (dcPrinter.StartPage() < 0)
			{
				MessageBox(_T("Could not start page"));
				dcPrinter.AbortDoc();
			}
			else
			{
				SetTextColor(dcPrinter,RGB(0, 255, 0));		  //���ô�ӡ����������ɫ����ʵ���������������			
				DrawBmp(&dcPrinter);           //���ô�ӡͼƬ��������׼������Ӧ���ǻ���һ��Ҫ��ӡ��ͼƬ��
				CString str; //��ȡϵͳʱ��
				CTime tm;
				tm=CTime::GetCurrentTime();
				str=tm.Format("%Y-%m-%d %X");
				dcPrinter.TextOut(10, 10, str, str.GetLength());
				dcPrinter.EndPage();            //������ҳ
				dcPrinter.EndDoc();             //����      
			}
		}
		dcPrinter.Detach();             //�ͷŶ���
		dcPrinter.DeleteDC();           //�ͷ��豸

	}

}
void CDebug::DrawBmp(CDC*pDC)
{

	SIZE s1, s2 ;
	s1.cx = pDC->GetDeviceCaps(HORZRES) ;
	s1.cy = pDC->GetDeviceCaps(VERTRES) ;
	pDC->SetMapMode(MM_ANISOTROPIC); //ת������ӳ��ģʽ
	int xLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSX);  //�õ��豸ÿ�߼�Ӣ�����������
	int yLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSY);
	float fWidth = (float)xLogPixPerInch / 96 ; //�õ�������Ļӳ�䵽�Ӵ���С����
	float fHeight = (float)yLogPixPerInch / 96; //һ��õ���fWidth = fHeight 
	CDC memDC;        //����һ���豸
	CClientDC dc(this);      //��ȡ�ͻ�
	memDC.CreateCompatibleDC( &dc );
	CBitmap m_bmp;//�������Ա
	BITMAP bm;//���λͼ��Ϣ�Ľṹ
	CString bmp_path =g_path + _T("\\screen_photo\\erzhihu.bmp");
	HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,bmp_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//����bitmapָ��
	m_bmp.Attach(hBitmap1);//���������cbitmap����
	m_bmp.GetBitmap(&bm);//
	CSize size = CSize(bm.bmWidth, bm.bmHeight);
	pDC->SetWindowExt(size); //�����Ӵ���С
	long xExt = (long)(fWidth * size.cx); //�õ��Ӵ���С
	long yExt = (long)(fHeight * size.cy) ;
	if(xExt>s1.cx)
	{

		yExt=yExt*s1.cx/xExt;
		xExt=s1.cx;
	}
	pDC->SetViewportExt((int)xExt-200, (int)yExt-200); //�����Ӵ���С(�ɵ�����Ļӳ�䵽�Ӵ���С)
	memDC.SelectObject( m_bmp );  //Ϊ�豸ѡ�����
	pDC->BitBlt(40, 40,(int)xExt, (int)yExt, &memDC, 0, 0, SRCCOPY);             //��Դ��copy����ʽװ�ص�pDC�豸��
	DeleteObject(hBitmap1);//�ǵ�ɾ��	
	memDC.DeleteDC();	
}

void CDebug::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	if (g_sort.chgsqu)
	{//itemnum���
		g_sort.chgsqu = 0;
		m_debuglist.DeleteAllItems();
		int itemnum = g_sort.itemnum;
		CString itemstr = g_sort.itemstr;
		/*if (g_sort.islongsqu)
		{
		itemnum = 12;
		itemstr = _T("����Ƭ�ֵ�����");
		}
		else
		{
		itemnum = 24;
		itemstr = _T("�ֵ�����");
		}*/
		//ini�ļ���ʼ�����ȡ

		CString strValue;
		//int iniint=0;
		//CString strFilePath;
		//strFilePath = g_path + _T("\\SortSet.ini"); 

		/////////////////
		//CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
		//BOOL ifFind = findini.FindFile(strFilePath);  
		//if( ifFind )  
		//{
		//	for(int i=0;i<itemnum+6;i++)
		//	{
		//		strValue.Format(_T("%d��"),i+1);
		//		g_status.Ysort[i]=GetPrivateProfileInt(itemstr,strValue,0,strFilePath);  //д��ȫ�ֱ���
		//	}
		//	strValue.Format(_T("%d��"),itemnum+7);
		//	g_status.Xwait=GetPrivateProfileInt(itemstr,strValue,0,strFilePath);  //д��ȫ�ֱ���
		//	strValue.Format(_T("%d��"),itemnum+8);
		//	g_status.Xcheck=GetPrivateProfileInt(itemstr,strValue,0,strFilePath);  //д��ȫ�ֱ���
		//	strValue.Format(_T("%d��"),itemnum+9);
		//	g_status.Ywait=GetPrivateProfileInt(itemstr,strValue,0,strFilePath);  //д��ȫ�ֱ���
		//}

		for(int i=0;i<itemnum+6;i++)
		{
			strValue="";
			strValue.Format(_T("%d"),i+1);//_0611_ԭi+1
			m_debuglist.InsertItem(i, strValue);   
			if(3 == g_status._MOTOR_NUM)
				strValue.Format(_T("%d"),g_status.Zsort[i]);//��
			else
				strValue.Format(_T("%d"),g_status.Ysort[i]);//��
			m_debuglist.SetItemText(i, 1, strValue);   
			strValue.Format(_T("��%02d���ĵ������"),i+1);//��//_0611_ԭi+1
			m_debuglist.SetItemText(i, 2, strValue);   
		}
		strValue.Format(_T("%d"),itemnum+7);m_debuglist.InsertItem(itemnum+6, strValue); strValue.Format(_T("%d"),g_status.Xwait);m_debuglist.SetItemText(itemnum+6, 1, strValue);m_debuglist.SetItemText(itemnum+6, 2, _T("X��ȴ�λ��"));   
		strValue.Format(_T("%d"),itemnum+8);m_debuglist.InsertItem(itemnum+7, strValue); strValue.Format(_T("%d"),g_status.Xcheck);m_debuglist.SetItemText(itemnum+7, 1, strValue);m_debuglist.SetItemText(itemnum+7, 2, _T("X���Ƭλ��"));   
		strValue.Format(_T("%d"),itemnum+9);m_debuglist.InsertItem(itemnum+8, strValue); strValue.Format(_T("%d"),g_status.Ywait);m_debuglist.SetItemText(itemnum+8, 1, strValue);m_debuglist.SetItemText(itemnum+8, 2, _T("Y��ȴ�λ��"));   
	}

	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CString bmp_path =g_path + _T("\\screen_photo\\erzhihu.bmp");//_T("d://tmep1.bmp")
	CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
	BOOL ifFind = findini.FindFile(bmp_path);  
	if( !ifFind )  
		return;
	CBitmap m_bmp;//�������Ա
	BITMAP bm;//���λͼ��Ϣ�Ľṹ
	HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,bmp_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//����bitmapָ��
	if (hBitmap1 == NULL)
	{
		DeleteObject(hBitmap1);//�ǵ�ɾ��
		return;
	}
	m_bmp.Attach(hBitmap1);//���������cbitmap����
	m_bmp.GetBitmap(&bm);
	//m_bmp.LoadBitmapA(bmp_path);

	CWnd * pWnd = GetDlgItem(IDC_PREVIEW);
	CDC* pDC = pWnd->GetDC();
	CRect rect;
	GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
	//

	CDC memDC;        //����һ���豸
	CClientDC dc1(this);      //��ȡ�ͻ�
	memDC.CreateCompatibleDC( &dc1 );
	memDC.SelectObject( m_bmp );  //Ϊ�豸ѡ�����
	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	DeleteObject(hBitmap1);//�ǵ�ɾ��	
	memDC.DeleteDC();	

	pWnd->ReleaseDC(pDC);
	//pDC->DeleteDC();

}


void CDebug::OnBnClickedAlert()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(1);//���ؼ���ֵ�������
	__int32 ioData;
	if(m_alert)
	{
		ioData=AC6641_DI(g_hDevice,5) | 0x10;// 
		AC6641_DO(g_hDevice,5,ioData);
	}
	else
	{
		ioData=AC6641_DI(g_hDevice,5) & 0xef;// 
		AC6641_DO(g_hDevice,5,ioData);
	}
}


void CDebug::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_VGAthreshold=GetDlgItemInt(IDC_EDIT1,0,0);
}


void CDebug::OnBnClickedChkDebug()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_status._DEBUGSCR = ((CButton*)GetDlgItem(IDC_CHK_DEBUG))->GetCheck();
}


void CDebug::OnBnClickedChkTblow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool chk = ((CButton*)GetDlgItem(IDC_CHK_TBLOW))->GetCheck();
	if(chk)
	{
		TBLOW;
	}
	else
	{
		TNOBLOW;
	}
}


void CDebug::OnBnClickedEasySet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(3 == g_status._MOTOR_NUM)
	{
		CString str1,str2;
		CString strFilePath;
		int num = g_sort.itemnum+6;//��λ��
		int firstStep = g_status.Zsort[0];
		int secondStep = g_status.Zsort[1];
		int endStep = g_status.Zsort[g_sort.itemnum+5];
		strFilePath = g_path + _T("\\SortSet.ini"); 
		CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
		BOOL ifFind = findini.FindFile(strFilePath);  
		if( ifFind )  
		{
			switch(AfxMessageBox(_T("�Ե�һ���ڶ���Ϊ���գ�"),MB_YESNOCANCEL))
			{
			case 6:
				for (int i=2;i<g_sort.itemnum+6;i++)
				{
					g_status.Zsort[i] = firstStep + (secondStep - firstStep)*i;

					str2.Format(_T("%d"),g_status.Zsort[i]);
					str1.Format(_T("%d��"),i+1);

					WritePrivateProfileString(g_sort.itemstr,str1,str2,strFilePath);  
					m_debuglist.SetItemText(i, 1, str2);   
				}
				break;
			case 7:
				for (int i=1;i<g_sort.itemnum+5;i++)
				{
					g_status.Zsort[i] = firstStep + (endStep - firstStep)*i/(num - 1);

					str2.Format(_T("%d"),g_status.Zsort[i]);
					str1.Format(_T("%d��"),i+1);

					WritePrivateProfileString(g_sort.itemstr,str1,str2,strFilePath);  
					m_debuglist.SetItemText(i, 1, str2);   
				}
				break;
			case 2:
				Sleep(1);
				break;
			default:
				break;
			}


		}
	}
	else
	{
		CString str1, str2;
		CString strFilePath;
		int num = g_sort.itemnum + 6;//��λ��
		int firstStep = g_status.Ysort[0];
		int secondStep = g_status.Ysort[1];
		int endStep = g_status.Ysort[g_sort.itemnum + 5];
		strFilePath = g_path + _T("\\SortSet.ini");
		CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
		BOOL ifFind = findini.FindFile(strFilePath);
		if (ifFind)
		{
			switch (AfxMessageBox(_T("�Ե�һ���ڶ���Ϊ���գ�"), MB_YESNOCANCEL))
			{
			case 6:
				for (int i = 2; i<g_sort.itemnum + 6; i++)
				{
					g_status.Ysort[i] = firstStep + (secondStep - firstStep)*i;

					str2.Format(_T("%d"), g_status.Ysort[i]);
					str1.Format(_T("%d��"), i + 1);

					WritePrivateProfileString(g_sort.itemstr, str1, str2, strFilePath);
					m_debuglist.SetItemText(i, 1, str2);
				}
				break;
			case 7:
				for (int i = 1; i<g_sort.itemnum + 5; i++)
				{
					g_status.Ysort[i] = firstStep + (endStep - firstStep)*i / (num - 1);

					str2.Format(_T("%d"), g_status.Ysort[i]);
					str1.Format(_T("%d��"), i + 1);

					WritePrivateProfileString(g_sort.itemstr, str1, str2, strFilePath);
					m_debuglist.SetItemText(i, 1, str2);
				}
				break;
			case 2:
				Sleep(1);
				break;
			default:
				break;
			}


		}
	}
}


void CDebug::OnBnClickedMotor3Run()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(g_hDevice==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("ac6641 ��ʧ��"));
		return;
	}
	int tmp=GetDlgItemInt(IDC_MOTOR_NUM,0,1);//����ת��
	int tmpfx=m_motorturn.GetCurSel();//�������ת
	if(tmp>65535||tmp<0)
	{
		AfxMessageBox(_T("����ת������ȷ�򳬳���Χ"));
		return;
	}
	GetDlgItem(IDC_MOTOR3_RUN)->EnableWindow(0);

	MotorCtrl(3,tmpfx,tmp);//
	GetDlgItem(IDC_MOTOR3_RUN)->EnableWindow(1);
}


void CDebug::OnBnClickedZZero()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(g_hDevice==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("ac6641 ��ʧ��"));
		return;
	}
	GetDlgItem(IDC_Z_ZERO)->EnableWindow(0);

	ReturnZero(3);
	GetDlgItem(IDC_Z_ZERO)->EnableWindow(1);

}


void CDebug::OnBnClickedTipZ()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool chk = ((CButton*)GetDlgItem(IDC_TIP_Z))->GetCheck();
	TubeCtrl(3,chk);
}


void CDebug::OnBnClickedChkCy()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool chk = ((CButton*)GetDlgItem(IDC_CHK_CY))->GetCheck();
	CylinderCtrlDbg(chk);
}


void CDebug::OnBnClickedChkEfg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool chk = ((CButton*)GetDlgItem(IDC_CHK_EFG))->GetCheck();
	StartFTLong(chk);
}

