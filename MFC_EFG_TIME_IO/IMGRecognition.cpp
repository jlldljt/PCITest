#include "StdAfx.h"
#include "IMGRecognition.h"
double gd_PI = 3.1415926;
CFont font;
double CalculateVectorAngle(double x1, double y1, double x2, double y2);

CIMGRecognition::CIMGRecognition(void)
{
  g_stu_square.nN = 0;
  g_stu_square.bIsLap = 0;
  g_stu_square.bJudgeFeature = 0;
  stuRef.Dev = 0;
  //stuRef.SAg = 82;
  //stuRef.BAg = 98;
  //font.CreatePointFont(100, _T("黑体"), NULL);
}


CIMGRecognition::~CIMGRecognition(void)
{
  font.DeleteObject();
}

int CIMGRecognition::readBmp(char* bmpName, unsigned char*& pBmpBuf, int& bmpWidth, int& bmpHeight)
{
  //二进制读方式打开指定的图像文件 
  FILE* fp;
  fp = NULL;
  int suc = fopen_s(&fp, bmpName, "rb");


  if (suc != 0) return suc;
  //跳过位图文件头结构BITMAPFILEHEADER 
  fseek(fp, sizeof(BITMAPFILEHEADER), 0);
  //定义位图信息头结构变量，读取位图信息头进内存，存放在变量head中 
  BITMAPINFOHEADER head;
  fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);
  //获取图像宽、高、每像素所占位数等信息 
  bmpWidth = head.biWidth;
  bmpHeight = head.biHeight;
  biBitCount = head.biBitCount;
  //定义变量，计算图像每行像素所占的字节数（必须是4的倍数） 
  int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
  //灰度图像有颜色表，且颜色表表项为256 
  if (biBitCount == 8)   //申请颜色表所需要的空间，读颜色表进内存 
  {
    //RGBQUAD *pColorTable1=NULL;//颜色表指针//mmy 去除把颜色表保存下来用于保存bmp
    pColorTable1 = new RGBQUAD[256];
    fread(pColorTable1, sizeof(RGBQUAD), 256, fp);
    //delete[]  pColorTable1;
  }
  else
  {
    pColorTable1 = NULL;
  }
  //申请位图数据所需要的空间，读位图数据进内存 
  pBmpBuf = new unsigned char[lineByte * bmpHeight];
  fread(pBmpBuf, 1, lineByte * bmpHeight, fp);
  //关闭文件 
  fclose(fp);
  fp = NULL;
  return 0;

}

bool CIMGRecognition::saveBmp(char* bmpName, unsigned char* imgBuf, int width, int height, int biBitCount, RGBQUAD* pColorTable)
{
  //如果位图数据指针为0，则没有数据传入，函数返回 
  if (!imgBuf)
    return 0;
  //颜色表大小，以字节为单位，灰度图像颜色表为1024字节，彩色图像颜色表大小为0 
  int colorTablesize = 0;
  if (biBitCount == 8)
    colorTablesize = 1024;
  //待存储图像数据每行字节数为4的倍数 
  int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;
  //以二进制写的方式打开文件 
  FILE * fp;
  fopen_s(&fp, bmpName, "wb");

  // FILE *fp=fopen(bmpName,"wb"); 
  if (fp == 0) return 0;
  //申请位图文件头结构变量，填写文件头信息 
  BITMAPFILEHEADER fileHead;
  fileHead.bfType = 0x4D42;//bmp类型 
  //bfSize是图像文件4个组成部分之和 
  fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
    + colorTablesize + lineByte * height;
  fileHead.bfReserved1 = 0;
  fileHead.bfReserved2 = 0;
  //bfOffBits是图像文件前3个部分所需空间之和 
  fileHead.bfOffBits = 54 + colorTablesize;
  //写文件头进文件 
  fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);
  //申请位图信息头结构变量，填写信息头信息 
  BITMAPINFOHEADER head;
  head.biBitCount = biBitCount;
  head.biClrImportant = 0;
  head.biClrUsed = 0;
  head.biCompression = 0;
  head.biHeight = height;
  head.biPlanes = 1;
  head.biSize = 40;
  head.biSizeImage = lineByte * height;
  head.biWidth = width;
  head.biXPelsPerMeter = 0;
  head.biYPelsPerMeter = 0;
  //写位图信息头进内存 
  fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);
  //如果灰度图像，有颜色表，写入文件  
  if (biBitCount == 8) {
    //pColorTable=new RGBQUAD[256]; //MMY由于这里新建了颜色表，但是值是默认值不是读入的颜色表值，所以保存的bnp由于颜色表值都一样而只有一种颜色
    fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);
  }
  //写位图数据进文件 
  fwrite(imgBuf, height * lineByte, 1, fp);
  //关闭文件 
  fclose(fp);
  //delete[] pColorTable;//MMY
  //pColorTable=NULL;//MMY
  return 1;

}

//读取图像阀值
//返回-1 读取失败
//返回正整数 图像阀值
//oprtWth 算子宽度
//oprtDVal 算子的允许差值
//validDVal 可以计算阀值的有效差值
int CIMGRecognition::read_image_fz(unsigned char*& pBmpBuf, int bmpWidth, int bmpHeight, int oprtWth = 16, int oprtDVal = 25, int validDVal = 50)
{
  int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4; //mmy
  int nthreshold = -1, nTmpThreshold = -1, nMaxThrd = 0, nMinThrd = 255;
  int nTmpRgb, nMinRgb, nMaxRgb, nTotalRgb;
  for (int i = 0; i <= bmpHeight - 1; i++)
  {
    nthreshold = -1;
    nTmpThreshold = -1;
    for (int j = 0; j <= bmpWidth - 1; j++)
    {
      if (i >= bmpHeight - oprtWth || j >= bmpWidth - oprtWth)
        continue;

      nMaxRgb = 0;
      nMinRgb = 255;
      nTotalRgb = 0;

      for (int m = i; m < i + oprtWth; m++)
      {
        //nTmpRgb=*(pBmpBuf+m*lineByte+j*(biBitCount>>3));
        nTmpRgb = ReadBmpGray(pBmpBuf, biBitCount, bmpWidth, bmpHeight, m, j);
        if (nTmpRgb > nMaxRgb) nMaxRgb = nTmpRgb;
        if (nTmpRgb < nMinRgb) nMinRgb = nTmpRgb;
        nTotalRgb += nTmpRgb;
        nTmpRgb = ReadBmpGray(pBmpBuf, biBitCount, bmpWidth, bmpHeight, m, (j + oprtWth - 1));
        //nTmpRgb=*(pBmpBuf+m*lineByte+(j+oprtWth-1)*(biBitCount>>3));
        if (nTmpRgb > nMaxRgb) nMaxRgb = nTmpRgb;
        if (nTmpRgb < nMinRgb) nMinRgb = nTmpRgb;
        nTotalRgb += nTmpRgb;
      }

      for (int m = j + 1; m < j + oprtWth - 1; m++)
      {
        //nTmpRgb=*(pBmpBuf+i*lineByte+m*(biBitCount>>3));
        nTmpRgb = ReadBmpGray(pBmpBuf, biBitCount, bmpWidth, bmpHeight, i, m);
        if (nTmpRgb > nMaxRgb) nMaxRgb = nTmpRgb;
        if (nTmpRgb < nMinRgb) nMinRgb = nTmpRgb;
        nTotalRgb += nTmpRgb;
        nTmpRgb = ReadBmpGray(pBmpBuf, biBitCount, bmpWidth, bmpHeight, (i + oprtWth - 1), m);
        //nTmpRgb=*(pBmpBuf+(i+oprtWth-1)*lineByte+m*(biBitCount>>3));
        if (nTmpRgb > nMaxRgb) nMaxRgb = nTmpRgb;
        if (nTmpRgb < nMinRgb) nMinRgb = nTmpRgb;
        nTotalRgb += nTmpRgb;
      }

      if ((nMaxRgb - nMinRgb) > oprtDVal)
        continue;

      nTmpThreshold = nTotalRgb / ((oprtWth << 2) - 4);
      if (nTmpThreshold > nMaxThrd) nMaxThrd = nTmpThreshold;
      if (nTmpThreshold < nMinThrd) nMinThrd = nTmpThreshold;
      if (-1 == nthreshold)
      {
        nthreshold = nTmpThreshold;
        continue;
      }

      if (abs(nthreshold - nTmpThreshold) > validDVal)
      {
        nthreshold = (abs(nthreshold - nTmpThreshold) >> 2) + (nthreshold > nTmpThreshold ? nTmpThreshold : nthreshold);
        return nthreshold;
      }
    }
  }
  if (nMaxThrd - nMinThrd > validDVal)
    return (nMaxThrd - nMinThrd) * 5 / 12 + nMinThrd;
  else
    return -1;
}


//xs，fb全0；jg，tx白-1，黑0；
//返回0,图像有可识别区域
//返回1,图像无可识别区域
//晶片多个读取
int CIMGRecognition::read_image_mul(unsigned char*& pBmpBuf, int*& xsvalue, int*& fbvalue, int*& jgvalue, int*& txvalue, int bmpWidth, int bmpHeight, int threshold, bool debug, bool delNoise = 0)
{
  int type = 1;
  int temp_rgb = 0;
  int i_fz = threshold;//阀值
  int i, j;
  int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
  int min_t, max_b, min_l, max_r;
  //min_l=300;max_r=bmpWidth-3;min_t=2;max_b=bmpHeight-3;
  min_l = 3; max_r = bmpWidth - 3; min_t = 2; max_b = bmpHeight - 3;
  //min_l=bmpWidth>>2;max_r=bmpWidth-5;min_t=5;max_b=bmpHeight-5;(1806)
  //min_l=(340);max_r=1815;min_t=5;max_b=bmpHeight-30;
  for (i = 0; i <= bmpHeight - 1; i++)   //进行二值化处理：在中心区域内按像素值判断二值化，中心区域外全部置0
  {
    for (j = 0; j <= bmpWidth - 1; j++)
    {
      *(xsvalue + (bmpHeight - 1 - i) * bmpWidth + j) = 0;
      *(fbvalue + (bmpHeight - 1 - i) * bmpWidth + j) = 0;
      temp_rgb = ReadBmpGray(pBmpBuf, biBitCount, bmpWidth, bmpHeight, i, j);
      //temp_rgb=*(pBmpBuf+i*lineByte+j*(biBitCount>>3));

      if (i >= min_t && i <= max_b && j >= min_l && j <= max_r && temp_rgb > i_fz)//在指定区域内进行二值化，超过区域认为是黑色置0
      {
        *(jgvalue + (bmpHeight - 1 - i) * bmpWidth + j) = -1;
        *(txvalue + (bmpHeight - 1 - i) * bmpWidth + j) = -1;
        if (debug)
          data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, (bmpHeight - 1 - i), j, 255, 255, 255);//debug
        type++;
      }
      else
      {
        *(jgvalue + (bmpHeight - 1 - i) * bmpWidth + j) = 0;
        *(txvalue + (bmpHeight - 1 - i) * bmpWidth + j) = 0;
        if (debug)
          data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, (bmpHeight - 1 - i), j, 0, 0, 0);//debug
      }
    }
  }
  if (debug)
  {
    sprintf(szname, "%s\\read1.bmp", chPathMain);
    saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
  }
  //去噪
  if (delNoise)
  {
    int bWhite, bBlack;
    int oprtWth = 32;//以oprtWth的长宽算子遍历全图像，去噪16
    for (i = 0; i <= bmpHeight - 1; i++)
    {
      for (j = 0; j <= bmpWidth - 1; j++)
      {
        if (i < bmpHeight - oprtWth && j < bmpWidth - oprtWth)
        {
          bWhite = 0;
          bBlack = 0;
          for (int m = i; m < i + oprtWth; m++)
          {
            if (-1 == *(jgvalue + m * bmpWidth + j) || -1 == *(jgvalue + m * bmpWidth + j + oprtWth - 1))
            {
              bWhite++;
              break;
            }
          }
          for (int m = j + 1; m < j + oprtWth - 1; m++)
          {
            if (-1 == *(jgvalue + i * bmpWidth + m) || -1 == *(jgvalue + (i + oprtWth - 1) * bmpWidth + m))
            {
              bWhite++;
              break;
            }
          }
          if (bWhite < 1)
          {
            for (int m = i; m < i + oprtWth; m++)
            {
              for (int n = j; n < j + oprtWth; n++)
              {
                if (1 == *(fbvalue + m * bmpWidth + n))
                  continue;
                *(fbvalue + m * bmpWidth + n) = 1;//标记已处理
                *(jgvalue + (m)* bmpWidth + n) = 0;
                *(txvalue + (m)* bmpWidth + n) = 0;
                if (debug)
                  data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, (m), n, 0, 0, 0);//debug
              }
            }
          }
        }
      }
    }
  }
  //saveBmp(".\\PIC\\read2.bmp",pBmpBuf,bmpWidth,bmpHeight,biBitCount,pColorTable1);
  if (type > 20)
  {
    type = 0;
  }
  return type;
}
/*int CIMGRecognition::read_image_mul(unsigned char * &pBmpBuf,int * &xsvalue,int * &fbvalue,int * &jgvalue,int *& txvalue,int bmpWidth,int bmpHeight,int threshold,bool debug)
{
  int type=1;
  int temp_rgb=0;
  int i_fz=threshold;//阀值
  int i,j,k;
  int lineByte=(bmpWidth*biBitCount/8+3)/4*4;
  int min_t,max_b,min_l,max_r;
  min_l=1;max_r=bmpWidth-2;min_t=1;max_b=bmpHeight-2;

  for(i=0;i<=bmpHeight-1;i++)   //进行二值化处理：在中心区域内按像素值判断二值化，中心区域外全部置0
  {
    for(j=0;j<=bmpWidth-1;j++)
    {
      *(xsvalue+(bmpHeight-1-i)*bmpWidth+j)=0;
      *(fbvalue+(bmpHeight-1-i)*bmpWidth+j)=0;
      temp_rgb=*(pBmpBuf+i*lineByte+j*(biBitCount/8));
      if (i>=min_t && i<=max_b && j>=min_l && j<=max_r && temp_rgb>i_fz)
      {
        *(jgvalue+(bmpHeight-1-i)*bmpWidth+j)=-1;
        *(txvalue+(bmpHeight-1-i)*bmpWidth+j)=-1;
        if(debug)
          data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,(bmpHeight-1-i),j,255,255,255);//debug
        type++;
      }else
      {
        *(jgvalue+(bmpHeight-1-i)*bmpWidth+j)=0;
        *(txvalue+(bmpHeight-1-i)*bmpWidth+j)=0;
        if (debug)
          data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,(bmpHeight-1-i),j,0,0,0);//debug
      }
    }
  }

  if (type>20)
  {
    type=0;
  }
  return type;
}*/
//返回-1,在图像中心部位没找到片
//返回0,正常找到图像
//返回1,不正常找到图像,片超出图像位置
//中心晶片单个读取
int CIMGRecognition::read_image_blank(unsigned char*& pBmpBuf, int*& xsvalue, int*& fbvalue, int*& jgvalue, int*& txvalue, int bmpWidth, int bmpHeight, int threshold)
{

  /*
  delete xsvalue;
  delete fbvalue;
  delete jgvalue;
  delete txvalue;
  xsvalue=new int[bmpWidth * bmpHeight];
  fbvalue=new int[bmpWidth * bmpHeight];
  jgvalue=new int[bmpWidth * bmpHeight];
    txvalue=new int[bmpWidth * bmpHeight];
  */
  int type = 0;
  int temp_rgb = 0;
  int i_fz = 255 - threshold;//,fzk=80;//阀值
  int i, j, k;
  int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
  //int temprgb[20]={0};

    //单个晶片二值化：
    //先计算晶片所在的中心区域：上极值，下极值，左极值，右极值
    //对中心区域进行二值化处理
  int min_t, max_b, min_l, max_r;
  min_t = bmpHeight;
  min_l = bmpWidth;
  max_r = 0;
  max_b = 0;
  int bmpzxh = (int)(bmpHeight / 2);
  int bmpzxw = (int)(bmpWidth / 2);
  int tempr, templ, tempcs;
  tempr = 0;
  templ = 0;
  int kmax = 0;

  int min_l_h, max_r_h, min_t_w, max_b_w; //储存各边界点的坐标
  min_t = 10000; max_b = 0; min_l = 10000; max_r = 0;
  for (k = 0; k < 1; k++) {
    for (i = bmpzxh; i >= 0; i--)  //bmp数据中零位相当于图片中的左下角 //从中心点开始对每行进行循环判断，当像素值小于阀值时终止
      //左右两边在中心线开始的像素值都低于阀值时退出行循环
    {
      tempr = 0;
      templ = 0;
      tempcs = 0;
      for (j = bmpzxw; j >= 0; j--)
      {
        temp_rgb = *(pBmpBuf + i * lineByte + j * (biBitCount / 8));
        if (temp_rgb > 255 - i_fz)
        {
          tempr = 1;
          if (min_l > j) {
            min_l = j;  //左下边极值
            min_l_h = i;
          }
        }
        else {//这就是造成斜度太大的多边形，左右两边会被阉割的问题，一旦找到黑色就不继续在这一部分找极值导致一些出头鸟没判断过去
         //这不是原因，原因在下面重新找左右极值时，下一句判断==1造成的
          if (tempr == 0) tempcs++;
          break;
        }
      }
      for (j = bmpzxw + 1; j <= bmpWidth - 1; j++)
      {
        temp_rgb = *(pBmpBuf + i * lineByte + j * (biBitCount / 8));
        if (temp_rgb > 255 - i_fz) {
          templ = 1;
          if (max_r < j) {
            max_r = j;  //右下边极值
            max_r_h = i;
          }
        }
        else {
          if (templ == 0) tempcs++;
          break;
        }
      }
      if (tempcs >= 2) break;
    }
    if (min_l == 10000 || max_r == 0) break;//如果没有极值，既中心区域是黑色，退出检测

    for (i = bmpzxh; i <= bmpHeight - 1; i++)
    {
      tempr = 0;
      templ = 0;
      tempcs = 0;
      for (j = bmpzxw; j >= 0; j--)
      {
        temp_rgb = *(pBmpBuf + i * lineByte + j * (biBitCount / 8));
        if (temp_rgb > 255 - i_fz) {
          tempr = 1;
          if (min_l > j) {
            min_l = j;  //左上边极值
            min_l_h = i;
          }
        }
        else {
          if (tempr == 0) tempcs++;
          break;
        }
      }
      for (j = bmpzxw + 1; j <= bmpWidth - 1; j++)
      {
        temp_rgb = *(pBmpBuf + i * lineByte + j * (biBitCount / 8));
        if (temp_rgb > 255 - i_fz) {
          templ = 1;
          if (max_r < j) {
            max_r = j;  //右上边极值
            max_r_h = i;
          }
        }
        else {
          if (templ == 0) tempcs++;
          break;
        }
      }
      if (tempcs >= 2) break;
    }
    if (min_l == 10000 || max_r == 0) break;//如果没有极值，既中心区域是黑色，退出检测

    for (j = min_l; j <= bmpWidth - 1; j++)
    {
      tempr = 0;
      templ = 0;
      tempcs = 0;
      for (i = min_l_h; i >= 0; i--)   //minlh是minl的y坐标
      {
        temp_rgb = *(pBmpBuf + i * lineByte + j * (biBitCount / 8));
        if (temp_rgb > 255 - i_fz) {
          tempr = 1;
          if (min_t > i) {
            min_t = i;  //下边极值，相对图像中是上边极值
            min_t_w = j;
          }
        }
        else {
          if (tempr == 0) tempcs++;
          break;
        }
      }
      for (i = min_l_h + 1; i <= bmpHeight - 1; i++)
      {
        temp_rgb = *(pBmpBuf + i * lineByte + j * (biBitCount / 8));
        if (temp_rgb > 255 - i_fz) {
          templ = 1;
          if (max_b < i) {
            max_b = i;  //上边极值，相对图像中是下边极值
            max_b_w = j;
          }
        }
        else {
          if (templ == 0) tempcs++;
          break;
        }
      }
      if (tempcs >= 2) break;
    }
    if (min_t == 10000 || max_b == 0) break;
    for (j = max_r; j >= 0; j--)
    {
      tempr = 0;
      templ = 0;
      tempcs = 0;
      for (i = max_r_h; i >= 0; i--)
      {
        temp_rgb = *(pBmpBuf + i * lineByte + j * (biBitCount / 8));
        if (temp_rgb > 255 - i_fz) {
          tempr = 1;
          if (min_t > i) {
            min_t = i;  //下边极值
            min_t_w = j;
          }
        }
        else {
          if (tempr == 0) tempcs++;
          break;
        }
      }
      for (i = max_r_h + 1; i <= bmpHeight - 1; i++)
      {
        temp_rgb = *(pBmpBuf + i * lineByte + j * (biBitCount / 8));
        if (temp_rgb > 255 - i_fz) {
          templ = 1;
          if (max_b < i) {
            max_b = i;  //上边极值
            max_b_w = j;
          }
        }
        else {
          if (templ == 0) tempcs++;
          break;
        }
      }
      if (tempcs >= 2) break;
    }
    if (min_t == 10000 || max_b == 0) break;
    //重新计算左右两边的极限
    for (i = max_b; i >= 0; i--)
      //左右两边在中心线开始的像素值都低于阀值时退出行循环
    {
      tempr = 0;
      templ = 0;
      tempcs = 0;
      for (j = max_b_w; j >= 0; j--)
      {
        temp_rgb = *(pBmpBuf + i * lineByte + j * (biBitCount / 8));
        if (temp_rgb > 255 - i_fz) {
          tempr = 1;
          if (min_l > j) {
            min_l = j;  //左边极值
            min_l_h = i;
          }
        }
        else {
          if (tempr == 0) tempcs++;//原先判断=1，mmy改=0，这是造成左右极值没找对的原因；以下同
          break;
        }
      }
      for (j = max_b_w + 1; j <= bmpWidth - 1; j++)
      {
        temp_rgb = *(pBmpBuf + i * lineByte + j * (biBitCount / 8));
        if (temp_rgb > 255 - i_fz) {
          templ = 1;
          if (max_r < j) {
            max_r = j;  //右边极值
            max_r_h = i;
          }
        }
        else {
          if (templ == 0) tempcs++;
          break;
        }
      }
      if (tempcs >= 2) break;
    }

    for (i = min_t; i <= bmpHeight - 1; i++)
    {
      tempr = 0;
      templ = 0;
      tempcs = 0;
      for (j = min_t_w; j >= 0; j--)
      {
        temp_rgb = *(pBmpBuf + i * lineByte + j * (biBitCount / 8));
        if (temp_rgb > 255 - i_fz) {
          tempr = 1;
          if (min_l > j) {
            min_l = j;  //左边极值
            min_l_h = i;
          }
        }
        else {
          if (tempr == 0) tempcs++;
          break;
        }
      }
      for (j = min_t_w + 1; j <= bmpWidth - 1; j++)
      {
        temp_rgb = *(pBmpBuf + i * lineByte + j * (biBitCount / 8));
        if (temp_rgb > 255 - i_fz) {
          templ = 1;
          if (max_r < j) {
            max_r = j;  //右边极值
            max_r_h = i;
          }
        }
        else {
          if (templ == 0) tempcs++;
          break;
        }
      }
      if (tempcs >= 2) break;
    }
  }
  if (min_l == 0)
  {
    min_l = 1; type = 1;
  }
  if (max_r == bmpWidth - 1)
  {
    max_r = bmpWidth - 2; type = 1;
  }
  if (min_t == 0)
  {
    min_t = 1; type = 1;
  }
  if (max_b == bmpHeight - 1)
  {
    max_b = bmpHeight - 2; type = 1;
  }
  if (min_l == 10000 || max_r == 0 || min_t == 10000 || max_b == 0) {
    for (i = 0; i <= bmpHeight - 1; i++)   //进行二值化处理：在中心区域内按像素值判断二值化，中心区域外全部置0
    {
      for (j = 0; j <= bmpWidth - 1; j++)
      {
        *(xsvalue + (bmpHeight - 1 - i) * bmpWidth + j) = 0;
        *(fbvalue + (bmpHeight - 1 - i) * bmpWidth + j) = 0;
        *(jgvalue + (bmpHeight - 1 - i) * bmpWidth + j) = 0;
        *(txvalue + (bmpHeight - 1 - i) * bmpWidth + j) = 0;
      }
      type = -1;
    }
  }
  else {

    for (i = 0; i <= bmpHeight - 1; i++)   //进行二值化处理：在中心区域内按像素值判断二值化，中心区域外全部置0
    {

      for (j = 0; j <= bmpWidth - 1; j++)
      {
        *(xsvalue + (bmpHeight - 1 - i) * bmpWidth + j) = 0;
        *(fbvalue + (bmpHeight - 1 - i) * bmpWidth + j) = 0;
        temp_rgb = *(pBmpBuf + i * lineByte + j * (biBitCount / 8));
        if (i >= min_t - 1 && i <= max_b + 1 && j >= min_l - 1 && j <= max_r + 1 && temp_rgb > 255 - i_fz) {
          *(jgvalue + (bmpHeight - 1 - i) * bmpWidth + j) = -1;
          *(txvalue + (bmpHeight - 1 - i) * bmpWidth + j) = -1;
          if (g_stu_square.debug)data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, (bmpHeight - 1 - i), j, 255, 0, 0);//debug

        }
        else {
          *(jgvalue + (bmpHeight - 1 - i) * bmpWidth + j) = 0;
          *(txvalue + (bmpHeight - 1 - i) * bmpWidth + j) = 0;
        }
      }
    }
    //type=0;
  }
  return type;
}
int CIMGRecognition::bj_close(int* sz_h, int* sz_l, int sz_num, int zb_h, int zb_l)
{
  int tempi;
  for (tempi = 0; tempi <= sz_num - 1; tempi++)
  {
    if ((*(sz_h + tempi) == zb_h) && (*(sz_l + tempi) == zb_l))
    {
      return tempi;        //返回在何处闭合
    //  exit;
    }
  }
  return -1;   //-1表示不闭合
}
//对区域进行填充
//返回区域面积
/*
int data_check_fill_bj1(unsigned char * &pBmpBuf,int qybn, int fillv, int temp_close, int * temp_bj_h, int * temp_bj_l,int * & xsvalue,int * & fbvalue,int * & jgvalue,int *& txvalue,int bmpWidth,int bmpHeight,int tmin_h,int tmax_h,int tmin_l,int tmax_l)
{
  //int lineByte=(bmpWidth * biBitCount/8+3)/4*4; //mmy
 int temph,templ,tempw1,tempw2,tempi,templn,tempflag;
 int *templb,*temple;
 templb=NULL;
 temple=NULL;
 templb=new int[1000];
 temple=new int[1000];
 int temparea=0;

//for (temph=0;temph<=temp_close-1;temph++)
//   *(jgvalue+(*(temp_bj_h+temph))*bmpWidth+*(temp_bj_l+temph))=fillv;

  for (temph=tmin_h;temph<=tmax_h;temph++)//行循环
  {
    tempw1=-1;
    tempw2=-1;
  tempflag=-1;
  templn=0;
  for(int j=0;j<1000;j++){//将两个列数组初始化
    *(templb+j)=-1;
    *(temple+j)=-1;
  }
    for (templ=tmin_l;templ<=tmax_l;templ++)//列循环
  {
    //mmy
    data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,temph,templ,0,0,0);

    //*(pBmpBuf+(bmpHeight-1-temph)*lineByte+templ*(biBitCount/8))=0;
    //		*(pBmpBuf+(bmpHeight-1-temph)*lineByte+templ*(biBitCount/8)+1)=0;
    //		*(pBmpBuf+(bmpHeight-1-temph)*lineByte+templ*(biBitCount/8)+2)=0;
        //
    if (*(jgvalue+temph*bmpWidth+templ)==qybn) //如果当前jg的值是传入的区域的编号，说明是边界
    {
            *(fbvalue+temph*bmpWidth+templ)=qybn;//边界填充
      *(xsvalue+temph*bmpWidth+templ)=qybn;//区域内填充
      //*(jgvalue+temph*bmpWidth+templ)=fillv;//mmy去掉置换jg，保留jg的原貌
      ///////////////////////////////////////////mmy
      data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,temph,templ,255,0,0);
      temparea++;

      /////////////////////////////////////////////
            if (tempw1==-1)
        tempw1=templ;   //记录本行第一次像素值和区域编号相同的列
            tempw2=templ;   //记录最后一个相同的列号
    }
    if(tempw1>=0 && *(jgvalue+temph*bmpWidth+templ)==-1){//如果本行已经记录第一次像素值和区域编号相同的列，且当前值是白色
      if(*(templb+templn)==-1)  //起点
        *(templb+templn)=templ;//记录templn行的起点到数组templb
    }
    if(*(jgvalue+temph*bmpWidth+templ)==qybn && *(templb+templn)>=0){//如果当前值是0，且已经记录templn行的起点到数组templb
      *(temple+templn)=templ-1;  //结束，记录记录templn行的终点点到数组temple，记录到前一个是因为当期是0了，前一个是-1，才在区域内，0 是边界
      templn++;
    }


  }
  if(tempw2==tmax_l && *(templb+templn)>-1) {
    *(temple+templn)=tempw2;  //结束
    templn++;
  }
    if (tempw1>=0 && tempw2>=0 ){  //判断本行是否有和区域号相同的像素
    if(templn==0){  //中间为没有-1，情况一是只有边界
      for (tempi=tempw1;tempi<=tempw2;tempi++)
      {
        if(*(xsvalue+temph*bmpWidth+tempi)!=qybn && *(xsvalue+temph*bmpWidth+tempi)!=0)
        {
            *(xsvalue+temph*bmpWidth+tempi)=qybn;  //区域内填充
            *(jgvalue+temph*bmpWidth+tempi)=fillv;
            /////mmy
          ///////////////暂时去除  data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,temph,tempi,255,255,255);
            temparea++;
            ////////////////
        }
      }
    }else{
      for(int tempj=0;tempj<templn;tempj++){
        if(*(temple+tempj)<=tempw2)
        for (tempi=*(templb+tempj);tempi<=*(temple+tempj);tempi++)
        {
          *(xsvalue+temph*bmpWidth+tempi)=qybn; //区域内填充
          *(jgvalue+temph*bmpWidth+tempi)=fillv;
           /////mmy
///////////////暂时去除data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,temph,tempi,255,255,255);
temparea++;

        ////////////////
        }
      }
    }
  }
  }
  delete[] templb; templb=NULL;
  delete[] temple;  temple=NULL;
  return temparea;
}
*/
//
//对区域进行填充
//返回区域面积
//jg tx数组依赖
//操作xs，jg，fb
int CIMGRecognition::data_check_fill_bj(unsigned char*& pBmpBuf, int qybn, int fillv, int temp_close, int* temp_bj_h, int* temp_bj_l, int*& xsvalue, int*& fbvalue, int*& jgvalue, int*& txvalue, int bmpWidth, int bmpHeight, int& tmin_h, int& tmax_h, int& tmin_l, int& tmax_l, bool debug)
{
  //int lineByte=(bmpWidth * biBitCount/8+3)/4*4; //mmy
  int temph, templ, tempw1, tempw2, templn, tempflag;
  int* templb, * temple;
  templb = NULL;
  temple = NULL;
  templb = new int[bmpHeight];
  temple = new int[bmpHeight];
  int temparea = 0;
  int nMinH = bmpHeight, nMinL = bmpWidth, nMaxH = 0, nMaxL = 0;
  //for (temph=0;temph<=temp_close-1;temph++)
  //   *(jgvalue+(*(temp_bj_h+temph))*bmpWidth+*(temp_bj_l+temph))=fillv;

  for (temph = tmin_h; temph <= tmax_h; temph++)//行循环
  {
    tempw1 = -1;
    tempw2 = -1;
    tempflag = 0;
    templn = 0;
    for (int j = 0; j < bmpHeight; j++) {//将两个列数组初始化
      *(templb + j) = -1;
      *(temple + j) = -1;
    }
    for (templ = tmin_l; templ <= tmax_l; templ++)//列循环
    {
      //mmy
      //if(debug)data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,temph,templ,0,0,0);//_20140708

      //*(pBmpBuf+(bmpHeight-1-temph)*lineByte+templ*(biBitCount/8))=0;
      //		*(pBmpBuf+(bmpHeight-1-temph)*lineByte+templ*(biBitCount/8)+1)=0;
      //		*(pBmpBuf+(bmpHeight-1-temph)*lineByte+templ*(biBitCount/8)+2)=0;
      //如果是边界
      if (*(jgvalue + temph * bmpWidth + templ) == qybn) //如果当前jg的值是传入的区域的编号，说明是边界
      {
        *(fbvalue + temph * bmpWidth + templ) = qybn;//边界填充
        *(xsvalue + temph * bmpWidth + templ) = qybn;//区域内填充

        if (nMinH > temph) nMinH = temph;
        if (nMaxH < temph) nMaxH = temph;
        if (nMinL > templ) nMinL = templ;
        if (nMaxL < templ) nMaxL = templ;
        //*(jgvalue+temph*bmpWidth+templ)=fillv;//mmy去掉置换jg，保留jg的原貌
        ///////////////////////////////////////////mmy
        if (debug)data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, temph, templ, 255, 255, 255);
        temparea++;

        /////////////////////////////////////////////
        if (tempw1 == -1)
          tempw1 = templ;   //记录本行第一次边界所在列号
        tempw2 = templ;   //记录最后一个相同的列号
        continue;
      }
      else if (tempw1 >= 0)//如果不是边界但已找到边界1
      {
        if (templ == tempw2 + 1)//判断当前列是否就在边界后面，与边界接壤
        {

          switch (*(txvalue + (temph)* bmpWidth + templ - 1))//判断当前点的前一列的点边界tx方向标记
          {
          case 1://上
            tempflag = -1;
            break;
          case 2://右
            if (*(jgvalue + (temph - 1) * bmpWidth + templ - 1) == qybn && temph > tmin_h && templ > 0)//前边上方是区域
            {
              tempflag = -1;
            }
            else if (*(jgvalue + (temph + 1) * bmpWidth + templ - 1) == qybn && temph < tmax_h && templ>0)//前边x方是区域
            {
              tempflag = 0;
            }
            break;
          case 3://下
            tempflag = 0;
            break;
          case 4://左
            if (debug)AfxMessageBox(_T("unbelievable"));
            break;
          default:
            break;
          }
          if (-1 == tempflag)//如果-1，说明是区域内，需要填充
          {
            *(xsvalue + temph * bmpWidth + templ) = qybn;  //区域内填充
            *(jgvalue + temph * bmpWidth + templ) = fillv;
            /////mmy
            if (debug)data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, temph, templ, 100, 100, 100);
            temparea++;
          }
        }
        else//与边界不接壤
        {
          if (-1 == tempflag)
          {
            *(xsvalue + temph * bmpWidth + templ) = qybn;  //区域内填充
            *(jgvalue + temph * bmpWidth + templ) = fillv;
            /////mmy
            if (debug)data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, temph, templ, 100, 100, 100);
            temparea++;
          }
        }
      }
    }
  }

  tmin_h = nMinH;
  tmax_h = nMaxH;
  tmin_l = nMinL;
  tmax_l = nMaxL;
  delete[] templb; templb = NULL;
  delete[] temple;  temple = NULL;
  return temparea;
}
//temph、templ相对于图像显示的左上角
void CIMGRecognition::data_bmp_set_color(unsigned char*& pBmpBuf, int biBitCount, int bmpWidth, int bmpHeight, int temph, int templ, unsigned char R, unsigned char G, unsigned char B)
{
  if (temph < 0 || templ < 0 || temph >= bmpHeight || templ >= bmpWidth)
    return;
  int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4; //mmy
  switch (biBitCount)
  {
  case 8:
    *(pBmpBuf + (bmpHeight - 1 - temph) * lineByte + templ * (biBitCount / 8)) = (G + B + R) / 3;
    break;
  case 24:
    *(pBmpBuf + (bmpHeight - 1 - temph) * lineByte + templ * (biBitCount / 8)) = B;
    *(pBmpBuf + (bmpHeight - 1 - temph) * lineByte + templ * (biBitCount / 8) + 1) = G;
    *(pBmpBuf + (bmpHeight - 1 - temph) * lineByte + templ * (biBitCount / 8) + 2) = R;
    break;
  default:
    break;
  }

}

