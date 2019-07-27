#pragma once

class CCpkSet {
public:
  CCpkSet() : m_min_cp(0), m_max_ca(0), m_min_cpk(0), m_start_num(0) {};
  ~CCpkSet() {};
public:
  double m_min_cp;    // cp����
  double m_max_ca;    // ca����
  double m_min_cpk;   // cpk����
  int m_start_num; // cpk�������ʼ����
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
// �ƻ��ż�¼�ṹ
class CProcessCard {
public:
  CProcessCard() : center(0), usl(0), lsl(0), avg(0), std(0), cp(0), ca(0), cpk(0), num(0) {};
  ~CProcessCard() {};
  void Reset() {
    no=_T("");    // ���̿����
    center=0;    // ����
    axis = _T("");   // ������
    usl = 0;       // ����
    lsl = 0;       // ����
    avg = 0;    // ��ֵ
    std = 0;    // ɢ��
    cp = 0;     // cp
    ca = 0;     // ca
    cpk = 0;    // cpk
    num = 0;
  }
public:
  CString no;    // ���̿����
  int center;    // ����
  CString axis;   // ������
  int usl;       // ����
  int lsl;       // ����
  double avg;    // ��ֵ
  double std;    // ɢ��
  double cp;     // cp
  double ca;     // ca
  double cpk;    // cpk
  int num;
};
// ���̿���¼�ṹ
class CProcessData {
public:
  CProcessData() {};
  ~CProcessData() {};
public:
  int no;         // ���
  int laser0;  // ԭ��
  int phi0;    // ԭ��
  int laser;   // ��
  int phi;     // ��
  int equal;   // ��Ч��
  int    sort;    //��λ
};

class CCpkLib
{
public:
  CCpkLib(LPCTSTR exePath);
  ~CCpkLib();

public:
  // ����//
  CString m_exePath;
  // .ini
  CString m_iniPath;
  // ��//
  CString m_cpkPath;
  // ��//
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
  
  void CloseCsv();
  // �򿪼ƻ���
  BOOL OpenProcessCard(CString plannedNo, CString &ProcessCardNo, BOOL force_new = FALSE);

  BOOL AddToPlannedCsv(CProcessCard processCard);

  BOOL AddCpkToPlannedCsv(CProcessCard processCard);

  BOOL AddToProcessCardCsv(CProcessData processData);

  BOOL GetData(char * strYMD);
  // ���processCard
  BOOL CalcCpk(CString plannedNo, CString ProcessCardNo, CProcessCard &processCard);
  // OpenProcessCard
  BOOL CalcCpk(CProcessCard &processCard);

  BOOL CalcTrend(CString plannedNo, CArray<double, double> &arrCpk);
};

