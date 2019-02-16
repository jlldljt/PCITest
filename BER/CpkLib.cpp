#include "StdAfx.h"
#include "CpkLib.h"
#include "BER.h"

CCpkLib::CCpkLib(LPCTSTR exePath)
{
	m_exePath = exePath;

	CFileFind find;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����  
	
	if (!find.FindFile(m_exePath + _T("\\CPK\\*.*")))
	{
		if (!CreateDirectory(m_exePath + _T("\\CPK\\"), NULL))
		{
			AfxMessageBox(_T("CPKĿ¼����ʧ�ܣ�"));
			exit(0);
		}
	}

	m_iniPath = m_exePath + _T("\\CPK\\CpkSet.ini");

	m_cpkPath = m_exePath + _T("\\CPK\\");

	m_csv_plannedNo = NULL;

	m_csv_processCardNo = NULL;

	Init();
	find.Close();
}


CCpkLib::~CCpkLib()
{
	if (m_csv_plannedNo)
	{
		delete m_csv_plannedNo;
		m_csv_plannedNo = NULL;
	}

	if (m_csv_processCardNo)
	{
		delete m_csv_processCardNo;
		m_csv_processCardNo = NULL;
	}
}

void CCpkLib::LoadPlannedNo(void)
{
	CFileFind finder;
	CString strPlan, strdir;
	strdir = m_cpkPath + _T("*.*");
	BOOL b_finded = (BOOL)finder.FindFile(strdir);
	while (b_finded)
	{
		b_finded = (BOOL)finder.FindNextFile();
		if (finder.IsDots())  continue;
		strPlan = finder.GetFileName();
		if (finder.IsDirectory())
		{
			m_arrPlannedNo.Add(strPlan);
		}
		else
		{
			continue;
		}
	}
	finder.Close();
}

BOOL CCpkLib::LoadProcessCardNo(CString plannedNo)
{
	CString path;
	path = m_cpkPath +plannedNo+ _T("\\")+plannedNo+ _T(".csv");

	CCSVFile *csv_plannedNo = new CCSVFile(path, CCSVFile::modeRead);

	CProcessCard processCard;

	if (csv_plannedNo)
	{
		CStringArray arr;
		m_arrProcessCardNo.RemoveAll();
		csv_plannedNo->SeekToBegin();
		while(csv_plannedNo->ReadData(arr))
		{
			if(arr.GetCount()==PLANNED_CSV_ROW_MAX){
				processCard.no = arr[PLANNED_CSV_ROW_NO];

				if(processCard.no == _T("���̿�"))
					continue;

				processCard.axis = arr[PLANNED_CSV_ROW_AXIS];
				processCard.center = _ttoi(arr[PLANNED_CSV_ROW_CENTER]);
				processCard.usl = _ttoi(arr[PLANNED_CSV_ROW_USL]);
				processCard.lsl = _ttoi(arr[PLANNED_CSV_ROW_LSL]);
				processCard.avg = _ttof(arr[PLANNED_CSV_ROW_AVG]);
				processCard.std = _ttof(arr[PLANNED_CSV_ROW_STD]);
				processCard.cp = _ttof(arr[PLANNED_CSV_ROW_CP]);
				processCard.ca = _ttof(arr[PLANNED_CSV_ROW_CA]);
				processCard.cpk = _ttof(arr[PLANNED_CSV_ROW_CPK]);
				m_arrProcessCardNo.Add(processCard);
			}
		}



		// 
		// 

		/*m_gridProcessCardNo.InsertRow(m_pCpk->m_arrProcessCardNo[i].no, -1);
		str.Format(_T("%d"), m_pCpk->m_arrProcessCardNo[i].center);
		m_gridProcessCardNo.SetItemText(line, 1, str);
		str.Format(_T("%d"), m_pCpk->m_arrProcessCardNo[i].usl);
		m_gridProcessCardNo.SetItemText(line, 2, str);
		str.Format(_T("%d"), m_pCpk->m_arrProcessCardNo[i].lsl);
		m_gridProcessCardNo.SetItemText(line, 3, str);
		str.Format(_T("%.3f"), m_pCpk->m_arrProcessCardNo[i].avg);
		m_gridProcessCardNo.SetItemText(line, 4, str);
		str.Format(_T("%.3f"), m_pCpk->m_arrProcessCardNo[i].std);
		m_gridProcessCardNo.SetItemText(line, 5, str);
		str.Format(_T("%.3f"), m_pCpk->m_arrProcessCardNo[i].cp);
		m_gridProcessCardNo.SetItemText(line, 6, str);
		str.Format(_T("%.3f"), m_pCpk->m_arrProcessCardNo[i].ca);
		m_gridProcessCardNo.SetItemText(line, 7, str);
		str.Format(_T("%.3f"), m_pCpk->m_arrProcessCardNo[i].cpk);
		m_gridProcessCardNo.SetItemText(line, 8, str);*/
		delete csv_plannedNo;

		csv_plannedNo = NULL;

	}

	return TRUE;
}

