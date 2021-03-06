﻿/*
   area.c
   Nate Robins, 1997

   A simple program to compute the area of a rasterized triangle.

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#if defined (__APPLE__) || defined (MACOSX)
#include <GLUT/glut.h>
#else
#include "gl/glut.h"
#endif

#include <math.h>
const GLfloat Pi = 3.1415926536f;

#define WIDTH 400 
#define HEIGHT 400 
// 参看 SUB16
//截图程序，复制到别的地方调用即可截取
#define WindowWidth  400 
#define WindowHeight 400 

//#include <stdio.h> 
//#include <stdlib.h> 

/* 函数 grab
 * 抓取窗口中的像素
 * 假设窗口宽度为 WindowWidth，高度为 WindowHeight
 */
#define BMP_Header_Length 54 
void grab(void)
{
  FILE* pDummyFile;
  FILE* pWritingFile;
  GLubyte* pPixelData;
  GLubyte  BMP_Header[BMP_Header_Length];
  GLint    i, j;
  GLint    PixelDataLength;

  // 计算像素数据的实际长度 
  i = WindowWidth * 3;   // 得到每一行的像素数据长度 
  while (i % 4 != 0)      // 补充数据，直到 i 是的倍数 
    ++i;               // 本来还有更快的算法， 
                       // 但这里仅追求直观，对速度没有太高要求 
  PixelDataLength = i * WindowHeight;

  // 分配内存和打开文件 
  pPixelData = (GLubyte*)malloc(PixelDataLength);
  if (pPixelData == 0)
    exit(0);

  pDummyFile = fopen("dummy.bmp", "rb");//任意的24位bmp
  if (pDummyFile == 0)
    exit(0);

  pWritingFile = fopen("grab.bmp", "wb");
  if (pWritingFile == 0)
    exit(0);
  glReadBuffer(GL_FRONT); //以保证读取到的内容正好就是显示的内容。
  // 读取像素 
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  glReadPixels(0, 0, WindowWidth, WindowHeight,
    GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);//读取现在窗口中显示的图像像素

  // 把 dummy.bmp 的文件头复制为新文件的文件头 
  fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);
  fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
  fseek(pWritingFile, 0x0012, SEEK_SET);
  i = WindowWidth;
  j = WindowHeight;
  fwrite(&i, sizeof(i), 1, pWritingFile);
  fwrite(&j, sizeof(j), 1, pWritingFile);

  // 写入像素数据 
  fseek(pWritingFile, 0, SEEK_END);
  fwrite(pPixelData, PixelDataLength, 1, pWritingFile);

  // 释放内存和关闭文件 
  fclose(pDummyFile);
  fclose(pWritingFile);
  free(pPixelData);
}

/* 函数 power_of_two
 * 检查一个整数是否为 2 的整数次方，如果是，返回 1，否则返回 0
 * 实际上只要查看其二进制位中有多少个，如果正好有 1 个，返回 1，否则返回 0
 * 在“查看其二进制位中有多少个”时使用了一个小技巧
 * 使用 n &= (n-1)可以使得 n 中的减少一个（具体原理大家可以自己思考）
 */
int power_of_two(int n)
{
  if (n <= 0)
    return 0;
  return (n & (n - 1)) == 0;
}

/* 函数 load_texture
 * 读取一个 BMP 文件作为纹理
 * 如果失败，返回 0，如果成功，返回纹理编号
 */
GLuint load_texture(const char* file_name)
{
  GLint width, height, total_bytes;
  GLubyte* pixels = 0;
  GLuint last_texture_ID, texture_ID = 0;

  // 打开文件，如果失败，返回 
  FILE* pFile = fopen(file_name, "rb");
  if (pFile == 0)
    return 0;

  // 读取文件中图象的宽度和高度 
  fseek(pFile, 0x0012, SEEK_SET);
  fread(&width, 4, 1, pFile);
  fread(&height, 4, 1, pFile);
  fseek(pFile, BMP_Header_Length, SEEK_SET);

  // 计算每行像素所占字节数，并根据此数据计算总像素字节数 
  {
    GLint line_bytes = width * 3;
    while (line_bytes % 4 != 0)
      ++line_bytes;
    total_bytes = line_bytes * height;
  }

  // 根据总像素字节数分配内存 
  pixels = (GLubyte*)malloc(total_bytes);
  if (pixels == 0)
  {
    fclose(pFile);
    return 0;
  }

  // 读取像素数据 
  if (fread(pixels, total_bytes, 1, pFile) <= 0)
  {
    free(pixels);
    fclose(pFile);
    return 0;
  }

  // 在旧版本的 OpenGL 中 
  // 如果图象的宽度和高度不是的整数次方，则需要进行缩放 
  // 这里并没有检查 OpenGL 版本，出于对版本兼容性的考虑，按旧版本处理 
  // 另外，无论是旧版本还是新版本， 
  // 当图象的宽度和高度超过当前 OpenGL 实现所支持的最大值时，也要进行缩放 
  {
    GLint max;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
    if (!power_of_two(width)
      || !power_of_two(height)
      || width > max
      || height > max)
    {
      const GLint new_width = 256;
      const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形 
      GLint new_line_bytes, new_total_bytes;
      GLubyte* new_pixels = 0;

      // 计算每行需要的字节数和总字节数 
      new_line_bytes = new_width * 3;
      while (new_line_bytes % 4 != 0)
        ++new_line_bytes;
      new_total_bytes = new_line_bytes * new_height;

      // 分配内存 
      new_pixels = (GLubyte*)malloc(new_total_bytes);
      if (new_pixels == 0)
      {
        free(pixels);
        fclose(pFile);
        return 0;
      }

      // 进行像素缩放 
      gluScaleImage(GL_RGB,
        width, height, GL_UNSIGNED_BYTE, pixels,
        new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

      // 释放原来的像素数据，把 pixels 指向新的像素数据，并重新设置 width 和 height 
      free(pixels);
      pixels = new_pixels;
      width = new_width;
      height = new_height;
    }
  }

  // 分配一个新的纹理编号 
  glGenTextures(1, &texture_ID);
  if (texture_ID == 0)
  {
    free(pixels);
    fclose(pFile);
    return 0;
  }

  // 绑定新的纹理，载入纹理并设置纹理参数 
  // 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复 
  glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)& last_texture_ID);
  glBindTexture(GL_TEXTURE_2D, texture_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
    GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
  glBindTexture(GL_TEXTURE_2D, last_texture_ID);

  // 之前为 pixels 分配的内存可在使用 glTexImage2D 以后释放 
  // 因为此时像素数据已经被 OpenGL 另行保存了一份（可能被保存到专门的图形硬件中） 
  free(pixels);
  return texture_ID;
}

//     //////////////////////////////////////////////// MY test//////////////////////////////////////////////////
//
#define SUB 20
#if SUB ==1
//第一课 
///////////////////////////////
void myDisplay(void)
{
glClear(GL_COLOR_BUFFER_BIT);
glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
glFlush();
}
#elif SUB== 2
// 第二课  几何图形绘制

