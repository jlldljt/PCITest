// Device.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZPJ.h"
#include "Device.h"
#include "afxdialogex.h"


// CDevice �Ի���

IMPLEMENT_DYNAMIC(CDevice, CDialogEx)

CDevice::CDevice(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDevice::IDD, pParent)
{
	g_dlgDevice=this;
}

CDevice dlgDevice;
CDevice *g_dlgDevice=NULL;

CDevice::~CDevice()
{
}

void CDevice::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DEVICE, m_listDevice);
	DDX_Control(pDX, IDC_EDIT_RS232_RECEIVE, m_edtRs232Rcv);
	DDX_Control(pDX, IDC_CMB_EFGSTASEL, m_cmb_efgstasel);
}


BEGIN_MESSAGE_MAP(CDevice, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_COMCLEAR, &CDevice::OnBnClickedBtnComclear)
	ON_BN_CLICKED(IDC_BTN_STMINIT, &CDevice::OnBnClickedBtnStminit)
	ON_BN_CLICKED(IDC_BTN_STMSTOP, &CDevice::OnBnClickedBtnStmstop)
	ON_BN_CLICKED(IDC_BTN_XM, &CDevice::OnBnClickedBtnXm)
	ON_BN_CLICKED(IDC_BTN_YM, &CDevice::OnBnClickedBtnYm)
	ON_BN_CLICKED(IDC_BTN_CHGM, &CDevice::OnBnClickedBtnChgm)
	ON_BN_CLICKED(IDC_BTN_SORTM, &CDevice::OnBnClickedBtnSortm)
	ON_BN_CLICKED(IDC_BTN_RAWM, &CDevice::OnBnClickedBtnRawm)
	ON_BN_CLICKED(IDC_BTN_XZ, &CDevice::OnBnClickedBtnXz)
	ON_BN_CLICKED(IDC_BTN_YZ, &CDevice::OnBnClickedBtnYz)
	ON_BN_CLICKED(IDC_BTN_CHGZ, &CDevice::OnBnClickedBtnChgz)
	ON_BN_CLICKED(IDC_BTN_SORTZ, &CDevice::OnBnClickedBtnSortz)
	ON_BN_CLICKED(IDC_BTN_RAWV, &CDevice::OnBnClickedBtnRawv)
	ON_BN_CLICKED(IDC_BTN_UPZ, &CDevice::OnBnClickedBtnUpz)
	ON_BN_CLICKED(IDC_BTN_DOWNZ, &CDevice::OnBnClickedBtnDownz)
	ON_BN_CLICKED(IDC_BTN_XYM, &CDevice::OnBnClickedBtnXym)
	ON_BN_CLICKED(IDC_BTN_EFGTEST, &CDevice::OnBnClickedBtnEfgtest)
	ON_BN_CLICKED(IDC_BTN_EFGDYN, &CDevice::OnBnClickedBtnEfgdyn)
//	ON_BN_CLICKED(IDC_BTN_EFGSTART, &CDevice::OnBnClickedBtnEfgstart)
	ON_BN_CLICKED(IDC_CHK_XYMH, &CDevice::OnBnClickedChkXymh)
	ON_BN_CLICKED(IDC_CHK_INMH, &CDevice::OnBnClickedChkInmh)
	ON_BN_CLICKED(IDC_CHK_OUTMH, &CDevice::OnBnClickedChkOutmh)
	ON_BN_CLICKED(IDC_CHK_SORTMH, &CDevice::OnBnClickedChkSortmh)
	ON_BN_CLICKED(IDC_CHK_XYSN, &CDevice::OnBnClickedChkXysn)
	ON_BN_CLICKED(IDC_CHK_INSN, &CDevice::OnBnClickedChkInsn)
	ON_BN_CLICKED(IDC_CHK_OUTSN, &CDevice::OnBnClickedChkOutsn)
	ON_BN_CLICKED(IDC_CHK_SORTSN, &CDevice::OnBnClickedChkSortsn)
	ON_BN_CLICKED(IDC_BTN_XY1SET, &CDevice::OnBnClickedBtnXy1set)
	ON_BN_CLICKED(IDC_BTN_XY2SET, &CDevice::OnBnClickedBtnXy2set)
	ON_BN_CLICKED(IDC_BTN_XY3SET, &CDevice::OnBnClickedBtnXy3set)
	ON_BN_CLICKED(IDC_BTN_SORTUPSET, &CDevice::OnBnClickedBtnSortupset)
	ON_BN_CLICKED(IDC_BTN_SORTINSET, &CDevice::OnBnClickedBtnSortinset)
	ON_BN_CLICKED(IDC_BTN_SORTOUTSET, &CDevice::OnBnClickedBtnSortoutset)
	ON_BN_CLICKED(IDC_BTN_SORTDOWNSET, &CDevice::OnBnClickedBtnSortdownset)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DEVICE, &CDevice::OnLvnItemchangedListDevice)
	ON_BN_CLICKED(IDC_BTN_LISTSET, &CDevice::OnBnClickedBtnListset)
	ON_BN_CLICKED(IDC_CHK_BLOW, &CDevice::OnBnClickedChkBlow)
	ON_BN_CLICKED(IDC_CHK_ALARM, &CDevice::OnBnClickedChkAlarm)
	ON_BN_CLICKED(IDC_BTN_CLBPOS1SET, &CDevice::OnBnClickedBtnClbpos1set)
	ON_BN_CLICKED(IDC_BTN_CLBPOS2SET, &CDevice::OnBnClickedBtnClbpos2set)
	ON_BN_CLICKED(IDC_BTN_CLBPOS3SET, &CDevice::OnBnClickedBtnClbpos3set)
	ON_BN_CLICKED(IDC_BTN_COM, &CDevice::OnBnClickedBtnCom)
	ON_BN_CLICKED(IDC_BTN_LISTTEST, &CDevice::OnBnClickedBtnListtest)
	ON_BN_CLICKED(IDC_BTN_SORTUPTEST, &CDevice::OnBnClickedBtnSortuptest)
	ON_BN_CLICKED(IDC_BTN_SORTINTEST, &CDevice::OnBnClickedBtnSortintest)
	ON_BN_CLICKED(IDC_BTN_SORTOUTTEST, &CDevice::OnBnClickedBtnSortouttest)
	ON_BN_CLICKED(IDC_BTN_SORTDOWNTEST, &CDevice::OnBnClickedBtnSortdowntest)
	ON_BN_CLICKED(IDC_BTN_XY1TEST, &CDevice::OnBnClickedBtnXy1test)
	ON_BN_CLICKED(IDC_BTN_XY2TEST, &CDevice::OnBnClickedBtnXy2test)
	ON_BN_CLICKED(IDC_BTN_XY3TEST, &CDevice::OnBnClickedBtnXy3test)
	ON_BN_CLICKED(IDC_BTN_CLBPOS1TEST, &CDevice::OnBnClickedBtnClbpos1test)
	ON_BN_CLICKED(IDC_BTN_CLBPOS2TEST, &CDevice::OnBnClickedBtnClbpos2test)
	ON_BN_CLICKED(IDC_BTN_CLBPOS3TEST, &CDevice::OnBnClickedBtnClbpos3test)
	ON_CBN_SELCHANGE(IDC_CMB_EFGSTASEL, &CDevice::OnCbnSelchangeCmbEfgstasel)
