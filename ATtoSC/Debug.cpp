// Debug.cpp : 实现文件
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
extern void MotorCtrl(int n,bool zf,int steps);//控制1、2号电机
extern void CylinderCtrlDbg(bool stat);
extern void TubeCtrl(int n,bool stat);//控制1、2号机械臂的气嘴开关
extern void SortBlock(int n);//分档
extern void VibrateBlock();//震料器
extern void ReturnZero(int n);//控制1、2号机械臂回零位
extern void BlowBlock();//吹气
extern void StartFTLong(bool se);
extern int head[10][8],height_num,width_num;//图片标志数据
//px1000 是2 46  c199是 1 41
#define HEAD_H_OFFSET 2
#define HEAD_W_OFFSET 46
extern RGBCAPTUREBUFFER pCaptureBuffer;
extern RGBBITMAP       RGBBitmap1;     //第一个采集事件位图信息
extern TPicRegion pDest;
extern double m_VGAthreshold;
int listsortsel;//标记点中的list中的编号
bool DebugShow;//实时显示标志
bool testStart;//efg信号收发标志
bool dynamicDetect;//动态检测线程标志
unsigned int dynamicNum;//动态检测次数
CDebug g_csDebug;
CDebug *gp_csDebug = NULL;
//CString pathSaveR1=(_T(".\\check\\R1Number.txt"));_0610
UINT DebugDisplay(LPVOID pParam)//显示
{
	while(1)
	{
		if(!debugupdate)
			return 0;
		::PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_DISPLAY,0,NULL);//发送显示消息给主窗口
		Sleep(100);
	}
	return 0;
}
UINT TestStart(LPVOID pParam)//efg信号收发测试
{
	while(testStart)
	{

		StartFT();//发开始检测信号
		while(1)//接收
		{
			if(AC6641_DI(g_hDevice,6) & 0x02)//当高电平
				if(AC6641_DI(g_hDevice,6) & 0x02)
				{
					while(1)
					{
						if(!(AC6641_DI(g_hDevice,6) & 0x02))//当低时
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
UINT DynamicDetect(LPVOID pParam)//动态检测
{
	CStdioFile file;
	file.Open(pathSaveR1,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
	CString _degree,_tmp;
	int nNum = 0;
	ReturnZero(1);//归零
	ReturnZero(2);//归零
	Sleep(500);
	MotorCtrl(2,1,g_status.Ywait);//移动到等待位置
	MotorCtrl(1,1,g_status.Xwait);//移动到等待上片位置
	StartFT();//发开始检测信号
	Sleep(4000);
	while(dynamicDetect && nNum++<dynamicNum)//-->0)
	{
		/*ReturnZero(2);//归零
		Sleep(500);
		TubeCtrl(2,1);//吸气
		Sleep(500);
		TubeCtrl(2,0);//关气
		Sleep(500);
		MotorCtrl(2,1,g_status.Ywait);//移动到等待位置

		StartFT();//发开始检测信号*/

		MotorCtrl(1,1,g_status.Xcheck-g_status.Xwait);//移动到上片位置
		Sleep(500);
		TubeCtrl(1,1);//吸气
		Sleep(g_tim.onX);
		TubeCtrl(1,0);//关气

		Sleep(g_tim.offX);

		ReturnZero(1);//归零

		Sleep(300);
		StartFT();//发开始检测信号
		MotorCtrl(1,1,g_status.Xwait);//移动到等待上片位置

		while(dynamicDetect && 1)//接收
		{
			if(AC6641_DI(g_hDevice,6) & 0x02)//当高电平
			{
				Sleep(50);
				if(AC6641_DI(g_hDevice,6) & 0x02)
				{
					Sleep(50);
					while(dynamicDetect &&1)
					{
						if(!(AC6641_DI(g_hDevice,6) & 0x02))//当低时
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

		pDest.pdata = new TARGB32[800*600];//b,g,r,a  4个8位组成一个像素
		pDest.height = height;
		pDest.width  = width;
		pDest.byte_width = width << 2;

		g_height=height;
		g_width=width;
		g_lpGray=new BYTE[g_height*g_width];

		int iW=width, iH=height;
		DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
		//SaveImageToFile(_T("screen_photo\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存采集图像

		DECODE_RGB_TO_GRAY((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);//转灰度:输入lpVoidBuffer，输出pDest

		//SaveImageToFile(_T("screen_photo\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存灰度图像
		DECODE_THRESHOLD(pDest,pDest,g_lpGray,m_VGAthreshold);//转二值化，pDest里存的是0-255的二值化图片，lpGray存的是0-1的二值化数据

		//SaveImageToFile(_T("screen_photo\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存二值化图像
		delete [] pDest.pdata;
		pDest.pdata=NULL;
		LoadMark(g_path + _T("\\check\\qz_42.txt"));

		FindOrigin(g_lpGray);
		if(height_num>100 || width_num>200|| (height_num==0&&width_num==0))
		{ 
			//AfxMessageBox(_T("没有找到图像标记,请重新设定阈值"));
			dynamicNum++;
			delete []g_lpGray;
			g_lpGray=NULL;
			continue;
		}
		g_num=new int[10*10*8];
		GetNumber();
		ScanDegree(g_lpGray,g_path + _T("\\check\\position.txt"));//获得degree[]
		_degree.Format(_T("%d\t%d\t%d\t%d\t%d\t%d"),nNum,degree[0]*100000+degree[1]*10000+degree[2]*1000+degree[3]*100+degree[4]*10+degree[5],
			degree[6]*100000+degree[7]*10000+degree[8]*1000+degree[9]*100+degree[10]*10+degree[11],
			degree[12]*100000+degree[13]*10000+degree[14]*1000+degree[15]*100+degree[16]*10+degree[17],
			degree[18]*100000+degree[19]*10000+degree[20]*1000+degree[21]*100+degree[22]*10+degree[23],
			degree[24]*100000+degree[25]*10000+degree[26]*1000+degree[27]*100+degree[28]*10+degree[29]);

		ScanDegreeDynamic(g_lpGray,g_path + _T("\\check\\positionD1.txt"));//获得degree[]
		_tmp.Format(_T("\tD1:%.3f"),degree[0]*100+degree[1]*10+degree[2]+degree[3]*0.1+degree[4]*0.01+degree[5]*0.001);
		_degree +=_tmp;
		ScanDegreeDynamic(g_lpGray,g_path + _T("\\check\\positionD2.txt"));//获得degree[]
		_tmp.Format(_T("\tD2:%.3f"),degree[0]*100+degree[1]*10+degree[2]+degree[3]*0.1+degree[4]*0.01+degree[5]*0.001);
		_degree +=_tmp;
		ScanDegreeDynamic(g_lpGray,g_path + _T("\\check\\positionDM.txt"));//获得degree[]
		_tmp.Format(_T("\tDM:%.3f"),degree[0]*100+degree[1]*10+degree[2]+degree[3]*0.1+degree[4]*0.01+degree[5]*0.001);
		_degree +=_tmp;
		ScanDegreeDynamic(g_lpGray,g_path + _T("\\check\\positionR1.txt"));//获得degree[]
		_tmp.Format(_T("\tR1:%.3f"),degree[0]*100+degree[1]*10+degree[2]+degree[3]*0.1+degree[4]*0.01+degree[5]*0.001);
		_degree +=_tmp;
		ScanDegreeDynamic(g_lpGray,g_path + _T("\\check\\positionAL.txt"));//获得degree[]
		_tmp.Format(_T("\tAL:%.3f\r\n"),degree[0]*100+degree[1]*10+degree[2]+degree[3]*0.1+degree[4]*0.01+degree[5]*0.001);
		_degree +=_tmp;

		delete []g_num;
		delete []g_lpGray;
		g_lpGray=NULL;
		g_num=NULL;
		///
		//保存角度到文件
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
	AfxMessageBox(_T("动态检测结束"));
	gp_csDebug->SetDlgItemText(IDC_DYNAMIC_DETECT,_T("开始检测"));
	return 0;
}
void LoadMark(CString file_temp)
{
	CStdioFile File2;// 定义一个CStdioFile类变量File
	CString FileData2;//,file_temp;// 定义一个CString，作为一个缓冲区
	//file_temp="check\\qz_42.txt";   //保存文件相机参数文件；
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
				if(tmep_num>75)//判断图片标记
				{
					first=true;
					height_num=i;
					width_num=j;
					strTmp.Format(_T("输出行，列值：%d，%d ",i,j));
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
#define white 1//表示前景
#define black 0//表示背景
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
      for (int x = 0; x<10; x++)//10行
      {
        
        int dest_templet_have_white_color = 0;//同白颜色出现，去掉此次跳变，从字体的第一个边起算
        int dest_color_updown = 0;//dest跳变记录
        int templet_color_updown = 0;//templet跳变记录
        int dest_temp_color = black;//记录前一点
        int templet_temp_color = black;//记录前一点

        for (int y = 0; y<8; y++)//8列

        {
          int templet = head[x][y];
          int dest = lpGray[(i + x)*g_width + (j + y)];
          if (white == dest && 7 != y)
          {
            dest_temp_color = white;
          }
          else if (dest_temp_color == white)//一次波动
          {
            dest_color_updown++;
            dest_temp_color = black;
          }

          if (white == templet && 7 != y)
          {
            templet_temp_color = white;
          }
          else if (templet_temp_color == white)//一次波动
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
            if (dest_temp_color == white)//一次波动
            {
              dest_color_updown++;
              dest_temp_color = black;
            }
            if (templet_temp_color == white)//一次波动
            {
              templet_color_updown++;
              templet_temp_color = black;
            }
          }


          if (1 == dest_color_updown && 1 == templet_color_updown)//两个一次跳变都结束
          {
            if (!dest_templet_have_white_color)//字体的一部分没有重合象素
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
          else if (1 < dest_color_updown || 1 < templet_color_updown)//跳变异常
          {
            different_rows++;
            break;
          }
          else if (7 == y && dest_color_updown != templet_color_updown)
          {
            different_rows++;
            break;
          }

        }//for (int l = 0; l < 8; l++)//8列
      }//for (int k = 0; k < 10; k++)//10行

      if (different_rows == 0)//判断图片标记
      {
        first = true;
        height_num = i- HEAD_H_OFFSET;
        width_num = j- HEAD_W_OFFSET;
        strTmp.Format(_T("输出行，列值：%d，%d ", i, j));
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
				if(0==w_num&&b_num<3)//判断图片标记
				{
					first=true;
					height_num=i;
					width_num=j;
					strTmp.Format(_T("输出行，列值：%d，%d ",i,j));
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
//从图片中扫描角度
void GetNumber()
{

	CStdioFile File2;// 定义一个CStdioFile类变量File
	CString FileData2,file_temp;// 定义一个CString，作为一个缓冲区
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
	CStdioFile File2;// 定义一个CStdioFile类变量File
	CString FileData2;//,file_temp;// 定义一个CString，作为一个缓冲区
	CString fileName_position(file_temp);//
	File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;

	int t=0;
	int nocheckout=0;
	char tmep_num=0;
	for(int m=0;m<30;m++,t++)//总共30个数
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10行8列
		{
			int j=x;
			{
				for(int p=0;p<10;p++)//10个数
				{
					for(int k=0;k<10;k++)//10行
						for(int l=0;l<8;l++)//8列
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
	if(nocheckout>5 ) //未找到匹配数字
		{
			degree[0]=-1;
			//break;
		}
}

//匹配gnum中的数字
int MatchNumber(int x, int y, BYTE* gray, int front_color, int back_color)
{
  int ret = -1;
  int min_different_rows = 1000;//不符合的行数
  int min_different_p = 0;
  for (int p = 0; p<10; p++)//10个数
  {
    int different_rows = 0;
    for (int k = 0; k < 10; k++)//10行
    {
      int dest_templet_have_front_color = 0;//同白颜色出现，去掉此次跳变，从字体的第一个边起算
      int dest_color_updown = 0;//dest跳变记录
      int templet_color_updown = 0;//templet跳变记录
      int dest_temp_color = back_color;//记录前一点
      int templet_temp_color = back_color;//记录前一点

      for (int l = 0; l < 8; l++)//8列

      {
        int templet = g_num[p * 80 + k * 8 + l];
        int dest = gray[(y + k + height_num)*g_width + (x + l + width_num)];
       
        if (front_color == dest)
        {
          dest_temp_color = front_color;
        }
        else if (dest_temp_color == front_color)//一次波动
        {
          dest_color_updown++;
          dest_temp_color = back_color;
        }

        if (front_color == templet)
        {
          templet_temp_color = front_color;
        }
        else if (templet_temp_color == front_color)//一次波动
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
          if (dest_temp_color == front_color)//一次波动
          {
            dest_color_updown++;
            dest_temp_color = back_color;
          }
          if (templet_temp_color == front_color)//一次波动
          {
            templet_color_updown++;
            templet_temp_color = back_color;
          }
        }

        if (1 == dest_color_updown && 1 == templet_color_updown)//两个一次跳变都结束
        {
          if (!dest_templet_have_front_color)//字体的一部分没有重合象素
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
        else if (1 < dest_color_updown || 1 < templet_color_updown)//跳变异常
        {
          different_rows++;
          break;
        }
        else if (7 == l && dest_color_updown != templet_color_updown)
        {
          different_rows++;
          break;
        }


      }//for (int l = 0; l < 8; l++)//8列
    }//for (int k = 0; k < 10; k++)//10行

    if (different_rows < min_different_rows)
    {
      min_different_rows = different_rows;
      min_different_p = p;
    }

  }//for (int p = 0; p<10; p++)//10个数
  if (min_different_rows<3)
  {
    ret = min_different_p;
  }

  return ret;
}
//20171102 mmy
//比较30个数，看目标在哪个模板中的字体区域中最重合,判断每行的趋势

void ScanDegree2(BYTE * lpGray, CString file_temp)
{
#define white 1
#define black 0
  CStdioFile File2;// 定义一个CStdioFile类变量File
  CString FileData2;//,file_temp;// 定义一个CString，作为一个缓冲区
  CString fileName_position(file_temp);//
  File2.Open(fileName_position, CFile::modeRead);
  int x, y;
  //CString str;
  int min_different_rows = 1000;//不符合的行数
  int min_different_p = 0;
  int t = 0;
  int nocheckout = 0;
  // int tmep_num = 0;

  for (int m = 0; m<30; m++, t++)//总共30个数
  {
    min_different_rows = 1000;
    File2.ReadString(FileData2);
    x = _wtoi(FileData2);
    File2.ReadString(FileData2);
    y = _wtoi(FileData2);
    //  tmep_num = 0;
    int i = y;//10行8列
    {
      int j = x;
      {
        min_different_p = MatchNumber(j, i, lpGray, white, black);
        //for (int p = 0; p<10; p++)//10个数
        //{
        //  int different_rows = 0;
        //  for (int k = 0; k < 10; k++)//10行
        //  {
        //    int dest_templet_have_white_color = 0;//同白颜色出现，去掉此次跳变，从字体的第一个边起算
        //    int dest_color_updown = 0;//dest跳变记录
        //    int templet_color_updown = 0;//templet跳变记录
        //    int dest_temp_color = black;//记录前一点
        //    int templet_temp_color = black;//记录前一点

        //    for (int l = 0; l < 8; l++)//8列

        //    {
        //      int templet = g_num[p * 80 + k * 8 + l];
        //      int dest = lpGray[(i + k/* + (height_num - 2)*/)*g_width + (j + l/* + (width_num - 46)*/)];
        //      /*if (lpGray[(i + k + (height_num - 2))*g_width + (j + l + (width_num - 46))] == g_num[p * 80 + k * 8 + l])
        //      tmep_num++;*/
        //      if (white == dest)
        //      {
        //        dest_temp_color = white;
        //      }
        //      else if (dest_temp_color == white)//一次波动
        //      {
        //        dest_color_updown++;
        //        dest_temp_color = black;
        //      }

        //      if (white == templet)
        //      {
        //        templet_temp_color = white;
        //      }
        //      else if (templet_temp_color == white)//一次波动
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
        //        if (dest_temp_color == white)//一次波动
        //        {
        //          dest_color_updown++;
        //          dest_temp_color = black;
        //        }
        //        if (templet_temp_color == white)//一次波动
        //        {
        //          templet_color_updown++;
        //          templet_temp_color = black;
        //        }
        //      }

        //      if (1 == dest_color_updown && 1 == templet_color_updown)//两个一次跳变都结束
        //      {
        //        if (!dest_templet_have_white_color)//字体的一部分没有重合象素
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
        //      else if (1 < dest_color_updown || 1 < templet_color_updown)//跳变异常
        //      {
        //        different_rows++;
        //        break;
        //      }
        //      else if (7 == l && dest_color_updown != templet_color_updown)
        //      {
        //        different_rows++;
        //        break;
        //      }


        //    }//for (int l = 0; l < 8; l++)//8列
        //  }//for (int k = 0; k < 10; k++)//10行

        //  if (different_rows < min_different_rows)
        //  {
        //    min_different_rows = different_rows;
        //    min_different_p = p;
        //  }
        //}//for (int p = 0; p<10; p++)//10个数

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
  if (nocheckout>5) //未找到匹配数字
  {
    degree[0] = -1;
    //break;
  }
#undef white
#undef black
}
void ScanDegreeEX(BYTE * lpGray,CString file_temp)
{
	CStdioFile File2;// 定义一个CStdioFile类变量File
	CString FileData2;//,file_temp;// 定义一个CString，作为一个缓冲区
	CString fileName_position(file_temp);//
	File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;

	int t=0;
	int nocheckout=0;
	char tmep_num=0;
	for(int m=0;m<30;m++,t++)//总共30个数
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10行8列
		{
			int j=x;
			{
				for(int p=0;p<10;p++)//10个数
				{
					for(int k=0;k<10;k++)//10行
						for(int l=0;l<8;l++)//8列
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
	if(nocheckout>5 ) //未找到匹配数字
	{
		degree[0]=-1;
		//break;
	}
}
void ScanDegreeDynamic2(BYTE * lpGray,CString file_temp)
{
	CStdioFile File2;// 定义一个CStdioFile类变量File
	CString FileData2;//,file_temp;// 定义一个CString，作为一个缓冲区
	CString fileName_position(file_temp);//
	BOOL open = File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;
	
	int t=0;
	char tmep_num=0;
	for(int m=0;m<6;m++,t++)//总共6个数
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10行8列
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
	CStdioFile File2;// 定义一个CStdioFile类变量File
	CString FileData2;//,file_temp;// 定义一个CString，作为一个缓冲区
	CString fileName_position(file_temp);//
	BOOL open = File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;

	int t=0;
	char tmep_num=0;
	for(int m=0;m<6;m++,t++)//总共6个数
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10行8列
		{
			int j=x;
			{
				for(int p=0;p<10;p++)//10个数
				{
					for(int k=0;k<10;k++)//10行
						for(int l=0;l<8;l++)//8列
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

		pDest.pdata = new TARGB32[800*600];//b,g,r,a  4个8位组成一个像素
		pDest.height = height;
		pDest.width  = width;
		pDest.byte_width = width << 2;

		g_height=height;
		g_width=width;
		lpGray=new BYTE[g_height*g_width];

		int iW=width, iH=height;
		//DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
		//SaveImageToFile(_T("screen_photo\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存采集图像

		DECODE_RGB_TO_GRAY((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);//转灰度:输入lpVoidBuffer，输出pDest

		//SaveImageToFile(_T("screen_photo\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存灰度图像
		DECODE_THRESHOLD(pDest,pDest,lpGray,m_VGAthreshold);//转二值化，pDest里存的是0-255的二值化图片，lpGray存的是0-1的二值化数据

		//SaveImageToFile(_T("screen_photo\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存二值化图像
		delete [] pDest.pdata;
		pDest.pdata=NULL;
		LoadMark(g_path + _T("\\check\\qz_42.txt"));

		FindOrigin(lpGray);
		if(height_num>100 || width_num>200|| (height_num==0&&width_num==0))
		{ 

			//AfxMessageBox(_T("没有找到图像标记,请重新设定阈值"));
		}
		else
			break;
	}
	g_num=new int[10*10*8];
	GetNumber();

	//////////////////////////////////////////////////
	CStdioFile File2;// 定义一个CStdioFile类变量File
	CString FileData2;//,file_temp;// 定义一个CString，作为一个缓冲区
	CString fileName_position(file_temp);//
	File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;
	int M[5]={0};
	int t=0;
	char tmep_num=0;
	for(int m=0;m<5;m++,t++)//总共5个数
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10行8列
		{
			int j=x;
			{
				for(int p=0;p<10;p++)//10个数
				{
					for(int k=0;k<10;k++)//10行
						for(int l=0;l<8;l++)//8列
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
	if(0!=M[0]*10000+M[1]*1000+M[2]*100+M[3]*10+M[4])//判断M值
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

		pDest.pdata = new TARGB32[800*600];//b,g,r,a  4个8位组成一个像素
		pDest.height = height;
		pDest.width  = width;
		pDest.byte_width = width << 2;

		g_height=height;
		g_width=width;
		lpGray=new BYTE[g_height*g_width];

		int iW=width, iH=height;
		//DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
		//SaveImageToFile(_T("screen_photo\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存采集图像

		DECODE_RGB_TO_GRAY((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);//转灰度:输入lpVoidBuffer，输出pDest

		//SaveImageToFile(_T("screen_photo\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存灰度图像
		DECODE_THRESHOLD(pDest,pDest,lpGray,m_VGAthreshold);//转二值化，pDest里存的是0-255的二值化图片，lpGray存的是0-1的二值化数据

		//SaveImageToFile(_T("screen_photo\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存二值化图像
		delete [] pDest.pdata;
		pDest.pdata=NULL;
		LoadMark(g_path + _T("\\check\\qz_42.txt"));

		FindOrigin(lpGray);
		if(height_num>100 || width_num>200|| (height_num==0&&width_num==0))
		{ 

			//AfxMessageBox(_T("没有找到图像标记,请重新设定阈值"));
		}
		else
			break;
	}
	g_num=new int[10*10*8];
	GetNumber();

	//////////////////////////////////////////////////
	CStdioFile File2;// 定义一个CStdioFile类变量File
	CString FileData2;//,file_temp;// 定义一个CString，作为一个缓冲区
	CString fileName_position(file_temp);//
	File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;
	int M[5]={0};
	int t=0;
	char tmep_num=0;
	for(int m=0;m<5;m++,t++)//总共5个数
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10行8列
		{
			int j=x;
			{
				for(int p=0;p<10;p++)//10个数
				{
					for(int k=0;k<10;k++)//10行
						for(int l=0;l<8;l++)//8列
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
	if(14742==M[0]*10000+M[1]*1000+M[2]*100+M[3]*10+M[4] || 14993==M[0]*10000+M[1]*1000+M[2]*100+M[3]*10+M[4])//判断M值
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
    (degree[0] * 10 + degree[1]) * 3600 + (degree[2] * 10 + degree[3]) * 60 + (degree[4] * 10 + degree[5]),//光轴
    (degree[6] * 10 + degree[7]) * 3600 + (degree[8] * 10 + degree[9]) * 60 + (degree[10] * 10 + degree[11]),//电轴
    (degree[12] * 10 + degree[13]) * 3600 + (degree[14] * 10 + degree[15]) * 60 + (degree[16] * 10 + degree[17]),//等效角
    (degree[18] * 10 + degree[19]) * 3600 + (degree[20] * 10 + degree[21]) * 60 + (degree[22] * 10 + degree[23]),//原始光轴
    (degree[24] * 10 + degree[25]) * 3600 + (degree[26] * 10 + degree[27]) * 60 + (degree[28] * 10 + degree[29]) 
  };//原始电轴；保存测定出的角度，转换成秒

  int t0 = (degree[0] * 10 + degree[1]) * 3600 + (degree[2] * 10 + degree[3]) * 60 + (degree[4] * 10 + degree[5]) + g_sort.R1t[g_sort.R1Num];//光轴//_0614
  int e0 = (degree[6] * 10 + degree[7]) * 3600 + (degree[8] * 10 + degree[9]) * 60 + (degree[10] * 10 + degree[11]) + g_sort.R1e[g_sort.R1Num];//电轴
  int equal_angle;
    
  // 等效角 = 原始光轴 + （原始电轴 - 等效角电轴参数）/等效角因子
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
// CDebug 对话框

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
			psamp->OnMyMessage_Sta1(wParam, lParam);//缓存里已经采集到一帧图像
			break;
		}
	case RGBWM_MODECHANGED:
		{
			psamp->OnMyMessage_modechange(wParam, lParam);//检测改变采集模式
			break;
		}
	case RGBWM_NOSIGNAL:
		{
			psamp->OnMyMessage_nosignal(wParam, lParam);//没信号
			break;
		}
	default:
		{
			break;
		}
	}
	//	::PostMessage(psamp->m_hWnd, message, wParam, lParam);	
}
//保存当前显示屏上所有的图形信息 add wygwl 2010-7-16
BOOL SaveImageToFile(CString sFile, int iW, int iH, BYTE * pInfo) 
{

#define WIDTHBYTES(bits) ((((bits) + 31) >> 5) << 2)
	int nBitCount = 32;
	int nLineBytes = WIDTHBYTES(iW * nBitCount);
	int dwDibSize = sizeof(BITMAPINFOHEADER) + nLineBytes*iH;//图像头和图像数据

	CString sPathName;
	unsigned char * pDib= NULL;
	{
		sPathName  = sFile;
		pDib = new unsigned char[dwDibSize];
		if (pDib == NULL) 
			return 0;
		memset(pDib, 0, dwDibSize);//给pDib指针的数据清零dwDibSize字节
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
		for ( int i = 0; i < iH; i++) // 上下反向
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
			file.Write(pDib, dwDibSize);//把pDib写入到文件中，长度为dwDibSize
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
// bitmap 位图CBitmap对象指针
// lpFileName 为位图文件名  
bool SaveBitmapToFile(CBitmap* bitmap, LPCWSTR lpFileName)
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

/******************************************************************************/
/*
*处理SDK发出的消息RGBWM_FRAMECAPTURED
*消息RGBWM_FRAMECAPTURED:表明当前采集卡缓存准备就绪，其中有一帧图象数据等待被处理
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

	//if ( g_bSaveImage ) // 保存图像到磁盘
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
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  检测当前VGA采集卡缓存是否就绪，检测失败（打开的设备队列中没有找到该硬件设备句柄）\n"));//检测失败（打开的设备队列中没有找到该硬件设备句柄）
				break;
			}

		case RGBERROR_INVALID_INDEX:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  检测当前VGA采集卡缓存是否就绪，没有找到设备索引\n"));//没有找到参数指针
				break;
			}

		case RGBERROR_BUFFER_TOO_SMALL:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  检测当前VGA采集卡缓存是否就绪，缓存太小\n"));//没有找到参数指针
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
*处理像素发生改变时SDK发出的消息RGBWM_MODECHANGED
*重新设置采集参数
*/
void CDebug::OnMyMessage_modechange(WPARAM wParam, LPARAM lParam)
{
	unsigned long *pbufferSize = 0L;	
	RGBCAPTUREPARMS CaptureParms;
	if (!SetCapture(m_hWnd, &CaptureParms, &hRGBCapture, pCaptureBuffer, &bufferSize, &RGBBitmap1))  //设置采集参数
	{
		AfxMessageBox(_T("SetCapture(),设置采集参数失败"));
		return;
	}
}
/*****************************************************************************/
/*
*设置采集参数
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
	/*检测当前输入的VGA视频信号分辨率*/
	error = RGBCaptureDetectVideoMode(*phRGBCapture, 0, 0, &pCaptureParms->VideoTimings, FALSE);
	if (RGBERROR_NO_ERROR != error)
	{
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureDetectVideoMode(), 检测当前输入的VGA视频信号分辨率，检测失败（打开的设备队列中没有找到该硬件设备句柄）\n"));//检测失败（打开的设备队列中没有找到该硬件设备句柄）
				break;
			}

		case RGBERROR_INVALID_POINTER:
			{
				AfxMessageBox(_T("RGBCaptureDetectVideoMode(), 检测当前输入的VGA视频信号分辨率，没有找到参数指针\n"));//没有找到参数指针
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
	/*设置窗口标题*/
	//char szWndTitle[120];
	//wsprintf(szWndTitle, "VGA采集卡： ");
	//::SetWindowText(::GetParent(this->m_hWnd), szWndTitle);

	//////////////////////////////////////////////////////////////////////////
	/* 设置位图参数为输出采集数据做准备 */

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
	//	AfxMessageBox(_T("CRGBSAMPDlg::SetCapture DDRelease不成功!!! \n"));
	//	//return FALSE;
	//}
	//if (DD_OK !=ddrawRGB.DDInit(hWnd,pCaptureParms->VideoTimings.HorAddrTime,pCaptureParms->VideoTimings.VerAddrTime))
	//{
	//	AfxMessageBox(_T("CRGBSAMPDlg::SetCapture DDInit画图初始化不成功!!! \n"));
	//	//return FALSE;
	//}	
	//SetEvent(m_hDDrawEvent);

	//////////////////////////////////////////////////////////////////////////
	/*此处检测缓存是否就绪，成功则继续采集事件*/
	error = RGBCaptureFrameBufferReady (hRGBCapture,
		pCaptureBuffer.Index,
		*pBufferSize);
	if (error != RGBERROR_NO_ERROR)
	{
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  检测当前VGA采集卡缓存是否就绪，检测失败（打开的设备队列中没有找到该硬件设备句柄）\n"));//检测失败（打开的设备队列中没有找到该硬件设备句柄）
				break;
			}

		case RGBERROR_INVALID_INDEX:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  检测当前VGA采集卡缓存是否就绪，没有找到设备索引\n"));//没有找到参数指针
				break;
			}

		case RGBERROR_BUFFER_TOO_SMALL:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  检测当前VGA采集卡缓存是否就绪，缓存太小\n"));//没有找到参数指针
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
			AfxMessageBox(_T("FreeBuffers(),释放缓存失败！"));
			return FALSE;
		}
		return FALSE;
	}
	return TRUE;
}
/*****************************************************************************/
/*
*处理当SDK采集不到正确的数据时发出的消息RGBWM_NOSIGNAL
*/
void CDebug::OnMyMessage_nosignal(WPARAM wParam, LPARAM lParam)
{
	CString strInfo;
	switch(wParam)
	{
	case NOSIGNAL_NO_RESOLUTION:
		{
			strInfo = _T("无 VGA 信 号(错误的分辨率 !!!)");
			break;
		}
	case NOSIGNAL_DDRCHECK_ERROR:
		{
			strInfo = _T("无 VGA 信 号(DDR 自检出错 !!!)");
			break;
		}
	case NOSIGNAL_H_OVERFLOW:
		{
			strInfo = _T("无 VGA 信 号(水平偏移量溢出 !!!)");
			break;
		}
	case NOSIGNAL_V_OVERFLOW:
		{
			strInfo = _T("无 VGA 信 号(垂直偏移量溢出 !!!)");
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
*释放缓存区
*/
BOOL CDebug::FreeBuffers(HRGBCAPTURE hRGBCapture, HWND hWnd, RGBCAPTUREBUFFER *pCaptureBuffer)
{
	RGBERROR error;
	HDC hDC = ::GetDC(hWnd);

	if (bUsingAccBuffer1 && (hRGBCapture == hRGBCapture))
	{
		bUsingAccBuffer1 = false;  //修改标志缓存是否使用的参数为false
	}


	::ReleaseDC(hWnd, hDC);

	if(pCaptureBuffer->LpVoidBuffer)
	{
		//////////////////////////////////////////////////////////////////////////
		/*删除当前的VGA采集缓存区在使用队列中的索引*/
		error = RGBCaptureReleaseBuffer(hRGBCapture, pCaptureBuffer->Index);
		if (RGBERROR_NO_ERROR != error)
		{
			switch(error)
			{
			case RGBERROR_INVALID_HRGBCAPTURE:
				{
					AfxMessageBox(_T("RGBCaptureReleaseBuffer(),   删除当前的VGA采集缓存区在使用队列中的索引，注册失败（打开的设备队列中没有找到该硬件设备句柄）\n"));//检测失败（打开的设备队列中没有找到该硬件设备句柄）
					break;
				}
			case RGBERROR_INVALID_POINTER:
				{
					AfxMessageBox(_T("RGBCaptureReleaseBuffer(),    删除当前的VGA采集缓存区在使用队列中的索引，没有找到指向当前缓存存储区的指针\n"));//没有找到指向当前缓存存储区的指针
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
		/*释放缓存区*/
		GlobalFree(pCaptureBuffer->LpVoidBuffer); 
		pCaptureBuffer->LpVoidBuffer = NULL;
	}

	//delete pCaptureBuffer;
	return TRUE;
}
/*****************************************************************************/ 
/*
*数出像素位数
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
*检测当前像素模式
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
*按照指定大小分配缓存
*/
BOOL CDebug::AllocateBuffers(HRGBCAPTURE hRGBCapture, 
	RGBCAPTUREBUFFER *pCaptureBuffer, 
	HWND hWnd, 
	UINT BufferSize)
{
	//////////////////////////////////////////////////////////////////////////
	/*分配缓存*/
	pCaptureBuffer->LpVoidBuffer = GlobalAlloc(GMEM_FIXED, BufferSize);
	if (pCaptureBuffer->LpVoidBuffer == NULL)
	{
		return FALSE;
	}
	AfxMessageBox(_T("%d"),GlobalSize(pCaptureBuffer->LpVoidBuffer));
	//////////////////////////////////////////////////////////////////////////
	/*注册当前的VGA采集事件*/
	RGBERROR error;
	error = RGBCaptureUseMemoryBuffer(hRGBCapture, pCaptureBuffer->LpVoidBuffer, BufferSize, &pCaptureBuffer->Index);
	if (RGBERROR_NO_ERROR != error)
	{
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureUseMemoryBuffer(),  注册当前的VGA采集事件，注册失败（打开的设备队列中没有找到该硬件设备句柄）\n"));//检测失败（打开的设备队列中没有找到该硬件设备句柄）
				break;
			}

		case RGBERROR_INVALID_POINTER:
			{
				AfxMessageBox(_T("RGBCaptureUseMemoryBuffer(),  注册当前的VGA采集事件，没有找到指向当前缓存存储区的指针\n"));//没有找到指向当前缓存存储区的指针
				break;
			}

		case RGBERROR_INVALID_INDEX:
			{
				AfxMessageBox(_T("RGBCaptureUseMemoryBuffer(),  注册当前的VGA采集事件，没有找到对应当前缓存存储区的索引\n"));//没有找到对应当前缓存存储区的索引
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
		bUsingAccBuffer1 = true;  //修改标志缓存使用的参数为TRUE
	}

	return TRUE;
}
// CDebug 消息处理程序
BOOL CDebug::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//debug全局变量初始化
	debugupdate=0;
	testStart=0;
	dynamicDetect=0;
	//itemnum相关
	int itemnum = g_sort.itemnum;
	CString itemstr = g_sort.itemstr;
	/*if (g_sort.islongsqu)
	{
	itemnum = 12;
	itemstr = _T("长方片分档步数");
	}
	else
	{
	itemnum = 24;
	itemstr = _T("分档步数");
	}*/
	//ini文件初始化或读取

	CString strFilePath,strValue;
	int iniint=0;
	GetModuleFileName(NULL,strFilePath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
	strFilePath.ReleaseBuffer(); 
	int pos = strFilePath.ReverseFind('\\'); 
	strFilePath = strFilePath.Left(pos)+_T("\\SortSet.ini"); 

	///////////////
	CFileFind findini;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用   
	BOOL ifFind = findini.FindFile(strFilePath);  
	if( ifFind )  
	{
		for(int i=0;i<itemnum+6;i++)
		{
			strValue.Format(_T("%d档"),i+1);
			if(3 == g_status._MOTOR_NUM)
				g_status.Zsort[i]=GetPrivateProfileInt(itemstr,strValue,0,strFilePath);  //写入全局变量
			else
				g_status.Ysort[i]=GetPrivateProfileInt(itemstr,strValue,0,strFilePath);  //写入全局变量

		}
		strValue.Format(_T("%d档"),itemnum+7);
		g_status.Xwait=GetPrivateProfileInt(itemstr,strValue,0,strFilePath);  //写入全局变量
		strValue.Format(_T("%d档"),itemnum+8);
		g_status.Xcheck=GetPrivateProfileInt(itemstr,strValue,0,strFilePath);  //写入全局变量
		strValue.Format(_T("%d档"),itemnum+9);
		g_status.Ywait=GetPrivateProfileInt(itemstr,strValue,0,strFilePath);  //写入全局变量
		strValue.Format(_T("%d档"),itemnum+10);
		g_status.Ypass=GetPrivateProfileInt(itemstr,strValue,0,strFilePath);  //写入全局变量

	}
	//组合框初始化
	m_motorturn.AddString(_T("正转"));
	m_motorturn.AddString(_T("反转"));
	m_motorturn.SetCurSel(0);
	//初始化Debug页listcontrol控件
	CRect listRect;

	m_debuglist.GetClientRect(&listRect);
	m_debuglist.SetExtendedStyle(m_debuglist.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_debuglist.InsertColumn(0, _T("档位"), LVCFMT_CENTER, listRect.Width()/8*2, 0);   
	m_debuglist.InsertColumn(1, _T("步数"), LVCFMT_CENTER, listRect.Width()/8*2, 1);   
	m_debuglist.InsertColumn(2, _T("说明"), LVCFMT_CENTER, listRect.Width()/8*4, 2);

	for(int i=0;i<itemnum+6;i++)
	{
		strValue="";
		strValue.Format(_T("%d"),i+1);//_0611_原i+1
		m_debuglist.InsertItem(i, strValue);   
		if(3 == g_status._MOTOR_NUM)
		{
			strValue.Format(_T("%d"),g_status.Zsort[i]);//度
		}
		else
		{
			strValue.Format(_T("%d"),g_status.Ysort[i]);//度
		}
		m_debuglist.SetItemText(i, 1, strValue);   
		strValue.Format(_T("第%02d档的电机步数"),i+1);//度//_0611_原i+1
		m_debuglist.SetItemText(i, 2, strValue);   
	}
	strValue.Format(_T("%d"),itemnum+7);m_debuglist.InsertItem(itemnum+6, strValue); strValue.Format(_T("%d"),g_status.Xwait);m_debuglist.SetItemText(itemnum+6, 1, strValue);m_debuglist.SetItemText(itemnum+6, 2, _T("X轴等待位置"));   
	strValue.Format(_T("%d"),itemnum+8);m_debuglist.InsertItem(itemnum+7, strValue); strValue.Format(_T("%d"),g_status.Xcheck);m_debuglist.SetItemText(itemnum+7, 1, strValue);m_debuglist.SetItemText(itemnum+7, 2, _T("X轴放片位置"));   
	strValue.Format(_T("%d"),itemnum+9);m_debuglist.InsertItem(itemnum+8, strValue); strValue.Format(_T("%d"),g_status.Ywait);m_debuglist.SetItemText(itemnum+8, 1, strValue);m_debuglist.SetItemText(itemnum+8, 2, _T("Y轴等待位置"));   
	if(3 == g_status._MOTOR_NUM)
	{strValue.Format(_T("%d"),itemnum+10);m_debuglist.InsertItem(itemnum+9, strValue); strValue.Format(_T("%d"),g_status.Ypass);m_debuglist.SetItemText(itemnum+9, 1, strValue);m_debuglist.SetItemText(itemnum+9, 2, _T("Y轴放片位置"));   
	}


	// TODO:  初始化
	signalflag = FALSE;
	//pData = new LPVOID[1600*1200*4];
	datamode = 2;
	bufferSize = 0L;         //第1个采集卡对应的缓存大小
	m_VGAthreshold=110;
	SetDlgItemInt(IDC_EDIT1,m_VGAthreshold,0);
	/*EnableToolTips(TRUE);   //true为打开提示功能，false 为关闭提示功能
	m_tooltip.Create(this);//创建
	m_tooltip.SetTipTextColor(RGB(0,0,255));//设定文字颜色
	m_tooltip.SetDelayTime(150);//设定文字停留时间
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_LOAD),"载入图片后，进行实时检测。");
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_COMPARE),"一载入图片，点击后开始实时检测。");
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_STOP_VGA),"停止采集，需要重新初始化vga后才能再次采集。");
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_TEST),"测试编辑框内的图片文件，可更改测试图片重新点击测试。");
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_BMP_PATH),"测试图片路径");
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_VGA_THRASHOLD),"检测图片时使用的阈值");*/

	// TODO: VGA初始化
	RGBERROR error;
	m_iIndex=0;
	hRGBCapture = 0;
	BufferSize=1024*768*3;
	RGBCAPTUREPARMS CaptureParms;  //存放当前采集卡的参数
	error = RGBCaptureInitialise(&m_ulNumofcard);//初始化
	if (RGBERROR_NO_ERROR != error)
	{
		//初始化失败，返回值仅为：RGBERROR_HARDWARE_NOT_FOUND
		if (RGBERROR_HARDWARE_NOT_FOUND == error)
		{
			//没有找到硬件设备,打印日志
			AfxMessageBox(_T("RGBCaptureInitialise() 初始化，没有找到硬件设备\n"));
			//break;
			//return(FALSE);
		}
		return 0;
	}
	//打开采集卡
	error = RGBCaptureOpen(m_iIndex,&hRGBCapture);
	if (RGBERROR_NO_ERROR != error)     //打开设备失败，返回结果如下：
	{	
		switch (error)
		{
		case RGBERROR_HARDWARE_NOT_FOUND :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，没有找到硬件设备\n"));//没有找到设备
				break;
			}
		case RGBERROR_INVALID_POINTER :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，没有找到指向该硬件设备的指针\n"));//没有找到设备
				break;
			}
		case RGBERROR_INVALID_INDEX :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，没有找到硬件设备的索引\n"));//没有找到设备
				break;
			}
		case RGBERROR_DEVICE_IN_USE :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，硬件设备正在被使用\n"));//硬件设备正在被使用
				break;
			}
		case RGBERROR_UNABLE_TO_LOAD_DRIVER :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，不能加载驱动\n"));//软硬件不批配
				break;
			}
		case RGBERROR_INVALID_DEVICE :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，硬件不存在\n"));//软硬件不批配
				break;
			}
		default:
			{
				break;
			}
		}
		return 0;
	}
	pCaptureParms.Input=0;//VGA输入
	pCaptureParms.datamode=2;//0,uyvy,2,RGB格式
	pCaptureParms.BuffersNumber=0;// 高帧率
	pCaptureParms.graytest=0;
	pCaptureParms.FramespSec=50;// 50帧
	pCaptureParms.Size=sizeof(pCaptureParms);// 结构长度
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
		| RGBCAPTURE_PARM_VDIF_DESCRIPTION;    //设置的参数有：当前像素模式，主窗口句柄，通知消息模式
	//pixelFormat = DetectPixelFormat(m_hWnd,(DWORD *)&pRGBBitmap->colorMark);
	//pCaptureParms.PixelFormat = pixelFormat;
	//pCaptureParms.PixelFormat=RGBPIXELFORMAT_888;
	pCaptureParms.NotifyArg=this;
	pCaptureParms.NotifyFn=callbackNotify;
	pCaptureParms.NotifyFlags = RGBNOTIFY_NO_SIGNAL | RGBNOTIFY_MODE_CHANGE ;
	//pCaptureParms.HWnd=m_hWnd;
	//配置参数
	error = RGBCaptureSetParameters(hRGBCapture, &pCaptureParms,RGBCAPTURE_FLAG_CURRENT|RGBCAPTURE_FLAG_REAL_WORLD);
	if (RGBERROR_NO_ERROR != error)
	{
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，没有找到硬件设备\n"));//没有找到硬件设备
				break;
			}

		case RGBERROR_INVALID_POINTER:
			{  
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，没有找到参数指针\n"));//没有找到参数指针
				break;
			}

		case RGBERROR_INVALID_SIZE:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，参数指针大小错误\n"));//参数指针大小错误
				break;
			}

		case RGBERROR_INVALID_FLAGS:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的标志位操作\n"));//不支持的标志位操作
				break;
			}

		case RGBERROR_INVALID_FORMAT:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的输入信号格式\n"));//不支持的输入信号格式
				break;
			}

		case RGBERROR_INVALID_BLACKLEVEL:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的黑色度\n"));//不支持的黑色度
				break;
			}

		case RGBERROR_INVALID_PIXEL_FORMAT:
			{   
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的像素模式\n"));//不支持的像素模式
				break;
			}

		case RGBERROR_INVALID_HWND:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不是当前窗口句柄\n"));//不是当前窗口句柄
				break;
			}

		case RGBERROR_INVALID_SYNCEDGE:
			{   
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的同步沿\n"));//不支持的同步沿
				break;
			}

		case RGBERROR_HSCALED_OUT_OF_RANGE:
			{  
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的缩放比例\n"));//不支持的缩放比例
				break;
			}

		default:
			{
				break;
			}
		}
		return 0;
	}
	/*开始采集事件*/
	error = RGBCaptureEnable(hRGBCapture, TRUE);
	if (RGBERROR_NO_ERROR != error)     //打开设备失败，返回结果如下：
	{	
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() 启动采集卡，打开队列中没有找到该硬件设备句柄\n"));//打开队列中没有找到该硬件设备句柄
				break;
			}

		case RGBERROR_CAPTURE_OUTSTANDING:
			{
				AfxMessageBox(_T("RGBCaptureEnable() 启动采集卡，硬件设备没有打开\n"));//硬件设备没有打开
				break;
			}

		case RGBERROR_THREAD_FAILURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() 启动采集卡，启动线程失败\n"));//启动线程失败
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
	/*设置采集参数*/
	if (!SetCapture(m_hWnd, &CaptureParms, &hRGBCapture, pCaptureBuffer, &bufferSize, &RGBBitmap1)) 
	{
		AfxMessageBox(_T("SetCapture(),设置采集参数失败"));
		return 0;
	}//初始化完毕
	UpdateData(1);//获得阈值
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDebug::OnBnClickedMotor1Run()
{
	// TODO: 在此添加控件通知处理程序代码
	if(g_hDevice==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("ac6641 打开失败"));
		return;
	}
	int tmp=GetDlgItemInt(IDC_MOTOR_NUM,0,1);//读入转数
	if(tmp>65535||tmp<0)
	{
		AfxMessageBox(_T("输入转数不正确或超出范围"));
		return;
	}
	GetDlgItem(IDC_MOTOR1_RUN)->EnableWindow(0);
	int ioData=tmp%256;
	AC6641_DO(g_hDevice,0,ioData);//发送定时低位
	//AC6641_DO(g_hDevice,2,ioData);//发送Y定时低位
	ioData=tmp/256,
		AC6641_DO(g_hDevice,1,ioData);//发送定时高位
	//AC6641_DO(g_hDevice,3,ioData);//发送定时高位
	ioData=AC6641_DI(g_hDevice,5) & 0xfb;//52低电平，全电流
	AC6641_DO(g_hDevice,5,ioData);
	ioData=AC6641_DI(g_hDevice,4) | 0x02;//将p41置高，为下降沿做准备。
	//ioData=AC6641_DI(g_hDevice,4) | 0x20;//将p41置高，为下降沿做准备。
	tmp=m_motorturn.GetCurSel();//获得正反转
	0==tmp?(ioData=ioData & 0xfe):(ioData=ioData | 0x01);
	//0==tmp?(ioData=ioData & 0xef):(ioData=ioData | 0x10);
	AC6641_DO(g_hDevice,4,ioData);//正反转
	Sleep(1);
	ioData=ioData & 0xfd;
	//ioData=ioData & 0xdf;
	AC6641_DO(g_hDevice,4,ioData);//下降沿
	Sleep(1);
	ioData=ioData | 0x02;
	//ioData=ioData | 0x20;
	AC6641_DO(g_hDevice,4,ioData);//
	for(int i = 0; i<1000; i++)
	{
		if(!(AC6641_DI(g_hDevice,6) & 0x01))
			break;//当高电平时
		if(i == 998)
		{
			AfxMessageBox(_T("error"));
			break;
		}
		Sleep(1);
	}
	//while(AC6641_DI(g_hDevice,6) & 0x10);//当高电平时
	ioData=AC6641_DI(g_hDevice,5) | 0x04;//52高电平，半电流
	AC6641_DO(g_hDevice,5,ioData);
	GetDlgItem(IDC_MOTOR1_RUN)->EnableWindow(1);
}




void CDebug::OnBnClickedAcRead()
{
	// TODO: 在此添加控件通知处理程序代码
	AC6641_SetIoMode(g_hDevice,0x0,0x0);//设模式，对应7-0和11-8组,=0为输入
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
	// TODO: 在此添加控件通知处理程序代码
	AC6641_SetIoMode(g_hDevice,0xff,0xf);//设模式，对应7-0和11-8组,=1为输出
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
  CBitmap m_bmp;//创建类成员

  BITMAP bm;//存放位图信息的结构
  CString bmp_path = path;
  HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL, bmp_path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);//创建bitmap指针
                                                                                             //m_bmp.LoadBitmap(bmp_path);
  if (hBitmap1 == NULL)
  {
    DeleteObject(hBitmap1);//记得删除
    return;
  }
  m_bmp.Attach(hBitmap1);//关联句柄和cbitmap关联
  m_bmp.GetBitmap(&bm);//

  g_height = bm.bmHeight;//图像的宽高    
  g_width = bm.bmWidth;
  g_widthBytes = bm.bmWidthBytes;//图像一行数据所占的字节数
  int size = g_height*g_widthBytes;
  BYTE *lpBits = new BYTE[size];
  m_bmp.GetBitmapBits(size, lpBits);//得到RGB数据

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
      // lpGray[i*g_width+j]=(R*0.3+G*0.59+B*0.11)>m_VGAthreshold ? 1 : 0;///(B+G+R)/3;//转灰度
      g_lpGray[i*g_width + j] = ((B + G + R) / 3)>m_VGAthreshold ? 1 : 0;///(B+G+R)/3;//转灰度
    }

  if (pWnd)
  {
    CDC* pDC = pWnd->GetDC();
    CRect rect;
    pWnd->GetClientRect(&rect);

    CDC memDC;        //定义一个设备
    memDC.CreateCompatibleDC(pDC);
    memDC.SelectObject(m_bmp);  //为设备选择对象//因下面需要使用到m_bmp.GetBitmapBits，这里不能选入，不然会拿不出数据
    pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
    
    memDC.DeleteDC();

    pWnd->ReleaseDC(pDC);
  }

  DeleteObject(hBitmap1);//记得删除	
}


//调入本地图像，经过灰度转换，二值化转换，然后进行像素匹配，识别数字
void CDebug::OnBnClickedScrread()
{
	// TODO: 在此添加控件通知处理程序代码
	CBitmap m_bmp;//创建类成员

	BITMAP bm;//存放位图信息的结构
	CString bmp_path =g_path + _T("\\screen_photo\\yuanshi.bmp");//_T("d://tmep1.bmp")
	HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,bmp_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//创建bitmap指针
	//m_bmp.LoadBitmap(bmp_path);
	if (hBitmap1 == NULL)
	{
		DeleteObject(hBitmap1);//记得删除
		return;
	}
	m_bmp.Attach(hBitmap1);//关联句柄和cbitmap关联
	m_bmp.GetBitmap(&bm);//


	//
	g_height = bm.bmHeight;//图像的宽高    
	g_width = bm.bmWidth;
	g_widthBytes=bm.bmWidthBytes;//图像一行数据所占的字节数
	int size=g_height*g_widthBytes; 
	BYTE *lpBits=new BYTE[size]; 
	m_bmp.GetBitmapBits(size,lpBits);//得到RGB数据
	BYTE *lpGray=new BYTE[g_height*g_width];
	int R, G, B;
	for(int i=0;i<g_height;i++)
		for(int j=0;j<g_width;j++)
		{
		  B=lpBits[i*g_widthBytes+j*4];
		  G=lpBits[i*g_widthBytes+j*4+1];
		  R=lpBits[i*g_widthBytes+j*4+2];
		 // lpGray[i*g_width+j]=(R*0.3+G*0.59+B*0.11)>m_VGAthreshold ? 1 : 0;///(B+G+R)/3;//转灰度
          lpGray[i*g_width + j] = ((B + G + R) / 3)>m_VGAthreshold ? 1 : 0;///(B+G+R)/3;//转灰度
		}
	//for(int i=0;i<g_height;i++)
	//	for(int j=0;j<g_width;j++)
	//	{
	//		if(lpGray[i*g_width+j]> m_VGAthreshold )//二值化
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
			ScanDegree2(lpGray,g_path + _T("\\check\\position.txt"));//获得degree[]

      CalcEquAngle(); // DATE:180421

			CString _degree,_tmp;
			_degree.Format(_T("光轴角度：%d\r\n电轴角度：%d\r\n等效角度：%d\r\n原始光轴：%d\r\n原始电轴：%d"),degree[0]*100000+degree[1]*10000+degree[2]*1000+degree[3]*100+degree[4]*10+degree[5],
				degree[6]*100000+degree[7]*10000+degree[8]*1000+degree[9]*100+degree[10]*10+degree[11],
				degree[12]*100000+degree[13]*10000+degree[14]*1000+degree[15]*100+degree[16]*10+degree[17],
				degree[18]*100000+degree[19]*10000+degree[20]*1000+degree[21]*100+degree[22]*10+degree[23],
				degree[24]*100000+degree[25]*10000+degree[26]*1000+degree[27]*100+degree[28]*10+degree[29]);

			ScanDegreeDynamic2(lpGray,g_path + _T("\\check\\positionD1.txt"));//获得degree[]
			_tmp.Format(_T("\r\nD1:%.3f"),degree[0]*100+degree[1]*10+degree[2]+degree[3]*0.1+degree[4]*0.01+degree[5]*0.001);
			_degree +=_tmp;
			ScanDegreeDynamic2(lpGray,g_path + _T("\\check\\positionD2.txt"));//获得degree[]
			_tmp.Format(_T("\r\nD2:%.3f"),degree[0]*100+degree[1]*10+degree[2]+degree[3]*0.1+degree[4]*0.01+degree[5]*0.001);
			_degree +=_tmp;
			ScanDegreeDynamic2(lpGray,g_path + _T("\\check\\positionDM.txt"));//获得degree[]
			_tmp.Format(_T("\r\nDM:%.3f"),degree[0]*100+degree[1]*10+degree[2]+degree[3]*0.1+degree[4]*0.01+degree[5]*0.001);
			_degree +=_tmp;
			ScanDegreeDynamic2(lpGray,g_path + _T("\\check\\positionR1.txt"));//获得degree[]
			_tmp.Format(_T("\r\nR1:%.3f"),degree[0]*100+degree[1]*10+degree[2]+degree[3]*0.1+degree[4]*0.01+degree[5]*0.001);
			_degree +=_tmp;
			ScanDegreeDynamic2(lpGray,g_path + _T("\\check\\positionAL.txt"));//获得degree[]
			_tmp.Format(_T("\r\nAL:%.3f"),degree[0]*100+degree[1]*10+degree[2]+degree[3]*0.1+degree[4]*0.01+degree[5]*0.001);
			_degree +=_tmp;

			//
			CWnd * pWnd = GetDlgItem(IDC_PREVIEW);
			CDC* pDC = pWnd->GetDC();
			CRect rect;
			GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
			//

			CDC memDC;        //定义一个设备
			CClientDC dc(this);      //获取客户
			memDC.CreateCompatibleDC( &dc );
			memDC.SelectObject( m_bmp );  //为设备选择对象//因下面需要使用到m_bmp.GetBitmapBits，这里不能选入，不然会拿不出数据
			pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			DeleteObject(hBitmap1);//记得删除	
			memDC.DeleteDC();	

			pWnd->ReleaseDC(pDC);


			GetDlgItem(IDC_SHOW_DEG)->SetWindowText(_degree);
			UpdateData(0);
			DeleteObject(hBitmap1);//记得删除	
			delete []lpBits;
			delete []lpGray;
			delete []g_num;
			g_num=NULL;
			lpBits=NULL;
			lpGray=NULL;

}


void CDebug::OnBnClickedVgaInit()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: VGA初始化
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
	RGBCAPTUREPARMS CaptureParms;  //存放当前采集卡的参数
	error = RGBCaptureInitialise(&m_ulNumofcard);//初始化
	if (RGBERROR_NO_ERROR != error)
	{
		//初始化失败，返回值仅为：RGBERROR_HARDWARE_NOT_FOUND
		if (RGBERROR_HARDWARE_NOT_FOUND == error)
		{
			//没有找到硬件设备,打印日志
			AfxMessageBox(_T("RGBCaptureInitialise() 初始化，没有找到硬件设备\n"));
			//break;
			//return(FALSE);
		}
		return;
	}
	//打开采集卡
	error = RGBCaptureOpen(m_iIndex,&hRGBCapture);
	if (RGBERROR_NO_ERROR != error)     //打开设备失败，返回结果如下：
	{	
		switch (error)
		{
		case RGBERROR_HARDWARE_NOT_FOUND :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，没有找到硬件设备\n"));//没有找到设备
				break;
			}
		case RGBERROR_INVALID_POINTER :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，没有找到指向该硬件设备的指针\n"));//没有找到设备
				break;
			}
		case RGBERROR_INVALID_INDEX :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，没有找到硬件设备的索引\n"));//没有找到设备
				break;
			}
		case RGBERROR_DEVICE_IN_USE :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，硬件设备正在被使用\n"));//硬件设备正在被使用
				break;
			}
		case RGBERROR_UNABLE_TO_LOAD_DRIVER :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，不能加载驱动\n"));//软硬件不批配
				break;
			}
		case RGBERROR_INVALID_DEVICE :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，硬件不存在\n"));//软硬件不批配
				break;
			}
		default:
			{
				break;
			}
		}
		return;
	}
	pCaptureParms.Input=0;//VGA输入
	pCaptureParms.datamode=2;//0,uyvy,2,RGB格式
	pCaptureParms.BuffersNumber=0;// 高帧率
	pCaptureParms.graytest=0;
	pCaptureParms.FramespSec=50;// 50帧
	pCaptureParms.Size=sizeof(pCaptureParms);// 结构长度
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
		| RGBCAPTURE_PARM_VDIF_DESCRIPTION;    //设置的参数有：当前像素模式，主窗口句柄，通知消息模式
	//pixelFormat = DetectPixelFormat(m_hWnd,(DWORD *)&pRGBBitmap->colorMark);
	//pCaptureParms.PixelFormat = pixelFormat;
	//pCaptureParms.PixelFormat=RGBPIXELFORMAT_888;
	pCaptureParms.NotifyArg=this;
	pCaptureParms.NotifyFn=callbackNotify;
	pCaptureParms.NotifyFlags = RGBNOTIFY_NO_SIGNAL | RGBNOTIFY_MODE_CHANGE ;
	//pCaptureParms.HWnd=m_hWnd;
	//配置参数
	error = RGBCaptureSetParameters(hRGBCapture, &pCaptureParms,RGBCAPTURE_FLAG_CURRENT|RGBCAPTURE_FLAG_REAL_WORLD);
	if (RGBERROR_NO_ERROR != error)
	{
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，没有找到硬件设备\n"));//没有找到硬件设备
				break;
			}

		case RGBERROR_INVALID_POINTER:
			{  
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，没有找到参数指针\n"));//没有找到参数指针
				break;
			}

		case RGBERROR_INVALID_SIZE:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，参数指针大小错误\n"));//参数指针大小错误
				break;
			}

		case RGBERROR_INVALID_FLAGS:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的标志位操作\n"));//不支持的标志位操作
				break;
			}

		case RGBERROR_INVALID_FORMAT:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的输入信号格式\n"));//不支持的输入信号格式
				break;
			}

		case RGBERROR_INVALID_BLACKLEVEL:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的黑色度\n"));//不支持的黑色度
				break;
			}

		case RGBERROR_INVALID_PIXEL_FORMAT:
			{   
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的像素模式\n"));//不支持的像素模式
				break;
			}

		case RGBERROR_INVALID_HWND:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不是当前窗口句柄\n"));//不是当前窗口句柄
				break;
			}

		case RGBERROR_INVALID_SYNCEDGE:
			{   
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的同步沿\n"));//不支持的同步沿
				break;
			}

		case RGBERROR_HSCALED_OUT_OF_RANGE:
			{  
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的缩放比例\n"));//不支持的缩放比例
				break;
			}

		default:
			{
				break;
			}
		}
		return;
	}
	/*开始采集事件*/
	error = RGBCaptureEnable(hRGBCapture, TRUE);
	if (RGBERROR_NO_ERROR != error)     //打开设备失败，返回结果如下：
	{	
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() 启动采集卡，打开队列中没有找到该硬件设备句柄\n"));//打开队列中没有找到该硬件设备句柄
				break;
			}

		case RGBERROR_CAPTURE_OUTSTANDING:
			{
				AfxMessageBox(_T("RGBCaptureEnable() 启动采集卡，硬件设备没有打开\n"));//硬件设备没有打开
				break;
			}

		case RGBERROR_THREAD_FAILURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() 启动采集卡，启动线程失败\n"));//启动线程失败
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
	/*设置采集参数*/
	if (!SetCapture(m_hWnd, &CaptureParms, &hRGBCapture, pCaptureBuffer, &bufferSize, &RGBBitmap1)) 
	{
		AfxMessageBox(_T("SetCapture(),设置采集参数失败"));
		return;
	}
	//初始化完毕
}


void CDebug::OnBnClickedLoadPic()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 载入图片
	/////////////
	//GetDlgItem(IDC_BUTTON_VGAINITIALISE)->EnableWindow(0);
	GetDlgItem(IDC_LOAD_PIC)->EnableWindow(1);
	GetDlgItem(IDC_COMPARE)->EnableWindow(1);
	//GetDlgItem(IDC_BUTTON_STOP_VGA)->EnableWindow(1);
	/////////////
	UpdateData(1);//获得阈值
	//char * readPath;//
	//int bmpWidth,bmpHeight,biBitCount;
	//unsigned char * pBmpBuf=NULL;
	//readPath=bmp_path.GetBuffer(bmp_path.GetLength());//新获取其内部字符缓冲区的指针
	//int suc=readBmp(readPath,pBmpBuf,bmpWidth,bmpHeight,biBitCount); 

	DWORD width = RGBBitmap1.bmiHeader.biWidth;
	DWORD height = -RGBBitmap1.bmiHeader.biHeight;

	pDest.pdata = new TARGB32[800*600];//b,g,r,a  4个8位组成一个像素
	pDest.height = height;
	pDest.width  = width;
	pDest.byte_width = width << 2;

	g_height=height;
	g_width=width;
	g_lpGray=new BYTE[g_height*g_width];

	int iW=width, iH=height;
	DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
	SaveImageToFile(g_path + _T("\\screen_photo\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存采集图像

	DECODE_RGB_TO_GRAY((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);//转灰度:输入lpVoidBuffer，输出pDest

	SaveImageToFile(g_path + _T("\\screen_photo\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存灰度图像
	DECODE_THRESHOLD(pDest,pDest,g_lpGray,m_VGAthreshold);//转二值化，pDest里存的是0-255的二值化图片，lpGray存的是0-1的二值化数据
	DECODE_THRESHOLD2(pDest,pDest,m_VGAthreshold);//像素对换
	SaveImageToFile(g_path + _T("\\screen_photo\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存二值化图像


	delete [] pDest.pdata;
	pDest.pdata=NULL;
	///////////////////////
	CBitmap m_bmp;//创建类成员
	BITMAP bm;//存放位图信息的结构
	CString bmp_path =g_path + _T("\\screen_photo\\erzhihu.bmp");//_T("d://tmep1.bmp")
	HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,bmp_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//创建bitmap指针
	if (hBitmap1 == NULL)
	{
		DeleteObject(hBitmap1);//记得删除
		return;
	}
	m_bmp.Attach(hBitmap1);//关联句柄和cbitmap关联
	m_bmp.GetBitmap(&bm);
	//m_bmp.LoadBitmapA(bmp_path);

	CWnd * pWnd = GetDlgItem(IDC_PREVIEW);
	CDC* pDC = pWnd->GetDC();
	CRect rect;
	GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
	//

	CDC memDC;        //定义一个设备
	CClientDC dc(this);      //获取客户
	memDC.CreateCompatibleDC( &dc );
	memDC.SelectObject( m_bmp );  //为设备选择对象
	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	DeleteObject(hBitmap1);//记得删除	
	memDC.DeleteDC();	

	pWnd->ReleaseDC(pDC);
	//pDC->DeleteDC();
	//GetDlgItem(IDC_PRINT)->EnableWindow(1);
}


void CDebug::OnBnClickedCompare()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 比较 
	/*
	话说检测屏幕的时候得到的画面有的异样，用原始qz_42可能定位不了，所以可以调试修改相关数据，
	具体模型可以观察保存下来的erzhihua.bmp图片
	*/
	LoadMark(g_path + _T("\\check\\qz_42.txt"));
	/*
	找到图片的标记起点
	*/
	FindOrigin2(g_lpGray);
	if(height_num>100 || width_num>100|| (height_num<0&&width_num<0))
	{ 
		AfxMessageBox(_T("没有找到图像标记,请重新设定阈值"));
		delete []g_lpGray;
		g_lpGray=NULL;
		GetDlgItem(IDC_COMPARE)->EnableWindow(0);
		return;
	}
	g_num=new int[10*10*8];
	GetNumber();
	ScanDegree2(g_lpGray,g_path + _T("\\check\\position.txt"));//获得degree[]
	CString _degree;
	_degree.Format(_T("光轴角度：%d\r\n电轴角度：%d\r\n等效角度：%d\r\n原始光轴：%d\r\n原始电轴：%d"),degree[0]*100000+degree[1]*10000+degree[2]*1000+degree[3]*100+degree[4]*10+degree[5],
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
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 停止采集
	/////////////
	GetDlgItem(IDC_VGA_INIT)->EnableWindow(1);
	GetDlgItem(IDC_LOAD_PIC)->EnableWindow(0);
	GetDlgItem(IDC_COMPARE)->EnableWindow(0);
	GetDlgItem(IDC_STOP_VGA)->EnableWindow(0);
	/////////////
	//stopVGA(m_hWnd, &hRGBCapture1, pCaptureBuffer1);
	RGBERROR error;	
	error = RGBCaptureEnable(hRGBCapture, FALSE);
	if (RGBERROR_NO_ERROR != error)     //打开设备失败，返回结果如下：
	{	
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() 启动采集卡，打开队列中没有找到该硬件设备句柄\n"));//打开队列中没有找到该硬件设备句柄
				break;
			}

		case RGBERROR_CAPTURE_OUTSTANDING:
			{
				AfxMessageBox(_T("RGBCaptureEnable() 启动采集卡，硬件设备没有打开\n"));//硬件设备没有打开
				break;
			}

		case RGBERROR_THREAD_FAILURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() 启动采集卡，启动线程失败\n"));//启动线程失败
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
	/*释放缓存*/
	if (NULL != &pCaptureBuffer)
	{
		if (!FreeBuffers(hRGBCapture, m_hWnd, &pCaptureBuffer))
		{
			AfxMessageBox(_T("FreeBuffers(),释放缓存失败！"));
			return;
		}
	}
	/*关闭采集卡*/
	RGBCaptureClose(hRGBCapture);
	hRGBCapture = 0;
}


void CDebug::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	listsortsel=pNMLV->iItem;
	*pResult = 0;
}


void CDebug::OnBnClickedSortMove()
{
	// TODO: 在此添加控件通知处理程序代码
	ReturnZero(1);//归零
	ReturnZero(2);//归零
	if(3 == g_status._MOTOR_NUM)
		ReturnZero(3);//归零
	Sleep(500);
	if(3 == g_status._MOTOR_NUM)
	{
		if(listsortsel<g_sort.itemnum + 6)//Y
			MotorCtrl(3,1,g_status.Zsort[listsortsel]);//转动到档位
		else if(listsortsel==g_sort.itemnum+8)//Y
			MotorCtrl(2,1,g_status.Ywait);//移动到等待位置
		else if(listsortsel==g_sort.itemnum+9)//Y
			MotorCtrl(2,1,g_status.Ypass);//移动到放片位置
		else if(listsortsel==g_sort.itemnum+6)//X
		{
			MotorCtrl(2,1,g_status.Ywait);//移动到等待位置
			MotorCtrl(1,1,g_status.Xwait);//移动到等待上片位置
		}
		else if(listsortsel==g_sort.itemnum+7)//X
		{
			MotorCtrl(2,1,g_status.Ywait);//移动到等待位置
			MotorCtrl(1,1,g_status.Xcheck);//移动到上片位置
		}
	}
	else
	{
		if(listsortsel<g_sort.itemnum + 6)//Y
			MotorCtrl(2,1,g_status.Ysort[listsortsel]);//转动到档位
		else if(listsortsel==g_sort.itemnum+8)//Y
			MotorCtrl(2,1,g_status.Ywait);//移动到等待位置
		else if(listsortsel==g_sort.itemnum+6)//X
		{
			MotorCtrl(2,1,g_status.Ywait);//移动到等待位置
			MotorCtrl(1,1,g_status.Xwait);//移动到等待上片位置
		}
		else if(listsortsel==g_sort.itemnum+7)//X
		{
			MotorCtrl(2,1,g_status.Ywait);//移动到等待位置
			MotorCtrl(1,1,g_status.Xcheck);//移动到上片位置
		}
	}
}


void CDebug::OnBnClickedSortTest()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned int plusenum = GetDlgItemInt(IDC_PLUSE_NUM,0,0);
	//ReturnZero(1);//归零
	//ReturnZero(2);//归零
	if(3 == g_status._MOTOR_NUM)
	{
		if(listsortsel<g_sort.itemnum+6)//Z
		{
			ReturnZero(3);//归零
			Sleep(500);
			MotorCtrl(3,1,plusenum);//转动到档位
		}
		else if(listsortsel==g_sort.itemnum+8)//Y
		{
			ReturnZero(2);//归零
			Sleep(500);
			MotorCtrl(2,1,plusenum);//移动到等待位置
		}
		else if(listsortsel==g_sort.itemnum+9)//Y
		{
			ReturnZero(2);//归零
			Sleep(500);
			MotorCtrl(2,1,plusenum);//移动到放片位置
		}
		else if(listsortsel==g_sort.itemnum+6)//X
		{
			ReturnZero(1);//归零
			Sleep(500);
			MotorCtrl(1,1,plusenum);//移动到等待上片位置
		}
		else if(listsortsel==g_sort.itemnum+7)//X
		{
			ReturnZero(1);//归零
			Sleep(500);
			MotorCtrl(1,1,plusenum);//移动到上片位置
		}
	}
	else
	{
		if(listsortsel<g_sort.itemnum+6)//Y
		{
			ReturnZero(2);//归零
			Sleep(500);
			MotorCtrl(2,1,plusenum);//转动到档位
		}
		else if(listsortsel==g_sort.itemnum+8)//Y
		{
			ReturnZero(2);//归零
			Sleep(500);
			MotorCtrl(2,1,plusenum);//移动到等待位置
		}
		else if(listsortsel==g_sort.itemnum+6)//X
		{
			ReturnZero(1);//归零
			Sleep(500);
			MotorCtrl(1,1,plusenum);//移动到等待上片位置
		}
		else if(listsortsel==g_sort.itemnum+7)//X
		{
			ReturnZero(1);//归零
			Sleep(500);
			MotorCtrl(1,1,plusenum);//移动到上片位置
		}
	}
}


void CDebug::OnBnClickedSortSet()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned int plusenum = GetDlgItemInt(IDC_PLUSE_NUM,0,0);
	CString str1,str2;
	CString strFilePath;
	/*GetModuleFileName(NULL,strFilePath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
	strFilePath.ReleaseBuffer(); 
	int pos = strFilePath.ReverseFind('\\'); */
	strFilePath = g_path + _T("\\SortSet.ini"); 
	CFileFind findini;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用   
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
				str1.Format(_T("%d档"),listsortsel+1);

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
				str1.Format(_T("%d档"),listsortsel+1);

				WritePrivateProfileString(g_sort.itemstr,str1,str2,strFilePath);  
			}
			m_debuglist.SetItemText(listsortsel, 1, str2);  
		} 
	}
}


void CDebug::OnBnClickedMotor2Run()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	if(g_hDevice==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("ac6641 打开失败"));
		return;
	}
	int tmp=GetDlgItemInt(IDC_MOTOR_NUM,0,1);//读入转数
	if(tmp>65535||tmp<0)
	{
		AfxMessageBox(_T("输入转数不正确或超出范围"));
		return;
	}
	GetDlgItem(IDC_MOTOR2_RUN)->EnableWindow(0);
	int ioData=tmp%256;
	//AC6641_DO(g_hDevice,0,ioData);//发送定时低位
	AC6641_DO(g_hDevice,2,ioData);//发送Y定时低位
	ioData=tmp/256,
		//AC6641_DO(g_hDevice,1,ioData);//发送定时高位
		AC6641_DO(g_hDevice,3,ioData);//发送定时高位
	ioData=AC6641_DI(g_hDevice,5) & 0xf7;//53低电平，全电流
	AC6641_DO(g_hDevice,5,ioData);
	//ioData=AC6641_DI(g_hDevice,4) | 0x02;//将p41置高，为下降沿做准备。
	ioData=AC6641_DI(g_hDevice,4) | 0x20;//将p41置高，为下降沿做准备。
	tmp=m_motorturn.GetCurSel();//获得正反转
	//0==tmp?(ioData=ioData & 0xfe):(ioData=ioData | 0x01);
	0==tmp?(ioData=ioData & 0xef):(ioData=ioData | 0x10);
	AC6641_DO(g_hDevice,4,ioData);//正反转
	Sleep(1);
	//ioData=ioData & 0xfd;
	ioData=ioData & 0xdf;
	AC6641_DO(g_hDevice,4,ioData);//下降沿
	Sleep(1);
	//ioData=ioData | 0x02;
	ioData=ioData | 0x20;
	AC6641_DO(g_hDevice,4,ioData);//
	//while(AC6641_DI(g_hDevice,6) & 0x01);//当高电平时
	//while(AC6641_DI(g_hDevice,6) & 0x10);//当高电平时
	for(int i = 0; i<1000; i++)
	{
		if(!(AC6641_DI(g_hDevice,6) & 0x10))
			break;//当高电平时
		if(i == 998)
		{
			AfxMessageBox(_T("error"));
			break;
		}
		Sleep(1);
	}
	ioData=AC6641_DI(g_hDevice,5) | 0x08;//53高电平，半电流
	AC6641_DO(g_hDevice,5,ioData);
	GetDlgItem(IDC_MOTOR2_RUN)->EnableWindow(1);
}


void CDebug::OnBnClickedTipX()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(1);//将控件的值传入变量
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
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(1);//将控件的值传入变量
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
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(1);//将控件的值传入变量
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
	// TODO: 在此添加控件通知处理程序代码
	if(g_hDevice==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("ac6641 打开失败"));
		return;
	}
	int tmp=10000;//读入转数
	if(tmp>65535||tmp<0)
	{
		AfxMessageBox(_T("输入转数不正确或超出范围"));
		return;
	}
	GetDlgItem(IDC_Y_ZERO)->EnableWindow(0);
	int ioData=tmp%256;
	//AC6641_DO(g_hDevice,0,ioData);//发送定时低位
	AC6641_DO(g_hDevice,2,ioData);//发送Y定时低位
	ioData=tmp/256,
		//AC6641_DO(g_hDevice,1,ioData);//发送定时高位
		AC6641_DO(g_hDevice,3,ioData);//发送定时高位
	ioData=AC6641_DI(g_hDevice,5) & 0xf7;//53低电平，全电流
	AC6641_DO(g_hDevice,5,ioData);
	//ioData=AC6641_DI(g_hDevice,4) | 0x02;//将p41置高，为下降沿做准备。
	ioData=AC6641_DI(g_hDevice,4) | 0x20;//将p41置高，为下降沿做准备。
	tmp=0;//获得正反转
	//0==tmp?(ioData=ioData & 0xfe):(ioData=ioData | 0x01);
	0==tmp?(ioData=ioData & 0xef):(ioData=ioData | 0x10);
	AC6641_DO(g_hDevice,4,ioData);//正反转
	Sleep(1);
	//ioData=ioData & 0xfd;
	ioData=ioData & 0xdf;
	AC6641_DO(g_hDevice,4,ioData);//下降沿
	Sleep(1);
	//ioData=ioData | 0x02;
	ioData=ioData | 0x20;
	AC6641_DO(g_hDevice,4,ioData);//
	//while(AC6641_DI(g_hDevice,6) & 0x01);//当高电平时
	//while(AC6641_DI(g_hDevice,6) & 0x10);//当高电平时
	for(int i = 0; i<1000; i++)
	{
		if(!(AC6641_DI(g_hDevice,6) & 0x10))
			break;//当高电平时
		if(i == 998)
		{
			AfxMessageBox(_T("error"));
			break;
		}
		Sleep(1);
	}
	ioData=AC6641_DI(g_hDevice,5) | 0x08;//53高电平，半电流
	AC6641_DO(g_hDevice,5,ioData);
	GetDlgItem(IDC_Y_ZERO)->EnableWindow(1);
}


void CDebug::OnBnClickedXZero()
{
	// TODO: 在此添加控件通知处理程序代码
	if(g_hDevice==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("ac6641 打开失败"));
		return;
	}
	int tmp=10000;//读入转数
	if(tmp>65535||tmp<0)
	{
		AfxMessageBox(_T("输入转数不正确或超出范围"));
		return;
	}
	GetDlgItem(IDC_X_ZERO)->EnableWindow(0);
	int ioData=tmp%256;
	AC6641_DO(g_hDevice,0,ioData);//发送定时低位
	ioData=tmp/256,
		AC6641_DO(g_hDevice,1,ioData);//发送定时高位
	ioData=AC6641_DI(g_hDevice,5) & 0xfb;//52低电平，全电流
	AC6641_DO(g_hDevice,5,ioData);
	ioData=AC6641_DI(g_hDevice,4) | 0x02;//将p41置高，为下降沿做准备。
	tmp=0;//获得正反转
	0==tmp?(ioData=ioData & 0xfe):(ioData=ioData | 0x01);
	AC6641_DO(g_hDevice,4,ioData);//正反转
	Sleep(1);
	ioData=ioData & 0xfd;
	AC6641_DO(g_hDevice,4,ioData);//下降沿
	Sleep(1);
	ioData=ioData | 0x02;
	AC6641_DO(g_hDevice,4,ioData);//
	//while(AC6641_DI(g_hDevice,6) & 0x01);//当高电平时
	for(int i = 0; i<1000; i++)
	{
		if(!(AC6641_DI(g_hDevice,6) & 0x01))
			break;//当高电平时
		if(i == 998)
		{
			AfxMessageBox(_T("error"));
			break;
		}
		Sleep(1);
	}
	ioData=AC6641_DI(g_hDevice,5) | 0x04;//52高电平，半电流
	AC6641_DO(g_hDevice,5,ioData);
	GetDlgItem(IDC_X_ZERO)->EnableWindow(1);
}


void CDebug::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON1)->EnableWindow(0);
	if(debugupdate)
	{
		debugupdate=0;
		GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("实时"));
		Sleep(1000);
	}
	else
	{
		debugupdate=1;
		GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("停止"));
		p_show=AfxBeginThread(DebugDisplay,NULL,THREAD_PRIORITY_NORMAL,	0,CREATE_SUSPENDED);
		p_show->ResumeThread();


	}
	GetDlgItem(IDC_BUTTON1)->EnableWindow(1);
}