const int n = 20;
const GLfloat R = 0.5f;
const GLfloat Pi = 3.1415926536f;
void myDisplay(void)
{

  int i;
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_POLYGON);//GL_POINTS， GL_LINES， GL_LINE_STRIP，GL_LINE_LOOP， GL_TRIANGLES，GL_TRIANGLE_STRIP，GL_TRIANGLE_FAN ,GL_POLYGON
  for (i = 0; i < n; i++)
  {
    GLdouble cosx = cos(2 * Pi * i / n);
    GLfloat x = R * cosx;
    GLfloat y = R * sin(2 * Pi * i / n);
    glVertex2f(x, y);
  }
  glEnd();
  glFlush();
}
#elif SUB== 3
//五角形
#include <math.h> 
const GLfloat Pi = 3.1415926536f;
void myDisplay(void) 
{     
  GLfloat a = 1 / (2-2*cos(72*Pi/180));    
  GLfloat bx = a * cos(18 * Pi/180);     
  GLfloat by = a * sin(18 * Pi/180);    
  GLfloat cy = -a * cos(18 * Pi/180);    
  GLfloat         PointA[2] = { 0, a }, 
PointB[2] = { bx, by }, 
PointC[2] = { 0.5, cy },
PointD[2] = { -0.5, cy },
PointE[2] = { -bx, by };

glClear(GL_COLOR_BUFFER_BIT);     // 按照 A->C->E->B->D->A 的顺序，可以一笔将五角星画出     
glBegin(GL_LINE_LOOP);         
glVertex2fv(PointA);        
glVertex2fv(PointC);         
glVertex2fv(PointE);        
glVertex2fv(PointB);        
glVertex2fv(PointD);    
glEnd();    
glFlush();
}

#elif SUB== 4
/* 由于 OpenGL 默认坐标值只能从-1 到 1，（可以修改，但方法留到以后讲） 所以我们设置一个因子 factor，把所有的坐标值等比例缩小， 这样就可以画出更多个正弦周期 试修改 factor 的值，观察变化情况 */ 
#include <math.h> 
const GLfloat factor = 0.1f; 
void myDisplay(void) 
{    
  GLfloat x; 
  glClear(GL_COLOR_BUFFER_BIT); 
  glBegin(GL_LINES);       
  glVertex2f(-1.0f, 0.0f);      
  glVertex2f(1.0f, 0.0f);        // 以上两个点可以画 x轴        
  glVertex2f(0.0f, -1.0f);        
  glVertex2f(0.0f, 1.0f);        // 以上两个点可以画 y 轴    
  glEnd();    
  glBegin(GL_LINE_STRIP);    
  for(x=-1.0f/factor; x<1.0f/factor; x+=0.01f)    
  {         glVertex2f(x*factor, sin(x)*factor);     }    
  glEnd();     
  glFlush(); 
} 
#elif SUB== 5
//点的大小默认为 1 个像素，但也可以改变之。改变的命令为 glPointSize，其函数原型如下： void glPointSize(GLfloat size); size 必须大于 0.0f，默认值为 1.0f，单位为“像素”。 注意：对于具体的 OpenGL 实现，点的大小都有个限度的，如果设置的 size 超过最大值，则设置可能会有 问题。 例子： 
void myDisplay(void) 
{ 
  glClear(GL_COLOR_BUFFER_BIT);   
  glPointSize(5.0f);    
  glBegin(GL_POINTS);      
  glVertex2f(0.0f, 0.0f);      
  glVertex2f(0.5f, 0.5f);    
  glEnd();   
  glFlush();
 }
#elif SUB== 6
//划线
void myDisplay(void) 
{ 
  glClear(GL_COLOR_BUFFER_BIT);  
  glEnable(GL_LINE_STIPPLE);   //虚线模式
  glLineStipple(2, 0x0F0F);     //设置虚线样式
glLineWidth(10.0f);   //设置线宽
glBegin(GL_LINES);     
glVertex2f(0.0f, 0.0f);      
glVertex2f(0.5f, 0.5f);  
glEnd();    
glFlush(); 
}
#elif SUB== 7
//关于多边形，有两个面，可以glPolygonMode(GL_FRONT/(GL_BACK, GL_FILL/ GL_LINE)设置不同的填充方式；
//glFrontFace可以反转正反面设置； CCW 方向为“正面”，CCW 即 CounterClockWise，逆时针  ；设置 CW方向为“正面”，CW 即 ClockWise，顺时针 
//一般约定为“顶点以逆时针顺序出现在屏幕上的面”为“正面”，另一个面即成为“反面”。
/*在三维空间中，一个多边形虽然有两个面，但我们无法看见背面的那些多边形，而一些多边形虽然是正面的，但被其他多边形所遮挡。如果将无法看见的多边形和可见的多边形同等对待，无疑会降低我们处理图 形的效率。
在这种时候，可以将不必要的面剔除。 首先，使用 glEnable(GL_CULL_FACE); 来启动剔除功能（使用 glDisable(GL_CULL_FACE)可以关闭之） 
然后，使用 glCullFace 来进行剔除。 glCullFace 的参数可以是 GL_FRONT，GL_BACK 或者 GL_FRONT_AND_BACK，分别表示剔除正面、剔 除反面、剔除正反两面的多边形。 
注意：剔除功能只影响多边形，而对点和直线无影响。例如，使用 glCullFace(GL_FRONT_AND_BACK) 后，所有的多边形都将被剔除，所以看见的就只有点和直线。
*/
void myDisplay(void) {
  glClear(GL_COLOR_BUFFER_BIT);    
  glPolygonMode(GL_FRONT, GL_FILL); // 设置正面为填充模式     
  glPolygonMode(GL_BACK, GL_LINE);  // 设置反面为线形模式     
  glFrontFace(GL_CCW);              // 设置逆时针方向为正面     
  glBegin(GL_POLYGON);              // 按逆时针绘制一个正方形，在左下方 
  glVertex2f(-0.5f, -0.5f);         
  glVertex2f(0.0f, -0.5f);        
  glVertex2f(0.0f, 0.0f);        
  glVertex2f(-0.5f, 0.0f);     
  glEnd();     glBegin(GL_POLYGON);              // 按顺时针绘制一个正方形，在右上方        
  glVertex2f(0.0f, 0.0f);         glVertex2f(0.0f, 0.5f);       
  glVertex2f(0.5f, 0.5f);         glVertex2f(0.5f, 0.0f);    
  glEnd();     glFlush(); 
} 
#elif SUB== 8
//多边形镂空样式
/*直线可以被画成虚线，而多边形则可以进行镂空。 首先，使用 glEnable(GL_POLYGON_STIPPLE);来启动镂空模式（使用 glDisable(GL_POLYGON_STIPPLE) 可以关闭之）。 然后，使用 glPolygonStipple 来设置镂空的样式。 void glPolygonStipple(const GLubyte *mask); 其中的参数 mask 指向一个长度为 128 字节的空间，它表示了一个 32*32 的矩形应该如何镂空。其中：第一 个字节表示了最左下方的从左到右（也可以是从右到左，这个可以修改）8 个像素是否镂空（1 表示不镂空， 显示该像素；0 表示镂空，显示其后面的颜色），最后一个字节表示了最右上方的 8 个像素是否镂空。 */
//用画图程序创建32*32的单色位图
void myDisplay(void) {    
static GLubyte Mask[128];     FILE *fp;     fp = fopen("mask.bmp", "rb");    
if( !fp )         exit(0);     
if( fseek(fp, -(int)sizeof(Mask), SEEK_END) )         exit(0);     
if( !fread(Mask, sizeof(Mask), 1, fp) )        exit(0);    
fclose(fp);     glClear(GL_COLOR_BUFFER_BIT);   
glEnable(GL_POLYGON_STIPPLE);     glPolygonStipple(Mask);     glRectf(-0.5f, -0.5f, 0.0f, 0.0f);  // 在左下方绘制一个有镂空效果的正方形     
glDisable(GL_POLYGON_STIPPLE);     glRectf(0.0f, 0.0f, 0.5f, 0.5f);    // 在右上方绘制一个无镂空效果的正方形     
glFlush(); } 