//temph、templ相对于图像显示的左下角
int CIMGRecognition::ReadBmpGray(unsigned char*& pBmpBuf, int biBitCount, int bmpWidth, int bmpHeight, int temph, int templ)
{
  if (temph < 0 || templ < 0 || temph >= bmpHeight || templ >= bmpWidth)
    return -1;
  int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4; //mmy
  switch (biBitCount)
  {
  case 8:
    return *(pBmpBuf + temph * lineByte + templ * (biBitCount >> 3));
    break;
  case 24:
    return (*(pBmpBuf + (temph)* lineByte + templ * (biBitCount >> 3)) +
      *(pBmpBuf + (temph)* lineByte + templ * (biBitCount >> 3) + 1) +
      *(pBmpBuf + (temph)* lineByte + templ * (biBitCount >> 3) + 2)) / 3;
    break;
  default:
    return -1;
    break;
  }

}
//判断圆形并提取圆形特征
//返回5，不是正方形
//返回4，线段没找全
//返回3，凸点
//返回2，凹点
//返回1，顶点数多了
//返回0，正常
//函数最后将特征量保存到全局结构
int CIMGRecognition::data_check_feature_circleTMP(unsigned char*& pBmpBuf, int biBitCount, int bmpWidth, int bmpHeight, int bj_num, int* bj_h, int* bj_l, int qybn, int*& xsvalue, int*& fbvalue, int*& jgvalue, int*& txvalue, int outpointallowlen, int defectallowlen, int& tmin_h, int& tmax_h, int& tmin_l, int& tmax_l)
{
  double centreh, centrel;//存储边界中心行列
  int* pnCriBjNum = NULL, nCriNum = 0;
  double templen = 0, minlen = 0, maxlen = 0, oklen = 0, radious = 0;//中间变量，用于计算
  int nNum;//计数和不在圈上点计数
  int criticalvalue = 0;//判断圆的允许半径偏差

  //centreh=(tmax_h+tmin_h);

  for (int i = 0; i < bj_num; i++)//循环边界算大致中心
  {
    SortAvg(i + 1, *(bj_h + i), centreh);
    SortAvg(i + 1, *(bj_l + i), centrel);
  }
  if (g_stu_square.debug)data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, (int)centreh, (int)centrel, 255, 50, 50);
  do {
    criticalvalue++;
    g_stu_square.rcglineok = 0;
    int nOKNum = bj_num * 5 / 360;
    nNum = 0;

    for (int i = 0; i < bj_num; i++)//
    {
      templen = LenFromP1toP2(centrel, centreh, *(bj_l + i), *(bj_h + i));

      if (0 == nNum)//如果i==0
      {
        minlen = templen;
        maxlen = templen;
        nNum++;
        continue;
      }
      if (minlen > templen)//如果minlen比templen大
        minlen = templen;
      if (maxlen < templen)//如果maxlen比templen小
        maxlen = templen;
      nNum++;
      if (nNum > nOKNum)
      {
        if (maxlen - minlen > criticalvalue)
        {


          continue;
        }
        else
        {
          oklen = (minlen + maxlen) / 2;
          minlen = oklen;
          maxlen = oklen;

          break;
        }
      }

    }
  } while (oklen == 0 && criticalvalue <= bj_num >> 7);//如果定点数不是4尝试改变找点参数从新找过
  //以上找点

  if (oklen == 0)
    return 1;//形状有问题
  pnCriBjNum = new int[bj_num]; nCriNum = 0;
  do {
    criticalvalue++;
    nNum = 0;
    for (int i = 0; i < bj_num; i++)//
    {
      templen = LenFromP1toP2(centrel, centreh, *(bj_l + i), *(bj_h + i));
      if (abs(oklen - templen) > criticalvalue)//不在圈上
        nNum++;
      else
      {
        *(pnCriBjNum + nCriNum) = i;
        SortAvg(nCriNum + 1, (int)templen, radious);
        nCriNum++;
      }
    }
  } while (nNum > bj_num * 10 / 360 && criticalvalue < bj_num >> 7);
  if (nNum > bj_num * 10 / 360)
  {

    delete[] pnCriBjNum; pnCriBjNum = NULL;
    return 2;
  }
  g_stu_square.pnWth[g_stu_square.nN] = (int)radious;
  g_stu_square.pnZPX[g_stu_square.nN] = (int)centrel;
  g_stu_square.pnZPY[g_stu_square.nN] = (int)centreh;
  //保存到结构
  if (g_stu_square.bJudgeFeature)
  {
    if (g_stu_square.pnWth[g_stu_square.nN] > (stuRef.Rad - stuRef.Dev) && g_stu_square.pnWth[g_stu_square.nN] < (stuRef.Rad + stuRef.Dev))
      g_stu_square.nN++;
  }
  else
    g_stu_square.nN++;
  //
  if (g_stu_square.debug)
    for (int j = tmin_l; j < tmax_l; j++)//
    {
      if (fabs(centrel - j) <= radious)//用于图像上显示
      {
        data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, int(sqrt(pow(radious, 2) - pow((j - centrel), 2)) + centreh), j, 0, 255, 0);
        data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, int(-sqrt(pow(radious, 2) - pow((j - centrel), 2)) + centreh), j, 0, 255, 0);
      }
    }
  if (g_stu_square.debug)
    for (int j = tmin_h; j < tmax_h; j++)//
    {
      if (fabs(centreh - j) <= radious)
      {
        data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, j, int(sqrt(pow(radious, 2) - pow((j - centreh), 2)) + centrel), 0, 255, 0);
        data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, j, int(-sqrt(pow(radious, 2) - pow((j - centreh), 2)) + centrel), 0, 255, 0);
      }
    }
  delete[] pnCriBjNum; pnCriBjNum = NULL;
  return 0;
}
//无分割
//判断圆形并提取圆形特征简化版
//返回非0，不正常1.可能不是圆 2.形状不规范
//返回0，正常
//函数最后将特征量保存到全局结构
int CIMGRecognition::data_check_feature_circle(unsigned char*& pBmpBuf, int biBitCount, int bmpWidth, int bmpHeight, int bj_num, int* bj_h, int* bj_l, int qybn, int*& xsvalue, int*& fbvalue, int*& jgvalue, int*& txvalue, int outpointallowlen, int defectallowlen, int& tmin_h, int& tmax_h, int& tmin_l, int& tmax_l)
{
  double centreh, centrel;//存储边界中心行列
  double templen = 0, minlen = 0, maxlen = 0, oklen = 0, radious = 0;//中间变量，用于计算
  int nNum;//计数和不在圈上点计数
  int criticalvalue = defectallowlen;//bj_num/100;//判断圆的允许半径偏差
  bool bMov = 0, bL = 0;
  centreh = double(tmax_h + tmin_h) / 2;
  centrel = double(tmax_l + tmin_l) / 2;
  maxlen = double(tmax_h - tmin_h);
  minlen = double(tmax_l - tmin_l);
  if (abs(maxlen - minlen) > criticalvalue && (abs(maxlen - minlen) < (minlen + maxlen) / 4))
  {
    radious = (maxlen > minlen) ? maxlen / 2 : minlen / 2;
    bMov = 1;
    bL = (maxlen > minlen) ? 1 : 0;
    if (bL)
      centrel = tmin_l;
    else
      centreh = tmin_h;
  }
  else if (abs(maxlen - minlen) <= criticalvalue)	//
    radious = (maxlen + minlen) / 4;
  else
    return 1;
  minlen = radious;
  maxlen = radious;

  do
  {
    if (g_stu_square.debug)data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, (int)centreh, (int)centrel, 255, 50, 50);

    g_stu_square.rcglineok = 0;
    int nOKNum = bj_num * 2 / 5;//最大允许不在范围内的点数
    nNum = 0;//不在范围内的点数

    for (int i = 0; i < bj_num; i++)//
    {
      templen = LenFromP1toP2(centrel, centreh, *(bj_l + i), *(bj_h + i));
      if (abs(radious - templen) > criticalvalue)
        nNum++;

      if (nNum > nOKNum)
      {
        if (!bMov)
          return 2;
        else
        {
          if (bL)
          {
            centrel++;
            if (centrel >= tmax_l)
              return 2;
          }
          else
          {
            centreh++;
            if (centreh >= tmax_h)
              return 2;
          }
          break;
        }
      }
      else if (i == bj_num - 1)
      {
        bMov = 0;
        break;
      }

    }

  } while (centrel < tmax_l && centreh < tmax_h && bMov);
  g_stu_square.pnWth[g_stu_square.nN] = (int)radious;
  g_stu_square.pnZPX[g_stu_square.nN] = (int)centrel;
  g_stu_square.pnZPY[g_stu_square.nN] = (int)centreh;
  //保存到结构
  if (g_stu_square.bJudgeFeature)
  {
    if (g_stu_square.pnWth[g_stu_square.nN] > (stuRef.Rad - stuRef.Dev) && g_stu_square.pnWth[g_stu_square.nN] < (stuRef.Rad + stuRef.Dev))
      g_stu_square.nN++;
  }
  else
    g_stu_square.nN++;
  //
  if (g_stu_square.debug)
    for (int j = tmin_l; j < tmax_l; j++)//
    {
      if (fabs(centrel - j) <= radious)//用于图像上显示
      {
        data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, int(sqrt(pow(radious, 2) - pow((j - centrel), 2)) + centreh), j, 0, 255, 0);
        data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, int(-sqrt(pow(radious, 2) - pow((j - centrel), 2)) + centreh), j, 0, 255, 0);
      }
    }
  if (g_stu_square.debug)
    for (int j = tmin_h; j < tmax_h; j++)//
    {
      if (fabs(centreh - j) <= radious)
      {
        data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, j, int(sqrt(pow(radious, 2) - pow((j - centreh), 2)) + centrel), 0, 255, 0);
        data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, j, int(-sqrt(pow(radious, 2) - pow((j - centreh), 2)) + centrel), 0, 255, 0);
      }
    }
  return 0;
}
//分割,需在给予特征之后使用
//判断圆形并提取圆形特征简化版
//返回非0，不正常1.可能不是圆 2.形状不规范
//返回0，正常
//函数最后将特征量保存到全局结构
int CIMGRecognition::data_check_feature_circle_split(unsigned char*& pBmpBuf, int biBitCount, int bmpWidth, int bmpHeight
  , int bj_num, int* bj_h, int* bj_l, int* bj_fx, int qybn, int*& xsvalue, int*& fbvalue, int*& jgvalue, int*& txvalue
  , int outpointallowlen, int defectallowlen, int& tmin_h, int& tmax_h, int& tmin_l, int& tmax_l)
{
  if (bj_num < (stuRef.Rad << 2) || 0 == stuRef.Rad)
  {
    return 1;
  }
  double centreh, centrel;//存储边界中心行列
  double templen = 0, minlen = 0, maxlen = 0, oklen = 0, radious = 0;//中间变量，用于计算
  int nNum, maxi;//计数和不在圈上点计数
  int criticalvalue = defectallowlen;//bj_num/100;//判断圆的允许半径偏差
  int stepLen = stuRef.Rad >> 1;//一个向量的像素数
  int type = 0;
  vector lV = { 0,0 };//遍历时的局部临时向量
  centreh = double(tmax_h + tmin_h) / 2;
  centrel = double(tmax_l + tmin_l) / 2;
  maxlen = double(tmax_h - tmin_h);
  minlen = double(tmax_l - tmin_l);

  /*	if(abs(maxlen - minlen)<criticalvalue && abs( maxlen - minlen)/4>(stuRef.Rad -stuRef.Dev) && abs( maxlen - minlen)/4<(stuRef.Rad +stuRef.Dev))//直接判断圆
    {
      minlen=radious;
      maxlen=radious;

      if(g_stu_square.debug)data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,(int)centreh,(int)centrel,255,50,50);

      g_stu_square.rcglineok=0;
      int nOKNum=bj_num*2/5;//最大允许不在范围内的点数
      nNum=0;//不在范围内的点数

      for (int i=0;i<bj_num;i++)//
      {
        templen=LenFromP1toP2(centrel,centreh,*(bj_l+i),*(bj_h+i));
        if(abs(radious-templen)>criticalvalue)
          nNum++;

        if (nNum>nOKNum)
        {
          return 2;
        }
      }

    }
    else//通过判断边界找圆*/
  {
    type = 2;
    int tmpStep = 0;
    vector tmpV[100];//依次保存遍历的v
    int tmpfi[100] = { 0 }, fiN = 0, compareFiN = 0;//依次保存遍历的v第一个点的序号，v的角度和遍历编号
    double tmpAng[100] = { 0 };
    bool needReSta = 0;
    int unWanded = 0;//平行线次数
    for (int fi = 0; fi < bj_num; fi++)
    {
      tmpStep++;
      switch (*(bj_fx + fi))
      {
      case 1:
        lV.y--;
        break;
      case 2:
        lV.x++;
        break;
      case 3:
        lV.y++;
        break;
      case 4:
        lV.x--;
        break;
      default:
        break;
      }
      if (tmpStep > stepLen)//一个向量
      {
        tmpV[fiN].x = lV.x;//保存到零时向量
        tmpV[fiN].y = lV.y;
        lV.x = 0;
        lV.y = 0;


        tmpStep = 0;
        int iCW;
        double dAngle;

        if (0 == fiN)
        {
          vector zeroV = { 1,0 };
          iCW = VectorCW(zeroV, tmpV[fiN]);
          dAngle = VectorAngle(zeroV, tmpV[fiN]);
        }
        else
        {
          iCW = VectorCW(tmpV[fiN - 1], tmpV[fiN]);
          dAngle = VectorAngle(tmpV[fiN - 1], tmpV[fiN]);
        }

        tmpAng[fiN] = dAngle;

        if (0 == fiN)//如果是第一个向量
        {
          //fiN ++;//编号指向下一个向量
        }
        else if (-1 == iCW)//趋势顺时针
        {
          if (dAngle < 15)
          {
            unWanded++;
          }
          else if (dAngle > 40 || unWanded >= 2)//|| unWanded ==1&& dAngle>90)//角度太大
          {
            needReSta = 1;
          }
          else
          {
            unWanded = 0;

            //fiN ++;
          }

          if (fiN > 3 && 0 == needReSta && unWanded < 2)
          {
            int nTmpCW = VectorCW(tmpV[0], tmpV[fiN]);
            double dTmpAng = VectorAngle(tmpV[0], tmpV[fiN]);
            if (1 == nTmpCW || -1 == nTmpCW && dTmpAng > 160 || 0 == nTmpCW && dTmpAng == 180)
            {
              tmpfi[fiN + 1] = fi;

              //找中心
              centrel = *(bj_l + tmpfi[0]);//临时做第一点
              centreh = *(bj_h + tmpfi[0]);
              maxlen = 0; maxi = 0; nNum = 0;
              for (int fii = tmpfi[fiN + 1]; fii > tmpfi[0]; fii--)
              {
                templen = LenFromP1toP2(centrel, centreh, *(bj_l + fii), *(bj_h + fii));
                if (templen > maxlen)
                {
                  maxlen = templen;
                  maxi = fii;
                  nNum = 0;
                }
                else
                {
                  nNum++;
                  if (nNum > stepLen)
                  {
                    centrel = (centrel + *(bj_l + maxi)) / 2;
                    centreh = (centreh + *(bj_h + maxi)) / 2;
                    radious = maxlen / 2;
                    break;
                  }
                }
              }
              //保存到结果数组
              g_stu_square.pnWth[g_stu_square.nN] = (int)radious;
              g_stu_square.pnZPX[g_stu_square.nN] = (int)centrel;
              g_stu_square.pnZPY[g_stu_square.nN] = (int)centreh;
              //保存到结构
              bool lenOK = 0;
              //if(g_stu_square.bJudgeFeature)
              {
                if (g_stu_square.pnWth[g_stu_square.nN] > (stuRef.Rad - stuRef.Dev) && g_stu_square.pnWth[g_stu_square.nN] < (stuRef.Rad + stuRef.Dev))
                {
                  g_stu_square.nN++;
                  lenOK = 1;
                }
              }
              /*else
              {
                g_stu_square.nN++;
                lenOK = 1;
              }*/
              //去除重复的中心
              if (1 == lenOK)
              {
                for (int i = g_stu_square.nN - 2; i >= 0; i--)
                {
                  if (LenFromP1toP2(g_stu_square.pnZPX[i], g_stu_square.pnZPY[i], centrel, centreh) < (stuRef.Rad << 1))
                  {
                    g_stu_square.nN--;
                    lenOK = 0;
                    break;
                  }
                  //if(sqrt(pow(double(g_stu_square.pnZPX[i]-xt),2)+pow(double(g_stu_square.pnZPY[i]-yt),2))<(stuRef.Rad)) //点击位置离片的粗略中心相距小于s_sort.allowable_devision，这么做主要可以当点不准时设置调节
                }
                //在图像上做标记
                if (1 == lenOK)
                {
                  if (g_stu_square.debug)
                    for (int j = tmin_l; j < tmax_l; j++)//
                    {
                      if (fabs(centrel - j) <= radious)//用于图像上显示
                      {
                        data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, int(sqrt(pow(radious, 2) - pow((j - centrel), 2)) + centreh), j, 0, 255, 0);
                        data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, int(-sqrt(pow(radious, 2) - pow((j - centrel), 2)) + centreh), j, 0, 255, 0);
                      }
                    }
                  if (g_stu_square.debug)
                    for (int j = tmin_h; j < tmax_h; j++)//
                    {
                      if (fabs(centreh - j) <= radious)
                      {
                        data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, j, int(sqrt(pow(radious, 2) - pow((j - centreh), 2)) + centrel), 0, 255, 0);
                        data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, j, int(-sqrt(pow(radious, 2) - pow((j - centreh), 2)) + centrel), 0, 255, 0);
                      }
                    }
                  type = 0;
                }

              }
              ///////////////////////////////////////越界检查
              if (g_stu_square.nN == 20)
              {
                return type;
              }
              needReSta = 1;
              //break;
            }
          }
        }
        else if (1 == iCW)//趋势逆时针
        {
          if (dAngle < 10)
          {
            //fiN ++;
            unWanded++;
          }
          else//角度太大
          {
            needReSta = 1;
          }
        }
        else
        {
          //fiN ++;
          unWanded++;
        }

        if (1 == needReSta)//如果需要重新开始找圆的趋势
        {
          unWanded = 0;
          needReSta = 0;
          vector breakV;
          breakV.x = tmpV[fiN].x + tmpV[fiN - 1].x;
          breakV.y = tmpV[fiN].y + tmpV[fiN - 1].y;
          point breakPoint, vPoint;
          double farPL = 0, farPi = 0;
          vPoint.x = *(bj_l + fi);
          vPoint.y = *(bj_h + fi);
          for (int fii = tmpfi[fiN - 1]; fii < fi; fii++)//重最远点重新匹配
          {

            breakPoint.x = *(bj_l + fii);
            breakPoint.y = *(bj_h + fii);
            templen = Distance(breakV, breakPoint, vPoint);
            if (templen > farPL)
            {
              farPL = templen;
              farPi = fii;
            }
          }
          fiN = 0;//重新开始
          fi = farPi;
          if (g_stu_square.debug)
          {
            data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, *(bj_h + fi), *(bj_l + fi), 0, 0, 255);
          }

        }
        else
        {
          fiN++;
          if (fiN == 100)
          {
            return type;
          }
        }

        tmpfi[fiN] = fi;//用于提取下一个向量的起始坐标
      }
      if (fi == bj_num - 1)
      {
        return type;
      }
      /*if (VectorLen(lV)==stuRef.Rad)
      {

      }
      else
      {
      }*/
    }
    //找中心
  /*	centrel = *(bj_l+ tmpfi[0]);
    centreh = *(bj_h+ tmpfi[0]);
    maxlen = 0;maxi=0;nNum =0;
    for (int fi = tmpfi[fiN + 1]; fi > tmpfi[0] ; fi --)
    {
      templen=LenFromP1toP2(centrel,centreh,*(bj_l+fi),*(bj_h+fi));
      if (templen > maxlen)
      {
        maxlen = templen;
        maxi = fi;
        nNum = 0;
      }
      else
      {
        nNum++;
        if (nNum > stepLen)
        {
          centrel = (centrel + *(bj_l+ maxi))/2;
          centreh = (centreh + *(bj_h+ maxi))/2;
          radious = maxlen/2;
          break;
        }
      }
    }*/
  }

  /*
  g_stu_square.pnWth[g_stu_square.nN]=(int)radious;
  g_stu_square.pnZPX[g_stu_square.nN]=(int)centrel;
  g_stu_square.pnZPY[g_stu_square.nN]=(int)centreh;
  //保存到结构
  if(g_stu_square.bJudgeFeature)
  {
    if(g_stu_square.pnWth[g_stu_square.nN]>(stuRef.Rad -stuRef.Dev) && g_stu_square.pnWth[g_stu_square.nN]<(stuRef.Rad +stuRef.Dev))
      g_stu_square.nN++;
  }
  else
    g_stu_square.nN++;
  //
  if(g_stu_square.debug)
    for(int j=tmin_l;j<tmax_l;j++)//
    {
      if(fabs(centrel-j)<=radious)//用于图像上显示
      {
        data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,int(sqrt(pow(radious,2)-pow((j-centrel),2))+centreh),j,0,255,0);
        data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,int(-sqrt(pow(radious,2)-pow((j-centrel),2))+centreh),j,0,255,0);
      }
    }
    if(g_stu_square.debug)
      for(int j=tmin_h;j<tmax_h;j++)//
      {
        if(fabs(centreh-j)<=radious)
        {
          data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,j,int(sqrt(pow(radious,2)-pow((j-centreh),2))+centrel),0,255,0);
          data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,j,int(-sqrt(pow(radious,2)-pow((j-centreh),2))+centrel),0,255,0);
        }
      }*/
  return 0;
}
//提取多边形区域边长
//返回5，不是正方形
//返回4，线段没找全
//返回3，凸点
//返回2，凹点
//返回1，顶点数多了
//返回0，正常
//函数最后将特征量保存到全局结构
int CIMGRecognition::data_check_feature(unsigned char*& pBmpBuf, int biBitCount, int bmpWidth, int bmpHeight, int bj_num, int* bj_h, int* bj_l, int qybn, int*& xsvalue, int*& fbvalue, int*& jgvalue, int*& txvalue, int outpointallowlen/*判断直线与点的最大距离*/, int defectallowlen/*判断缺陷in or out用*/)
{
  double centreh, centrel;//存储边界中心行列
  int pi_point_h[50];//折点行坐标
  int pi_point_l[50];//折点列坐标
  int pi_point_no[50];//折点位置
  int pi_point_trend[50];//折点的趋势
  int beginno = -1;//开始检查的第一个折点
  bool trend = 0;//趋势，1上升，0下降，一开始置0
  int i_point_num = 0;//折点数
  double templen, minlen, maxlen;//中间变量，用于计算折点
  int minno, maxno;//配合记录上面的点在bj数组中的位置
  int minnum, maxnum;//记录找到min点，max点的计数值，当max超过criticalvalue，就表示找到min点；如果min超过criticalvalue，就表示找到max点
  int criticalvalue = bj_num / 360;//反向趋势临界点数，固定值
  //int outpointallowlen=4;//允许的点到直线的距离
  do {
    if (!g_stu_square.debug && criticalvalue < (bj_num >> 4))
    {
      if (criticalvalue < (bj_num >> 7))
        criticalvalue = (bj_num >> 7);
      else if (criticalvalue < (bj_num >> 6))
        criticalvalue = (bj_num >> 6);
      else if (criticalvalue < (bj_num >> 5))
        criticalvalue = (bj_num >> 5);
      else
        criticalvalue = (bj_num >> 4);
    }
    else
      criticalvalue++;
    beginno = -1;//开始检查的第一个折点
    trend = 0;//趋势，1上升，0下降，一开始置0
    i_point_num = 0;//折点数
    g_stu_square.rcglineok = 0;
    g_stu_square.bPN[g_stu_square.nN] = 0;
    for (int i = 0; i < bj_num; i++)//循环边界算大致中心
    {
      SortAvg(i + 1, *(bj_h + i), centreh);
      SortAvg(i + 1, *(bj_l + i), centrel);
    }

    for (int i = 0; i < bj_num; i++)//循环边界找一开始的总体趋势
    {
      templen = LenFromP1toP2(centrel, centreh, *(bj_l + i), *(bj_h + i));

      if (!i)//如果i==0
      {
        minlen = templen; minno = i; minnum = 0;
        maxlen = templen; maxno = i; maxnum = 0;
        continue;
      }

      if (minlen > templen)//如果minlen比templen大，下降趋势
      {
        minlen = templen; minno = i; minnum++;
      }
      if (maxlen < templen)//如果maxlen比templen小，上升趋势
      {
        maxlen = templen; maxno = i; maxnum++;
      }

      if (minnum > criticalvalue || maxnum > criticalvalue)//如果大于临界值
      {
        trend = maxnum > minnum ? 1 : 0;
        break;
      }
    }

    for (int i = 0; i<bj_num && i>beginno || i < beginno; i = (bj_num == i + 1) ? 0 : (i + 1))//i*pow(double(bj_num-1-i),0)+1)//i*=pow(double(bj_num- ++i),0)会无效，因++i先于=号，导致i提前溢出
    //for (int i=0;i<bj_num;i++)//循环边界找一个标准折点作为开始
    {
      if (bj_num == i + 1 && -1 == beginno)//防止圆形导致该循环找不到起始趋势而不能退出
        break;
      templen = LenFromP1toP2(centrel, centreh, *(bj_l + i), *(bj_h + i));

      if (!i && beginno == -1)//如果i==0
      {
        minlen = templen; minno = i; minnum = 0;
        maxlen = templen; maxno = i; maxnum = 0;
        continue;
      }

      if (minlen > templen)//如果minlen比templen大，下降趋势
      {
        if (trend)
        {
          minlen = templen; minno = i; minnum++;
        }
        else
        {
          minlen = templen; minno = i; minnum = 0;
          maxlen = templen; maxno = i; maxnum = 0;
        }

      }
      if (maxlen < templen)//如果maxlen比templen小，上升趋势
      {

        if (trend)
        {
          minlen = templen; minno = i; minnum = 0;
          maxlen = templen; maxno = i; maxnum = 0;
        }
        else
        {
          maxlen = templen; maxno = i; maxnum++;
        }
      }

      if (minnum > criticalvalue || maxnum > criticalvalue)//如果大于临界值
      {
        pi_point_no[i_point_num] = maxnum > minnum ? minno : maxno;
        if (-1 == beginno)
        {
          beginno = pi_point_no[i_point_num];
        }
        if (minnum > criticalvalue)
        {

          pi_point_h[i_point_num] = *(bj_h + pi_point_no[i_point_num]);//折点行坐标
          pi_point_l[i_point_num] = *(bj_l + pi_point_no[i_point_num]);//折点列坐标
          pi_point_trend[i_point_num] = trend;//折点的趋势,短的0，远的1；
          i_point_num++;
        }
        trend = !trend;

        minlen = templen; minno = i; minnum = 0;
        maxlen = templen; maxno = i; maxnum = 0;

        //break;
      }
    }
  } while (i_point_num != 4 && criticalvalue < bj_num >> 4);//如果定点数不是4尝试改变找点参数从新找过
  //以上找点
  if (g_stu_square.debug)
    for (int i = 0; i < i_point_num; i++)//特征之一：顶点
      data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, pi_point_h[i], pi_point_l[i], 0, 255, 0);
  if (i_point_num != 4)
  {
    return 1;//形状有问题
  }
  double* pd_line_a, * pd_line_b, * pd_line_c;
  int successequationnum = 0, outs = 0, * i_out_no = NULL, * pnOutLnNo = NULL;//outs总的不在线段上的点数
  double* i_out_len = NULL;
  i_out_no = new int[bj_num]; //最后是边界的凹凸区域的位置编号，当片重叠时，所有点都有可能在边界外，所有该数组不能比bjnum小
  pnOutLnNo = new int[bj_num];//保存每个out点是在哪条线段上
  i_out_len = new double[bj_num]; //最后是边界的对应的距离
  pd_line_a = new double[4];
  pd_line_b = new double[4];
  pd_line_c = new double[4];
  for (int i = 0; i < 4; i++)//拟合线段
  {
    //g_stu_square.i_maxlen_defect=0;
    int startno, endno, totalouts = 0;//,totalouts总的不在当前线段上的点数
    double a, b, c, lens;//ax+by+c=0,lens点到直线距离的累加
    int nexti = (i == 3) ? 0 : i + 1;

    if (pi_point_no[nexti] > pi_point_no[i])//
    {
      startno = (pi_point_no[nexti] + pi_point_no[i]) >> 1;
      endno = pi_point_no[nexti] - 1;
      for (; startno > pi_point_no[i] && endno < pi_point_no[nexti]; startno--, endno--)//找符合直线
      {
        if (!CalcEquationABC(bj_l[startno], bj_h[startno], bj_l[endno], bj_h[endno], a, b, c))
          continue;
        outs -= totalouts;
        totalouts = 0;
        lens = 0;
        for (int j = pi_point_no[i]; j < pi_point_no[nexti]; j++)//边界点与该直线匹配
        {
          double templen = LenFromPtoL(bj_l[j], bj_h[j], a, b, c);
          if (templen > outpointallowlen)
          {
            i_out_no[outs] = j;
            pnOutLnNo[outs] = i;
            i_out_len[outs] = templen;
            outs++;
            totalouts++;
            //if(g_stu_square.i_maxlen_defect<templen)//保存最大距离
              //g_stu_square.i_maxlen_defect=templen;
          }
          else
          {
            lens += templen;
          }
          if (outs > bj_num || totalouts > (pi_point_no[nexti] - pi_point_no[i]) >> 1 || (int)lens > (((pi_point_no[nexti] - pi_point_no[i] + 1 - totalouts) * (outpointallowlen) >> 2) + ((pi_point_no[nexti] - pi_point_no[i] + 1 - totalouts) >> 6)))
            break;
        }
        if (outs > bj_num)
          break;
        if (totalouts > (pi_point_no[nexti] - pi_point_no[i]) >> 1 || (int)lens > (((pi_point_no[nexti] - pi_point_no[i] + 1 - totalouts) * (outpointallowlen) >> 2) + ((pi_point_no[nexti] - pi_point_no[i] + 1 - totalouts) >> 6)))
          continue;
        else
        {
          pd_line_a[i] = a, pd_line_b[i] = b, pd_line_c[i] = c;
          g_stu_square.d_a[i] = a;//保存
          g_stu_square.d_b[i] = b;
          g_stu_square.d_c[i] = c;
          /*for(int j=1;j<bmpWidth-1;j++)
            data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,0==b?j:int(-(a*j+c)/b),0==b?-c/a:j,255,255,0);
          for(int j=0;j<totalouts;j++)
          {
            data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,*(bj_h+i_out_no[j]),*(bj_l+i_out_no[j]),0,0,255);
            double templen=LenFromPtoL(*(bj_l+i_out_no[j]),*(bj_h+i_out_no[j],a,b,c);
          }
          */successequationnum++;
          break;
        }
      }


    }
    else////////////////////////pi_point_no[nexti]<pi_point_no[i]
    {

      startno = pi_point_no[i] + ((pi_point_no[nexti] + bj_num - pi_point_no[i]) >> 1);//右移优先级低
      endno = pi_point_no[nexti] - 1;
      if (startno >= bj_num)
        startno = startno - bj_num;
      if (endno < 0)
        endno = bj_num + endno;
      for (; (startno > pi_point_no[i] || startno < pi_point_no[nexti]) && (endno > pi_point_no[i] || endno < pi_point_no[nexti]);
        startno = (0 == startno) ? bj_num - 1 : (startno - 1), endno = (0 == endno) ? bj_num - 1 : (endno - 1))//找符合直线
      {
        if (!CalcEquationABC(bj_l[startno], bj_h[startno], bj_l[endno], bj_h[endno], a, b, c))
          continue;
        outs -= totalouts;
        totalouts = 0;
        lens = 0;
        for (int j = pi_point_no[i];
          j < bj_num && j >= pi_point_no[i] || j <= pi_point_no[nexti];
          j = (bj_num == j + 1) ? 0 : (j + 1))//边界点与该直线匹配
        {
          double templen = LenFromPtoL(bj_l[j], bj_h[j], a, b, c);
          if (templen > outpointallowlen)//是否超过规定的距离
          {
            i_out_no[outs] = j;
            pnOutLnNo[outs] = i;
            i_out_len[outs] = templen;
            outs++;
            totalouts++;
            //if(g_stu_square.i_maxlen_defect<templen)//保存最大距离
            //	g_stu_square.i_maxlen_defect=templen;
          }
          else
          {
            lens += templen;
          }
          if (outs > bj_num || totalouts > (pi_point_no[nexti] + bj_num - pi_point_no[i]) >> 1 || (int)lens > (((pi_point_no[nexti] + bj_num - pi_point_no[i] + 1 - totalouts) * (outpointallowlen) >> 2) + ((pi_point_no[nexti] + bj_num - pi_point_no[i] + 1 - totalouts) >> 6) + ((pi_point_no[nexti] + bj_num - pi_point_no[i] + 1 - totalouts) >> 8)))//如果不考虑识别率最后一个+去掉
            break;
        }
        if (outs > bj_num)
          break;
        if (totalouts > (pi_point_no[nexti] + bj_num - pi_point_no[i]) >> 1 || (int)lens > (((pi_point_no[nexti] + bj_num - pi_point_no[i] + 1 - totalouts) * (outpointallowlen) >> 2) + ((pi_point_no[nexti] + bj_num - pi_point_no[i] + 1 - totalouts) >> 6) + ((pi_point_no[nexti] + bj_num - pi_point_no[i] + 1 - totalouts) >> 8)))
          continue;
        else
        {
          pd_line_a[i] = a, pd_line_b[i] = b, pd_line_c[i] = c;
          g_stu_square.d_a[i] = a;//保存
          g_stu_square.d_b[i] = b;
          g_stu_square.d_c[i] = c;
          /*for(int j=1;j<bmpWidth-1;j++)
            data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,0==b?j:int(-(a*j+c)/b),0==b?-c/a:j,255,255,0);
          for(int j=0;j<totalouts;j++)
            data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,*(bj_h+i_out_no[j]),*(bj_l+i_out_no[j]),0,0,255);
          */successequationnum++;
          break;
        }
      }
    }
  }//拟合线段结束

  if (successequationnum != 4)//如果线段方程不等于4
  {
    delete[] i_out_no; i_out_no = NULL;
    delete[] pnOutLnNo; pnOutLnNo = NULL;
    delete[] i_out_len; i_out_len = NULL;
    delete[] pd_line_a; pd_line_a = NULL;
    delete[] pd_line_b; pd_line_b = NULL;
    delete[] pd_line_c; pd_line_c = NULL;
    return 4;
  }

  g_stu_square.rcglineok = 1;
  int nSquare = IsSquare(stuRef.SAg, stuRef.BAg);
  if (0 != nSquare)
  {

    delete[] i_out_no; i_out_no = NULL;
    delete[] pnOutLnNo; pnOutLnNo = NULL;
    delete[] i_out_len; i_out_len = NULL;
    delete[] pd_line_a; pd_line_a = NULL;
    delete[] pd_line_b; pd_line_b = NULL;
    delete[] pd_line_c; pd_line_c = NULL;
    return 5;//不是正方形

  }
  //计算该矩形边长//并保存到数组
  g_stu_square.side = 0;

  for (int m = 0; m < 4; m++)//计算边长
  {
    int n = (m == 0 ? 3 : m - 1);
    g_stu_square.d_len[m] = LenFromP1toP2(g_stu_square.d_point_x[m], g_stu_square.d_point_y[m], g_stu_square.d_point_x[n], g_stu_square.d_point_y[n]);
    g_stu_square.side += (int)g_stu_square.d_len[m];
  }
  //保存长宽到全局数组
  if (g_stu_square.d_len[0] > g_stu_square.d_len[1])
  {
    g_stu_square.pnLen[g_stu_square.nN] = (int(g_stu_square.d_len[0] + g_stu_square.d_len[2]) >> 1);
    g_stu_square.pnWth[g_stu_square.nN] = (int(g_stu_square.d_len[1] + g_stu_square.d_len[3]) >> 1);
  }
  else
  {
    g_stu_square.pnWth[g_stu_square.nN] = (int(g_stu_square.d_len[0] + g_stu_square.d_len[2]) >> 1);
    g_stu_square.pnLen[g_stu_square.nN] = (int(g_stu_square.d_len[1] + g_stu_square.d_len[3]) >> 1);
  }

  int sags = 0, crests = 0, unceasouts = 0, unceasallownum = bj_num >> 5;//sags凹角的数量,crests凸角数量
  //画边界线
  if (1)
  {
    if (g_stu_square.debug)
      for (int j = 1; j < bmpWidth - 2; j++)//特征之一：边的线x
      {
        for (int li = 0; li < 4; li++)
          if (fabs(pd_line_a[li]) < fabs(pd_line_b[li]))//用于图像上显示
            data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, 0 == pd_line_b[li] ? 0 : int(-(pd_line_a[li] * j + pd_line_c[li]) / pd_line_b[li]), 0 == pd_line_b[li] ? int(-pd_line_c[li] / pd_line_a[li]) : j, 255, 255, 0);
      }
    if (g_stu_square.debug)
      for (int j = 1; j < bmpHeight - 2; j++)//特征之一：边的线y
      {
        for (int li = 0; li < 4; li++)
          if (fabs(pd_line_a[li]) >= fabs(pd_line_b[li]))
            data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, 0 == pd_line_a[li] ? int(-pd_line_c[li] / pd_line_b[li]) : j, 0 == pd_line_a[li] ? 0 : int(-(pd_line_b[li] * j + pd_line_c[li]) / pd_line_a[li]), 255, 255, 0);
      }
  }
  //double l_maxlen=0;
  int nBeginj = 0, nEndj = 0, nUnceasNo = 0;//保存在ioutno数组中缺陷开始结束位置，共outs个缺陷
  bool bIn = 0;

  //特征之一：缺陷点，利用保存的缺陷点编号，从边界数据中取出分析，
  for (int j = 0; j < outs; j++)
  {

    if (g_stu_square.debug)data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, *(bj_h + i_out_no[j]), *(bj_l + i_out_no[j]), 0, 0, 100);
    //double templen=LenFromPtoL(*(bj_l+i_out_no[j]),*(bj_h+i_out_no[j],a,b,c);



    bool in = InOrOut(*(bj_l + i_out_no[j]), *(bj_h + i_out_no[j]), pd_line_a, pd_line_b, pd_line_c);
    if (j > 0)
      if (i_out_no[j] == i_out_no[j - 1] + 1)//编号延续上一编号，同一缺陷
      {
        if (i_out_len[j] > defectallowlen && i_out_len[j - 1] > defectallowlen)
        {

          if (g_stu_square.debug)data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, *(bj_h + i_out_no[j]), *(bj_l + i_out_no[j]), 0, 255, 255);
          unceasouts++;//用于判断in or out的点数

        }
        nUnceasNo++;//一个缺陷的点数
        nEndj = j;
        bIn = in;
      }
      else//一个缺陷结束
      {
        if (nUnceasNo > unceasallownum/2)
        {
          ///////////////////////////////////////////////////////匹配判断方向的折线START////////////////////////////////////////////////////////////////////////////////////////
          int startno, endno, totalouts = 0, outs = 0;//,totalouts总的不在当前线段上的点数
          double a, b, c, lens;//ax+by+c=0,lens点到直线距离的累加
          bool bSuccess = 0;
          if (i_out_no[nEndj] > i_out_no[nBeginj])//
          {
            startno = (i_out_no[nEndj] + i_out_no[nBeginj]) >> 1;
            endno = i_out_no[nEndj] - 1;
            for (; startno > i_out_no[nBeginj] && endno < i_out_no[nEndj]; startno--, endno--)//找符合直线
            {
              if (!CalcEquationABC(bj_l[startno], bj_h[startno], bj_l[endno], bj_h[endno], a, b, c))
                continue;
              outs -= totalouts;
              totalouts = 0;
              lens = 0;
              for (int j = i_out_no[nBeginj]; j < i_out_no[nEndj]; j++)//边界点与该直线匹配
              {
                double templen = LenFromPtoL(bj_l[j], bj_h[j], a, b, c);
                if (templen > outpointallowlen)
                {
                  outs++;
                  totalouts++;
                }
                else
                {
                  lens += templen;
                }
                if (outs > bj_num || totalouts > (i_out_no[nEndj] - i_out_no[nBeginj]) >> 1 || (int)lens > (((i_out_no[nEndj] - i_out_no[nBeginj] + 1 - totalouts) * (outpointallowlen) >> 2) + ((i_out_no[nEndj] - i_out_no[nBeginj] + 1 - totalouts) >> 6)))
                  break;
              }
              if (outs > bj_num)
                break;
              if (totalouts > (i_out_no[nEndj] - i_out_no[nBeginj]) >> 1 || (int)lens > (((i_out_no[nEndj] - i_out_no[nBeginj] + 1 - totalouts) * (outpointallowlen) >> 2) + ((i_out_no[nEndj] - i_out_no[nBeginj] + 1 - totalouts) >> 6)))
                continue;
              else
              {
                bSuccess = 1;
                break;
              }
            }


          }
          else//i_out_no[nEndj]<i_out_no[nBeginj]
          {

            startno = i_out_no[nBeginj] + ((i_out_no[nEndj] + bj_num - i_out_no[nBeginj]) >> 1);//右移优先级低
            endno = i_out_no[nEndj] - 1;
            if (startno >= bj_num)
              startno = startno - bj_num;
            if (endno < 0)
              endno = bj_num + endno;
            for (; (startno > i_out_no[nBeginj] || startno < i_out_no[nEndj]) && (endno > i_out_no[nBeginj] || endno < i_out_no[nEndj]);
              startno = (0 == startno) ? bj_num - 1 : (startno - 1), endno = (0 == endno) ? bj_num - 1 : (endno - 1))//找符合直线
            {
              if (!CalcEquationABC(bj_l[startno], bj_h[startno], bj_l[endno], bj_h[endno], a, b, c))
                continue;
              outs -= totalouts;
              totalouts = 0;
              lens = 0;
              for (int j = i_out_no[nBeginj];
                j < bj_num && j >= i_out_no[nBeginj] || j <= i_out_no[nEndj];
                j = (bj_num == j + 1) ? 0 : (j + 1))//边界点与该直线匹配
              {
                double templen = LenFromPtoL(bj_l[j], bj_h[j], a, b, c);
                if (templen > outpointallowlen)//是否超过规定的距离
                {
                  outs++;
                  totalouts++;
                }
                else
                {
                  lens += templen;
                }
                if (outs > bj_num || totalouts > (i_out_no[nEndj] + bj_num - i_out_no[nBeginj]) >> 1 || (int)lens > (((i_out_no[nEndj] + bj_num - i_out_no[nBeginj] + 1 - totalouts) * (outpointallowlen) >> 2) + ((i_out_no[nEndj] + bj_num - i_out_no[nBeginj] + 1 - totalouts) >> 6)))
                  break;
              }
              if (outs > bj_num)
                break;
              if (totalouts > (i_out_no[nEndj] + bj_num - i_out_no[nBeginj]) >> 1 || (int)lens > (((i_out_no[nEndj] + bj_num - i_out_no[nBeginj] + 1 - totalouts) * (outpointallowlen) >> 2) + ((i_out_no[nEndj] + bj_num - i_out_no[nBeginj] + 1 - totalouts) >> 6)))
                continue;
              else
              {
                bSuccess = 1;
                break;
              }
            }
          }

          //找所属角
          if (bSuccess)
          {
            int nLenNo1, nLenNo2;//保存折线所在边的编号
            if (pnOutLnNo[nBeginj] == pnOutLnNo[nEndj])
            {
              int nLenNo = pnOutLnNo[nBeginj];//保存第几条线段0~3
              int nLenNoNext = (3 == nLenNo) ? 0 : (nLenNo + 1);
              int nDstnBegin = i_out_no[nBeginj] >= pi_point_no[nLenNo] ? i_out_no[nBeginj] - pi_point_no[nLenNo] : i_out_no[nBeginj] + bj_num - pi_point_no[nLenNo];//这条折线开始点到该折线所在线段的开始点之间的距离
              int nDstnEnd = pi_point_no[nLenNoNext] >= i_out_no[nEndj] ? pi_point_no[nLenNoNext] - i_out_no[nEndj] : pi_point_no[nLenNoNext] + bj_num - i_out_no[nEndj];//这条折线结束点到该折线所在线段的结束点之间的距离
              if (nDstnBegin > nDstnEnd)
              {
                nLenNo1 = nLenNo;
                nLenNo2 = nLenNoNext;
              }
              else
              {
                nLenNo2 = nLenNo;
                nLenNo1 = (0 == nLenNo) ? 3 : (nLenNo - 1);
              }
            }
            else
            {
              nLenNo1 = pnOutLnNo[nBeginj];
              nLenNo2 = pnOutLnNo[nEndj];
            }


            switch (IsPositiveOrNegative(a, b, c, nLenNo1, nLenNo2))
            {
            case -1:
              //g_stu_square.bPN[g_stu_square.nN]=0;
              break;
            case 0:
              g_stu_square.bPN[g_stu_square.nN] = -1;
            //计算第一条边与x轴正向的夹角
            if(g_stu_square.bPN[g_stu_square.nN] != 0)
              g_stu_square.angreePN[g_stu_square.nN] = CalculateVectorAngle(g_stu_square.pnPX[g_stu_square.nN][nLenNo2]- g_stu_square.pnPX[g_stu_square.nN][nLenNo1],
                g_stu_square.pnPY[g_stu_square.nN][nLenNo2] - g_stu_square.pnPY[g_stu_square.nN][nLenNo1],
                1,0);
            g_stu_square.lenNo1PN[g_stu_square.nN] = nLenNo1;
            //
              break;
            case 1:
              g_stu_square.bPN[g_stu_square.nN] = 1;
            //计算第一条边与x轴正向的夹角
            if(g_stu_square.bPN[g_stu_square.nN] != 0)
              g_stu_square.angreePN[g_stu_square.nN] = CalculateVectorAngle(g_stu_square.pnPX[g_stu_square.nN][nLenNo2]- g_stu_square.pnPX[g_stu_square.nN][nLenNo1],
                g_stu_square.pnPY[g_stu_square.nN][nLenNo2] - g_stu_square.pnPY[g_stu_square.nN][nLenNo1],
                1,0);
            g_stu_square.lenNo1PN[g_stu_square.nN] = nLenNo1;
            //
              break;
            default:
              break;
            }
            if (g_stu_square.debug)
            {
              for (int j = 1; j < bmpWidth - 2; j++)//特征之一：边的线x
              {
                if (fabs(a) < fabs(b))//用于图像上显示
                  data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, 0 == b ? 0 : int(-(a * j + c) / b), 0 == b ? int(-c / a) : j, 255, 255, 0);
              }

              for (int j = 1; j < bmpHeight - 2; j++)//特征之一：边的线y
              {
                if (fabs(a) >= fabs(b))
                  data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, 0 == a ? int(-c / b) : j, 0 == a ? 0 : int(-(b * j + c) / a), 255, 255, 0);
              }
            }
          }
          /////////////////////////////////////////////////////////匹配判断方向的折线END///////////////////////////////////////////////////////////////////////////////////////////////////
        }
        unceasouts = 0;
        nUnceasNo = 0;
        nBeginj = j;
        nEndj = j;
        bIn = 0;
      }
    if (unceasouts == unceasallownum)
    {
      if (in)//是方向角，尝试直线拟合
        sags++;
      else
        crests++;
      unceasouts++;//防止多次判定情况发生
    }

    //如果最后一个循环，判断是否有折角，防止下一次退出后，该折角不处理
    if (j == outs - 1)
      if (nUnceasNo > unceasallownum/2)
      {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int startno, endno, totalouts = 0, outs = 0;//,totalouts总的不在当前线段上的点数
        double a, b, c, lens;//ax+by+c=0,lens点到直线距离的累加
        bool bSuccess = 0;
        if (i_out_no[nEndj] > i_out_no[nBeginj])//
        {
          startno = (i_out_no[nEndj] + i_out_no[nBeginj]) >> 1;
          endno = i_out_no[nEndj] - 1;
          for (; startno > i_out_no[nBeginj] && endno < i_out_no[nEndj]; startno--, endno--)//找符合直线
          {
            if (!CalcEquationABC(bj_l[startno], bj_h[startno], bj_l[endno], bj_h[endno], a, b, c))
              continue;
            outs -= totalouts;
            totalouts = 0;
            lens = 0;
            for (int j = i_out_no[nBeginj]; j < i_out_no[nEndj]; j++)//边界点与该直线匹配
            {
              double templen = LenFromPtoL(bj_l[j], bj_h[j], a, b, c);
              if (templen > outpointallowlen)
              {
                outs++;
                totalouts++;
              }
              else
              {
                lens += templen;
              }
              if (outs > bj_num || totalouts > (i_out_no[nEndj] - i_out_no[nBeginj]) >> 1 || (int)lens > (((i_out_no[nEndj] - i_out_no[nBeginj] + 1 - totalouts) * (outpointallowlen) >> 2) + ((i_out_no[nEndj] - i_out_no[nBeginj] + 1 - totalouts) >> 6)))
                break;
            }
            if (outs > bj_num)
              break;
            if (totalouts > (i_out_no[nEndj] - i_out_no[nBeginj]) >> 1 || (int)lens > (((i_out_no[nEndj] - i_out_no[nBeginj] + 1 - totalouts) * (outpointallowlen) >> 2) + ((i_out_no[nEndj] - i_out_no[nBeginj] + 1 - totalouts) >> 6)))
              continue;
            else
            {
              bSuccess = 1;
              break;
            }
          }


        }
        else//i_out_no[nEndj]<i_out_no[nBeginj]
        {

          startno = i_out_no[nBeginj] + ((i_out_no[nEndj] + bj_num - i_out_no[nBeginj]) >> 1);//右移优先级低
          endno = i_out_no[nEndj] - 1;
          if (startno >= bj_num)
            startno = startno - bj_num;
          if (endno < 0)
            endno = bj_num + endno;
          for (; (startno > i_out_no[nBeginj] || startno < i_out_no[nEndj]) && (endno > i_out_no[nBeginj] || endno < i_out_no[nEndj]);
            startno = (0 == startno) ? bj_num - 1 : (startno - 1), endno = (0 == endno) ? bj_num - 1 : (endno - 1))//找符合直线
          {
            if (!CalcEquationABC(bj_l[startno], bj_h[startno], bj_l[endno], bj_h[endno], a, b, c))
              continue;
            outs -= totalouts;
            totalouts = 0;
            lens = 0;
            for (int j = i_out_no[nBeginj];
              j < bj_num && j >= i_out_no[nBeginj] || j <= i_out_no[nEndj];
              j = (bj_num == j + 1) ? 0 : (j + 1))//边界点与该直线匹配
            {
              double templen = LenFromPtoL(bj_l[j], bj_h[j], a, b, c);
              if (templen > outpointallowlen)//是否超过规定的距离
              {
                outs++;
                totalouts++;
              }
              else
              {
                lens += templen;
              }
              if (outs > bj_num || totalouts > (i_out_no[nEndj] + bj_num - i_out_no[nBeginj]) >> 1 || (int)lens > (((i_out_no[nEndj] + bj_num - i_out_no[nBeginj] + 1 - totalouts) * (outpointallowlen) >> 2) + ((i_out_no[nEndj] + bj_num - i_out_no[nBeginj] + 1 - totalouts) >> 6)))
                break;
            }
            if (outs > bj_num)
              break;
            if (totalouts > (i_out_no[nEndj] + bj_num - i_out_no[nBeginj]) >> 1 || (int)lens > (((i_out_no[nEndj] + bj_num - i_out_no[nBeginj] + 1 - totalouts) * (outpointallowlen) >> 2) + ((i_out_no[nEndj] + bj_num - i_out_no[nBeginj] + 1 - totalouts) >> 6)))
              continue;
            else
            {
              bSuccess = 1;
              break;
            }
          }
        }
        //找所属角
        if (bSuccess)
        {
          int nLenNo1, nLenNo2;//保存折线所在边的编号
          if (pnOutLnNo[nBeginj] == pnOutLnNo[nEndj])
          {
            int nLenNo = pnOutLnNo[nBeginj];//保存第几条线段0~3
            int nLenNoNext = (3 == nLenNo) ? 0 : (nLenNo + 1);
            int nDstnBegin = i_out_no[nBeginj] >= pi_point_no[nLenNo] ? i_out_no[nBeginj] - pi_point_no[nLenNo] : i_out_no[nBeginj] + bj_num - pi_point_no[nLenNo];//这条折线开始点到该折线所在线段的开始点之间的距离
            int nDstnEnd = pi_point_no[nLenNoNext] >= i_out_no[nEndj] ? pi_point_no[nLenNoNext] - i_out_no[nEndj] : pi_point_no[nLenNoNext] + bj_num - i_out_no[nEndj];//这条折线结束点到该折线所在线段的结束点之间的距离
            if (nDstnBegin > nDstnEnd)
            {
              nLenNo1 = nLenNo;
              nLenNo2 = nLenNoNext;
            }
            else
            {
              nLenNo2 = nLenNo;
              nLenNo1 = (0 == nLenNo) ? 3 : (nLenNo - 1);
            }
          }
          else
          {
            nLenNo1 = pnOutLnNo[nBeginj];
            nLenNo2 = pnOutLnNo[nEndj];
          }

          switch (IsPositiveOrNegative(a, b, c, nLenNo1, nLenNo2))
          {
          case -1:
            //g_stu_square.bPN[g_stu_square.nN]=0;
            break;
          case 0:
            g_stu_square.bPN[g_stu_square.nN] = -1;
          //计算第一条边与x轴正向的夹角
          if (g_stu_square.bPN[g_stu_square.nN] != 0)
            g_stu_square.angreePN[g_stu_square.nN] = CalculateVectorAngle(g_stu_square.pnPX[g_stu_square.nN][nLenNo2] - g_stu_square.pnPX[g_stu_square.nN][nLenNo1],
              g_stu_square.pnPY[g_stu_square.nN][nLenNo2] - g_stu_square.pnPY[g_stu_square.nN][nLenNo1],
              1, 0);
          g_stu_square.lenNo1PN[g_stu_square.nN] = nLenNo1;
          //
            break;
          case 1:
            g_stu_square.bPN[g_stu_square.nN] = 1;
          //计算第一条边与x轴正向的夹角
          if (g_stu_square.bPN[g_stu_square.nN] != 0)
            g_stu_square.angreePN[g_stu_square.nN] = CalculateVectorAngle(g_stu_square.pnPX[g_stu_square.nN][nLenNo2] - g_stu_square.pnPX[g_stu_square.nN][nLenNo1],
              g_stu_square.pnPY[g_stu_square.nN][nLenNo2] - g_stu_square.pnPY[g_stu_square.nN][nLenNo1],
              1, 0);
          g_stu_square.lenNo1PN[g_stu_square.nN] = nLenNo1;
          //
            break;
          default:
            break;
          }
          if (g_stu_square.debug)
          {
            for (int j = 1; j < bmpWidth - 2; j++)//特征之一：边的线x
            {
              if (fabs(a) < fabs(b))//用于图像上显示
                data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, 0 == b ? 0 : int(-(a * j + c) / b), 0 == b ? int(-c / a) : j, 255, 255, 0);
            }

            for (int j = 1; j < bmpHeight - 2; j++)//特征之一：边的线y
            {
              if (fabs(a) >= fabs(b))
                data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, 0 == a ? int(-c / b) : j, 0 == a ? 0 : int(-(b * j + c) / a), 255, 255, 0);
            }
          }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      }

  }
  //保存到结构
  g_stu_square.sags = sags;
  g_stu_square.crests = crests;
  if (g_stu_square.bJudgeFeature)
  {
    if (g_stu_square.pnLen[g_stu_square.nN] > (stuRef.Len - stuRef.Dev) && g_stu_square.pnLen[g_stu_square.nN]<(stuRef.Len + stuRef.Dev)
      && g_stu_square.pnWth[g_stu_square.nN]>(stuRef.Wth - stuRef.Dev) && g_stu_square.pnWth[g_stu_square.nN] < (stuRef.Wth + stuRef.Dev))
      g_stu_square.nN++;
  }
  else
    g_stu_square.nN++;
  //
  delete[] i_out_no; i_out_no = NULL;
  delete[] pnOutLnNo; pnOutLnNo = NULL;
  delete[] i_out_len; i_out_len = NULL;
  delete[] pd_line_a; pd_line_a = NULL;
  delete[] pd_line_b; pd_line_b = NULL;
  delete[] pd_line_c; pd_line_c = NULL;
  if (sags > 0)
    return 2;
  if (crests > 0)
    return 3;
  return 0;
}
//提取多边形区域边长//分割,需在给予特征之后使用
//返回5，不是正方形
//返回1，顶点数多了
//返回0，正常
//函数最后将特征量保存到全局结构
int CIMGRecognition::data_check_feature_split(unsigned char*& pBmpBuf, int biBitCount, int bmpWidth, int bmpHeight
  , int bj_num, int* bj_h, int* bj_l, int* bj_fx, int qybn, int*& xsvalue, int*& fbvalue, int*& jgvalue, int*& txvalue
  , int outpointallowlen, int defectallowlen)
{
  if (bj_num < (stuRef.Len + stuRef.Wth) || 0 == stuRef.Len)
  {
    return 5;
  }
  double centreh, centrel;//存储边界中心行列
  vector vertexV[200];//折点所在向量
  int vertexNo[200];//折点在bjnum位置
  int vertexAng[200];//折点的角度，顺时针<0,逆时针>0
  int vertexAlowN = 200;
  int vertexN = 0;//折点数
  int i_point_num = 0;//折点数
  double templen = 0, minlen = 0, maxlen = 0;//,oklen=0,radious=0;//中间变量，用于计算折点
  int nNum, maxi;//计数和不在线上点计数
  int minno, maxno;//配合记录上面的点在bj数组中的位置
  int minnum, maxnum;//记录找到min点，max点的计数值，当max超过criticalvalue，就表示找到min点；如果min超过criticalvalue，就表示找到max点
  int criticalvalue = defectallowlen;//反向趋势临界点数，固定值
  int stepLen = stuRef.Wth >> 3;//一个向量的像素数
  int type = 5;
  vector lV = { 0,0 };//遍历时的局部临时向量

  int tmpStep = 0;
  vector tmpV[100];//依次保存遍历的v
  vector sumlV = { 0,0 }, sumV0 = { 0,0 };
  int tmpfi[100] = { 0 }, fiN = 0;//依次保存遍历的v第一个点的序号，和遍历编号
  double tmpAng[100] = { 0 };//v和上一v，第一个v和x轴的角度
  bool needReSta = 0, staOK = 0;
  int unWanded = 0;//平行线次数
  for (int fi = 0; fi < bj_num; fi++)
  {
    tmpStep++;
    switch (*(bj_fx + fi))
    {
    case 1:
      lV.y--;
      break;
    case 2:
      lV.x++;
      break;
    case 3:
      lV.y++;
      break;
    case 4:
      lV.x--;
      break;
    default:
      break;
    }

    if (fi == bj_num - 1)//最后一条与起始点的关系
    {
      //if(tmpStep <= stepLen)// && fiN!=0)//最后一条不能只有一条不到
      {
        int iCW;
        double dAngle;
        vector breakV; //用于计算最后一条向量	
        breakV.x = *(bj_l + tmpfi[fiN]) - *(bj_l + tmpfi[0]);
        breakV.y = *(bj_h + tmpfi[fiN]) - *(bj_h + tmpfi[0]);
        iCW = VectorCW(vertexV[vertexN - 2], breakV);//最后第二条和最后一条合格向量的关系
        dAngle = VectorAngle(vertexV[vertexN - 2], breakV);
        vertexAng[vertexN - 1] = iCW * dAngle;



        iCW = VectorCW(breakV, vertexV[0]);//第一条和最后一条合格向量的关系
        dAngle = VectorAngle(breakV, vertexV[0]);

        if (dAngle >= 10)
        {
          //算折点
          breakV.x += vertexV[0].x + lV.x;
          breakV.y += vertexV[0].y + lV.y;
          point breakPoint, vPoint;
          double farPL = 0;
          int farPi = 0;
          vPoint.x = *(bj_l + vertexNo[1]);
          vPoint.y = *(bj_h + vertexNo[1]);
          /////////////////////////////////
          for (int i = tmpfi[fiN - 1]; 0 <= i && i < vertexNo[1] || tmpfi[fiN - 1] <= i && i < bj_num; i = (bj_num == i + 1) ? 0 : (i + 1))//重最远点重新匹配
          {

            breakPoint.x = *(bj_l + i);
            breakPoint.y = *(bj_h + i);
            templen = Distance(breakV, breakPoint, vPoint);
            if (templen > farPL)
            {
              farPL = templen;
              farPi = i;
            }
          }
          //if(farPi != vertexNo[0])
          {
            vertexNo[0] = farPi;

            vertexV[vertexN - 1].x = *(bj_l + farPi) - *(bj_l + vertexNo[vertexN - 1]);//重置最后一条向量
            vertexV[vertexN - 1].y = *(bj_h + farPi) - *(bj_h + vertexNo[vertexN - 1]);
            vertexV[0].x = *(bj_l + vertexNo[1]) - *(bj_l + farPi);//重置第一条向量
            vertexV[0].y = *(bj_h + vertexNo[1]) - *(bj_h + farPi);
            breakV.x = *(bj_l + farPi) - *(bj_l + vertexNo[vertexN - 1]);
            breakV.y = *(bj_h + farPi) - *(bj_h + vertexNo[vertexN - 1]);

            iCW = VectorCW(vertexV[vertexN - 2], breakV);//最后第二条和最后一条合格向量的关系
            dAngle = VectorAngle(vertexV[vertexN - 2], breakV);
            vertexAng[vertexN - 1] = iCW * dAngle;
            iCW = VectorCW(breakV, vertexV[0]);//第一条和最后一条合格向量的关系
            dAngle = VectorAngle(breakV, vertexV[0]);
            vertexAng[0] = iCW * dAngle;
            iCW = VectorCW(vertexV[0], vertexV[1]);//第一条和最后一条合格向量的关系
            dAngle = VectorAngle(vertexV[0], vertexV[1]);
            vertexAng[1] = iCW * dAngle;
          }
          //else
          //	vertexAng[0] = 0;
        }
        else
        {
          /*int farPi = vertexNo[0] ;

          vertexV[vertexN-1].x = *(bj_l + farPi)- *(bj_l + vertexNo[vertexN-1]);//重置最后一条向量
          vertexV[vertexN-1].y = *(bj_h + farPi) - *(bj_h + vertexNo[vertexN-1]);
          vertexV[0].x = *(bj_l + vertexNo[1])-*(bj_l + farPi);//重置第一条向量
          vertexV[0].y = *(bj_h + vertexNo[1])-*(bj_h + farPi);
          breakV.x =  *(bj_l + farPi) - *(bj_l +vertexNo[vertexN-1]);
          breakV.y = *(bj_h + farPi) - *(bj_h +vertexNo[vertexN-1]);

          iCW = VectorCW(vertexV[vertexN-2],breakV);//最后第二条和最后一条合格向量的关系
          dAngle = VectorAngle(vertexV[vertexN-2],breakV);
          vertexAng[vertexN-1] = iCW * dAngle;
          iCW = VectorCW(breakV,vertexV[0]);//第一条和最后一条合格向量的关系
          dAngle = VectorAngle(breakV,vertexV[0]);
          vertexAng[0] = iCW * dAngle;*/
          vertexNo[0] = vertexNo[vertexN - 1];
          int farPi = vertexNo[0];
          vertexN--;
          //vertexV[vertexN-1].x = *(bj_l + farPi)- *(bj_l + vertexNo[vertexN-1]);//重置最后一条向量
          //vertexV[vertexN-1].y = *(bj_h + farPi) - *(bj_h + vertexNo[vertexN-1]);
          vertexV[0].x = *(bj_l + vertexNo[1]) - *(bj_l + farPi);//重置第一条向量
          vertexV[0].y = *(bj_h + vertexNo[1]) - *(bj_h + farPi);
          breakV.x = *(bj_l + farPi) - *(bj_l + vertexNo[vertexN - 1]);
          breakV.y = *(bj_h + farPi) - *(bj_h + vertexNo[vertexN - 1]);

          iCW = VectorCW(vertexV[vertexN - 2], breakV);//最后第二条和最后一条合格向量的关系
          dAngle = VectorAngle(vertexV[vertexN - 2], breakV);
          vertexAng[vertexN - 1] = iCW * dAngle;
          iCW = VectorCW(breakV, vertexV[0]);//第一条和最后一条合格向量的关系
          dAngle = VectorAngle(breakV, vertexV[0]);
          vertexAng[0] = iCW * dAngle;
          iCW = VectorCW(vertexV[0], vertexV[1]);//第一条和最后一条合格向量的关系
          dAngle = VectorAngle(vertexV[0], vertexV[1]);
          vertexAng[1] = iCW * dAngle;
        }
      }
      break;
    }
    else if (tmpStep > stepLen)//一个向量
    {
      sumlV.x += lV.x;
      sumlV.y += lV.y;
      tmpV[fiN].x = lV.x;//保存到零时向量
      tmpV[fiN].y = lV.y;
      lV.x = 0;
      lV.y = 0;


      tmpStep = 0;
      int iCW, iCW2, iCW3;
      double dAngle, dAngle2, dAngle3;

      if (0 == fiN)
      {
        vector zeroV = { 1,0 };
        iCW = VectorCW(zeroV, tmpV[fiN]);
        dAngle = VectorAngle(zeroV, tmpV[fiN]);
      }
      else
      {
        iCW = VectorCW(sumV0, tmpV[fiN]);
        dAngle = VectorAngle(sumV0, tmpV[fiN]);
        iCW2 = VectorCW(sumlV, tmpV[fiN]);
        dAngle2 = VectorAngle(sumlV, tmpV[fiN]);
        iCW3 = VectorCW(sumlV, sumV0);
        dAngle3 = VectorAngle(sumlV, sumV0);
      }
      tmpAng[fiN] = dAngle;

      if (0 == fiN)//如果是第一个向量
      {
        vertexNo[vertexN] = tmpfi[0];//先找全再进行形状匹配
        //;//折点的趋势
        vertexN++;//折点数++
        sumV0.x = sumlV.x;
        sumV0.y = sumlV.y;
        if (vertexN >= vertexAlowN)
          return type;
      }
      else if (-1 == iCW)//趋势顺时针
      {
        if ((dAngle < 10 || dAngle2 < 5) && (unWanded < 2 || dAngle3 < 5))
        {
          unWanded = 0;
          //sumV0.x += tmpV[fiN].x;
          //sumV0.y += tmpV[fiN].y;
          sumV0.x = sumlV.x;
          sumV0.y = sumlV.y;
        }
        else
        {
          //iCW = VectorCW(sumlV,tmpV[fiN]);
          //dAngle = VectorAngle(sumlV,tmpV[fiN]);
          //if (dAngle >=5)
          {
            unWanded++;
          }
          if (unWanded > 2)
            needReSta = 1;
        }


      }
      else if (1 == iCW)//趋势逆时针
      {
        if ((dAngle < 10 || dAngle2 < 5) && (unWanded < 2 || dAngle3 < 5))
        {
          //fiN ++;
          unWanded = 0;
          //sumV0.x += tmpV[fiN].x;
          //sumV0.y += tmpV[fiN].y;
          sumV0.x = sumlV.x;
          sumV0.y = sumlV.y;
        }
        else//角度太大
        {
          //iCW = VectorCW(sumlV,tmpV[fiN]);
          //dAngle = VectorAngle(sumlV,tmpV[fiN]);
          //if (dAngle >=5)
          {
            unWanded++;
          }
          if (unWanded > 2)
            needReSta = 1;
        }
      }
      else if (dAngle > 175 && dAngle < 185)
      {
        //fiN ++;
        unWanded++;
        if (unWanded > 2)
          needReSta = 1;
      }

      if (1 == needReSta || unWanded > 0 && (bj_num - 1 - fi) <= stepLen)//如果需要重新开始找折点
      {

        needReSta = 0;
        vector breakV = { 0,0 }; //用于找折点的直线向量
        sumlV.x = 0;
        sumlV.y = 0;
        for (int i = 1; i <= unWanded; i++)
        {
          breakV.x += tmpV[fiN - i].x;// +tmpV[fiN - 2].x + tmpV[fiN - 3].x;// + tmpV[fiN - 4].x;tmpV[fiN].x + tmpV[fiN - 1].x + 
          breakV.y += tmpV[fiN - i].y;// +tmpV[fiN - 2].y + tmpV[fiN - 3].y;// + tmpV[fiN - 4].y;tmpV[fiN].y + tmpV[fiN - 1].y + 
        }
        point breakPoint, vPoint;
        double farPL = 0;
        int farPi = 0;
        vPoint.x = *(bj_l + tmpfi[fiN]);
        vPoint.y = *(bj_h + tmpfi[fiN]);


        for (int fii = tmpfi[fiN - (unWanded)]; fii < tmpfi[fiN]; fii++)//重最远点重新匹配
        {

          breakPoint.x = *(bj_l + fii);
          breakPoint.y = *(bj_h + fii);
          templen = Distance(breakV, breakPoint, vPoint);
          if (templen > farPL)
          {
            farPL = templen;
            farPi = fii;
          }
        }
        unWanded = 0;
        fiN = 0;//重新开始
        fi = farPi;
        vertexV[vertexN - 1].x = *(bj_l + farPi) - *(bj_l + vertexNo[vertexN - 1]);//保存折点所在向量
        vertexV[vertexN - 1].y = *(bj_h + farPi) - *(bj_h + vertexNo[vertexN - 1]);
        if (vertexN > 1)
        {
          iCW = VectorCW(vertexV[vertexN - 2], vertexV[vertexN - 1]);
          dAngle = VectorAngle(vertexV[vertexN - 2], vertexV[vertexN - 1]);
          vertexAng[vertexN - 1] = iCW * dAngle;
        }

      }
      else
      {
        fiN++;
        if (fiN == 100)
        {
          return type;
        }
      }

      tmpfi[fiN] = fi;//用于提取下一个向量的起始坐标
    }
  }
  double sSlant = (stuRef.Len >> 2) - stuRef.Dev;//斜线最短
  double bSlant = (stuRef.Len >> 1) - stuRef.Dev;//斜线最长
  /////////////////////////////////////找四边形/////////////////////////////////////////////////////////////////////////////////////
  if (vertexN > 3)
  {
    for (int i = 0; i < 4; i++)
    {
      g_stu_square.d_len[i] = 0;
    }
    //double _90Len[10]={0};
    int _90Num = 0, tmp90N = 0, pnNum = 0;//连续90°
    int tmp90 = 0;//保存可能成为90°的角，即由2个小于90°的角组成
    int tmpPN = 0;
    needReSta = 0;
    staOK = 0;
    /*for(int vertexI = vertexN-1 ; vertexI > vertexN-3 ; vertexI --)
    {
      if (stuRef.SAg<=abs(vertexAng[vertexI]) && abs(vertexAng[vertexI])<=stuRef.BAg)//90°
      {
        {
          _90Num ++;
          tmp90N = _90Num >3?0:_90Num;
          g_stu_square.pnPX[g_stu_square.nN][tmp90N] = double(*(bj_l + vertexNo[vertexI]));
          g_stu_square.pnPY[g_stu_square.nN][tmp90N] = double(*(bj_h + vertexNo[vertexI]));
          g_stu_square.d_len[tmp90N] += templen;
          templen = VectorLen(vertexV[vertexI]);
          break;
        }
      }
      else if(5 < abs(vertexAng[vertexI]) && abs(vertexAng[vertexI])<stuRef.SAg)//<90°
      {
      }
      else
        break;
    }*/
    for (int vertexI = 0; vertexI < vertexN; vertexI++)
    {
      if (g_stu_square.debug)
      {
        data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, *(bj_h + vertexNo[vertexI]), *(bj_l + vertexNo[vertexI]), 200, 100, 0);
      }
      if (vertexAng[vertexI] > 0)//如果大于0
      {
        /*int tempI = vertexI;
        vector tempV = vertexV[vertex]
        do
        {
          templen = VectorLen(vertexV[vertexI]);
          if (templen < sSlant)//可能是小凸起
          {
            templen = VectorLen(vertexV[vertexI+1]);
            if (templen <sSlant)
            {

            }
          }
          tempI ++;
        }
        while(tempI - vertexI <4)*/
        if (abs(vertexAng[vertexI]) <= 5)// 0°
        {
          if (pnNum > 0)
          {
            pnNum++;
          }
          else
            g_stu_square.d_len[tmp90N] += templen;
        }
        else
          needReSta = 1;

      }
      else if (0 == staOK)//判断角度
      {
        templen = VectorLen(vertexV[vertexI]);
        if (stuRef.SAg <= abs(vertexAng[vertexI]) && abs(vertexAng[vertexI]) <= stuRef.BAg)//90°
        {
          if (pnNum > 0)
          {
            needReSta = 1;
          }
          else
          {
            _90Num++;
            tmp90N = _90Num > 3 ? 0 : _90Num;
            g_stu_square.pnPX[g_stu_square.nN][tmp90N] = double(*(bj_l + vertexNo[vertexI]));
            g_stu_square.pnPY[g_stu_square.nN][tmp90N] = double(*(bj_h + vertexNo[vertexI]));
            g_stu_square.d_len[tmp90N] += templen;

          }


        }
        else if (5 < abs(vertexAng[vertexI]) && abs(vertexAng[vertexI]) < stuRef.SAg)//<90°
        {
          pnNum++;
          double tmpTempLen = templen;
          double tmpVectorLen = 0;//斜线长度
          int tempAngle = 0;
          for (int i = pnNum - 1; i >= 0; i--)
          {
            tempAngle += abs((vertexAng[vertexI - i]));
          }
          for (int i = pnNum - 1; i > 0; i--)
          {
            tmpVectorLen += VectorLen(vertexV[vertexI - i]);
          }

          if (tmpVectorLen >= bSlant)
          {
            needReSta = 1;
          }
          else if (1 == pnNum && 0 == vertexI)
          {
            tmpVectorLen = VectorLen(vertexV[vertexN - 1]);//斜线长度
            tempAngle = abs(vertexAng[vertexI]) + abs(vertexAng[vertexN - 1]);
            if (tmpVectorLen >= bSlant)
            {
            }
            else if (stuRef.SAg <= tempAngle && tempAngle <= stuRef.BAg)//2个小角相加等于90
            {
              if (sSlant < tmpVectorLen && tmpVectorLen < bSlant)
              {
                if (15 <= abs(vertexAng[vertexI]) && abs(vertexAng[vertexI]) <= 25)//dAngle1>15 && dAngle1<25
                {
                  tmpPN = -1;

                }
                else
                {
                  tmpPN = 1;
                }
              }
              g_stu_square.bPN[g_stu_square.nN] = tmpPN;
              int tmpI = vertexI;
              point tmpP, tmpVP;

              tmpVP.x = *(bj_l + vertexNo[tmpI]);
              tmpVP.y = *(bj_h + vertexNo[tmpI]);
              tmpP.x = *(bj_l + vertexNo[vertexN - 1]);
              tmpP.y = *(bj_h + vertexNo[vertexN - 1]);
              templen = Distance(vertexV[tmpI], tmpP, tmpVP);
              g_stu_square.d_len[_90Num] += templen;
              _90Num++;
              tmp90N = _90Num > 3 ? 0 : _90Num;


              tmpI = vertexN - 2;
              tmpVP.x = *(bj_l + vertexNo[tmpI]);
              tmpVP.y = *(bj_h + vertexNo[tmpI]);
              tmpP.x = *(bj_l + vertexNo[vertexI]);
              tmpP.y = *(bj_h + vertexNo[vertexI]);
              templen = Distance(vertexV[tmpI], tmpP, tmpVP);
              vector tmpV = VectorProjection(vertexV[vertexN - 1], vertexV[tmpI]);
              g_stu_square.pnPX[g_stu_square.nN][tmp90N] = double(*(bj_l + vertexNo[vertexN - 1]) + tmpV.x);
              g_stu_square.pnPY[g_stu_square.nN][tmp90N] = double(*(bj_h + vertexNo[vertexN - 1]) + tmpV.y);
              g_stu_square.d_len[tmp90N] += templen;
              pnNum = 0;
              g_stu_square.d_len[tmp90N] += tmpTempLen;
            }

          }
          //int tempAngle = abs(vertexAng[vertexI]) + abs(vertexAng[vertexI-1]);
          else if (stuRef.SAg <= tempAngle && tempAngle <= stuRef.BAg)//小角相加等于90
          {
            if (sSlant < tmpVectorLen && tmpVectorLen < bSlant)
            {
              if (15 <= abs(vertexAng[vertexI]) && abs(vertexAng[vertexI]) <= 25)//dAngle1>15 && dAngle1<25
              {
                tmpPN = -1;
              }
              else
              {
                tmpPN = 1;
              }
            }
            int tmpI = vertexI;
            point tmpP, tmpVP;

            tmpVP.x = *(bj_l + vertexNo[tmpI]);
            tmpVP.y = *(bj_h + vertexNo[tmpI]);
            tmpP.x = *(bj_l + vertexNo[vertexI - pnNum + 1]);
            tmpP.y = *(bj_h + vertexNo[vertexI - pnNum + 1]);
            templen = Distance(vertexV[tmpI], tmpP, tmpVP);
            g_stu_square.d_len[_90Num] += templen;
            _90Num++;
            tmp90N = _90Num > 3 ? 0 : _90Num;


            tmpI = vertexI == pnNum - 1 ? vertexN - 1 : vertexI - pnNum;
            tmpVP.x = *(bj_l + vertexNo[tmpI]);
            tmpVP.y = *(bj_h + vertexNo[tmpI]);
            tmpP.x = *(bj_l + vertexNo[vertexI]);
            tmpP.y = *(bj_h + vertexNo[vertexI]);
            templen = Distance(vertexV[tmpI], tmpP, tmpVP);
            vector tmpV = { 0,0 };
            for (int i = 1; i < pnNum; i++)
            {
              tmpV.x += vertexV[vertexI - i].x;
              tmpV.y += vertexV[vertexI - i].y;
            }
            tmpV = VectorProjection(tmpV, vertexV[tmpI]);
            g_stu_square.pnPX[g_stu_square.nN][tmp90N] = double(*(bj_l + vertexNo[vertexI - pnNum + 1]) + tmpV.x);
            g_stu_square.pnPY[g_stu_square.nN][tmp90N] = double(*(bj_h + vertexNo[vertexI - pnNum + 1]) + tmpV.y);
            g_stu_square.d_len[tmp90N] += templen;
            pnNum = 0;
            g_stu_square.d_len[tmp90N] += tmpTempLen;
          }
          else if (tempAngle > stuRef.BAg)
          {
            needReSta = 1;
          }


          //else
            //g_stu_square.d_len[tmp90N] += templen;
        }
        else if (abs(vertexAng[vertexI]) <= 5)// 0°
        {
          if (pnNum > 0)
          {
            pnNum++;
          }
          else
            g_stu_square.d_len[tmp90N] += templen;
        }
        else//>90°
        {
          needReSta = 1;
        }
      }
      /*if (_90Num == 4)
      {
        needReSta = 1;
      }*/


      if (1 == needReSta || vertexI == vertexN - 1)
      {

        if (_90Num >= 2)
        {
          if (_90Num == 2)
          {
            if (1 != needReSta)
            {
              g_stu_square.pnPX[g_stu_square.nN][3] = double(*(bj_l + vertexNo[0]));
              g_stu_square.pnPY[g_stu_square.nN][3] = double(*(bj_h + vertexNo[0]));
            }
            else
            {
              g_stu_square.pnPX[g_stu_square.nN][3] = double(*(bj_l + vertexNo[vertexI]));
              g_stu_square.pnPY[g_stu_square.nN][3] = double(*(bj_h + vertexNo[vertexI]));
            }
          }
          bool lenOK = 0, wthOK = 0;
          double maxlen = 0;
          int maxi = 0;
          for (int i = 0; i < 3; i++)
          {
            if (g_stu_square.d_len[i] > maxlen)
            {
              maxlen = g_stu_square.d_len[i];
              maxi = i;
            }
          }
          if (g_stu_square.d_len[maxi] >= (stuRef.Len - stuRef.Dev) && g_stu_square.d_len[maxi] < (stuRef.Len + stuRef.Dev) && !lenOK)
          {
            g_stu_square.pnLen[g_stu_square.nN] = g_stu_square.d_len[maxi];
            lenOK = 1;
          }
          for (int i = 0; i < 3; i++)
          {
            //int midDev = (stuRef.Len -stuRef.Wth)>>1;
            if (i != maxi && i != (maxi + 2 > 3 ? maxi - 2 : maxi + 2))
              if (g_stu_square.d_len[i] > (stuRef.Wth - stuRef.Dev) && g_stu_square.d_len[i] < (stuRef.Wth + stuRef.Dev) && !wthOK)
              {
                g_stu_square.pnWth[g_stu_square.nN] = g_stu_square.d_len[i];
                wthOK = 1;
              }
            if (lenOK && wthOK)
            {
              g_stu_square.pnZPX[g_stu_square.nN] = (g_stu_square.pnPX[g_stu_square.nN][1] + g_stu_square.pnPX[g_stu_square.nN][3]) / 2;
              g_stu_square.pnZPY[g_stu_square.nN] = (g_stu_square.pnPY[g_stu_square.nN][1] + g_stu_square.pnPY[g_stu_square.nN][3]) / 2;
              if (3 >= _90Num)
              {
                g_stu_square.pnPX[g_stu_square.nN][0] = (g_stu_square.pnZPX[g_stu_square.nN] - g_stu_square.pnPX[g_stu_square.nN][2]) * 2 + g_stu_square.pnPX[g_stu_square.nN][2];
                g_stu_square.pnPY[g_stu_square.nN][0] = (g_stu_square.pnZPY[g_stu_square.nN] - g_stu_square.pnPY[g_stu_square.nN][2]) * 2 + g_stu_square.pnPY[g_stu_square.nN][2];
              }
              else if (4 == _90Num)
              {
              }

              //保存到结果数组
              g_stu_square.nN++;


              //保存到结构
              bool lenOK = 1;
              //去除重复的中心
              if (1 == lenOK)
              {
                for (int i = g_stu_square.nN - 2; i >= 0; i--)
                {
                  if (LenFromP1toP2(g_stu_square.pnZPX[i], g_stu_square.pnZPY[i], g_stu_square.pnZPX[g_stu_square.nN - 1], g_stu_square.pnZPY[g_stu_square.nN - 1]) < stuRef.Len)
                  {
                    g_stu_square.nN--;
                    lenOK = 0;
                    break;
                  }
                  //if(sqrt(pow(double(g_stu_square.pnZPX[i]-xt),2)+pow(double(g_stu_square.pnZPY[i]-yt),2))<(stuRef.Rad)) //点击位置离片的粗略中心相距小于s_sort.allowable_devision，这么做主要可以当点不准时设置调节
                }
                for (int i = 0; i < 4; i++)
                {
                  int fi = i;
                  int bi = i == 3 ? 0 : i + 1;
                  if (!CalcEquationABC(g_stu_square.pnPX[g_stu_square.nN][fi], g_stu_square.pnPY[g_stu_square.nN][fi], g_stu_square.pnPX[g_stu_square.nN][bi], g_stu_square.pnPY[g_stu_square.nN][bi]
                    , g_stu_square.d_a[fi], g_stu_square.d_b[fi], g_stu_square.d_c[fi]))
                    lenOK = 0;
                }
                //在图像上做标记
                if (1 == lenOK && g_stu_square.debug)
                {


                  for (int j = 1; j < bmpWidth - 2; j++)//特征之一：边的线x
                  {
                    for (int li = 0; li < 4; li++)
                      if (fabs(g_stu_square.d_a[li]) < fabs(g_stu_square.d_b[li]))//用于图像上显示
                        data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, 0 == g_stu_square.d_b[li] ? 0 : int(-(g_stu_square.d_a[li] * j + g_stu_square.d_c[li]) / g_stu_square.d_b[li]), 0 == g_stu_square.d_b[li] ? int(-g_stu_square.d_c[li] / g_stu_square.d_a[li]) : j, 255, 255, 0);
                  }

                  for (int j = 1; j < bmpHeight - 2; j++)//特征之一：边的线y
                  {
                    for (int li = 0; li < 4; li++)
                      if (fabs(g_stu_square.d_a[li]) >= fabs(g_stu_square.d_b[li]))
                        data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, 0 == g_stu_square.d_a[li] ? int(-g_stu_square.d_c[li] / g_stu_square.d_b[li]) : j, 0 == g_stu_square.d_a[li] ? 0 : int(-(g_stu_square.d_b[li] * j + g_stu_square.d_c[li]) / g_stu_square.d_a[li]), 255, 255, 0);
                  }
                  if (lenOK)
                    type = 0;
                }
              }

              if (g_stu_square.nN == 20)
                return type;
              break;
            }
          }

        }//if(_90Num >2)
        _90Num = 0, tmp90N = 0, pnNum = 0;//连续90°
        tmp90 = 0;//保存可能成为90°的角，即由2个小于90°的角组成
        tmpPN = 0;
        needReSta = 0;
        staOK = 0;
        if (abs(vertexAng[vertexI]) < stuRef.BAg && vertexAng[vertexI] < 0 && vertexI != (vertexN - 1) && vertexI != 0)
          vertexI--;
        for (int i = 0; i < 4; i++)
        {
          g_stu_square.d_len[i] = 0;
        }
      }//if(1 == needReSta)
    }//for(int vertexI = 0 ; vertexI < vertexN ; vertexI ++)

  }//if (vertexN > 3)
  return type;
}