//	ON_BN_CLICKED(IDC_BTN_OPENEFG, &CDevice::OnBnClickedBtnOpenefg)
//	ON_BN_CLICKED(IDC_BTN_CLOSEEFG, &CDevice::OnBnClickedBtnCloseefg)
	ON_BN_CLICKED(IDC_BTN_STMRESTA, &CDevice::OnBnClickedBtnStmresta)
	ON_BN_CLICKED(IDC_BTN_SNCHGSET, &CDevice::OnBnClickedBtnSnchgset)
	ON_BN_CLICKED(IDC_BTN_SNCHGTEST, &CDevice::OnBnClickedBtnSnchgtest)
	ON_BN_CLICKED(IDC_BTN_SNCHGM, &CDevice::OnBnClickedBtnSnchgm)
  ON_EN_CHANGE(IDC_EDT_SNCHGX, &CDevice::OnEnChangeEdtSnchgx)
END_MESSAGE_MAP()


//1-4���ƶ���num������1-7��special�����������1-4���㣻5������6-7��һ�񣩣�value������������ƶ�������
void CDevice::AxisMove(char num , int value , bool special)
{
	if(0 == value && 0 == special)
		return;
	switch(num)
	{
	case 0x01:
	case 0x02:
		if (special)
		{
			{
			gstuRun.chCmd[0]='M';
			gstuRun.chCmd[1]='L';
			gstuRun.chCmd[2]=0x00;
			gstuRun.chCmd[3]=0x00;							
			gstuRun.chCmd[4]=0x00;							
			gstuRun.chCmd[5]=0x00;							
			gstuRun.chCmd[6]=0x00;
			}
		}else
		{
			gstuRun.chCmd[0]='M';
			gstuRun.chCmd[1]=num;
			value>0 ? gstuRun.chCmd[2]='Z' : gstuRun.chCmd[2]='F';
			gstuRun.chCmd[3]=char(value>>16);						//������λ
			gstuRun.chCmd[4]=char(value>>8);						//������λ
			gstuRun.chCmd[5]=char(value);							//������λ
			gstuRun.chCmd[6]=0x00;
		}
		break;
	case 0x03:
	case 0x04:
		if (special)
		{
			if(1)
				{gstuRun.chCmd[0]='M';
			gstuRun.chCmd[1]=num;
			gstuRun.chCmd[2]='L';
			gstuRun.chCmd[3]=0x00;							
			gstuRun.chCmd[4]=0x00;							
			gstuRun.chCmd[5]=0x00;							
			gstuRun.chCmd[6]=0x00;
			}else
			{
				gstuRun.chCmd[0]='M';
			gstuRun.chCmd[1]='L';
			gstuRun.chCmd[2]=0x00;
			gstuRun.chCmd[3]=0x00;							
			gstuRun.chCmd[4]=0x00;							
			gstuRun.chCmd[5]=0x00;							
			gstuRun.chCmd[6]=0x00;
			}
		}else
		{
			gstuRun.chCmd[0]='M';
			gstuRun.chCmd[1]=num;
			value>0 ? gstuRun.chCmd[2]='Z' : gstuRun.chCmd[2]='F';
			gstuRun.chCmd[3]=char(value>>16);						//������λ
			gstuRun.chCmd[4]=char(value>>8);						//������λ
			gstuRun.chCmd[5]=char(value);							//������λ
			gstuRun.chCmd[6]=0x00;
		}
		break;
	case 0x05:
		if (special)
		{
			gstuRun.chCmd[0]='M';
			gstuRun.chCmd[1]=num;
			gstuRun.chCmd[2]='D';
			gstuRun.chCmd[3]=0x00;							
			gstuRun.chCmd[4]=0x00;							
			gstuRun.chCmd[5]=0x00;							
			gstuRun.chCmd[6]=0x00;
		}else
		{
			gstuRun.chCmd[0]='M';
			gstuRun.chCmd[1]=num;
			value>0 ? gstuRun.chCmd[2]='Z' : gstuRun.chCmd[2]='F';
			gstuRun.chCmd[3]=char(value>>16);						//������λ
			gstuRun.chCmd[4]=char(value>>8);						//������λ
			gstuRun.chCmd[5]=char(value);							//������λ
			gstuRun.chCmd[6]=0x00;
		}
		break;
	case 0x06:
	case 0x07:
		if (special)
		{
			gstuRun.chCmd[0]='M';
			gstuRun.chCmd[1]=num;
			gstuRun.chCmd[2]='Z';
			gstuRun.chCmd[3]=0x00;							
			gstuRun.chCmd[4]=0x00;							
			gstuRun.chCmd[5]=0x00;							
			gstuRun.chCmd[6]=0x00;
		}else
		{
		/*	gstuRun.chCmd[0]='M';
			gstuRun.chCmd[1]=num;
			nValue>0 ? gstuRun.chCmd[2]='Z' : gstuRun.chCmd[2]='F';
			gstuRun.chCmd[3]=char(value>>16);						//������λ
			gstuRun.chCmd[4]=char(value>>8);						//������λ
			gstuRun.chCmd[5]=char(value);							//������λ
			gstuRun.chCmd[6]=0x00;*/
		}
		break;
	default:
		break;
	}
	gclsCom.SendCharToCom(gstuRun.chCmd,1);
}

