
#include "corner.h"

#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>
#include "cirnum.h"
#include <utility>//pair
using namespace std;

using namespace cv;

/**
 * @brief ������ͼ�����ϸ��
 * @param[in] srcΪ����ͼ��,��cvThreshold�����������8λ�Ҷ�ͼ���ʽ��Ԫ����ֻ��0��1,1������Ԫ�أ�0����Ϊ�հ�
 * @param[out] dstΪ��srcϸ��������ͼ��,��ʽ��src��ʽ��ͬ������ǰ��Ҫ����ռ䣬Ԫ����ֻ��0��1,1������Ԫ�أ�0����Ϊ�հ�
 * @param[in] maxIterations���Ƶ���������������������ƣ�Ĭ��Ϊ-1���������Ƶ���������ֱ��������ս��
 */
void thinImage(IplImage* src, IplImage* dst, int maxIterations)
{
  CvSize size = cvGetSize(src);
  cvCopy(src, dst);//��src�е����ݿ�����dst��  
  int count = 0;  //��¼��������  
  while (true)
  {
    count++;
    if (maxIterations != -1 && count > maxIterations) //���ƴ������ҵ�����������  
      break;
    //std::cout << count << ' ';�����������  
    vector<pair<int, int> > mFlag; //���ڱ����Ҫɾ���ĵ�  
    //�Ե���  
    for (int i = 0; i < size.height; ++i)
    {
      for (int j = 0; j < size.width; ++j)
      {
        //��������ĸ����������б��  
        //  p9 p2 p3  
        //  p8 p1 p4  
        //  p7 p6 p5  
        int p1 = CV_IMAGE_ELEM(dst, uchar, i, j) / 255;
        int p2 = (i == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j) / 255;
        int p3 = (i == 0 || j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j + 1) / 255;
        int p4 = (j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i, j + 1) / 255;
        int p5 = (i == size.height - 1 || j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j + 1) / 255;
        int p6 = (i == size.height - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j) / 255;
        int p7 = (i == size.height - 1 || j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j - 1) / 255;
        int p8 = (j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i, j - 1) / 255;
        int p9 = (i == 0 || j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j - 1) / 255;

        if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6)
        {
          int ap = 0;
          if (p2 == 0 && p3 == 1) ++ap;
          if (p3 == 0 && p4 == 1) ++ap;
          if (p4 == 0 && p5 == 1) ++ap;
          if (p5 == 0 && p6 == 1) ++ap;
          if (p6 == 0 && p7 == 1) ++ap;
          if (p7 == 0 && p8 == 1) ++ap;
          if (p8 == 0 && p9 == 1) ++ap;
          if (p9 == 0 && p2 == 1) ++ap;

          if (ap == 1)
          {
            if (p2 * p4 * p6 == 0)
            {
              if (p4 * p6 * p8 == 0)
              {
                //���  
                mFlag.push_back(make_pair(i, j));
              }
            }
          }
        }
      }
    }

    //����ǵĵ�ɾ��  
    for (vector<pair<int, int> >::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
    {
      CV_IMAGE_ELEM(dst, uchar, i->first, i->second) = 0;
    }

    //ֱ��û�е����㣬�㷨����  
    if (mFlag.size() == 0)
    {
      break;
    }
    else
    {
      mFlag.clear();//��mFlag���  
    }

    //�Ե���  
    for (int i = 0; i < size.height; ++i)
    {
      for (int j = 0; j < size.width; ++j)
      {
        //��������ĸ����������б��  
        //  p9 p2 p3  
        //  p8 p1 p4  
        //  p7 p6 p5  
        int p1 = CV_IMAGE_ELEM(dst, uchar, i, j) / 255;
        if (p1 != 1) continue;
        int p2 = (i == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j) / 255;
        int p3 = (i == 0 || j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j + 1) / 255;
        int p4 = (j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i, j + 1) / 255;
        int p5 = (i == size.height - 1 || j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j + 1) / 255;
        int p6 = (i == size.height - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j) / 255;
        int p7 = (i == size.height - 1 || j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j - 1) / 255;
        int p8 = (j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i, j - 1) / 255;
        int p9 = (i == 0 || j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j - 1) / 255;

        if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6)
        {
          int ap = 0;
          if (p2 == 0 && p3 == 1) ++ap;
          if (p3 == 0 && p4 == 1) ++ap;
          if (p4 == 0 && p5 == 1) ++ap;
          if (p5 == 0 && p6 == 1) ++ap;
          if (p6 == 0 && p7 == 1) ++ap;
          if (p7 == 0 && p8 == 1) ++ap;
          if (p8 == 0 && p9 == 1) ++ap;
          if (p9 == 0 && p2 == 1) ++ap;

          if (ap == 1)
          {
            if (p2 * p4 * p8 == 0)
            {
              if (p2 * p6 * p8 == 0)
              {
                //���  
                mFlag.push_back(make_pair(i, j));
              }
            }
          }
        }
      }
    }
    //ɾ��  
    for (vector<pair<int, int> >::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
    {
      CV_IMAGE_ELEM(dst, uchar, i->first, i->second) = 0;
    }

    //ֱ��û�е����㣬�㷨����  
    if (mFlag.size() == 0)
    {
      break;
    }
    else
    {
      mFlag.clear();//��mFlag���  
    }
  }
}