#elif SUB== 9
//颜色
void myDisplay(void) 
{
  glClearColor(1.0f, 0.0f, 0.0f, 0.0f);//指定清屏颜色
  glClear(GL_COLOR_BUFFER_BIT);    
glShadeModel(GL_SMOOTH);   // 设置颜色填充方式。平滑方式，这也是默认方式 glShadeModel(GL_FLAT);     // 单色方式 
glColor3f(0.0f, 1.0f, 1.0f);    
glRectf(-1.0f, 0.5f, 0.5f, 1.0f);   
//

glBegin(GL_TRIANGLE_FAN);     
glColor3f(1.0f, 1.0f, 1.0f);     glVertex2f(0.0f, 0.0f);    
for (int i = 0; i <= 8; ++i) {
  glColor3f(i & 0x04, i & 0x02, i & 0x01);
  glVertex2f(cos(i * Pi / 4), sin(i * Pi / 4));
}     glEnd();
//
glFlush(); }
#elif SUB== 10
/*
1、模型变换和视图变换 
由于模型和视图的变换都通过矩阵运算来实现，在进行变换前，应先设置当前操作的矩阵为“模型 视图矩阵”。
设置的方法是以 GL_MODELVIEW 为参数调用 glMatrixMode 函数，像这样： glMatrixMode(GL_MODELVIEW); 
通常，我们需要在进行变换前把当前矩阵设置为单位矩阵。这也只需要一行代码： glLoadIdentity();

 然后，就可以进行模型变换和视图变换了。进行模型和视图变换，主要涉及到三个函数： 
 glTranslate*，把当前矩阵和一个表示移动物体的矩阵相乘。三个参数分别表示了在三个坐标上的 位移值。 
 glRotate*，把当前矩阵和一个表示旋转物体的矩阵相乘。物体将绕着(0,0,0)到(x,y,z)的直线以逆时 针旋转，参数 angle 表示旋转的角度。 
 glScale*，把当前矩阵和一个表示缩放物体的矩阵相乘。x,y,z 分别表示在该方向上的缩放比例。
 ***********************
 gluLookAt改变观察者的位置
 ***********************
 2、投影变换
 投影变换就是定义一个可视空间，可视空间以外的物体不会被绘制到屏幕上。（注意，从现在起，坐标可以不再是-1.0 到 1.0 了！）
 glMatrixMode(GL_PROJECTION);
 要在进行变换前把当前矩阵设置为单位矩阵。 glLoadIdentity();
 透视投影所产生的结果类似于照片，有近大远小的效果，比如在火车头内向前照一个铁轨的照片，两条铁 轨似乎在远处相交了。 使用 glFrustum 函数可以将当前的可视空间设置为透视投影空间
 正投影相当于在无限远处观察得到的结果，它只是一种理想状态。但对于计算机来说，使用正投影有可能 获得更好的运行速度。使用 glOrtho 函数可以将当前的可视空间设置为正投影空间
 如果绘制的图形空间本身就是二维的，可以使用 gluOrtho2D。他的使用类似于 glOrgho。 
 3、视口变换 
 当一切工作已经就绪，只需要把像素绘制到屏幕上了。这时候还剩最后一个问题：应该把像素绘
制到窗口的哪个区域呢？通常情况下，默认是完整的填充整个窗口，但我们完全可以只填充一半。 
使用 glViewport 来定义视口。其中前两个参数定义了视口的左下脚（0,0 表示最左下方），后两个 参数分别是宽度和高度。
4、矩阵堆栈
当我们需要保存时，
调用 glPushMatrix 函数，它相当于把矩阵（相当于盘子）放到堆栈上。当需要恢复最近一次的保 存时，调用 glPopMatrix函数，它相当于把矩阵从堆栈上取下。
OpenGL 规定堆栈的容量至少可以 容纳 32 个矩阵，某些 OpenGL 实现中，堆栈的容量实际上超过了 32 个。因此不必过于担心矩阵 的容量问题。 
通常，用这种先保存后恢复的措施，比先变换再逆变换要更方便，更快速。 
注意：模型视图矩阵和投影矩阵都有相应的堆栈。使用 glMatrixMode 来指定当前操作的究竟是模 型视图矩阵还是投影矩阵。 

 
 */

 /*：本课开始，我们正式进入了三维的 OpenGL 世界。 OpenGL 通过矩阵变换来把三维物体转变为二维图象，进而在屏幕上显示出来。为了指定当前操 作的是何种矩阵，我们使用了函数 glMatrixMode。 我们可以移动、旋转观察点或者移动、旋转物体，使用的函数是 glTranslate*和 glRotate*。 我们可以缩放物体，使用的函数是 glScale*。 我们可以定义可视空间，这个空间可以是“正投影”的（使用 glOrtho 或 gluOrtho2D），也可以是“透 视投影”的（使用 glFrustum 或 gluPerspective）。 我们可以定义绘制到窗口的范围，使用的函数是 glViewport。 矩阵有自己的“堆栈”，方便进行保存和恢复。这在绘制复杂图形时很有帮助。使用的函数是 glPushMatrix和 glPopMatrix。
  */

 // 太阳、地球和月亮 
 // 假设每个月都是 30 天 
 // 一年 12 个月，共是 360 天 
static int day = 30; // day 的变化：从 0 到 359 
void myDisplay(void)
{/*还有一个需要注意的细节：OpenGL 把三维坐标中的物体绘制到二维屏幕，绘制的顺序是按照代 码的顺序来进行的。因此后绘制的物体会遮住先绘制的物体，即使后绘制的物体在先绘制的物体 的“后面”也是如此。使用深度测试可以解决这一问题。使用的方法是：1、以 GL_DEPTH_TEST 为参数调用 glEnable 函数，启动深度测试。2、在必要时（通常是每次绘制画面开始时），清空深 度缓冲，即：glClear(GL_DEPTH_BUFFER_BIT);其中，glClear(GL_COLOR_BUFFER_BIT)与 glClear(GL_DEPTH_BUFFER_BIT)可以合并写为： glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 且后者的运行速度可能比前者快。 */
 
  glEnable(GL_DEPTH_TEST);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(75, 1, 10000, 400000000);//设置可视空间
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0,0, 400000000, 0, 0, 0, 0, 1, 0); //gluLookAt(0, -200000000, 200000000, 0, 0, 0, 0, 0, 1);

  // 绘制红色的“太阳” 
  glColor3f(1.0f, 0.0f, 0.0f);
  glutSolidSphere(69600000, 20, 20);//绘制球体
  // 绘制蓝色的“地球” 
  glColor3f(0.0f, 0.0f, 1.0f);
  glRotatef(day / 360.0 * 360.0, 0.0f, 0.0f, -1.0f);
  glTranslatef(150000000, 0.0f, 0.0f);
  glutSolidSphere(15945000, 20, 20);
  // 绘制黄色的“月亮” 
  glColor3f(1.0f, 1.0f, 0.0f);
  glRotatef(day / 30.0 * 360.0 - day / 360.0 * 360.0, 0.0f, 0.0f, -1.0f);
  glTranslatef(38000000, 0.0f, 0.0f);
  glutSolidSphere(4345000, 20, 20);

  glFlush();
}
#elif SUB== 11