//xy�������������꣬����
void CDevice::XYMove(int xVal, int yVal)
{
	if(0 > xVal || 0 > yVal)
		return;

	gstuRun.chCmd[0]='U';
	gstuRun.chCmd[1]=char(xVal>>16);
	gstuRun.chCmd[2]=char(xVal>>8);
	gstuRun.chCmd[3]=char(xVal);							
	gstuRun.chCmd[4]=char(yVal>>16);							
	gstuRun.chCmd[5]=char(yVal>>8);							
	gstuRun.chCmd[6]=char(yVal);
	gclsCom.SendCharToCom(gstuRun.chCmd,1);
}

//STM���̳�ʼ��1��ֹͣ0,2��ͣ
void CDevice::StmSE(bool se)
{
	switch(se)
	{
	case 1:
		gstuRun.chCmd[0]='I';
		gstuRun.chCmd[1]='1';
		gstuRun.chCmd[2]='1';
		gstuRun.chCmd[3]=0x00;
		gstuRun.chCmd[4]=0x00;
		gstuRun.chCmd[5]=0x00;
		gstuRun.chCmd[6]=0x00;
		break;
	case 0:
		gstuRun.chCmd[0]='I';
		gstuRun.chCmd[1]='0';
		gstuRun.chCmd[2]='0';
		gstuRun.chCmd[3]=0x00;
		gstuRun.chCmd[4]=0x00;
		gstuRun.chCmd[5]=0x00;
		gstuRun.chCmd[6]=0x00;
		break;
	default:
		break;
	}
	gclsCom.SendCharToCom(gstuRun.chCmd,0);
}

//STM������ͣ1���˳���ͣ0
void CDevice::StmPause(bool pause)
{
	switch(pause)
	{
	case 1:
		gstuRun.chCmd[0]='I';
		gstuRun.chCmd[1]='2';
		gstuRun.chCmd[2]='1';
		gstuRun.chCmd[3]=0x00;
		gstuRun.chCmd[4]=0x00;
		gstuRun.chCmd[5]=0x00;
		gstuRun.chCmd[6]=0x00;
		break;
	case 0:
		gstuRun.chCmd[0]='I';
		gstuRun.chCmd[1]='2';
		gstuRun.chCmd[2]='0';
		gstuRun.chCmd[3]=0x00;
		gstuRun.chCmd[4]=0x00;
		gstuRun.chCmd[5]=0x00;
		gstuRun.chCmd[6]=0x00;
		break;
	default:
		break;
	}
	gclsCom.SendCharToCom(gstuRun.chCmd,0);
}

void CDevice::StmReSta()
{
		gstuRun.chCmd[0]='I';
		gstuRun.chCmd[1]='I';
		gstuRun.chCmd[2]='I';
		gstuRun.chCmd[3]=0x00;
		gstuRun.chCmd[4]=0x00;
		gstuRun.chCmd[5]=0x00;
		gstuRun.chCmd[6]=0x00;
	gclsCom.SendCharToCom(gstuRun.chCmd,0);
}
//EFG����
void CDevice::EFGCtrl(bool cw)
{
	if (cw)
	{
		gstuRun.chCmd[0]='F';
		gstuRun.chCmd[1]='S';
		gstuRun.chCmd[2]=0x00;
		gstuRun.chCmd[3]=0x00;
		gstuRun.chCmd[4]=0x00;
		gstuRun.chCmd[5]=0x00;
		gstuRun.chCmd[6]=0x00;
	}
	else
	{
		return;
	}
	gclsCom.SendCharToCom(gstuRun.chCmd,1);

}
//EFGͨ�Ͽ���
void CDevice::EFGCut(bool cut)
{
	if (cut)
	{
		gstuRun.chCmd[0]='F';
		gstuRun.chCmd[1]='H';
		gstuRun.chCmd[2]=0x00;
		gstuRun.chCmd[3]=0x00;
		gstuRun.chCmd[4]=0x00;
		gstuRun.chCmd[5]=0x00;
		gstuRun.chCmd[6]=0x00;
	}
	else
	{
		gstuRun.chCmd[0]='F';
		gstuRun.chCmd[1]='L';
		gstuRun.chCmd[2]=0x00;
		gstuRun.chCmd[3]=0x00;
		gstuRun.chCmd[4]=0x00;
		gstuRun.chCmd[5]=0x00;
		gstuRun.chCmd[6]=0x00;
	}
	gclsCom.SendCharToCom(gstuRun.chCmd,1);

}

//���������
void CDevice::MHCtrl(char num, bool ctrl)
{
	if (ctrl)
	{
		gstuRun.chCmd[0]='C';
		gstuRun.chCmd[1]=num;
		gstuRun.chCmd[2]='1';
		gstuRun.chCmd[3]=0x00;
		gstuRun.chCmd[4]=0x00;
		gstuRun.chCmd[5]=0x00;
		gstuRun.chCmd[6]=0x00;
	}
	else
	{
		gstuRun.chCmd[0]='C';
		gstuRun.chCmd[1]=num;
		gstuRun.chCmd[2]='0';
		gstuRun.chCmd[3]=0x00;
		gstuRun.chCmd[4]=0x00;
		gstuRun.chCmd[5]=0x00;
		gstuRun.chCmd[6]=0x00;
	}
	gclsCom.SendCharToCom(gstuRun.chCmd,1);
}

//�������
void CDevice::SNCtrl(char num, bool ctrl)
{
	if (ctrl)
	{
		gstuRun.chCmd[0]='S';
		gstuRun.chCmd[1]=num;
		gstuRun.chCmd[2]='1';
		gstuRun.chCmd[3]=0x00;
		gstuRun.chCmd[4]=0x00;
		gstuRun.chCmd[5]=0x00;
		gstuRun.chCmd[6]=0x00;
	}
	else
	{
		gstuRun.chCmd[0]='S';
		gstuRun.chCmd[1]=num;
		gstuRun.chCmd[2]='0';
		gstuRun.chCmd[3]=0x00;
		gstuRun.chCmd[4]=0x00;
		gstuRun.chCmd[5]=0x00;
		gstuRun.chCmd[6]=0x00;
	}
	gclsCom.SendCharToCom(gstuRun.chCmd,1);
}

//��������
void CDevice::BlowCtrl(bool ctrl)
{
	if (ctrl)
	{
		gstuRun.chCmd[0]='Q';
		gstuRun.chCmd[1]='1';
		gstuRun.chCmd[2]=0x00;
		gstuRun.chCmd[3]=0x00;
		gstuRun.chCmd[4]=0x00;
		gstuRun.chCmd[5]=0x00;
		gstuRun.chCmd[6]=0x00;
	}
	else
	{
		gstuRun.chCmd[0]='Q';
		gstuRun.chCmd[1]='0';
		gstuRun.chCmd[2]=0x00;
		gstuRun.chCmd[3]=0x00;
		gstuRun.chCmd[4]=0x00;
		gstuRun.chCmd[5]=0x00;
		gstuRun.chCmd[6]=0x00;
	}
	gclsCom.SendCharToCom(gstuRun.chCmd,1);
}