void CDebug::OnBnClickedTest()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_TEST)->EnableWindow(0);
	if(!testStart)
	{
		GetDlgItem(IDC_TEST)->SetWindowText(_T("停止"));
		testStart=1;
		p_test=AfxBeginThread(TestStart,NULL,THREAD_PRIORITY_NORMAL,	0,CREATE_SUSPENDED);
		p_test->ResumeThread();

	}
	else
	{
		GetDlgItem(IDC_TEST)->SetWindowText(_T("测试"));
		testStart=0;
		Sleep(1000);
	}
	GetDlgItem(IDC_TEST)->EnableWindow(1);
}


void CDebug::OnBnClickedDynamicDetect()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_DYNAMIC_DETECT)->EnableWindow(0);
	if(!dynamicDetect)
	{
		dynamicNum=GetDlgItemInt(IDC_DETECT_NUM,0,0);
		if(0==dynamicNum)
			dynamicNum=30;
		CStdioFile file;
		file.Open(pathSaveR1,CFile::modeCreate|CFile::modeWrite);
		CString _degree;
		_degree.Format(_T("%d次\r\n序号\t光轴\t电轴\t等效角\t原始光\t原始电\tD1\tD2\tDM\tR1\tAL\r\n"),dynamicNum);
		char szANSIString[MAX_PATH];  
		WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,_degree,-1,szANSIString,sizeof(szANSIString),NULL,NULL); 
		file.Seek(0,CFile::end);
		//myFile.Write(szANSIString,_sort.GetLength()*2-98);
		file.Write(szANSIString,CStringA(_degree).GetLength());

		//file.WriteString( _degree );
		file.Close();
		GetDlgItem(IDC_DYNAMIC_DETECT)->SetWindowText(_T("停止检测"));
		dynamicDetect=1;
		p_dynamic=AfxBeginThread(DynamicDetect,NULL,THREAD_PRIORITY_NORMAL,	0,CREATE_SUSPENDED);
		p_dynamic->ResumeThread();
	}
	else
	{
		/*if(dynamicNum>0)
		{
		CString str;
		str.Format(_T("还剩 %d次"),dynamicNum);
		int status=AfxMessageBox(str,MB_OKCANCEL);
		if(status==2)//按确定进入
		{
		GetDlgItem(IDC_DYNAMIC_DETECT)->EnableWindow(1);
		return;
		}
		}*/
		//GetDlgItem(IDC_DYNAMIC_DETECT)->SetWindowText(_T("动态检测"));
		dynamicDetect=0;
		Sleep(1000);
		//SetDlgItemInt(IDC_DETECT_NUM,dynamicNum,0);
	}
	GetDlgItem(IDC_DYNAMIC_DETECT)->EnableWindow(1);
}