// 太阳、地球和月亮 
// 假设每个月都是 30 天 
// 一年 12 个月，共是 360 天 
static int day = 200; // day 的变化：从 0 到 359 
void myDisplay(void)
{
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(75, 1, 10000, 400000000);//设置可视空间
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 200000000, 200000000, 0, 0, 0, 0, 1, 0); //gluLookAt(0, -200000000, 200000000, 0, 0, 0, 0, 0, 1);

  // 绘制红色的“太阳” 
  glColor3f(1.0f, 0.0f, 0.0f);
  glutSolidSphere(69600000, 20, 20);//绘制球体
  // 绘制蓝色的“地球” 
  glColor3f(0.0f, 0.0f, 1.0f);
  glRotatef(day / 360.0 * 360.0, 0.0f, 0.0f, -1.0f);
  glTranslatef(150000000, 0.0f, 0.0f);
  glutSolidSphere(15945000, 20, 20);
  // 绘制黄色的“月亮” 
  glColor3f(1.0f, 1.0f, 0.0f);
  glRotatef(day / 30.0 * 360.0 - day / 360.0 * 360.0, 0.0f, 0.0f, -1.0f);
  glTranslatef(38000000, 0.0f, 0.0f);
  glutSolidSphere(4345000, 20, 20);

  glFlush();
  /****************************************************
   这里的内容照搬上一课的，只因为使用了双缓冲，补上最后这句
  *****************************************************/
  glutSwapBuffers();
}

void myIdle(void)
{
  /* 新的函数，在空闲时调用，作用是把日期往后移动一天并重新绘制，达到动画效果 */
  ++day;
  if (day >= 360)
    day = 0;
  myDisplay();
  _sleep(100);//由于使用cpu刷新频率很快，需要加延时，显卡的垂直同步也可以解决这个问题
}


#elif SUB== 12
/* 光照，需要指定顶点的法线向量，用于光的放射计算
 与指定点的颜色类似，法线向量一旦被指定，除非再指定新的法线向量，否则以后指定的所有顶点都将以这一向量作为自己的法线向量。
 使用 glColor* 函数可以指定颜 色，而使用 glNormal* 函数则可以指定法线向量。
 注意：
 使用 glTranslate*函数或者 glRotate*函数可以改变物体的外观，但法线向量并不会 随之改变。
 然而，使用 glScale*函数，对每一坐标轴进行不同程度的缩放，很有可能导 致法线向量的不正确，虽然 OpenGL 提供了一些措施来修正这一问题，但由此也带来了 各种开销。
 因此，在使用了法线向量的场合，应尽量避免使用 glScale*函数。即使使用， 也最好保证各坐标轴进行等比例缩放
 */
/*控制光源
在 OpenGL 中，仅仅支持有限数量的光源。使用 GL_LIGHT0 表示第 0 号光源，GL_LIGHT1 表示 第1号光源，依次类推，OpenGL至少会支持8个光源，即GL_LIGHT0到GL_LIGHT7。使用glEnable 函数可以开启它们。例如，glEnable(GL_LIGHT0);可以开启第 0 号光源。使用 glDisable 函数则可 以关闭光源
每一个光源都可以设置其属性，这一动作是通过 glLight*函数完成的。glLight*函数具有三个参数， 第一个参数指明是设置哪一个光源的属性，第二个参数指明是设置该光源的哪一个属性，第三个 参数则是指明把该属性值设置成多少。
关于法线向量。球体表面任何一点的法线向量，就是球心到该点的向量。如果使用 glutSolidSphere 函数来绘制球体，则该函数会自动的指定这些法线向量，不必再手工指出。如果是自己指定若干 的顶点来绘制一个球体，则需要自己指定法线响亮。
*/
/*控制材质
材质与光源相似，也需要设置众多的属性。不同的是，光源是通过 glLight*函数来设置 的，而材质则是通过 glMaterial*函数来设置的。 
glMaterial*函数有三个参数。第一个参数表示指定哪一面的属性。可以是 GL_FRONT、 GL_BACK 或者 GL_FRONT_AND_BACK。分别表示设置“正面”“背面”的材质，或者两 面同时设置。（关于“正面”“背面”的内容需要参看前些课程的内容）第二、第三个参数与 glLight*函数的第二、三个参数作用类似。
*/
/*选择光照模型
，光照模型包括四个部分的内容：全局环境光线（即那些充分散射，无法分清究竟来自哪个光源的光线）的强度、观察点位置是在较近位置还是在无限远处、物体正面与背面是否分别计算光 照、镜面颜色（即 GL_SPECULAR 属性所指定的颜色)的计算是否从其它光照计算中分离出来， 并在纹理操作以后在进行应用。 
以上四方面的内容都通过同一个函数 glLightModel*来进行设置。该函数有两个参数，第一个表示 要设置的项目，第二个参数表示要设置成的值。
*/
/*
，OpenGL 默认是关闭光照处理的。要打开光照处理功能，使 用下面的语句： glEnable(GL_LIGHTING); 要关闭光照处理功能，使用 glDisable(GL_LIGHTING);即可。
*/
//#include <gl/glut.h> 


static GLfloat angle = 0.0f;

void myDisplay(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // 创建透视效果视图     
  glMatrixMode(GL_PROJECTION);     
  glLoadIdentity();     
  gluPerspective(90.0f, 1.0f, 1.0f, 20.0f);     
  glMatrixMode(GL_MODELVIEW);     
  glLoadIdentity();     
  gluLookAt(0.0, 5.0, -10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 

  // 定义太阳光源，它是一种白色的光源     
  {     
    GLfloat sun_light_position[] = {0.0f, 0.0f, 0.0f, 1.0f};     
    GLfloat sun_light_ambient[]  = {0.0f, 0.0f, 0.0f, 1.0f};     
    GLfloat sun_light_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};     
    GLfloat sun_light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f}; 

  glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);     
  glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);     
  glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);     
  glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);

  glEnable(GL_LIGHT0);     
  glEnable(GL_LIGHTING);     
  glEnable(GL_DEPTH_TEST);
}

// 定义太阳的材质并绘制太阳     
  {         
    GLfloat sun_mat_ambient[]  = {0.0f, 0.0f, 0.0f, 1.0f};         
  GLfloat sun_mat_diffuse[]  = {0.0f, 0.0f, 0.0f, 1.0f};         
  GLfloat sun_mat_specular[] = {1.0f, 1.0f, 0.0f, 1.0f};         
  GLfloat sun_mat_emission[] = {1.0f, 0.6f, 0.6f, 1.0f};         
  GLfloat sun_mat_shininess  = 0.0f; 

glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);         
glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);         
glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);         
glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);         
glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);

glutSolidSphere(2.0, 40, 32);     
  }

// 定义地球的材质并绘制地球     
  {         
    GLfloat earth_mat_ambient[]  = {0.5f, 0.5f, 1.0f, 1.0f};         
    GLfloat earth_mat_diffuse[]  = {0.5f, 0.5f, 1.0f, 1.0f};         
    GLfloat earth_mat_specular[] = {0.8f, 0.8f, 1.0f, 1.0f};         
    GLfloat earth_mat_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};         
    GLfloat earth_mat_shininess  = 5.0f; 

glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_ambient);         
glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_diffuse);         
glMaterialfv(GL_FRONT, GL_SPECULAR, earth_mat_specular);         
glMaterialfv(GL_FRONT, GL_EMISSION, earth_mat_emission);         
glMaterialf(GL_FRONT, GL_SHININESS, earth_mat_shininess);

glRotatef(angle, 0.0f, -1.0f, 0.0f);         
glTranslatef(5.0f, 0.0f, 0.0f);         
glutSolidSphere(2.0, 40, 32);     
  }

glutSwapBuffers(); 
} 
void myIdle(void) {
  angle += 1.0f;    
  if (angle >= 360.0f)         
    angle = 0.0f;
  myDisplay();
  _sleep(100);
}