//利用前一个平均值算下一个平均值
void CIMGRecognition::SortAvg(int sum, int value, double& avg)
{
  double n = sum;
  double xi = value;
  //double avg;
  if (1 == n)
  {
    avg = xi;//平均值

  }
  if (n > 1)
  {
    avg = (avg + xi / (n - 1)) / (n / (n - 1));//求平均值
  }

}
//算两点之间长度
double CIMGRecognition::LenFromP1toP2(double P1X, double P1Y, double P2X, double P2Y)
{
  return (sqrt(pow(double(P1X - P2X), 2) + pow(double(P1Y - P2Y), 2)));
}
//算点到直线的距离
double CIMGRecognition::LenFromPtoL(double PX, double PY, double a, double b, double c)
{
  return sqrt(pow((a * PX + b * PY + c), 2) / (pow(a, 2) + pow(b, 2)));
}
//算直线的方程ax+by+c=0
bool CIMGRecognition::CalcEquationABC(double P1X, double P1Y, double P2X, double P2Y, double& a, double& b, double& c)
{
  if (P1X == P2X)
  {
    if (P1Y == P2Y)
      return 0;
    a = 1;
    b = 0;
    c = -P1X;
  }
  else if (P1Y == P2Y)
  {
    a = 0;
    b = 1;
    c = -P1Y;
  }
  else
  {
    b = -1;
    a = (P1Y - P2Y) / (P1X - P2X);
    c = P1Y - a * P1X;
  }
  return 1;
}
//
//算点的位置//只能使用在此程序中//只能使用与四边形且边按顺序存放
bool CIMGRecognition::InOrOut(double PX, double PY, double*& a, double*& b, double*& c)
{
  bool in = 1;
  for (int i = 0; i < 2; i++)
  {
    if (a[i] * b[i] * a[i + 2] * b[i + 2] <= 0)//例外处理,当2条直线斜度一条小于等于90，一条大于等于90
    {
      if (a[i] != 0 && b[i] != 0)
      {
        if (abs(a[i] / b[i]) > 1)
        {
          if ((PX + (b[i] * PY + c[i]) / a[i]) * (PX + (b[i + 2] * PY + c[i + 2]) / a[i + 2]) > 0)
            in = 0;
        }
        else
        {
          if ((PY + (a[i] * PX + c[i]) / b[i]) * (PY + (a[i + 2] * PX + c[i + 2]) / b[i + 2]) > 0)
            in = 0;
        }
      }
      else if (a[i + 2] != 0 && b[i + 2] != 0)
      {
        if (abs(a[i + 2] / b[i + 2]) > 1)
        {
          if ((PX + (b[i] * PY + c[i]) / a[i]) * (PX + (b[i + 2] * PY + c[i + 2]) / a[i + 2]) > 0)
            in = 0;
        }
        else
        {
          if ((PY + (a[i] * PX + c[i]) / b[i]) * (PY + (a[i + 2] * PX + c[i + 2]) / b[i + 2]) > 0)
            in = 0;
        }
      }
    }
    else if ((a[i] * PX + b[i] * PY + c[i]) * (a[i + 2] * PX + b[i + 2] * PY + c[i + 2]) > 0)//正常情况
      in = 0;
  }
  return in;
  //return sqrt(pow((a*PX+b*PY+c),2)/(pow(a,2)+pow(b,2)));
}
//判断是否是矩形
//返回-1没有识别成功，不执行
//返回0，成功
//返回1，角度不正常
int CIMGRecognition::IsSquare(double smallAg, double bigAg)
{
  if (!g_stu_square.rcglineok)
    return -1;
  bool yes = 0;
  int nexti;
  for (int i = 0; i < 4; i++)//依次算4个交点
  {
    nexti = (i == 3 ? 0 : i + 1);
    if (g_stu_square.d_a[i] == 0 && g_stu_square.d_a[nexti] == 0 || g_stu_square.d_b[i] == 0 && g_stu_square.d_b[nexti] == 0)
      return -1;
    else if (g_stu_square.d_a[i] == 0 && g_stu_square.d_b[nexti] == 0)
    {
      g_stu_square.d_point_y[i] = -g_stu_square.d_c[i] / g_stu_square.d_b[i];
      g_stu_square.d_point_x[i] = -g_stu_square.d_c[nexti] / g_stu_square.d_a[nexti];
    }
    else if (g_stu_square.d_a[i] == 0)
    {
      g_stu_square.d_point_y[i] = -g_stu_square.d_c[i] / g_stu_square.d_b[i];
      g_stu_square.d_point_x[i] = -(g_stu_square.d_b[nexti] * g_stu_square.d_point_y[i] + g_stu_square.d_c[nexti]) / (g_stu_square.d_a[nexti]);
    }
    else if (g_stu_square.d_b[nexti] == 0)
    {
      g_stu_square.d_point_x[i] = -g_stu_square.d_c[nexti] / g_stu_square.d_a[nexti];
      g_stu_square.d_point_y[i] = -(g_stu_square.d_a[i] * g_stu_square.d_point_x[i] + g_stu_square.d_c[i]) / (g_stu_square.d_b[i]);

    }
    else if (g_stu_square.d_a[i] != 0 && g_stu_square.d_b[nexti] != 0)
    {
      g_stu_square.d_point_x[i] = ((g_stu_square.d_b[i] * g_stu_square.d_c[nexti]) / (g_stu_square.d_b[nexti] * g_stu_square.d_a[i]) - g_stu_square.d_c[i] / g_stu_square.d_a[i]) / (1 - (g_stu_square.d_b[i] * g_stu_square.d_a[nexti]) / (g_stu_square.d_b[nexti] * g_stu_square.d_a[i]));
      g_stu_square.d_point_y[i] = -(g_stu_square.d_a[nexti] * g_stu_square.d_point_x[i] + g_stu_square.d_c[nexti]) / (g_stu_square.d_b[nexti]);
    }
    //保存到结果数组(顶点坐标）
    g_stu_square.pnPX[g_stu_square.nN][i] = (int)g_stu_square.d_point_x[i];
    g_stu_square.pnPY[g_stu_square.nN][i] = (int)g_stu_square.d_point_y[i];
    double angle = abs(atan(-g_stu_square.d_a[nexti] / g_stu_square.d_b[nexti]) - atan(-g_stu_square.d_a[i] / g_stu_square.d_b[i])) / gd_PI * 180;//不清楚内外角，这里可以不处理
    g_stu_square.d_theta[i] = angle;
    if (angle > bigAg || angle < smallAg)
      yes = 1;
  }
  //保存到结果数组（中心坐标）
  g_stu_square.pnZPX[g_stu_square.nN] = (int(g_stu_square.d_point_x[0] + g_stu_square.d_point_x[1] + g_stu_square.d_point_x[2] + g_stu_square.d_point_x[3]) >> 2);
  g_stu_square.pnZPY[g_stu_square.nN] = (int(g_stu_square.d_point_y[0] + g_stu_square.d_point_y[1] + g_stu_square.d_point_y[2] + g_stu_square.d_point_y[3]) >> 2);
  return yes;
}
//判断是否是正反面
//返回-1没有识别成功，不执行
//返回0，反面
//返回1，正面
int  CIMGRecognition::IsPositiveOrNegative(double a, double b, double c, int beginLineNo, int endLineNo)
{
  if (!g_stu_square.rcglineok)
    return -1;
  double dAngle1 = 0, dAngle2 = 0;
  double min_deg = stuRef.PN_sag, max_deg = stuRef.PN_bag;

  int bPositive = 0;
  //第一个角
  {
    if (g_stu_square.d_a[beginLineNo] == 0 && a == 0 || g_stu_square.d_b[beginLineNo] == 0 && b == 0)
      return -1;
    else if (g_stu_square.d_a[beginLineNo] == 0 && b == 0)
    {
      g_stu_square.d_point_y[beginLineNo] = -g_stu_square.d_c[beginLineNo] / g_stu_square.d_b[beginLineNo];
      g_stu_square.d_point_x[beginLineNo] = -c / a;
    }
    else if (g_stu_square.d_a[beginLineNo] == 0)
    {
      g_stu_square.d_point_y[beginLineNo] = -g_stu_square.d_c[beginLineNo] / g_stu_square.d_b[beginLineNo];
      g_stu_square.d_point_x[beginLineNo] = -(b * g_stu_square.d_point_y[beginLineNo] + c) / (a);
    }
    else if (b == 0)
    {
      g_stu_square.d_point_x[beginLineNo] = -c / a;
      g_stu_square.d_point_y[beginLineNo] = -(g_stu_square.d_a[beginLineNo] * g_stu_square.d_point_x[beginLineNo] + g_stu_square.d_c[beginLineNo]) / (g_stu_square.d_b[beginLineNo]);

    }
    else if (g_stu_square.d_a[beginLineNo] != 0 && b != 0)
    {
      g_stu_square.d_point_x[beginLineNo] = ((g_stu_square.d_b[beginLineNo] * c) / (b * g_stu_square.d_a[beginLineNo]) - g_stu_square.d_c[beginLineNo] / g_stu_square.d_a[beginLineNo]) / (1 - (g_stu_square.d_b[beginLineNo] * a) / (b * g_stu_square.d_a[beginLineNo]));
      g_stu_square.d_point_y[beginLineNo] = -(a * g_stu_square.d_point_x[beginLineNo] + c) / (b);
    }
    dAngle1 = abs(atan(-a / b) - atan(-g_stu_square.d_a[beginLineNo] / g_stu_square.d_b[beginLineNo])) / gd_PI * 180;
    if (dAngle1 > 90)//由于前面保证了是矩形，这里必须可以是《90°的
      dAngle1 = 180 - dAngle1;
  }
  //第二个角
  {
    if (a == 0 && g_stu_square.d_a[endLineNo] == 0 || b == 0 && g_stu_square.d_b[endLineNo] == 0)
      return -1;
    else if (a == 0 && g_stu_square.d_b[endLineNo] == 0)
    {
      g_stu_square.d_point_y[endLineNo] = -c / b;
      g_stu_square.d_point_x[endLineNo] = -g_stu_square.d_c[endLineNo] / g_stu_square.d_a[endLineNo];
    }
    else if (a == 0)
    {
      g_stu_square.d_point_y[endLineNo] = -c / b;
      g_stu_square.d_point_x[endLineNo] = -(g_stu_square.d_b[endLineNo] * g_stu_square.d_point_y[endLineNo] + g_stu_square.d_c[endLineNo]) / (g_stu_square.d_a[endLineNo]);
    }
    else if (g_stu_square.d_b[endLineNo] == 0)
    {
      g_stu_square.d_point_x[endLineNo] = -g_stu_square.d_c[endLineNo] / g_stu_square.d_a[endLineNo];
      g_stu_square.d_point_y[endLineNo] = -(a * g_stu_square.d_point_x[endLineNo] + c) / (b);

    }
    else if (a != 0 && g_stu_square.d_b[endLineNo] != 0)
    {
      g_stu_square.d_point_x[endLineNo] = ((b * g_stu_square.d_c[endLineNo]) / (g_stu_square.d_b[endLineNo] * a) - c / a) / (1 - (b * g_stu_square.d_a[endLineNo]) / (g_stu_square.d_b[endLineNo] * a));
      g_stu_square.d_point_y[endLineNo] = -(g_stu_square.d_a[endLineNo] * g_stu_square.d_point_x[endLineNo] + g_stu_square.d_c[endLineNo]) / (g_stu_square.d_b[endLineNo]);
    }
    dAngle2 = abs(atan(-g_stu_square.d_a[endLineNo] / g_stu_square.d_b[endLineNo]) - atan(-a / b)) / gd_PI * 180;
    if (dAngle2 > 90)
      dAngle2 = 180 - dAngle2;
  }
  if (dAngle1 < dAngle2 && dAngle1>min_deg/*15*/ && dAngle1 < max_deg/*25*/)
    bPositive = 1;
  else if (dAngle1 > dAngle2 && dAngle2 > min_deg && dAngle2 < max_deg)
    bPositive = 0;
  else
    bPositive = -1;
  return bPositive;
}