void CDebug::OnBnClickedPrint()
{
	// TODO: 在此添加控件通知处理程序代码


	CPrintDialog dlg(FALSE,PD_PAGENUMS);     //定义打印机
	//dlg.GetDefaults();
	if (dlg.DoModal() == IDCANCEL)             //获取用户对打印机的设置
		return;
	HDC hdcPrinter = dlg.GetPrinterDC();       //获得打印机的句柄
	if (hdcPrinter == NULL)
	{
		MessageBox(_T("Buy a printer!"));
	}
	else
	{
		//GetDlgItem(IDC_PRINT)->EnableWindow(0);
		CDC dcPrinter;                            //定义一个设备
		dcPrinter.Attach(hdcPrinter);             //把打印机附于这个设备
		DOCINFO docinfo;                          //打印机属性
		memset(&docinfo, 0, sizeof(docinfo));
		docinfo.cbSize = sizeof(docinfo);
		docinfo.lpszDocName = _T("SC-EFG");
		CPrintInfo info;
		info.m_rectDraw.SetRect(0,0, 
			dcPrinter.GetDeviceCaps(HORZRES), 
			dcPrinter.GetDeviceCaps(VERTRES));//设置范围
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
				SetTextColor(dcPrinter,RGB(0, 255, 0));		  //设置打印机的字体颜色，其实还可以设置字体的			
				DrawBmp(&dcPrinter);           //调用打印图片函数，标准的做法应该是还传一个要打印的图片的
				CString str; //获取系统时间
				CTime tm;
				tm=CTime::GetCurrentTime();
				str=tm.Format("%Y-%m-%d %X");
				dcPrinter.TextOut(10, 10, str, str.GetLength());
				dcPrinter.EndPage();            //结束这页
				dcPrinter.EndDoc();             //结束      
			}
		}
		dcPrinter.Detach();             //释放对象
		dcPrinter.DeleteDC();           //释放设备

	}

}
void CDebug::DrawBmp(CDC*pDC)
{

	SIZE s1, s2 ;
	s1.cx = pDC->GetDeviceCaps(HORZRES) ;
	s1.cy = pDC->GetDeviceCaps(VERTRES) ;
	pDC->SetMapMode(MM_ANISOTROPIC); //转化坐标映射模式
	int xLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSX);  //得到设备每逻辑英寸的像素数量
	int yLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSY);
	float fWidth = (float)xLogPixPerInch / 96 ; //得到电脑屏幕映射到视窗大小比率
	float fHeight = (float)yLogPixPerInch / 96; //一般得到的fWidth = fHeight 
	CDC memDC;        //定义一个设备
	CClientDC dc(this);      //获取客户
	memDC.CreateCompatibleDC( &dc );
	CBitmap m_bmp;//创建类成员
	BITMAP bm;//存放位图信息的结构
	CString bmp_path =g_path + _T("\\screen_photo\\erzhihu.bmp");
	HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,bmp_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//创建bitmap指针
	m_bmp.Attach(hBitmap1);//关联句柄和cbitmap关联
	m_bmp.GetBitmap(&bm);//
	CSize size = CSize(bm.bmWidth, bm.bmHeight);
	pDC->SetWindowExt(size); //设置视窗大小
	long xExt = (long)(fWidth * size.cx); //得到视窗大小
	long yExt = (long)(fHeight * size.cy) ;
	if(xExt>s1.cx)
	{

		yExt=yExt*s1.cx/xExt;
		xExt=s1.cx;
	}
	pDC->SetViewportExt((int)xExt-200, (int)yExt-200); //设置视窗大小(由电脑屏幕映射到视窗大小)
	memDC.SelectObject( m_bmp );  //为设备选择对象
	pDC->BitBlt(40, 40,(int)xExt, (int)yExt, &memDC, 0, 0, SRCCOPY);             //把源以copy的形式装载到pDC设备中
	DeleteObject(hBitmap1);//记得删除	
	memDC.DeleteDC();	
}