//��������
void CDevice::AlarmCtrl(bool ctrl)
{
	if (ctrl)
	{
		gstuRefresh.alarm = 1;
		gstuRun.chCmd[0]='B';
		gstuRun.chCmd[1]='1';
		gstuRun.chCmd[2]=0x00;
		gstuRun.chCmd[3]=0x00;
		gstuRun.chCmd[4]=0x00;
		gstuRun.chCmd[5]=0x00;
		gstuRun.chCmd[6]=0x00;
	}
	else
	{
		gstuRefresh.alarm = 0;
		gstuRun.chCmd[0]='B';
		gstuRun.chCmd[1]='0';
		gstuRun.chCmd[2]=0x00;
		gstuRun.chCmd[3]=0x00;
		gstuRun.chCmd[4]=0x00;
		gstuRun.chCmd[5]=0x00;
		gstuRun.chCmd[6]=0x00;
	}
	gclsCom.SendCharToCom(gstuRun.chCmd,1);
}

//��Ƭ�ֵ���λ�ã�
void CDevice::PosSet(char num , char num1 , int value)
{

	gstuRun.chCmd[0]='L';
	gstuRun.chCmd[1]=num;
	gstuRun.chCmd[2]=num1;
	gstuRun.chCmd[3]=char(value>>16);						//������λ
	gstuRun.chCmd[4]=char(value>>8);						//������λ
	gstuRun.chCmd[5]=char(value);							//������λ
	gstuRun.chCmd[6]=0x00;

	gclsCom.SendCharToCom(gstuRun.chCmd,1);
}

//XY����תλ�ã�
void CDevice::XYPosSet(char num , int valuex , int valuey)
{
	switch(num)
	{
	case 0x01:
		num='1';
		break;
	case 0x02:
		num='2';
		break;
	case 0x03:
		num='3';
		break;
	default:
		break;
	}
	gstuRun.chCmd[0]=num;
	gstuRun.chCmd[1]=char(valuex>>16);
	gstuRun.chCmd[2]=char(valuex>>8);
	gstuRun.chCmd[3]=char(valuex);
	gstuRun.chCmd[4]=char(valuey>>16);						//������λ
	gstuRun.chCmd[5]=char(valuey>>8);						//������λ
	gstuRun.chCmd[6]=char(valuey);							//������λ
	

	gclsCom.SendCharToCom(gstuRun.chCmd,1);
}

//XY���귢�ͣ�
int CDevice::XYSend(int valuex , int valuey)
{
	//if(valuex<0 ||valuey<0)
	//	AfxMessageBox(_T("xy������ָ���"));
	//gstuRun.chCmd[0]='W';
	//gstuRun.chCmd[1]=char(valuex>>16);
	//gstuRun.chCmd[2]=char(valuex>>8);
	//gstuRun.chCmd[3]=char(valuex);
	//gstuRun.chCmd[4]=char(valuey>>16);						//������λ
	//gstuRun.chCmd[5]=char(valuey>>8);						//������λ
	//gstuRun.chCmd[6]=char(valuey);							//������λ


	//gclsCom.SendCharToCom(gstuRun.chCmd,1);

  int x = gclsCom.WriteVar(VAR_X, valuex);

  int y = gclsCom.WriteVar(VAR_Y, valuey);

  if (x || y)
    return -1;
  else
    return 0;
}
int  CDevice::StatusGet(int &status)
{
  return gclsCom.ReadVar(VAR_STATUS, status);
}
int CDevice::StatusSet(int status)
{
  return gclsCom.WriteVar(VAR_STATUS, status);
}

//��λ���ͣ�
void CDevice::SortSend(char num)
{
	if(num<0)
		AfxMessageBox(_T("�ֵ����ָ���"));
	gstuRun.chCmd[0]='E';
	gstuRun.chCmd[1]=num;
	gstuRun.chCmd[2]=0x00;
	gstuRun.chCmd[3]=0x00;
	gstuRun.chCmd[4]=0x00;						//������λ
	gstuRun.chCmd[5]=0x00;						//������λ
	gstuRun.chCmd[6]=0x00;							//������λ


	gclsCom.SendCharToCom(gstuRun.chCmd,1);
}
//������λ������λ��
bool CDevice::EasyPosSet()
{
	int nValue1,nValue2,nN=0;
	gstuRun.chStmCmd=0;
	//������ת1
	nValue1 = GetDlgItemInt(IDC_EDT_XY1X,0,0);
	nValue2 = GetDlgItemInt(IDC_EDT_XY1Y,0,0);
	XYPosSet(1,nValue1,nValue2);
	while(!(gstuRun.bPosSet==1))
	{
		Sleep(1);
		if(++nN>500)
			return 0;
	}
	gstuRun.bPosSet=0;
	nN=0;
	//������ת2
	nValue1 = GetDlgItemInt(IDC_EDT_XY2X,0,0);
	nValue2 = GetDlgItemInt(IDC_EDT_XY2Y,0,0);
	XYPosSet(2,nValue1,nValue2);
	while(!(gstuRun.bPosSet==1))
	{
		Sleep(1);
		if(++nN>500)
			return 0;
	}
	gstuRun.bPosSet=0;
	nN=0;
	//������ת3
	nValue1 = GetDlgItemInt(IDC_EDT_XY3X,0,0);
	nValue2 = GetDlgItemInt(IDC_EDT_XY3Y,0,0);
	XYPosSet(3,nValue1,nValue2);
	while(!(gstuRun.bPosSet==1))
	{
		Sleep(1);
		if(++nN>500)
			return 0;
	}
	gstuRun.bPosSet=0;
	nN=0;
	//��Ƭ��������ת
	nValue1 = GetDlgItemInt(IDC_EDT_SORTUP,0,0);

	PosSet(0x01,0x01,nValue1);
	while(!(gstuRun.bPosSet==2))
	{
		Sleep(1);
		if(++nN>500)
			return 0;
	}
	gstuRun.bPosSet=0;
	nN=0;
	//��Ƭ�ỻƬ��Ƭ
	nValue1 = GetDlgItemInt(IDC_EDT_SORTIN,0,0);
	PosSet(0x01,0x02,nValue1);
	while(!(gstuRun.bPosSet==2))
	{
		Sleep(1);
		if(++nN>500)
			return 0;
	}
	gstuRun.bPosSet=0;
	nN=0;
	//��Ƭ�ỻƬ��Ƭ
	nValue1 = GetDlgItemInt(IDC_EDT_SORTOUT,0,0);
	PosSet(0x01,0x03,nValue1);
	while(!(gstuRun.bPosSet==2))
	{
		Sleep(1);
		if(++nN>500)
			return 0;
	}
	gstuRun.bPosSet=0;
	nN=0;
	//��Ƭ��������ת
	nValue1 = GetDlgItemInt(IDC_EDT_SORTDOWN,0,0);
	PosSet(0x01,0x04,nValue1);
	while(!(gstuRun.bPosSet==2))
	{
		Sleep(1);
		if(++nN>500)
			return 0;
	}
	gstuRun.bPosSet=0;
	nN=0;
	//�ֵ�λ��
	CString csTmp;
	int nTmp;
	for(int i =0;i<30 ; i++)
	{
		csTmp = m_listDevice.GetItemText(i,1);   
		nTmp=_wtoi(csTmp);
		PosSet(0x02,char(i),nTmp);
		while(!(gstuRun.bPosSet==2))
		{
			Sleep(1);
			if(++nN>500)
				return 0;
		}
		gstuRun.bPosSet=0;
		nN=0;
	}
	gstuRun.chStmCmd=0;
	SetDlgItemTextW(IDC_MSG,_T("����λ���趨���"));
	return 1;
}

