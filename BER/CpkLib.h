#pragma once

class CCpkSet {
public:
  CCpkSet() : m_min_cp(0), m_max_ca(0), m_min_cpk(0), m_start_num(0) {};
  ~CCpkSet() {};
public:
  double m_min_cp;    // cp上限
  double m_max_ca;    // ca上限
  double m_min_cpk;   // cpk上限
  int m_start_num; // cpk计算的起始数量
};

enum  ProcessCsvRow {
	PROCESS_CSV_ROW_NO = 0, 
  PROCESS_CSV_ROW_LASER,
  PROCESS_CSV_ROW_PHI,
  PROCESS_CSV_ROW_EQUAL,
  PROCESS_CSV_ROW_LASER0,
  PROCESS_CSV_ROW_PHI0,
  PROCESS_CSV_ROW_POS,
  PROCESS_CSV_ROW_MAX
};
enum PlannedCsvRow {
  PLANNED_CSV_ROW_DATA = 0,
  PLANNED_CSV_ROW_NO,
  PLANNED_CSV_ROW_AXIS,
  PLANNED_CSV_ROW_CENTER,
  PLANNED_CSV_ROW_USL,
  PLANNED_CSV_ROW_LSL,
  PLANNED_CSV_ROW_AVG,
  PLANNED_CSV_ROW_STD,
  PLANNED_CSV_ROW_CP,
  PLANNED_CSV_ROW_CA,
  PLANNED_CSV_ROW_CPK,
  PLANNED_CSV_ROW_MAX
};
class CProcessCard {
public:
  CProcessCard() {};
  ~CProcessCard() {};
public:
  CString no;    // 流程卡编号
  int center;    // 中心
  CString axis;   // 所测轴
  int usl;       // 上限
  int lsl;       // 下限
  double avg;    // 均值
  double std;    // 散差
  double cp;     // cp
  double ca;     // ca
  double cpk;    // cpk
};

class CProcessData {
public:
  CProcessData() {};
  ~CProcessData() {};
public:
  int no;         // 编号
  double laser0;  // 原光
  double phi0;    // 原电
  double laser;   // 光
  double phi;     // 电
  double equal;   // 等效角
  int    sort;    //档位
};

class CCpkLib
{
public:
  CCpkLib(LPCTSTR exePath);
  ~CCpkLib();

public:
  // 不带//
  CString m_exePath;
  // .ini
  CString m_iniPath;
  // 带//
  CString m_cpkPath;
  // 带//
  CString m_plannedNo;
  // .txt
  CString m_processCardNo;

  CCSVFile *m_csv_plannedNo;

  CCSVFile *m_csv_processCardNo;

  CCpkSet m_cpkSet;

  CArray<CString, CString> m_arrPlannedNo;

  void LoadPlannedNo(void);

  CArray<CProcessCard, CProcessCard> m_arrProcessCardNo;

  BOOL LoadProcessCardNo(CString plannedNo);

  void Init(void);

  void SaveSet(void);
  // 打开计划号
  BOOL OpenProcessCard(CString plannedNo, CString ProcessCardNo);

  BOOL AddToPlannedCsv(CProcessCard processCard);

  BOOL AddCpkToPlannedCsv(CProcessCard processCard);

  BOOL AddToProcessCardCsv(CProcessData processData);

  BOOL GetData(char * strYMD);
  // 填充processCard
  BOOL CalcCpk(CString plannedNo, CString ProcessCardNo, CProcessCard &processCard);

};