void Imagecutoff(IplImage* src, IplImage* dst, int x, int y, int width, int height)
{

  cvSetImageROI(src, cvRect(x, y, width, height));
  cvCopy(src, dst, 0);
  cvResetImageROI(src);
}

IplImage * FastRecognition(IplImage * src)
{
  IplImage* temp1;
  //if ((temp1 = cvLoadImage("./ƥ��ģ��.png", 1)) == 0) {
  //	return src;
  //}
  //MatchTemplate(src, temp1);
  //long time1 = GetTickCount();
  //AutoPicSave(src);

  IplImage* dst2;
  IplImage* dst3;
  /*src = */Imagecutoff(src, src, 0, 0, src->width - 50, src->height - 10);
  dst2 = cvCreateImage(cvGetSize(src), src->depth, 1);
  dst3 = cvCreateImage(cvGetSize(src), src->depth, 1);
  cvCvtColor(src, dst2, CV_BGR2GRAY);
  cvErode(dst2, dst2, 0, 2);
  cvDilate(dst2, dst2, 0, 2);
  //cvThreshold(dst2, dst2,100,255.0,CV_THRESH_BINARY);
  //cvSmooth(dst2, dst2, CV_MEDIAN, 3);

  cvCanny(dst2, dst2, 30, 100, 3);
  cvDilate(dst2, dst2, 0, 5);

  cvErode(dst2, dst3, 0, 5);
  //cvThreshold(dst2, dst2, 100, 255.0, CV_THRESH_BINARY);
  thinImage(dst3, dst2, -1);
  cvNamedWindow("Thin");
  cvShowImage("Thin", dst2);

#if 0
  IplImage * color_dst = cvCreateImage(cvGetSize(src), 8, 3);
  CvMemStorage* storage = cvCreateMemStorage();
  CvMemStorage* storage2 = cvCreateMemStorage(0);
  CvSeq* lines = 0;
  CvSeq* LinesCombine = cvCreateSeq(CV_32FC2, sizeof(CvSeq), sizeof(CvPoint2D32f), storage2);
  lines = cvHoughLines2(dst2, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 10, 5, 20);
  for (int i = 0; i < lines->total; i++)
  {
    CvPoint* line = (CvPoint*)cvGetSeqElem(lines, i);

    if (i == 0)
    {
      cvSeqPush(LinesCombine, line);
      cvSeqPush(LinesCombine, ++line);
    }
    else
    {
      int Flag = 0;
      for (int j = 0; j < LinesCombine->total; j++)
      {

        CvPoint* line2 = (CvPoint*)cvGetSeqElem(LinesCombine, j * 2);
        double k2 = (double)(line2[1].y - line2[0].y) / (line2[1].x - line2[0].x);
        double b2 = line2[0].y - k2 * line2[0].x;
        double A = k2;
        double B = -1;
        double C = b2;

        double k = (double)(line[1].y - line[0].y) / (line[1].x - line[0].x);//��ǰֱ�ߵ�б��
        double d = abs(A * (line[0].x) + B * (line[0].y) + C) / sqrt(pow(A, 2) + pow(B, 2));//�㵽ֱ�ߵľ���
        CvPoint line3[2];
        if (abs(atan(k) - atan(k2)) * 180 / CV_PI < 10 && d < 20)//�ж�Ϊͬһ���ߡ��ϲ��߶�
        {
          //if(line[0].x< line2[0].x)
          Flag = 1;
        }
      }
      if (Flag == 0)
      {
        cvSeqPush(LinesCombine, line);
        cvSeqPush(LinesCombine, ++line);
      }
    }
    //cvLine(color_dst, line[0], line[1], CV_RGB(255, 0, 0), 1, CV_AA);	
  }
  for (int i = 0; i < LinesCombine->total; i++)
  {
    CvPoint* line = (CvPoint*)cvGetSeqElem(LinesCombine, i * 2);
    cvLine(color_dst, line[0], line[1], CV_RGB(255, 0, 0), 1, CV_AA);
  }
  cvNamedWindow("Hough");
  cvShowImage("Hough", color_dst);

  cvWaitKey(0);

  cvReleaseImage(&color_dst);
  cvReleaseMemStorage(&storage);
  cvReleaseMemStorage(&storage2);
#else
  double dConArea;
  CvSeq* pContour = NULL;
  CvSeq* pConInner = NULL;
  CvMemStorage* pStorage = NULL;
  IplImage* color_dst = cvCreateImage(cvGetSize(src), 8, 3);
  cvCvtColor(dst2, color_dst, CV_GRAY2RGB);

  // ִ������   
  if (dst2)
  {
    // ������������   
    pStorage = cvCreateMemStorage(0);
    cvFindContours(dst2, pStorage, &pContour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    // �����������   
    cvDrawContours(color_dst, pContour, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 2, CV_FILLED, 8, cvPoint(0, 0));
    // ������ѭ��   
    int wai = 0;
    int nei = 0;
    for (; pContour != NULL; pContour = pContour->h_next)
    {
      wai++;
      // ������ѭ��   
      for (pConInner = pContour->v_next; pConInner != NULL; pConInner = pConInner->h_next)
      {
        nei++;
        // ���������   
        dConArea = fabs(cvContourArea(pConInner, CV_WHOLE_SEQ));
        printf("%f\n", dConArea);
      }
      CvRect rect = cvBoundingRect(pContour, 0);
      float* line = new float[4];
      cvFitLine(pContour, CV_DIST_L2, 0, 0.01, 0.01, line);
      //cvLine(color_dst, line[0], line[1], CV_RGB(255, 0, 0), 1, CV_AA);
      delete line;

      cvRectangle(color_dst, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height), CV_RGB(0, 255, 0), 1, 8, 0);
    }

    cvReleaseMemStorage(&pStorage);
    pStorage = NULL;
  }
  cvNamedWindow("����");
  cvShowImage("����", color_dst);
#endif


  //cvCopy(dst2, src);
  //m_image.CopyOf(img);
  //UpdateAllViews(NULL);	
  return dst2;
}



int corner_main(void)
{
  //if ((temp1 = cvLoadImage("./ƥ��ģ��.png", 1)) == 0) {
 //	return src;
 //}
 //MatchTemplate(src, temp1);
 //long time1 = GetTickCount();
 //AutoPicSave(src);

  FastRecognition();

  return 0;
}