// 回复带符号的角度，1>2 cw +  1>2 ccw -//由于图像坐标与实际上下颠倒，需要加负号
// 改成都是 cw的角度 0 - 360°
double CIMGRecognition::CalculateVectorAngle(double x1, double y1, double x2, double y2)
{
  //acos return radian,we should transform it into degree  
  double angle;// = acos((x1 * x2 + y1 * y2) / sqrt((x1 * x1 + y1 * y1) * (x2 * x2 + y2 * y2) * 1.0)) * 180 / 3.14;
  int cw;
  vector v1 = { x1, y1 }, v2 = { x2, y2 };

  angle = VectorAngle(v1, v2);
  cw = VectorCW(v1, v2);
   //return angle*cw;
  return cw < 0 ? angle : 360-angle;
}
//-1未识别
//debug识别中生成可查看的bmp图，以及完整识别
//PDC，指定窗口pDC，可以进行窗口标识
//
//
//识别函数，外部唯一调用函数
int CIMGRecognition::RCGBMP(CDC * pDC, CRect rect, char* bmpName, int outpointallowlen = 2, int defectallowlen = 2, int threshold = 120, bool debug = 0, bool circle = 0, bool bAutoThrd = 0, bool delNoise = 0)
{
  if (pDC != NULL)
  {
    pDC->SetTextColor(RGB(255, 0, 255));
    pDC->SetBkMode(TRANSPARENT);
    pDC->SelectObject(&font);
    //pDC ->TextOutW(0,0,_T("123456789"));
  }
  m_bmp_file = bmpName;
  g_stu_square.debug = debug;
  unsigned char* pBmpBuf = NULL;
  int* xsvalue = NULL;
  int* fbvalue = NULL;
  int* jgvalue = NULL;
  int* txvalue = NULL;
  int bmpWidth, bmpHeight;
  g_stu_square.nN = 0;//已识别数置0
  g_stu_square.bIsLap = 0;//重叠表示z置0
  int type = -1;//识别类型置-1
  //char * readPath=NULL;//"temps2.bmp";
  char readPath[MAX_PATH];
  //WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_bmp_file,-1,readPath,CStringA(m_bmp_file).GetLength(),NULL,NULL); 
  WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, m_bmp_file, -1, readPath, sizeof(readPath), NULL, NULL);
  //tempstr=m_bmp_file.GetBuffer(CStringA(m_bmp_file).GetLength());
  int nRd = readBmp(readPath, pBmpBuf, bmpWidth, bmpHeight); //8\24w
  if (nRd != 0)
  {
    return type;
  }
  if (bAutoThrd)//如果指定自动阈值，进行自动阈值
  {
    int nTmpThrd = read_image_fz(pBmpBuf, bmpWidth, bmpHeight);//检测当前图像的中间阈值
    if (nTmpThrd != -1)
      threshold = nTmpThrd;
  }
  int pos = m_bmp_file.ReverseFind('\\');
  m_bmp_file = m_bmp_file.Left(pos);
  WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, m_bmp_file, -1, chPathMain, sizeof(chPathMain), NULL, NULL);
  //保存图像尺寸到全局
  g_stu_square.nBMPH = bmpHeight;
  g_stu_square.nBMPW = bmpWidth;
  //saveBmp("temps.bmp",pBmpBuf,bmpWidth,bmpHeight,biBitCount,NULL);
  //m_bmp_file.ReleaseBuffer();
  /*
  readPath=m_bmp_file.GetBuffer(m_bmp_file.GetLength());
  readBmp(readPath,pBmpBuf,bmpWidth,bmpHeight);
  m_bmp_file.ReleaseBuffer();*/
  int temp_bmp_data = bmpWidth * bmpHeight;

  xsvalue = new int[temp_bmp_data]; //全0//最后是边界加区域数组，值是对应的区域编号
  //memset(xsvalue,0,temp_bmp_data);
  fbvalue = new int[temp_bmp_data]; //全0//最后是边界数组，边界值是对应的区域编号,在图片预处理中有用，标记已预处理
  //memset(fbvalue,0,temp_bmp_data);
  jgvalue = new int[temp_bmp_data]; //白的-1，黑的0//用于边界处理的数组
  txvalue = new int[temp_bmp_data]; //白的-1，黑的0//如果当前点是边界n的找过的点，标记n//改为记录方向，为填充区域提供信息
  int lb_blank = 0;
  //二值化处理，识别多片可以考虑不判断极值，直接二值化，更改readimagefz

  if (1)//xs，fb全0；jg，tx白-1，黑0；
    lb_blank = read_image_mul(pBmpBuf, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, threshold, debug, delNoise);//按照阈值二值化图像，去噪
  else
    lb_blank = read_image_blank(pBmpBuf, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, threshold);//判断晶片是否在中心
  //debug
  if (debug)
  {
    sprintf(szname, "%s\\read.bmp", chPathMain);
    saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
  }
  //
  if (lb_blank != 0)//无可识别区域，提前退出RCGBMP
  {
    delete[] pBmpBuf; pBmpBuf = NULL;
    delete[] xsvalue; xsvalue = NULL;
    delete[] fbvalue; fbvalue = NULL;
    delete[] jgvalue; jgvalue = NULL;
    delete[] txvalue; txvalue = NULL;

    delete[] pColorTable1;//MMY
    pColorTable1 = NULL;//MMY
//		pWnd->ReleaseDC(pDC);

    switch (lb_blank)
    {
    case -1:if (debug && pDC != NULL)pDC->TextOutW(0, 0, _T("无片")); break;//AfxMessageBox(_T("无片"));
    case 1:if (debug && pDC != NULL)pDC->TextOutW(0, 0, _T("单片可能已达到图像边缘,如果想继续检测,只能修改代码:)")); break;
      //AfxMessageBox(_T("单片可能已达到图像边缘,如果想继续检测,只能修改代码:)"));
    }
    return type;//type=-1
  }

  int tempi, tempj, templ, temp_qyno, temp_qy_num, temp_qy_num2, temp_xsvalue;
  int temp_qy_i, temp_qs_h, temp_qs_l, temp_qy_h, temp_qy_l, temp_fx;//,temp_fxnum;
  int* temp_bj_h;//行边界
  int* temp_bj_l;//列边界
  int* temp_bj_fx;//方向

  //temp_bj_h,temp_bj_l:array of integer;
  // temp_bj_fx:array of integer;
  int temp_bj_num = 0;//debug下未置0，导致release下出错，release下是乱值
  int temp_total_bj, temp_close;
  int min_h, max_h, min_l, max_l;
  int* temp_bj_z;
  temp_qyno = 1; //区域处理
  temp_qy_num = 1;
  tempi = -10;
  temp_xsvalue = -1; //区域处理白色

  //储存边界点的坐标
  temp_total_bj = (bmpWidth + bmpHeight) << 3;  //每个边界点数 注意指针释放 delete temp_bj_h，太小了，存不下，当片都在一块时
  temp_bj_z = new int[temp_total_bj];	//边界点的重复处理标记
  temp_bj_h = new int[temp_total_bj];      //边界点的行坐标
  temp_bj_l = new int[temp_total_bj];      //边界点的列坐标
  temp_bj_fx = new int[temp_total_bj];      //边界点的寻找方向
  //遍历整幅图像寻找边界起始点
  for (tempj = 1; tempj <= bmpHeight - 2 && g_stu_square.nN < 20; tempj++)     //行循环
  {
    for (templ = 1; templ <= bmpWidth - 2 && g_stu_square.nN < 20; templ++)   //列循环，寻找边界起始点
    {
      //jg标记中像素不是-1且前一个像素不是0继续下一个循环（包括已经处理的区域），否则可能找到起始点
      if (!((*(jgvalue + tempj * bmpWidth + templ) == temp_xsvalue) && (*(jgvalue + tempj * bmpWidth + templ - 1) == 0)))
        continue;
      //mmy
      /*if(!((*(jgvalue+(tempj-1)*bmpWidth+templ)==-1 || *(jgvalue+(tempj+1)*bmpWidth+templ)==-1)
        &&
        (*(jgvalue+(tempj)*bmpWidth+templ-1)==-1 || *(jgvalue+(tempj)*bmpWidth+templ+1)==-1))
        )*/
        //这句很关键，考虑不周会导致边界的顺序是逆时针的（此程序逆时针会出现很多问题，一些逻辑默认其是顺时针的），或者边界的起点是不希望的地方
      if (!(((*(jgvalue + (tempj + 1) * bmpWidth + templ) == -1) && (*(jgvalue + (tempj)* bmpWidth + templ + 1) == -1)) && (*(jgvalue + (tempj - 1) * bmpWidth + templ) == 0)))
        continue;
      //xs标记的值不是0，表示是已识别出的有效区域
      if (*(xsvalue + tempj * bmpWidth + templ) > 0)
        continue;
      //起始点已找到，开始进行找边界
      temp_close = -1;                          //边界是否闭合的标志，-1表示不闭合，0以上数据表示闭合位置，好像没用上
      temp_bj_num = 0;
      //像素点为-1的进行处理：记录当前位置
      temp_qs_h = tempj;   //当前边界起始行记录
      temp_qs_l = templ;   //当前边界起始列记录
      *(jgvalue + tempj * bmpWidth + templ) = temp_qy_num;//把jg像素标记数组值置成当前区域编号。起始为1
      //fbvalue[tempj][templ]=temp_qy_num;
      temp_fx = 2;   //下一步检查的方向标记，1——向上，2——向右，3——向下，4——向左
      temp_qy_l = templ;    //初始位置设定
      temp_qy_h = tempj;
      //temp_fxnum=0;  //初始执行方向数设定为0
      // tempi=0;
      *temp_bj_h = tempj;  //记录边界起始行坐标到数组
      *temp_bj_l = templ;  //记录边界起始列坐标到数组
      *temp_bj_fx = temp_fx;//记录边界起始方向
      temp_bj_num = 0;   //连续边界点数置0
      *(temp_bj_z + temp_bj_num) = 1;  //第temp_bj_num个连续边界点的重复处理数1
      min_h = bmpHeight; max_h = 0; min_l = bmpWidth; max_l = 0;
      //画边界起始点
      if (debug && pDC != NULL)pDC->Ellipse(temp_qy_l * rect.right / bmpWidth - 2, temp_qy_h * rect.bottom / bmpHeight - 2, temp_qy_l * rect.right / bmpWidth + 2, temp_qy_h * rect.bottom / bmpHeight + 2);
      //pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));
      temp_qy_num2 = temp_qy_num;//
      //找边界循环开始
      for (temp_qy_i = 1; temp_qy_i < temp_total_bj && g_stu_square.nN < 20; temp_qy_i++)
      {
        //::Sleep(1);
        //按方向拿出下一个点
        switch (temp_fx) {
        case 1:
          temp_qy_h = temp_qy_h - 1;        //向上
          break;
        case 2:
          temp_qy_l = temp_qy_l + 1;        //向右
          break;
        case 3:
          temp_qy_h = temp_qy_h + 1;        //向下
          break;
        case 4:
          temp_qy_l = temp_qy_l - 1;        //向左
          break;
        }
        //画检测到的待检验边界点
        //pDC->Ellipse(temp_qy_l*rect.right/bmpWidth-2,temp_qy_h*rect.bottom/bmpHeight-2,temp_qy_l*rect.right/bmpWidth+2,temp_qy_h*rect.bottom/bmpHeight+2);
        //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));
        ++temp_bj_num;//连续边界点数++
        *(temp_bj_z + temp_bj_num) = 0;//当前点的重复处理标记清0，是新点，待检测，对应标记清0
        //mmy
        bool needback = 0;//退回处理标记清0

        if (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) == temp_qy_num)//如果jg当前点已经标记为当前区域，说明有封闭区域产生
        {
          //因为是有效点，必须作为标准边界点保存，由于是结束，所以方向处理可以不做
          //*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)=temp_qy_num;  //将当前像素值修改为当前区域区域标记边界设置//此处貌似多余
          *(temp_bj_h + temp_bj_num) = temp_qy_h;  //行坐标
          *(temp_bj_l + temp_bj_num) = temp_qy_l;  //列坐标
          *(temp_bj_fx + temp_bj_num) = temp_fx;
          *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = temp_fx;  //当前像素值方向，必须改变，防止开始点是别的区域转过来导致填充的时候方向不符合要求而出差错

          if ((temp_qy_l == temp_qs_l) && (temp_qy_h == temp_qs_h))//判断是否起始行列
          {
            //   showmessage('回到起点');
            break;  //回到起点，当前区域的找边界循环结束
          }
          else
          {
            //由于封闭，所以直接退回
            //int nTmpH = temp_qy_h , nTmpL = temp_qy_l;		//保存退回起始行列
            if (debug && pDC != NULL)
              pDC->TextOutW(0, 0, _T("有接触区域"));
            //AfxMessageBox(_T("有接触区域"));
            int nTmpN = 0;								//保存退回边界点数
            int* pnTmpH, *pnTmpL, *pnTmpFx;
            pnTmpH = new int[temp_bj_num];						//保存封闭曲线边界点的行坐标
            pnTmpL = new int[temp_bj_num];						//保存封闭曲线边界点的列坐标
            pnTmpFx = new int[temp_bj_num];						//方向
            temp_qy_num2++;										//备用区域编号++，新区域
            int nMinH = bmpHeight, nMinL = bmpWidth, nMaxH = 0, nMaxL = 0;

            do
            {

              *(pnTmpH + nTmpN) = *(temp_bj_h + temp_bj_num);	//行坐标 
              *(pnTmpL + nTmpN) = *(temp_bj_l + temp_bj_num);	//列坐标

              if (nMinH > * (temp_bj_h + temp_bj_num)) nMinH = *(temp_bj_h + temp_bj_num);
              if (nMaxH < *(temp_bj_h + temp_bj_num)) nMaxH = *(temp_bj_h + temp_bj_num);
              if (nMinL > * (temp_bj_l + temp_bj_num)) nMinL = *(temp_bj_l + temp_bj_num);
              if (nMaxL < *(temp_bj_l + temp_bj_num)) nMaxL = *(temp_bj_l + temp_bj_num);

              *(jgvalue + *(temp_bj_h + temp_bj_num) * bmpWidth + *(temp_bj_l + temp_bj_num)) = temp_qy_num2;  //将当前像素值修改为b色，同时取消区域标记
              //*(txvalue+*(temp_bj_h + temp_bj_num)*bmpWidth+*(temp_bj_l + temp_bj_num))=*(temp_bj_fx+temp_bj_num);  //将当前像素值修改为区域值，说明已找过
              nTmpN++;
              temp_bj_num--;		//总点数-1，回点
            } while (((temp_qy_h != *(temp_bj_h + temp_bj_num)) || (temp_qy_l != *(temp_bj_l + temp_bj_num))) && temp_bj_num != 0);//退回循环 ///临时加上temp_bj_num!=0因为会出现temp_bjnum == -1 而死在这里
            /////////////////////////////////

            //int mj=data_check_fill_bj(pBmpBuf,temp_qy_num2,0,temp_close,pnTmpH,pnTmpL,xsvalue,fbvalue,jgvalue,txvalue,bmpWidth,bmpHeight, 1,bmpHeight-2,1,bmpWidth-2);
            //填充区域并返回区域面积
            int mj = data_check_fill_bj(pBmpBuf, temp_qy_num2, 0, temp_close, pnTmpH, pnTmpL, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, nMinH, nMaxH, nMinL, nMaxL, debug);
            if (debug)
            {
              sprintf(szname, "%s\\tmpbj.bmp", chPathMain);
              saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
            }
            /////////////////////////////////
            temp_qy_h = *(temp_bj_h + temp_bj_num);  //行坐标
            temp_qy_l = *(temp_bj_l + temp_bj_num);  //列坐标
            temp_fx = *(temp_bj_fx + temp_bj_num);
            if (nTmpN > ((bmpWidth + bmpHeight) >> 4) && temp_qy_num2 < 10000)//如果点数符合要求就判断特征
            {
              bool bChk = 1;
              if (circle)
              {

                if (g_stu_square.bJudgeFeature)//如果要匹配特征，则首先判断面积是否符合要求
                {
                  if (mj > ((stuRef.Rad * stuRef.Rad * gd_PI) * 1.5))//如果面积》1.5倍特征面积
                  {
                    g_stu_square.bIsLap = 1;
                    bChk = 0;
                  }
                  if (mj < ((stuRef.Rad * stuRef.Rad * gd_PI) * 0.5))//如果面积《0.5倍特征面积
                    bChk = 0;
                }
                if (bChk)
                {
                  for (int n = 0, x = nTmpN - 1; n < x; n++, x--)//前面是按退回的顺序存放坐标，这里重新倒回来,在判断特征时需要这样
                  {
                    int nTmp = *(pnTmpH + n);
                    *(pnTmpH + n) = *(pnTmpH + x);
                    *(pnTmpH + x) = nTmp;
                    nTmp = *(pnTmpL + n);
                    *(pnTmpL + n) = *(pnTmpL + x);
                    *(pnTmpL + x) = nTmp;
                  }
                  switch (data_check_feature_circle(pBmpBuf, biBitCount, bmpWidth, bmpHeight, nTmpN, pnTmpH, pnTmpL, temp_qy_num2, xsvalue, fbvalue, jgvalue, txvalue, outpointallowlen, defectallowlen, nMinH, nMaxH, nMinL, nMaxL))
                  {
                  case 0:
                    type = 0;
                    break;
                  default:
                    type = -1;
                    break;
                  }
                  //在屏幕上画出边界
                  if (pDC != NULL && type != -1)
                  {
                    for (int n = 0; n < nTmpN; n++)
                      pDC->SetPixel(pnTmpL[n] * rect.right / bmpWidth, pnTmpH[n] * rect.bottom / bmpHeight, RGB(0, 255, 0));
                  }
                  //在屏幕上标注识别结果
                  if (pDC != NULL && debug)
                    switch (type)
                    {
                    case 0:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("正常"));
                      break;
                    case -1:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("未识别"));
                      break;
                    default:break;
                    }

                }
              }
              else
              {
                if (g_stu_square.bJudgeFeature)
                {
                  if (mj > (stuRef.Len * stuRef.Wth * 1.5))
                  {
                    g_stu_square.bIsLap = 1;
                    bChk = 0;
                  }
                  if (mj < (stuRef.Len * stuRef.Wth * 0.5))
                    bChk = 0;
                }
                if (bChk)
                {
                  for (int n = 0, x = nTmpN - 1; n < x; n++, x--)//前面是按退回的顺序存放坐标，这里重新倒回来,在判断特征时需要这样
                  {
                    int nTmp = *(pnTmpH + n);
                    *(pnTmpH + n) = *(pnTmpH + x);
                    *(pnTmpH + x) = nTmp;
                    nTmp = *(pnTmpL + n);
                    *(pnTmpL + n) = *(pnTmpL + x);
                    *(pnTmpL + x) = nTmp;
                  }
                  switch (data_check_feature(pBmpBuf, biBitCount, bmpWidth, bmpHeight, nTmpN, pnTmpH, pnTmpL, temp_qy_num2, xsvalue, fbvalue, jgvalue, txvalue, outpointallowlen, defectallowlen))
                  {
                  case 0://if(debug)
                        // AfxMessageBox(_T("正常"));
                    type = 0;
                    break;
                  case 1://if(debug)
                         //AfxMessageBox(_T("形状有问题"));
                    type = -1;
                    break;
                  case 2://if(debug)
                        // AfxMessageBox(_T("少块"));
                    type = 2;
                    break;
                  case 3://if(debug)
                        // AfxMessageBox(_T("多块"));
                    type = 1;
                    break;
                  case 4://if(debug)
                        // AfxMessageBox(_T("线段没找全"));
                    type = -1;
                    break;
                  case 5://if(debug)
                      //AfxMessageBox(_T("不是正方形"));
                    type = -1;//毛坯片一般没有角度要求
                    break;
                  default:break;
                  }
                  //int l_issquare=IsSquare(88,92);
                  if (pDC != NULL && type != -1)
                  {
                    for (int n = 0; n < nTmpN; n++)
                      pDC->SetPixel(pnTmpL[n] * rect.right / bmpWidth, pnTmpH[n] * rect.bottom / bmpHeight, RGB(0, 255, 0));

                  }

                  if (pDC != NULL && debug)
                    switch (type)
                    {
                    case 0:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("正常"));
                      break;
                    case -1:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("未识别"));
                      break;
                    case 2:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("少块"));//AfxMessageBox(_T("少块"));
                      break;
                    case 1:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("多块"));//AfxMessageBox(_T("多块"));
                      break;
                    default:break;
                    }

                }
              }
            }

            delete[] pnTmpH; pnTmpH = NULL;
            delete[] pnTmpL; pnTmpL = NULL;
            delete[] pnTmpFx; pnTmpFx = NULL;
            needback = 1;
          }
        }

        //
        //先判断本身是否为0，如果是退回并重新设定方向，如果不是继续
        //mmy增加判断是否是已经检测过的区域
        if ((temp_qy_h < 1) ||
          (temp_qy_h + 1 >= bmpHeight) ||
          (temp_qy_l < 1) ||
          (temp_qy_l + 1 >= bmpWidth) ||
          (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) != -1) ||//&&*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)!=temp_qy_num)||//第二个判定取消mmy
          *(xsvalue + temp_qy_h * bmpWidth + temp_qy_l) > 0 || //是别的区域
          *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) > -1//或者已经是找过的区域
          )//||(*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)>0&&*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)<temp_qy_num))     //新的一点为0，退回上一点，并将方向+1
        {//如果超界或者当前的，或者/、像素标记数组值不是-1且//像素标记数组值不是当前区域编号，即已标记
          needback = 1;
        }
        else if ((*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l + 1)) *
          (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l + 1)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l + 1)) *
          (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l - 1)) *
          (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l - 1)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l - 1)) != 0
          )//如果没黑色
        {
          if (abs((*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l + 1)) *
            (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l + 1)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l + 1)) *
            (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l - 1)) *
            (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l - 1)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l - 1))
          ) == 1)//全白色
            needback = 1;
        }

        //上面循环是退回处理，如果不需要退回，往下执行
        //temp_fxnum=0;//方向数置0？？？
        /*
        //判断是否回到了已经处理的边界，即是否形成了闭合
        temp_close=bj_close(temp_bj_h,temp_bj_l,temp_bj_num,temp_qy_h,temp_qy_l);  //计算是否闭合
         //保存已找到的边界点行列数组，连续边界点数，起始当前边界的区域起始行列
        //if( (temp_close>=0) && (temp_close<=temp_bj_num-2))//mmy取消注释
          //break;
        //非为0，先判断是否为已经计算
        if (*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)==temp_qy_num)//如果当前点是当前区域的一部分
        {
          if ((temp_qy_l==temp_qs_l) && (temp_qy_h==temp_qs_h) )//判断是否起始行列
          {
            //   showmessage('回到起点');
            break;  //回到起点，当前区域的找边界循环结束
          }
        }
        *///mmy
        //检查周边8点中是否有零，如果没有即为非边界，退回 (*(jgvalue+(temp_qy_h+1)*bmpWidth+temp_qy_l))
        if (needback)
        {//退回处理
          //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,0,0));//mmy

          switch (temp_fx) {
          case 1:
            temp_qy_h = temp_qy_h + 1;       //向上
            break;
          case 2:
            temp_qy_l = temp_qy_l - 1;       //向右
            break;
          case 3:
            temp_qy_h = temp_qy_h - 1;        //向下
            break;
          case 4:
            temp_qy_l = temp_qy_l + 1;        //向左
            break;
          }
          //去除不是边界的标识
            //pDC->Ellipse(temp_qy_l*rect.right/bmpWidth-2,temp_qy_h*rect.bottom/bmpHeight-2,temp_qy_l*rect.right/bmpWidth+2,temp_qy_h*rect.bottom/bmpHeight+2);
            //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));//mmy

          --temp_bj_num;//连续边界点数--
          if (temp_bj_num < 0)
            AfxMessageBox(_T("TEMPBJNUM<0"));
          *(temp_bj_z + temp_bj_num) = *(temp_bj_z + temp_bj_num) + 1;//MMY
          temp_fx = temp_fx + 1;
          if (temp_fx == 5)
            temp_fx = 1;
          ////////////////////mmy，以前没有//该点重做了3次，取消边界标记并退回处理
          bool needend = 0;
          while (3 <= *(temp_bj_z + temp_bj_num))//第temp_bj_num个连续边界点的重复处理数为3次
          {
            *(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) = -1;  //将当前像素值修改b色，同时取消区域标记，不能改黑，会导致jg数组改变引起下一次循环误判该点
            *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = 0;  //将当前像素值方向置0，表示处理过方向
            //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,0,0));//mmy

            if (0 == temp_bj_num) {//如果连续边界点数只有0个，退出找边界循环
              needend = 1;
              break;
            }
            switch (temp_fx) {
            case 1:
              temp_qy_h = temp_qy_h - 1;        //向上
              break;
            case 2:
              temp_qy_l = temp_qy_l + 1;        //向右
              break;
            case 3:
              temp_qy_h = temp_qy_h + 1;        //向下
              break;
            case 4:
              temp_qy_l = temp_qy_l - 1;        //向左
              break;
            }
            //pDC->Ellipse(temp_qy_l*rect.right/bmpWidth-2,temp_qy_h*rect.bottom/bmpHeight-2,temp_qy_l*rect.right/bmpWidth+2,temp_qy_h*rect.bottom/bmpHeight+2);
      //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));//
            --temp_bj_num;//连续边界点数--
            if (temp_bj_num < 0)
              AfxMessageBox(_T("TEMPBJNUM<0"));
            *(temp_bj_z + temp_bj_num) = *(temp_bj_z + temp_bj_num) + 1;//MMY
            temp_fx = (temp_fx + 3);//mmy从2改为3
            if (temp_fx >= 5)	temp_fx = temp_fx - 4;
          }
          ////////////////////
          if (needend)
          {
            break;
          }
          continue;
        }
        else
        {   //，不退回处理有零存在 ,是边界
          //temp_bj_num=temp_bj_num+1;//连续边界点数++//当增加needback一些内容后出现图像不能回退成功，发现tempbjnum每次都会多加一次
        //if(!debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));//
          *(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) = temp_qy_num;  //将当前像素值修改为当前区域区域标记边界设置
          tempi = tempi + 1;//不解？？？？？？？
          *(temp_bj_h + temp_bj_num) = temp_qy_h;  //行坐标
          *(temp_bj_l + temp_bj_num) = temp_qy_l;  //列坐标
          *(temp_bj_fx + temp_bj_num) = temp_fx;
          *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = temp_fx;  //当前像素值方向
          if (min_h > temp_qy_h) min_h = temp_qy_h;
          if (max_h < temp_qy_h) max_h = temp_qy_h;
          if (min_l > temp_qy_l) min_l = temp_qy_l;
          if (max_l < temp_qy_l) max_l = temp_qy_l;
          temp_fx = (temp_fx - 1);
          if (temp_fx == 0)
            temp_fx = 4;  //循环
        }
      }//找边界循环2结束

      if (temp_bj_num == 0 || g_stu_square.nN >= 20) {//如果连续边界点数只有0，退出，
        *(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) = -1;//将当前像素值修改为b色，同时取消区域标记
        *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = 0;  //将当前像素值方向置0
        //if(debug && pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,0,0));//mmy

        //continue;//继续下一行列循环//不知道要不要continue
      }
      else
      {
        //data_check_fill_bj(pBmpBuf,temp_qy_num,0,temp_close,temp_bj_h,temp_bj_l,xsvalue,fbvalue,jgvalue,txvalue,bmpWidth,bmpHeight, min_h,max_h,min_l,max_l);
        //xsvalue,fbvalue,jgvalue,txvalue只有jgvalue有改，tempqynum的初始值是1
        //int mj=data_check_fill_bj(pBmpBuf,temp_qy_num,0,temp_close,temp_bj_h,temp_bj_l,xsvalue,fbvalue,jgvalue,txvalue,bmpWidth,bmpHeight, 1,bmpHeight-2,1,bmpWidth-2);
        int mj = data_check_fill_bj(pBmpBuf, temp_qy_num, 0, temp_close, temp_bj_h, temp_bj_l, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, min_h, max_h, min_l, max_l, debug);
        if (debug)
        {
          sprintf(szname, "%s\\tmpbj.bmp", chPathMain);
          saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
        }
        //if(temp_bj_num>(bmpWidth+bmpHeight)/3)
        if (temp_bj_num > (bmpWidth + bmpHeight) >> 4)
        {
          bool bChk = 1;
          if (circle)
          {
            if (g_stu_square.bJudgeFeature)//如果要匹配特征，则首先判断面积是否符合要求
            {
              if (mj > ((stuRef.Rad * stuRef.Rad * gd_PI) * 1.5))//如果面积》1.5倍特征面积
              {
                g_stu_square.bIsLap = 1;
                bChk = 0;
              }
              if (mj < ((stuRef.Rad * stuRef.Rad * gd_PI) * 0.5))//如果面积《0.5倍特征面积
                bChk = 0;
            }
            /*if (g_stu_square.bJudgeFeature)
            {
              if (mj>((stuRef.Rad*stuRef.Rad*gd_PI)*1.5))
              {
                g_stu_square.bIsLap=1;
              }
            }*/
            if (bChk)
            {
              switch (data_check_feature_circle(pBmpBuf, biBitCount, bmpWidth, bmpHeight, temp_bj_num, temp_bj_h, temp_bj_l, temp_qy_num, xsvalue, fbvalue, jgvalue, txvalue, outpointallowlen, defectallowlen, min_h, max_h, min_l, max_l))
              {
              case 0:
                type = 0;
                break;
              default:
                type = -1;
                break;
              }

              if (pDC != NULL && type != -1)
              {
                for (int n = 0; n < temp_bj_num; n++)
                  pDC->SetPixel(temp_bj_l[n] * rect.right / bmpWidth, temp_bj_h[n] * rect.bottom / bmpHeight, RGB(0, 255, 0));
              }

              if (pDC != NULL && debug)
                switch (type)
                {
                case 0:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("正常"));
                  break;
                case -1:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("未识别"));
                  break;
                default:break;
                }
            }
          }
          else
          {
            if (g_stu_square.bJudgeFeature)
            {
              if (mj > (stuRef.Len * stuRef.Wth * 1.5))
              {
                g_stu_square.bIsLap = 1;
                bChk = 0;
              }
              if (mj < (stuRef.Len * stuRef.Wth * 0.5))
                bChk = 0;
            }
            /*if (g_stu_square.bJudgeFeature)
            {
              if (mj>(stuRef.Len*stuRef.Wth<<1))
              {
                g_stu_square.bIsLap=1;
              }
            }*/
            if (bChk)
            {
              switch (data_check_feature(pBmpBuf, biBitCount, bmpWidth, bmpHeight, temp_bj_num, temp_bj_h, temp_bj_l, temp_qy_num, xsvalue, fbvalue, jgvalue, txvalue, outpointallowlen, defectallowlen))
              {
              case 0://if(debug)
                    // AfxMessageBox(_T("正常"));
                type = 0;
                break;
              case 1://if(debug)
                    //AfxMessageBox(_T("形状有问题"));
                type = -1;
                break;
              case 2://if(debug)
                    // AfxMessageBox(_T("少块"));
                type = 2;
                break;
              case 3://if(debug)
                    // AfxMessageBox(_T("多块"));
                type = 1;
                break;
              case 4://if(debug)
                    // AfxMessageBox(_T("线段没找全"));
                type = -1;
                break;
              case 5://if(debug)
                  //AfxMessageBox(_T("不是正方形"));
                type = -1;//毛坯片一般没有角度要求
                break;
              default:break;
              }
              //int l_issquare=IsSquare(88,92);
              if (pDC != NULL && type != -1)
              {
                for (int n = 0; n < temp_bj_num; n++)
                  pDC->SetPixel(temp_bj_l[n] * rect.right / bmpWidth, temp_bj_h[n] * rect.bottom / bmpHeight, RGB(0, 255, 0));


              }

              if (pDC != NULL && debug)
                switch (type)
                {
                case 0:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("正常"));
                  break;
                case -1:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("未识别"));
                  break;
                case 2:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("少块"));//AfxMessageBox(_T("少块"));
                  break;
                case 1:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("多块"));//AfxMessageBox(_T("多块"));
                  break;
                default:break;
                }
            }

          }
          //int zc=temp_bj_num;//按图像来说，不一定边界点数就==周长，周长要等算出边长之后再来考虑
          if (debug) {
            sprintf(szname, "%s\\result%d.bmp", chPathMain, temp_qy_num);
            saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
          }
          //if(!debug)
          //	break;
        }
      }
      //g_tmp_path=_T(".\\PIC\\temp1.bmp");
      temp_qy_num = temp_qy_num2 + 1;//一个区域检测完成，区域数++，
      temp_qy_num2 = temp_qy_num;
      //int i_count_mouse=temp_qy_num;
      //UpdateData(0);
      if (!debug)
        if (temp_qy_num > 10000)
          break;
    }   //列循环结束
    //
    {
      /*if(temp_bj_num>(bmpWidth+bmpHeight)*2/3)
        break;*/

    }
    if (!debug)
      if (temp_qy_num > 10000)
        break;
  }  //行循环结束
  //saveBmp("temps.bmp",pBmpBuf,bmpWidth,bmpHeight,biBitCount,pColorTable1);
  if (debug && pDC != NULL)
  {
    pDC->TextOutW(0, rect.bottom - 15, _T("正常结束"));//AfxMessageBox(_T("识别结束"));
  }
  delete[] temp_bj_h; temp_bj_h = NULL;
  delete[] temp_bj_l; temp_bj_l = NULL;
  delete[] temp_bj_fx; temp_bj_fx = NULL;
  delete[] temp_bj_z; temp_bj_z = NULL;


  //pDC->SetPixel(int(100*rect.right/bmpWidth),int(100*rect.bottom/bmpHeight),RGB(255,0,0));//点画

  //pDC->Ellipse(100*rect.right/bmpWidth-2,100*rect.bottom/bmpHeight-2,100*rect.right/bmpWidth+2,100*rect.bottom/bmpHeight+2);


  delete[] pBmpBuf; pBmpBuf = NULL;
  delete[] xsvalue; xsvalue = NULL;
  delete[] fbvalue; fbvalue = NULL;
  delete[] jgvalue; jgvalue = NULL;
  delete[] txvalue; txvalue = NULL;

  delete[] pColorTable1;//MMY
  pColorTable1 = NULL;//MMY
  return type;
}
//与RCGBMP一样，当修改时同步修改，只在调用圆正方形特征识别时不一样
int CIMGRecognition::RCGBMPSPLIT(CDC * pDC, CRect rect, char* bmpName, int outpointallowlen = 2, int defectallowlen = 2, int threshold = 120, bool debug = 0, bool circle = 0, bool bAutoThrd = 0, bool delNoise = 0)
{
  CBrush* pOldBrush = NULL;
  if (pDC != NULL)
  {
    pDC->SetTextColor(RGB(255, 0, 255));
    pDC->SetBkMode(TRANSPARENT);
    pDC->SelectObject(&font);
    //pDC ->TextOutW(0,0,_T("123456789"));
    pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
  }
  m_bmp_file = bmpName;
  g_stu_square.debug = debug;
  unsigned char* pBmpBuf = NULL;
  int* xsvalue = NULL;
  int* fbvalue = NULL;
  int* jgvalue = NULL;
  int* txvalue = NULL;
  int bmpWidth, bmpHeight;
  g_stu_square.nN = 0;//已识别数置0
  g_stu_square.bIsLap = 0;//重叠表示z置0
  int type = -1;//识别类型置-1
  //char * readPath=NULL;//"temps2.bmp";
  char readPath[MAX_PATH];
  //WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_bmp_file,-1,readPath,CStringA(m_bmp_file).GetLength(),NULL,NULL); 
  WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, m_bmp_file, -1, readPath, sizeof(readPath), NULL, NULL);
  //tempstr=m_bmp_file.GetBuffer(CStringA(m_bmp_file).GetLength());
  int nRd = readBmp(readPath, pBmpBuf, bmpWidth, bmpHeight); //8\24w
  if (nRd != 0)
  {
    return type;
  }
  if (bAutoThrd)//如果指定自动阈值，进行自动阈值
  {
    int nTmpThrd = read_image_fz(pBmpBuf, bmpWidth, bmpHeight);//检测当前图像的中间阈值
    if (nTmpThrd != -1)
      threshold = nTmpThrd;
  }
  int pos = m_bmp_file.ReverseFind('\\');
  m_bmp_file = m_bmp_file.Left(pos);
  WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, m_bmp_file, -1, chPathMain, sizeof(chPathMain), NULL, NULL);
  //保存图像尺寸到全局
  g_stu_square.nBMPH = bmpHeight;
  g_stu_square.nBMPW = bmpWidth;
  //saveBmp("temps.bmp",pBmpBuf,bmpWidth,bmpHeight,biBitCount,NULL);
  //m_bmp_file.ReleaseBuffer();
  /*
  readPath=m_bmp_file.GetBuffer(m_bmp_file.GetLength());
  readBmp(readPath,pBmpBuf,bmpWidth,bmpHeight);
  m_bmp_file.ReleaseBuffer();*/
  int temp_bmp_data = bmpWidth * bmpHeight;

  xsvalue = new int[temp_bmp_data]; //全0//最后是边界加区域数组，值是对应的区域编号
  //memset(xsvalue,0,temp_bmp_data);
  fbvalue = new int[temp_bmp_data]; //全0//最后是边界数组，边界值是对应的区域编号,在图片预处理中有用，标记已预处理
  //memset(fbvalue,0,temp_bmp_data);
  jgvalue = new int[temp_bmp_data]; //白的-1，黑的0//用于边界处理的数组
  txvalue = new int[temp_bmp_data]; //白的-1，黑的0//如果当前点是边界n的找过的点，标记n//改为记录方向，为填充区域提供信息
  int lb_blank = 0;
  //二值化处理，识别多片可以考虑不判断极值，直接二值化，更改readimagefz

  if (1)//xs，fb全0；jg，tx白-1，黑0；
    lb_blank = read_image_mul(pBmpBuf, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, threshold, debug, delNoise);//按照阈值二值化图像，去噪
  else
    lb_blank = read_image_blank(pBmpBuf, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, threshold);//判断晶片是否在中心
  //debug
  if (debug)
  {
    sprintf(szname, "%s\\read.bmp", chPathMain);
    saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
  }
  //
  if (lb_blank != 0)//无可识别区域，提前退出RCGBMP
  {
    delete[] pBmpBuf; pBmpBuf = NULL;
    delete[] xsvalue; xsvalue = NULL;
    delete[] fbvalue; fbvalue = NULL;
    delete[] jgvalue; jgvalue = NULL;
    delete[] txvalue; txvalue = NULL;

    delete[] pColorTable1;//MMY
    pColorTable1 = NULL;//MMY
//		pWnd->ReleaseDC(pDC);
    if (pDC != NULL)
      pDC->SelectObject(pOldBrush);
    switch (lb_blank)
    {
    case -1:if (debug && pDC != NULL)pDC->TextOutW(0, 0, _T("无片")); break;//AfxMessageBox(_T("无片"));
    case 1:if (debug && pDC != NULL)pDC->TextOutW(0, 0, _T("单片可能已达到图像边缘,如果想继续检测,只能修改代码:)")); break;
      //AfxMessageBox(_T("单片可能已达到图像边缘,如果想继续检测,只能修改代码:)"));
    }
    return type;//type=-1
  }

  int tempi, tempj, templ, temp_qyno, temp_qy_num, temp_qy_num2, temp_xsvalue;
  int temp_qy_i, temp_qs_h, temp_qs_l, temp_qy_h, temp_qy_l, temp_fx;//,temp_fxnum;
  int* temp_bj_h;//行边界
  int* temp_bj_l;//列边界
  int* temp_bj_fx;//方向

  //temp_bj_h,temp_bj_l:array of integer;
  // temp_bj_fx:array of integer;
  int temp_bj_num = 0;//debug下未置0，导致release下出错，release下是乱值
  int temp_total_bj, temp_close;
  int min_h, max_h, min_l, max_l;
  int* temp_bj_z;
  temp_qyno = 1; //区域处理
  temp_qy_num = 1;
  tempi = -10;
  temp_xsvalue = -1; //区域处理白色

  //储存边界点的坐标
  temp_total_bj = (bmpWidth + bmpHeight) << 3;  //每个边界点数 注意指针释放 delete temp_bj_h，太小了，存不下，当片都在一块时
  temp_bj_z = new int[temp_total_bj];	//边界点的重复处理标记
  temp_bj_h = new int[temp_total_bj];      //边界点的行坐标
  temp_bj_l = new int[temp_total_bj];      //边界点的列坐标
  temp_bj_fx = new int[temp_total_bj];      //边界点的寻找方向
  //遍历整幅图像寻找边界起始点
  for (tempj = 1; tempj <= bmpHeight - 2 && g_stu_square.nN < 20; tempj++)     //行循环
  {
    for (templ = 1; templ <= bmpWidth - 2 && g_stu_square.nN < 20; templ++)   //列循环，寻找边界起始点
    {
      //jg标记中像素不是-1且前一个像素不是0继续下一个循环（包括已经处理的区域），否则可能找到起始点
      if (!((*(jgvalue + tempj * bmpWidth + templ) == temp_xsvalue) && (*(jgvalue + tempj * bmpWidth + templ - 1) == 0)))
        continue;
      //mmy
      /*if(!((*(jgvalue+(tempj-1)*bmpWidth+templ)==-1 || *(jgvalue+(tempj+1)*bmpWidth+templ)==-1)
        &&
        (*(jgvalue+(tempj)*bmpWidth+templ-1)==-1 || *(jgvalue+(tempj)*bmpWidth+templ+1)==-1))
        )*/
        //这句很关键，考虑不周会导致边界的顺序是逆时针的（此程序逆时针会出现很多问题，一些逻辑默认其是顺时针的），或者边界的起点是不希望的地方
      if (!(((*(jgvalue + (tempj + 1) * bmpWidth + templ) == -1) && (*(jgvalue + (tempj)* bmpWidth + templ + 1) == -1)) && (*(jgvalue + (tempj - 1) * bmpWidth + templ) == 0)))
        continue;
      //xs标记的值不是0，表示是已识别出的有效区域
      if (*(xsvalue + tempj * bmpWidth + templ) > 0)
        continue;
      //起始点已找到，开始进行找边界
      temp_close = -1;                          //边界是否闭合的标志，-1表示不闭合，0以上数据表示闭合位置，好像没用上
      temp_bj_num = 0;
      //像素点为-1的进行处理：记录当前位置
      temp_qs_h = tempj;   //当前边界起始行记录
      temp_qs_l = templ;   //当前边界起始列记录
      *(jgvalue + tempj * bmpWidth + templ) = temp_qy_num;//把jg像素标记数组值置成当前区域编号。起始为1
      //fbvalue[tempj][templ]=temp_qy_num;
      temp_fx = 2;   //下一步检查的方向标记，1——向上，2——向右，3——向下，4——向左
      temp_qy_l = templ;    //初始位置设定
      temp_qy_h = tempj;
      //temp_fxnum=0;  //初始执行方向数设定为0
      // tempi=0;
      *temp_bj_h = tempj;  //记录边界起始行坐标到数组
      *temp_bj_l = templ;  //记录边界起始列坐标到数组
      *temp_bj_fx = temp_fx;//记录边界起始方向
      temp_bj_num = 0;   //连续边界点数置0
      *(temp_bj_z + temp_bj_num) = 1;  //第temp_bj_num个连续边界点的重复处理数1
      min_h = bmpHeight; max_h = 0; min_l = bmpWidth; max_l = 0;
      //画边界起始点
      if (debug && pDC != NULL)pDC->Ellipse(temp_qy_l * rect.right / bmpWidth - 2, temp_qy_h * rect.bottom / bmpHeight - 2, temp_qy_l * rect.right / bmpWidth + 2, temp_qy_h * rect.bottom / bmpHeight + 2);
      //pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));
      temp_qy_num2 = temp_qy_num;//
      //找边界循环开始
      for (temp_qy_i = 1; temp_qy_i < temp_total_bj && g_stu_square.nN < 20; temp_qy_i++)
      {
        //::Sleep(1);
        //按方向拿出下一个点
        switch (temp_fx) {
        case 1:
          temp_qy_h = temp_qy_h - 1;        //向上
          break;
        case 2:
          temp_qy_l = temp_qy_l + 1;        //向右
          break;
        case 3:
          temp_qy_h = temp_qy_h + 1;        //向下
          break;
        case 4:
          temp_qy_l = temp_qy_l - 1;        //向左
          break;
        }
        //画检测到的待检验边界点
        //pDC->Ellipse(temp_qy_l*rect.right/bmpWidth-2,temp_qy_h*rect.bottom/bmpHeight-2,temp_qy_l*rect.right/bmpWidth+2,temp_qy_h*rect.bottom/bmpHeight+2);
        //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));
        ++temp_bj_num;//连续边界点数++
        *(temp_bj_z + temp_bj_num) = 0;//当前点的重复处理标记清0，是新点，待检测，对应标记清0
        //mmy
        bool needback = 0;//退回处理标记清0

        if (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) == temp_qy_num)//如果jg当前点已经标记为当前区域，说明有封闭区域产生
        {
          //因为是有效点，必须作为标准边界点保存，由于是结束，所以方向处理可以不做
          //*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)=temp_qy_num;  //将当前像素值修改为当前区域区域标记边界设置//此处貌似多余
          *(temp_bj_h + temp_bj_num) = temp_qy_h;  //行坐标
          *(temp_bj_l + temp_bj_num) = temp_qy_l;  //列坐标
          *(temp_bj_fx + temp_bj_num) = temp_fx;
          *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = temp_fx;  //当前像素值方向，必须改变，防止开始点是别的区域转过来导致填充的时候方向不符合要求而出差错

          if ((temp_qy_l == temp_qs_l) && (temp_qy_h == temp_qs_h))//判断是否起始行列
          {
            //   showmessage('回到起点');
            break;  //回到起点，当前区域的找边界循环结束
          }
          else
          {
            //由于封闭，所以直接退回
            //int nTmpH = temp_qy_h , nTmpL = temp_qy_l;		//保存退回起始行列
            if (debug && pDC != NULL)
              pDC->TextOutW(0, 0, _T("有接触区域"));
            //AfxMessageBox(_T("有接触区域"));
            int nTmpN = 0;								//保存退回边界点数
            int* pnTmpH, *pnTmpL, *pnTmpFx;
            pnTmpH = new int[temp_bj_num];						//保存封闭曲线边界点的行坐标
            pnTmpL = new int[temp_bj_num];						//保存封闭曲线边界点的列坐标
            pnTmpFx = new int[temp_bj_num];						//方向
            temp_qy_num2++;										//备用区域编号++，新区域
            int nMinH = bmpHeight, nMinL = bmpWidth, nMaxH = 0, nMaxL = 0;

            do
            {

              *(pnTmpH + nTmpN) = *(temp_bj_h + temp_bj_num);	//行坐标 
              *(pnTmpL + nTmpN) = *(temp_bj_l + temp_bj_num);	//列坐标
              *(pnTmpFx + nTmpN) = *(temp_bj_fx + temp_bj_num);	//列坐标
              if (nMinH > * (temp_bj_h + temp_bj_num)) nMinH = *(temp_bj_h + temp_bj_num);
              if (nMaxH < *(temp_bj_h + temp_bj_num)) nMaxH = *(temp_bj_h + temp_bj_num);
              if (nMinL > * (temp_bj_l + temp_bj_num)) nMinL = *(temp_bj_l + temp_bj_num);
              if (nMaxL < *(temp_bj_l + temp_bj_num)) nMaxL = *(temp_bj_l + temp_bj_num);

              *(jgvalue + *(temp_bj_h + temp_bj_num) * bmpWidth + *(temp_bj_l + temp_bj_num)) = temp_qy_num2;  //将当前像素值修改为b色，同时取消区域标记
              //*(txvalue+*(temp_bj_h + temp_bj_num)*bmpWidth+*(temp_bj_l + temp_bj_num))=*(temp_bj_fx+temp_bj_num);  //将当前像素值修改为区域值，说明已找过
              nTmpN++;
              temp_bj_num--;		//总点数-1，回点
            } while (((temp_qy_h != *(temp_bj_h + temp_bj_num)) || (temp_qy_l != *(temp_bj_l + temp_bj_num))) && temp_bj_num != 0);//退回循环 ///临时加上temp_bj_num!=0因为会出现temp_bjnum == -1 而死在这里
            /////////////////////////////////

            //int mj=data_check_fill_bj(pBmpBuf,temp_qy_num2,0,temp_close,pnTmpH,pnTmpL,xsvalue,fbvalue,jgvalue,txvalue,bmpWidth,bmpHeight, 1,bmpHeight-2,1,bmpWidth-2);
            //填充区域并返回区域面积
            int mj = data_check_fill_bj(pBmpBuf, temp_qy_num2, 0, temp_close, pnTmpH, pnTmpL, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, nMinH, nMaxH, nMinL, nMaxL, debug);
            if (debug)
            {
              sprintf(szname, "%s\\tmpbj.bmp", chPathMain);
              saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
            }
            /////////////////////////////////
            temp_qy_h = *(temp_bj_h + temp_bj_num);  //行坐标
            temp_qy_l = *(temp_bj_l + temp_bj_num);  //列坐标
            temp_fx = *(temp_bj_fx + temp_bj_num);
            if (nTmpN > ((bmpWidth + bmpHeight) >> 4) && temp_qy_num2 < 10000)//如果点数符合要求就判断特征
            {
              bool bChk = 1;
              if (circle)
              {

                if (g_stu_square.bJudgeFeature)//如果要匹配特征，则首先判断面积是否符合要求
                {
                  if (mj > ((stuRef.Rad * stuRef.Rad * gd_PI) * 1.5))//如果面积》1.5倍特征面积
                  {
                    g_stu_square.bIsLap = 1;
                    bChk = 0;
                  }
                  if (mj < ((stuRef.Rad * stuRef.Rad * gd_PI) * 0.5))//如果面积《0.5倍特征面积
                    bChk = 0;
                }
                if (bChk)
                {
                  for (int n = 0, x = nTmpN - 1; n < x; n++, x--)//前面是按退回的顺序存放坐标，这里重新倒回来,在判断特征时需要这样
                  {
                    int nTmp = *(pnTmpH + n);
                    *(pnTmpH + n) = *(pnTmpH + x);
                    *(pnTmpH + x) = nTmp;
                    nTmp = *(pnTmpL + n);
                    *(pnTmpL + n) = *(pnTmpL + x);
                    *(pnTmpL + x) = nTmp;
                    nTmp = *(pnTmpFx + n);
                    *(pnTmpFx + n) = *(pnTmpFx + x);
                    *(pnTmpFx + x) = nTmp;
                  }
                  switch (data_check_feature_circle_split(pBmpBuf, biBitCount, bmpWidth, bmpHeight, nTmpN, pnTmpH, pnTmpL, pnTmpFx, temp_qy_num2, xsvalue, fbvalue, jgvalue, txvalue, outpointallowlen, defectallowlen, nMinH, nMaxH, nMinL, nMaxL))
                  {
                  case 0:
                    type = 0;
                    break;
                  default:
                    type = -1;
                    break;
                  }
                  //在屏幕上画出边界
                /*	if(pDC!=NULL && type!=-1)
                  {
                    for(int n = 0; n<nTmpN; n++)
                      pDC->SetPixel(pnTmpL[n]*rect.right/bmpWidth,pnTmpH[n]*rect.bottom/bmpHeight,RGB(0,255,0));
                  }
                  //在屏幕上标注识别结果
                  if(pDC!=NULL &&debug)
                  switch(type)
                  {
                  case 0:
                    pDC->TextOutW(pnTmpL[0]*rect.right/bmpWidth,pnTmpH[0]*rect.bottom/bmpHeight,_T("正常"));
                    break;
                  case -1:
                    pDC->TextOutW(pnTmpL[0]*rect.right/bmpWidth,pnTmpH[0]*rect.bottom/bmpHeight,_T("未识别"));
                    break;
                  default:break;
                  }*/

                }
              }
              else
              {
                if (g_stu_square.bJudgeFeature)
                {
                  if (mj > (stuRef.Len * stuRef.Wth * 1.5))
                  {
                    g_stu_square.bIsLap = 1;
                    bChk = 0;
                  }
                  if (mj < (stuRef.Len * stuRef.Wth * 0.5))
                    bChk = 0;
                }
                if (bChk)
                {
                  for (int n = 0, x = nTmpN - 1; n < x; n++, x--)//前面是按退回的顺序存放坐标，这里重新倒回来,在判断特征时需要这样
                  {
                    int nTmp = *(pnTmpH + n);
                    *(pnTmpH + n) = *(pnTmpH + x);
                    *(pnTmpH + x) = nTmp;
                    nTmp = *(pnTmpL + n);
                    *(pnTmpL + n) = *(pnTmpL + x);
                    *(pnTmpL + x) = nTmp;
                    nTmp = *(pnTmpFx + n);
                    *(pnTmpFx + n) = *(pnTmpFx + x);
                    *(pnTmpFx + x) = nTmp;
                  }
                  switch (data_check_feature_split(pBmpBuf, biBitCount, bmpWidth, bmpHeight, nTmpN, pnTmpH, pnTmpL, pnTmpFx, temp_qy_num2, xsvalue, fbvalue, jgvalue, txvalue, outpointallowlen, defectallowlen))
                  {
                  case 0://if(debug)
                        // AfxMessageBox(_T("正常"));
                    type = 0;
                    break;
                  case 1://if(debug)
                         //AfxMessageBox(_T("形状有问题"));
                    type = -1;
                    break;
                  case 2://if(debug)
                        // AfxMessageBox(_T("少块"));
                    type = 2;
                    break;
                  case 3://if(debug)
                        // AfxMessageBox(_T("多块"));
                    type = 1;
                    break;
                  case 4://if(debug)
                        // AfxMessageBox(_T("线段没找全"));
                    type = -1;
                    break;
                  case 5://if(debug)
                      //AfxMessageBox(_T("不是正方形"));
                    type = -1;//毛坯片一般没有角度要求
                    break;
                  default:break;
                  }
                  //int l_issquare=IsSquare(88,92);
                  /*if(pDC!=NULL && type!=-1)
                  {
                    for(int n = 0; n<nTmpN; n++)
                      pDC->SetPixel(pnTmpL[n]*rect.right/bmpWidth,pnTmpH[n]*rect.bottom/bmpHeight,RGB(0,255,0));

                  }*/

                  if (pDC != NULL && debug)
                    switch (type)
                    {
                    case 0:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("正常"));
                      break;
                    case -1:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("未识别"));
                      break;
                    case 2:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("少块"));//AfxMessageBox(_T("少块"));
                      break;
                    case 1:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("多块"));//AfxMessageBox(_T("多块"));
                      break;
                    default:break;
                    }

                }
              }
            }

            delete[] pnTmpH; pnTmpH = NULL;
            delete[] pnTmpL; pnTmpL = NULL;
            delete[] pnTmpFx; pnTmpFx = NULL;
            needback = 1;
          }
        }

        //
        //先判断本身是否为0，如果是退回并重新设定方向，如果不是继续
        //mmy增加判断是否是已经检测过的区域
        if ((temp_qy_h < 1) ||
          (temp_qy_h + 1 >= bmpHeight) ||
          (temp_qy_l < 1) ||
          (temp_qy_l + 1 >= bmpWidth) ||
          (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) != -1) ||//&&*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)!=temp_qy_num)||//第二个判定取消mmy
          *(xsvalue + temp_qy_h * bmpWidth + temp_qy_l) > 0 || //是别的区域
          *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) > -1//或者已经是找过的区域
          )//||(*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)>0&&*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)<temp_qy_num))     //新的一点为0，退回上一点，并将方向+1
        {//如果超界或者当前的，或者/、像素标记数组值不是-1且//像素标记数组值不是当前区域编号，即已标记
          needback = 1;
        }
        else if ((*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l + 1)) *
          (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l + 1)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l + 1)) *
          (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l - 1)) *
          (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l - 1)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l - 1)) != 0
          )//如果没黑色
        {
          if (abs((*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l + 1)) *
            (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l + 1)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l + 1)) *
            (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l - 1)) *
            (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l - 1)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l - 1))
          ) == 1)//全白色
            needback = 1;
        }

        //上面循环是退回处理，如果不需要退回，往下执行
        //temp_fxnum=0;//方向数置0？？？
        /*
        //判断是否回到了已经处理的边界，即是否形成了闭合
        temp_close=bj_close(temp_bj_h,temp_bj_l,temp_bj_num,temp_qy_h,temp_qy_l);  //计算是否闭合
         //保存已找到的边界点行列数组，连续边界点数，起始当前边界的区域起始行列
        //if( (temp_close>=0) && (temp_close<=temp_bj_num-2))//mmy取消注释
          //break;
        //非为0，先判断是否为已经计算
        if (*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)==temp_qy_num)//如果当前点是当前区域的一部分
        {
          if ((temp_qy_l==temp_qs_l) && (temp_qy_h==temp_qs_h) )//判断是否起始行列
          {
            //   showmessage('回到起点');
            break;  //回到起点，当前区域的找边界循环结束
          }
        }
        *///mmy
        //检查周边8点中是否有零，如果没有即为非边界，退回 (*(jgvalue+(temp_qy_h+1)*bmpWidth+temp_qy_l))
        if (needback)
        {//退回处理
          //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,0,0));//mmy

          switch (temp_fx) {
          case 1:
            temp_qy_h = temp_qy_h + 1;       //向上
            break;
          case 2:
            temp_qy_l = temp_qy_l - 1;       //向右
            break;
          case 3:
            temp_qy_h = temp_qy_h - 1;        //向下
            break;
          case 4:
            temp_qy_l = temp_qy_l + 1;        //向左
            break;
          }
          //去除不是边界的标识
            //pDC->Ellipse(temp_qy_l*rect.right/bmpWidth-2,temp_qy_h*rect.bottom/bmpHeight-2,temp_qy_l*rect.right/bmpWidth+2,temp_qy_h*rect.bottom/bmpHeight+2);
            //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));//mmy

          --temp_bj_num;//连续边界点数--
          if (temp_bj_num < 0)
            AfxMessageBox(_T("TEMPBJNUM<0"));
          *(temp_bj_z + temp_bj_num) = *(temp_bj_z + temp_bj_num) + 1;//MMY
          temp_fx = temp_fx + 1;
          if (temp_fx == 5)
            temp_fx = 1;
          ////////////////////mmy，以前没有//该点重做了3次，取消边界标记并退回处理
          while (3 <= *(temp_bj_z + temp_bj_num))//第temp_bj_num个连续边界点的重复处理数为3次
          {
            *(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) = -1;  //将当前像素值修改b色，同时取消区域标记，不能改黑，会导致jg数组改变引起下一次循环误判该点
            *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = 0;  //将当前像素值方向置0，表示处理过方向
            //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,0,0));//mmy

            if (0 == temp_bj_num) {//如果连续边界点数只有0个，退出找边界循环
              break;
            }
            switch (temp_fx) {
            case 1:
              temp_qy_h = temp_qy_h - 1;        //向上
              break;
            case 2:
              temp_qy_l = temp_qy_l + 1;        //向右
              break;
            case 3:
              temp_qy_h = temp_qy_h + 1;        //向下
              break;
            case 4:
              temp_qy_l = temp_qy_l - 1;        //向左
              break;
            }
            //pDC->Ellipse(temp_qy_l*rect.right/bmpWidth-2,temp_qy_h*rect.bottom/bmpHeight-2,temp_qy_l*rect.right/bmpWidth+2,temp_qy_h*rect.bottom/bmpHeight+2);
      //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));//
            --temp_bj_num;//连续边界点数--
            if (temp_bj_num < 0)
              AfxMessageBox(_T("TEMPBJNUM<0"));
            *(temp_bj_z + temp_bj_num) = *(temp_bj_z + temp_bj_num) + 1;//MMY
            temp_fx = (temp_fx + 3);//mmy从2改为3
            if (temp_fx >= 5)	temp_fx = temp_fx - 4;
          }
          ////////////////////
          continue;
        }
        else
        {   //，不退回处理有零存在 ,是边界
          //temp_bj_num=temp_bj_num+1;//连续边界点数++//当增加needback一些内容后出现图像不能回退成功，发现tempbjnum每次都会多加一次
        //if(!debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));//
          *(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) = temp_qy_num;  //将当前像素值修改为当前区域区域标记边界设置
          tempi = tempi + 1;//不解？？？？？？？
          *(temp_bj_h + temp_bj_num) = temp_qy_h;  //行坐标
          *(temp_bj_l + temp_bj_num) = temp_qy_l;  //列坐标
          *(temp_bj_fx + temp_bj_num) = temp_fx;
          *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = temp_fx;  //当前像素值方向
          if (min_h > temp_qy_h) min_h = temp_qy_h;
          if (max_h < temp_qy_h) max_h = temp_qy_h;
          if (min_l > temp_qy_l) min_l = temp_qy_l;
          if (max_l < temp_qy_l) max_l = temp_qy_l;
          temp_fx = (temp_fx - 1);
          if (temp_fx == 0)
            temp_fx = 4;  //循环
        }
      }//找边界循环2结束

      if (temp_bj_num == 0 || g_stu_square.nN >= 20) {//如果连续边界点数只有0，退出，
        *(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) = -1;//将当前像素值修改为b色，同时取消区域标记
        *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = 0;  //将当前像素值方向置0
        //if(debug && pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,0,0));//mmy

        //continue;//继续下一行列循环//不知道要不要continue
      }
      else
      {
        //data_check_fill_bj(pBmpBuf,temp_qy_num,0,temp_close,temp_bj_h,temp_bj_l,xsvalue,fbvalue,jgvalue,txvalue,bmpWidth,bmpHeight, min_h,max_h,min_l,max_l);
        //xsvalue,fbvalue,jgvalue,txvalue只有jgvalue有改，tempqynum的初始值是1
        //int mj=data_check_fill_bj(pBmpBuf,temp_qy_num,0,temp_close,temp_bj_h,temp_bj_l,xsvalue,fbvalue,jgvalue,txvalue,bmpWidth,bmpHeight, 1,bmpHeight-2,1,bmpWidth-2);
        int mj = data_check_fill_bj(pBmpBuf, temp_qy_num, 0, temp_close, temp_bj_h, temp_bj_l, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, min_h, max_h, min_l, max_l, debug);
        if (debug)
        {
          sprintf(szname, "%s\\tmpbj.bmp", chPathMain);
          saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
        }
        //if(temp_bj_num>(bmpWidth+bmpHeight)/3)
        if (temp_bj_num > (bmpWidth + bmpHeight) >> 4)
        {
          bool bChk = 1;
          if (circle)
          {
            if (g_stu_square.bJudgeFeature)//如果要匹配特征，则首先判断面积是否符合要求
            {
              if (mj > ((stuRef.Rad * stuRef.Rad * gd_PI) * 1.5))//如果面积》1.5倍特征面积
              {
                g_stu_square.bIsLap = 1;
                bChk = 0;
              }
              if (mj < ((stuRef.Rad * stuRef.Rad * gd_PI) * 0.5))//如果面积《0.5倍特征面积
                bChk = 0;
            }
            /*if (g_stu_square.bJudgeFeature)
            {
              if (mj>((stuRef.Rad*stuRef.Rad*gd_PI)*1.5))
              {
                g_stu_square.bIsLap=1;
              }
            }*/
            if (bChk)
            {
              switch (data_check_feature_circle_split(pBmpBuf, biBitCount, bmpWidth, bmpHeight, temp_bj_num, temp_bj_h, temp_bj_l, temp_bj_fx, temp_qy_num, xsvalue, fbvalue, jgvalue, txvalue, outpointallowlen, defectallowlen, min_h, max_h, min_l, max_l))
              {
              case 0:
                type = 0;
                break;
              default:
                type = -1;
                break;
              }

              /*if(pDC!=NULL && type!=-1)
              {
                for(int n = 0; n<temp_bj_num; n++)
                  pDC->SetPixel(temp_bj_l[n]*rect.right/bmpWidth,temp_bj_h[n]*rect.bottom/bmpHeight,RGB(0,255,0));
              }

              if(pDC!=NULL &&debug)
              switch(type)
              {
              case 0:pDC->TextOutW(temp_bj_l[0]*rect.right/bmpWidth,temp_bj_h[0]*rect.bottom/bmpHeight,_T("正常"));
                break;
              case -1:pDC->TextOutW(temp_bj_l[0]*rect.right/bmpWidth,temp_bj_h[0]*rect.bottom/bmpHeight,_T("未识别"));
                break;
              default:break;
              }*/
            }
          }
          else
          {
            if (g_stu_square.bJudgeFeature)
            {
              if (mj > (stuRef.Len * stuRef.Wth * 1.5))
              {
                g_stu_square.bIsLap = 1;
                bChk = 0;
              }
              if (mj < (stuRef.Len * stuRef.Wth * 0.5))
                bChk = 0;
            }
            /*if (g_stu_square.bJudgeFeature)
            {
              if (mj>(stuRef.Len*stuRef.Wth<<1))
              {
                g_stu_square.bIsLap=1;
              }
            }*/
            if (bChk)
            {
              switch (data_check_feature_split(pBmpBuf, biBitCount, bmpWidth, bmpHeight, temp_bj_num, temp_bj_h, temp_bj_l, temp_bj_fx, temp_qy_num, xsvalue, fbvalue, jgvalue, txvalue, outpointallowlen, defectallowlen))
              {
              case 0://if(debug)
                    // AfxMessageBox(_T("正常"));
                type = 0;
                break;
              case 1://if(debug)
                    //AfxMessageBox(_T("形状有问题"));
                type = -1;
                break;
              case 2://if(debug)
                    // AfxMessageBox(_T("少块"));
                type = 2;
                break;
              case 3://if(debug)
                    // AfxMessageBox(_T("多块"));
                type = 1;
                break;
              case 4://if(debug)
                    // AfxMessageBox(_T("线段没找全"));
                type = -1;
                break;
              case 5://if(debug)
                  //AfxMessageBox(_T("不是正方形"));
                type = -1;//毛坯片一般没有角度要求
                break;
              default:break;
              }
              //int l_issquare=IsSquare(88,92);
              /*if(pDC!=NULL && type!=-1)
              {
                for(int n = 0; n<temp_bj_num; n++)
                  pDC->SetPixel(temp_bj_l[n]*rect.right/bmpWidth,temp_bj_h[n]*rect.bottom/bmpHeight,RGB(0,255,0));


              }*/

              if (pDC != NULL && debug)
                switch (type)
                {
                case 0:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("正常"));
                  break;
                case -1:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("未识别"));
                  break;
                case 2:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("少块"));//AfxMessageBox(_T("少块"));
                  break;
                case 1:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("多块"));//AfxMessageBox(_T("多块"));
                  break;
                default:break;
                }
            }

          }
          //int zc=temp_bj_num;//按图像来说，不一定边界点数就==周长，周长要等算出边长之后再来考虑
          if (debug) {
            sprintf(szname, "%s\\result%d.bmp", chPathMain, temp_qy_num);
            saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
          }
          //if(!debug)
          //	break;
        }
      }
      //g_tmp_path=_T(".\\PIC\\temp1.bmp");
      temp_qy_num = temp_qy_num2 + 1;//一个区域检测完成，区域数++，
      temp_qy_num2 = temp_qy_num;
      //int i_count_mouse=temp_qy_num;
      //UpdateData(0);
      if (!debug)
        if (temp_qy_num > 10000)
          break;
    }   //列循环结束
    //
    {
      /*if(temp_bj_num>(bmpWidth+bmpHeight)*2/3)
        break;*/

    }
    if (!debug)
      if (temp_qy_num > 10000)
        break;
  }  //行循环结束
  //saveBmp("temps.bmp",pBmpBuf,bmpWidth,bmpHeight,biBitCount,pColorTable1);
  if (debug && pDC != NULL)
  {
    pDC->TextOutW(0, rect.bottom - 15, _T("正常结束"));//AfxMessageBox(_T("识别结束"));
  }
  //在屏幕上画出边界
  if (pDC != NULL && g_stu_square.nN > 0)
  {

    if (circle)
    {
      /*for(int n = 0; n<nTmpN; n++)
        pDC->SetPixel(pnTmpL[n]*rect.right/bmpWidth,pnTmpH[n]*rect.bottom/bmpHeight,RGB(0,255,0));*/
      for (int i = 0; i < g_stu_square.nN; i++)
        pDC->Ellipse
        (g_stu_square.pnZPX[i] * rect.Width() / g_stu_square.nBMPW - g_stu_square.pnWth[i] * rect.Width() / g_stu_square.nBMPW
          , g_stu_square.pnZPY[i] * rect.Height() / g_stu_square.nBMPH - g_stu_square.pnWth[i] * rect.Width() / g_stu_square.nBMPW
          , g_stu_square.pnZPX[i] * rect.Width() / g_stu_square.nBMPW + g_stu_square.pnWth[i] * rect.Width() / g_stu_square.nBMPW
          , g_stu_square.pnZPY[i] * rect.Height() / g_stu_square.nBMPH + g_stu_square.pnWth[i] * rect.Width() / g_stu_square.nBMPW
        );
    }
    else
    {
      POINT pt;
      for (int i = 0; i < g_stu_square.nN; i++)
      {
        pt.x = g_stu_square.pnPX[i][3] * rect.Width() / g_stu_square.nBMPW;
        pt.y = g_stu_square.pnPY[i][3] * rect.Height() / g_stu_square.nBMPH;
        pDC->MoveTo(pt);
        for (int j = 0; j < 4; j++)
        {
          pDC->LineTo(g_stu_square.pnPX[i][j] * rect.Width() / g_stu_square.nBMPW,
            g_stu_square.pnPY[i][j] * rect.Height() / g_stu_square.nBMPH);
        }
        //if(pDC!=NULL)//debug&& 
        pDC->Ellipse
        (g_stu_square.pnZPX[i] * rect.Width() / g_stu_square.nBMPW - 10 * rect.Width() / g_stu_square.nBMPW
          , g_stu_square.pnZPY[i] * rect.Height() / g_stu_square.nBMPH - 10 * rect.Width() / g_stu_square.nBMPW
          , g_stu_square.pnZPX[i] * rect.Width() / g_stu_square.nBMPW + 10 * rect.Width() / g_stu_square.nBMPW
          , g_stu_square.pnZPY[i] * rect.Height() / g_stu_square.nBMPH + 10 * rect.Width() / g_stu_square.nBMPW
        );
      }

    }
  }
  delete[] temp_bj_h; temp_bj_h = NULL;
  delete[] temp_bj_l; temp_bj_l = NULL;
  delete[] temp_bj_fx; temp_bj_fx = NULL;
  delete[] temp_bj_z; temp_bj_z = NULL;


  //pDC->SetPixel(int(100*rect.right/bmpWidth),int(100*rect.bottom/bmpHeight),RGB(255,0,0));//点画

  //pDC->Ellipse(100*rect.right/bmpWidth-2,100*rect.bottom/bmpHeight-2,100*rect.right/bmpWidth+2,100*rect.bottom/bmpHeight+2);


  delete[] pBmpBuf; pBmpBuf = NULL;
  delete[] xsvalue; xsvalue = NULL;
  delete[] fbvalue; fbvalue = NULL;
  delete[] jgvalue; jgvalue = NULL;
  delete[] txvalue; txvalue = NULL;
  if (pDC != NULL)
    pDC->SelectObject(pOldBrush);
  delete[] pColorTable1;//MMY
  pColorTable1 = NULL;//MMY
  return type;
}
//与RCGBMPSPLIT一样，只是不是调用文件名，而是数据内存
int CIMGRecognition::RCGBMPDATASPLIT(CDC * pDC, CRect rect, unsigned char*& pBmpBuf, int outpointallowlen = 2, int defectallowlen = 2, int threshold = 120, bool debug = 0, bool circle = 0, bool bAutoThrd = 0, bool delNoise = 0)
{
  CBrush* pOldBrush = NULL;
  if (pDC != NULL)
  {
    pDC->SetTextColor(RGB(255, 0, 255));
    pDC->SetBkMode(TRANSPARENT);
    pDC->SelectObject(&font);
    //pDC ->TextOutW(0,0,_T("123456789"));
    pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
  }
  //	m_bmp_file=NULL;
  g_stu_square.debug = debug;
  //	unsigned char * pBmpBuf=NULL;
  int* xsvalue = NULL;
  int* fbvalue = NULL;
  int* jgvalue = NULL;
  int* txvalue = NULL;
  int bmpWidth, bmpHeight;
  g_stu_square.nN = 0;//已识别数置0
  g_stu_square.bIsLap = 0;//重叠表示z置0
  int type = -1;//识别类型置-1
  //char * readPath=NULL;//"temps2.bmp";
  char readPath[MAX_PATH];
  //WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_bmp_file,-1,readPath,CStringA(m_bmp_file).GetLength(),NULL,NULL); 
  WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, m_bmp_file, -1, readPath, sizeof(readPath), NULL, NULL);
  //tempstr=m_bmp_file.GetBuffer(CStringA(m_bmp_file).GetLength());
  //int nRd=readBmp(readPath,pBmpBuf,bmpWidth,bmpHeight); //8\24w
  bmpWidth = 1920;
  bmpHeight = 1080;
  biBitCount = 24;
  //if (nRd!=0)
  {
    //	return type;
  }
  if (bAutoThrd)//如果指定自动阈值，进行自动阈值
  {
    int nTmpThrd = read_image_fz(pBmpBuf, bmpWidth, bmpHeight);//检测当前图像的中间阈值
    if (nTmpThrd != -1)
      threshold = nTmpThrd;
  }
  int pos = m_bmp_file.ReverseFind('\\');
  m_bmp_file = m_bmp_file.Left(pos);
  WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, m_bmp_file, -1, chPathMain, sizeof(chPathMain), NULL, NULL);
  //保存图像尺寸到全局
  g_stu_square.nBMPH = bmpHeight;
  g_stu_square.nBMPW = bmpWidth;
  //saveBmp("temps.bmp",pBmpBuf,bmpWidth,bmpHeight,biBitCount,NULL);
  //m_bmp_file.ReleaseBuffer();
  /*
  readPath=m_bmp_file.GetBuffer(m_bmp_file.GetLength());
  readBmp(readPath,pBmpBuf,bmpWidth,bmpHeight);
  m_bmp_file.ReleaseBuffer();*/
  int temp_bmp_data = bmpWidth * bmpHeight;

  xsvalue = new int[temp_bmp_data]; //全0//最后是边界加区域数组，值是对应的区域编号
  //memset(xsvalue,0,temp_bmp_data);
  fbvalue = new int[temp_bmp_data]; //全0//最后是边界数组，边界值是对应的区域编号,在图片预处理中有用，标记已预处理
  //memset(fbvalue,0,temp_bmp_data);
  jgvalue = new int[temp_bmp_data]; //白的-1，黑的0//用于边界处理的数组
  txvalue = new int[temp_bmp_data]; //白的-1，黑的0//如果当前点是边界n的找过的点，标记n//改为记录方向，为填充区域提供信息
  int lb_blank = 0;
  //二值化处理，识别多片可以考虑不判断极值，直接二值化，更改readimagefz

  if (1)//xs，fb全0；jg，tx白-1，黑0；
    lb_blank = read_image_mul(pBmpBuf, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, threshold, debug, delNoise);//按照阈值二值化图像，去噪
  else
    lb_blank = read_image_blank(pBmpBuf, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, threshold);//判断晶片是否在中心
  //debug
  if (debug)
  {
    sprintf(szname, "%s\\read.bmp", chPathMain);
    saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
  }
  //
  if (lb_blank != 0)//无可识别区域，提前退出RCGBMP
  {
    //delete[] pBmpBuf; pBmpBuf=NULL; 
    delete[] xsvalue; xsvalue = NULL;
    delete[] fbvalue; fbvalue = NULL;
    delete[] jgvalue; jgvalue = NULL;
    delete[] txvalue; txvalue = NULL;

    delete[] pColorTable1;//MMY
    pColorTable1 = NULL;//MMY
//		pWnd->ReleaseDC(pDC);
    if (pDC != NULL)
      pDC->SelectObject(pOldBrush);
    switch (lb_blank)
    {
    case -1:if (debug && pDC != NULL)pDC->TextOutW(0, 0, _T("无片")); break;//AfxMessageBox(_T("无片"));
    case 1:if (debug && pDC != NULL)pDC->TextOutW(0, 0, _T("单片可能已达到图像边缘,如果想继续检测,只能修改代码:)")); break;
      //AfxMessageBox(_T("单片可能已达到图像边缘,如果想继续检测,只能修改代码:)"));
    }
    return type;//type=-1
  }

  int tempi, tempj, templ, temp_qyno, temp_qy_num, temp_qy_num2, temp_xsvalue;
  int temp_qy_i, temp_qs_h, temp_qs_l, temp_qy_h, temp_qy_l, temp_fx;//,temp_fxnum;
  int* temp_bj_h;//行边界
  int* temp_bj_l;//列边界
  int* temp_bj_fx;//方向

  //temp_bj_h,temp_bj_l:array of integer;
  // temp_bj_fx:array of integer;
  int temp_bj_num = 0;//debug下未置0，导致release下出错，release下是乱值
  int temp_total_bj, temp_close;
  int min_h, max_h, min_l, max_l;
  int* temp_bj_z;
  temp_qyno = 1; //区域处理
  temp_qy_num = 1;
  tempi = -10;
  temp_xsvalue = -1; //区域处理白色

  //储存边界点的坐标
  temp_total_bj = (bmpWidth + bmpHeight) << 3;  //每个边界点数 注意指针释放 delete temp_bj_h，太小了，存不下，当片都在一块时
  temp_bj_z = new int[temp_total_bj];	//边界点的重复处理标记
  temp_bj_h = new int[temp_total_bj];      //边界点的行坐标
  temp_bj_l = new int[temp_total_bj];      //边界点的列坐标
  temp_bj_fx = new int[temp_total_bj];      //边界点的寻找方向
  //遍历整幅图像寻找边界起始点
  for (tempj = 1; tempj <= bmpHeight - 2 && g_stu_square.nN < 20; tempj++)     //行循环
  {
    for (templ = 1; templ <= bmpWidth - 2 && g_stu_square.nN < 20; templ++)   //列循环，寻找边界起始点
    {
      //jg标记中像素不是-1且前一个像素不是0继续下一个循环（包括已经处理的区域），否则可能找到起始点
      if (!((*(jgvalue + tempj * bmpWidth + templ) == temp_xsvalue) && (*(jgvalue + tempj * bmpWidth + templ - 1) == 0)))
        continue;
      //mmy
      /*if(!((*(jgvalue+(tempj-1)*bmpWidth+templ)==-1 || *(jgvalue+(tempj+1)*bmpWidth+templ)==-1)
        &&
        (*(jgvalue+(tempj)*bmpWidth+templ-1)==-1 || *(jgvalue+(tempj)*bmpWidth+templ+1)==-1))
        )*/
        //这句很关键，考虑不周会导致边界的顺序是逆时针的（此程序逆时针会出现很多问题，一些逻辑默认其是顺时针的），或者边界的起点是不希望的地方
      if (!(((*(jgvalue + (tempj + 1) * bmpWidth + templ) == -1) && (*(jgvalue + (tempj)* bmpWidth + templ + 1) == -1)) && (*(jgvalue + (tempj - 1) * bmpWidth + templ) == 0)))
        continue;
      //xs标记的值不是0，表示是已识别出的有效区域
      if (*(xsvalue + tempj * bmpWidth + templ) > 0)
        continue;
      //起始点已找到，开始进行找边界
      temp_close = -1;                          //边界是否闭合的标志，-1表示不闭合，0以上数据表示闭合位置，好像没用上
      temp_bj_num = 0;
      //像素点为-1的进行处理：记录当前位置
      temp_qs_h = tempj;   //当前边界起始行记录
      temp_qs_l = templ;   //当前边界起始列记录
      *(jgvalue + tempj * bmpWidth + templ) = temp_qy_num;//把jg像素标记数组值置成当前区域编号。起始为1
      //fbvalue[tempj][templ]=temp_qy_num;
      temp_fx = 2;   //下一步检查的方向标记，1——向上，2——向右，3——向下，4——向左
      temp_qy_l = templ;    //初始位置设定
      temp_qy_h = tempj;
      //temp_fxnum=0;  //初始执行方向数设定为0
      // tempi=0;
      *temp_bj_h = tempj;  //记录边界起始行坐标到数组
      *temp_bj_l = templ;  //记录边界起始列坐标到数组
      *temp_bj_fx = temp_fx;//记录边界起始方向
      temp_bj_num = 0;   //连续边界点数置0
      *(temp_bj_z + temp_bj_num) = 1;  //第temp_bj_num个连续边界点的重复处理数1
      min_h = bmpHeight; max_h = 0; min_l = bmpWidth; max_l = 0;
      //画边界起始点
      if (debug && pDC != NULL)pDC->Ellipse(temp_qy_l * rect.right / bmpWidth - 2, temp_qy_h * rect.bottom / bmpHeight - 2, temp_qy_l * rect.right / bmpWidth + 2, temp_qy_h * rect.bottom / bmpHeight + 2);
      //pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));
      temp_qy_num2 = temp_qy_num;//
      //找边界循环开始
      for (temp_qy_i = 1; temp_qy_i < temp_total_bj && g_stu_square.nN < 20; temp_qy_i++)
      {
        //::Sleep(1);
        //按方向拿出下一个点
        switch (temp_fx) {
        case 1:
          temp_qy_h = temp_qy_h - 1;        //向上
          break;
        case 2:
          temp_qy_l = temp_qy_l + 1;        //向右
          break;
        case 3:
          temp_qy_h = temp_qy_h + 1;        //向下
          break;
        case 4:
          temp_qy_l = temp_qy_l - 1;        //向左
          break;
        }
        //画检测到的待检验边界点
        //pDC->Ellipse(temp_qy_l*rect.right/bmpWidth-2,temp_qy_h*rect.bottom/bmpHeight-2,temp_qy_l*rect.right/bmpWidth+2,temp_qy_h*rect.bottom/bmpHeight+2);
        //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));
        ++temp_bj_num;//连续边界点数++
        *(temp_bj_z + temp_bj_num) = 0;//当前点的重复处理标记清0，是新点，待检测，对应标记清0
        //mmy
        bool needback = 0;//退回处理标记清0

        if (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) == temp_qy_num)//如果jg当前点已经标记为当前区域，说明有封闭区域产生
        {
          //因为是有效点，必须作为标准边界点保存，由于是结束，所以方向处理可以不做
          //*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)=temp_qy_num;  //将当前像素值修改为当前区域区域标记边界设置//此处貌似多余
          *(temp_bj_h + temp_bj_num) = temp_qy_h;  //行坐标
          *(temp_bj_l + temp_bj_num) = temp_qy_l;  //列坐标
          *(temp_bj_fx + temp_bj_num) = temp_fx;
          *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = temp_fx;  //当前像素值方向，必须改变，防止开始点是别的区域转过来导致填充的时候方向不符合要求而出差错

          if ((temp_qy_l == temp_qs_l) && (temp_qy_h == temp_qs_h))//判断是否起始行列
          {
            //   showmessage('回到起点');
            break;  //回到起点，当前区域的找边界循环结束
          }
          else
          {
            //由于封闭，所以直接退回
            //int nTmpH = temp_qy_h , nTmpL = temp_qy_l;		//保存退回起始行列
            if (debug && pDC != NULL)
              pDC->TextOutW(0, 0, _T("有接触区域"));
            //AfxMessageBox(_T("有接触区域"));
            int nTmpN = 0;								//保存退回边界点数
            int* pnTmpH, *pnTmpL, *pnTmpFx;
            pnTmpH = new int[temp_bj_num];						//保存封闭曲线边界点的行坐标
            pnTmpL = new int[temp_bj_num];						//保存封闭曲线边界点的列坐标
            pnTmpFx = new int[temp_bj_num];						//方向
            temp_qy_num2++;										//备用区域编号++，新区域
            int nMinH = bmpHeight, nMinL = bmpWidth, nMaxH = 0, nMaxL = 0;

            do
            {

              *(pnTmpH + nTmpN) = *(temp_bj_h + temp_bj_num);	//行坐标 
              *(pnTmpL + nTmpN) = *(temp_bj_l + temp_bj_num);	//列坐标
              *(pnTmpFx + nTmpN) = *(temp_bj_fx + temp_bj_num);	//列坐标
              if (nMinH > * (temp_bj_h + temp_bj_num)) nMinH = *(temp_bj_h + temp_bj_num);
              if (nMaxH < *(temp_bj_h + temp_bj_num)) nMaxH = *(temp_bj_h + temp_bj_num);
              if (nMinL > * (temp_bj_l + temp_bj_num)) nMinL = *(temp_bj_l + temp_bj_num);
              if (nMaxL < *(temp_bj_l + temp_bj_num)) nMaxL = *(temp_bj_l + temp_bj_num);

              *(jgvalue + *(temp_bj_h + temp_bj_num) * bmpWidth + *(temp_bj_l + temp_bj_num)) = temp_qy_num2;  //将当前像素值修改为b色，同时取消区域标记
              //*(txvalue+*(temp_bj_h + temp_bj_num)*bmpWidth+*(temp_bj_l + temp_bj_num))=*(temp_bj_fx+temp_bj_num);  //将当前像素值修改为区域值，说明已找过
              nTmpN++;
              temp_bj_num--;		//总点数-1，回点
            } while (((temp_qy_h != *(temp_bj_h + temp_bj_num)) || (temp_qy_l != *(temp_bj_l + temp_bj_num))) && temp_bj_num != 0);//退回循环 ///临时加上temp_bj_num!=0因为会出现temp_bjnum == -1 而死在这里
            /////////////////////////////////

            //int mj=data_check_fill_bj(pBmpBuf,temp_qy_num2,0,temp_close,pnTmpH,pnTmpL,xsvalue,fbvalue,jgvalue,txvalue,bmpWidth,bmpHeight, 1,bmpHeight-2,1,bmpWidth-2);
            //填充区域并返回区域面积
            int mj = data_check_fill_bj(pBmpBuf, temp_qy_num2, 0, temp_close, pnTmpH, pnTmpL, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, nMinH, nMaxH, nMinL, nMaxL, debug);
            if (debug)
            {
              sprintf(szname, "%s\\tmpbj.bmp", chPathMain);
              saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
            }
            /////////////////////////////////
            temp_qy_h = *(temp_bj_h + temp_bj_num);  //行坐标
            temp_qy_l = *(temp_bj_l + temp_bj_num);  //列坐标
            temp_fx = *(temp_bj_fx + temp_bj_num);
            if (nTmpN > ((bmpWidth + bmpHeight) >> 4) && temp_qy_num2 < 10000)//如果点数符合要求就判断特征
            {
              bool bChk = 1;
              if (circle)
              {

                if (g_stu_square.bJudgeFeature)//如果要匹配特征，则首先判断面积是否符合要求
                {
                  if (mj > ((stuRef.Rad * stuRef.Rad * gd_PI) * 1.5))//如果面积》1.5倍特征面积
                  {
                    g_stu_square.bIsLap = 1;
                    bChk = 0;
                  }
                  if (mj < ((stuRef.Rad * stuRef.Rad * gd_PI) * 0.5))//如果面积《0.5倍特征面积
                    bChk = 0;
                }
                if (bChk)
                {
                  for (int n = 0, x = nTmpN - 1; n < x; n++, x--)//前面是按退回的顺序存放坐标，这里重新倒回来,在判断特征时需要这样
                  {
                    int nTmp = *(pnTmpH + n);
                    *(pnTmpH + n) = *(pnTmpH + x);
                    *(pnTmpH + x) = nTmp;
                    nTmp = *(pnTmpL + n);
                    *(pnTmpL + n) = *(pnTmpL + x);
                    *(pnTmpL + x) = nTmp;
                    nTmp = *(pnTmpFx + n);
                    *(pnTmpFx + n) = *(pnTmpFx + x);
                    *(pnTmpFx + x) = nTmp;
                  }
                  switch (data_check_feature_circle_split(pBmpBuf, biBitCount, bmpWidth, bmpHeight, nTmpN, pnTmpH, pnTmpL, pnTmpFx, temp_qy_num2, xsvalue, fbvalue, jgvalue, txvalue, outpointallowlen, defectallowlen, nMinH, nMaxH, nMinL, nMaxL))
                  {
                  case 0:
                    type = 0;
                    break;
                  default:
                    type = -1;
                    break;
                  }
                  //在屏幕上画出边界
                /*	if(pDC!=NULL && type!=-1)
                  {
                    for(int n = 0; n<nTmpN; n++)
                      pDC->SetPixel(pnTmpL[n]*rect.right/bmpWidth,pnTmpH[n]*rect.bottom/bmpHeight,RGB(0,255,0));
                  }
                  //在屏幕上标注识别结果
                  if(pDC!=NULL &&debug)
                  switch(type)
                  {
                  case 0:
                    pDC->TextOutW(pnTmpL[0]*rect.right/bmpWidth,pnTmpH[0]*rect.bottom/bmpHeight,_T("正常"));
                    break;
                  case -1:
                    pDC->TextOutW(pnTmpL[0]*rect.right/bmpWidth,pnTmpH[0]*rect.bottom/bmpHeight,_T("未识别"));
                    break;
                  default:break;
                  }*/

                }
              }
              else
              {
                if (g_stu_square.bJudgeFeature)
                {
                  if (mj > (stuRef.Len * stuRef.Wth * 1.5))
                  {
                    g_stu_square.bIsLap = 1;
                    bChk = 0;
                  }
                  if (mj < (stuRef.Len * stuRef.Wth * 0.5))
                    bChk = 0;
                }
                if (bChk)
                {
                  for (int n = 0, x = nTmpN - 1; n < x; n++, x--)//前面是按退回的顺序存放坐标，这里重新倒回来,在判断特征时需要这样
                  {
                    int nTmp = *(pnTmpH + n);
                    *(pnTmpH + n) = *(pnTmpH + x);
                    *(pnTmpH + x) = nTmp;
                    nTmp = *(pnTmpL + n);
                    *(pnTmpL + n) = *(pnTmpL + x);
                    *(pnTmpL + x) = nTmp;
                    nTmp = *(pnTmpFx + n);
                    *(pnTmpFx + n) = *(pnTmpFx + x);
                    *(pnTmpFx + x) = nTmp;
                  }
                  switch (data_check_feature_split(pBmpBuf, biBitCount, bmpWidth, bmpHeight, nTmpN, pnTmpH, pnTmpL, pnTmpFx, temp_qy_num2, xsvalue, fbvalue, jgvalue, txvalue, outpointallowlen, defectallowlen))
                  {
                  case 0://if(debug)
                        // AfxMessageBox(_T("正常"));
                    type = 0;
                    break;
                  case 1://if(debug)
                         //AfxMessageBox(_T("形状有问题"));
                    type = -1;
                    break;
                  case 2://if(debug)
                        // AfxMessageBox(_T("少块"));
                    type = 2;
                    break;
                  case 3://if(debug)
                        // AfxMessageBox(_T("多块"));
                    type = 1;
                    break;
                  case 4://if(debug)
                        // AfxMessageBox(_T("线段没找全"));
                    type = -1;
                    break;
                  case 5://if(debug)
                      //AfxMessageBox(_T("不是正方形"));
                    type = -1;//毛坯片一般没有角度要求
                    break;
                  default:break;
                  }
                  //int l_issquare=IsSquare(88,92);
                  /*if(pDC!=NULL && type!=-1)
                  {
                    for(int n = 0; n<nTmpN; n++)
                      pDC->SetPixel(pnTmpL[n]*rect.right/bmpWidth,pnTmpH[n]*rect.bottom/bmpHeight,RGB(0,255,0));

                  }*/

                  if (pDC != NULL && debug)
                    switch (type)
                    {
                    case 0:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("正常"));
                      break;
                    case -1:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("未识别"));
                      break;
                    case 2:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("少块"));//AfxMessageBox(_T("少块"));
                      break;
                    case 1:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("多块"));//AfxMessageBox(_T("多块"));
                      break;
                    default:break;
                    }

                }
              }
            }

            delete[] pnTmpH; pnTmpH = NULL;
            delete[] pnTmpL; pnTmpL = NULL;
            delete[] pnTmpFx; pnTmpFx = NULL;
            needback = 1;
          }
        }

        //
        //先判断本身是否为0，如果是退回并重新设定方向，如果不是继续
        //mmy增加判断是否是已经检测过的区域
        if ((temp_qy_h < 1) ||
          (temp_qy_h + 1 >= bmpHeight) ||
          (temp_qy_l < 1) ||
          (temp_qy_l + 1 >= bmpWidth) ||
          (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) != -1) ||//&&*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)!=temp_qy_num)||//第二个判定取消mmy
          *(xsvalue + temp_qy_h * bmpWidth + temp_qy_l) > 0 || //是别的区域
          *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) > -1//或者已经是找过的区域
          )//||(*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)>0&&*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)<temp_qy_num))     //新的一点为0，退回上一点，并将方向+1
        {//如果超界或者当前的，或者/、像素标记数组值不是-1且//像素标记数组值不是当前区域编号，即已标记
          needback = 1;
        }
        else if ((*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l + 1)) *
          (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l + 1)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l + 1)) *
          (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l - 1)) *
          (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l - 1)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l - 1)) != 0
          )//如果没黑色
        {
          if (abs((*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l + 1)) *
            (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l + 1)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l + 1)) *
            (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l - 1)) *
            (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l - 1)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l - 1))
          ) == 1)//全白色
            needback = 1;
        }

        //上面循环是退回处理，如果不需要退回，往下执行
        //temp_fxnum=0;//方向数置0？？？
        /*
        //判断是否回到了已经处理的边界，即是否形成了闭合
        temp_close=bj_close(temp_bj_h,temp_bj_l,temp_bj_num,temp_qy_h,temp_qy_l);  //计算是否闭合
         //保存已找到的边界点行列数组，连续边界点数，起始当前边界的区域起始行列
        //if( (temp_close>=0) && (temp_close<=temp_bj_num-2))//mmy取消注释
          //break;
        //非为0，先判断是否为已经计算
        if (*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)==temp_qy_num)//如果当前点是当前区域的一部分
        {
          if ((temp_qy_l==temp_qs_l) && (temp_qy_h==temp_qs_h) )//判断是否起始行列
          {
            //   showmessage('回到起点');
            break;  //回到起点，当前区域的找边界循环结束
          }
        }
        *///mmy
        //检查周边8点中是否有零，如果没有即为非边界，退回 (*(jgvalue+(temp_qy_h+1)*bmpWidth+temp_qy_l))
        if (needback)
        {//退回处理
          //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,0,0));//mmy

          switch (temp_fx) {
          case 1:
            temp_qy_h = temp_qy_h + 1;       //向上
            break;
          case 2:
            temp_qy_l = temp_qy_l - 1;       //向右
            break;
          case 3:
            temp_qy_h = temp_qy_h - 1;        //向下
            break;
          case 4:
            temp_qy_l = temp_qy_l + 1;        //向左
            break;
          }
          //去除不是边界的标识
            //pDC->Ellipse(temp_qy_l*rect.right/bmpWidth-2,temp_qy_h*rect.bottom/bmpHeight-2,temp_qy_l*rect.right/bmpWidth+2,temp_qy_h*rect.bottom/bmpHeight+2);
            //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));//mmy

          --temp_bj_num;//连续边界点数--
          if (temp_bj_num < 0)
            AfxMessageBox(_T("TEMPBJNUM<0"));
          *(temp_bj_z + temp_bj_num) = *(temp_bj_z + temp_bj_num) + 1;//MMY
          temp_fx = temp_fx + 1;
          if (temp_fx == 5)
            temp_fx = 1;
          ////////////////////mmy，以前没有//该点重做了3次，取消边界标记并退回处理
          while (3 <= *(temp_bj_z + temp_bj_num))//第temp_bj_num个连续边界点的重复处理数为3次
          {
            *(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) = -1;  //将当前像素值修改b色，同时取消区域标记，不能改黑，会导致jg数组改变引起下一次循环误判该点
            *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = 0;  //将当前像素值方向置0，表示处理过方向
            //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,0,0));//mmy

            if (0 == temp_bj_num) {//如果连续边界点数只有0个，退出找边界循环
              break;
            }
            switch (temp_fx) {
            case 1:
              temp_qy_h = temp_qy_h - 1;        //向上
              break;
            case 2:
              temp_qy_l = temp_qy_l + 1;        //向右
              break;
            case 3:
              temp_qy_h = temp_qy_h + 1;        //向下
              break;
            case 4:
              temp_qy_l = temp_qy_l - 1;        //向左
              break;
            }
            //pDC->Ellipse(temp_qy_l*rect.right/bmpWidth-2,temp_qy_h*rect.bottom/bmpHeight-2,temp_qy_l*rect.right/bmpWidth+2,temp_qy_h*rect.bottom/bmpHeight+2);
      //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));//
            --temp_bj_num;//连续边界点数--
            if (temp_bj_num < 0)
              AfxMessageBox(_T("TEMPBJNUM<0"));
            *(temp_bj_z + temp_bj_num) = *(temp_bj_z + temp_bj_num) + 1;//MMY
            temp_fx = (temp_fx + 3);//mmy从2改为3
            if (temp_fx >= 5)	temp_fx = temp_fx - 4;
          }
          ////////////////////
          continue;
        }
        else
        {   //，不退回处理有零存在 ,是边界
          //temp_bj_num=temp_bj_num+1;//连续边界点数++//当增加needback一些内容后出现图像不能回退成功，发现tempbjnum每次都会多加一次
        //if(!debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));//
          *(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) = temp_qy_num;  //将当前像素值修改为当前区域区域标记边界设置
          tempi = tempi + 1;//不解？？？？？？？
          *(temp_bj_h + temp_bj_num) = temp_qy_h;  //行坐标
          *(temp_bj_l + temp_bj_num) = temp_qy_l;  //列坐标
          *(temp_bj_fx + temp_bj_num) = temp_fx;
          *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = temp_fx;  //当前像素值方向
          if (min_h > temp_qy_h) min_h = temp_qy_h;
          if (max_h < temp_qy_h) max_h = temp_qy_h;
          if (min_l > temp_qy_l) min_l = temp_qy_l;
          if (max_l < temp_qy_l) max_l = temp_qy_l;
          temp_fx = (temp_fx - 1);
          if (temp_fx == 0)
            temp_fx = 4;  //循环
        }
      }//找边界循环2结束

      if (temp_bj_num == 0 || g_stu_square.nN >= 20) {//如果连续边界点数只有0，退出，
        *(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) = -1;//将当前像素值修改为b色，同时取消区域标记
        *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = 0;  //将当前像素值方向置0
        //if(debug && pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,0,0));//mmy

        //continue;//继续下一行列循环//不知道要不要continue
      }
      else
      {
        //data_check_fill_bj(pBmpBuf,temp_qy_num,0,temp_close,temp_bj_h,temp_bj_l,xsvalue,fbvalue,jgvalue,txvalue,bmpWidth,bmpHeight, min_h,max_h,min_l,max_l);
        //xsvalue,fbvalue,jgvalue,txvalue只有jgvalue有改，tempqynum的初始值是1
        //int mj=data_check_fill_bj(pBmpBuf,temp_qy_num,0,temp_close,temp_bj_h,temp_bj_l,xsvalue,fbvalue,jgvalue,txvalue,bmpWidth,bmpHeight, 1,bmpHeight-2,1,bmpWidth-2);
        int mj = data_check_fill_bj(pBmpBuf, temp_qy_num, 0, temp_close, temp_bj_h, temp_bj_l, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, min_h, max_h, min_l, max_l, debug);
        if (debug)
        {
          sprintf(szname, "%s\\tmpbj.bmp", chPathMain);
          saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
        }
        //if(temp_bj_num>(bmpWidth+bmpHeight)/3)
        if (temp_bj_num > (bmpWidth + bmpHeight) >> 4)
        {
          bool bChk = 1;
          if (circle)
          {
            if (g_stu_square.bJudgeFeature)//如果要匹配特征，则首先判断面积是否符合要求
            {
              if (mj > ((stuRef.Rad * stuRef.Rad * gd_PI) * 1.5))//如果面积》1.5倍特征面积
              {
                g_stu_square.bIsLap = 1;
                bChk = 0;
              }
              if (mj < ((stuRef.Rad * stuRef.Rad * gd_PI) * 0.5))//如果面积《0.5倍特征面积
                bChk = 0;
            }
            /*if (g_stu_square.bJudgeFeature)
            {
              if (mj>((stuRef.Rad*stuRef.Rad*gd_PI)*1.5))
              {
                g_stu_square.bIsLap=1;
              }
            }*/
            if (bChk)
            {
              switch (data_check_feature_circle_split(pBmpBuf, biBitCount, bmpWidth, bmpHeight, temp_bj_num, temp_bj_h, temp_bj_l, temp_bj_fx, temp_qy_num, xsvalue, fbvalue, jgvalue, txvalue, outpointallowlen, defectallowlen, min_h, max_h, min_l, max_l))
              {
              case 0:
                type = 0;
                break;
              default:
                type = -1;
                break;
              }

              /*if(pDC!=NULL && type!=-1)
              {
                for(int n = 0; n<temp_bj_num; n++)
                  pDC->SetPixel(temp_bj_l[n]*rect.right/bmpWidth,temp_bj_h[n]*rect.bottom/bmpHeight,RGB(0,255,0));
              }

              if(pDC!=NULL &&debug)
              switch(type)
              {
              case 0:pDC->TextOutW(temp_bj_l[0]*rect.right/bmpWidth,temp_bj_h[0]*rect.bottom/bmpHeight,_T("正常"));
                break;
              case -1:pDC->TextOutW(temp_bj_l[0]*rect.right/bmpWidth,temp_bj_h[0]*rect.bottom/bmpHeight,_T("未识别"));
                break;
              default:break;
              }*/
            }
          }
          else
          {
            if (g_stu_square.bJudgeFeature)
            {
              if (mj > (stuRef.Len * stuRef.Wth * 1.5))
              {
                g_stu_square.bIsLap = 1;
                bChk = 0;
              }
              if (mj < (stuRef.Len * stuRef.Wth * 0.5))
                bChk = 0;
            }
            /*if (g_stu_square.bJudgeFeature)
            {
              if (mj>(stuRef.Len*stuRef.Wth<<1))
              {
                g_stu_square.bIsLap=1;
              }
            }*/
            if (bChk)
            {
              switch (data_check_feature_split(pBmpBuf, biBitCount, bmpWidth, bmpHeight, temp_bj_num, temp_bj_h, temp_bj_l, temp_bj_fx, temp_qy_num, xsvalue, fbvalue, jgvalue, txvalue, outpointallowlen, defectallowlen))
              {
              case 0://if(debug)
                    // AfxMessageBox(_T("正常"));
                type = 0;
                break;
              case 1://if(debug)
                    //AfxMessageBox(_T("形状有问题"));
                type = -1;
                break;
              case 2://if(debug)
                    // AfxMessageBox(_T("少块"));
                type = 2;
                break;
              case 3://if(debug)
                    // AfxMessageBox(_T("多块"));
                type = 1;
                break;
              case 4://if(debug)
                    // AfxMessageBox(_T("线段没找全"));
                type = -1;
                break;
              case 5://if(debug)
                  //AfxMessageBox(_T("不是正方形"));
                type = -1;//毛坯片一般没有角度要求
                break;
              default:break;
              }
              //int l_issquare=IsSquare(88,92);
              /*if(pDC!=NULL && type!=-1)
              {
                for(int n = 0; n<temp_bj_num; n++)
                  pDC->SetPixel(temp_bj_l[n]*rect.right/bmpWidth,temp_bj_h[n]*rect.bottom/bmpHeight,RGB(0,255,0));


              }*/

              if (pDC != NULL && debug)
                switch (type)
                {
                case 0:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("正常"));
                  break;
                case -1:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("未识别"));
                  break;
                case 2:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("少块"));//AfxMessageBox(_T("少块"));
                  break;
                case 1:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("多块"));//AfxMessageBox(_T("多块"));
                  break;
                default:break;
                }
            }

          }
          //int zc=temp_bj_num;//按图像来说，不一定边界点数就==周长，周长要等算出边长之后再来考虑
          if (debug) {
            sprintf(szname, "%s\\result%d.bmp", chPathMain, temp_qy_num);
            saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
          }
          //if(!debug)
          //	break;
        }
      }
      //g_tmp_path=_T(".\\PIC\\temp1.bmp");
      temp_qy_num = temp_qy_num2 + 1;//一个区域检测完成，区域数++，
      temp_qy_num2 = temp_qy_num;
      //int i_count_mouse=temp_qy_num;
      //UpdateData(0);
      if (!debug)
        if (temp_qy_num > 10000)
          break;
    }   //列循环结束
    //
    {
      /*if(temp_bj_num>(bmpWidth+bmpHeight)*2/3)
        break;*/

    }
    if (!debug)
      if (temp_qy_num > 10000)
        break;
  }  //行循环结束
  //saveBmp("temps.bmp",pBmpBuf,bmpWidth,bmpHeight,biBitCount,pColorTable1);
  if (debug && pDC != NULL)
  {
    pDC->TextOutW(0, rect.bottom - 15, _T("正常结束"));//AfxMessageBox(_T("识别结束"));
  }
  //在屏幕上画出边界
  if (pDC != NULL && g_stu_square.nN > 0)
  {
    BITMAPINFOHEADER RGB32BITSBITMAPINFO = {
    sizeof(BITMAPINFOHEADER),    // biSize 
    1920,                    // biWidth; 
    1080,                // biHeight; 
    1,                            // biPlanes; 
    24,                            // biBitCount 
    BI_RGB,                        // biCompression; 
    (1920 * 24 / 8 + 3) / 4 * 4 * 1080,                            // biSizeImage; 
    0,                            // biXPelsPerMeter; 
    0,                            // biYPelsPerMeter; 
    0,                            // biClrUsed; 
    0                            // biClrImportant; 
    };
    StretchDIBits(pDC->GetSafeHdc(), 0, 0, rect.Width(), rect.Height(),
      0, 0, 1920, 1080, pBmpBuf,
      (BITMAPINFO*)& RGB32BITSBITMAPINFO, DIB_RGB_COLORS, SRCCOPY);
    if (circle)
    {
      /*for(int n = 0; n<nTmpN; n++)
        pDC->SetPixel(pnTmpL[n]*rect.right/bmpWidth,pnTmpH[n]*rect.bottom/bmpHeight,RGB(0,255,0));*/
      for (int i = 0; i < g_stu_square.nN; i++)
        pDC->Ellipse
        (g_stu_square.pnZPX[i] * rect.Width() / g_stu_square.nBMPW - g_stu_square.pnWth[i] * rect.Width() / g_stu_square.nBMPW
          , g_stu_square.pnZPY[i] * rect.Height() / g_stu_square.nBMPH - g_stu_square.pnWth[i] * rect.Width() / g_stu_square.nBMPW
          , g_stu_square.pnZPX[i] * rect.Width() / g_stu_square.nBMPW + g_stu_square.pnWth[i] * rect.Width() / g_stu_square.nBMPW
          , g_stu_square.pnZPY[i] * rect.Height() / g_stu_square.nBMPH + g_stu_square.pnWth[i] * rect.Width() / g_stu_square.nBMPW
        );
    }
    else
    {
      POINT pt;
      for (int i = 0; i < g_stu_square.nN; i++)
      {
        pt.x = g_stu_square.pnPX[i][3] * rect.Width() / g_stu_square.nBMPW;
        pt.y = g_stu_square.pnPY[i][3] * rect.Height() / g_stu_square.nBMPH;
        pDC->MoveTo(pt);
        for (int j = 0; j < 4; j++)
        {
          pDC->LineTo(g_stu_square.pnPX[i][j] * rect.Width() / g_stu_square.nBMPW,
            g_stu_square.pnPY[i][j] * rect.Height() / g_stu_square.nBMPH);
        }
        //if(pDC!=NULL)//debug&& 
        pDC->Ellipse
        (g_stu_square.pnZPX[i] * rect.Width() / g_stu_square.nBMPW - 10 * rect.Width() / g_stu_square.nBMPW
          , g_stu_square.pnZPY[i] * rect.Height() / g_stu_square.nBMPH - 10 * rect.Width() / g_stu_square.nBMPW
          , g_stu_square.pnZPX[i] * rect.Width() / g_stu_square.nBMPW + 10 * rect.Width() / g_stu_square.nBMPW
          , g_stu_square.pnZPY[i] * rect.Height() / g_stu_square.nBMPH + 10 * rect.Width() / g_stu_square.nBMPW
        );
      }

    }
  }
  delete[] temp_bj_h; temp_bj_h = NULL;
  delete[] temp_bj_l; temp_bj_l = NULL;
  delete[] temp_bj_fx; temp_bj_fx = NULL;
  delete[] temp_bj_z; temp_bj_z = NULL;


  //pDC->SetPixel(int(100*rect.right/bmpWidth),int(100*rect.bottom/bmpHeight),RGB(255,0,0));//点画

  //pDC->Ellipse(100*rect.right/bmpWidth-2,100*rect.bottom/bmpHeight-2,100*rect.right/bmpWidth+2,100*rect.bottom/bmpHeight+2);


