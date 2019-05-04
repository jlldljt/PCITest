/*
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
//     //////////////////////////////////////////////// MY test//////////////////////////////////////////////////
//
#define SUB 10
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
static int day = 200; // day 的变化：从 0 到 359 
void myDisplay(void)
{/*还有一个需要注意的细节：OpenGL 把三维坐标中的物体绘制到二维屏幕，绘制的顺序是按照代 码的顺序来进行的。因此后绘制的物体会遮住先绘制的物体，即使后绘制的物体在先绘制的物体 的“后面”也是如此。使用深度测试可以解决这一问题。使用的方法是：1、以 GL_DEPTH_TEST 为参数调用 glEnable 函数，启动深度测试。2、在必要时（通常是每次绘制画面开始时），清空深 度缓冲，即：glClear(GL_DEPTH_BUFFER_BIT);其中，glClear(GL_COLOR_BUFFER_BIT)与 glClear(GL_DEPTH_BUFFER_BIT)可以合并写为： glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 且后者的运行速度可能比前者快。 */
 
  glEnable(GL_DEPTH_TEST);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(75, 1, 1, 400000000);//设置可视空间
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, -200000000, 200000000, 0, 0, 0, 0, 0, 1);

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
#endif
int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(400, 400);
  glutCreateWindow("test ");
  glutDisplayFunc(&myDisplay);
  glutMainLoop();
  return 0;
}

