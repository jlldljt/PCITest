#include "StdAfx.h"
#include "StandardLib.h"
#include "ZPJ.h"

//CStandardLib::CStandardLib()
//{
//}

CStandardLib::CStandardLib(LPCTSTR path, int series_num, int std_num)
{
  m_sqlPath = path;

  Init(series_num, std_num);

  LoadSeries();

  LoadSet();
}


CStandardLib::~CStandardLib()
{
}

void CStandardLib::Init(int series_num, int std_num)
{
  SQLiteWrapper sqlite;

  if (!sqlite.Open(CW2A(m_sqlPath)))
  {
    exit(0);
  }
  //创建标准库系列表
  std::string sql = "CREATE TABLE SERIES("  \
    "ID INT PRIMARY KEY     NOT NULL," \
    "电低           INT," \
    "电高           INT," \
    "光低       INT," \
    "光高        INT );";

  bool isOk = sqlite.Writedb(std::string(sql));
  //添加个系列
  if (true == isOk)
  {
    int cnt = 0;

    for (cnt = 0; cnt < series_num; cnt++)
    {
      char s1[10];
      char s2[10];
      char s3[10];
      char s4[10];
      char s5[10];
      itoa(cnt+1, s1, 10);
      itoa(0, s2, 10);
      itoa(0, s3, 10);
      itoa(0, s4, 10);
      itoa(0, s5, 10);
      sql = "INSERT INTO SERIES (ID,电低,电高,光低,光高) "  \
        "VALUES ("\
        + std::string(s1) + ","\
        + std::string(s2) + "," \
        + std::string(s3) + "," \
        + std::string(s4) + "," \
        + std::string(s5) + " );";

      isOk = sqlite.Writedb(std::string(sql));

      if (false == isOk)
        continue;
      /////////////////////////////////////////
      //创建标准库表
      sql = "CREATE TABLE STDS"\
        + std::string(s1) + "("  \
        "ID INT PRIMARY KEY     NOT NULL," \
        "电           INT," \
        "光           INT );";

      isOk = sqlite.Writedb(std::string(sql));

      if (false == isOk)
        continue;
      //添加10个系列
      if (true == isOk)
      {
        int cnt = 0;

        for (cnt = 0; cnt < std_num; cnt++)
        {
          char s6[10];
          char s7[10];
          char s8[10];
          itoa(cnt+1, s6, 10);
          itoa(0, s7, 10);
          itoa(0, s8, 10);
          sql =
            "INSERT INTO STDS"\
            + std::string(s1) + "(ID,电,光) "  \
            "VALUES ("\
            + std::string(s6) + ","\
            + std::string(s7) + "," \
            + std::string(s8) + " );";

          isOk = sqlite.Writedb(std::string(sql));
        }
      }
    }
  }

  //创建标准库系列表
  sql = "CREATE TABLE STDSET("  \
    "ID INT PRIMARY KEY     NOT NULL," \
    "对标选择               TEXT," \
    "电轴最大差值           INT," \
    "光轴最大差值           INT," \
    "库最小片数             INT," \
    "对标测量次数           INT," \
    "最小对标片数           INT," \
    "密码                   TEXT );";

  isOk = sqlite.Writedb(std::string(sql));
  //添加个系列
  if (true == isOk)
  {
    sql = "INSERT INTO STDSET (ID,对标选择,电轴最大差值,光轴最大差值,库最小片数,对标测量次数,最小对标片数,密码) "  \
      "VALUES (1,'电轴',30,10,5,2,5 ,'111111');";

    isOk = sqlite.Writedb(std::string(sql));
  }
  sqlite.Close();
}