void CDebug::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	if (g_sort.chgsqu)
	{//itemnum相关
		g_sort.chgsqu = 0;
		m_debuglist.DeleteAllItems();
		int itemnum = g_sort.itemnum;
		CString itemstr = g_sort.itemstr;
		/*if (g_sort.islongsqu)
		{
		itemnum = 12;
		itemstr = _T("长方片分档步数");
		}
		else
		{
		itemnum = 24;
		itemstr = _T("分档步数");
		}*/
		//ini文件初始化或读取

		CString strValue;
		//int iniint=0;
		//CString strFilePath;
		//strFilePath = g_path + _T("\\SortSet.ini"); 

		/////////////////
		//CFileFind findini;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用   
		//BOOL ifFind = findini.FindFile(strFilePath);  
		//if( ifFind )  
		//{
		//	for(int i=0;i<itemnum+6;i++)
		//	{
		//		strValue.Format(_T("%d档"),i+1);
		//		g_status.Ysort[i]=GetPrivateProfileInt(itemstr,strValue,0,strFilePath);  //写入全局变量
		//	}
		//	strValue.Format(_T("%d档"),itemnum+7);
		//	g_status.Xwait=GetPrivateProfileInt(itemstr,strValue,0,strFilePath);  //写入全局变量
		//	strValue.Format(_T("%d档"),itemnum+8);
		//	g_status.Xcheck=GetPrivateProfileInt(itemstr,strValue,0,strFilePath);  //写入全局变量
		//	strValue.Format(_T("%d档"),itemnum+9);
		//	g_status.Ywait=GetPrivateProfileInt(itemstr,strValue,0,strFilePath);  //写入全局变量
		//}

		for(int i=0;i<itemnum+6;i++)
		{
			strValue="";
			strValue.Format(_T("%d"),i+1);//_0611_原i+1
			m_debuglist.InsertItem(i, strValue);   
			if(3 == g_status._MOTOR_NUM)
				strValue.Format(_T("%d"),g_status.Zsort[i]);//度
			else
				strValue.Format(_T("%d"),g_status.Ysort[i]);//度
			m_debuglist.SetItemText(i, 1, strValue);   
			strValue.Format(_T("第%02d档的电机步数"),i+1);//度//_0611_原i+1
			m_debuglist.SetItemText(i, 2, strValue);   
		}
		strValue.Format(_T("%d"),itemnum+7);m_debuglist.InsertItem(itemnum+6, strValue); strValue.Format(_T("%d"),g_status.Xwait);m_debuglist.SetItemText(itemnum+6, 1, strValue);m_debuglist.SetItemText(itemnum+6, 2, _T("X轴等待位置"));   
		strValue.Format(_T("%d"),itemnum+8);m_debuglist.InsertItem(itemnum+7, strValue); strValue.Format(_T("%d"),g_status.Xcheck);m_debuglist.SetItemText(itemnum+7, 1, strValue);m_debuglist.SetItemText(itemnum+7, 2, _T("X轴放片位置"));   
		strValue.Format(_T("%d"),itemnum+9);m_debuglist.InsertItem(itemnum+8, strValue); strValue.Format(_T("%d"),g_status.Ywait);m_debuglist.SetItemText(itemnum+8, 1, strValue);m_debuglist.SetItemText(itemnum+8, 2, _T("Y轴等待位置"));   
	}

	// 不为绘图消息调用 CDialogEx::OnPaint()
	CString bmp_path =g_path + _T("\\screen_photo\\erzhihu.bmp");//_T("d://tmep1.bmp")
	CFileFind findini;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用   
	BOOL ifFind = findini.FindFile(bmp_path);  
	if( !ifFind )  
		return;
	CBitmap m_bmp;//创建类成员
	BITMAP bm;//存放位图信息的结构
	HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,bmp_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//创建bitmap指针
	if (hBitmap1 == NULL)
	{
		DeleteObject(hBitmap1);//记得删除
		return;
	}
	m_bmp.Attach(hBitmap1);//关联句柄和cbitmap关联
	m_bmp.GetBitmap(&bm);
	//m_bmp.LoadBitmapA(bmp_path);

	CWnd * pWnd = GetDlgItem(IDC_PREVIEW);
	CDC* pDC = pWnd->GetDC();
	CRect rect;
	GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
	//

	CDC memDC;        //定义一个设备
	CClientDC dc1(this);      //获取客户
	memDC.CreateCompatibleDC( &dc1 );
	memDC.SelectObject( m_bmp );  //为设备选择对象
	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	DeleteObject(hBitmap1);//记得删除	
	memDC.DeleteDC();	

	pWnd->ReleaseDC(pDC);
	//pDC->DeleteDC();

}