#elif SUB== 13
/*显示列表，用于存放需要重复绘制的内容，减少cpu开销
使用显示列表一般有四个步骤：分配显示列表编号、创建显示列表、调用显示列表、销毁显示列 表。
使用 glGenLists 函数来自动分 配一个没有使用的显示列表编号。 glGenLists 函数有一个参数 i，表示要分配 i 个连续的未使用的显示列表编号。返回的是分配的若 干连续编号中最小的一个。
可以使用 glIsList 函数判断一个编号是否已经被用作显示列表。
使用 glNewList 开始装 入，使用 glEndList 结束装入。显示列表只能装入 一部分OpenGL 函数，而不能装入其它内容。
使用 glCallList 函数可以调用一个显示列表，
使用 glCallLists 函数可以调用一系列的显示列表，在使用该函数前，需要用 glListBase 函数来设置一个偏移量
注：“调用显示列表”这个动作本身也可以被装在另一个显示列表中。
销毁显示列表可以回收资源。使用 glDeleteLists 来销毁一串编号连续的显示列表。

*/
#define ColoredVertex(c, v) do{ glColor3fv(c); glVertex3fv(v); }while(0) 

GLfloat angle = 0.0f;

void myDisplay(void)
{
  static int list = 0;
  if (list == 0)
  {
    // 如果显示列表不存在，则创建 
    /* GLfloat
        PointA[] = {-0.5, -5*sqrt(5)/48,  sqrt(3)/6},
        PointB[] = { 0.5, -5*sqrt(5)/48,  sqrt(3)/6},
        PointC[] = {   0, -5*sqrt(5)/48, -sqrt(3)/3},
        PointD[] = {   0, 11*sqrt(6)/48,          0}; */
        // 2007 年 4 月 27 日修改 
    GLfloat
      PointA[] = { 0.5f, -sqrt(6.0f) / 12, -sqrt(3.0f) / 6 },
      PointB[] = { -0.5f, -sqrt(6.0f) / 12, -sqrt(3.0f) / 6 },
      PointC[] = { 0.0f, -sqrt(6.0f) / 12,  sqrt(3.0f) / 3 },
      PointD[] = { 0.0f,   sqrt(6.0f) / 4,             0 };
    GLfloat
      ColorR[] = { 1, 0, 0 },
      ColorG[] = { 0, 1, 0 },
      ColorB[] = { 0, 0, 1 },
      ColorY[] = { 1, 1, 0 };

    list = glGenLists(1);
    glNewList(list, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    // 平面 ABC 
    ColoredVertex(ColorR, PointA);
    ColoredVertex(ColorG, PointB);
    ColoredVertex(ColorB, PointC);
    // 平面 ACD 
    ColoredVertex(ColorR, PointA);
    ColoredVertex(ColorB, PointC);
    ColoredVertex(ColorY, PointD);
    // 平面 CBD 
    ColoredVertex(ColorB, PointC);
    ColoredVertex(ColorG, PointB);
    ColoredVertex(ColorY, PointD);
    // 平面 BAD 
    ColoredVertex(ColorG, PointB);
    ColoredVertex(ColorR, PointA);
    ColoredVertex(ColorY, PointD);
    glEnd();
    glEndList();

    glEnable(GL_DEPTH_TEST);
  }
  // 已经创建了显示列表，在每次绘制正四面体时将调用它 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  glRotatef(angle, 1, 0.5, 0);
  glCallList(list);
  glPopMatrix();
  glutSwapBuffers();
}

void myIdle(void)
{
  ++angle;
  if (angle >= 360.0f)
    angle = 0.0f;
  myDisplay();
  _sleep(40);
}

#elif SUB== 14
/*混合
通常可以用来实现半透明。
要使用 OpenGL 的混合功能，只需要调用：glEnable(GL_BLEND);即可。 要关闭 OpenGL 的混合功能，只需要调用：glDisable(GL_BLEND);即可。
OpenGL 会把源颜色和目标颜色各自取出，并乘以一个系数（源颜色乘以的系数称为“源 因子”，目标颜色乘以的系数称为“目标因子”），再混合
源因子和目标因子是可以通过 glBlendFunc 函数来进行设置的。glBlendFunc 有两个参数， 前者表示源因子，后者表示目标因子。

*/
void myDisplay(void)
{
  glClear(GL_COLOR_BUFFER_BIT);

  glEnable(GL_BLEND);
  /*GL_ZERO：     表示使用 0.0 作为因子，实际上相当于不使用这种颜色参与混合运算。 
  GL_ONE：      表示使用 1.0 作为因子，实际上相当于完全的使用了这种颜色参与混合运 算。 
  GL_SRC_ALPHA：表示使用源颜色的 alpha 值来作为因子。 
  GL_DST_ALPHA：表示使用目标颜色的 alpha 值来作为因子。
  GL_ONE_MINUS_SRC_ALPHA：表示用 1.0 减去源颜色的 alpha 值来作为因子。 
  GL_ONE_MINUS_DST_ALPHA：表示用 1.0 减去目标颜色的 alpha 值来作为因子。 */
  //glBlendFunc(GL_ONE, GL_ZERO);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glBlendFunc(GL_ONE, GL_ONE);
  glColor4f(1, 0, 0, 0.5);
  glRectf(-1, -1, 0.5, 0.5);
  glColor4f(0, 1, 0, 0.5);
  glRectf(-0.5, -0.5, 1, 1);

  glutSwapBuffers();
}
#elif SUB== 15
/*三维混合
总结起来，绘制顺序就是：首先绘制所有不透明的物体。如果两个物体都是不透明的，
则谁先谁后都没有关系。然后，将深度缓冲区设置为只读。接下来，绘制所有半透明的物体。
如果两个物体都是半透明的，则谁先谁后只需要根据自己的意愿（注意了，先绘 制的将成为“目标颜色”，后绘制的将成为“源颜色”，所以绘制的顺序将会对结果造成一 些影响）。
最后，将深度缓冲区设置为可读可写形式。
调用 glDepthMask(GL_FALSE); 可将深度缓冲区设置为只读形式 。 调用glDepthMask(GL_TRUE);可将深度缓冲区设置为可读可写形式。
*/
void setLight(void) {
  static const GLfloat light_position[] = { 1.0f, 1.0f, -1.0f, 1.0f };
  static const GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
  static const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  static const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
} 
//每一个球体颜色不同。所以它们的材质也都不同。这里用一个函数来设置材质。 
void setMatirial(const GLfloat mat_diffuse[4], GLfloat mat_shininess) {
  static const GLfloat mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
  static const GLfloat mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
  glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
} 
//有了这两个函数，我们就可以根据前面的知识写出整个程序代码了。这里只给出了绘制的部分， 其它部分大家可以自行完成。 
void myDisplay(void) {     // 定义一些材质颜色
  const static GLfloat red_color[] = {1.0f, 0.0f, 0.0f, 1.0f};
  const static GLfloat green_color[] = {0.0f, 1.0f, 0.0f, 0.3333f};
  const static GLfloat blue_color[] = {0.0f, 0.0f, 1.0f, 0.5f}; 

// 清除屏幕
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

// 启动混合并设置混合因子
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

// 设置光源
setLight(); 

// 以(0, 0, 0.5)为中心，绘制一个半径为.3 的不透明红色球体（离观察者最远）
setMatirial(red_color, 30.0);
glPushMatrix();
glTranslatef(0.0f, 0.0f, 0.5f);
glutSolidSphere(0.3, 30, 30);
glPopMatrix(); 

// 下面将绘制半透明物体了，因此将深度缓冲设置为只读
glDepthMask(GL_FALSE); 

// 以(0.2, 0, -0.5)为中心，绘制一个半径为.2 的半透明蓝色球体（离观察者最近）
setMatirial(blue_color, 30.0);
glPushMatrix();
glTranslatef(0.2f, 0.0f, -0.5f);
glutSolidSphere(0.2, 30, 30);
glPopMatrix(); 

// 以(0.1, 0, 0)为中心，绘制一个半径为.15 的半透明绿色球体（在前两个球体之间）
setMatirial(green_color, 30.0);
glPushMatrix();
glTranslatef(0.1, 0, 0);
glutSolidSphere(0.15, 30, 30);
glPopMatrix(); 

// 完成半透明物体的绘制，将深度缓冲区恢复为可读可写的形式
glDepthMask(GL_TRUE); 

  glutSwapBuffers();
}

#elif SUB== 16
/*绘制bmp像素图
glReadPixels：读取一些像素。
glDrawPixels：绘制一些像素。
glCopyPixels：复制一些像素。
3.2 解决 OpenGL 常用的 RGB 像素数据与 BMP 文件的 BGR 像素数据顺序不一致问题： GL_RGB 
消除 BMP 文件中“对齐”带来的影响修改对齐：int alignment = 4; glPixelStorei(GL_UNPACK_ALIGNMENT, alignment); 
*/

/*在 glDrawPixels 中，不必显式的指定绘制的位置，这是因为绘制的位置是由另一个函数 glRa
sterPos*来指定的
OpenGL 在绘制像素之前，可以对像素进行若干处理。最常用的可能就是对整个像素图象进行放 大/缩小。
使用 glPixelZoom 来设置放大/缩小的系数，该函数有两个参数，分别是水平方向系数和 垂直方向系数*/
#define FileName "Bliss.bmp" //24bit bmp

static GLint    ImageWidth;
static GLint    ImageHeight;
static GLint    PixelLength;
static GLubyte* PixelData;

//#include <stdio.h> 
//#include <stdlib.h> 

void display(void)
{
  // 清除屏幕并不必要 
  // 每次绘制时，画面都覆盖整个屏幕 
  // 因此无论是否清除屏幕，结果都一样 
  // glClear(GL_COLOR_BUFFER_BIT); 

  // 绘制像素 
  glDrawPixels(ImageWidth, ImageHeight,
    GL_BGR_EXT, GL_UNSIGNED_BYTE, PixelData);

  // 完成绘制 
  glutSwapBuffers();
}

int main(int argc, char* argv[])
{
  // 打开文件 
  FILE* pFile = fopen("dummy.bmp", "rb");
  if (pFile == 0)
    exit(0);

  // 读取图象的大小信息 
  fseek(pFile, 0x0012, SEEK_SET);
  fread(&ImageWidth, sizeof(ImageWidth), 1, pFile);
  fread(&ImageHeight, sizeof(ImageHeight), 1, pFile);

  // 计算像素数据长度 
  PixelLength = ImageWidth * 3;
  while (PixelLength % 4 != 0)
    ++PixelLength;
  PixelLength *= ImageHeight;

  // 读取像素数据 
  PixelData = (GLubyte*)malloc(PixelLength);
  if (PixelData == 0)
    exit(0);

  fseek(pFile, 54, SEEK_SET);
  fread(PixelData, PixelLength, 1, pFile);

  // 关闭文件 
  fclose(pFile);

  // 初始化 GLUT 并运行 
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(ImageWidth, ImageHeight);
  glutCreateWindow(FileName);
  glutDisplayFunc(&display);
  glutMainLoop();

  // 释放内存 
  // 实际上，glutMainLoop 函数永远不会返回，这里也永远不会到达 
  // 这里写释放内存只是出于一种个人习惯 
  // 不用担心内存无法释放。在程序结束时操作系统会自动回收所有内存 
  free(PixelData);

  return 0;
}
#elif SUB== 17
/*。使用 glCopyPixels相比以通过 glReadPixels 和 glDrawPixels 组合
直接从像素数据复制出新的像素数据，避免了多余的数据的格式转换，并且也可能减少一些数据复制操
作（因为数据可能直接由显卡负责复制，不需要经过主内存），因此效率比较高。 */
void myDisplay(void)
{
  // 清除屏幕 
  glClear(GL_COLOR_BUFFER_BIT);

  // 绘制 
  glBegin(GL_TRIANGLES);
  glColor3f(1.0f, 0.0f, 0.0f);    glVertex2f(0.0f, 0.0f);
  glColor3f(0.0f, 1.0f, 0.0f);    glVertex2f(1.0f, 0.0f);
  glColor3f(0.0f, 0.0f, 1.0f);    glVertex2f(0.5f, 1.0f);
  glEnd();
  glPixelZoom(-0.5f, -0.5f);
  glRasterPos2i(1, 1);
  glCopyPixels(WindowWidth / 2, WindowHeight / 2,
    WindowWidth / 2, WindowHeight / 2, GL_COLOR);

  // 完成绘制，并抓取图象保存为 BMP 文件 
  glutSwapBuffers();
  grab();
}
#elif SUB== 18
/*纹理
。OpenGL 纹理映射功能支持将一些像素数据经过变换（即使是比较不规则的变 换）将其附着到各种形状的多边形表面。纹理映射功能十分强大，利用它可以实现目前计算机动
画中的大多数效果，但是它也很复杂
。OpenGL 支持一维
纹理、二维纹理和三维纹理，这里我们仅介绍二维纹理。可以使用以下语句来启用和禁用二维纹理：
    glEnable(GL_TEXTURE_2D);  // 启用二维纹理
    glDisable(GL_TEXTURE_2D); // 禁用二维纹理 
    
    使用纹理前，还必须载入纹理。利用 glTexImage2D 函数可以载入一个二维的纹理，

    */
/*
OpenGL 在以前的很多版本中，限制纹理的大小必须是 2 的整数次方，即纹理的宽度和高度只能是 16, 32, 64, 128, 256 等值。尽量使用大小
为 2 的整数次方的纹理，当这个要求无法满足时，使用 gluScaleImage 函数把图象缩放至所指定的大小（在
后面的例子中有用到）。另外，无论旧版本还是新版本，都限制了纹理大小的最大值，例如，某 OpenGL
实现可能要求纹理最大不能超过 1024*1024。可以使用如下的代码来获得 OpenGL 所支持的最大纹理： 
GLint max; 
glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max); 
 在很长一段时间内，很多图形程序都喜欢使用 256*256 大小的纹理，不仅因为 256 是 2 的整数次方，也因
为某些硬件可以使用 8 位的整数来表示纹理坐标，2 的 8 次方正好是 256，这一巧妙的组合为处理纹理坐标
时的硬件优化创造了一些不错的条件。 
 
这样 max的值就是当前 OpenGL 实现中所支持的最大纹理。
举个例子，如果有一幅大小为 width*height，格式为 Windows 系统中使用最普遍的 24 位 BGR，保存在 pix
els 中的像素图象。则把这样一幅图象载入为纹理可使用以下代码：
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pix
els); 
*/
/*纹理坐标
我们先来回忆一下之前学过的一点内容：
当我们绘制一个三角形时，只需要指定三个顶点的颜色。三角形中其它各点的颜色不需要我们指定，这些
点的颜色是 OpenGL 自己通过计算得到的。
在我们学习 OpneGL 光照时，法线向量、材质的指定，都是只需要在顶点处指定一下就可以了，其它地方
的法线向量和材质都是 OpenGL 自己通过计算去获得
纹理的使用方法也与此类似。只要指定每一个顶点在纹理图象中所对应的像素位置，OpenGL 就会自动计
算顶点以外的其它点在纹理图象中所对应的像素位置。 

二维纹理为例，规定纹理最左下角的坐标为(0, 0)，最右上角的坐标为(1, 1)，于是纹理中的每一个像素的位
置都可以用两个浮点数来表示（三维纹理会用三个浮点数表示，一维纹理则只用一个即可）。
使用 glTexCoord*系列函数来指定纹理坐标。这些函数的用法与使用 glVertex*系列函数来指定顶点坐标十
分相似。例如：glTexCoord2f(0.0f, 0.0f);指定使用(0, 0)纹理坐标

当我们用一个坐标表示顶点在三维空间的位置时，可以使用 glRotate*等函数来对坐标进行转换。 纹理坐标也可以进行这种转换。
只要使用 glMatrixMode(GL_TEXTURE);，就可以切换到纹理矩阵 （另外还有透视矩阵 GL_PROJECTION 和模型视图矩阵 GL_MODELVIEW，详细情况在第五课有 讲述），
然后 glRotate*，glScale*，glTranslate*等操作矩阵的函数就可以用来处理“对纹理坐标进 行转换”的工作了。在简单应用中，可能不会对矩阵进行任何变换，这样考虑问题会比较简单。 
*/

/*
使用 glTexParameter*系列函数来设置纹理参数*/

/*纹理对象正是这样一种机制。我们可以把每一幅纹理（包括纹理的像素数据、纹理大小等信息，也包括了
前面所讲的纹理参数）放到一个纹理对象中，通过创建多个纹理对象来达到同时保存多幅纹理的目的。这
样一来，在第一次使用纹理前，把所有的纹理都载入，然后在绘制时只需要指明究竟使用哪一个纹理对象
就可以了。 
 
 使用纹理对象和使用显示列表有相似之处：使用一个正整数来作为纹理对象的编号。在使用前，可以调用
glGenTextures 来分配纹理对象。该函数有两种比较常见的用法：
GLuint texture_ID;
glGenTextures(1, &texture_ID); // 分配一个纹理对象的编号

或者：
GLuint texture_ID_list[5];
glGenTextures(5, texture_ID_list); // 分配 5 个纹理对象的编号


零是一个特殊的纹理对象编号，表示“默认的纹理对象”，在分配正确的情况下，glGenTextures 不会分配这
个编号。与 glGenTextures 对应的是 glDeleteTextures，用于销毁一个纹理对象。

在分配了纹理对象编号后，使用 glBindTexture 函数来指定“当前所使用的纹理对象”。然后就可以使用 glTe
xImage*系列函数来指定纹理像素、使用 glTexParameter*系列函数来指定纹理参数、使用 glTexCoord*系列
函数来指定纹理坐标了。如果不使用 glBindTexture 函数，那么 glTexImage*、glTexParameter*、glTexCoor
d*系列函数默认在一个编号为 0 的纹理对象上进行操作。
使用多个纹理对象，就可以使 OpenGL 同时保存多个纹理。在使用时只需要调用 glBindTexture 函数，在不
同纹理之间进行切换，而不需要反复载入纹理，因此动画的绘制速度会有非常明显的提升。
 */


/* 两个纹理对象的编号
 */
GLuint texGround;
GLuint texWall;
GLfloat angle = 0.0f;
void display(void)
{
  // 清除屏幕 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // 设置视角 
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(75, 1, 1, 21);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //glPushMatrix();
  gluLookAt(1, 5, 5, 0, 0, 0, 0, 0, 1);

  glRotatef(angle, 0,0, 1);
  //glPopMatrix();

  // 使用“地”纹理绘制土地 
  glBindTexture(GL_TEXTURE_2D, texGround);//使用纹理ground
  glBegin(GL_QUADS);//四边形，下面指定纹理的4个点和四边形的4个点,在16*16的四边形里填充5*5个纹理
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-8.0f, -8.0f, 0.0f);
  glTexCoord2f(0.0f, 5.0f); glVertex3f(-8.0f, 8.0f, 0.0f);
  glTexCoord2f(5.0f, 5.0f); glVertex3f(8.0f, 8.0f, 0.0f);
  glTexCoord2f(5.0f, 0.0f); glVertex3f(8.0f, -8.0f, 0.0f);
  glEnd();
  // 使用“墙”纹理绘制栅栏 
  glBindTexture(GL_TEXTURE_2D, texWall);
  glBegin(GL_QUADS); //12*1.5的四边形里填充5 * 1个纹理
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, -3.0f, 0.0f);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.0f, -3.0f, 1.5f);
  glTexCoord2f(5.0f, 1.0f); glVertex3f(6.0f, -3.0f, 1.5f);
  glTexCoord2f(5.0f, 0.0f); glVertex3f(6.0f, -3.0f, 0.0f);
  glEnd();

  // 旋转后再绘制一个 
  glRotatef(-90, 0, 0, 1);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, -3.0f, 0.0f);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.0f, -3.0f, 1.5f);
  glTexCoord2f(5.0f, 1.0f); glVertex3f(6.0f, -3.0f, 1.5f);
  glTexCoord2f(5.0f, 0.0f); glVertex3f(6.0f, -3.0f, 0.0f);
  glEnd();

  // 交换缓冲区，并保存像素数据到文件 
  glutSwapBuffers();
  //grab();
}
void myIdle(void)
{
  ++angle;
  if (angle >= 360.0f)
    angle = 0.0f;
  display();
  _sleep(40);
}
int main(int argc, char* argv[])
{
  // GLUT 初始化 
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(WindowWidth, WindowHeight);
  glutCreateWindow("test ");
  glutDisplayFunc(&display);
  glutIdleFunc(&myIdle);
  // 在这里做一些初始化 
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  texGround = load_texture("ground.bmp");
  texWall = load_texture("wall.bmp");

  // 开始显示 
  glutMainLoop();

  return 0;
}
#elif SUB== 19
/*片段测试
片断测试其实就是测试每一个像素，只有通过测试的像素才会被绘制，没有通过测试的像素则不进行绘制。
*/
/*
剪裁测试用于限制绘制区域。我们可以指定一个矩形的剪裁窗口，当启用剪裁测试后，只有在这个窗口之
内的像素才能被绘制，其它像素则会被丢弃。换句话说，无论怎么绘制，剪裁窗口以外的像素将不会被修
改。
可以通过下面的代码来启用或禁用剪裁测试：
glEnable(GL_SCISSOR_TEST);  // 启用剪裁测试
glDisable(GL_SCISSOR_TEST); // 禁用剪裁测试

可以通过下面的代码来指定一个位置在(x, y)，宽度为 width，高度为 height 的剪裁窗口。
glScissor(x, y, width, height)
注意，OpenGL 窗口坐标是以左下角为(0, 0)，右上角为(width, height)的，这与 Windows 系统窗口 有所不同。
 ;
*/
/*Alph
a 测试。当每个像素即将绘制时，如果启动了 Alpha 测试，OpenGL 会检查像素的 Alpha 值，只有Alpha 值
满足条件的像素才会进行绘制（严格的说，满足条件的像素会通过本项测试，进行下一种测试，只有所有
测试都通过，才能进行绘制），不满足条件的则不进行绘制*/
/*可以通过下面的代码来启用或禁用 Alpha 测试：
glEnable(GL_ALPHA_TEST);  // 启用 Alpha 测试 
glDisable(GL_ALPHA_TEST); // 禁用 Alpha 测试 


可以通过下面的代码来设置 Alpha 测试条件为“大于 0.5 则通过”：
glAlphaFunc(GL_GREATER, 0.5f);*/
/* 将当前纹理 BGR 格式转换为 BGRA 格式
 * 纹理中像素的 RGB 值如果与指定 rgb 相差不超过 absolute，则将 Alpha 设置为 0.0，否则设置为 1.0
 */
