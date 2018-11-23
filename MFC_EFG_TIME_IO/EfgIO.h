#pragma once

#include "MultiCardCtrl.h"

#define MAX_SORT_NUM 50
#define MAX_TYPE_NUM 2

#define MAX_CARD_LEN 50

#define IO_ON 1
#define IO_OFF 0

#define MOTOR_X 0
#define MOTOR_Y 1
#define MOTOR_U 2
// 大部分数据保存到ini
typedef struct  {
  
  struct {//激光相关
    int out3;
    int out6;
    double factor_a;//幅度因子，反映激光晃动的距离 amp = atan (A * factor_a / factor_l) 若factor_l = 1；则简化为amp = atan (A * factor_a)
    double factor_l;//距离因子，反映反射光到镜头的距离
    double offset;//角度偏移
  }laser;
  
  struct {//X光相关
    int threshold;//阈值
    int confirmNum;//确认数量，宽度超过多少象素
    int ignore;//忽略值，忽略的值
    double factor_h;//纵向因子，平滑次数
    double factor_w;//横向因子,平滑宽度
  }xray;

  struct {  //电机相关
    struct {
      int pos;
      int dst_pos;
      float max_freq;
      float min_freq;
      float flexible;
    }x;
    struct {
      int pos;
      int dst_pos;
      float max_freq;
      float min_freq;
      float flexible;
    }y;
    struct {
      int pos;
      int dst_pos;
      float max_freq;
      float min_freq;
      float flexible;
    }u;
  }motor;
  
  struct//用户配置
  {
    int type;//晶片类型。0:0类片，1：1类片
    int type_sort_num[MAX_TYPE_NUM];//各晶片类型的档位数量
    struct
    {
      int x_off;//x放料位置
      int x_wait;//x等待位置
      int y_wait;//y等待位置
      int u_;//u位置
      int y_off[MAX_TYPE_NUM][MAX_SORT_NUM];//y放料位置，按类型不同，有多种档位，最大MAX_SORT_NUM
      int primary_n;//主测角低
      int primary_p;//主角高
      int secondary_n;//次角低
      int secondary_p;//次角高
      int not_detected;//未检出
    }pos;
    struct{//动作停留时间
      int x_on;//x吸料时间
      int x_off;//x放料时间
      int y_on;//y吸料时间
      int y_off;//y放料时间
      int blow;//吹气时间
      int clean;//清理三孔时间
    }time;
    struct{//测量相关
      TCHAR card[MAX_CARD_LEN];//流程卡
      struct {//主测量
        int type;// 无0光轴1电轴2等效角3
        int center_degree;
        int step_degree;
      }primary;
      struct {
        int type;// 无0光轴1电轴2等效角3
        int min_degree;
        int max_degree;
      }secondary;
      struct {// 等效角 = 原始光轴 + （原始电轴 - 等效角电轴参数）/等效角因子
        int phi;//等效角电轴参数
        int factor;//等效角因子
      }equivalent_angle;
     
    }measure;
  }user_config;//用户输入，使用时，需要转换，
  //位置相关

  int auto_run;//板卡自动启动
}EFG_ConfigParam;

// 大部分数据不保存到ini
typedef struct {
  struct
  {
    int y_off[MAX_SORT_NUM];    //y_off各位置的分档数量
    int primary_n;//主测角低
    int primary_p;//主角高
    int secondary_n;//次角低
    int secondary_p;//次角高
    int not_detected;//未检出
  }num;
  struct//单位是秒
  {
    int sort_sec[MAX_SORT_NUM];     // y_off各位置的角度,秒

  }degree;
  struct//单位是°
  {
    double D1, D2, DM, R1;              // x光结果，单位°
    double pluse_num,min_pluse_num,max_pluse_num;               // 转盘脉冲数
	  int    pluse_cnt;
    double u_g;                      // 公式计算处的u轴的g值
    double A, w, t, k;              // 激光结果
    double cur_phi1, cur_phi0, cur_theta1, cur_theta0, cur_equ; // 当前片
    double avg_phi1, avg_phi0, avg_theta1, avg_theta0, avg_equ;       // 平均
    double std_phi1, std_phi0, std_theta1, std_theta0, std_equ;       // 散差
    double std2_phi1, std2_phi0, std2_theta1, std2_theta0, std2_equ;   //中间值
    int cur_pos;//当前片档位
    int cur_pos_step;//当前片档位
    int num;//测量数量
  }measure;
}EFG_ResultParam;