//EFG��ʼ���λ������
void CDevice::EFGStaSel(char nN)
{
	
	gstuRun.chCmd[0]=0xEF;
	gstuRun.chCmd[1]=0x30 + nN;
	gstuRun.chCmd[2]=0x00;
	gstuRun.chCmd[3]=0x00;
	gstuRun.chCmd[4]=0x00;
	gstuRun.chCmd[5]=0x00;
	gstuRun.chCmd[6]=0x00;
	
	gclsCom.SendCharToCom(gstuRun.chCmd,1);

}
// CDevice ��Ϣ�������


void CDevice::OnBnClickedBtnComclear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	gclsCom.stuInf.str="";
	gstuRefresh.bComUpdate=1;
}


void CDevice::OnBnClickedBtnStminit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	StmSE(1);
}


void CDevice::OnBnClickedBtnStmstop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	StmSE(0);
}


void CDevice::OnBnClickedBtnXm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	gstuRun.chCmd[0]='7';
	gstuRun.chCmd[1]=0x00;
	gstuRun.chCmd[2]=0x00;
	gstuRun.chCmd[3]=0x00;							
	gstuRun.chCmd[4]=0x00;							
	gstuRun.chCmd[5]=0x00;							
	gstuRun.chCmd[6]=0x00;
	
	gclsCom.SendCharToCom(gstuRun.chCmd,1);
}


void CDevice::OnBnClickedBtnYm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(0)
	{
		BOOL bFlag=0;
		int nValue = GetDlgItemInt(IDC_EDT_Y,&bFlag,1);
		if(0 == bFlag)
		{
			AfxMessageBox(_T("error number"));
			return;
		}
		AxisMove(0x02,nValue,0);
	}
	else
		EasyPosSet();
}


void CDevice::OnBnClickedBtnChgm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValue = GetDlgItemInt(IDC_EDT_CHG,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error number"));
		return;
	}
	AxisMove(0x03,nValue,0);
}


void CDevice::OnBnClickedBtnSortm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValue = GetDlgItemInt(IDC_EDT_SORT,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error number"));
		return;
	}
	if(1)
		AxisMove(0x04,nValue,0);
	else
		SortSend(char(nValue));
}


void CDevice::OnBnClickedBtnRawm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValue = 100;//GetDlgItemInt(IDC_EDT_RAW,&bFlag,1);
	/*if(0 == bFlag)
	{
		AfxMessageBox(_T("error number"));
		return;
	}*/
	AxisMove(0x05,nValue,0);
}


void CDevice::OnBnClickedBtnXz()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AxisMove(0x01,0,1);
}


void CDevice::OnBnClickedBtnYz()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AxisMove(0x02,0,1);
}

void CDevice::OnBnClickedBtnChgz()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AxisMove(0x03,0,1);
}


void CDevice::OnBnClickedBtnSortz()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AxisMove(0x04,0,1);
}


void CDevice::OnBnClickedBtnRawv()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AxisMove(0x05,0,1);
}


void CDevice::OnBnClickedBtnUpz()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AxisMove(0x06,0,1);
}


void CDevice::OnBnClickedBtnDownz()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AxisMove(0x07,0,1);
}


void CDevice::OnBnClickedBtnXym()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValueX = GetDlgItemInt(IDC_EDT_X,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error X number"));
		return;
	}
	int nValueY = GetDlgItemInt(IDC_EDT_Y,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error Y number"));
		return;
	}
	/*if(1)
		XYMove(nValueX,nValueY);
	else
		XYSend(nValueX,nValueY);*/

  int status;
  StatusGet(status);
  if (status != 0) {
    AfxMessageBox(_T("status is not ready"));
  } 
  else {
    XYSend(nValueX, nValueY);
    StatusSet(1);
  }





}



void CDevice::OnBnClickedBtnEfgtest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
  int no = GetDlgItemInt(IDC_EDT_DYNNUM);
  int val = GetDlgItemInt(IDC_EDT_SNCHGX);
  gclsCom.WriteVar(no, val);


}


void CDevice::OnBnClickedBtnEfgdyn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

  int no = GetDlgItemInt(IDC_EDT_DYNNUM);
  int val = GetDlgItemInt(IDC_EDT_SNCHGX);
  gclsCom.ReadVar(no, val);

  SetDlgItemInt(IDC_EDT_SNCHGX, val);
	
}


//void CDevice::OnBnClickedBtnEfgstart()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	EFGCtrl(1);
//	for(int i = 0;i<5;i++)
//		{
//		g_dlgDevice->EFGCtrl(true);
//		Sleep(100);
//		}
//}


void CDevice::OnBnClickedChkXymh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_XYMH);
	int nStat = pChk->GetCheck();
	if (nStat)
	{
		MHCtrl(0x01,1);
	} 
	else
	{
		MHCtrl(0x01,0);
	}
}