void CCpkLib::Init(void)
{
	CFileFind findini;

	BOOL ifFind = findini.FindFile(m_iniPath);

	if (!ifFind)
	{
		WritePrivateProfileString(_T("CPK�趨"), _T("CP����"), _T("1.0"), m_iniPath);
		WritePrivateProfileString(_T("CPK�趨"), _T("CA����"), _T("0.5"), m_iniPath);
		WritePrivateProfileString(_T("CPK�趨"), _T("CPK����"), _T("1.0"), m_iniPath);
		WritePrivateProfileString(_T("CPK�趨"), _T("��ʼ����"), _T("20"), m_iniPath);
	}

	TCHAR ret[100] = { 0 };
	GetPrivateProfileString(_T("CPK�趨"), _T("CP����"), _T(""), ret, sizeof(ret), m_iniPath);
	m_cpkSet.m_min_cp = _ttof(ret);
	GetPrivateProfileString(_T("CPK�趨"), _T("CA����"), _T(""), ret, sizeof(ret), m_iniPath);
	m_cpkSet.m_max_ca = _ttof(ret);
	GetPrivateProfileString(_T("CPK�趨"), _T("CPK����"), _T(""), ret, sizeof(ret), m_iniPath);
	m_cpkSet.m_min_cpk = _ttof(ret);
	GetPrivateProfileString(_T("CPK�趨"), _T("��ʼ����"), _T(""), ret, sizeof(ret), m_iniPath);
	m_cpkSet.m_start_num = _ttoi(ret);
	findini.Close();
}

void CCpkLib::SaveSet(void)
{
	CString str;

	str.Format(_T("%.3f"), m_cpkSet.m_min_cp);
	WritePrivateProfileString(_T("CPK�趨"), _T("CP����"), str, m_iniPath);
	str.Format(_T("%.3f"), m_cpkSet.m_max_ca);
	WritePrivateProfileString(_T("CPK�趨"), _T("CA����"), str, m_iniPath);
	str.Format(_T("%.3f"), m_cpkSet.m_min_cpk);
	WritePrivateProfileString(_T("CPK�趨"), _T("CPK����"), str, m_iniPath);
	str.Format(_T("%d"), m_cpkSet.m_start_num);
	WritePrivateProfileString(_T("CPK�趨"), _T("��ʼ����"), str, m_iniPath);
}

