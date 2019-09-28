#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>
#include "cirnum.h"

using namespace cv;
void hc1(void) {
  Mat img = imread("IMG_79661.jpg");
  imshow("imgorg", img);
  Mat gray;
  cvtColor(img, gray, COLOR_BGR2GRAY);
  imshow("gray", gray);
  Mat thresh;
  threshold(gray, thresh, 199, 255, CV_THRESH_BINARY_INV);
  imshow("thresh", thresh);
  waitKey(0);
  // destroyAllWindows();
  //
  Mat blurred;
  blur(gray, blurred, Size(7, 7));  //��ֵ�˲�
  imshow("blurred", blurred);
  Mat lap;
  cv::Laplacian(blurred, lap, 0, 5); // no delta lapMask = lap > 50; // thresholding to values > 50
  imshow("lap", lap);
  Mat lapMask;
  threshold(lap, lapMask, 50, 255, THRESH_BINARY);
  imshow("lapMask", lapMask);

  Mat dilatedThresholdedLaplacian;
  cv::dilate(lapMask, dilatedThresholdedLaplacian, cv::Mat()); // dilate the edge mask once
  imshow("dilatedThresholdedLaplacian", dilatedThresholdedLaplacian);
  Mat separated = thresh - dilatedThresholdedLaplacian;

  //subtract(thresh,dilatedThresholdedLaplacian,separated,noArray()) ;
  imshow("separated", separated);


  std::vector<std::vector<cv::Point> > contours;
  cv::findContours(separated.clone(), contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

  double minArea = 500;
  double minCircleRatio = 0.5;
  for (unsigned int i = 0; i < contours.size(); ++i)
  {
    double cArea = cv::contourArea(contours[i]);
    if (cArea < minArea) continue;

    //filteredContours.push_back(contours[i]);
    //cv::drawContours(input, contours, i, cv::Scalar(0,255,0), 1);
    cv::Point2f center;
    float radius;
    cv::minEnclosingCircle(contours[i], center, radius);

    double circleArea = radius * radius * CV_PI;

    if (cArea / circleArea < minCircleRatio) continue;

    cv::circle(img, center, radius, cv::Scalar(0, 0, 255), 2);
  }
  imshow("imgend", img);
  waitKey(0);

}

void myimshow(const string& winname, InputArray mat)
{
  namedWindow(winname, WINDOW_NORMAL);
  imshow(winname, mat);
  waitKey(0);
}
void hc2(void) {
  Mat img = imread("IMG_7966.JPG");
  myimshow("imgorg", img);
  Mat gray;
  cvtColor(img, gray, COLOR_BGR2GRAY);
  myimshow("gray", gray);
  Mat blurred;
  blur(gray, blurred, Size(7, 7));  //��ֵ�˲�
  myimshow("blurred", blurred);


  Mat thresh;
  threshold(blurred, thresh, 200, 255, CV_THRESH_BINARY);
  myimshow("thresh", thresh);
  ////

  blur(gray, blurred, Size(7, 7));  //��ֵ�˲�
  myimshow("blurred", blurred);

  Mat dilatedThresholdedLaplacian;
  cv::dilate(thresh, dilatedThresholdedLaplacian, cv::Mat()); // dilate the edge mask once
  myimshow("dilatedThresholdedLaplacian", dilatedThresholdedLaplacian);

  waitKey(0);
  // destroyAllWindows();
  //
  //Mat blurred;
  //blur(gray, blurred, Size(7, 7));  //��ֵ�˲�
  //myimshow("blurred", blurred);
  Mat lap;
  cv::Laplacian(blurred, lap, 0, 5); // no delta lapMask = lap > 50; // thresholding to values > 50
  myimshow("lap", lap);
  Mat lapMask;
  threshold(lap, lapMask, 50, 255, THRESH_BINARY_INV);
  myimshow("lapMask", lapMask);

  // Mat dilatedThresholdedLaplacian;
  cv::dilate(lapMask, dilatedThresholdedLaplacian, cv::Mat()); // dilate the edge mask once
  myimshow("dilatedThresholdedLaplacian", dilatedThresholdedLaplacian);
  Mat separated = thresh - dilatedThresholdedLaplacian;

  //subtract(thresh,dilatedThresholdedLaplacian,separated,noArray()) ;
  myimshow("separated", separated);


  std::vector<std::vector<cv::Point> > contours;
  cv::findContours(separated.clone(), contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

  double minArea = 500;
  double minCircleRatio = 0.5;
  for (unsigned int i = 0; i < contours.size(); ++i)
  {
    double cArea = cv::contourArea(contours[i]);
    if (cArea < minArea) continue;

    //filteredContours.push_back(contours[i]);
    //cv::drawContours(input, contours, i, cv::Scalar(0,255,0), 1);
    cv::Point2f center;
    float radius;
    cv::minEnclosingCircle(contours[i], center, radius);

    double circleArea = radius * radius * CV_PI;

    if (cArea / circleArea < minCircleRatio) continue;

    cv::circle(img, center, radius, cv::Scalar(0, 0, 255), 2);
  }
  myimshow("imgend", img);
  waitKey(0);

}
#define PI 3.1415926
typedef struct tPoint {
  int x;
  int y;
}iPoint;
typedef struct tagArcPara {
  struct centert {
    int x;
    int y;

  }center;
  int r;

}ArcPara;
int HoughArc(int X[], int Y[], int Cnt, int r, ArcPara* Arc) {
  vector<iPoint>center;
  vector<int>VoteCnt;
  double theta;
  int a, b;
  int minA, maxA, minB, maxB;
  int VotedFlag = 0;
  double deltaTheta = PI / 180;//���1��
  double startAngle = 150.0 * PI / 180;
  double endAngle = PI * 2 + PI / 6;
  center.clear();
  VoteCnt.clear();
  minA = maxA = X[0] - r;
  minB = maxB = X[0]; //theta = 0
                      //����a��b����С�����ֵ
  for (int i = 0; i < Cnt; i++)
  {
    for (theta = startAngle; theta < endAngle; theta += deltaTheta)
    {
      a = (int)(X[i] - r * cos(theta) + 0.5);
      b = (int)(Y[i] - r * sin(theta) + 0.5);
      if (a > maxA)
      {
        maxA = a;
      }
      else if (a < minA)
      {
        minA = a;
      }

      if (b > maxB)
      {
        maxB = b;
      }
      else if (b < minB)
      {
        minB = b;
      }

    }
  }
  //ȷ��a��b�ķ�Χ֮�󣬼�ȷ����Ʊ��Ĵ�С
  int aScale = maxA - minA + 1;
  int bScale = maxB - minB + 1;

  int* VoteBox = new int[aScale * bScale];
  //VoteBox��ʼ��Ϊ0
  for (int i = 0; i < aScale * bScale; i++)
  {
    VoteBox[i] = 0;
  }
  //��ʼͶƱ
  for (int i = 0; i < Cnt; i++)
  {
    //printf("%d  ",i);
    for (theta = startAngle; theta < endAngle; theta += deltaTheta)
    {

      a = (int)(X[i] - r * cos(theta) + 0.5);
      b = (int)(Y[i] - r * sin(theta) + 0.5);
      VoteBox[(b - minB) * aScale + a - minA] = VoteBox[(b - minB) * aScale + a - minA] + 1;
    }
  }

  //ɸѡƱ��
  int VoteMax = 0;
  int VoteMaxX, VoteMaxY;
  for (int i = 0; i < bScale; i++)
  {
    for (int j = 0; j < aScale; j++)
    {
      if (VoteBox[i * aScale + j] > VoteMax)
      {
        VoteMax = VoteBox[i * aScale + j];
        VoteMaxY = i;
        VoteMaxX = j;
      }
    }
  }

  int Count = 0;
  printf("VoteMax: %d", VoteMax);
  for (int i = 0; i < bScale; i++)
  {
    for (int j = 0; j < aScale; j++)
    {
      if (VoteBox[i * aScale + j] >= VoteMax)
      {
        Count++;
      }
    }
  }
  printf("   %d \n", Count);
  //�ͷ��ڴ�
  delete[] VoteBox;
  if (VoteMax > 3)
  {
    Arc->center.x = VoteMaxX + minA;
    Arc->center.y = VoteMaxY + minB;
    Arc->r = r;
    return 1;
  }
  else {
    return 0;
  }
  return 1;
}

/**
* @brief ������ͼ�����ϸ��
* @param srcΪ����ͼ��,��cvThreshold�����������8λ�Ҷ�ͼ���ʽ��Ԫ����ֻ��0��1,1������Ԫ�أ�0����Ϊ�հ�
* @param maxIterations���Ƶ���������������������ƣ�Ĭ��Ϊ-1���������Ƶ���������ֱ��������ս��
* @return Ϊ��srcϸ��������ͼ��,��ʽ��src��ʽ��ͬ��Ԫ����ֻ��0��1,1������Ԫ�أ�0����Ϊ�հ�
*/
cv::Mat thinImage(const cv::Mat& src, const int maxIterations = -1)
{
  assert(src.type() == CV_8UC1);
  cv::Mat dst;
  int width = src.cols;
  int height = src.rows;
  src.copyTo(dst);
  int count = 0;  //��¼��������  
  while (true)
  {
    count++;
    if (maxIterations != -1 && count > maxIterations) //���ƴ������ҵ�����������  
      break;
    std::vector<uchar*> mFlag; //���ڱ����Ҫɾ���ĵ�  
                                //�Ե���  
    for (int i = 0; i < height; ++i)
    {
      uchar* p = dst.ptr<uchar>(i);
      for (int j = 0; j < width; ++j)
      {
        //��������ĸ����������б��  
        //  p9 p2 p3  
        //  p8 p1 p4  
        //  p7 p6 p5  
        uchar p1 = p[j];
        if (p1 != 1) continue;
        uchar p4 = (j == width - 1) ? 0 : *(p + j + 1);
        uchar p8 = (j == 0) ? 0 : *(p + j - 1);
        uchar p2 = (i == 0) ? 0 : *(p - dst.step + j);
        uchar p3 = (i == 0 || j == width - 1) ? 0 : *(p - dst.step + j + 1);
        uchar p9 = (i == 0 || j == 0) ? 0 : *(p - dst.step + j - 1);
        uchar p6 = (i == height - 1) ? 0 : *(p + dst.step + j);
        uchar p5 = (i == height - 1 || j == width - 1) ? 0 : *(p + dst.step + j + 1);
        uchar p7 = (i == height - 1 || j == 0) ? 0 : *(p + dst.step + j - 1);
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

          if (ap == 1 && p2 * p4 * p6 == 0 && p4 * p6 * p8 == 0)
          {
            //���  
            mFlag.push_back(p + j);
          }
        }
      }
    }

    //����ǵĵ�ɾ��  
    for (std::vector<uchar*>::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
    {
      **i = 0;
    }

    //ֱ��û�е����㣬�㷨����  
    if (mFlag.empty())
    {
      break;
    }
    else
    {
      mFlag.clear();//��mFlag���  
    }

    //�Ե���  
    for (int i = 0; i < height; ++i)
    {
      uchar* p = dst.ptr<uchar>(i);
      for (int j = 0; j < width; ++j)
      {
        //��������ĸ����������б��  
        //  p9 p2 p3  
        //  p8 p1 p4  
        //  p7 p6 p5  
        uchar p1 = p[j];
        if (p1 != 1) continue;
        uchar p4 = (j == width - 1) ? 0 : *(p + j + 1);
        uchar p8 = (j == 0) ? 0 : *(p + j - 1);
        uchar p2 = (i == 0) ? 0 : *(p - dst.step + j);
        uchar p3 = (i == 0 || j == width - 1) ? 0 : *(p - dst.step + j + 1);
        uchar p9 = (i == 0 || j == 0) ? 0 : *(p - dst.step + j - 1);
        uchar p6 = (i == height - 1) ? 0 : *(p + dst.step + j);
        uchar p5 = (i == height - 1 || j == width - 1) ? 0 : *(p + dst.step + j + 1);
        uchar p7 = (i == height - 1 || j == 0) ? 0 : *(p + dst.step + j - 1);

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

          if (ap == 1 && p2 * p4 * p8 == 0 && p2 * p6 * p8 == 0)
          {
            //���  
            mFlag.push_back(p + j);
          }
        }
      }
    }

    //����ǵĵ�ɾ��  
    for (std::vector<uchar*>::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
    {
      **i = 0;
    }

    //ֱ��û�е����㣬�㷨����  
    if (mFlag.empty())
    {
      break;
    }
    else
    {
      mFlag.clear();//��mFlag���  
    }
  }
  return dst;
}

void thin(IplImage* hsv, Mat* pDst)
{

  IplImage* pTemp = cvCreateImage(cvGetSize(hsv), hsv->depth, 1);
  if (hsv->nChannels == 3)
    cvCvtColor(hsv, pTemp, CV_BGR2GRAY);
  else
    cvCopy(hsv, pTemp);
  //���pDst��Ϊ�գ������ͷŵ�������������ռ�  
  /*if (*pDst)
    cvReleaseImage(pDst);*/
    //pDst = cvCreateImage(cvGetSize(hsv), hsv->depth, 1);

    //��ԭͼ��ת��Ϊ��ֵͼ��    
    /*
    void cvThreshold
    (   const CvArr* src,
    CvArr* dst,
    double threshold,
    double max_value,
    int threshold_type );
    ��������������ֵ
    ��threshold_type = CV_THRESH_BINARY��ʱ��
    ����src�е���һ(x,y)�������أ�
    if src(x,y) > threshold
    dst(x,y) = max_value;
    ��������Ķ�ֵ���ǽ����д�����ֵ�ĵ�����ֵ������Ϊ1��
    ΪʲôҪ���ó�1�أ������ϸ���㷨�涨������
    ԭʵ�������е���ֵ�����128��̫���ˣ���Ϊ�������Ѿ������˲������Խ���ֵ��Ϊ1��
    */
    //cvNamedWindow("1");
    //cvShowImage("1", pTemp);
  cvThreshold(pTemp, pTemp, 150, 1, CV_THRESH_BINARY_INV);
  // cvNamedWindow("2");
   //cvShowImage("2", pTemp);
   //ͼ��ϸ��    
   //thinImage(pTemp, pDst);
  Mat tmp;
  tmp = Mat(pTemp);
  Mat dst = thinImage(tmp);
  //myimshow("3", dst);

  //IplImage iptmp = IplImage(dst);

  //*pDst = cvCreateImage(cvGetSize(&iptmp), iptmp.depth, iptmp.nChannels);

  *pDst = dst;
  //ͼ��ϸ������֮��pDst��ÿ������ֵҪôΪ0��ҪôΪ1������������Ҫ��1��Ϊ255�����������������Զ԰�  
 /* for (int i = 0; i<(pDst)->rows; ++i)
  {
    for (int j = 0; j<(pDst)->cols; ++j)
    {
      if (CV_IMAGE_ELEM(pDst, uchar, i, j) == 1)
        CV_IMAGE_ELEM(pDst, uchar, i, j) = 255;
    }
  }*/
  threshold(*pDst, *pDst, 0, 255, THRESH_BINARY);

  //cvNamedWindow("5");
  //cvShowImage("5", pDst);
  myimshow("5", *pDst);


  ///�ǵ�
  Mat inimage(*pDst);
  Mat cornerStrength;
  //cornerHarris(inimage, cornerStrength, 2, 3, 0.04, BORDER_DEFAULT);
  //cornerHarris(inimage, cornerStrength, 2, 3, 0.04);
  vector<Point2f> corners;
  int maxCorners = 25;
  double qualityLevel = 0.01;
  double minDistance = 20;
  int blockSize = 10;
  bool useHarrisDetector = false;
  double k = 0.04;
  goodFeaturesToTrack(inimage,
    corners,
    maxCorners,
    qualityLevel,
    minDistance,
    Mat(),
    blockSize,
    useHarrisDetector,
    k);
  /// Draw corners detected  
  //for (int i = 0; i < corners.size(); i++) {
  //  circle(inimage, corners[i], 5, Scalar(255), 2, 8, 0);
  //  circle(inimage, corners[i], 4, Scalar(0, 255, 0), 2, 8, 0);
  //}
  /// Show what you got  
  myimshow("inimage", inimage);
}

void circleLeastFit(const std::vector<cv::Point2f> points, cv::Point2f& center, double& radius)
{
  radius = 0.0f;
  double sum_x = 0.0f, sum_y = 0.0f;
  double sum_x2 = 0.0f, sum_y2 = 0.0f;
  double sum_x3 = 0.0f, sum_y3 = 0.0f;
  double sum_xy = 0.0f, sum_x1y2 = 0.0f, sum_x2y1 = 0.0f;
  int N = points.size();
  for (int i = 0; i < N; i++)
  {
    double x = points[i].x;
    double y = points[i].y;
    double x2 = x * x;
    double y2 = y * y;
    sum_x += x;
    sum_y += y;
    sum_x2 += x2;
    sum_y2 += y2;
    sum_x3 += x2 * x;
    sum_y3 += y2 * y;
    sum_xy += x * y;
    sum_x1y2 += x * y2;
    sum_x2y1 += x2 * y;
  }
  double C, D, E, G, H;
  double a, b, c;
  C = N * sum_x2 - sum_x * sum_x;
  D = N * sum_xy - sum_x * sum_y;
  E = N * sum_x3 + N * sum_x1y2 - (sum_x2 + sum_y2) * sum_x;
  G = N * sum_y2 - sum_y * sum_y;
  H = N * sum_x2y1 + N * sum_y3 - (sum_x2 + sum_y2) * sum_y;
  a = (H * D - E * G) / (C * G - D * D);
  b = (H * C - E * D) / (D * D - G * C);
  c = -(a * sum_x + b * sum_y + sum_x2 + sum_y2) / N;
  center.x = a / (-2);
  center.y = b / (-2);
  radius = sqrt(a * a + b * b - 4 * c) / 2;

}
/** @function main */
int main1111(int argc, char** argv)
{
  /*hc2();
  return 0;*/
  Mat src, src_gray;

  /// Read the image
  src = imread("IMG_8039.jpg", 1);
  Mat mask = Mat::zeros(src.size(), CV_8UC1);

  if (!src.data)
  {
    return -1;
  }

  /// Convert it to gray
  cvtColor(src, src_gray, CV_BGR2GRAY);

  cv::morphologyEx(src_gray, src_gray, cv::MORPH_OPEN, cv::Mat(5, 5, CV_8U, cv::Scalar(1)), cvPoint(-1, -1), 1);
  cv::morphologyEx(src_gray, src_gray, cv::MORPH_CLOSE, cv::Mat(3, 3, CV_8U, cv::Scalar(1)), cvPoint(-1, -1), 1);
  //IplImage *sm_src;
  //*sm_src = IplImage(src_gray);
  /// Reduce the noise so we avoid false circle detection
  GaussianBlur(src_gray, src_gray, Size(9, 9), 2, 2);
  myimshow("gs", src_gray);
  vector<Vec3f> circles;

  Mat blurred;
  blur(src_gray, blurred, Size(10, 10));  //��ֵ�˲�
  myimshow("blurred", blurred);


  //Mat dilatedThresholdedLaplacian;
  //cv::dilate(blurred, dilatedThresholdedLaplacian, cv::Mat()); // dilate the edge mask once
  //myimshow("dilatedThresholdedLaplacian", dilatedThresholdedLaplacian);

  Mat thresh;
  threshold(blurred, thresh, 200, 255, CV_THRESH_BINARY);
  myimshow("thresh", thresh);////

  /////////////////////////////////
  /// Apply the Hough Transform to find the circles
  HoughCircles(thresh, circles, CV_HOUGH_GRADIENT,
    2, // accumulator resolution (size of the image / 2)
    20, // minimum distance between two circles
    128, // Canny high threshold
    80, // minimum number of votes
    100, 360); // min and max radius
    //1, src_gray.rows/8, 200, 100, 0, 0 );
  //myimshow("hc", circles);
  Mat maskImage;
  int radius, maskradius;
  Point seed;

  if (circles.size() <= 0) {
    waitKey(0);
    return -1;
  }
  /// Draw the circles detected
  for (size_t i = 0; i < circles.size(); i++)
  {
    Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
    seed.x = center.x;
    seed.y = center.y;
    //floodFill(mask, seed, 255, NULL, cvScalarAll(0), cvScalarAll(0), CV_FLOODFILL_FIXED_RANGE);
    radius = cvRound(circles[i][2]);
    maskradius = radius/* * 2 / 3*/;
    // circle center
    circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
    // circle outline
    circle(src, center, radius, Scalar(0, 0, 255), 3, 8, 0);
    circle(mask, center, maskradius, Scalar(255));//�ںڲ��ϻ�һ����ɫ������circle

    ////////////
    //��ˮ���
    //pi��ֵ��ʾΪ v(pi),if  v(seed)-loDiff<v(pi)<v(seed)+upDiff,��pi��ֵ����ΪnewVal
    floodFill(mask, seed, 255, NULL, cvScalarAll(0), cvScalarAll(0), CV_FLOODFILL_FIXED_RANGE);//��circle�������������circle
    //mask(rect).setTo(255);
    //threshold(mask, mask, 170, 255, CV_THRESH_BINARY_INV);
    myimshow("fill", mask);
    thresh.copyTo(maskImage, mask);//��mask���ƣ�mask�׵ĵط���ʾͼ�Σ�mask�ڵĵط�ȫ���ڸ�
    myimshow("mask", mask);
    myimshow("img2", maskImage);
    //
    //circle(maskImage, center, radius, Scalar(0, 0, 255), 3, 8, 0);
    circle(maskImage, center, maskradius, Scalar(255));//��һ����Ȧ

    myimshow("fill1", maskImage);
    floodFill(maskImage, Point(0, 0), 255, NULL, cvScalarAll(0), cvScalarAll(0), CV_FLOODFILL_FIXED_RANGE);//��Ȧ����䣬����Ȧ�ⶼ��ɰ�ɫ
    myimshow("fill2", maskImage);
    waitKey(0);
    destroyAllWindows();
    break;;
  }

  Rect rect(seed.x - maskradius, seed.y - maskradius, maskradius * 2, maskradius * 2);
  Mat image_roi = maskImage(rect);//��������


  IplImage ipmask = IplImage(image_roi);

  IplImage* erodedImage = cvCreateImage(cvGetSize(&ipmask), ipmask.depth, ipmask.nChannels);
  Mat thinimage;
  IplImage* smothimage = cvCreateImage(cvGetSize(&ipmask), ipmask.depth, ipmask.nChannels);
  cvErode(&ipmask, erodedImage, 0, 2);//��ʴ  
  cvSmooth(erodedImage, smothimage, CV_GAUSSIAN, 5, 5);
  //erode(maskImage, eroImage, cv::Mat());
  myimshow("erodedImage", Mat(smothimage));
  thin(smothimage, &thinimage);
  myimshow("thinimage", thinimage);
  //cvNamedWindow("thinimage");
  //cvShowImage("thinimage", thinimage);
  /// Show your results
  //myimshow( "Hough Circle Transform Demo", src );
  //�и�
  int cutlen = maskradius * 1 / 3;
  Rect cutrect(cutlen, cutlen, (maskradius - cutlen) * 2, (maskradius - cutlen) * 2);
  Mat image_cut = thinimage(cutrect);
  myimshow("image_cut", image_cut);

  //Mat resultImage = Mat::zeros(img.size(), CV_8U);
  Mat dstImage = Mat::zeros(image_cut.size(), CV_8U);// Mat(thinimage);

  //���������Ͳ�νṹ  
  vector<vector<Point>>contours;
  vector<Vec4i>hierarchy;
  findContours(image_cut, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
  //���������  
  /*for (vector<vector<Point>>::iterator it=contours.begin();it!=contours.end();++it)
  {
  for (vector<Point>::iterator inner_it=it->begin();inner_it!=it->end();++inner_it)
  {
  cout<<*inner_it<<endl;
  }
  }
  */
  //�±����  
  for (int i = 0; i < contours.size(); i++)
  {
    for (int j = 0; j < contours[i].size(); j++)
    {
      //cout << contours[i][j].x << "   " << contours[i][j].y << endl;
      //ofstream f;
      //f.open("E:/����������.txt", ios::out | ios::app);
      contours[i][j].x;
      contours[i][j].y;
    }
  }
  Mat dstImage2;
  //thinimage.copyTo(dstImage);
  cvtColor(dstImage, dstImage2, CV_GRAY2BGR);
  //���������������������ɫ���Ƴ�ÿ�����������ɫ  
  int index = 0;
  for (; index >= 0; index = hierarchy[index][0])
  {
    Scalar color(rand() % 255, rand() % 255, rand() % 255);
    //Scalar color( 255);
    drawContours(dstImage2, contours, index, color, 1, 8, hierarchy);
    // Mat resultImage = Mat::zeros(img.size(), CV_8U);
   //drawContours(dstImage, contours, -1, Scalar(255, 0, 255));
  }

  myimshow("����ͼ", dstImage2);



  waitKey(0);
  return 0;


}
/** @function main */
void drawHist(Mat srcImage)
{
  int channels = 0;
  MatND dstHist;
  int histSize[] = { 256 };       //���д��int histSize = 256;���ü���ֱ��ͼ�ĺ�����ʱ���ñ�����д��&histSize    
  float midRanges[] = { 0, 256 };
  const float* ranges[] = { midRanges };
  calcHist(&srcImage, 1, &channels, Mat(), dstHist, 1, histSize, ranges, true, false);

  //����ֱ��ͼ,�����ȴ���һ���ڵ׵�ͼ��Ϊ�˿�����ʾ��ɫ�����Ըû���ͼ����һ��8λ��3ͨ��ͼ��    
  Mat drawImage = Mat::zeros(Size(256, 256), CV_8UC3);
  //�κ�һ��ͼ���ĳ�����ص��ܸ����п��ܻ�ܶ࣬���������������ͼ��ĳߴ磬  
  //������Ҫ�ȶԸ������з�Χ�����ƣ���minMaxLoc�������õ�����ֱ��ͼ������ص�������    
  double g_dHistMaxValue;
  minMaxLoc(dstHist, 0, &g_dHistMaxValue, 0, 0);
  //�����صĸ������ϵ�ͼ������Χ��    
  for (int i = 0; i < 256; i++)
  {
    int value = cvRound(dstHist.at<float>(i) * 256 * 0.9 / g_dHistMaxValue);
    line(drawImage, Point(i, drawImage.rows - 1), Point(i, drawImage.rows - 1 - value), Scalar(0, 0, 255));
  }

  myimshow("hist", drawImage);
}



double alpha_value, beta_value;
int trackbar_value = 40;
const int trackbar_max = 100;
Mat imgsrc1, imgsrc2, dstimg;
void trackbar_cb(int, void*) {

  alpha_value = (double)trackbar_value / trackbar_max;

  beta_value = (1.0 - alpha_value);

  //����alpha��beata��ֵ���������ں�

  /*addWeighted(imgsrc1, alpha_value, imgsrc2, beta_value, 0.0, dstimg);

  imshow("src_gray", dstimg);*/

}


int cirnum_main(void)
{
  /*hc2();
  return 0;*/
  Mat src, src_gray;

  /// Read the image
  src = imread("IMG_8055.jpg", 1);
  resize(src, src, Size(src.cols / 4, src.rows / 4), 0, 0, INTER_LINEAR);
  Mat mask = Mat::zeros(src.size(), CV_8UC1);

  if (!src.data)
  {
    return -1;
  }

  /// Convert it to gray
  cvtColor(src, src_gray, CV_BGR2GRAY);
  drawHist(src_gray);
  myimshow("src_gray", src_gray);////
  char trackbarname[50];
  sprintf(trackbarname, "͸��ֵ ");
  createTrackbar(trackbarname, "src_gray", &trackbar_value, trackbar_max, trackbar_cb);
  trackbar_cb(trackbar_value, 0);
  Mat thresh;
  threshold(src_gray, thresh, 180, 255, THRESH_BINARY);
  //adaptiveThreshold(src_gray, thresh, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 1);
  myimshow("thresh", thresh);////
  cv::morphologyEx(thresh, thresh, cv::MORPH_OPEN, cv::Mat(3, 3, CV_8U, cv::Scalar(1)), cvPoint(-1, -1), 1);
  myimshow("MORPH_OPEN", thresh);
  cv::morphologyEx(thresh, thresh, cv::MORPH_CLOSE, cv::Mat(3, 3, CV_8U, cv::Scalar(1)), cvPoint(-1, -1), 1);
  myimshow("MORPH_CLOSE", thresh);

  //IplImage *sm_src;

    //Mat resultImage = Mat::zeros(img.size(), CV_8U);
  Mat dstImage = Mat::zeros(/*image_cut*/thresh.size(), CV_8U);// Mat(thinimage);

                                                               //���������Ͳ�νṹ  
  vector<vector<Point>>contours;
  vector<Vec4i>hierarchy;
  findContours(/*image_cut*/thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
  //���������  
  /*for (vector<vector<Point>>::iterator it=contours.begin();it!=contours.end();++it)
  {
  for (vector<Point>::iterator inner_it=it->begin();inner_it!=it->end();++inner_it)
  {
  cout<<*inner_it<<endl;
  }
  }
  */
  //�±����  
  for (int i = 0; i < contours.size(); i++)
  {
    for (int j = 0; j < contours[i].size(); j++)
    {
      //cout << contours[i][j].x << "   " << contours[i][j].y << endl;
      //ofstream f;
      //f.open("E:/����������.txt", ios::out | ios::app);
      contours[i][j].x;
      contours[i][j].y;
    }
  }
  Mat dstImage2;
  //thinimage.copyTo(dstImage);
  cvtColor(dstImage, dstImage2, CV_GRAY2BGR);
  //���������������������ɫ���Ƴ�ÿ�����������ɫ  
  int index = 0;
  /*hierarchy[i][0],hierarchy[i][1],hierarchy[i][2],hierarchy[i][3],�ֱ��ʾ���ǵ�i������(contours[i])����һ����ǰһ���������ĵ�һ������(��һ��������)�Ͱ�����������(������)��*/
  for (; index >= 0; index = hierarchy[index][0])
  {
    if (contours[index].size() < 100)
      continue;
    int i = index;
    for (; i >= 0; i = hierarchy[i][2])
    {
      /*for (int x = )
      {

      }*/
      Scalar color(rand() % 255, rand() % 255, rand() % 255);
      drawContours(dstImage2, contours, i, color, 1);
      myimshow("����ͼ", dstImage2);
    }
    //Scalar color(rand() % 255, rand() % 255, rand() % 255);
    ////Scalar color( 255);
    //drawContours(dstImage2, contours, index, color, 1, 8, hierarchy);
    //// Mat resultImage = Mat::zeros(img.size(), CV_8U);
    ////drawContours(dstImage, contours, -1, Scalar(255, 0, 255));
    //myimshow("����ͼ", dstImage2);
  }



  //*sm_src = IplImage(src_gray);
  /// Reduce the noise so we avoid false circle detection
  /*GaussianBlur(thresh, thresh, Size(9, 9), 2, 2);
  myimshow("gs", thresh);*/
  vector<Vec3f> circles;

  Mat blurred;
  blur(thresh, blurred, Size(10, 10));  //��ֵ�˲�
  myimshow("blurred", blurred);


  //Mat dilatedThresholdedLaplacian;
  //cv::dilate(blurred, dilatedThresholdedLaplacian, cv::Mat()); // dilate the edge mask once
  //myimshow("dilatedThresholdedLaplacian", dilatedThresholdedLaplacian);



                             /////////////////////////////////
                             /// Apply the Hough Transform to find the circles
  HoughCircles(thresh, circles, CV_HOUGH_GRADIENT,
    2, // accumulator resolution (size of the image / 2)
    5, // minimum distance between two circles
    128, // Canny high threshold
    70, // minimum number of votes
    30, 360); // min and max radius
               //1, src_gray.rows/8, 200, 100, 0, 0 );
               //myimshow("hc", circles);
  Mat maskImage;
  int radius, maskradius;
  Point seed;

  if (circles.size() <= 0) {
    waitKey(0);
    return -1;
  }
  /// Draw the circles detected
  for (size_t i = 0; i < circles.size(); i++)
  {
    Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
    seed.x = center.x;
    seed.y = center.y;
    //floodFill(mask, seed, 255, NULL, cvScalarAll(0), cvScalarAll(0), CV_FLOODFILL_FIXED_RANGE);
    radius = cvRound(circles[i][2]);
    maskradius = radius/* * 2 / 3*/;
    // circle center
    circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
    // circle outline
    circle(src, center, radius, Scalar(0, 0, 255), 3, 8, 0);
    circle(mask, center, maskradius, Scalar(255));//�ںڲ��ϻ�һ����ɫ������circle

                                                  ////////////
                                                  //��ˮ���
                                                  //pi��ֵ��ʾΪ v(pi),if  v(seed)-loDiff<v(pi)<v(seed)+upDiff,��pi��ֵ����ΪnewVal
    floodFill(mask, seed, 255, NULL, cvScalarAll(0), cvScalarAll(0), CV_FLOODFILL_FIXED_RANGE);//��circle�������������circle
                                                                                               //mask(rect).setTo(255);
                                                                                               //threshold(mask, mask, 170, 255, CV_THRESH_BINARY_INV);
    myimshow("fill", mask);
    thresh.copyTo(maskImage, mask);//��mask���ƣ�mask�׵ĵط���ʾͼ�Σ�mask�ڵĵط�ȫ���ڸ�
    myimshow("mask", mask);
    myimshow("img2", maskImage);
    //
    //circle(maskImage, center, radius, Scalar(0, 0, 255), 3, 8, 0);
    circle(maskImage, center, maskradius, Scalar(255));//��һ����Ȧ

    myimshow("fill1", maskImage);
    floodFill(maskImage, Point(0, 0), 255, NULL, cvScalarAll(0), cvScalarAll(0), CV_FLOODFILL_FIXED_RANGE);//��Ȧ����䣬����Ȧ�ⶼ��ɰ�ɫ
    myimshow("fill2", maskImage);
    waitKey(0);
    destroyAllWindows();
    break;;
  }

  Rect rect(seed.x - maskradius, seed.y - maskradius, maskradius * 2, maskradius * 2);
  Mat image_roi = maskImage(rect);//��������


  IplImage ipmask = IplImage(image_roi);

  IplImage* erodedImage = cvCreateImage(cvGetSize(&ipmask), ipmask.depth, ipmask.nChannels);
  Mat thinimage;
  IplImage* smothimage = cvCreateImage(cvGetSize(&ipmask), ipmask.depth, ipmask.nChannels);
  cvErode(&ipmask, erodedImage, 0, 2);//��ʴ  
  cvSmooth(erodedImage, smothimage, CV_GAUSSIAN, 5, 5);
  //erode(maskImage, eroImage, cv::Mat());
  myimshow("erodedImage", Mat(smothimage));
  thin(smothimage, &thinimage);
  myimshow("thinimage", thinimage);
  //cvNamedWindow("thinimage");
  //cvShowImage("thinimage", thinimage);
  /// Show your results
  //myimshow( "Hough Circle Transform Demo", src );
  //�и�
  int cutlen = maskradius * 1 / 3;
  Rect cutrect(cutlen, cutlen, (maskradius - cutlen) * 2, (maskradius - cutlen) * 2);
  Mat image_cut = thinimage(cutrect);
  myimshow("image_cut", image_cut);





  waitKey(0);
  return 0;


}