void CDebug::OnBnClickedAlert()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(1);//将控件的值传入变量
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
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	m_VGAthreshold=GetDlgItemInt(IDC_EDIT1,0,0);
}


void CDebug::OnBnClickedChkDebug()
{
	// TODO: 在此添加控件通知处理程序代码
	g_status._DEBUGSCR = ((CButton*)GetDlgItem(IDC_CHK_DEBUG))->GetCheck();
}


void CDebug::OnBnClickedChkTblow()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	if(3 == g_status._MOTOR_NUM)
	{
		CString str1,str2;
		CString strFilePath;
		int num = g_sort.itemnum+6;//档位数
		int firstStep = g_status.Zsort[0];
		int secondStep = g_status.Zsort[1];
		int endStep = g_status.Zsort[g_sort.itemnum+5];
		strFilePath = g_path + _T("\\SortSet.ini"); 
		CFileFind findini;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用   
		BOOL ifFind = findini.FindFile(strFilePath);  
		if( ifFind )  
		{
			switch(AfxMessageBox(_T("以第一档第二档为参照？"),MB_YESNOCANCEL))
			{
			case 6:
				for (int i=2;i<g_sort.itemnum+6;i++)
				{
					g_status.Zsort[i] = firstStep + (secondStep - firstStep)*i;

					str2.Format(_T("%d"),g_status.Zsort[i]);
					str1.Format(_T("%d档"),i+1);

					WritePrivateProfileString(g_sort.itemstr,str1,str2,strFilePath);  
					m_debuglist.SetItemText(i, 1, str2);   
				}
				break;
			case 7:
				for (int i=1;i<g_sort.itemnum+5;i++)
				{
					g_status.Zsort[i] = firstStep + (endStep - firstStep)*i/(num - 1);

					str2.Format(_T("%d"),g_status.Zsort[i]);
					str1.Format(_T("%d档"),i+1);

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
		int num = g_sort.itemnum + 6;//档位数
		int firstStep = g_status.Ysort[0];
		int secondStep = g_status.Ysort[1];
		int endStep = g_status.Ysort[g_sort.itemnum + 5];
		strFilePath = g_path + _T("\\SortSet.ini");
		CFileFind findini;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用   
		BOOL ifFind = findini.FindFile(strFilePath);
		if (ifFind)
		{
			switch (AfxMessageBox(_T("以第一档第二档为参照？"), MB_YESNOCANCEL))
			{
			case 6:
				for (int i = 2; i<g_sort.itemnum + 6; i++)
				{
					g_status.Ysort[i] = firstStep + (secondStep - firstStep)*i;

					str2.Format(_T("%d"), g_status.Ysort[i]);
					str1.Format(_T("%d档"), i + 1);

					WritePrivateProfileString(g_sort.itemstr, str1, str2, strFilePath);
					m_debuglist.SetItemText(i, 1, str2);
				}
				break;
			case 7:
				for (int i = 1; i<g_sort.itemnum + 5; i++)
				{
					g_status.Ysort[i] = firstStep + (endStep - firstStep)*i / (num - 1);

					str2.Format(_T("%d"), g_status.Ysort[i]);
					str1.Format(_T("%d档"), i + 1);

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
	// TODO: 在此添加控件通知处理程序代码
	if(g_hDevice==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("ac6641 打开失败"));
		return;
	}
	int tmp=GetDlgItemInt(IDC_MOTOR_NUM,0,1);//读入转数
	int tmpfx=m_motorturn.GetCurSel();//获得正反转
	if(tmp>65535||tmp<0)
	{
		AfxMessageBox(_T("输入转数不正确或超出范围"));
		return;
	}
	GetDlgItem(IDC_MOTOR3_RUN)->EnableWindow(0);

	MotorCtrl(3,tmpfx,tmp);//
	GetDlgItem(IDC_MOTOR3_RUN)->EnableWindow(1);
}


void CDebug::OnBnClickedZZero()
{
	// TODO: 在此添加控件通知处理程序代码
	if(g_hDevice==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("ac6641 打开失败"));
		return;
	}
	GetDlgItem(IDC_Z_ZERO)->EnableWindow(0);

	ReturnZero(3);
	GetDlgItem(IDC_Z_ZERO)->EnableWindow(1);

}


void CDebug::OnBnClickedTipZ()
{
	// TODO: 在此添加控件通知处理程序代码
	bool chk = ((CButton*)GetDlgItem(IDC_TIP_Z))->GetCheck();
	TubeCtrl(3,chk);
}


void CDebug::OnBnClickedChkCy()
{
	// TODO: 在此添加控件通知处理程序代码
	bool chk = ((CButton*)GetDlgItem(IDC_CHK_CY))->GetCheck();
	CylinderCtrlDbg(chk);
}


void CDebug::OnBnClickedChkEfg()
{
	// TODO: 在此添加控件通知处理程序代码
	bool chk = ((CButton*)GetDlgItem(IDC_CHK_EFG))->GetCheck();
	StartFTLong(chk);
}