BOOL CCpkLib::OpenProcessCard(CString plannedNo, CString ProcessCardNo)
{
	CString path_plannedNo = m_cpkPath + plannedNo;

	CFileFind findini;

	if (!findini.FindFile(path_plannedNo + _T("\\*.*")))
	{
		if (!CreateDirectory(path_plannedNo + _T("\\"), NULL))
		{
			AfxMessageBox(_T("�ƻ���Ŀ¼����ʧ�ܣ�"));
			findini.Close();
			return FALSE;
		}
	}

	CString path = path_plannedNo + _T("\\") + plannedNo + _T(".csv");

	BOOL ifFind = findini.FindFile(path);

	TRY
	{
		if (m_csv_plannedNo)
		{
			delete m_csv_plannedNo;
			m_csv_plannedNo = NULL;
		}

		if (!m_csv_plannedNo)
			m_csv_plannedNo = new CCSVFile(path, CCSVFile::modeWrite);
		else
			AfxMessageBox(_T("�ƻ���csv��ʧ��"));
	}
	CATCH(CFileException, e)
	{
		CString str;
		e->GetErrorMessage(str.GetBufferSetLength(256), 256);
		str.ReleaseBufferSetLength(256);
		AfxMessageBox(str);
		findini.Close();
		return FALSE;
	}
	END_CATCH

		if (!ifFind)  //ini����������һ��
		{
			if (m_csv_plannedNo) {
				CStringArray arr;
				CString str;
				arr.Add(_T("Data"));
				arr.Add(_T("���̿�"));
				arr.Add(_T("������"));
				arr.Add(_T("����"));
				arr.Add(_T("����"));
				arr.Add(_T("����"));
				arr.Add(_T("��ֵ"));
				arr.Add(_T("ɢ��"));
				arr.Add(_T("CP"));
				arr.Add(_T("CA"));
				arr.Add(_T("CPK"));
				m_csv_plannedNo->WriteData(arr);
				arr.RemoveAll();

			}
		}

		path = path_plannedNo + _T("\\") + ProcessCardNo + _T(".csv");

		ifFind = findini.FindFile(path);


		TRY
		{
			if (m_csv_processCardNo)
			{
				delete m_csv_processCardNo;
				m_csv_processCardNo = NULL;
			}
			if (!m_csv_processCardNo)
				m_csv_processCardNo = new CCSVFile(path, CCSVFile::modeWrite);
			else
				AfxMessageBox(_T("���̿�csv��ʧ��"));
		}
		CATCH(CFileException, e)
		{
			CString str;
			e->GetErrorMessage(str.GetBufferSetLength(256), 256);
			str.ReleaseBufferSetLength(256);
			AfxMessageBox(str);
			findini.Close();
			return FALSE;
		}
		END_CATCH
			findini.Close();
		if (!ifFind)  //ini����������һ��
		{

			if (m_csv_processCardNo) {
				CStringArray arr;
				CString str;
				arr.Add(_T("No"));
				arr.Add(_T("����"));
				arr.Add(_T("����"));
				arr.Add(_T("��Ч��"));
				arr.Add(_T("ԭʼ����"));
				arr.Add(_T("ԭʼ����"));
				arr.Add(_T("��λ"));
				m_csv_processCardNo->WriteData(arr);
				arr.RemoveAll();
				return TRUE;
			}
		}
		else
		{
			return FALSE;
		}
}