void CStandardLib::Save(void)
{
  SQLiteWrapper sqlite;

  if (!sqlite.Open(CW2A(m_sqlPath)))
  {
    return;
  }

  sqlite.Begin();
  // 保存series表
  int count = m_series.GetCount();

  int i = 0;

  for (i = 0; i < count; i++)
  {
    char s1[10];
    char s2[10];
    char s3[10];
    char s4[10];
    char s5[10];
    itoa(m_series[i].m_no, s1, 10);
    itoa(m_series[i].m_phi_n, s2, 10);
    itoa(m_series[i].m_phi_p, s3, 10);
    itoa(m_series[i].m_laser_n, s4, 10);
    itoa(m_series[i].m_laser_p, s5, 10);
    
    std::string sql = "UPDATE SERIES SET 电低 = " + std::string(s2) + ","\
      + "电高 = " + std::string(s3) + ","\
      + "光低 = " + std::string(s4) + ","\
      + "光高 = " + std::string(s5) + " "\
      + "WHERE ID = " + std::string(s1) + ";";

    bool isOk = sqlite.Writedb(std::string(sql));

    if (false == isOk)
    {
      sql = "INSERT INTO SERIES (ID,电低,电高,光低,光高) "  \
        "VALUES ("\
        + std::string(s1) + ","\
        + std::string(s2) + "," \
        + std::string(s3) + "," \
        + std::string(s4) + "," \
        + std::string(s5) + " );";

      bool isOk = sqlite.Writedb(std::string(sql));
    }

  }
  // 保存m_seriesNo的片表
  if (m_seriesNo >= 0)
  {
    count = m_lib.GetCount();

    i = 0;

    for (i = 0; i < count; i++)
    {
      char s0[10];
      char s1[10];
      char s2[10];
      char s3[10];
      itoa(m_seriesNo, s0, 10);
      itoa(m_lib[i].m_no, s1, 10);
      itoa(m_lib[i].m_phi, s2, 10);
      itoa(m_lib[i].m_laser, s3, 10);

      std::string sql = "UPDATE STDS" + std::string(s0) + " SET "\
        + "电 = " + std::string(s2) + ","\
        + "光 = " + std::string(s3) + " "\
        + "WHERE ID = " + std::string(s1) + ";";

      bool isOk = sqlite.Writedb(std::string(sql));

      if (false == isOk)
      {
        sql =
          "INSERT INTO STDS"\
          + std::string(s0) + " (ID,电,光) "  \
          "VALUES ("\
          + std::string(s1) + ","\
          + std::string(s2) + "," \
          + std::string(s3) + " );";

        isOk = sqlite.Writedb(std::string(sql));
      }
    }
  }
  sqlite.Commit();

  sqlite.Close();
}

//加载series
void CStandardLib::LoadSeries(void)
{
  m_seriesNo = -1;

  m_series.RemoveAll();

  ClearCheck();

  m_lib.RemoveAll();

  SQLiteWrapper sqlite;

  if (!sqlite.Open(CW2A(m_sqlPath)))
  {
    return;
  }
  //查询
  std::string sql = "SELECT ID,电低,电高,光低,光高 FROM SERIES ORDER BY ID;";

  bool isOk = sqlite.Writedb(std::string(sql));
  //添加个系列
  if (true == isOk)
  {
    SQLiteStatement* stmt = sqlite.Readdb(sql);
    int count;

    if (stmt)
    {
      while (stmt->NextRow())
      {
        CStdSeries series;
        series.m_no = stmt->ValueInt(0);
        series.m_phi_n = stmt->ValueInt(1);
        series.m_phi_p = stmt->ValueInt(2);
        series.m_laser_n = stmt->ValueInt(3);
        series.m_laser_p = stmt->ValueInt(4);
        m_series.Add(series);
      }
    }
    delete stmt;
  }
  sqlite.Close();
}

