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
  //font.CreatePointFont(100, _T("����"), NULL);
}


CIMGRecognition::~CIMGRecognition(void)
{
  font.DeleteObject();
}

int CIMGRecognition::readBmp(char* bmpName, unsigned char*& pBmpBuf, int& bmpWidth, int& bmpHeight)
{
  //�����ƶ���ʽ��ָ����ͼ���ļ� 
  FILE* fp;
  fp = NULL;
  int suc = fopen_s(&fp, bmpName, "rb");


  if (suc != 0) return suc;
  //����λͼ�ļ�ͷ�ṹBITMAPFILEHEADER 
  fseek(fp, sizeof(BITMAPFILEHEADER), 0);
  //����λͼ��Ϣͷ�ṹ��������ȡλͼ��Ϣͷ���ڴ棬����ڱ���head�� 
  BITMAPINFOHEADER head;
  fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);
  //��ȡͼ����ߡ�ÿ������ռλ������Ϣ 
  bmpWidth = head.biWidth;
  bmpHeight = head.biHeight;
  biBitCount = head.biBitCount;
  //�������������ͼ��ÿ��������ռ���ֽ�����������4�ı����� 
  int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
  //�Ҷ�ͼ������ɫ������ɫ�����Ϊ256 
  if (biBitCount == 8)   //������ɫ������Ҫ�Ŀռ䣬����ɫ����ڴ� 
  {
    //RGBQUAD *pColorTable1=NULL;//��ɫ��ָ��//mmy ȥ������ɫ�����������ڱ���bmp
    pColorTable1 = new RGBQUAD[256];
    fread(pColorTable1, sizeof(RGBQUAD), 256, fp);
    //delete[]  pColorTable1;
  }
  else
  {
    pColorTable1 = NULL;
  }
  //����λͼ��������Ҫ�Ŀռ䣬��λͼ���ݽ��ڴ� 
  pBmpBuf = new unsigned char[lineByte * bmpHeight];
  fread(pBmpBuf, 1, lineByte * bmpHeight, fp);
  //�ر��ļ� 
  fclose(fp);
  fp = NULL;
  return 0;

}

bool CIMGRecognition::saveBmp(char* bmpName, unsigned char* imgBuf, int width, int height, int biBitCount, RGBQUAD* pColorTable)
{
  //���λͼ����ָ��Ϊ0����û�����ݴ��룬�������� 
  if (!imgBuf)
    return 0;
  //��ɫ���С�����ֽ�Ϊ��λ���Ҷ�ͼ����ɫ��Ϊ1024�ֽڣ���ɫͼ����ɫ���СΪ0 
  int colorTablesize = 0;
  if (biBitCount == 8)
    colorTablesize = 1024;
  //���洢ͼ������ÿ���ֽ���Ϊ4�ı��� 
  int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;
  //�Զ�����д�ķ�ʽ���ļ� 
  FILE * fp;
  fopen_s(&fp, bmpName, "wb");

  // FILE *fp=fopen(bmpName,"wb"); 
  if (fp == 0) return 0;
  //����λͼ�ļ�ͷ�ṹ��������д�ļ�ͷ��Ϣ 
  BITMAPFILEHEADER fileHead;
  fileHead.bfType = 0x4D42;//bmp���� 
  //bfSize��ͼ���ļ�4����ɲ���֮�� 
  fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
    + colorTablesize + lineByte * height;
  fileHead.bfReserved1 = 0;
  fileHead.bfReserved2 = 0;
  //bfOffBits��ͼ���ļ�ǰ3����������ռ�֮�� 
  fileHead.bfOffBits = 54 + colorTablesize;
  //д�ļ�ͷ���ļ� 
  fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);
  //����λͼ��Ϣͷ�ṹ��������д��Ϣͷ��Ϣ 
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
  //дλͼ��Ϣͷ���ڴ� 
  fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);
  //����Ҷ�ͼ������ɫ��д���ļ�  
  if (biBitCount == 8) {
    //pColorTable=new RGBQUAD[256]; //MMY���������½�����ɫ������ֵ��Ĭ��ֵ���Ƕ������ɫ��ֵ�����Ա����bnp������ɫ��ֵ��һ����ֻ��һ����ɫ
    fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);
  }
  //дλͼ���ݽ��ļ� 
  fwrite(imgBuf, height * lineByte, 1, fp);
  //�ر��ļ� 
  fclose(fp);
  //delete[] pColorTable;//MMY
  //pColorTable=NULL;//MMY
  return 1;

}

//��ȡͼ��ֵ
//����-1 ��ȡʧ��
//���������� ͼ��ֵ
//oprtWth ���ӿ��
//oprtDVal ���ӵ������ֵ
//validDVal ���Լ��㷧ֵ����Ч��ֵ
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