//	delete[] pBmpBuf; pBmpBuf=NULL;
  delete[] xsvalue; xsvalue = NULL;
  delete[] fbvalue; fbvalue = NULL;
  delete[] jgvalue; jgvalue = NULL;
  delete[] txvalue; txvalue = NULL;
  if (pDC != NULL)
    pDC->SelectObject(pOldBrush);
  delete[] pColorTable1;//MMY
  pColorTable1 = NULL;//MMY
  return type;
}

//Calcucate angle between vector 1 and 2
//return degree
double CIMGRecognition::VectorAngle(vector v1, vector v2)
{
  //acos return radian,we should transform it into degree  
  double angle =  acos((v1.x * v2.x + v1.y * v2.y) / sqrt((v1.x * v1.x + v1.y * v1.y) * (v2.x * v2.x + v2.y * v2.y))) * 180 / gd_PI;//计算角度
  //if (angle == 0) {
  //  double pn = v1.x / v2.x;
  //  if (pn < 0) {
  //    angle = 180.0;
  //  }
  //}
  return angle;
}

double CIMGRecognition::VectorRadian(vector v1, vector v2)
{
  //acos return radian,we should transform it into degree  
  return acos((v1.x * v2.x + v1.y * v2.y) / sqrt((v1.x * v1.x + v1.y * v1.y) * (v2.x * v2.x + v2.y * v2.y)));//计算弧度

}

