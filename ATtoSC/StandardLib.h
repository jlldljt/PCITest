#pragma once
// #include "CSVFile.h"

class CStdLib {
public:
  CStdLib() : m_no(-1), m_laser(0), m_phi(0){};
  CStdLib(int no, int laser, int phi) : m_no(no), m_laser(laser), m_phi(phi) {};
  ~CStdLib() {};
  int m_no;      //ID
  int m_laser;   //光轴
  int m_phi;     //电轴
};

class CStdSeries {
public:
  CStdSeries() : m_no(-1), m_phi_p(0), m_phi_n(0), m_laser_p(0), m_laser_n(0) {};
  CStdSeries(int no, int phi_p, int phi_n, int laser_p, int laser_n)
    : m_no(no), m_phi_p(phi_p), m_phi_n(phi_n), m_laser_p(laser_p), m_laser_n(laser_n) {};
  ~CStdSeries() {};
public:
  int m_no;      //系列ID
  int m_phi_p;   //电高
  int m_phi_n;   //电低
  int m_laser_p; //光高
  int m_laser_n; //光低
};

class CStdSet {
public:
  CStdSet() : m_maxd_phi(0), m_maxd_laser(0), m_min_lib_num(0), m_test_cnt(0), m_min_test_num(0), m_password(""),m_type(""){};
  ~CStdSet() {};
public:
  //int m_no;      //ID
  int m_maxd_phi;   //电轴最大差值
  int m_maxd_laser;   //光轴最大差值
  int m_min_lib_num; //库最小片数，改成用来判断最低通过片数
  int m_test_cnt; //对标测量次数
  int m_min_test_num; //最小对标片数
  CString m_password;
  CString m_type;// 对标类型
};

typedef struct tagResult{
  double m_avg;
  double m_std;
  double m_std2;
}Result;

class CStdResult {
public:
  CStdResult() 
  { 
    Clear();
  };
  ~CStdResult() {};
public:
  int m_sum;
  Result m_checking_laser, m_checking_phi;
  Result m_checked_laser, m_checked_phi;
  double m_dLaser, m_dPhi;

  void Clear(void)
  {
    m_sum = 0;
    memset(&m_checking_laser, 0, sizeof(Result));
    memset(&m_checking_phi, 0, sizeof(Result));
    memset(&m_checked_laser, 0, sizeof(Result));
    memset(&m_checked_phi, 0, sizeof(Result));
    m_dLaser = 0;
    m_dPhi = 0;
  }
  void AddCheck(Result &check_result, double angle);

  void Add(CStdLib checking, CStdLib checked)
  {
    m_sum++;
    AddCheck(m_checking_laser, checking.m_laser);
    AddCheck(m_checking_phi, checking.m_phi);
    AddCheck(m_checked_laser, checked.m_laser);
    AddCheck(m_checked_phi, checked.m_phi);
    m_dLaser = fabs(m_checking_laser.m_avg - m_checked_laser.m_avg);
    m_dPhi = fabs(m_checking_phi.m_avg - m_checked_phi.m_avg);
  }

};

class CStandardLib
{
public:
  //CStandardLib();
  CStandardLib(LPCTSTR path, int series = 20,int std = 10);
  ~CStandardLib();
public:
  CString m_sqlPath;
  //CCSVFile *m_csvStd;
  // 标准片系列
  CArray<CStdSeries, CStdSeries> m_series;
  // 当前加载的系列No
  int m_seriesNo;
  // 已加载系列的标准片
  CArray<CStdLib, CStdLib> m_lib;
  // 对标的实测值
  CArray<CStdLib, CStdLib> m_checked;
  int pass_num;
  // 对标的标准值
  CArray<CStdLib, CStdLib> m_checking;
  // 配置
  CStdSet m_set;
  // 对标结果
  CStdResult m_result;

  // 初始化数据库
  void Init(int series_num, int std_num);

  void Save(void);
  // 加载所有系列到array
  void LoadSeries(void);
  // 加载seriesNo系列的标准片到array
  void LoadLib(int seriesNo);
  // 设置系列
  void SetSeries(CStdSeries std);
  // 设置当前系列标准片
  void SetStd(CStdLib std);
  // 设置已测量的标准片
  void SetStdChecking(CStdLib std);
  CStdLib GetStdChecking(int no);
  void SetStdChecked(CStdLib std);
  CStdLib GetStdChecked(int no);
  // 加载配置
  void LoadSet(/*int id*/);
  // 检查密码
  BOOL CheckPassword(CString password);
  // 保存对标设置
  void SaveSet(void);
  // 清除对标
  void ClearCheck();
  UCHAR AddCheck(CStdLib checking, CStdLib checked);
  // 检测是否已经检查过了
  BOOL IsChecked(int no);
};