enum EFG_IChannel
{
  LASER_CNT_GATE= 1,//激光计数门控
  XRAY_CNT_GATE= 2,//x光计数门控
  TURNTABLE_ZERO = 4,//转盘零位
};

enum EFG_OChannel
{
 // U_DIR = 0,//
  U_EN = 1,//
  XRAY_CTRL_GATE = 2,//光门
  U_GATE = 3,//
  U_OUTP = 4,//
  U_OUTN = 5,//
};
//按序号
enum AC6641_Channel
{
  X_DIR=4*8,
  X_GO,
  X_NOZZLE=4*8+3,
  Y_DIR,
  Y_GO,
  Y_NOZZLE = 4 * 8 + 7,
  BLOW,
  XRAY_GATE,
  X_FULL_CURRENT,
  Y_FULL_CURRENT,
  ALERT,
  CLEAN,
  X_STATE=6*8,
  Y_STATE=6*8+4,
  READY=6*8+7,//有料开关
  U_DIR=10*8,
  U_GO,
  //U_NOZZLE=10*8+3,
  U_STATE = 11 * 8,
};
//按port
enum AC6641_Port
{
  X_STEP_LOW=0,
  X_STEP_HIGH=1,
  Y_STEP_LOW=2,
  Y_STEP_HIGH=3,
  U_STEP_LOW=8,
  U_STEP_HIGH=9,
};

enum EFG_T0Channel
{
  LASER_CNT_START = 4,//激光计数起始channel
  //XRAY_CNT_START = 8,//x光计数起始channel
};

enum EFG_T1Channel
{
  XRAY_CNT_START = 0,//x光计数起始channel
};

class CEfgIO
{
public:
  CEfgIO();
  ~CEfgIO();
private:
  CMultiCardCtrl m_multiCardCtrl;
  BOOL m_on;//efg是否开启
  int m_on_card_no;//efg 开启所用的板卡序号
  int m_on_iocard_no;//efg 开启所用的io板卡序号(ac6641

  BOOL AddPCI(int cardNo, PCIType type);//添加一个pci指针，最多3个,initPCI调用
  BOOL RunPCI(int cardNo);//按照配置启动一块pci，cardon调用
  BOOL StopPCI(int cardNo); //停止一块pci，cardoff调用
public:
  EFG_ConfigParam m_configParam;
  EFG_ResultParam m_resultParam;
  int GetPCINum();//获取pci板卡的数量
  CString GetPCIDesc(int cardNo);//获取某块pci的设备描述
  PCIType GetPCIType(int cardNo);//获取某块pci的类型
  int GetPCIDeviceNumber(int cardNo);//获取某块pci的设备序号
  void InitPCI();//初始化m_multiCardCtrl
  CPCIBase* GetPCI(int cardNo);//获得pci的指针
  BOOL CardOn();//efg所需板卡启动
  BOOL CardOff();//efg所需板卡停止
  int GetCardIndex();
  int GetIOCardIndex();
  //获得efg板卡
  //激光扫描
  int GetOut3();
  int GetOut6();
  //封装读写//TODO: 增加卡修改
  double ReadDi(EFG_IChannel channel);
  double ReadDi(AC6641_Channel channel);
  void WriteDo(EFG_OChannel channel, double param);
  void WriteDo(AC6641_Channel channel, double param);
  void WritePort(AC6641_Port port, double param);
  double ReadT0(EFG_T0Channel channel);
  void MotoRun(int moto_index, double param);
  void MotoRunNoWait(int moto_index, double param);
  void MotoZero(int moto_index);
  BOOL CheckMotoEnd(int moto_index);
  CString GetMeasureType(int index);
  void InitEfgIO(void);
  // 获取当前角度的下料位置
  int GetCurOffPos(int &pos_num);
  // 判断主测角的档位 -1 低 ；=档位数 高
  void JudegSortWhich(double sec, int & which);
  void ClearMeasureResult(void);
private:



};