//Calcucate length of vector v
//return Length
double CIMGRecognition::VectorLen(vector v)
{
  return sqrt(v.x * v.x + v.y * v.y);
}
//nor axis
//return CW
//1 vf->vb cw
//-1 vf->vb ccw
//bmp axis reverse
int CIMGRecognition::VectorCW(vector vf, vector vb)
{
  int cw;

  cw = vb.x * vf.y - vb.y * vf.x;//>0);?1:-1;
  if (cw >= 0)
    return 1;
  else if (cw < 0)
    return -1;
  else
    return 0;
}
//Calucate distance between point(p) and line(only know its vector v and one point vp)
double CIMGRecognition::Distance(vector v, point p, point vp)
{
  vector tmpVn;
  tmpVn.x = v.y;
  tmpVn.y = -v.x;
  return fabs(tmpVn.x * (vp.x - p.x) + tmpVn.y * (vp.y - p.y)) / sqrt(tmpVn.x * tmpVn.x + tmpVn.y * tmpVn.y);
}

vector CIMGRecognition::VectorProjection(vector v, vector basev)
{
  //
  /*double ang = VectorAngle(basev,v);
  double ang2 = VectorAngle(v,basev);
  double m = cos(ang);
  double len = VectorLen(v);
  double len2 = VectorLen(basev);
  double tmp = len * m / len2;*/
  //
  double tmpA = VectorLen(v) * cos(VectorRadian(v, basev)) / VectorLen(basev);
  vector tmpV;
  tmpV.x = tmpA * basev.x;
  tmpV.y = tmpA * basev.y;
  return tmpV;
}