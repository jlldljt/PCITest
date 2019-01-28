#pragma once
#include "CSVFile.h"

class CStdLib {
public:
  CStdLib() : m_no(0), m_laser(0), m_phi(0){};
  CStdLib(int no, int laser, int phi) : m_no(no), m_laser(laser), m_phi(phi) {};
  ~CStdLib() {};
  int m_no;      //ID
  int m_laser;   //光轴
  int m_phi;     //电轴
};

class CStdSeries {
public:
  CStdSeries() : m_no(0), m_phi_p(0), m_phi_n(0), m_laser_p(0), m_laser_n(0) {};
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

class CStandardLib
{
public:
  //CStandardLib();
  CStandardLib(LPCTSTR path, int series = 10,int std = 50);
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
  // 对标过的标准片
  CArray<CStdLib, CStdLib> m_checked;


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
};