void CDevice::OnBnClickedChkInmh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_INMH);
	int nStat = pChk->GetCheck();
	if (nStat)
	{
		MHCtrl(0x02,1);
	} 
	else
	{
		MHCtrl(0x02,0);
	}
}


void CDevice::OnBnClickedChkOutmh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_OUTMH);
	int nStat = pChk->GetCheck();
	if (nStat)
	{
		MHCtrl(0x03,1);
	} 
	else
	{
		MHCtrl(0x03,0);
	}
}


void CDevice::OnBnClickedChkSortmh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_SORTMH);
	int nStat = pChk->GetCheck();
	if (nStat)
	{
		MHCtrl(0x04,1);
	} 
	else
	{
		MHCtrl(0x04,0);
	}
}


void CDevice::OnBnClickedChkXysn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_XYSN);
	int nStat = pChk->GetCheck();
	if (nStat)
	{
		SNCtrl(0x01,1);
	} 
	else
	{
		SNCtrl(0x01,0);
	}
}


void CDevice::OnBnClickedChkInsn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_INSN);
	int nStat = pChk->GetCheck();
	if (nStat)
	{
		SNCtrl(0x02,1);
	} 
	else
	{
		SNCtrl(0x02,0);
	}
}


void CDevice::OnBnClickedChkOutsn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_OUTSN);
	int nStat = pChk->GetCheck();
	if (nStat)
	{
		SNCtrl(0x03,1);
	} 
	else
	{
		SNCtrl(0x03,0);
	}
}


void CDevice::OnBnClickedChkSortsn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_SORTSN);
	int nStat = pChk->GetCheck();
	if (nStat)
	{
		SNCtrl(0x04,1);
	} 
	else
	{
		SNCtrl(0x04,0);
	}
}


void CDevice::OnBnClickedBtnXy1set()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValueX = GetDlgItemInt(IDC_EDT_X,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error X number"));
		return;
	}
	int nValueY = GetDlgItemInt(IDC_EDT_Y,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error Y number"));
		return;
	}
	SetDlgItemInt(IDC_EDT_XY1X,nValueX,1);
	SetDlgItemInt(IDC_EDT_XY1Y,nValueY,1);
	CString strValue = _T("");
	CFileFind findini;
	BOOL ifFind = findini.FindFile(gstuPathInf.csPathIni);  

	if( !ifFind )  
	{  
		AfxMessageBox(_T("�������ļ�"));
		return;
	}  

	strValue.Format(_T("%d"),nValueX);
	WritePrivateProfileString(_T("ȡ����ת"),_T("1X"),strValue,gstuPathInf.csPathIni); 
	strValue.Format(_T("%d"),nValueY);
	WritePrivateProfileString(_T("ȡ����ת"),_T("1Y"),strValue,gstuPathInf.csPathIni); 
	XYPosSet(1,nValueX,nValueY);
}


void CDevice::OnBnClickedBtnXy2set()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValueX = GetDlgItemInt(IDC_EDT_X,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error X number"));
		return;
	}
	int nValueY = GetDlgItemInt(IDC_EDT_Y,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error Y number"));
		return;
	}
	SetDlgItemInt(IDC_EDT_XY2X,nValueX,1);
	SetDlgItemInt(IDC_EDT_XY2Y,nValueY,1);
	CString strValue = _T("");
	CFileFind findini;
	BOOL ifFind = findini.FindFile(gstuPathInf.csPathIni);  

	if( !ifFind )  
	{  
		AfxMessageBox(_T("�������ļ�"));
		return;
	}  

	strValue.Format(_T("%d"),nValueX);
	WritePrivateProfileString(_T("ȡ����ת"),_T("2X"),strValue,gstuPathInf.csPathIni); 
	strValue.Format(_T("%d"),nValueY);
	WritePrivateProfileString(_T("ȡ����ת"),_T("2Y"),strValue,gstuPathInf.csPathIni); 
	XYPosSet(2,nValueX,nValueY);
}


void CDevice::OnBnClickedBtnXy3set()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValueX = GetDlgItemInt(IDC_EDT_X,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error X number"));
		return;
	}
	int nValueY = GetDlgItemInt(IDC_EDT_Y,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error Y number"));
		return;
	}
	SetDlgItemInt(IDC_EDT_XY3X,nValueX,1);
	SetDlgItemInt(IDC_EDT_XY3Y,nValueY,1);
	CString strValue = _T("");
	CFileFind findini;
	BOOL ifFind = findini.FindFile(gstuPathInf.csPathIni);  

	if( !ifFind )  
	{  
		AfxMessageBox(_T("�������ļ�"));
		return;
	}  

	strValue.Format(_T("%d"),nValueX);
	WritePrivateProfileString(_T("ȡ����ת"),_T("3X"),strValue,gstuPathInf.csPathIni); 
	strValue.Format(_T("%d"),nValueY);
	WritePrivateProfileString(_T("ȡ����ת"),_T("3Y"),strValue,gstuPathInf.csPathIni); 
	
	
	XYPosSet(3,nValueX,nValueY);
	
}


void CDevice::OnBnClickedBtnSortupset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValue = GetDlgItemInt(IDC_EDT_CHG,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error number"));
		return;
	}
	SetDlgItemInt(IDC_EDT_SORTUP,nValue,1);
	CString strValue = _T("");
	CFileFind findini;
	BOOL ifFind = findini.FindFile(gstuPathInf.csPathIni);  

	if( !ifFind )  
	{  
		AfxMessageBox(_T("�������ļ�"));
		return;
	}  

	strValue.Format(_T("%d"),nValue);
	WritePrivateProfileString(_T("��Ƭλ��"),_T("������ת"),strValue,gstuPathInf.csPathIni);
	
	PosSet(0x01,0x01,nValue);
	
}


void CDevice::OnBnClickedBtnSortinset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValue = GetDlgItemInt(IDC_EDT_CHG,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error number"));
		return;
	}
	SetDlgItemInt(IDC_EDT_SORTIN,nValue,1);
	CString strValue = _T("");
	CFileFind findini;
	BOOL ifFind = findini.FindFile(gstuPathInf.csPathIni);  

	if( !ifFind )  
	{  
		AfxMessageBox(_T("�������ļ�"));
		return;
	}  

	strValue.Format(_T("%d"),nValue);
	WritePrivateProfileString(_T("��Ƭλ��"),_T("��Ƭ��Ƭ"),strValue,gstuPathInf.csPathIni);
	
	PosSet(0x01,0x02,nValue);
	
}