//xs��fbȫ0��jg��tx��-1����0��
//����0,ͼ���п�ʶ������
//����1,ͼ���޿�ʶ������
//��Ƭ�����ȡ
int CIMGRecognition::read_image_mul(unsigned char*& pBmpBuf, int*& xsvalue, int*& fbvalue, int*& jgvalue, int*& txvalue, int bmpWidth, int bmpHeight, int threshold, bool debug, bool delNoise = 0)
{
  int type = 1;
  int temp_rgb = 0;
  int i_fz = threshold;//��ֵ
  int i, j;
  int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
  int min_t, max_b, min_l, max_r;
  //min_l=300;max_r=bmpWidth-3;min_t=2;max_b=bmpHeight-3;
  min_l = 3; max_r = bmpWidth - 3; min_t = 2; max_b = bmpHeight - 3;
  //min_l=bmpWidth>>2;max_r=bmpWidth-5;min_t=5;max_b=bmpHeight-5;(1806)
  //min_l=(340);max_r=1815;min_t=5;max_b=bmpHeight-30;
  for (i = 0; i <= bmpHeight - 1; i++)   //���ж�ֵ�����������������ڰ�����ֵ�ж϶�ֵ��������������ȫ����0
  {
    for (j = 0; j <= bmpWidth - 1; j++)
    {
      *(xsvalue + (bmpHeight - 1 - i) * bmpWidth + j) = 0;
      *(fbvalue + (bmpHeight - 1 - i) * bmpWidth + j) = 0;
      temp_rgb = ReadBmpGray(pBmpBuf, biBitCount, bmpWidth, bmpHeight, i, j);
      //temp_rgb=*(pBmpBuf+i*lineByte+j*(biBitCount>>3));

      if (i >= min_t && i <= max_b && j >= min_l && j <= max_r && temp_rgb > i_fz)//��ָ�������ڽ��ж�ֵ��������������Ϊ�Ǻ�ɫ��0
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
  //ȥ��
  if (delNoise)
  {
    int bWhite, bBlack;
    int oprtWth = 32;//��oprtWth�ĳ������ӱ���ȫͼ��ȥ��16
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
                *(fbvalue + m * bmpWidth + n) = 1;//����Ѵ���
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
  int i_fz=threshold;//��ֵ
  int i,j,k;
  int lineByte=(bmpWidth*biBitCount/8+3)/4*4;
  int min_t,max_b,min_l,max_r;
  min_l=1;max_r=bmpWidth-2;min_t=1;max_b=bmpHeight-2;

  for(i=0;i<=bmpHeight-1;i++)   //���ж�ֵ�����������������ڰ�����ֵ�ж϶�ֵ��������������ȫ����0
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
//����-1,��ͼ�����Ĳ�λû�ҵ�Ƭ
//����0,�����ҵ�ͼ��
//����1,�������ҵ�ͼ��,Ƭ����ͼ��λ��
//���ľ�Ƭ������ȡ
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
  int i_fz = 255 - threshold;//,fzk=80;//��ֵ
  int i, j, k;
  int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
  //int temprgb[20]={0};

    //������Ƭ��ֵ����
    //�ȼ��㾧Ƭ���ڵ����������ϼ�ֵ���¼�ֵ����ֵ���Ҽ�ֵ
    //������������ж�ֵ������
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

  int min_l_h, max_r_h, min_t_w, max_b_w; //������߽�������
  min_t = 10000; max_b = 0; min_l = 10000; max_r = 0;
  for (k = 0; k < 1; k++) {
    for (i = bmpzxh; i >= 0; i--)  //bmp��������λ�൱��ͼƬ�е����½� //�����ĵ㿪ʼ��ÿ�н���ѭ���жϣ�������ֵС�ڷ�ֵʱ��ֹ
      //���������������߿�ʼ������ֵ�����ڷ�ֵʱ�˳���ѭ��
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
            min_l = j;  //���±߼�ֵ
            min_l_h = i;
          }
        }
        else {//��������б��̫��Ķ���Σ��������߻ᱻ�˸�����⣬һ���ҵ���ɫ�Ͳ���������һ�����Ҽ�ֵ����һЩ��ͷ��û�жϹ�ȥ
         //�ⲻ��ԭ��ԭ�����������������Ҽ�ֵʱ����һ���ж�==1��ɵ�
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
            max_r = j;  //���±߼�ֵ
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
    if (min_l == 10000 || max_r == 0) break;//���û�м�ֵ�������������Ǻ�ɫ���˳����

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
            min_l = j;  //���ϱ߼�ֵ
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
            max_r = j;  //���ϱ߼�ֵ
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
    if (min_l == 10000 || max_r == 0) break;//���û�м�ֵ�������������Ǻ�ɫ���˳����

    for (j = min_l; j <= bmpWidth - 1; j++)
    {
      tempr = 0;
      templ = 0;
      tempcs = 0;
      for (i = min_l_h; i >= 0; i--)   //minlh��minl��y����
      {
        temp_rgb = *(pBmpBuf + i * lineByte + j * (biBitCount / 8));
        if (temp_rgb > 255 - i_fz) {
          tempr = 1;
          if (min_t > i) {
            min_t = i;  //�±߼�ֵ�����ͼ�������ϱ߼�ֵ
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
            max_b = i;  //�ϱ߼�ֵ�����ͼ�������±߼�ֵ
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
            min_t = i;  //�±߼�ֵ
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
            max_b = i;  //�ϱ߼�ֵ
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
    //���¼����������ߵļ���
    for (i = max_b; i >= 0; i--)
      //���������������߿�ʼ������ֵ�����ڷ�ֵʱ�˳���ѭ��
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
            min_l = j;  //��߼�ֵ
            min_l_h = i;
          }
        }
        else {
          if (tempr == 0) tempcs++;//ԭ���ж�=1��mmy��=0������������Ҽ�ֵû�ҶԵ�ԭ������ͬ
          break;
        }
      }
      for (j = max_b_w + 1; j <= bmpWidth - 1; j++)
      {
        temp_rgb = *(pBmpBuf + i * lineByte + j * (biBitCount / 8));
        if (temp_rgb > 255 - i_fz) {
          templ = 1;
          if (max_r < j) {
            max_r = j;  //�ұ߼�ֵ
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
            min_l = j;  //��߼�ֵ
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
            max_r = j;  //�ұ߼�ֵ
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
    for (i = 0; i <= bmpHeight - 1; i++)   //���ж�ֵ�����������������ڰ�����ֵ�ж϶�ֵ��������������ȫ����0
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

    for (i = 0; i <= bmpHeight - 1; i++)   //���ж�ֵ�����������������ڰ�����ֵ�ж϶�ֵ��������������ȫ����0
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
      return tempi;        //�����ںδ��պ�
    //  exit;
    }
  }
  return -1;   //-1��ʾ���պ�
}
//������������
//�����������
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

  for (temph=tmin_h;temph<=tmax_h;temph++)//��ѭ��
  {
    tempw1=-1;
    tempw2=-1;
  tempflag=-1;
  templn=0;
  for(int j=0;j<1000;j++){//�������������ʼ��
    *(templb+j)=-1;
    *(temple+j)=-1;
  }
    for (templ=tmin_l;templ<=tmax_l;templ++)//��ѭ��
  {
    //mmy
    data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,temph,templ,0,0,0);

    //*(pBmpBuf+(bmpHeight-1-temph)*lineByte+templ*(biBitCount/8))=0;
    //		*(pBmpBuf+(bmpHeight-1-temph)*lineByte+templ*(biBitCount/8)+1)=0;
    //		*(pBmpBuf+(bmpHeight-1-temph)*lineByte+templ*(biBitCount/8)+2)=0;
        //
    if (*(jgvalue+temph*bmpWidth+templ)==qybn) //�����ǰjg��ֵ�Ǵ��������ı�ţ�˵���Ǳ߽�
    {
            *(fbvalue+temph*bmpWidth+templ)=qybn;//�߽����
      *(xsvalue+temph*bmpWidth+templ)=qybn;//���������
      //*(jgvalue+temph*bmpWidth+templ)=fillv;//mmyȥ���û�jg������jg��ԭò
      ///////////////////////////////////////////mmy
      data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,temph,templ,255,0,0);
      temparea++;

      /////////////////////////////////////////////
            if (tempw1==-1)
        tempw1=templ;   //��¼���е�һ������ֵ����������ͬ����
            tempw2=templ;   //��¼���һ����ͬ���к�
    }
    if(tempw1>=0 && *(jgvalue+temph*bmpWidth+templ)==-1){//��������Ѿ���¼��һ������ֵ����������ͬ���У��ҵ�ǰֵ�ǰ�ɫ
      if(*(templb+templn)==-1)  //���
        *(templb+templn)=templ;//��¼templn�е���㵽����templb
    }
    if(*(jgvalue+temph*bmpWidth+templ)==qybn && *(templb+templn)>=0){//�����ǰֵ��0�����Ѿ���¼templn�е���㵽����templb
      *(temple+templn)=templ-1;  //��������¼��¼templn�е��յ�㵽����temple����¼��ǰһ������Ϊ������0�ˣ�ǰһ����-1�����������ڣ�0 �Ǳ߽�
      templn++;
    }


  }
  if(tempw2==tmax_l && *(templb+templn)>-1) {
    *(temple+templn)=tempw2;  //����
    templn++;
  }
    if (tempw1>=0 && tempw2>=0 ){  //�жϱ����Ƿ��к��������ͬ������
    if(templn==0){  //�м�Ϊû��-1�����һ��ֻ�б߽�
      for (tempi=tempw1;tempi<=tempw2;tempi++)
      {
        if(*(xsvalue+temph*bmpWidth+tempi)!=qybn && *(xsvalue+temph*bmpWidth+tempi)!=0)
        {
            *(xsvalue+temph*bmpWidth+tempi)=qybn;  //���������
            *(jgvalue+temph*bmpWidth+tempi)=fillv;
            /////mmy
          ///////////////��ʱȥ��  data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,temph,tempi,255,255,255);
            temparea++;
            ////////////////
        }
      }
    }else{
      for(int tempj=0;tempj<templn;tempj++){
        if(*(temple+tempj)<=tempw2)
        for (tempi=*(templb+tempj);tempi<=*(temple+tempj);tempi++)
        {
          *(xsvalue+temph*bmpWidth+tempi)=qybn; //���������
          *(jgvalue+temph*bmpWidth+tempi)=fillv;
           /////mmy
///////////////��ʱȥ��data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,temph,tempi,255,255,255);
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
//������������
//�����������
//jg tx��������
//����xs��jg��fb
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

  for (temph = tmin_h; temph <= tmax_h; temph++)//��ѭ��
  {
    tempw1 = -1;
    tempw2 = -1;
    tempflag = 0;
    templn = 0;
    for (int j = 0; j < bmpHeight; j++) {//�������������ʼ��
      *(templb + j) = -1;
      *(temple + j) = -1;
    }
    for (templ = tmin_l; templ <= tmax_l; templ++)//��ѭ��
    {
      //mmy
      //if(debug)data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,temph,templ,0,0,0);//_20140708

      //*(pBmpBuf+(bmpHeight-1-temph)*lineByte+templ*(biBitCount/8))=0;
      //		*(pBmpBuf+(bmpHeight-1-temph)*lineByte+templ*(biBitCount/8)+1)=0;
      //		*(pBmpBuf+(bmpHeight-1-temph)*lineByte+templ*(biBitCount/8)+2)=0;
      //����Ǳ߽�
      if (*(jgvalue + temph * bmpWidth + templ) == qybn) //�����ǰjg��ֵ�Ǵ��������ı�ţ�˵���Ǳ߽�
      {
        *(fbvalue + temph * bmpWidth + templ) = qybn;//�߽����
        *(xsvalue + temph * bmpWidth + templ) = qybn;//���������

        if (nMinH > temph) nMinH = temph;
        if (nMaxH < temph) nMaxH = temph;
        if (nMinL > templ) nMinL = templ;
        if (nMaxL < templ) nMaxL = templ;
        //*(jgvalue+temph*bmpWidth+templ)=fillv;//mmyȥ���û�jg������jg��ԭò
        ///////////////////////////////////////////mmy
        if (debug)data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, temph, templ, 255, 255, 255);
        temparea++;

        /////////////////////////////////////////////
        if (tempw1 == -1)
          tempw1 = templ;   //��¼���е�һ�α߽������к�
        tempw2 = templ;   //��¼���һ����ͬ���к�
        continue;
      }
      else if (tempw1 >= 0)//������Ǳ߽絫���ҵ��߽�1
      {
        if (templ == tempw2 + 1)//�жϵ�ǰ���Ƿ���ڱ߽���棬��߽����
        {

          switch (*(txvalue + (temph)* bmpWidth + templ - 1))//�жϵ�ǰ���ǰһ�еĵ�߽�tx������
          {
          case 1://��
            tempflag = -1;
            break;
          case 2://��
            if (*(jgvalue + (temph - 1) * bmpWidth + templ - 1) == qybn && temph > tmin_h && templ > 0)//ǰ���Ϸ�������
            {
              tempflag = -1;
            }
            else if (*(jgvalue + (temph + 1) * bmpWidth + templ - 1) == qybn && temph < tmax_h && templ>0)//ǰ��x��������
            {
              tempflag = 0;
            }
            break;
          case 3://��
            tempflag = 0;
            break;
          case 4://��
            if (debug)AfxMessageBox(_T("unbelievable"));
            break;
          default:
            break;
          }
          if (-1 == tempflag)//���-1��˵���������ڣ���Ҫ���
          {
            *(xsvalue + temph * bmpWidth + templ) = qybn;  //���������
            *(jgvalue + temph * bmpWidth + templ) = fillv;
            /////mmy
            if (debug)data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, temph, templ, 100, 100, 100);
            temparea++;
          }
        }
        else//��߽粻����
        {
          if (-1 == tempflag)
          {
            *(xsvalue + temph * bmpWidth + templ) = qybn;  //���������
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
//temph��templ�����ͼ����ʾ�����Ͻ�
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

//temph��templ�����ͼ����ʾ�����½�
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
//�ж�Բ�β���ȡԲ������
//����5������������
//����4���߶�û��ȫ
//����3��͹��
//����2������
//����1������������
//����0������
//����������������浽ȫ�ֽṹ
int CIMGRecognition::data_check_feature_circleTMP(unsigned char*& pBmpBuf, int biBitCount, int bmpWidth, int bmpHeight, int bj_num, int* bj_h, int* bj_l, int qybn, int*& xsvalue, int*& fbvalue, int*& jgvalue, int*& txvalue, int outpointallowlen, int defectallowlen, int& tmin_h, int& tmax_h, int& tmin_l, int& tmax_l)
{
  double centreh, centrel;//�洢�߽���������
  int* pnCriBjNum = NULL, nCriNum = 0;
  double templen = 0, minlen = 0, maxlen = 0, oklen = 0, radious = 0;//�м���������ڼ���
  int nNum;//�����Ͳ���Ȧ�ϵ����
  int criticalvalue = 0;//�ж�Բ������뾶ƫ��

  //centreh=(tmax_h+tmin_h);

  for (int i = 0; i < bj_num; i++)//ѭ���߽����������
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

      if (0 == nNum)//���i==0
      {
        minlen = templen;
        maxlen = templen;
        nNum++;
        continue;
      }
      if (minlen > templen)//���minlen��templen��
        minlen = templen;
      if (maxlen < templen)//���maxlen��templenС
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
  } while (oklen == 0 && criticalvalue <= bj_num >> 7);//�������������4���Ըı��ҵ���������ҹ�
  //�����ҵ�

  if (oklen == 0)
    return 1;//��״������
  pnCriBjNum = new int[bj_num]; nCriNum = 0;
  do {
    criticalvalue++;
    nNum = 0;
    for (int i = 0; i < bj_num; i++)//
    {
      templen = LenFromP1toP2(centrel, centreh, *(bj_l + i), *(bj_h + i));
      if (abs(oklen - templen) > criticalvalue)//����Ȧ��
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
  //���浽�ṹ
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
      if (fabs(centrel - j) <= radious)//����ͼ������ʾ
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
//�޷ָ�
//�ж�Բ�β���ȡԲ�������򻯰�
//���ط�0��������1.���ܲ���Բ 2.��״���淶
//����0������
//����������������浽ȫ�ֽṹ
int CIMGRecognition::data_check_feature_circle(unsigned char*& pBmpBuf, int biBitCount, int bmpWidth, int bmpHeight, int bj_num, int* bj_h, int* bj_l, int qybn, int*& xsvalue, int*& fbvalue, int*& jgvalue, int*& txvalue, int outpointallowlen, int defectallowlen, int& tmin_h, int& tmax_h, int& tmin_l, int& tmax_l)
{
  double centreh, centrel;//�洢�߽���������
  double templen = 0, minlen = 0, maxlen = 0, oklen = 0, radious = 0;//�м���������ڼ���
  int nNum;//�����Ͳ���Ȧ�ϵ����
  int criticalvalue = defectallowlen;//bj_num/100;//�ж�Բ������뾶ƫ��
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
    int nOKNum = bj_num * 2 / 5;//��������ڷ�Χ�ڵĵ���
    nNum = 0;//���ڷ�Χ�ڵĵ���

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
  //���浽�ṹ
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
      if (fabs(centrel - j) <= radious)//����ͼ������ʾ
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
//�ָ�,���ڸ�������֮��ʹ��
//�ж�Բ�β���ȡԲ�������򻯰�
//���ط�0��������1.���ܲ���Բ 2.��״���淶
//����0������
//����������������浽ȫ�ֽṹ
int CIMGRecognition::data_check_feature_circle_split(unsigned char*& pBmpBuf, int biBitCount, int bmpWidth, int bmpHeight
  , int bj_num, int* bj_h, int* bj_l, int* bj_fx, int qybn, int*& xsvalue, int*& fbvalue, int*& jgvalue, int*& txvalue
  , int outpointallowlen, int defectallowlen, int& tmin_h, int& tmax_h, int& tmin_l, int& tmax_l)
{
  if (bj_num < (stuRef.Rad << 2) || 0 == stuRef.Rad)
  {
    return 1;
  }
  double centreh, centrel;//�洢�߽���������
  double templen = 0, minlen = 0, maxlen = 0, oklen = 0, radious = 0;//�м���������ڼ���
  int nNum, maxi;//�����Ͳ���Ȧ�ϵ����
  int criticalvalue = defectallowlen;//bj_num/100;//�ж�Բ������뾶ƫ��
  int stepLen = stuRef.Rad >> 1;//һ��������������
  int type = 0;
  vector lV = { 0,0 };//����ʱ�ľֲ���ʱ����
  centreh = double(tmax_h + tmin_h) / 2;
  centrel = double(tmax_l + tmin_l) / 2;
  maxlen = double(tmax_h - tmin_h);
  minlen = double(tmax_l - tmin_l);

  /*	if(abs(maxlen - minlen)<criticalvalue && abs( maxlen - minlen)/4>(stuRef.Rad -stuRef.Dev) && abs( maxlen - minlen)/4<(stuRef.Rad +stuRef.Dev))//ֱ���ж�Բ
    {
      minlen=radious;
      maxlen=radious;

      if(g_stu_square.debug)data_bmp_set_color(pBmpBuf,biBitCount, bmpWidth,bmpHeight,(int)centreh,(int)centrel,255,50,50);

      g_stu_square.rcglineok=0;
      int nOKNum=bj_num*2/5;//��������ڷ�Χ�ڵĵ���
      nNum=0;//���ڷ�Χ�ڵĵ���

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
    else//ͨ���жϱ߽���Բ*/
  {
    type = 2;
    int tmpStep = 0;
    vector tmpV[100];//���α��������v
    int tmpfi[100] = { 0 }, fiN = 0, compareFiN = 0;//���α��������v��һ�������ţ�v�ĽǶȺͱ������
    double tmpAng[100] = { 0 };
    bool needReSta = 0;
    int unWanded = 0;//ƽ���ߴ���
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
      if (tmpStep > stepLen)//һ������
      {
        tmpV[fiN].x = lV.x;//���浽��ʱ����
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

        if (0 == fiN)//����ǵ�һ������
        {
          //fiN ++;//���ָ����һ������
        }
        else if (-1 == iCW)//����˳ʱ��
        {
          if (dAngle < 15)
          {
            unWanded++;
          }
          else if (dAngle > 40 || unWanded >= 2)//|| unWanded ==1&& dAngle>90)//�Ƕ�̫��
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

              //������
              centrel = *(bj_l + tmpfi[0]);//��ʱ����һ��
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
              //���浽�������
              g_stu_square.pnWth[g_stu_square.nN] = (int)radious;
              g_stu_square.pnZPX[g_stu_square.nN] = (int)centrel;
              g_stu_square.pnZPY[g_stu_square.nN] = (int)centreh;
              //���浽�ṹ
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
              //ȥ���ظ�������
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
                  //if(sqrt(pow(double(g_stu_square.pnZPX[i]-xt),2)+pow(double(g_stu_square.pnZPY[i]-yt),2))<(stuRef.Rad)) //���λ����Ƭ�Ĵ����������С��s_sort.allowable_devision����ô����Ҫ���Ե��㲻׼ʱ���õ���
                }
                //��ͼ���������
                if (1 == lenOK)
                {
                  if (g_stu_square.debug)
                    for (int j = tmin_l; j < tmax_l; j++)//
                    {
                      if (fabs(centrel - j) <= radious)//����ͼ������ʾ
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
              ///////////////////////////////////////Խ����
              if (g_stu_square.nN == 20)
              {
                return type;
              }
              needReSta = 1;
              //break;
            }
          }
        }
        else if (1 == iCW)//������ʱ��
        {
          if (dAngle < 10)
          {
            //fiN ++;
            unWanded++;
          }
          else//�Ƕ�̫��
          {
            needReSta = 1;
          }
        }
        else
        {
          //fiN ++;
          unWanded++;
        }

        if (1 == needReSta)//�����Ҫ���¿�ʼ��Բ������
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
          for (int fii = tmpfi[fiN - 1]; fii < fi; fii++)//����Զ������ƥ��
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
          fiN = 0;//���¿�ʼ
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

        tmpfi[fiN] = fi;//������ȡ��һ����������ʼ����
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
    //������
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
  //���浽�ṹ
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
      if(fabs(centrel-j)<=radious)//����ͼ������ʾ
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
//��ȡ���������߳�
//����5������������
//����4���߶�û��ȫ
//����3��͹��
//����2������
//����1������������
//����0������
//����������������浽ȫ�ֽṹ
int CIMGRecognition::data_check_feature(unsigned char*& pBmpBuf, int biBitCount, int bmpWidth, int bmpHeight, int bj_num, int* bj_h, int* bj_l, int qybn, int*& xsvalue, int*& fbvalue, int*& jgvalue, int*& txvalue, int outpointallowlen/*�ж�ֱ������������*/, int defectallowlen/*�ж�ȱ��in or out��*/)
{
  double centreh, centrel;//�洢�߽���������
  int pi_point_h[50];//�۵�������
  int pi_point_l[50];//�۵�������
  int pi_point_no[50];//�۵�λ��
  int pi_point_trend[50];//�۵������
  int beginno = -1;//��ʼ���ĵ�һ���۵�
  bool trend = 0;//���ƣ�1������0�½���һ��ʼ��0
  int i_point_num = 0;//�۵���
  double templen, minlen, maxlen;//�м���������ڼ����۵�
  int minno, maxno;//��ϼ�¼����ĵ���bj�����е�λ��
  int minnum, maxnum;//��¼�ҵ�min�㣬max��ļ���ֵ����max����criticalvalue���ͱ�ʾ�ҵ�min�㣻���min����criticalvalue���ͱ�ʾ�ҵ�max��
  int criticalvalue = bj_num / 360;//���������ٽ�������̶�ֵ
  //int outpointallowlen=4;//����ĵ㵽ֱ�ߵľ���
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
    beginno = -1;//��ʼ���ĵ�һ���۵�
    trend = 0;//���ƣ�1������0�½���һ��ʼ��0
    i_point_num = 0;//�۵���
    g_stu_square.rcglineok = 0;
    g_stu_square.bPN[g_stu_square.nN] = 0;
    for (int i = 0; i < bj_num; i++)//ѭ���߽����������
    {
      SortAvg(i + 1, *(bj_h + i), centreh);
      SortAvg(i + 1, *(bj_l + i), centrel);
    }

    for (int i = 0; i < bj_num; i++)//ѭ���߽���һ��ʼ����������
    {
      templen = LenFromP1toP2(centrel, centreh, *(bj_l + i), *(bj_h + i));

      if (!i)//���i==0
      {
        minlen = templen; minno = i; minnum = 0;
        maxlen = templen; maxno = i; maxnum = 0;
        continue;
      }

      if (minlen > templen)//���minlen��templen���½�����
      {
        minlen = templen; minno = i; minnum++;
      }
      if (maxlen < templen)//���maxlen��templenС����������
      {
        maxlen = templen; maxno = i; maxnum++;
      }

      if (minnum > criticalvalue || maxnum > criticalvalue)//��������ٽ�ֵ
      {
        trend = maxnum > minnum ? 1 : 0;
        break;
      }
    }

    for (int i = 0; i<bj_num && i>beginno || i < beginno; i = (bj_num == i + 1) ? 0 : (i + 1))//i*pow(double(bj_num-1-i),0)+1)//i*=pow(double(bj_num- ++i),0)����Ч����++i����=�ţ�����i��ǰ���
    //for (int i=0;i<bj_num;i++)//ѭ���߽���һ����׼�۵���Ϊ��ʼ
    {
      if (bj_num == i + 1 && -1 == beginno)//��ֹԲ�ε��¸�ѭ���Ҳ�����ʼ���ƶ������˳�
        break;
      templen = LenFromP1toP2(centrel, centreh, *(bj_l + i), *(bj_h + i));

      if (!i && beginno == -1)//���i==0
      {
        minlen = templen; minno = i; minnum = 0;
        maxlen = templen; maxno = i; maxnum = 0;
        continue;
      }

      if (minlen > templen)//���minlen��templen���½�����
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
      if (maxlen < templen)//���maxlen��templenС����������
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

      if (minnum > criticalvalue || maxnum > criticalvalue)//��������ٽ�ֵ
      {
        pi_point_no[i_point_num] = maxnum > minnum ? minno : maxno;
        if (-1 == beginno)
        {
          beginno = pi_point_no[i_point_num];
        }
        if (minnum > criticalvalue)
        {

          pi_point_h[i_point_num] = *(bj_h + pi_point_no[i_point_num]);//�۵�������
          pi_point_l[i_point_num] = *(bj_l + pi_point_no[i_point_num]);//�۵�������
          pi_point_trend[i_point_num] = trend;//�۵������,�̵�0��Զ��1��
          i_point_num++;
        }
        trend = !trend;

        minlen = templen; minno = i; minnum = 0;
        maxlen = templen; maxno = i; maxnum = 0;

        //break;
      }
    }
  } while (i_point_num != 4 && criticalvalue < bj_num >> 4);//�������������4���Ըı��ҵ���������ҹ�
  //�����ҵ�
  if (g_stu_square.debug)
    for (int i = 0; i < i_point_num; i++)//����֮һ������
      data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, pi_point_h[i], pi_point_l[i], 0, 255, 0);
  if (i_point_num != 4)
  {
    return 1;//��״������
  }
  double* pd_line_a, * pd_line_b, * pd_line_c;
  int successequationnum = 0, outs = 0, * i_out_no = NULL, * pnOutLnNo = NULL;//outs�ܵĲ����߶��ϵĵ���
  double* i_out_len = NULL;
  i_out_no = new int[bj_num]; //����Ǳ߽�İ�͹�����λ�ñ�ţ���Ƭ�ص�ʱ�����е㶼�п����ڱ߽��⣬���и����鲻�ܱ�bjnumС
  pnOutLnNo = new int[bj_num];//����ÿ��out�����������߶���
  i_out_len = new double[bj_num]; //����Ǳ߽�Ķ�Ӧ�ľ���
  pd_line_a = new double[4];
  pd_line_b = new double[4];
  pd_line_c = new double[4];
  for (int i = 0; i < 4; i++)//����߶�
  {
    //g_stu_square.i_maxlen_defect=0;
    int startno, endno, totalouts = 0;//,totalouts�ܵĲ��ڵ�ǰ�߶��ϵĵ���
    double a, b, c, lens;//ax+by+c=0,lens�㵽ֱ�߾�����ۼ�
    int nexti = (i == 3) ? 0 : i + 1;

    if (pi_point_no[nexti] > pi_point_no[i])//
    {
      startno = (pi_point_no[nexti] + pi_point_no[i]) >> 1;
      endno = pi_point_no[nexti] - 1;
      for (; startno > pi_point_no[i] && endno < pi_point_no[nexti]; startno--, endno--)//�ҷ���ֱ��
      {
        if (!CalcEquationABC(bj_l[startno], bj_h[startno], bj_l[endno], bj_h[endno], a, b, c))
          continue;
        outs -= totalouts;
        totalouts = 0;
        lens = 0;
        for (int j = pi_point_no[i]; j < pi_point_no[nexti]; j++)//�߽�����ֱ��ƥ��
        {
          double templen = LenFromPtoL(bj_l[j], bj_h[j], a, b, c);
          if (templen > outpointallowlen)
          {
            i_out_no[outs] = j;
            pnOutLnNo[outs] = i;
            i_out_len[outs] = templen;
            outs++;
            totalouts++;
            //if(g_stu_square.i_maxlen_defect<templen)//����������
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
          g_stu_square.d_a[i] = a;//����
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

      startno = pi_point_no[i] + ((pi_point_no[nexti] + bj_num - pi_point_no[i]) >> 1);//�������ȼ���
      endno = pi_point_no[nexti] - 1;
      if (startno >= bj_num)
        startno = startno - bj_num;
      if (endno < 0)
        endno = bj_num + endno;
      for (; (startno > pi_point_no[i] || startno < pi_point_no[nexti]) && (endno > pi_point_no[i] || endno < pi_point_no[nexti]);
        startno = (0 == startno) ? bj_num - 1 : (startno - 1), endno = (0 == endno) ? bj_num - 1 : (endno - 1))//�ҷ���ֱ��
      {
        if (!CalcEquationABC(bj_l[startno], bj_h[startno], bj_l[endno], bj_h[endno], a, b, c))
          continue;
        outs -= totalouts;
        totalouts = 0;
        lens = 0;
        for (int j = pi_point_no[i];
          j < bj_num && j >= pi_point_no[i] || j <= pi_point_no[nexti];
          j = (bj_num == j + 1) ? 0 : (j + 1))//�߽�����ֱ��ƥ��
        {
          double templen = LenFromPtoL(bj_l[j], bj_h[j], a, b, c);
          if (templen > outpointallowlen)//�Ƿ񳬹��涨�ľ���
          {
            i_out_no[outs] = j;
            pnOutLnNo[outs] = i;
            i_out_len[outs] = templen;
            outs++;
            totalouts++;
            //if(g_stu_square.i_maxlen_defect<templen)//����������
            //	g_stu_square.i_maxlen_defect=templen;
          }
          else
          {
            lens += templen;
          }
          if (outs > bj_num || totalouts > (pi_point_no[nexti] + bj_num - pi_point_no[i]) >> 1 || (int)lens > (((pi_point_no[nexti] + bj_num - pi_point_no[i] + 1 - totalouts) * (outpointallowlen) >> 2) + ((pi_point_no[nexti] + bj_num - pi_point_no[i] + 1 - totalouts) >> 6) + ((pi_point_no[nexti] + bj_num - pi_point_no[i] + 1 - totalouts) >> 8)))//���������ʶ�������һ��+ȥ��
            break;
        }
        if (outs > bj_num)
          break;
        if (totalouts > (pi_point_no[nexti] + bj_num - pi_point_no[i]) >> 1 || (int)lens > (((pi_point_no[nexti] + bj_num - pi_point_no[i] + 1 - totalouts) * (outpointallowlen) >> 2) + ((pi_point_no[nexti] + bj_num - pi_point_no[i] + 1 - totalouts) >> 6) + ((pi_point_no[nexti] + bj_num - pi_point_no[i] + 1 - totalouts) >> 8)))
          continue;
        else
        {
          pd_line_a[i] = a, pd_line_b[i] = b, pd_line_c[i] = c;
          g_stu_square.d_a[i] = a;//����
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
  }//����߶ν���

  if (successequationnum != 4)//����߶η��̲�����4
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
    return 5;//����������

  }
  //����þ��α߳�//�����浽����
  g_stu_square.side = 0;

  for (int m = 0; m < 4; m++)//����߳�
  {
    int n = (m == 0 ? 3 : m - 1);
    g_stu_square.d_len[m] = LenFromP1toP2(g_stu_square.d_point_x[m], g_stu_square.d_point_y[m], g_stu_square.d_point_x[n], g_stu_square.d_point_y[n]);
    g_stu_square.side += (int)g_stu_square.d_len[m];
  }
  //���泤��ȫ������
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

  int sags = 0, crests = 0, unceasouts = 0, unceasallownum = bj_num >> 5;//sags���ǵ�����,crests͹������
  //���߽���
  if (1)
  {
    if (g_stu_square.debug)
      for (int j = 1; j < bmpWidth - 2; j++)//����֮һ���ߵ���x
      {
        for (int li = 0; li < 4; li++)
          if (fabs(pd_line_a[li]) < fabs(pd_line_b[li]))//����ͼ������ʾ
            data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, 0 == pd_line_b[li] ? 0 : int(-(pd_line_a[li] * j + pd_line_c[li]) / pd_line_b[li]), 0 == pd_line_b[li] ? int(-pd_line_c[li] / pd_line_a[li]) : j, 255, 255, 0);
      }
    if (g_stu_square.debug)
      for (int j = 1; j < bmpHeight - 2; j++)//����֮һ���ߵ���y
      {
        for (int li = 0; li < 4; li++)
          if (fabs(pd_line_a[li]) >= fabs(pd_line_b[li]))
            data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, 0 == pd_line_a[li] ? int(-pd_line_c[li] / pd_line_b[li]) : j, 0 == pd_line_a[li] ? 0 : int(-(pd_line_b[li] * j + pd_line_c[li]) / pd_line_a[li]), 255, 255, 0);
      }
  }
  //double l_maxlen=0;
  int nBeginj = 0, nEndj = 0, nUnceasNo = 0;//������ioutno������ȱ�ݿ�ʼ����λ�ã���outs��ȱ��
  bool bIn = 0;

  //����֮һ��ȱ�ݵ㣬���ñ����ȱ�ݵ��ţ��ӱ߽�������ȡ��������
  for (int j = 0; j < outs; j++)
  {

    if (g_stu_square.debug)data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, *(bj_h + i_out_no[j]), *(bj_l + i_out_no[j]), 0, 0, 100);
    //double templen=LenFromPtoL(*(bj_l+i_out_no[j]),*(bj_h+i_out_no[j],a,b,c);



    bool in = InOrOut(*(bj_l + i_out_no[j]), *(bj_h + i_out_no[j]), pd_line_a, pd_line_b, pd_line_c);
    if (j > 0)
      if (i_out_no[j] == i_out_no[j - 1] + 1)//���������һ��ţ�ͬһȱ��
      {
        if (i_out_len[j] > defectallowlen && i_out_len[j - 1] > defectallowlen)
        {

          if (g_stu_square.debug)data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, *(bj_h + i_out_no[j]), *(bj_l + i_out_no[j]), 0, 255, 255);
          unceasouts++;//�����ж�in or out�ĵ���

        }
        nUnceasNo++;//һ��ȱ�ݵĵ���
        nEndj = j;
        bIn = in;
      }
      else//һ��ȱ�ݽ���
      {
        if (nUnceasNo > unceasallownum/2)
        {
          ///////////////////////////////////////////////////////ƥ���жϷ��������START////////////////////////////////////////////////////////////////////////////////////////
          int startno, endno, totalouts = 0, outs = 0;//,totalouts�ܵĲ��ڵ�ǰ�߶��ϵĵ���
          double a, b, c, lens;//ax+by+c=0,lens�㵽ֱ�߾�����ۼ�
          bool bSuccess = 0;
          if (i_out_no[nEndj] > i_out_no[nBeginj])//
          {
            startno = (i_out_no[nEndj] + i_out_no[nBeginj]) >> 1;
            endno = i_out_no[nEndj] - 1;
            for (; startno > i_out_no[nBeginj] && endno < i_out_no[nEndj]; startno--, endno--)//�ҷ���ֱ��
            {
              if (!CalcEquationABC(bj_l[startno], bj_h[startno], bj_l[endno], bj_h[endno], a, b, c))
                continue;
              outs -= totalouts;
              totalouts = 0;
              lens = 0;
              for (int j = i_out_no[nBeginj]; j < i_out_no[nEndj]; j++)//�߽�����ֱ��ƥ��
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

            startno = i_out_no[nBeginj] + ((i_out_no[nEndj] + bj_num - i_out_no[nBeginj]) >> 1);//�������ȼ���
            endno = i_out_no[nEndj] - 1;
            if (startno >= bj_num)
              startno = startno - bj_num;
            if (endno < 0)
              endno = bj_num + endno;
            for (; (startno > i_out_no[nBeginj] || startno < i_out_no[nEndj]) && (endno > i_out_no[nBeginj] || endno < i_out_no[nEndj]);
              startno = (0 == startno) ? bj_num - 1 : (startno - 1), endno = (0 == endno) ? bj_num - 1 : (endno - 1))//�ҷ���ֱ��
            {
              if (!CalcEquationABC(bj_l[startno], bj_h[startno], bj_l[endno], bj_h[endno], a, b, c))
                continue;
              outs -= totalouts;
              totalouts = 0;
              lens = 0;
              for (int j = i_out_no[nBeginj];
                j < bj_num && j >= i_out_no[nBeginj] || j <= i_out_no[nEndj];
                j = (bj_num == j + 1) ? 0 : (j + 1))//�߽�����ֱ��ƥ��
              {
                double templen = LenFromPtoL(bj_l[j], bj_h[j], a, b, c);
                if (templen > outpointallowlen)//�Ƿ񳬹��涨�ľ���
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

          //��������
          if (bSuccess)
          {
            int nLenNo1, nLenNo2;//�����������ڱߵı��
            if (pnOutLnNo[nBeginj] == pnOutLnNo[nEndj])
            {
              int nLenNo = pnOutLnNo[nBeginj];//����ڼ����߶�0~3
              int nLenNoNext = (3 == nLenNo) ? 0 : (nLenNo + 1);
              int nDstnBegin = i_out_no[nBeginj] >= pi_point_no[nLenNo] ? i_out_no[nBeginj] - pi_point_no[nLenNo] : i_out_no[nBeginj] + bj_num - pi_point_no[nLenNo];//�������߿�ʼ�㵽�����������߶εĿ�ʼ��֮��ľ���
              int nDstnEnd = pi_point_no[nLenNoNext] >= i_out_no[nEndj] ? pi_point_no[nLenNoNext] - i_out_no[nEndj] : pi_point_no[nLenNoNext] + bj_num - i_out_no[nEndj];//�������߽����㵽�����������߶εĽ�����֮��ľ���
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
            //�����һ������x������ļн�
            if(g_stu_square.bPN[g_stu_square.nN] != 0)
              g_stu_square.angreePN[g_stu_square.nN] = CalculateVectorAngle(g_stu_square.pnPX[g_stu_square.nN][nLenNo2]- g_stu_square.pnPX[g_stu_square.nN][nLenNo1],
                g_stu_square.pnPY[g_stu_square.nN][nLenNo2] - g_stu_square.pnPY[g_stu_square.nN][nLenNo1],
                1,0);
            g_stu_square.lenNo1PN[g_stu_square.nN] = nLenNo1;
            //
              break;
            case 1:
              g_stu_square.bPN[g_stu_square.nN] = 1;
            //�����һ������x������ļн�
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
              for (int j = 1; j < bmpWidth - 2; j++)//����֮һ���ߵ���x
              {
                if (fabs(a) < fabs(b))//����ͼ������ʾ
                  data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, 0 == b ? 0 : int(-(a * j + c) / b), 0 == b ? int(-c / a) : j, 255, 255, 0);
              }

              for (int j = 1; j < bmpHeight - 2; j++)//����֮һ���ߵ���y
              {
                if (fabs(a) >= fabs(b))
                  data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, 0 == a ? int(-c / b) : j, 0 == a ? 0 : int(-(b * j + c) / a), 255, 255, 0);
              }
            }
          }
          /////////////////////////////////////////////////////////ƥ���жϷ��������END///////////////////////////////////////////////////////////////////////////////////////////////////
        }
        unceasouts = 0;
        nUnceasNo = 0;
        nBeginj = j;
        nEndj = j;
        bIn = 0;
      }
    if (unceasouts == unceasallownum)
    {
      if (in)//�Ƿ���ǣ�����ֱ�����
        sags++;
      else
        crests++;
      unceasouts++;//��ֹ����ж��������
    }

    //������һ��ѭ�����ж��Ƿ����۽ǣ���ֹ��һ���˳��󣬸��۽ǲ�����
    if (j == outs - 1)
      if (nUnceasNo > unceasallownum/2)
      {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int startno, endno, totalouts = 0, outs = 0;//,totalouts�ܵĲ��ڵ�ǰ�߶��ϵĵ���
        double a, b, c, lens;//ax+by+c=0,lens�㵽ֱ�߾�����ۼ�
        bool bSuccess = 0;
        if (i_out_no[nEndj] > i_out_no[nBeginj])//
        {
          startno = (i_out_no[nEndj] + i_out_no[nBeginj]) >> 1;
          endno = i_out_no[nEndj] - 1;
          for (; startno > i_out_no[nBeginj] && endno < i_out_no[nEndj]; startno--, endno--)//�ҷ���ֱ��
          {
            if (!CalcEquationABC(bj_l[startno], bj_h[startno], bj_l[endno], bj_h[endno], a, b, c))
              continue;
            outs -= totalouts;
            totalouts = 0;
            lens = 0;
            for (int j = i_out_no[nBeginj]; j < i_out_no[nEndj]; j++)//�߽�����ֱ��ƥ��
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

          startno = i_out_no[nBeginj] + ((i_out_no[nEndj] + bj_num - i_out_no[nBeginj]) >> 1);//�������ȼ���
          endno = i_out_no[nEndj] - 1;
          if (startno >= bj_num)
            startno = startno - bj_num;
          if (endno < 0)
            endno = bj_num + endno;
          for (; (startno > i_out_no[nBeginj] || startno < i_out_no[nEndj]) && (endno > i_out_no[nBeginj] || endno < i_out_no[nEndj]);
            startno = (0 == startno) ? bj_num - 1 : (startno - 1), endno = (0 == endno) ? bj_num - 1 : (endno - 1))//�ҷ���ֱ��
          {
            if (!CalcEquationABC(bj_l[startno], bj_h[startno], bj_l[endno], bj_h[endno], a, b, c))
              continue;
            outs -= totalouts;
            totalouts = 0;
            lens = 0;
            for (int j = i_out_no[nBeginj];
              j < bj_num && j >= i_out_no[nBeginj] || j <= i_out_no[nEndj];
              j = (bj_num == j + 1) ? 0 : (j + 1))//�߽�����ֱ��ƥ��
            {
              double templen = LenFromPtoL(bj_l[j], bj_h[j], a, b, c);
              if (templen > outpointallowlen)//�Ƿ񳬹��涨�ľ���
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
        //��������
        if (bSuccess)
        {
          int nLenNo1, nLenNo2;//�����������ڱߵı��
          if (pnOutLnNo[nBeginj] == pnOutLnNo[nEndj])
          {
            int nLenNo = pnOutLnNo[nBeginj];//����ڼ����߶�0~3
            int nLenNoNext = (3 == nLenNo) ? 0 : (nLenNo + 1);
            int nDstnBegin = i_out_no[nBeginj] >= pi_point_no[nLenNo] ? i_out_no[nBeginj] - pi_point_no[nLenNo] : i_out_no[nBeginj] + bj_num - pi_point_no[nLenNo];//�������߿�ʼ�㵽�����������߶εĿ�ʼ��֮��ľ���
            int nDstnEnd = pi_point_no[nLenNoNext] >= i_out_no[nEndj] ? pi_point_no[nLenNoNext] - i_out_no[nEndj] : pi_point_no[nLenNoNext] + bj_num - i_out_no[nEndj];//�������߽����㵽�����������߶εĽ�����֮��ľ���
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
          //�����һ������x������ļн�
          if (g_stu_square.bPN[g_stu_square.nN] != 0)
            g_stu_square.angreePN[g_stu_square.nN] = CalculateVectorAngle(g_stu_square.pnPX[g_stu_square.nN][nLenNo2] - g_stu_square.pnPX[g_stu_square.nN][nLenNo1],
              g_stu_square.pnPY[g_stu_square.nN][nLenNo2] - g_stu_square.pnPY[g_stu_square.nN][nLenNo1],
              1, 0);
          g_stu_square.lenNo1PN[g_stu_square.nN] = nLenNo1;
          //
            break;
          case 1:
            g_stu_square.bPN[g_stu_square.nN] = 1;
          //�����һ������x������ļн�
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
            for (int j = 1; j < bmpWidth - 2; j++)//����֮һ���ߵ���x
            {
              if (fabs(a) < fabs(b))//����ͼ������ʾ
                data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, 0 == b ? 0 : int(-(a * j + c) / b), 0 == b ? int(-c / a) : j, 255, 255, 0);
            }

            for (int j = 1; j < bmpHeight - 2; j++)//����֮һ���ߵ���y
            {
              if (fabs(a) >= fabs(b))
                data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, 0 == a ? int(-c / b) : j, 0 == a ? 0 : int(-(b * j + c) / a), 255, 255, 0);
            }
          }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      }

  }
  //���浽�ṹ
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
//��ȡ���������߳�//�ָ�,���ڸ�������֮��ʹ��
//����5������������
//����1������������
//����0������
//����������������浽ȫ�ֽṹ
int CIMGRecognition::data_check_feature_split(unsigned char*& pBmpBuf, int biBitCount, int bmpWidth, int bmpHeight
  , int bj_num, int* bj_h, int* bj_l, int* bj_fx, int qybn, int*& xsvalue, int*& fbvalue, int*& jgvalue, int*& txvalue
  , int outpointallowlen, int defectallowlen)
{
  if (bj_num < (stuRef.Len + stuRef.Wth) || 0 == stuRef.Len)
  {
    return 5;
  }
  double centreh, centrel;//�洢�߽���������
  vector vertexV[200];//�۵���������
  int vertexNo[200];//�۵���bjnumλ��
  int vertexAng[200];//�۵�ĽǶȣ�˳ʱ��<0,��ʱ��>0
  int vertexAlowN = 200;
  int vertexN = 0;//�۵���
  int i_point_num = 0;//�۵���
  double templen = 0, minlen = 0, maxlen = 0;//,oklen=0,radious=0;//�м���������ڼ����۵�
  int nNum, maxi;//�����Ͳ������ϵ����
  int minno, maxno;//��ϼ�¼����ĵ���bj�����е�λ��
  int minnum, maxnum;//��¼�ҵ�min�㣬max��ļ���ֵ����max����criticalvalue���ͱ�ʾ�ҵ�min�㣻���min����criticalvalue���ͱ�ʾ�ҵ�max��
  int criticalvalue = defectallowlen;//���������ٽ�������̶�ֵ
  int stepLen = stuRef.Wth >> 3;//һ��������������
  int type = 5;
  vector lV = { 0,0 };//����ʱ�ľֲ���ʱ����

  int tmpStep = 0;
  vector tmpV[100];//���α��������v
  vector sumlV = { 0,0 }, sumV0 = { 0,0 };
  int tmpfi[100] = { 0 }, fiN = 0;//���α��������v��һ�������ţ��ͱ������
  double tmpAng[100] = { 0 };//v����һv����һ��v��x��ĽǶ�
  bool needReSta = 0, staOK = 0;
  int unWanded = 0;//ƽ���ߴ���
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

    if (fi == bj_num - 1)//���һ������ʼ��Ĺ�ϵ
    {
      //if(tmpStep <= stepLen)// && fiN!=0)//���һ������ֻ��һ������
      {
        int iCW;
        double dAngle;
        vector breakV; //���ڼ������һ������	
        breakV.x = *(bj_l + tmpfi[fiN]) - *(bj_l + tmpfi[0]);
        breakV.y = *(bj_h + tmpfi[fiN]) - *(bj_h + tmpfi[0]);
        iCW = VectorCW(vertexV[vertexN - 2], breakV);//���ڶ��������һ���ϸ������Ĺ�ϵ
        dAngle = VectorAngle(vertexV[vertexN - 2], breakV);
        vertexAng[vertexN - 1] = iCW * dAngle;



        iCW = VectorCW(breakV, vertexV[0]);//��һ�������һ���ϸ������Ĺ�ϵ
        dAngle = VectorAngle(breakV, vertexV[0]);

        if (dAngle >= 10)
        {
          //���۵�
          breakV.x += vertexV[0].x + lV.x;
          breakV.y += vertexV[0].y + lV.y;
          point breakPoint, vPoint;
          double farPL = 0;
          int farPi = 0;
          vPoint.x = *(bj_l + vertexNo[1]);
          vPoint.y = *(bj_h + vertexNo[1]);
          /////////////////////////////////
          for (int i = tmpfi[fiN - 1]; 0 <= i && i < vertexNo[1] || tmpfi[fiN - 1] <= i && i < bj_num; i = (bj_num == i + 1) ? 0 : (i + 1))//����Զ������ƥ��
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

            vertexV[vertexN - 1].x = *(bj_l + farPi) - *(bj_l + vertexNo[vertexN - 1]);//�������һ������
            vertexV[vertexN - 1].y = *(bj_h + farPi) - *(bj_h + vertexNo[vertexN - 1]);
            vertexV[0].x = *(bj_l + vertexNo[1]) - *(bj_l + farPi);//���õ�һ������
            vertexV[0].y = *(bj_h + vertexNo[1]) - *(bj_h + farPi);
            breakV.x = *(bj_l + farPi) - *(bj_l + vertexNo[vertexN - 1]);
            breakV.y = *(bj_h + farPi) - *(bj_h + vertexNo[vertexN - 1]);

            iCW = VectorCW(vertexV[vertexN - 2], breakV);//���ڶ��������һ���ϸ������Ĺ�ϵ
            dAngle = VectorAngle(vertexV[vertexN - 2], breakV);
            vertexAng[vertexN - 1] = iCW * dAngle;
            iCW = VectorCW(breakV, vertexV[0]);//��һ�������һ���ϸ������Ĺ�ϵ
            dAngle = VectorAngle(breakV, vertexV[0]);
            vertexAng[0] = iCW * dAngle;
            iCW = VectorCW(vertexV[0], vertexV[1]);//��һ�������һ���ϸ������Ĺ�ϵ
            dAngle = VectorAngle(vertexV[0], vertexV[1]);
            vertexAng[1] = iCW * dAngle;
          }
          //else
          //	vertexAng[0] = 0;
        }
        else
        {
          /*int farPi = vertexNo[0] ;

          vertexV[vertexN-1].x = *(bj_l + farPi)- *(bj_l + vertexNo[vertexN-1]);//�������һ������
          vertexV[vertexN-1].y = *(bj_h + farPi) - *(bj_h + vertexNo[vertexN-1]);
          vertexV[0].x = *(bj_l + vertexNo[1])-*(bj_l + farPi);//���õ�һ������
          vertexV[0].y = *(bj_h + vertexNo[1])-*(bj_h + farPi);
          breakV.x =  *(bj_l + farPi) - *(bj_l +vertexNo[vertexN-1]);
          breakV.y = *(bj_h + farPi) - *(bj_h +vertexNo[vertexN-1]);

          iCW = VectorCW(vertexV[vertexN-2],breakV);//���ڶ��������һ���ϸ������Ĺ�ϵ
          dAngle = VectorAngle(vertexV[vertexN-2],breakV);
          vertexAng[vertexN-1] = iCW * dAngle;
          iCW = VectorCW(breakV,vertexV[0]);//��һ�������һ���ϸ������Ĺ�ϵ
          dAngle = VectorAngle(breakV,vertexV[0]);
          vertexAng[0] = iCW * dAngle;*/
          vertexNo[0] = vertexNo[vertexN - 1];
          int farPi = vertexNo[0];
          vertexN--;
          //vertexV[vertexN-1].x = *(bj_l + farPi)- *(bj_l + vertexNo[vertexN-1]);//�������һ������
          //vertexV[vertexN-1].y = *(bj_h + farPi) - *(bj_h + vertexNo[vertexN-1]);
          vertexV[0].x = *(bj_l + vertexNo[1]) - *(bj_l + farPi);//���õ�һ������
          vertexV[0].y = *(bj_h + vertexNo[1]) - *(bj_h + farPi);
          breakV.x = *(bj_l + farPi) - *(bj_l + vertexNo[vertexN - 1]);
          breakV.y = *(bj_h + farPi) - *(bj_h + vertexNo[vertexN - 1]);

          iCW = VectorCW(vertexV[vertexN - 2], breakV);//���ڶ��������һ���ϸ������Ĺ�ϵ
          dAngle = VectorAngle(vertexV[vertexN - 2], breakV);
          vertexAng[vertexN - 1] = iCW * dAngle;
          iCW = VectorCW(breakV, vertexV[0]);//��һ�������һ���ϸ������Ĺ�ϵ
          dAngle = VectorAngle(breakV, vertexV[0]);
          vertexAng[0] = iCW * dAngle;
          iCW = VectorCW(vertexV[0], vertexV[1]);//��һ�������һ���ϸ������Ĺ�ϵ
          dAngle = VectorAngle(vertexV[0], vertexV[1]);
          vertexAng[1] = iCW * dAngle;
        }
      }
      break;
    }
    else if (tmpStep > stepLen)//һ������
    {
      sumlV.x += lV.x;
      sumlV.y += lV.y;
      tmpV[fiN].x = lV.x;//���浽��ʱ����
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

      if (0 == fiN)//����ǵ�һ������
      {
        vertexNo[vertexN] = tmpfi[0];//����ȫ�ٽ�����״ƥ��
        //;//�۵������
        vertexN++;//�۵���++
        sumV0.x = sumlV.x;
        sumV0.y = sumlV.y;
        if (vertexN >= vertexAlowN)
          return type;
      }
      else if (-1 == iCW)//����˳ʱ��
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
      else if (1 == iCW)//������ʱ��
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
        else//�Ƕ�̫��
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

      if (1 == needReSta || unWanded > 0 && (bj_num - 1 - fi) <= stepLen)//�����Ҫ���¿�ʼ���۵�
      {

        needReSta = 0;
        vector breakV = { 0,0 }; //�������۵��ֱ������
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


        for (int fii = tmpfi[fiN - (unWanded)]; fii < tmpfi[fiN]; fii++)//����Զ������ƥ��
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
        fiN = 0;//���¿�ʼ
        fi = farPi;
        vertexV[vertexN - 1].x = *(bj_l + farPi) - *(bj_l + vertexNo[vertexN - 1]);//�����۵���������
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

      tmpfi[fiN] = fi;//������ȡ��һ����������ʼ����
    }
  }
  double sSlant = (stuRef.Len >> 2) - stuRef.Dev;//б�����
  double bSlant = (stuRef.Len >> 1) - stuRef.Dev;//б���
  /////////////////////////////////////���ı���/////////////////////////////////////////////////////////////////////////////////////
  if (vertexN > 3)
  {
    for (int i = 0; i < 4; i++)
    {
      g_stu_square.d_len[i] = 0;
    }
    //double _90Len[10]={0};
    int _90Num = 0, tmp90N = 0, pnNum = 0;//����90��
    int tmp90 = 0;//������ܳ�Ϊ90��Ľǣ�����2��С��90��Ľ����
    int tmpPN = 0;
    needReSta = 0;
    staOK = 0;
    /*for(int vertexI = vertexN-1 ; vertexI > vertexN-3 ; vertexI --)
    {
      if (stuRef.SAg<=abs(vertexAng[vertexI]) && abs(vertexAng[vertexI])<=stuRef.BAg)//90��
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
      else if(5 < abs(vertexAng[vertexI]) && abs(vertexAng[vertexI])<stuRef.SAg)//<90��
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
      if (vertexAng[vertexI] > 0)//�������0
      {
        /*int tempI = vertexI;
        vector tempV = vertexV[vertex]
        do
        {
          templen = VectorLen(vertexV[vertexI]);
          if (templen < sSlant)//������С͹��
          {
            templen = VectorLen(vertexV[vertexI+1]);
            if (templen <sSlant)
            {

            }
          }
          tempI ++;
        }
        while(tempI - vertexI <4)*/
        if (abs(vertexAng[vertexI]) <= 5)// 0��
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
      else if (0 == staOK)//�жϽǶ�
      {
        templen = VectorLen(vertexV[vertexI]);
        if (stuRef.SAg <= abs(vertexAng[vertexI]) && abs(vertexAng[vertexI]) <= stuRef.BAg)//90��
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
        else if (5 < abs(vertexAng[vertexI]) && abs(vertexAng[vertexI]) < stuRef.SAg)//<90��
        {
          pnNum++;
          double tmpTempLen = templen;
          double tmpVectorLen = 0;//б�߳���
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
            tmpVectorLen = VectorLen(vertexV[vertexN - 1]);//б�߳���
            tempAngle = abs(vertexAng[vertexI]) + abs(vertexAng[vertexN - 1]);
            if (tmpVectorLen >= bSlant)
            {
            }
            else if (stuRef.SAg <= tempAngle && tempAngle <= stuRef.BAg)//2��С����ӵ���90
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
          else if (stuRef.SAg <= tempAngle && tempAngle <= stuRef.BAg)//С����ӵ���90
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
        else if (abs(vertexAng[vertexI]) <= 5)// 0��
        {
          if (pnNum > 0)
          {
            pnNum++;
          }
          else
            g_stu_square.d_len[tmp90N] += templen;
        }
        else//>90��
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

              //���浽�������
              g_stu_square.nN++;


              //���浽�ṹ
              bool lenOK = 1;
              //ȥ���ظ�������
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
                  //if(sqrt(pow(double(g_stu_square.pnZPX[i]-xt),2)+pow(double(g_stu_square.pnZPY[i]-yt),2))<(stuRef.Rad)) //���λ����Ƭ�Ĵ����������С��s_sort.allowable_devision����ô����Ҫ���Ե��㲻׼ʱ���õ���
                }
                for (int i = 0; i < 4; i++)
                {
                  int fi = i;
                  int bi = i == 3 ? 0 : i + 1;
                  if (!CalcEquationABC(g_stu_square.pnPX[g_stu_square.nN][fi], g_stu_square.pnPY[g_stu_square.nN][fi], g_stu_square.pnPX[g_stu_square.nN][bi], g_stu_square.pnPY[g_stu_square.nN][bi]
                    , g_stu_square.d_a[fi], g_stu_square.d_b[fi], g_stu_square.d_c[fi]))
                    lenOK = 0;
                }
                //��ͼ���������
                if (1 == lenOK && g_stu_square.debug)
                {


                  for (int j = 1; j < bmpWidth - 2; j++)//����֮һ���ߵ���x
                  {
                    for (int li = 0; li < 4; li++)
                      if (fabs(g_stu_square.d_a[li]) < fabs(g_stu_square.d_b[li]))//����ͼ������ʾ
                        data_bmp_set_color(pBmpBuf, biBitCount, bmpWidth, bmpHeight, 0 == g_stu_square.d_b[li] ? 0 : int(-(g_stu_square.d_a[li] * j + g_stu_square.d_c[li]) / g_stu_square.d_b[li]), 0 == g_stu_square.d_b[li] ? int(-g_stu_square.d_c[li] / g_stu_square.d_a[li]) : j, 255, 255, 0);
                  }

                  for (int j = 1; j < bmpHeight - 2; j++)//����֮һ���ߵ���y
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
        _90Num = 0, tmp90N = 0, pnNum = 0;//����90��
        tmp90 = 0;//������ܳ�Ϊ90��Ľǣ�����2��С��90��Ľ����
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

//����ǰһ��ƽ��ֵ����һ��ƽ��ֵ
void CIMGRecognition::SortAvg(int sum, int value, double& avg)
{
  double n = sum;
  double xi = value;
  //double avg;
  if (1 == n)
  {
    avg = xi;//ƽ��ֵ

  }
  if (n > 1)
  {
    avg = (avg + xi / (n - 1)) / (n / (n - 1));//��ƽ��ֵ
  }

}
//������֮�䳤��
double CIMGRecognition::LenFromP1toP2(double P1X, double P1Y, double P2X, double P2Y)
{
  return (sqrt(pow(double(P1X - P2X), 2) + pow(double(P1Y - P2Y), 2)));
}
//��㵽ֱ�ߵľ���
double CIMGRecognition::LenFromPtoL(double PX, double PY, double a, double b, double c)
{
  return sqrt(pow((a * PX + b * PY + c), 2) / (pow(a, 2) + pow(b, 2)));
}
//��ֱ�ߵķ���ax+by+c=0
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
//����λ��//ֻ��ʹ���ڴ˳�����//ֻ��ʹ�����ı����ұ߰�˳����
bool CIMGRecognition::InOrOut(double PX, double PY, double*& a, double*& b, double*& c)
{
  bool in = 1;
  for (int i = 0; i < 2; i++)
  {
    if (a[i] * b[i] * a[i + 2] * b[i + 2] <= 0)//���⴦��,��2��ֱ��б��һ��С�ڵ���90��һ�����ڵ���90
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
    else if ((a[i] * PX + b[i] * PY + c[i]) * (a[i + 2] * PX + b[i + 2] * PY + c[i + 2]) > 0)//�������
      in = 0;
  }
  return in;
  //return sqrt(pow((a*PX+b*PY+c),2)/(pow(a,2)+pow(b,2)));
}
//�ж��Ƿ��Ǿ���
//����-1û��ʶ��ɹ�����ִ��
//����0���ɹ�
//����1���ǶȲ�����
int CIMGRecognition::IsSquare(double smallAg, double bigAg)
{
  if (!g_stu_square.rcglineok)
    return -1;
  bool yes = 0;
  int nexti;
  for (int i = 0; i < 4; i++)//������4������
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
    //���浽�������(�������꣩
    g_stu_square.pnPX[g_stu_square.nN][i] = (int)g_stu_square.d_point_x[i];
    g_stu_square.pnPY[g_stu_square.nN][i] = (int)g_stu_square.d_point_y[i];
    double angle = abs(atan(-g_stu_square.d_a[nexti] / g_stu_square.d_b[nexti]) - atan(-g_stu_square.d_a[i] / g_stu_square.d_b[i])) / gd_PI * 180;//���������ǣ�������Բ�����
    g_stu_square.d_theta[i] = angle;
    if (angle > bigAg || angle < smallAg)
      yes = 1;
  }
  //���浽������飨�������꣩
  g_stu_square.pnZPX[g_stu_square.nN] = (int(g_stu_square.d_point_x[0] + g_stu_square.d_point_x[1] + g_stu_square.d_point_x[2] + g_stu_square.d_point_x[3]) >> 2);
  g_stu_square.pnZPY[g_stu_square.nN] = (int(g_stu_square.d_point_y[0] + g_stu_square.d_point_y[1] + g_stu_square.d_point_y[2] + g_stu_square.d_point_y[3]) >> 2);
  return yes;
}
//�ж��Ƿ���������
//����-1û��ʶ��ɹ�����ִ��
//����0������
//����1������
int  CIMGRecognition::IsPositiveOrNegative(double a, double b, double c, int beginLineNo, int endLineNo)
{
  if (!g_stu_square.rcglineok)
    return -1;
  double dAngle1 = 0, dAngle2 = 0;
  double min_deg = stuRef.PN_sag, max_deg = stuRef.PN_bag;

  int bPositive = 0;
  //��һ����
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
    if (dAngle1 > 90)//����ǰ�汣֤���Ǿ��Σ������������ǡ�90���
      dAngle1 = 180 - dAngle1;
  }
  //�ڶ�����
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


// �ظ������ŵĽǶȣ�1>2 cw +  1>2 ccw -//����ͼ��������ʵ�����µߵ�����Ҫ�Ӹ���
// �ĳɶ��� cw�ĽǶ� 0 - 360��
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
//-1δʶ��
//debugʶ�������ɿɲ鿴��bmpͼ���Լ�����ʶ��
//PDC��ָ������pDC�����Խ��д��ڱ�ʶ
//
//
//ʶ�������ⲿΨһ���ú���
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
  g_stu_square.nN = 0;//��ʶ������0
  g_stu_square.bIsLap = 0;//�ص���ʾz��0
  int type = -1;//ʶ��������-1
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
  if (bAutoThrd)//���ָ���Զ���ֵ�������Զ���ֵ
  {
    int nTmpThrd = read_image_fz(pBmpBuf, bmpWidth, bmpHeight);//��⵱ǰͼ����м���ֵ
    if (nTmpThrd != -1)
      threshold = nTmpThrd;
  }
  int pos = m_bmp_file.ReverseFind('\\');
  m_bmp_file = m_bmp_file.Left(pos);
  WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, m_bmp_file, -1, chPathMain, sizeof(chPathMain), NULL, NULL);
  //����ͼ��ߴ絽ȫ��
  g_stu_square.nBMPH = bmpHeight;
  g_stu_square.nBMPW = bmpWidth;
  //saveBmp("temps.bmp",pBmpBuf,bmpWidth,bmpHeight,biBitCount,NULL);
  //m_bmp_file.ReleaseBuffer();
  /*
  readPath=m_bmp_file.GetBuffer(m_bmp_file.GetLength());
  readBmp(readPath,pBmpBuf,bmpWidth,bmpHeight);
  m_bmp_file.ReleaseBuffer();*/
  int temp_bmp_data = bmpWidth * bmpHeight;

  xsvalue = new int[temp_bmp_data]; //ȫ0//����Ǳ߽���������飬ֵ�Ƕ�Ӧ��������
  //memset(xsvalue,0,temp_bmp_data);
  fbvalue = new int[temp_bmp_data]; //ȫ0//����Ǳ߽����飬�߽�ֵ�Ƕ�Ӧ��������,��ͼƬԤ���������ã������Ԥ����
  //memset(fbvalue,0,temp_bmp_data);
  jgvalue = new int[temp_bmp_data]; //�׵�-1���ڵ�0//���ڱ߽紦�������
  txvalue = new int[temp_bmp_data]; //�׵�-1���ڵ�0//�����ǰ���Ǳ߽�n���ҹ��ĵ㣬���n//��Ϊ��¼����Ϊ��������ṩ��Ϣ
  int lb_blank = 0;
  //��ֵ������ʶ���Ƭ���Կ��ǲ��жϼ�ֵ��ֱ�Ӷ�ֵ��������readimagefz

  if (1)//xs��fbȫ0��jg��tx��-1����0��
    lb_blank = read_image_mul(pBmpBuf, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, threshold, debug, delNoise);//������ֵ��ֵ��ͼ��ȥ��
  else
    lb_blank = read_image_blank(pBmpBuf, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, threshold);//�жϾ�Ƭ�Ƿ�������
  //debug
  if (debug)
  {
    sprintf(szname, "%s\\read.bmp", chPathMain);
    saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
  }
  //
  if (lb_blank != 0)//�޿�ʶ��������ǰ�˳�RCGBMP
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
    case -1:if (debug && pDC != NULL)pDC->TextOutW(0, 0, _T("��Ƭ")); break;//AfxMessageBox(_T("��Ƭ"));
    case 1:if (debug && pDC != NULL)pDC->TextOutW(0, 0, _T("��Ƭ�����Ѵﵽͼ���Ե,�����������,ֻ���޸Ĵ���:)")); break;
      //AfxMessageBox(_T("��Ƭ�����Ѵﵽͼ���Ե,�����������,ֻ���޸Ĵ���:)"));
    }
    return type;//type=-1
  }

  int tempi, tempj, templ, temp_qyno, temp_qy_num, temp_qy_num2, temp_xsvalue;
  int temp_qy_i, temp_qs_h, temp_qs_l, temp_qy_h, temp_qy_l, temp_fx;//,temp_fxnum;
  int* temp_bj_h;//�б߽�
  int* temp_bj_l;//�б߽�
  int* temp_bj_fx;//����

  //temp_bj_h,temp_bj_l:array of integer;
  // temp_bj_fx:array of integer;
  int temp_bj_num = 0;//debug��δ��0������release�³���release������ֵ
  int temp_total_bj, temp_close;
  int min_h, max_h, min_l, max_l;
  int* temp_bj_z;
  temp_qyno = 1; //������
  temp_qy_num = 1;
  tempi = -10;
  temp_xsvalue = -1; //�������ɫ

  //����߽�������
  temp_total_bj = (bmpWidth + bmpHeight) << 3;  //ÿ���߽���� ע��ָ���ͷ� delete temp_bj_h��̫С�ˣ��治�£���Ƭ����һ��ʱ
  temp_bj_z = new int[temp_total_bj];	//�߽����ظ�������
  temp_bj_h = new int[temp_total_bj];      //�߽���������
  temp_bj_l = new int[temp_total_bj];      //�߽���������
  temp_bj_fx = new int[temp_total_bj];      //�߽���Ѱ�ҷ���
  //��������ͼ��Ѱ�ұ߽���ʼ��
  for (tempj = 1; tempj <= bmpHeight - 2 && g_stu_square.nN < 20; tempj++)     //��ѭ��
  {
    for (templ = 1; templ <= bmpWidth - 2 && g_stu_square.nN < 20; templ++)   //��ѭ����Ѱ�ұ߽���ʼ��
    {
      //jg��������ز���-1��ǰһ�����ز���0������һ��ѭ���������Ѿ���������򣩣���������ҵ���ʼ��
      if (!((*(jgvalue + tempj * bmpWidth + templ) == temp_xsvalue) && (*(jgvalue + tempj * bmpWidth + templ - 1) == 0)))
        continue;
      //mmy
      /*if(!((*(jgvalue+(tempj-1)*bmpWidth+templ)==-1 || *(jgvalue+(tempj+1)*bmpWidth+templ)==-1)
        &&
        (*(jgvalue+(tempj)*bmpWidth+templ-1)==-1 || *(jgvalue+(tempj)*bmpWidth+templ+1)==-1))
        )*/
        //���ܹؼ������ǲ��ܻᵼ�±߽��˳������ʱ��ģ��˳�����ʱ�����ֺܶ����⣬һЩ�߼�Ĭ������˳ʱ��ģ������߽߱������ǲ�ϣ���ĵط�
      if (!(((*(jgvalue + (tempj + 1) * bmpWidth + templ) == -1) && (*(jgvalue + (tempj)* bmpWidth + templ + 1) == -1)) && (*(jgvalue + (tempj - 1) * bmpWidth + templ) == 0)))
        continue;
      //xs��ǵ�ֵ����0����ʾ����ʶ�������Ч����
      if (*(xsvalue + tempj * bmpWidth + templ) > 0)
        continue;
      //��ʼ�����ҵ�����ʼ�����ұ߽�
      temp_close = -1;                          //�߽��Ƿ�պϵı�־��-1��ʾ���պϣ�0�������ݱ�ʾ�պ�λ�ã�����û����
      temp_bj_num = 0;
      //���ص�Ϊ-1�Ľ��д�����¼��ǰλ��
      temp_qs_h = tempj;   //��ǰ�߽���ʼ�м�¼
      temp_qs_l = templ;   //��ǰ�߽���ʼ�м�¼
      *(jgvalue + tempj * bmpWidth + templ) = temp_qy_num;//��jg���ر������ֵ�óɵ�ǰ�����š���ʼΪ1
      //fbvalue[tempj][templ]=temp_qy_num;
      temp_fx = 2;   //��һ�����ķ����ǣ�1�������ϣ�2�������ң�3�������£�4��������
      temp_qy_l = templ;    //��ʼλ���趨
      temp_qy_h = tempj;
      //temp_fxnum=0;  //��ʼִ�з������趨Ϊ0
      // tempi=0;
      *temp_bj_h = tempj;  //��¼�߽���ʼ�����굽����
      *temp_bj_l = templ;  //��¼�߽���ʼ�����굽����
      *temp_bj_fx = temp_fx;//��¼�߽���ʼ����
      temp_bj_num = 0;   //�����߽������0
      *(temp_bj_z + temp_bj_num) = 1;  //��temp_bj_num�������߽����ظ�������1
      min_h = bmpHeight; max_h = 0; min_l = bmpWidth; max_l = 0;
      //���߽���ʼ��
      if (debug && pDC != NULL)pDC->Ellipse(temp_qy_l * rect.right / bmpWidth - 2, temp_qy_h * rect.bottom / bmpHeight - 2, temp_qy_l * rect.right / bmpWidth + 2, temp_qy_h * rect.bottom / bmpHeight + 2);
      //pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));
      temp_qy_num2 = temp_qy_num;//
      //�ұ߽�ѭ����ʼ
      for (temp_qy_i = 1; temp_qy_i < temp_total_bj && g_stu_square.nN < 20; temp_qy_i++)
      {
        //::Sleep(1);
        //�������ó���һ����
        switch (temp_fx) {
        case 1:
          temp_qy_h = temp_qy_h - 1;        //����
          break;
        case 2:
          temp_qy_l = temp_qy_l + 1;        //����
          break;
        case 3:
          temp_qy_h = temp_qy_h + 1;        //����
          break;
        case 4:
          temp_qy_l = temp_qy_l - 1;        //����
          break;
        }
        //����⵽�Ĵ�����߽��
        //pDC->Ellipse(temp_qy_l*rect.right/bmpWidth-2,temp_qy_h*rect.bottom/bmpHeight-2,temp_qy_l*rect.right/bmpWidth+2,temp_qy_h*rect.bottom/bmpHeight+2);
        //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));
        ++temp_bj_num;//�����߽����++
        *(temp_bj_z + temp_bj_num) = 0;//��ǰ����ظ���������0�����µ㣬����⣬��Ӧ�����0
        //mmy
        bool needback = 0;//�˻ش�������0

        if (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) == temp_qy_num)//���jg��ǰ���Ѿ����Ϊ��ǰ����˵���з���������
        {
          //��Ϊ����Ч�㣬������Ϊ��׼�߽�㱣�棬�����ǽ��������Է�������Բ���
          //*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)=temp_qy_num;  //����ǰ����ֵ�޸�Ϊ��ǰ���������Ǳ߽�����//�˴�ò�ƶ���
          *(temp_bj_h + temp_bj_num) = temp_qy_h;  //������
          *(temp_bj_l + temp_bj_num) = temp_qy_l;  //������
          *(temp_bj_fx + temp_bj_num) = temp_fx;
          *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = temp_fx;  //��ǰ����ֵ���򣬱���ı䣬��ֹ��ʼ���Ǳ������ת������������ʱ���򲻷���Ҫ��������

          if ((temp_qy_l == temp_qs_l) && (temp_qy_h == temp_qs_h))//�ж��Ƿ���ʼ����
          {
            //   showmessage('�ص����');
            break;  //�ص���㣬��ǰ������ұ߽�ѭ������
          }
          else
          {
            //���ڷ�գ�����ֱ���˻�
            //int nTmpH = temp_qy_h , nTmpL = temp_qy_l;		//�����˻���ʼ����
            if (debug && pDC != NULL)
              pDC->TextOutW(0, 0, _T("�нӴ�����"));
            //AfxMessageBox(_T("�нӴ�����"));
            int nTmpN = 0;								//�����˻ر߽����
            int* pnTmpH, *pnTmpL, *pnTmpFx;
            pnTmpH = new int[temp_bj_num];						//���������߽߱���������
            pnTmpL = new int[temp_bj_num];						//���������߽߱���������
            pnTmpFx = new int[temp_bj_num];						//����
            temp_qy_num2++;										//����������++��������
            int nMinH = bmpHeight, nMinL = bmpWidth, nMaxH = 0, nMaxL = 0;

            do
            {

              *(pnTmpH + nTmpN) = *(temp_bj_h + temp_bj_num);	//������ 
              *(pnTmpL + nTmpN) = *(temp_bj_l + temp_bj_num);	//������

              if (nMinH > * (temp_bj_h + temp_bj_num)) nMinH = *(temp_bj_h + temp_bj_num);
              if (nMaxH < *(temp_bj_h + temp_bj_num)) nMaxH = *(temp_bj_h + temp_bj_num);
              if (nMinL > * (temp_bj_l + temp_bj_num)) nMinL = *(temp_bj_l + temp_bj_num);
              if (nMaxL < *(temp_bj_l + temp_bj_num)) nMaxL = *(temp_bj_l + temp_bj_num);

              *(jgvalue + *(temp_bj_h + temp_bj_num) * bmpWidth + *(temp_bj_l + temp_bj_num)) = temp_qy_num2;  //����ǰ����ֵ�޸�Ϊbɫ��ͬʱȡ��������
              //*(txvalue+*(temp_bj_h + temp_bj_num)*bmpWidth+*(temp_bj_l + temp_bj_num))=*(temp_bj_fx+temp_bj_num);  //����ǰ����ֵ�޸�Ϊ����ֵ��˵�����ҹ�
              nTmpN++;
              temp_bj_num--;		//�ܵ���-1���ص�
            } while (((temp_qy_h != *(temp_bj_h + temp_bj_num)) || (temp_qy_l != *(temp_bj_l + temp_bj_num))) && temp_bj_num != 0);//�˻�ѭ�� ///��ʱ����temp_bj_num!=0��Ϊ�����temp_bjnum == -1 ����������
            /////////////////////////////////

            //int mj=data_check_fill_bj(pBmpBuf,temp_qy_num2,0,temp_close,pnTmpH,pnTmpL,xsvalue,fbvalue,jgvalue,txvalue,bmpWidth,bmpHeight, 1,bmpHeight-2,1,bmpWidth-2);
            //������򲢷����������
            int mj = data_check_fill_bj(pBmpBuf, temp_qy_num2, 0, temp_close, pnTmpH, pnTmpL, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, nMinH, nMaxH, nMinL, nMaxL, debug);
            if (debug)
            {
              sprintf(szname, "%s\\tmpbj.bmp", chPathMain);
              saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
            }
            /////////////////////////////////
            temp_qy_h = *(temp_bj_h + temp_bj_num);  //������
            temp_qy_l = *(temp_bj_l + temp_bj_num);  //������
            temp_fx = *(temp_bj_fx + temp_bj_num);
            if (nTmpN > ((bmpWidth + bmpHeight) >> 4) && temp_qy_num2 < 10000)//�����������Ҫ����ж�����
            {
              bool bChk = 1;
              if (circle)
              {

                if (g_stu_square.bJudgeFeature)//���Ҫƥ���������������ж�����Ƿ����Ҫ��
                {
                  if (mj > ((stuRef.Rad * stuRef.Rad * gd_PI) * 1.5))//��������1.5���������
                  {
                    g_stu_square.bIsLap = 1;
                    bChk = 0;
                  }
                  if (mj < ((stuRef.Rad * stuRef.Rad * gd_PI) * 0.5))//��������0.5���������
                    bChk = 0;
                }
                if (bChk)
                {
                  for (int n = 0, x = nTmpN - 1; n < x; n++, x--)//ǰ���ǰ��˻ص�˳�������꣬�������µ�����,���ж�����ʱ��Ҫ����
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
                  //����Ļ�ϻ����߽�
                  if (pDC != NULL && type != -1)
                  {
                    for (int n = 0; n < nTmpN; n++)
                      pDC->SetPixel(pnTmpL[n] * rect.right / bmpWidth, pnTmpH[n] * rect.bottom / bmpHeight, RGB(0, 255, 0));
                  }
                  //����Ļ�ϱ�עʶ����
                  if (pDC != NULL && debug)
                    switch (type)
                    {
                    case 0:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("����"));
                      break;
                    case -1:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("δʶ��"));
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
                  for (int n = 0, x = nTmpN - 1; n < x; n++, x--)//ǰ���ǰ��˻ص�˳�������꣬�������µ�����,���ж�����ʱ��Ҫ����
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
                        // AfxMessageBox(_T("����"));
                    type = 0;
                    break;
                  case 1://if(debug)
                         //AfxMessageBox(_T("��״������"));
                    type = -1;
                    break;
                  case 2://if(debug)
                        // AfxMessageBox(_T("�ٿ�"));
                    type = 2;
                    break;
                  case 3://if(debug)
                        // AfxMessageBox(_T("���"));
                    type = 1;
                    break;
                  case 4://if(debug)
                        // AfxMessageBox(_T("�߶�û��ȫ"));
                    type = -1;
                    break;
                  case 5://if(debug)
                      //AfxMessageBox(_T("����������"));
                    type = -1;//ë��Ƭһ��û�нǶ�Ҫ��
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
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("����"));
                      break;
                    case -1:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("δʶ��"));
                      break;
                    case 2:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("�ٿ�"));//AfxMessageBox(_T("�ٿ�"));
                      break;
                    case 1:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("���"));//AfxMessageBox(_T("���"));
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
        //���жϱ����Ƿ�Ϊ0��������˻ز������趨����������Ǽ���
        //mmy�����ж��Ƿ����Ѿ�����������
        if ((temp_qy_h < 1) ||
          (temp_qy_h + 1 >= bmpHeight) ||
          (temp_qy_l < 1) ||
          (temp_qy_l + 1 >= bmpWidth) ||
          (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) != -1) ||//&&*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)!=temp_qy_num)||//�ڶ����ж�ȡ��mmy
          *(xsvalue + temp_qy_h * bmpWidth + temp_qy_l) > 0 || //�Ǳ������
          *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) > -1//�����Ѿ����ҹ�������
          )//||(*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)>0&&*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)<temp_qy_num))     //�µ�һ��Ϊ0���˻���һ�㣬��������+1
        {//���������ߵ�ǰ�ģ�����/�����ر������ֵ����-1��//���ر������ֵ���ǵ�ǰ�����ţ����ѱ��
          needback = 1;
        }
        else if ((*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l + 1)) *
          (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l + 1)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l + 1)) *
          (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l - 1)) *
          (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l - 1)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l - 1)) != 0
          )//���û��ɫ
        {
          if (abs((*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l + 1)) *
            (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l + 1)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l + 1)) *
            (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l - 1)) *
            (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l - 1)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l - 1))
          ) == 1)//ȫ��ɫ
            needback = 1;
        }

        //����ѭ�����˻ش����������Ҫ�˻أ�����ִ��
        //temp_fxnum=0;//��������0������
        /*
        //�ж��Ƿ�ص����Ѿ�����ı߽磬���Ƿ��γ��˱պ�
        temp_close=bj_close(temp_bj_h,temp_bj_l,temp_bj_num,temp_qy_h,temp_qy_l);  //�����Ƿ�պ�
         //�������ҵ��ı߽���������飬�����߽��������ʼ��ǰ�߽��������ʼ����
        //if( (temp_close>=0) && (temp_close<=temp_bj_num-2))//mmyȡ��ע��
          //break;
        //��Ϊ0�����ж��Ƿ�Ϊ�Ѿ�����
        if (*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)==temp_qy_num)//�����ǰ���ǵ�ǰ�����һ����
        {
          if ((temp_qy_l==temp_qs_l) && (temp_qy_h==temp_qs_h) )//�ж��Ƿ���ʼ����
          {
            //   showmessage('�ص����');
            break;  //�ص���㣬��ǰ������ұ߽�ѭ������
          }
        }
        *///mmy
        //����ܱ�8�����Ƿ����㣬���û�м�Ϊ�Ǳ߽磬�˻� (*(jgvalue+(temp_qy_h+1)*bmpWidth+temp_qy_l))
        if (needback)
        {//�˻ش���
          //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,0,0));//mmy

          switch (temp_fx) {
          case 1:
            temp_qy_h = temp_qy_h + 1;       //����
            break;
          case 2:
            temp_qy_l = temp_qy_l - 1;       //����
            break;
          case 3:
            temp_qy_h = temp_qy_h - 1;        //����
            break;
          case 4:
            temp_qy_l = temp_qy_l + 1;        //����
            break;
          }
          //ȥ�����Ǳ߽�ı�ʶ
            //pDC->Ellipse(temp_qy_l*rect.right/bmpWidth-2,temp_qy_h*rect.bottom/bmpHeight-2,temp_qy_l*rect.right/bmpWidth+2,temp_qy_h*rect.bottom/bmpHeight+2);
            //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));//mmy

          --temp_bj_num;//�����߽����--
          if (temp_bj_num < 0)
            AfxMessageBox(_T("TEMPBJNUM<0"));
          *(temp_bj_z + temp_bj_num) = *(temp_bj_z + temp_bj_num) + 1;//MMY
          temp_fx = temp_fx + 1;
          if (temp_fx == 5)
            temp_fx = 1;
          ////////////////////mmy����ǰû��//�õ�������3�Σ�ȡ���߽��ǲ��˻ش���
          bool needend = 0;
          while (3 <= *(temp_bj_z + temp_bj_num))//��temp_bj_num�������߽����ظ�������Ϊ3��
          {
            *(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) = -1;  //����ǰ����ֵ�޸�bɫ��ͬʱȡ�������ǣ����ܸĺڣ��ᵼ��jg����ı�������һ��ѭ�����иõ�
            *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = 0;  //����ǰ����ֵ������0����ʾ���������
            //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,0,0));//mmy

            if (0 == temp_bj_num) {//��������߽����ֻ��0�����˳��ұ߽�ѭ��
              needend = 1;
              break;
            }
            switch (temp_fx) {
            case 1:
              temp_qy_h = temp_qy_h - 1;        //����
              break;
            case 2:
              temp_qy_l = temp_qy_l + 1;        //����
              break;
            case 3:
              temp_qy_h = temp_qy_h + 1;        //����
              break;
            case 4:
              temp_qy_l = temp_qy_l - 1;        //����
              break;
            }
            //pDC->Ellipse(temp_qy_l*rect.right/bmpWidth-2,temp_qy_h*rect.bottom/bmpHeight-2,temp_qy_l*rect.right/bmpWidth+2,temp_qy_h*rect.bottom/bmpHeight+2);
      //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));//
            --temp_bj_num;//�����߽����--
            if (temp_bj_num < 0)
              AfxMessageBox(_T("TEMPBJNUM<0"));
            *(temp_bj_z + temp_bj_num) = *(temp_bj_z + temp_bj_num) + 1;//MMY
            temp_fx = (temp_fx + 3);//mmy��2��Ϊ3
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
        {   //�����˻ش���������� ,�Ǳ߽�
          //temp_bj_num=temp_bj_num+1;//�����߽����++//������needbackһЩ���ݺ����ͼ���ܻ��˳ɹ�������tempbjnumÿ�ζ�����һ��
        //if(!debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));//
          *(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) = temp_qy_num;  //����ǰ����ֵ�޸�Ϊ��ǰ���������Ǳ߽�����
          tempi = tempi + 1;//���⣿������������
          *(temp_bj_h + temp_bj_num) = temp_qy_h;  //������
          *(temp_bj_l + temp_bj_num) = temp_qy_l;  //������
          *(temp_bj_fx + temp_bj_num) = temp_fx;
          *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = temp_fx;  //��ǰ����ֵ����
          if (min_h > temp_qy_h) min_h = temp_qy_h;
          if (max_h < temp_qy_h) max_h = temp_qy_h;
          if (min_l > temp_qy_l) min_l = temp_qy_l;
          if (max_l < temp_qy_l) max_l = temp_qy_l;
          temp_fx = (temp_fx - 1);
          if (temp_fx == 0)
            temp_fx = 4;  //ѭ��
        }
      }//�ұ߽�ѭ��2����

      if (temp_bj_num == 0 || g_stu_square.nN >= 20) {//��������߽����ֻ��0���˳���
        *(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) = -1;//����ǰ����ֵ�޸�Ϊbɫ��ͬʱȡ��������
        *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = 0;  //����ǰ����ֵ������0
        //if(debug && pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,0,0));//mmy

        //continue;//������һ����ѭ��//��֪��Ҫ��Ҫcontinue
      }
      else
      {
        //data_check_fill_bj(pBmpBuf,temp_qy_num,0,temp_close,temp_bj_h,temp_bj_l,xsvalue,fbvalue,jgvalue,txvalue,bmpWidth,bmpHeight, min_h,max_h,min_l,max_l);
        //xsvalue,fbvalue,jgvalue,txvalueֻ��jgvalue�иģ�tempqynum�ĳ�ʼֵ��1
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
            if (g_stu_square.bJudgeFeature)//���Ҫƥ���������������ж�����Ƿ����Ҫ��
            {
              if (mj > ((stuRef.Rad * stuRef.Rad * gd_PI) * 1.5))//��������1.5���������
              {
                g_stu_square.bIsLap = 1;
                bChk = 0;
              }
              if (mj < ((stuRef.Rad * stuRef.Rad * gd_PI) * 0.5))//��������0.5���������
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
                case 0:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("����"));
                  break;
                case -1:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("δʶ��"));
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
                    // AfxMessageBox(_T("����"));
                type = 0;
                break;
              case 1://if(debug)
                    //AfxMessageBox(_T("��״������"));
                type = -1;
                break;
              case 2://if(debug)
                    // AfxMessageBox(_T("�ٿ�"));
                type = 2;
                break;
              case 3://if(debug)
                    // AfxMessageBox(_T("���"));
                type = 1;
                break;
              case 4://if(debug)
                    // AfxMessageBox(_T("�߶�û��ȫ"));
                type = -1;
                break;
              case 5://if(debug)
                  //AfxMessageBox(_T("����������"));
                type = -1;//ë��Ƭһ��û�нǶ�Ҫ��
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
                case 0:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("����"));
                  break;
                case -1:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("δʶ��"));
                  break;
                case 2:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("�ٿ�"));//AfxMessageBox(_T("�ٿ�"));
                  break;
                case 1:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("���"));//AfxMessageBox(_T("���"));
                  break;
                default:break;
                }
            }

          }
          //int zc=temp_bj_num;//��ͼ����˵����һ���߽������==�ܳ����ܳ�Ҫ������߳�֮����������
          if (debug) {
            sprintf(szname, "%s\\result%d.bmp", chPathMain, temp_qy_num);
            saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
          }
          //if(!debug)
          //	break;
        }
      }
      //g_tmp_path=_T(".\\PIC\\temp1.bmp");
      temp_qy_num = temp_qy_num2 + 1;//һ����������ɣ�������++��
      temp_qy_num2 = temp_qy_num;
      //int i_count_mouse=temp_qy_num;
      //UpdateData(0);
      if (!debug)
        if (temp_qy_num > 10000)
          break;
    }   //��ѭ������
    //
    {
      /*if(temp_bj_num>(bmpWidth+bmpHeight)*2/3)
        break;*/

    }
    if (!debug)
      if (temp_qy_num > 10000)
        break;
  }  //��ѭ������
  //saveBmp("temps.bmp",pBmpBuf,bmpWidth,bmpHeight,biBitCount,pColorTable1);
  if (debug && pDC != NULL)
  {
    pDC->TextOutW(0, rect.bottom - 15, _T("��������"));//AfxMessageBox(_T("ʶ�����"));
  }
  delete[] temp_bj_h; temp_bj_h = NULL;
  delete[] temp_bj_l; temp_bj_l = NULL;
  delete[] temp_bj_fx; temp_bj_fx = NULL;
  delete[] temp_bj_z; temp_bj_z = NULL;


  //pDC->SetPixel(int(100*rect.right/bmpWidth),int(100*rect.bottom/bmpHeight),RGB(255,0,0));//�㻭

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
//��RCGBMPһ�������޸�ʱͬ���޸ģ�ֻ�ڵ���Բ����������ʶ��ʱ��һ��
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
  g_stu_square.nN = 0;//��ʶ������0
  g_stu_square.bIsLap = 0;//�ص���ʾz��0
  int type = -1;//ʶ��������-1
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
  if (bAutoThrd)//���ָ���Զ���ֵ�������Զ���ֵ
  {
    int nTmpThrd = read_image_fz(pBmpBuf, bmpWidth, bmpHeight);//��⵱ǰͼ����м���ֵ
    if (nTmpThrd != -1)
      threshold = nTmpThrd;
  }
  int pos = m_bmp_file.ReverseFind('\\');
  m_bmp_file = m_bmp_file.Left(pos);
  WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, m_bmp_file, -1, chPathMain, sizeof(chPathMain), NULL, NULL);
  //����ͼ��ߴ絽ȫ��
  g_stu_square.nBMPH = bmpHeight;
  g_stu_square.nBMPW = bmpWidth;
  //saveBmp("temps.bmp",pBmpBuf,bmpWidth,bmpHeight,biBitCount,NULL);
  //m_bmp_file.ReleaseBuffer();
  /*
  readPath=m_bmp_file.GetBuffer(m_bmp_file.GetLength());
  readBmp(readPath,pBmpBuf,bmpWidth,bmpHeight);
  m_bmp_file.ReleaseBuffer();*/
  int temp_bmp_data = bmpWidth * bmpHeight;

  xsvalue = new int[temp_bmp_data]; //ȫ0//����Ǳ߽���������飬ֵ�Ƕ�Ӧ��������
  //memset(xsvalue,0,temp_bmp_data);
  fbvalue = new int[temp_bmp_data]; //ȫ0//����Ǳ߽����飬�߽�ֵ�Ƕ�Ӧ��������,��ͼƬԤ���������ã������Ԥ����
  //memset(fbvalue,0,temp_bmp_data);
  jgvalue = new int[temp_bmp_data]; //�׵�-1���ڵ�0//���ڱ߽紦�������
  txvalue = new int[temp_bmp_data]; //�׵�-1���ڵ�0//�����ǰ���Ǳ߽�n���ҹ��ĵ㣬���n//��Ϊ��¼����Ϊ��������ṩ��Ϣ
  int lb_blank = 0;
  //��ֵ������ʶ���Ƭ���Կ��ǲ��жϼ�ֵ��ֱ�Ӷ�ֵ��������readimagefz

  if (1)//xs��fbȫ0��jg��tx��-1����0��
    lb_blank = read_image_mul(pBmpBuf, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, threshold, debug, delNoise);//������ֵ��ֵ��ͼ��ȥ��
  else
    lb_blank = read_image_blank(pBmpBuf, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, threshold);//�жϾ�Ƭ�Ƿ�������
  //debug
  if (debug)
  {
    sprintf(szname, "%s\\read.bmp", chPathMain);
    saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
  }
  //
  if (lb_blank != 0)//�޿�ʶ��������ǰ�˳�RCGBMP
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
    case -1:if (debug && pDC != NULL)pDC->TextOutW(0, 0, _T("��Ƭ")); break;//AfxMessageBox(_T("��Ƭ"));
    case 1:if (debug && pDC != NULL)pDC->TextOutW(0, 0, _T("��Ƭ�����Ѵﵽͼ���Ե,�����������,ֻ���޸Ĵ���:)")); break;
      //AfxMessageBox(_T("��Ƭ�����Ѵﵽͼ���Ե,�����������,ֻ���޸Ĵ���:)"));
    }
    return type;//type=-1
  }

  int tempi, tempj, templ, temp_qyno, temp_qy_num, temp_qy_num2, temp_xsvalue;
  int temp_qy_i, temp_qs_h, temp_qs_l, temp_qy_h, temp_qy_l, temp_fx;//,temp_fxnum;
  int* temp_bj_h;//�б߽�
  int* temp_bj_l;//�б߽�
  int* temp_bj_fx;//����

  //temp_bj_h,temp_bj_l:array of integer;
  // temp_bj_fx:array of integer;
  int temp_bj_num = 0;//debug��δ��0������release�³���release������ֵ
  int temp_total_bj, temp_close;
  int min_h, max_h, min_l, max_l;
  int* temp_bj_z;
  temp_qyno = 1; //������
  temp_qy_num = 1;
  tempi = -10;
  temp_xsvalue = -1; //�������ɫ

  //����߽�������
  temp_total_bj = (bmpWidth + bmpHeight) << 3;  //ÿ���߽���� ע��ָ���ͷ� delete temp_bj_h��̫С�ˣ��治�£���Ƭ����һ��ʱ
  temp_bj_z = new int[temp_total_bj];	//�߽����ظ�������
  temp_bj_h = new int[temp_total_bj];      //�߽���������
  temp_bj_l = new int[temp_total_bj];      //�߽���������
  temp_bj_fx = new int[temp_total_bj];      //�߽���Ѱ�ҷ���
  //��������ͼ��Ѱ�ұ߽���ʼ��
  for (tempj = 1; tempj <= bmpHeight - 2 && g_stu_square.nN < 20; tempj++)     //��ѭ��
  {
    for (templ = 1; templ <= bmpWidth - 2 && g_stu_square.nN < 20; templ++)   //��ѭ����Ѱ�ұ߽���ʼ��
    {
      //jg��������ز���-1��ǰһ�����ز���0������һ��ѭ���������Ѿ���������򣩣���������ҵ���ʼ��
      if (!((*(jgvalue + tempj * bmpWidth + templ) == temp_xsvalue) && (*(jgvalue + tempj * bmpWidth + templ - 1) == 0)))
        continue;
      //mmy
      /*if(!((*(jgvalue+(tempj-1)*bmpWidth+templ)==-1 || *(jgvalue+(tempj+1)*bmpWidth+templ)==-1)
        &&
        (*(jgvalue+(tempj)*bmpWidth+templ-1)==-1 || *(jgvalue+(tempj)*bmpWidth+templ+1)==-1))
        )*/
        //���ܹؼ������ǲ��ܻᵼ�±߽��˳������ʱ��ģ��˳�����ʱ�����ֺܶ����⣬һЩ�߼�Ĭ������˳ʱ��ģ������߽߱������ǲ�ϣ���ĵط�
      if (!(((*(jgvalue + (tempj + 1) * bmpWidth + templ) == -1) && (*(jgvalue + (tempj)* bmpWidth + templ + 1) == -1)) && (*(jgvalue + (tempj - 1) * bmpWidth + templ) == 0)))
        continue;
      //xs��ǵ�ֵ����0����ʾ����ʶ�������Ч����
      if (*(xsvalue + tempj * bmpWidth + templ) > 0)
        continue;
      //��ʼ�����ҵ�����ʼ�����ұ߽�
      temp_close = -1;                          //�߽��Ƿ�պϵı�־��-1��ʾ���պϣ�0�������ݱ�ʾ�պ�λ�ã�����û����
      temp_bj_num = 0;
      //���ص�Ϊ-1�Ľ��д�����¼��ǰλ��
      temp_qs_h = tempj;   //��ǰ�߽���ʼ�м�¼
      temp_qs_l = templ;   //��ǰ�߽���ʼ�м�¼
      *(jgvalue + tempj * bmpWidth + templ) = temp_qy_num;//��jg���ر������ֵ�óɵ�ǰ�����š���ʼΪ1
      //fbvalue[tempj][templ]=temp_qy_num;
      temp_fx = 2;   //��һ�����ķ����ǣ�1�������ϣ�2�������ң�3�������£�4��������
      temp_qy_l = templ;    //��ʼλ���趨
      temp_qy_h = tempj;
      //temp_fxnum=0;  //��ʼִ�з������趨Ϊ0
      // tempi=0;
      *temp_bj_h = tempj;  //��¼�߽���ʼ�����굽����
      *temp_bj_l = templ;  //��¼�߽���ʼ�����굽����
      *temp_bj_fx = temp_fx;//��¼�߽���ʼ����
      temp_bj_num = 0;   //�����߽������0
      *(temp_bj_z + temp_bj_num) = 1;  //��temp_bj_num�������߽����ظ�������1
      min_h = bmpHeight; max_h = 0; min_l = bmpWidth; max_l = 0;
      //���߽���ʼ��
      if (debug && pDC != NULL)pDC->Ellipse(temp_qy_l * rect.right / bmpWidth - 2, temp_qy_h * rect.bottom / bmpHeight - 2, temp_qy_l * rect.right / bmpWidth + 2, temp_qy_h * rect.bottom / bmpHeight + 2);
      //pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));
      temp_qy_num2 = temp_qy_num;//
      //�ұ߽�ѭ����ʼ
      for (temp_qy_i = 1; temp_qy_i < temp_total_bj && g_stu_square.nN < 20; temp_qy_i++)
      {
        //::Sleep(1);
        //�������ó���һ����
        switch (temp_fx) {
        case 1:
          temp_qy_h = temp_qy_h - 1;        //����
          break;
        case 2:
          temp_qy_l = temp_qy_l + 1;        //����
          break;
        case 3:
          temp_qy_h = temp_qy_h + 1;        //����
          break;
        case 4:
          temp_qy_l = temp_qy_l - 1;        //����
          break;
        }
        //����⵽�Ĵ�����߽��
        //pDC->Ellipse(temp_qy_l*rect.right/bmpWidth-2,temp_qy_h*rect.bottom/bmpHeight-2,temp_qy_l*rect.right/bmpWidth+2,temp_qy_h*rect.bottom/bmpHeight+2);
        //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));
        ++temp_bj_num;//�����߽����++
        *(temp_bj_z + temp_bj_num) = 0;//��ǰ����ظ���������0�����µ㣬����⣬��Ӧ�����0
        //mmy
        bool needback = 0;//�˻ش�������0

        if (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) == temp_qy_num)//���jg��ǰ���Ѿ����Ϊ��ǰ����˵���з���������
        {
          //��Ϊ����Ч�㣬������Ϊ��׼�߽�㱣�棬�����ǽ��������Է�������Բ���
          //*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)=temp_qy_num;  //����ǰ����ֵ�޸�Ϊ��ǰ���������Ǳ߽�����//�˴�ò�ƶ���
          *(temp_bj_h + temp_bj_num) = temp_qy_h;  //������
          *(temp_bj_l + temp_bj_num) = temp_qy_l;  //������
          *(temp_bj_fx + temp_bj_num) = temp_fx;
          *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = temp_fx;  //��ǰ����ֵ���򣬱���ı䣬��ֹ��ʼ���Ǳ������ת������������ʱ���򲻷���Ҫ��������

          if ((temp_qy_l == temp_qs_l) && (temp_qy_h == temp_qs_h))//�ж��Ƿ���ʼ����
          {
            //   showmessage('�ص����');
            break;  //�ص���㣬��ǰ������ұ߽�ѭ������
          }
          else
          {
            //���ڷ�գ�����ֱ���˻�
            //int nTmpH = temp_qy_h , nTmpL = temp_qy_l;		//�����˻���ʼ����
            if (debug && pDC != NULL)
              pDC->TextOutW(0, 0, _T("�нӴ�����"));
            //AfxMessageBox(_T("�нӴ�����"));
            int nTmpN = 0;								//�����˻ر߽����
            int* pnTmpH, *pnTmpL, *pnTmpFx;
            pnTmpH = new int[temp_bj_num];						//���������߽߱���������
            pnTmpL = new int[temp_bj_num];						//���������߽߱���������
            pnTmpFx = new int[temp_bj_num];						//����
            temp_qy_num2++;										//����������++��������
            int nMinH = bmpHeight, nMinL = bmpWidth, nMaxH = 0, nMaxL = 0;

            do
            {

              *(pnTmpH + nTmpN) = *(temp_bj_h + temp_bj_num);	//������ 
              *(pnTmpL + nTmpN) = *(temp_bj_l + temp_bj_num);	//������
              *(pnTmpFx + nTmpN) = *(temp_bj_fx + temp_bj_num);	//������
              if (nMinH > * (temp_bj_h + temp_bj_num)) nMinH = *(temp_bj_h + temp_bj_num);
              if (nMaxH < *(temp_bj_h + temp_bj_num)) nMaxH = *(temp_bj_h + temp_bj_num);
              if (nMinL > * (temp_bj_l + temp_bj_num)) nMinL = *(temp_bj_l + temp_bj_num);
              if (nMaxL < *(temp_bj_l + temp_bj_num)) nMaxL = *(temp_bj_l + temp_bj_num);

              *(jgvalue + *(temp_bj_h + temp_bj_num) * bmpWidth + *(temp_bj_l + temp_bj_num)) = temp_qy_num2;  //����ǰ����ֵ�޸�Ϊbɫ��ͬʱȡ��������
              //*(txvalue+*(temp_bj_h + temp_bj_num)*bmpWidth+*(temp_bj_l + temp_bj_num))=*(temp_bj_fx+temp_bj_num);  //����ǰ����ֵ�޸�Ϊ����ֵ��˵�����ҹ�
              nTmpN++;
              temp_bj_num--;		//�ܵ���-1���ص�
            } while (((temp_qy_h != *(temp_bj_h + temp_bj_num)) || (temp_qy_l != *(temp_bj_l + temp_bj_num))) && temp_bj_num != 0);//�˻�ѭ�� ///��ʱ����temp_bj_num!=0��Ϊ�����temp_bjnum == -1 ����������
            /////////////////////////////////

            //int mj=data_check_fill_bj(pBmpBuf,temp_qy_num2,0,temp_close,pnTmpH,pnTmpL,xsvalue,fbvalue,jgvalue,txvalue,bmpWidth,bmpHeight, 1,bmpHeight-2,1,bmpWidth-2);
            //������򲢷����������
            int mj = data_check_fill_bj(pBmpBuf, temp_qy_num2, 0, temp_close, pnTmpH, pnTmpL, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, nMinH, nMaxH, nMinL, nMaxL, debug);
            if (debug)
            {
              sprintf(szname, "%s\\tmpbj.bmp", chPathMain);
              saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
            }
            /////////////////////////////////
            temp_qy_h = *(temp_bj_h + temp_bj_num);  //������
            temp_qy_l = *(temp_bj_l + temp_bj_num);  //������
            temp_fx = *(temp_bj_fx + temp_bj_num);
            if (nTmpN > ((bmpWidth + bmpHeight) >> 4) && temp_qy_num2 < 10000)//�����������Ҫ����ж�����
            {
              bool bChk = 1;
              if (circle)
              {

                if (g_stu_square.bJudgeFeature)//���Ҫƥ���������������ж�����Ƿ����Ҫ��
                {
                  if (mj > ((stuRef.Rad * stuRef.Rad * gd_PI) * 1.5))//��������1.5���������
                  {
                    g_stu_square.bIsLap = 1;
                    bChk = 0;
                  }
                  if (mj < ((stuRef.Rad * stuRef.Rad * gd_PI) * 0.5))//��������0.5���������
                    bChk = 0;
                }
                if (bChk)
                {
                  for (int n = 0, x = nTmpN - 1; n < x; n++, x--)//ǰ���ǰ��˻ص�˳�������꣬�������µ�����,���ж�����ʱ��Ҫ����
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
                  //����Ļ�ϻ����߽�
                /*	if(pDC!=NULL && type!=-1)
                  {
                    for(int n = 0; n<nTmpN; n++)
                      pDC->SetPixel(pnTmpL[n]*rect.right/bmpWidth,pnTmpH[n]*rect.bottom/bmpHeight,RGB(0,255,0));
                  }
                  //����Ļ�ϱ�עʶ����
                  if(pDC!=NULL &&debug)
                  switch(type)
                  {
                  case 0:
                    pDC->TextOutW(pnTmpL[0]*rect.right/bmpWidth,pnTmpH[0]*rect.bottom/bmpHeight,_T("����"));
                    break;
                  case -1:
                    pDC->TextOutW(pnTmpL[0]*rect.right/bmpWidth,pnTmpH[0]*rect.bottom/bmpHeight,_T("δʶ��"));
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
                  for (int n = 0, x = nTmpN - 1; n < x; n++, x--)//ǰ���ǰ��˻ص�˳�������꣬�������µ�����,���ж�����ʱ��Ҫ����
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
                        // AfxMessageBox(_T("����"));
                    type = 0;
                    break;
                  case 1://if(debug)
                         //AfxMessageBox(_T("��״������"));
                    type = -1;
                    break;
                  case 2://if(debug)
                        // AfxMessageBox(_T("�ٿ�"));
                    type = 2;
                    break;
                  case 3://if(debug)
                        // AfxMessageBox(_T("���"));
                    type = 1;
                    break;
                  case 4://if(debug)
                        // AfxMessageBox(_T("�߶�û��ȫ"));
                    type = -1;
                    break;
                  case 5://if(debug)
                      //AfxMessageBox(_T("����������"));
                    type = -1;//ë��Ƭһ��û�нǶ�Ҫ��
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
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("����"));
                      break;
                    case -1:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("δʶ��"));
                      break;
                    case 2:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("�ٿ�"));//AfxMessageBox(_T("�ٿ�"));
                      break;
                    case 1:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("���"));//AfxMessageBox(_T("���"));
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
        //���жϱ����Ƿ�Ϊ0��������˻ز������趨����������Ǽ���
        //mmy�����ж��Ƿ����Ѿ�����������
        if ((temp_qy_h < 1) ||
          (temp_qy_h + 1 >= bmpHeight) ||
          (temp_qy_l < 1) ||
          (temp_qy_l + 1 >= bmpWidth) ||
          (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) != -1) ||//&&*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)!=temp_qy_num)||//�ڶ����ж�ȡ��mmy
          *(xsvalue + temp_qy_h * bmpWidth + temp_qy_l) > 0 || //�Ǳ������
          *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) > -1//�����Ѿ����ҹ�������
          )//||(*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)>0&&*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)<temp_qy_num))     //�µ�һ��Ϊ0���˻���һ�㣬��������+1
        {//���������ߵ�ǰ�ģ�����/�����ر������ֵ����-1��//���ر������ֵ���ǵ�ǰ�����ţ����ѱ��
          needback = 1;
        }
        else if ((*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l + 1)) *
          (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l + 1)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l + 1)) *
          (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l - 1)) *
          (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l - 1)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l - 1)) != 0
          )//���û��ɫ
        {
          if (abs((*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l + 1)) *
            (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l + 1)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l + 1)) *
            (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l - 1)) *
            (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l - 1)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l - 1))
          ) == 1)//ȫ��ɫ
            needback = 1;
        }

        //����ѭ�����˻ش����������Ҫ�˻أ�����ִ��
        //temp_fxnum=0;//��������0������
        /*
        //�ж��Ƿ�ص����Ѿ�����ı߽磬���Ƿ��γ��˱պ�
        temp_close=bj_close(temp_bj_h,temp_bj_l,temp_bj_num,temp_qy_h,temp_qy_l);  //�����Ƿ�պ�
         //�������ҵ��ı߽���������飬�����߽��������ʼ��ǰ�߽��������ʼ����
        //if( (temp_close>=0) && (temp_close<=temp_bj_num-2))//mmyȡ��ע��
          //break;
        //��Ϊ0�����ж��Ƿ�Ϊ�Ѿ�����
        if (*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)==temp_qy_num)//�����ǰ���ǵ�ǰ�����һ����
        {
          if ((temp_qy_l==temp_qs_l) && (temp_qy_h==temp_qs_h) )//�ж��Ƿ���ʼ����
          {
            //   showmessage('�ص����');
            break;  //�ص���㣬��ǰ������ұ߽�ѭ������
          }
        }
        *///mmy
        //����ܱ�8�����Ƿ����㣬���û�м�Ϊ�Ǳ߽磬�˻� (*(jgvalue+(temp_qy_h+1)*bmpWidth+temp_qy_l))
        if (needback)
        {//�˻ش���
          //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,0,0));//mmy

          switch (temp_fx) {
          case 1:
            temp_qy_h = temp_qy_h + 1;       //����
            break;
          case 2:
            temp_qy_l = temp_qy_l - 1;       //����
            break;
          case 3:
            temp_qy_h = temp_qy_h - 1;        //����
            break;
          case 4:
            temp_qy_l = temp_qy_l + 1;        //����
            break;
          }
          //ȥ�����Ǳ߽�ı�ʶ
            //pDC->Ellipse(temp_qy_l*rect.right/bmpWidth-2,temp_qy_h*rect.bottom/bmpHeight-2,temp_qy_l*rect.right/bmpWidth+2,temp_qy_h*rect.bottom/bmpHeight+2);
            //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));//mmy

          --temp_bj_num;//�����߽����--
          if (temp_bj_num < 0)
            AfxMessageBox(_T("TEMPBJNUM<0"));
          *(temp_bj_z + temp_bj_num) = *(temp_bj_z + temp_bj_num) + 1;//MMY
          temp_fx = temp_fx + 1;
          if (temp_fx == 5)
            temp_fx = 1;
          ////////////////////mmy����ǰû��//�õ�������3�Σ�ȡ���߽��ǲ��˻ش���
          while (3 <= *(temp_bj_z + temp_bj_num))//��temp_bj_num�������߽����ظ�������Ϊ3��
          {
            *(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) = -1;  //����ǰ����ֵ�޸�bɫ��ͬʱȡ�������ǣ����ܸĺڣ��ᵼ��jg����ı�������һ��ѭ�����иõ�
            *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = 0;  //����ǰ����ֵ������0����ʾ���������
            //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,0,0));//mmy

            if (0 == temp_bj_num) {//��������߽����ֻ��0�����˳��ұ߽�ѭ��
              break;
            }
            switch (temp_fx) {
            case 1:
              temp_qy_h = temp_qy_h - 1;        //����
              break;
            case 2:
              temp_qy_l = temp_qy_l + 1;        //����
              break;
            case 3:
              temp_qy_h = temp_qy_h + 1;        //����
              break;
            case 4:
              temp_qy_l = temp_qy_l - 1;        //����
              break;
            }
            //pDC->Ellipse(temp_qy_l*rect.right/bmpWidth-2,temp_qy_h*rect.bottom/bmpHeight-2,temp_qy_l*rect.right/bmpWidth+2,temp_qy_h*rect.bottom/bmpHeight+2);
      //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));//
            --temp_bj_num;//�����߽����--
            if (temp_bj_num < 0)
              AfxMessageBox(_T("TEMPBJNUM<0"));
            *(temp_bj_z + temp_bj_num) = *(temp_bj_z + temp_bj_num) + 1;//MMY
            temp_fx = (temp_fx + 3);//mmy��2��Ϊ3
            if (temp_fx >= 5)	temp_fx = temp_fx - 4;
          }
          ////////////////////
          continue;
        }
        else
        {   //�����˻ش���������� ,�Ǳ߽�
          //temp_bj_num=temp_bj_num+1;//�����߽����++//������needbackһЩ���ݺ����ͼ���ܻ��˳ɹ�������tempbjnumÿ�ζ�����һ��
        //if(!debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));//
          *(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) = temp_qy_num;  //����ǰ����ֵ�޸�Ϊ��ǰ���������Ǳ߽�����
          tempi = tempi + 1;//���⣿������������
          *(temp_bj_h + temp_bj_num) = temp_qy_h;  //������
          *(temp_bj_l + temp_bj_num) = temp_qy_l;  //������
          *(temp_bj_fx + temp_bj_num) = temp_fx;
          *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = temp_fx;  //��ǰ����ֵ����
          if (min_h > temp_qy_h) min_h = temp_qy_h;
          if (max_h < temp_qy_h) max_h = temp_qy_h;
          if (min_l > temp_qy_l) min_l = temp_qy_l;
          if (max_l < temp_qy_l) max_l = temp_qy_l;
          temp_fx = (temp_fx - 1);
          if (temp_fx == 0)
            temp_fx = 4;  //ѭ��
        }
      }//�ұ߽�ѭ��2����

      if (temp_bj_num == 0 || g_stu_square.nN >= 20) {//��������߽����ֻ��0���˳���
        *(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) = -1;//����ǰ����ֵ�޸�Ϊbɫ��ͬʱȡ��������
        *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = 0;  //����ǰ����ֵ������0
        //if(debug && pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,0,0));//mmy

        //continue;//������һ����ѭ��//��֪��Ҫ��Ҫcontinue
      }
      else
      {
        //data_check_fill_bj(pBmpBuf,temp_qy_num,0,temp_close,temp_bj_h,temp_bj_l,xsvalue,fbvalue,jgvalue,txvalue,bmpWidth,bmpHeight, min_h,max_h,min_l,max_l);
        //xsvalue,fbvalue,jgvalue,txvalueֻ��jgvalue�иģ�tempqynum�ĳ�ʼֵ��1
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
            if (g_stu_square.bJudgeFeature)//���Ҫƥ���������������ж�����Ƿ����Ҫ��
            {
              if (mj > ((stuRef.Rad * stuRef.Rad * gd_PI) * 1.5))//��������1.5���������
              {
                g_stu_square.bIsLap = 1;
                bChk = 0;
              }
              if (mj < ((stuRef.Rad * stuRef.Rad * gd_PI) * 0.5))//��������0.5���������
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
              case 0:pDC->TextOutW(temp_bj_l[0]*rect.right/bmpWidth,temp_bj_h[0]*rect.bottom/bmpHeight,_T("����"));
                break;
              case -1:pDC->TextOutW(temp_bj_l[0]*rect.right/bmpWidth,temp_bj_h[0]*rect.bottom/bmpHeight,_T("δʶ��"));
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
                    // AfxMessageBox(_T("����"));
                type = 0;
                break;
              case 1://if(debug)
                    //AfxMessageBox(_T("��״������"));
                type = -1;
                break;
              case 2://if(debug)
                    // AfxMessageBox(_T("�ٿ�"));
                type = 2;
                break;
              case 3://if(debug)
                    // AfxMessageBox(_T("���"));
                type = 1;
                break;
              case 4://if(debug)
                    // AfxMessageBox(_T("�߶�û��ȫ"));
                type = -1;
                break;
              case 5://if(debug)
                  //AfxMessageBox(_T("����������"));
                type = -1;//ë��Ƭһ��û�нǶ�Ҫ��
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
                case 0:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("����"));
                  break;
                case -1:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("δʶ��"));
                  break;
                case 2:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("�ٿ�"));//AfxMessageBox(_T("�ٿ�"));
                  break;
                case 1:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("���"));//AfxMessageBox(_T("���"));
                  break;
                default:break;
                }
            }

          }
          //int zc=temp_bj_num;//��ͼ����˵����һ���߽������==�ܳ����ܳ�Ҫ������߳�֮����������
          if (debug) {
            sprintf(szname, "%s\\result%d.bmp", chPathMain, temp_qy_num);
            saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
          }
          //if(!debug)
          //	break;
        }
      }
      //g_tmp_path=_T(".\\PIC\\temp1.bmp");
      temp_qy_num = temp_qy_num2 + 1;//һ����������ɣ�������++��
      temp_qy_num2 = temp_qy_num;
      //int i_count_mouse=temp_qy_num;
      //UpdateData(0);
      if (!debug)
        if (temp_qy_num > 10000)
          break;
    }   //��ѭ������
    //
    {
      /*if(temp_bj_num>(bmpWidth+bmpHeight)*2/3)
        break;*/

    }
    if (!debug)
      if (temp_qy_num > 10000)
        break;
  }  //��ѭ������
  //saveBmp("temps.bmp",pBmpBuf,bmpWidth,bmpHeight,biBitCount,pColorTable1);
  if (debug && pDC != NULL)
  {
    pDC->TextOutW(0, rect.bottom - 15, _T("��������"));//AfxMessageBox(_T("ʶ�����"));
  }
  //����Ļ�ϻ����߽�
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


  //pDC->SetPixel(int(100*rect.right/bmpWidth),int(100*rect.bottom/bmpHeight),RGB(255,0,0));//�㻭

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
//��RCGBMPSPLITһ����ֻ�ǲ��ǵ����ļ��������������ڴ�
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
  g_stu_square.nN = 0;//��ʶ������0
  g_stu_square.bIsLap = 0;//�ص���ʾz��0
  int type = -1;//ʶ��������-1
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
  if (bAutoThrd)//���ָ���Զ���ֵ�������Զ���ֵ
  {
    int nTmpThrd = read_image_fz(pBmpBuf, bmpWidth, bmpHeight);//��⵱ǰͼ����м���ֵ
    if (nTmpThrd != -1)
      threshold = nTmpThrd;
  }
  int pos = m_bmp_file.ReverseFind('\\');
  m_bmp_file = m_bmp_file.Left(pos);
  WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, m_bmp_file, -1, chPathMain, sizeof(chPathMain), NULL, NULL);
  //����ͼ��ߴ絽ȫ��
  g_stu_square.nBMPH = bmpHeight;
  g_stu_square.nBMPW = bmpWidth;
  //saveBmp("temps.bmp",pBmpBuf,bmpWidth,bmpHeight,biBitCount,NULL);
  //m_bmp_file.ReleaseBuffer();
  /*
  readPath=m_bmp_file.GetBuffer(m_bmp_file.GetLength());
  readBmp(readPath,pBmpBuf,bmpWidth,bmpHeight);
  m_bmp_file.ReleaseBuffer();*/
  int temp_bmp_data = bmpWidth * bmpHeight;

  xsvalue = new int[temp_bmp_data]; //ȫ0//����Ǳ߽���������飬ֵ�Ƕ�Ӧ��������
  //memset(xsvalue,0,temp_bmp_data);
  fbvalue = new int[temp_bmp_data]; //ȫ0//����Ǳ߽����飬�߽�ֵ�Ƕ�Ӧ��������,��ͼƬԤ���������ã������Ԥ����
  //memset(fbvalue,0,temp_bmp_data);
  jgvalue = new int[temp_bmp_data]; //�׵�-1���ڵ�0//���ڱ߽紦�������
  txvalue = new int[temp_bmp_data]; //�׵�-1���ڵ�0//�����ǰ���Ǳ߽�n���ҹ��ĵ㣬���n//��Ϊ��¼����Ϊ��������ṩ��Ϣ
  int lb_blank = 0;
  //��ֵ������ʶ���Ƭ���Կ��ǲ��жϼ�ֵ��ֱ�Ӷ�ֵ��������readimagefz

  if (1)//xs��fbȫ0��jg��tx��-1����0��
    lb_blank = read_image_mul(pBmpBuf, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, threshold, debug, delNoise);//������ֵ��ֵ��ͼ��ȥ��
  else
    lb_blank = read_image_blank(pBmpBuf, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, threshold);//�жϾ�Ƭ�Ƿ�������
  //debug
  if (debug)
  {
    sprintf(szname, "%s\\read.bmp", chPathMain);
    saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
  }
  //
  if (lb_blank != 0)//�޿�ʶ��������ǰ�˳�RCGBMP
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
    case -1:if (debug && pDC != NULL)pDC->TextOutW(0, 0, _T("��Ƭ")); break;//AfxMessageBox(_T("��Ƭ"));
    case 1:if (debug && pDC != NULL)pDC->TextOutW(0, 0, _T("��Ƭ�����Ѵﵽͼ���Ե,�����������,ֻ���޸Ĵ���:)")); break;
      //AfxMessageBox(_T("��Ƭ�����Ѵﵽͼ���Ե,�����������,ֻ���޸Ĵ���:)"));
    }
    return type;//type=-1
  }

  int tempi, tempj, templ, temp_qyno, temp_qy_num, temp_qy_num2, temp_xsvalue;
  int temp_qy_i, temp_qs_h, temp_qs_l, temp_qy_h, temp_qy_l, temp_fx;//,temp_fxnum;
  int* temp_bj_h;//�б߽�
  int* temp_bj_l;//�б߽�
  int* temp_bj_fx;//����

  //temp_bj_h,temp_bj_l:array of integer;
  // temp_bj_fx:array of integer;
  int temp_bj_num = 0;//debug��δ��0������release�³���release������ֵ
  int temp_total_bj, temp_close;
  int min_h, max_h, min_l, max_l;
  int* temp_bj_z;
  temp_qyno = 1; //������
  temp_qy_num = 1;
  tempi = -10;
  temp_xsvalue = -1; //�������ɫ

  //����߽�������
  temp_total_bj = (bmpWidth + bmpHeight) << 3;  //ÿ���߽���� ע��ָ���ͷ� delete temp_bj_h��̫С�ˣ��治�£���Ƭ����һ��ʱ
  temp_bj_z = new int[temp_total_bj];	//�߽����ظ�������
  temp_bj_h = new int[temp_total_bj];      //�߽���������
  temp_bj_l = new int[temp_total_bj];      //�߽���������
  temp_bj_fx = new int[temp_total_bj];      //�߽���Ѱ�ҷ���
  //��������ͼ��Ѱ�ұ߽���ʼ��
  for (tempj = 1; tempj <= bmpHeight - 2 && g_stu_square.nN < 20; tempj++)     //��ѭ��
  {
    for (templ = 1; templ <= bmpWidth - 2 && g_stu_square.nN < 20; templ++)   //��ѭ����Ѱ�ұ߽���ʼ��
    {
      //jg��������ز���-1��ǰһ�����ز���0������һ��ѭ���������Ѿ���������򣩣���������ҵ���ʼ��
      if (!((*(jgvalue + tempj * bmpWidth + templ) == temp_xsvalue) && (*(jgvalue + tempj * bmpWidth + templ - 1) == 0)))
        continue;
      //mmy
      /*if(!((*(jgvalue+(tempj-1)*bmpWidth+templ)==-1 || *(jgvalue+(tempj+1)*bmpWidth+templ)==-1)
        &&
        (*(jgvalue+(tempj)*bmpWidth+templ-1)==-1 || *(jgvalue+(tempj)*bmpWidth+templ+1)==-1))
        )*/
        //���ܹؼ������ǲ��ܻᵼ�±߽��˳������ʱ��ģ��˳�����ʱ�����ֺܶ����⣬һЩ�߼�Ĭ������˳ʱ��ģ������߽߱������ǲ�ϣ���ĵط�
      if (!(((*(jgvalue + (tempj + 1) * bmpWidth + templ) == -1) && (*(jgvalue + (tempj)* bmpWidth + templ + 1) == -1)) && (*(jgvalue + (tempj - 1) * bmpWidth + templ) == 0)))
        continue;
      //xs��ǵ�ֵ����0����ʾ����ʶ�������Ч����
      if (*(xsvalue + tempj * bmpWidth + templ) > 0)
        continue;
      //��ʼ�����ҵ�����ʼ�����ұ߽�
      temp_close = -1;                          //�߽��Ƿ�պϵı�־��-1��ʾ���պϣ�0�������ݱ�ʾ�պ�λ�ã�����û����
      temp_bj_num = 0;
      //���ص�Ϊ-1�Ľ��д�����¼��ǰλ��
      temp_qs_h = tempj;   //��ǰ�߽���ʼ�м�¼
      temp_qs_l = templ;   //��ǰ�߽���ʼ�м�¼
      *(jgvalue + tempj * bmpWidth + templ) = temp_qy_num;//��jg���ر������ֵ�óɵ�ǰ�����š���ʼΪ1
      //fbvalue[tempj][templ]=temp_qy_num;
      temp_fx = 2;   //��һ�����ķ����ǣ�1�������ϣ�2�������ң�3�������£�4��������
      temp_qy_l = templ;    //��ʼλ���趨
      temp_qy_h = tempj;
      //temp_fxnum=0;  //��ʼִ�з������趨Ϊ0
      // tempi=0;
      *temp_bj_h = tempj;  //��¼�߽���ʼ�����굽����
      *temp_bj_l = templ;  //��¼�߽���ʼ�����굽����
      *temp_bj_fx = temp_fx;//��¼�߽���ʼ����
      temp_bj_num = 0;   //�����߽������0
      *(temp_bj_z + temp_bj_num) = 1;  //��temp_bj_num�������߽����ظ�������1
      min_h = bmpHeight; max_h = 0; min_l = bmpWidth; max_l = 0;
      //���߽���ʼ��
      if (debug && pDC != NULL)pDC->Ellipse(temp_qy_l * rect.right / bmpWidth - 2, temp_qy_h * rect.bottom / bmpHeight - 2, temp_qy_l * rect.right / bmpWidth + 2, temp_qy_h * rect.bottom / bmpHeight + 2);
      //pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));
      temp_qy_num2 = temp_qy_num;//
      //�ұ߽�ѭ����ʼ
      for (temp_qy_i = 1; temp_qy_i < temp_total_bj && g_stu_square.nN < 20; temp_qy_i++)
      {
        //::Sleep(1);
        //�������ó���һ����
        switch (temp_fx) {
        case 1:
          temp_qy_h = temp_qy_h - 1;        //����
          break;
        case 2:
          temp_qy_l = temp_qy_l + 1;        //����
          break;
        case 3:
          temp_qy_h = temp_qy_h + 1;        //����
          break;
        case 4:
          temp_qy_l = temp_qy_l - 1;        //����
          break;
        }
        //����⵽�Ĵ�����߽��
        //pDC->Ellipse(temp_qy_l*rect.right/bmpWidth-2,temp_qy_h*rect.bottom/bmpHeight-2,temp_qy_l*rect.right/bmpWidth+2,temp_qy_h*rect.bottom/bmpHeight+2);
        //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));
        ++temp_bj_num;//�����߽����++
        *(temp_bj_z + temp_bj_num) = 0;//��ǰ����ظ���������0�����µ㣬����⣬��Ӧ�����0
        //mmy
        bool needback = 0;//�˻ش�������0

        if (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) == temp_qy_num)//���jg��ǰ���Ѿ����Ϊ��ǰ����˵���з���������
        {
          //��Ϊ����Ч�㣬������Ϊ��׼�߽�㱣�棬�����ǽ��������Է�������Բ���
          //*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)=temp_qy_num;  //����ǰ����ֵ�޸�Ϊ��ǰ���������Ǳ߽�����//�˴�ò�ƶ���
          *(temp_bj_h + temp_bj_num) = temp_qy_h;  //������
          *(temp_bj_l + temp_bj_num) = temp_qy_l;  //������
          *(temp_bj_fx + temp_bj_num) = temp_fx;
          *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = temp_fx;  //��ǰ����ֵ���򣬱���ı䣬��ֹ��ʼ���Ǳ������ת������������ʱ���򲻷���Ҫ��������

          if ((temp_qy_l == temp_qs_l) && (temp_qy_h == temp_qs_h))//�ж��Ƿ���ʼ����
          {
            //   showmessage('�ص����');
            break;  //�ص���㣬��ǰ������ұ߽�ѭ������
          }
          else
          {
            //���ڷ�գ�����ֱ���˻�
            //int nTmpH = temp_qy_h , nTmpL = temp_qy_l;		//�����˻���ʼ����
            if (debug && pDC != NULL)
              pDC->TextOutW(0, 0, _T("�нӴ�����"));
            //AfxMessageBox(_T("�нӴ�����"));
            int nTmpN = 0;								//�����˻ر߽����
            int* pnTmpH, *pnTmpL, *pnTmpFx;
            pnTmpH = new int[temp_bj_num];						//���������߽߱���������
            pnTmpL = new int[temp_bj_num];						//���������߽߱���������
            pnTmpFx = new int[temp_bj_num];						//����
            temp_qy_num2++;										//����������++��������
            int nMinH = bmpHeight, nMinL = bmpWidth, nMaxH = 0, nMaxL = 0;

            do
            {

              *(pnTmpH + nTmpN) = *(temp_bj_h + temp_bj_num);	//������ 
              *(pnTmpL + nTmpN) = *(temp_bj_l + temp_bj_num);	//������
              *(pnTmpFx + nTmpN) = *(temp_bj_fx + temp_bj_num);	//������
              if (nMinH > * (temp_bj_h + temp_bj_num)) nMinH = *(temp_bj_h + temp_bj_num);
              if (nMaxH < *(temp_bj_h + temp_bj_num)) nMaxH = *(temp_bj_h + temp_bj_num);
              if (nMinL > * (temp_bj_l + temp_bj_num)) nMinL = *(temp_bj_l + temp_bj_num);
              if (nMaxL < *(temp_bj_l + temp_bj_num)) nMaxL = *(temp_bj_l + temp_bj_num);

              *(jgvalue + *(temp_bj_h + temp_bj_num) * bmpWidth + *(temp_bj_l + temp_bj_num)) = temp_qy_num2;  //����ǰ����ֵ�޸�Ϊbɫ��ͬʱȡ��������
              //*(txvalue+*(temp_bj_h + temp_bj_num)*bmpWidth+*(temp_bj_l + temp_bj_num))=*(temp_bj_fx+temp_bj_num);  //����ǰ����ֵ�޸�Ϊ����ֵ��˵�����ҹ�
              nTmpN++;
              temp_bj_num--;		//�ܵ���-1���ص�
            } while (((temp_qy_h != *(temp_bj_h + temp_bj_num)) || (temp_qy_l != *(temp_bj_l + temp_bj_num))) && temp_bj_num != 0);//�˻�ѭ�� ///��ʱ����temp_bj_num!=0��Ϊ�����temp_bjnum == -1 ����������
            /////////////////////////////////

            //int mj=data_check_fill_bj(pBmpBuf,temp_qy_num2,0,temp_close,pnTmpH,pnTmpL,xsvalue,fbvalue,jgvalue,txvalue,bmpWidth,bmpHeight, 1,bmpHeight-2,1,bmpWidth-2);
            //������򲢷����������
            int mj = data_check_fill_bj(pBmpBuf, temp_qy_num2, 0, temp_close, pnTmpH, pnTmpL, xsvalue, fbvalue, jgvalue, txvalue, bmpWidth, bmpHeight, nMinH, nMaxH, nMinL, nMaxL, debug);
            if (debug)
            {
              sprintf(szname, "%s\\tmpbj.bmp", chPathMain);
              saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
            }
            /////////////////////////////////
            temp_qy_h = *(temp_bj_h + temp_bj_num);  //������
            temp_qy_l = *(temp_bj_l + temp_bj_num);  //������
            temp_fx = *(temp_bj_fx + temp_bj_num);
            if (nTmpN > ((bmpWidth + bmpHeight) >> 4) && temp_qy_num2 < 10000)//�����������Ҫ����ж�����
            {
              bool bChk = 1;
              if (circle)
              {

                if (g_stu_square.bJudgeFeature)//���Ҫƥ���������������ж�����Ƿ����Ҫ��
                {
                  if (mj > ((stuRef.Rad * stuRef.Rad * gd_PI) * 1.5))//��������1.5���������
                  {
                    g_stu_square.bIsLap = 1;
                    bChk = 0;
                  }
                  if (mj < ((stuRef.Rad * stuRef.Rad * gd_PI) * 0.5))//��������0.5���������
                    bChk = 0;
                }
                if (bChk)
                {
                  for (int n = 0, x = nTmpN - 1; n < x; n++, x--)//ǰ���ǰ��˻ص�˳�������꣬�������µ�����,���ж�����ʱ��Ҫ����
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
                  //����Ļ�ϻ����߽�
                /*	if(pDC!=NULL && type!=-1)
                  {
                    for(int n = 0; n<nTmpN; n++)
                      pDC->SetPixel(pnTmpL[n]*rect.right/bmpWidth,pnTmpH[n]*rect.bottom/bmpHeight,RGB(0,255,0));
                  }
                  //����Ļ�ϱ�עʶ����
                  if(pDC!=NULL &&debug)
                  switch(type)
                  {
                  case 0:
                    pDC->TextOutW(pnTmpL[0]*rect.right/bmpWidth,pnTmpH[0]*rect.bottom/bmpHeight,_T("����"));
                    break;
                  case -1:
                    pDC->TextOutW(pnTmpL[0]*rect.right/bmpWidth,pnTmpH[0]*rect.bottom/bmpHeight,_T("δʶ��"));
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
                  for (int n = 0, x = nTmpN - 1; n < x; n++, x--)//ǰ���ǰ��˻ص�˳�������꣬�������µ�����,���ж�����ʱ��Ҫ����
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
                        // AfxMessageBox(_T("����"));
                    type = 0;
                    break;
                  case 1://if(debug)
                         //AfxMessageBox(_T("��״������"));
                    type = -1;
                    break;
                  case 2://if(debug)
                        // AfxMessageBox(_T("�ٿ�"));
                    type = 2;
                    break;
                  case 3://if(debug)
                        // AfxMessageBox(_T("���"));
                    type = 1;
                    break;
                  case 4://if(debug)
                        // AfxMessageBox(_T("�߶�û��ȫ"));
                    type = -1;
                    break;
                  case 5://if(debug)
                      //AfxMessageBox(_T("����������"));
                    type = -1;//ë��Ƭһ��û�нǶ�Ҫ��
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
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("����"));
                      break;
                    case -1:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("δʶ��"));
                      break;
                    case 2:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("�ٿ�"));//AfxMessageBox(_T("�ٿ�"));
                      break;
                    case 1:
                      pDC->TextOutW(pnTmpL[0] * rect.right / bmpWidth, pnTmpH[0] * rect.bottom / bmpHeight, _T("���"));//AfxMessageBox(_T("���"));
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
        //���жϱ����Ƿ�Ϊ0��������˻ز������趨����������Ǽ���
        //mmy�����ж��Ƿ����Ѿ�����������
        if ((temp_qy_h < 1) ||
          (temp_qy_h + 1 >= bmpHeight) ||
          (temp_qy_l < 1) ||
          (temp_qy_l + 1 >= bmpWidth) ||
          (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) != -1) ||//&&*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)!=temp_qy_num)||//�ڶ����ж�ȡ��mmy
          *(xsvalue + temp_qy_h * bmpWidth + temp_qy_l) > 0 || //�Ǳ������
          *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) > -1//�����Ѿ����ҹ�������
          )//||(*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)>0&&*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)<temp_qy_num))     //�µ�һ��Ϊ0���˻���һ�㣬��������+1
        {//���������ߵ�ǰ�ģ�����/�����ر������ֵ����-1��//���ر������ֵ���ǵ�ǰ�����ţ����ѱ��
          needback = 1;
        }
        else if ((*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l + 1)) *
          (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l + 1)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l + 1)) *
          (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l - 1)) *
          (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l - 1)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l - 1)) != 0
          )//���û��ɫ
        {
          if (abs((*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l + 1)) *
            (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l + 1)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l + 1)) *
            (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l)) * (*(jgvalue + (temp_qy_h + 1) * bmpWidth + temp_qy_l - 1)) *
            (*(jgvalue + temp_qy_h * bmpWidth + temp_qy_l - 1)) * (*(jgvalue + (temp_qy_h - 1) * bmpWidth + temp_qy_l - 1))
          ) == 1)//ȫ��ɫ
            needback = 1;
        }

        //����ѭ�����˻ش����������Ҫ�˻أ�����ִ��
        //temp_fxnum=0;//��������0������
        /*
        //�ж��Ƿ�ص����Ѿ�����ı߽磬���Ƿ��γ��˱պ�
        temp_close=bj_close(temp_bj_h,temp_bj_l,temp_bj_num,temp_qy_h,temp_qy_l);  //�����Ƿ�պ�
         //�������ҵ��ı߽���������飬�����߽��������ʼ��ǰ�߽��������ʼ����
        //if( (temp_close>=0) && (temp_close<=temp_bj_num-2))//mmyȡ��ע��
          //break;
        //��Ϊ0�����ж��Ƿ�Ϊ�Ѿ�����
        if (*(jgvalue+temp_qy_h*bmpWidth+temp_qy_l)==temp_qy_num)//�����ǰ���ǵ�ǰ�����һ����
        {
          if ((temp_qy_l==temp_qs_l) && (temp_qy_h==temp_qs_h) )//�ж��Ƿ���ʼ����
          {
            //   showmessage('�ص����');
            break;  //�ص���㣬��ǰ������ұ߽�ѭ������
          }
        }
        *///mmy
        //����ܱ�8�����Ƿ����㣬���û�м�Ϊ�Ǳ߽磬�˻� (*(jgvalue+(temp_qy_h+1)*bmpWidth+temp_qy_l))
        if (needback)
        {//�˻ش���
          //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,0,0));//mmy

          switch (temp_fx) {
          case 1:
            temp_qy_h = temp_qy_h + 1;       //����
            break;
          case 2:
            temp_qy_l = temp_qy_l - 1;       //����
            break;
          case 3:
            temp_qy_h = temp_qy_h - 1;        //����
            break;
          case 4:
            temp_qy_l = temp_qy_l + 1;        //����
            break;
          }
          //ȥ�����Ǳ߽�ı�ʶ
            //pDC->Ellipse(temp_qy_l*rect.right/bmpWidth-2,temp_qy_h*rect.bottom/bmpHeight-2,temp_qy_l*rect.right/bmpWidth+2,temp_qy_h*rect.bottom/bmpHeight+2);
            //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));//mmy

          --temp_bj_num;//�����߽����--
          if (temp_bj_num < 0)
            AfxMessageBox(_T("TEMPBJNUM<0"));
          *(temp_bj_z + temp_bj_num) = *(temp_bj_z + temp_bj_num) + 1;//MMY
          temp_fx = temp_fx + 1;
          if (temp_fx == 5)
            temp_fx = 1;
          ////////////////////mmy����ǰû��//�õ�������3�Σ�ȡ���߽��ǲ��˻ش���
          while (3 <= *(temp_bj_z + temp_bj_num))//��temp_bj_num�������߽����ظ�������Ϊ3��
          {
            *(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) = -1;  //����ǰ����ֵ�޸�bɫ��ͬʱȡ�������ǣ����ܸĺڣ��ᵼ��jg����ı�������һ��ѭ�����иõ�
            *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = 0;  //����ǰ����ֵ������0����ʾ���������
            //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,0,0));//mmy

            if (0 == temp_bj_num) {//��������߽����ֻ��0�����˳��ұ߽�ѭ��
              break;
            }
            switch (temp_fx) {
            case 1:
              temp_qy_h = temp_qy_h - 1;        //����
              break;
            case 2:
              temp_qy_l = temp_qy_l + 1;        //����
              break;
            case 3:
              temp_qy_h = temp_qy_h + 1;        //����
              break;
            case 4:
              temp_qy_l = temp_qy_l - 1;        //����
              break;
            }
            //pDC->Ellipse(temp_qy_l*rect.right/bmpWidth-2,temp_qy_h*rect.bottom/bmpHeight-2,temp_qy_l*rect.right/bmpWidth+2,temp_qy_h*rect.bottom/bmpHeight+2);
      //if(debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));//
            --temp_bj_num;//�����߽����--
            if (temp_bj_num < 0)
              AfxMessageBox(_T("TEMPBJNUM<0"));
            *(temp_bj_z + temp_bj_num) = *(temp_bj_z + temp_bj_num) + 1;//MMY
            temp_fx = (temp_fx + 3);//mmy��2��Ϊ3
            if (temp_fx >= 5)	temp_fx = temp_fx - 4;
          }
          ////////////////////
          continue;
        }
        else
        {   //�����˻ش���������� ,�Ǳ߽�
          //temp_bj_num=temp_bj_num+1;//�����߽����++//������needbackһЩ���ݺ����ͼ���ܻ��˳ɹ�������tempbjnumÿ�ζ�����һ��
        //if(!debug&& pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,255,0));//
          *(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) = temp_qy_num;  //����ǰ����ֵ�޸�Ϊ��ǰ���������Ǳ߽�����
          tempi = tempi + 1;//���⣿������������
          *(temp_bj_h + temp_bj_num) = temp_qy_h;  //������
          *(temp_bj_l + temp_bj_num) = temp_qy_l;  //������
          *(temp_bj_fx + temp_bj_num) = temp_fx;
          *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = temp_fx;  //��ǰ����ֵ����
          if (min_h > temp_qy_h) min_h = temp_qy_h;
          if (max_h < temp_qy_h) max_h = temp_qy_h;
          if (min_l > temp_qy_l) min_l = temp_qy_l;
          if (max_l < temp_qy_l) max_l = temp_qy_l;
          temp_fx = (temp_fx - 1);
          if (temp_fx == 0)
            temp_fx = 4;  //ѭ��
        }
      }//�ұ߽�ѭ��2����

      if (temp_bj_num == 0 || g_stu_square.nN >= 20) {//��������߽����ֻ��0���˳���
        *(jgvalue + temp_qy_h * bmpWidth + temp_qy_l) = -1;//����ǰ����ֵ�޸�Ϊbɫ��ͬʱȡ��������
        *(txvalue + temp_qy_h * bmpWidth + temp_qy_l) = 0;  //����ǰ����ֵ������0
        //if(debug && pDC!=NULL)pDC->SetPixel(temp_qy_l*rect.right/bmpWidth,temp_qy_h*rect.bottom/bmpHeight,RGB(0,0,0));//mmy

        //continue;//������һ����ѭ��//��֪��Ҫ��Ҫcontinue
      }
      else
      {
        //data_check_fill_bj(pBmpBuf,temp_qy_num,0,temp_close,temp_bj_h,temp_bj_l,xsvalue,fbvalue,jgvalue,txvalue,bmpWidth,bmpHeight, min_h,max_h,min_l,max_l);
        //xsvalue,fbvalue,jgvalue,txvalueֻ��jgvalue�иģ�tempqynum�ĳ�ʼֵ��1
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
            if (g_stu_square.bJudgeFeature)//���Ҫƥ���������������ж�����Ƿ����Ҫ��
            {
              if (mj > ((stuRef.Rad * stuRef.Rad * gd_PI) * 1.5))//��������1.5���������
              {
                g_stu_square.bIsLap = 1;
                bChk = 0;
              }
              if (mj < ((stuRef.Rad * stuRef.Rad * gd_PI) * 0.5))//��������0.5���������
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
              case 0:pDC->TextOutW(temp_bj_l[0]*rect.right/bmpWidth,temp_bj_h[0]*rect.bottom/bmpHeight,_T("����"));
                break;
              case -1:pDC->TextOutW(temp_bj_l[0]*rect.right/bmpWidth,temp_bj_h[0]*rect.bottom/bmpHeight,_T("δʶ��"));
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
                    // AfxMessageBox(_T("����"));
                type = 0;
                break;
              case 1://if(debug)
                    //AfxMessageBox(_T("��״������"));
                type = -1;
                break;
              case 2://if(debug)
                    // AfxMessageBox(_T("�ٿ�"));
                type = 2;
                break;
              case 3://if(debug)
                    // AfxMessageBox(_T("���"));
                type = 1;
                break;
              case 4://if(debug)
                    // AfxMessageBox(_T("�߶�û��ȫ"));
                type = -1;
                break;
              case 5://if(debug)
                  //AfxMessageBox(_T("����������"));
                type = -1;//ë��Ƭһ��û�нǶ�Ҫ��
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
                case 0:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("����"));
                  break;
                case -1:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("δʶ��"));
                  break;
                case 2:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("�ٿ�"));//AfxMessageBox(_T("�ٿ�"));
                  break;
                case 1:pDC->TextOutW(temp_bj_l[0] * rect.right / bmpWidth, temp_bj_h[0] * rect.bottom / bmpHeight, _T("���"));//AfxMessageBox(_T("���"));
                  break;
                default:break;
                }
            }

          }
          //int zc=temp_bj_num;//��ͼ����˵����һ���߽������==�ܳ����ܳ�Ҫ������߳�֮����������
          if (debug) {
            sprintf(szname, "%s\\result%d.bmp", chPathMain, temp_qy_num);
            saveBmp(szname, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable1);
          }
          //if(!debug)
          //	break;
        }
      }
      //g_tmp_path=_T(".\\PIC\\temp1.bmp");
      temp_qy_num = temp_qy_num2 + 1;//һ����������ɣ�������++��
      temp_qy_num2 = temp_qy_num;
      //int i_count_mouse=temp_qy_num;
      //UpdateData(0);
      if (!debug)
        if (temp_qy_num > 10000)
          break;
    }   //��ѭ������
    //
    {
      /*if(temp_bj_num>(bmpWidth+bmpHeight)*2/3)
        break;*/

    }
    if (!debug)
      if (temp_qy_num > 10000)
        break;
  }  //��ѭ������
  //saveBmp("temps.bmp",pBmpBuf,bmpWidth,bmpHeight,biBitCount,pColorTable1);
  if (debug && pDC != NULL)
  {
    pDC->TextOutW(0, rect.bottom - 15, _T("��������"));//AfxMessageBox(_T("ʶ�����"));
  }
  //����Ļ�ϻ����߽�
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


  //pDC->SetPixel(int(100*rect.right/bmpWidth),int(100*rect.bottom/bmpHeight),RGB(255,0,0));//�㻭

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
  double angle =  acos((v1.x * v2.x + v1.y * v2.y) / sqrt((v1.x * v1.x + v1.y * v1.y) * (v2.x * v2.x + v2.y * v2.y))) * 180 / gd_PI;//����Ƕ�
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
  return acos((v1.x * v2.x + v1.y * v2.y) / sqrt((v1.x * v1.x + v1.y * v1.y) * (v2.x * v2.x + v2.y * v2.y)));//���㻡��

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