void texture_colorkey(GLubyte r, GLubyte g, GLubyte b, GLubyte absolute)
{
  GLint width, height;
  GLubyte* pixels = 0;

  // 获得纹理的大小信息 
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

  // 分配空间并获得纹理像素 
  pixels = (GLubyte*)malloc(width * height * 4);
  if (pixels == 0)
    return;
  glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pixels);

  // 修改像素中的 Alpha 值 
  // 其中 pixels[i*4], pixels[i*4+1], pixels[i*4+2], pixels[i*4+3] 
  //   分别表示第 i 个像素的蓝、绿、红、Alpha 四种分量，0 表示最小，255 表示最大 
  {
    GLint i;
    GLint count = width * height;
    for (i = 0; i < count; ++i)
    {
      if (abs(pixels[i * 4] - b) <= absolute
        && abs(pixels[i * 4 + 1] - g) <= absolute
        && abs(pixels[i * 4 + 2] - r) <= absolute)
        pixels[i * 4 + 3] = 0;
      else
        pixels[i * 4 + 3] = 255;
    }
  }

  // 将修改后的像素重新设置到纹理中，释放内存 
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
    GL_BGRA_EXT, GL_UNSIGNED_BYTE, pixels);
  free(pixels);
}
void myDisplay(void)
{
  static int initialized = 0;
  static GLuint texWindow = 0;
  static GLuint texPicture = 0;

  // 执行初始化操作，包括：读取相片，读取相框，将相框由 BGR 颜色转换为 BGRA，启用二维纹理 
  if (!initialized)
  {
    texPicture = load_texture("pic.bmp");
    texWindow = load_texture("window.bmp");
    glBindTexture(GL_TEXTURE_2D, texWindow);
    texture_colorkey(255, 255, 255, 10);

    glEnable(GL_TEXTURE_2D);

    initialized = 1;
  }

  // 清除屏幕 
  glClear(GL_COLOR_BUFFER_BIT);

  // 绘制相片，此时不需要进行 Alpha 测试，所有的像素都进行绘制 
  glBindTexture(GL_TEXTURE_2D, texPicture);
  glDisable(GL_ALPHA_TEST);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);     glVertex2f(-1.0f, -1.0f);
  glTexCoord2f(0, 1);     glVertex2f(-1.0f, 1.0f);
  glTexCoord2f(1, 1);     glVertex2f(1.0f, 1.0f);
  glTexCoord2f(1, 0);     glVertex2f(1.0f, -1.0f);
  glEnd();

  // 绘制相框，此时进行 Alpha 测试，只绘制不透明部分的像素 
  glBindTexture(GL_TEXTURE_2D, texWindow);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.5f);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);     glVertex2f(-1.0f, -1.0f);
  glTexCoord2f(0, 1);     glVertex2f(-1.0f, 1.0f);
  glTexCoord2f(1, 1);     glVertex2f(1.0f, 1.0f);
  glTexCoord2f(1, 0);     glVertex2f(1.0f, -1.0f);
  glEnd();

  // 交换缓冲 
  glutSwapBuffers();
}