void CStandardLib::LoadLib(int seriesNo)
{
  m_seriesNo = seriesNo;

  ClearCheck();

  m_lib.RemoveAll();

  SQLiteWrapper sqlite;

  if (!sqlite.Open(CW2A(m_sqlPath)))
  {
    return;
  }

  char s[10];

  itoa(seriesNo, s, 10);

  std::string sql = "SELECT ID,电,光 FROM STDS" + std::string(s) + " ORDER BY ID;";

  bool isOk = sqlite.Writedb(std::string(sql));
  //添加个系列
  if (true == isOk)
  {
    SQLiteStatement* stmt = sqlite.Readdb(sql);
    int count;

    if (stmt)
    {
      while (stmt->NextRow())
      {
        CStdLib lib;
        lib.m_no = stmt->ValueInt(0);
        lib.m_phi = stmt->ValueInt(1);
        lib.m_laser = stmt->ValueInt(2);
        m_lib.Add(lib);
      }
    }
    delete stmt;
  }

  sqlite.Close();
}

void CStandardLib::SetSeries(CStdSeries series)
{
  int count = m_series.GetCount();

  int i;

  for (i = 0; i < count; i++)
  {
    if (m_series[i].m_no == series.m_no)
    {
      m_series[i] = series;
      break;
    }
  }

  if (i >= count)
  {
    m_series.Add(series);
  }
}

void CStandardLib::SetStd(CStdLib std)
{
  int count = m_lib.GetCount();

  int i;

  for (i = 0; i < count; i++)
  {
    if (m_lib[i].m_no == std.m_no)
    {
      m_lib[i] = std;
      break;
    }
  }

  if (i >= count)
  {
    m_lib.Add(std);
  }
}

void CStandardLib::SetStdChecking(CStdLib std)
{
  int count = m_checking.GetCount();

  int i;

  for (i = 0; i < count; i++)
  {
    if (m_checking[i].m_no == std.m_no)
    {
      m_checking[i] = std;
      break;
    }
  }

  if (i >= count)
  {
    m_checking.Add(std);
  }
}
CStdLib CStandardLib::GetStdChecking(int no)
{
  int count = m_checking.GetCount();

  int i;

  for (i = 0; i < count; i++)
  {
    if (m_checking[i].m_no == no)
    {
      return m_checking[i];
    }
  }
}
void CStandardLib::SetStdChecked(CStdLib std)
{
  int count = m_checked.GetCount();

  int i;

  for (i = 0; i < count; i++)
  {
    if (m_checked[i].m_no == std.m_no)
    {
      m_checked[i] = std;
      break;
    }
  }

  if (i >= count)
  {
    m_checked.Add(std);
  }
}

CStdLib CStandardLib::GetStdChecked(int no)
{
  int count = m_checked.GetCount();

  int i;

  for (i = 0; i < count; i++)
  {
    if (m_checked[i].m_no == no)
    {
      return m_checked[i];
    }
  }
}

void CStandardLib::LoadSet()
{
  SQLiteWrapper sqlite;

  if (!sqlite.Open(CW2A(m_sqlPath)))
  {
    return;
  }

  std::string sql = "SELECT 电轴最大差值,光轴最大差值,库最小片数,对标测量次数,最小对标片数,密码,对标选择 FROM STDSET WHERE ID = 1;";

  bool isOk = sqlite.Writedb(std::string(sql));
  //添加个系列
  if (true == isOk)
  {
    SQLiteStatement* stmt = sqlite.Readdb(sql);

    if (stmt)
    {
      while (stmt->NextRow())
      {
        m_set.m_maxd_phi = stmt->ValueInt(0);
        m_set.m_maxd_laser = stmt->ValueInt(1);
        m_set.m_min_lib_num = stmt->ValueInt(2);
        m_set.m_test_cnt = stmt->ValueInt(3);
        m_set.m_min_test_num = stmt->ValueInt(4);
        m_set.m_password = stmt->ValueString(5);
        m_set.m_type = stmt->ValueString(6);

      }
    }
    delete stmt;
  }

  sqlite.Close();
}