BOOL CCpkLib::AddToPlannedCsv(CProcessCard processCard)
{
	if (m_csv_plannedNo) {
		CStringArray arr;
		CString str;
		char ch[30];
		GetData(ch);
		arr.Add(CA2T(ch));        // DATA
		arr.Add(processCard.no);  //���̿�
		/*str.Format(_T("%d"), processCard.center);
		arr.Add(str);
		str.Format(_T("%d"), processCard.usl);
		arr.Add(str);
		str.Format(_T("%d"), processCard.lsl);
		arr.Add(str);
		str.Format(_T("%d"), processCard.cpk_num);
		arr.Add(str);
		str.Format(_T("%.3f"), processCard.avg);
		arr.Add(str);
		str.Format(_T("%.3f"), processCard.std);
		arr.Add(str);
		str.Format(_T("%.3f"), processCard.cp);
		arr.Add(str);
		str.Format(_T("%.3f"), processCard.ca);
		arr.Add(str);
		str.Format(_T("%.3f"), processCard.cpk);
		arr.Add(str);*/
		m_csv_plannedNo->WriteData(arr);
		arr.RemoveAll();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CCpkLib::AddCpkToPlannedCsv(CProcessCard processCard)
{
	if (m_csv_plannedNo) {
		CStringArray arr;
		CString str;
		//char ch[30];
		//GetData(ch);
		//arr.Add(CA2T(ch));        // DATA
		//arr.Add(processCard.no);  //���̿�
		arr.Add(processCard.axis);
		str.Format(_T("%d"), processCard.center);
		arr.Add(str);
		str.Format(_T("%d"), processCard.usl);
		arr.Add(str);
		str.Format(_T("%d"), processCard.lsl);
		arr.Add(str);
		str.Format(_T("%.3f"), processCard.avg);
		arr.Add(str);
		str.Format(_T("%.3f"), processCard.std);
		arr.Add(str);
		str.Format(_T("%.3f"), processCard.cp);
		arr.Add(str);
		str.Format(_T("%.3f"), processCard.ca);
		arr.Add(str);
		str.Format(_T("%.3f"), processCard.cpk);
		arr.Add(str);
		m_csv_plannedNo->WriteDataWithoutReturn(arr);
		arr.RemoveAll();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CCpkLib::AddToProcessCardCsv(CProcessData processData)
{
	if (m_csv_processCardNo) {
		CStringArray arr;
		CString str;
		str.Format(_T("%d"), processData.no);
		arr.Add(str);
		str.Format(_T("%.3f"), processData.laser);
		arr.Add(str);
		str.Format(_T("%.3f"), processData.phi);
		arr.Add(str);
		str.Format(_T("%.3f"), processData.equal);
		arr.Add(str);
		str.Format(_T("%.3f"), processData.laser0);
		arr.Add(str);
		str.Format(_T("%.3f"), processData.phi0);
		arr.Add(str);
		str.Format(_T("%d"), processData.sort);
		arr.Add(str);
		m_csv_processCardNo->WriteData(arr);
		arr.RemoveAll();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CCpkLib::GetData(char* data)
{
	time_t l_time;
	l_time = time(NULL);
	struct tm *l_struct_tm;
	l_struct_tm = localtime(&l_time);
	strftime(data, 30, "%Y-%m-%d--%H-%M-%S", l_struct_tm);
	return 1;
}

BOOL CCpkLib::CalcCpk(CString plannedNo, CString ProcessCardNo, CProcessCard & processCard)
{
  // ���ؼƻ��š�
  CString path;
  path = m_cpkPath + plannedNo + _T("\\") + plannedNo + _T(".csv");

  CCSVFile *csv = new CCSVFile(path, CCSVFile::modeRead);

  if (csv)
  {
    CStringArray arr;
    csv->SeekToBegin();
    while (csv->ReadData(arr))
    {
      if (arr.GetCount() > 5 ) {
        processCard.no = arr[PLANNED_CSV_ROW_NO];

        if (processCard.no == ProcessCardNo)
        {

          processCard.axis = arr[PLANNED_CSV_ROW_AXIS];
          processCard.center = _ttoi(arr[PLANNED_CSV_ROW_CENTER]);
          processCard.usl = _ttoi(arr[PLANNED_CSV_ROW_USL]);
          processCard.lsl = _ttoi(arr[PLANNED_CSV_ROW_LSL]);
          processCard.avg = 0;//_ttof(arr[PLANNED_CSV_ROW_AVG]);
          processCard.std = 0;//_ttof(arr[PLANNED_CSV_ROW_STD]);
          processCard.cp = 0;//_ttof(arr[PLANNED_CSV_ROW_CP]);
          processCard.ca = 0;//_ttof(arr[PLANNED_CSV_ROW_CA]);
          processCard.cpk =0;// _ttof(arr[PLANNED_CSV_ROW_CPK]);

          break;
        }
      }
    }

    delete csv;

    csv = NULL;

  }
  // �������̿�������CPK
  path = m_cpkPath + plannedNo + _T("\\") + plannedNo + _T(".csv");

  csv = new CCSVFile(path, CCSVFile::modeRead);

  CProcessData data;

  int num = 0;

  if (csv)
  {
    CStringArray arr;
    csv->SeekToBegin();
    while (csv->ReadData(arr))
    {
      if (arr.GetCount() == PROCESS_CSV_ROW_MAX) {

          data.no = _ttoi(arr[PROCESS_CSV_ROW_NO]);
          data.laser = _ttof(arr[PROCESS_CSV_ROW_LASER]);
          data.phi = _ttof(arr[PROCESS_CSV_ROW_PHI]);
          data.equal = _ttof(arr[PROCESS_CSV_ROW_EQUAL]);
          data.laser0 = _ttof(arr[PROCESS_CSV_ROW_LASER0]);
          data.phi0 = _ttof(arr[PROCESS_CSV_ROW_PHI0]);
          data.sort = _ttoi(arr[PROCESS_CSV_ROW_POS]);

          num++;

          if (processCard.axis == "����") 
          {

          }
          //CalcAvgStd(num, angle, check_result.m_avg, check_result.m_std, check_result.m_std2);

      }
    }

    delete csv;

    csv = NULL;

  }

  

  return TRUE;
}