#elif SUB== 20
/*模板测试是所有 OpenGL 测试中比较复杂的一种。 
 
首先，模板测试需要一个模板缓冲区，这个缓冲区是在初始化 OpenGL 时指定的。如果使用 GLUT 工具包，
可以在调用 glutInitDisplayMode 函数时在参数中加上 GLUT_STENCIL，例如： 
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL); 
 
在 Windows 操作系统中，即使没有明确要求使用模板缓冲区，有时候也会分配模板缓冲区。但为了保证程
序的通用性，最好还是明确指定使用模板缓冲区。如果确实没有分配模板缓冲区，则所有进行模板测试的
像素全部都会通过测试。 
 
通过 glEnable/glDisable 可以启用或禁用模板测试。 
glEnable(GL_STENCIL_TEST);  // 启用模板测试 
glDisable(GL_STENCIL_TEST); // 禁用模板测试 

从前面所讲可以知道，使用剪裁测试可以把绘制区域限制在一个矩形的区域内。但如果需要把绘 制区域限制在一个不规则的区域内，则需要使用模板测试。
例如：绘制一个湖泊，以及周围的树木，然后绘制树木在湖泊中的倒影。为了保证倒影被正确的 限制在湖泊表面，可以使用模板测试。
具体的步骤如下： (1) 关闭模板测试，绘制地面和树木。 (2) 开启模板测试，使用 glClear 设置所有像素的模板值为 0。 
(3) 设置glStencilFunc(GL_ALWAYS, 1, 1); glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);绘制 湖泊水面。这样一来，湖泊水面的像素的“模板值”为 1，而其它地方像素的“模板值”为 0。
(4) 设置 glStencilFunc(GL_EQUAL, 1, 1); glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);绘制倒影。 这样一来，只有“模板值”为 1 的像素才会被绘制，因此只有“水面”的像素才有可能被倒影的像素 替换，而其它像素则保持不变。
另外需要注意的地方就是：因为是绘制三维的场景，我们开启了深度测试。但是站在观察者的位置，球体
的镜像其实是在平面镜的“背后”，也就是说，如果按照常规的方式绘制，平面镜会把镜像覆盖掉，这不是
我们想要的效果。解决办法就是：设置深度缓冲区为只读，绘制平面镜，然后设置深度缓冲区为可写的状
态，绘制平面镜“背后”的镜像。
*/
void draw_sphere()
{
  // 设置光源 
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  {
    GLfloat
      pos[] = { 5.0f, 5.0f, 0.0f, 1.0f },
      ambient[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  }

  // 绘制一个球体 
  glColor3f(1, 0, 0);
  glPushMatrix();
  glTranslatef(0, 0, 2);
  glutSolidSphere(0.5, 20, 20);
  glPopMatrix();
}

void myDisplay(void)
{
  // 清除屏幕 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // 设置观察点 
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, 1, 5, 25);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(5, 0, 6.5, 0, 0, 0, 0, 1, 0);

  glEnable(GL_DEPTH_TEST);

  // 绘制球体 
  glDisable(GL_STENCIL_TEST);
  draw_sphere();

  // 绘制一个平面镜。在绘制的同时注意设置模板缓冲。 
  // 另外，为了保证平面镜之后的镜像能够正确绘制，在绘制平面镜时需要将深度缓冲区设置为只读的。 
  // 在绘制时暂时关闭光照效果 
  glClearStencil(0);
  glClear(GL_STENCIL_BUFFER_BIT);
  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glEnable(GL_STENCIL_TEST);


  glDisable(GL_LIGHTING);
  glColor3f(0.5f, 0.5f, 0.5f);
  glDepthMask(GL_FALSE);
  glRectf(-1.5f, -1.5f, 1.5f, 1.5f);
  glDepthMask(GL_TRUE);

  // 绘制一个与先前球体关于平面镜对称的球体，注意光源的位置也要发生对称改变 
  // 因为平面镜是在 X 轴和 Y轴所确定的平面，所以只要 Z 坐标取反即可实现对称 
  // 为了保证球体的绘制范围被限制在平面镜内部，使用模板测试 
  glStencilFunc(GL_EQUAL, 1, 0xFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glScalef(1.0f, 1.0f, -1.0f);
  draw_sphere();

  // 交换缓冲 
  glutSwapBuffers();

  // 截图 
  //grab();
}


#endif

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  //GLUT_SINGLE单缓存， GLUT_DOUBLE 双缓存（此时需要在绘制完成时使用glutSwapBuffers 交换缓冲区）

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

  //glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

  glutInitWindowPosition(100, 100);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("test ");
  glutDisplayFunc(&myDisplay);

  //glutIdleFunc(&myIdle);//glutIdleFunc  表示cpu空闲时做某事

  glutMainLoop();
  return 0;
}