void CDevice::OnBnClickedBtnSortoutset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValue = GetDlgItemInt(IDC_EDT_CHG,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error number"));
		return;
	}
	SetDlgItemInt(IDC_EDT_SORTOUT,nValue,1);
	CString strValue = _T("");
	CFileFind findini;
	BOOL ifFind = findini.FindFile(gstuPathInf.csPathIni);  

	if( !ifFind )  
	{  
		AfxMessageBox(_T("�������ļ�"));
		return;
	}  

	strValue.Format(_T("%d"),nValue);
	WritePrivateProfileString(_T("��Ƭλ��"),_T("��Ƭ��Ƭ"),strValue,gstuPathInf.csPathIni);
	
	PosSet(0x01,0x03,nValue);
	
}


void CDevice::OnBnClickedBtnSortdownset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValue = GetDlgItemInt(IDC_EDT_CHG,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error number"));
		return;
	}
	SetDlgItemInt(IDC_EDT_SORTDOWN,nValue,1);
	CString strValue = _T("");
	CFileFind findini;
	BOOL ifFind = findini.FindFile(gstuPathInf.csPathIni);  

	if( !ifFind )  
	{  
		AfxMessageBox(_T("�������ļ�"));
		return;
	}  

	strValue.Format(_T("%d"),nValue);
	WritePrivateProfileString(_T("��Ƭλ��"),_T("������ת"),strValue,gstuPathInf.csPathIni);
	
	PosSet(0x01,0x04,nValue);
	
}


void CDevice::OnLvnItemchangedListDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	nListSel = pNMLV->iItem;
	*pResult = 0;
}


void CDevice::OnBnClickedBtnListset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	unsigned int plusenum = GetDlgItemInt(IDC_EDT_SORT,0,0);
	CString str1,str2;
	CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
	BOOL ifFind = findini.FindFile(gstuPathInf.csPathIni);  
	if( !ifFind )  
	{  
		AfxMessageBox(_T("�������ļ�"));
		return;
	}else{
		if(nListSel<30)//Y
			gstuTrdStat.Ysort[nListSel]=plusenum;
		str2.Format(_T("%d"),plusenum);
		str1.Format(_T("%d��"),nListSel);
		WritePrivateProfileString(_T("�ֵ�����"),str1,str2,gstuPathInf.csPathIni);  
		m_listDevice.SetItemText(nListSel, 1, str2);   
	}
	
	PosSet(0x02,char(nListSel),plusenum);
	
}


void CDevice::OnBnClickedChkBlow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_BLOW);
	int nStat = pChk->GetCheck();
	if (nStat)
	{
		BlowCtrl(1);
	} 
	else
	{
		BlowCtrl(0);
	}
}


void CDevice::OnBnClickedChkAlarm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_ALARM);
	int nStat = pChk->GetCheck();
	if (nStat)
	{
		AlarmCtrl(1);
	} 
	else
	{
		AlarmCtrl(0);
	}
}


void CDevice::OnBnClickedBtnClbpos1set()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValueX = GetDlgItemInt(IDC_EDT_X,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error X number"));
		return;
	}
	int nValueY = GetDlgItemInt(IDC_EDT_Y,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error Y number"));
		return;
	}
	SetDlgItemInt(IDC_EDT_CLBPOS1X,nValueX,1);
	SetDlgItemInt(IDC_EDT_CLBPOS1Y,nValueY,1);
	CString strValue = _T("");
	CFileFind findini;
	BOOL ifFind = findini.FindFile(gstuPathInf.csPathIni);  

	if( !ifFind )  
	{  
		AfxMessageBox(_T("�������ļ�"));
		return;
	}  

	strValue.Format(_T("%d"),nValueX);
	WritePrivateProfileString(_T("ȡƬУ׼��"),_T("1X"),strValue,gstuPathInf.csPathIni); 
	strValue.Format(_T("%d"),nValueY);
	WritePrivateProfileString(_T("ȡƬУ׼��"),_T("1Y"),strValue,gstuPathInf.csPathIni); 
	gstuRcgInfo.nClbPosX[0] = nValueX;
	gstuRcgInfo.nClbPosY[0] = nValueY;
}


void CDevice::OnBnClickedBtnClbpos2set()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValueX = GetDlgItemInt(IDC_EDT_X,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error X number"));
		return;
	}
	int nValueY = GetDlgItemInt(IDC_EDT_Y,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error Y number"));
		return;
	}
	SetDlgItemInt(IDC_EDT_CLBPOS2X,nValueX,1);
	SetDlgItemInt(IDC_EDT_CLBPOS2Y,nValueY,1);
	CString strValue = _T("");
	CFileFind findini;
	BOOL ifFind = findini.FindFile(gstuPathInf.csPathIni);  

	if( !ifFind )  
	{  
		AfxMessageBox(_T("�������ļ�"));
		return;
	}  

	strValue.Format(_T("%d"),nValueX);
	WritePrivateProfileString(_T("ȡƬУ׼��"),_T("2X"),strValue,gstuPathInf.csPathIni); 
	strValue.Format(_T("%d"),nValueY);
	WritePrivateProfileString(_T("ȡƬУ׼��"),_T("2Y"),strValue,gstuPathInf.csPathIni); 
	gstuRcgInfo.nClbPosX[1] = nValueX;
	gstuRcgInfo.nClbPosY[1] = nValueY;
}


void CDevice::OnBnClickedBtnClbpos3set()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValueX = GetDlgItemInt(IDC_EDT_X,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error X number"));
		return;
	}
	int nValueY = GetDlgItemInt(IDC_EDT_Y,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error Y number"));
		return;
	}
	SetDlgItemInt(IDC_EDT_CLBPOS3X,nValueX,1);
	SetDlgItemInt(IDC_EDT_CLBPOS3Y,nValueY,1);
	CString strValue = _T("");
	CFileFind findini;
	BOOL ifFind = findini.FindFile(gstuPathInf.csPathIni);  

	if( !ifFind )  
	{  
		AfxMessageBox(_T("�������ļ�"));
		return;
	}  

	strValue.Format(_T("%d"),nValueX);
	WritePrivateProfileString(_T("ȡƬУ׼��"),_T("3X"),strValue,gstuPathInf.csPathIni); 
	strValue.Format(_T("%d"),nValueY);
	WritePrivateProfileString(_T("ȡƬУ׼��"),_T("3Y"),strValue,gstuPathInf.csPathIni); 
	gstuRcgInfo.nClbPosX[2] = nValueX;
	gstuRcgInfo.nClbPosY[2] = nValueY;
}