BOOL CStandardLib::CheckPassword(CString password)
{
  BOOL ret = FALSE;

  SQLiteWrapper sqlite;

  if (!sqlite.Open(CW2A(m_sqlPath)))
  {
    return ret;
  }

  std::string sql = "select ID from STDSET where 密码 = '" + std::string(CT2A(password)) + "';";

  bool isOk = sqlite.Writedb(std::string(sql));
  //添加个系列
  if (true == isOk)
  {
    SQLiteStatement* stmt = sqlite.Readdb(sql);

    if (stmt)
    {
      while (stmt->NextRow())
      {
        if (1 == stmt->ValueInt(0))
        {
          ret = TRUE;
        }
      }
    }
    delete stmt;
  }

  sqlite.Close();

  return ret;
}

void CStandardLib::SaveSet(void)
{
  SQLiteWrapper sqlite;

  if (!sqlite.Open(CW2A(m_sqlPath)))
  {
    return;
  }

  sqlite.Begin();

  char s1[10];
  char s2[10];
  char s3[10];
  char s4[10];
  char s5[10];

  itoa(m_set.m_maxd_phi, s1, 10);
  itoa(m_set.m_maxd_laser, s2, 10);
  itoa(m_set.m_min_lib_num, s3, 10);
  itoa(m_set.m_test_cnt, s4, 10);
  itoa(m_set.m_min_test_num, s5, 10);

  std::string sql = "UPDATE STDSET SET 电轴最大差值 = " + std::string(s1) + ","\
    + "光轴最大差值 = " + std::string(s2) + ", "\
    + "库最小片数 = " + std::string(s3) + ", "\
    + "对标测量次数 = " + std::string(s4) + ", "\
    + "最小对标片数 = " + std::string(s5) + ", "\
    + "对标选择 = '" + std::string(CT2A(m_set.m_type)) + "', "\
    + "密码 = '" + std::string(CT2A(m_set.m_password)) + "' "\
    + "WHERE ID = 1;";

  bool isOk = sqlite.Writedb(std::string(sql));

  if (false == isOk)
  {
    AfxMessageBox(_T("保存失败"));
  }
  else
  {
    AfxMessageBox(_T("保存成功"));
  }

  sqlite.Commit();

  sqlite.Close();
}

void CStandardLib::ClearCheck()
{
  m_checked.RemoveAll();
  m_checking.RemoveAll();
  ASSERT(m_checked.GetCount()== m_checking.GetCount());
  pass_num = 0;
  m_result.Clear();
}



//返回值说明
// 0x00 表示通过
// 0x01 表示laser未通过
// 0x02 表示phi未通过
// 0x03 表示都未通过
UCHAR CStandardLib::AddCheck(CStdLib checking, CStdLib checked)
{
  ASSERT(checking.m_no == checked.m_no);
  UCHAR cnt = 0;

  SetStdChecking(checking);
  SetStdChecked(checked);

  ASSERT(m_checking.GetCount() == m_checked.GetCount());

  if (abs(checking.m_laser - checked.m_laser) > m_set.m_maxd_laser)
    cnt |= 0x01;

  if (abs(checking.m_phi - checked.m_phi) > m_set.m_maxd_phi)
    cnt |= 0x02;

  if(m_set.m_type == _T("光轴"))
  {
    if(!(cnt & 0x01))
      pass_num++; 
  }
  else if(m_set.m_type == _T("电轴"))
  {
    if (!(cnt & 0x02))
      pass_num++;
  }
      


  m_result.Add(checking,checked);
  return cnt;
}

BOOL CStandardLib::IsChecked(int no)
{
  int count_checking = m_checking.GetCount();
  int count_checked = m_checked.GetCount();

  ASSERT(count_checking == count_checked);

  int count = count_checked;

  int i;

  for (i = 0; i < count; i++)
  {
    ASSERT(m_checking[i].m_no == m_checked[i].m_no);
    if (m_checked[i].m_no == no)
    {
      return TRUE;
    }
  }

  return FALSE;
}

void CStdResult::AddCheck(Result &check_result, double angle)
{
    CalcAvgStd(m_sum, angle, check_result.m_avg, check_result.m_std, check_result.m_std2);
}