void CDevice::OnBnClickedBtnCom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	gclsCom.CloseCom();
	CString l_str;
	int l_int=GetDlgItemInt(IDC_EDT_COM,0,0);
	l_str.Format(_T("%d"),l_int);
	gclsCom.OpenCom(l_int);
	if (gclsCom.stuInf.bComOpen)
	{
		AfxMessageBox(_T("��COM��������"));
		WritePrivateProfileString(_T("COM"),_T("�ں�"),l_str,gstuPathInf.csPathIni);  
	}
	
}


void CDevice::OnBnClickedBtnListtest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString csTmp = m_listDevice.GetItemText(nListSel,1);   
	int nTmp=_wtoi(csTmp);
	AxisMove(0x04,nTmp,0);
	
}


void CDevice::OnBnClickedBtnSortuptest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValue = GetDlgItemInt(IDC_EDT_SORTUP,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error number"));
		return;
	}
	AxisMove(0x03,nValue,0);
}


void CDevice::OnBnClickedBtnSortintest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValue = GetDlgItemInt(IDC_EDT_SORTIN,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error number"));
		return;
	}
	AxisMove(0x03,nValue,0);
}


void CDevice::OnBnClickedBtnSortouttest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValue = GetDlgItemInt(IDC_EDT_SORTOUT,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error number"));
		return;
	}
	AxisMove(0x03,nValue,0);
}


void CDevice::OnBnClickedBtnSortdowntest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValue = GetDlgItemInt(IDC_EDT_SORTDOWN,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error number"));
		return;
	}
	AxisMove(0x03,nValue,0);
}


void CDevice::OnBnClickedBtnXy1test()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValueX = GetDlgItemInt(IDC_EDT_XY1X,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error X number"));
		return;
	}
	int nValueY = GetDlgItemInt(IDC_EDT_XY1Y,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error Y number"));
		return;
	}
	XYMove(nValueX,nValueY);
}


void CDevice::OnBnClickedBtnXy2test()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValueX = GetDlgItemInt(IDC_EDT_XY2X,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error X number"));
		return;
	}
	int nValueY = GetDlgItemInt(IDC_EDT_XY2Y,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error Y number"));
		return;
	}
	XYMove(nValueX,nValueY);
}


void CDevice::OnBnClickedBtnXy3test()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValueX = GetDlgItemInt(IDC_EDT_XY3X,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error X number"));
		return;
	}
	int nValueY = GetDlgItemInt(IDC_EDT_XY3Y,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error Y number"));
		return;
	}
	XYMove(nValueX,nValueY);
}


void CDevice::OnBnClickedBtnClbpos1test()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValueX = GetDlgItemInt(IDC_EDT_CLBPOS1X,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error X number"));
		return;
	}
	int nValueY = GetDlgItemInt(IDC_EDT_CLBPOS1Y,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error Y number"));
		return;
	}
	XYMove(nValueX,nValueY);
}


void CDevice::OnBnClickedBtnClbpos2test()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValueX = GetDlgItemInt(IDC_EDT_CLBPOS2X,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error X number"));
		return;
	}
	int nValueY = GetDlgItemInt(IDC_EDT_CLBPOS2Y,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error Y number"));
		return;
	}
	XYMove(nValueX,nValueY);
}


void CDevice::OnBnClickedBtnClbpos3test()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValueX = GetDlgItemInt(IDC_EDT_CLBPOS3X,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error X number"));
		return;
	}
	int nValueY = GetDlgItemInt(IDC_EDT_CLBPOS3Y,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error Y number"));
		return;
	}
	XYMove(nValueX,nValueY);
}


void CDevice::OnCbnSelchangeCmbEfgstasel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	gstuRun.nEfgStaSel = m_cmb_efgstasel.GetCurSel();
	CString strValue;
	strValue.Format(_T("%d"),gstuRun.nEfgStaSel);
	WritePrivateProfileString(_T("EFG�趨"),_T("��ʼ���λ��"),strValue,gstuPathInf.csPathIni);
	EFGStaSel(char(gstuRun.nEfgStaSel));
}


//void CDevice::OnBnClickedBtnOpenefg()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	EFGCut(false);
//	//TimeRecord(1,1);
//}


//void CDevice::OnBnClickedBtnCloseefg()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	EFGCut(true);
//	//TimeRecord(1,0);
//}


void CDevice::OnBnClickedBtnStmresta()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	StmReSta();
	
}


void CDevice::OnBnClickedBtnSnchgset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValueX = GetDlgItemInt(IDC_EDT_X,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error X number"));
		return;
	}
	int nValueY = GetDlgItemInt(IDC_EDT_Y,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error Y number"));
		return;
	}
	SetDlgItemInt(IDC_EDT_SNCHGX,nValueX,1);
	SetDlgItemInt(IDC_EDT_SNCHGY,nValueY,1);
	CString strValue = _T("");
	CFileFind findini;
	BOOL ifFind = findini.FindFile(gstuPathInf.csPathIni);  

	if( !ifFind )  
	{  
		AfxMessageBox(_T("�������ļ�"));
		return;
	}  

	strValue.Format(_T("%d"),nValueX);
	WritePrivateProfileString(_T("����ͷ��"),_T("X"),strValue,gstuPathInf.csPathIni); 
	strValue.Format(_T("%d"),nValueY);
	WritePrivateProfileString(_T("����ͷ��"),_T("Y"),strValue,gstuPathInf.csPathIni); 
	gstuRcgInfo.nSNCHGX = nValueX;
	gstuRcgInfo.nSNCHGY = nValueY;
}


void CDevice::OnBnClickedBtnSnchgtest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bFlag=0;
	int nValueX = GetDlgItemInt(IDC_EDT_SNCHGX,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error X number"));
		return;
	}
	int nValueY = GetDlgItemInt(IDC_EDT_SNCHGY,&bFlag,1);
	if(0 == bFlag)
	{
		AfxMessageBox(_T("error Y number"));
		return;
	}
	XYMove(nValueX,nValueY);
}


void CDevice::OnBnClickedBtnSnchgm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	XYMove(gstuRcgInfo.nSNCHGX,gstuRcgInfo.nSNCHGY);
}


void CDevice::OnEnChangeEdtSnchgx()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
}
