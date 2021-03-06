// DlgRun1.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgRun1.h"
#include "MainFrm.h"
#include "EfgAlg.h"
#include "DlgCamera.h"
#include "DlgDebug1.h"

CWinThread* gTrdMainRun = NULL;
CWinThread* gTrdXRun = NULL;
CWinThread* gTrdYRun = NULL;
CWinThread* gTrdTurntableRun = NULL;
CWinThread* gTrdStaticMeasure = NULL;
CWinThread* gTrdCamera = NULL;
CWinThread* gTrdTransfer = NULL;
//=====================================================================正常测量========================================================================
UINT Thread_YRun(LPVOID pParam)
{
  SetThreadAffinityMask(GetCurrentThread(), 1);
  CDlgRun1* pdlg = (CDlgRun1*)pParam;

  while (1)
  {
    while (PAUSE == pdlg->m_run.state.mainrun)
    {
      Sleep(1);
    }

    switch (pdlg->m_run.state.y) {
    case START://拿料
      if (PAUSE == pdlg->m_run.state.turntable)//如果y停止，启动y
      {
        while (-1 == pdlg->m_io->StopMeasure());//停止测量
        Sleep(100);
        pdlg->m_io->MotoZero(MOTOR_Y);//回零
        pdlg->m_io->WriteDo(Y_NOZZLE, IO_ON);//打开吸嘴
        Sleep(pdlg->m_param->user_config.time.y_on);//延时
        pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_param->user_config.pos.y_wait);//移动到等待位置
        pdlg->m_run.state.turntable = END;
        Sleep(100);
        pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_result->measure.cur_pos_step);//移动到档位

        pdlg->m_run.state.y = END;
        pdlg->m_io->WriteDo(Y_NOZZLE, IO_OFF);//关闭吸嘴
        Sleep(pdlg->m_param->user_config.time.y_off);//延时
        pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_param->user_config.pos.y_wait);//移动到等待位置
      }
      break;
    case STOP:
      pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_param->user_config.pos.y_wait);//移动到等待位置
      pdlg->m_io->WriteDo(Y_NOZZLE, IO_OFF);//关闭吸嘴
      pdlg->m_run.state.y = END;
      return 0;
    default:
      Sleep(1);
      break;
      //gTrdYRun->SuspendThread();
    }
  }

  return 0;
}

UINT Thread_TurntableRun(LPVOID pParam)
{
  if (0 == SetThreadAffinityMask(GetCurrentThread(), 1))
    SetThreadAffinityMask(GetCurrentThread(), 1);

  CDlgRun1* pdlg = (CDlgRun1*)pParam;
  int n = pdlg->m_io->m_configParam.user_config.measure.cnt;//测量次数
  int cnt = 0;

  while (1)
  {
    switch (pdlg->m_run.state.turntable) {
    case START://测量
      pdlg->m_io->WriteDo(BLOW, IO_ON);//三孔吸
      if (GetMainFrame()->StartMeasure(pdlg->m_param->laser.out3, pdlg->m_param->laser.out6)) {
        Sleep(50);
        pdlg->m_run.state.turntable = RUNNING;
      }
      break;
    case RUNNING:
      if (GetMainFrame()->CheckMeasure()) {
        int ret = pdlg->FitAndUpdate(++cnt);

        if (ret == -1) {//出错，重新测
          cnt = 0;
        }
        //测量多圈逻辑
        if (cnt == n) {
          pdlg->m_run.state.turntable = WAIT;
          cnt = 0;
        }
        else {
          pdlg->m_run.state.turntable = START;
          //if(ret == -1)
          // cnt = 0;
        }
      }
      break;
    case WAIT:
      pdlg->m_io->WriteDo(BLOW, IO_OFF);//三孔吸
      if (END == pdlg->m_run.state.y)//如果y停止，启动y
      {
        //GetMainFrame()->m_diIntCounterSnap.CalcResult();//测量完成，等待y轴运行完毕，进行计算
        pdlg->CalcResult();
        //pdlg->m_io->GetCurOffPos();//得到当前的下料位置
        //pdlg->ShowResult(1);

        pdlg->m_run.state.turntable = PAUSE;
        pdlg->m_run.state.y = START;
        //gTrdYRun->ResumeThread();
      }
      break;
    case PAUSE:
      break;
    case STOP:
      while (!GetMainFrame()->CheckMeasure());
      pdlg->m_run.state.turntable = END;
      return 0;
    default:
      Sleep(1);
      break;
      //gTrdTurntableRun->SuspendThread();
    }
  }

  return 0;
}

UINT Thread_XRun(LPVOID pParam)
{
  SetThreadAffinityMask(GetCurrentThread(), 1);
  CDlgRun1* pdlg = (CDlgRun1*)pParam;

  while (1)
  {
    while (PAUSE == pdlg->m_run.state.mainrun)
    {
      Sleep(1);
    }

    switch (pdlg->m_run.state.x)
    {
    case START://等料
     // if (START == pdlg->m_run.state.user/*pdlg->m_io->ReadDi(READY)*/) {//如果有料
	  if (pdlg->m_run.transfer[TRANSFER_TAKE]==1) {
        pdlg->m_run.state.user = END;//取料标记清零
        pdlg->m_io->MotoZero(MOTOR_X);//回零
        pdlg->m_io->WriteDo(X_NOZZLE, IO_ON);//打开吸嘴
        Sleep(pdlg->m_param->user_config.time.x_on);//延时
        pdlg->m_io->MotoRun(MOTOR_X, pdlg->m_param->user_config.pos.x_wait);//移动到等待位置
        pdlg->m_run.transfer[TRANSFER_TAKE] = 0;//已取，中转位清零
        pdlg->m_run.state.x = WAIT;
      }
      break;
    case WAIT://等待上料
      if (END == pdlg->m_run.state.turntable)
      {
        pdlg->m_io->MotoRun(MOTOR_X, pdlg->m_param->user_config.pos.x_off);//移动到三孔支架
        pdlg->m_io->WriteDo(X_NOZZLE, IO_OFF);//关闭吸嘴
        Sleep(pdlg->m_param->user_config.time.x_off);//延时
        pdlg->m_io->MotoRunNoWait(MOTOR_X, pdlg->m_param->user_config.pos.x_wait);//移动到等待位置

        pdlg->m_run.state.x = START;
        pdlg->m_run.state.turntable = START;

        //gTrdTurntableRun->ResumeThread();
      }
      else
        Sleep(1);
      break;
    case STOP:
      pdlg->m_io->MotoZero(MOTOR_X);//回零
      pdlg->m_io->WriteDo(X_NOZZLE, IO_OFF);//关闭吸嘴
      pdlg->m_run.state.x = END;
      return 0;
    default:
      return 0;
    }
  }

  return 0;
}
//============================================================静态测量专属=======================================================================
UINT Thread_StaticMeasure(LPVOID pParam)
{
  if (0 == SetThreadAffinityMask(GetCurrentThread(), 1))
    SetThreadAffinityMask(GetCurrentThread(), 1);
  CDlgRun1* pdlg = (CDlgRun1*)pParam;

  while (1)
  {
    switch (pdlg->m_run.state.staticmeasure)
    {
    case START:
      if (END == pdlg->m_run.state.turntable)
      {
        pdlg->m_run.state.turntable = START;
        //gTrdTurntableRun->ResumeThread();
      }

      else if (START == pdlg->m_run.state.y)//如果y停止，启动y
      {
        pdlg->m_run.state.turntable = END;
        Sleep(pdlg->m_param->user_config.time.y_off);//延时
        pdlg->m_run.state.y = END;
      }
      else
        Sleep(1);
      break;
    case STOP:
      pdlg->m_run.state.staticmeasure = END;
      return 0;
    default:
      return 0;

    }

  }
  return 0;
}

//============================================================正反专属=======================================================================
UINT Thread_PNYRun(LPVOID pParam)
{
  SetThreadAffinityMask(GetCurrentThread(), 1);
  CDlgRun1* pdlg = (CDlgRun1*)pParam;

  while (1)
  {
    while (PAUSE == pdlg->m_run.state.mainrun)
    {
      Sleep(1);
    }

    switch (pdlg->m_run.state.y) {
    case START://拿料
      if (PAUSE == pdlg->m_run.state.turntable)//如果y停止，启动y
      {
        while (-1 == pdlg->m_io->StopMeasure());//停止测量
        Sleep(100);
        pdlg->m_io->MotoZero(MOTOR_Y);//回零
        pdlg->m_io->WriteDo(Y_NOZZLE, IO_ON);//打开吸嘴
        Sleep(pdlg->m_param->user_config.time.y_on);//延时
        pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_param->user_config.pos.y_wait);//移动到等待位置
        pdlg->m_run.state.turntable = END;
        Sleep(100);
        // 正反记号和放片
        pdlg->m_result->measure.pn = -1;
        switch (pdlg->m_result->measure.pn) {
        case 1:
          pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_io->GetYOffPos(12));//移动到档位

          pdlg->m_io->WriteDo(Y_NOZZLE, IO_OFF);//放片
          Sleep(pdlg->m_param->user_config.time.y_on);//延时

          pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_io->GetYOffPos(6));//后退

          pdlg->m_io->WriteDo(AIR_FLAG1, IO_ON);//点标记
          Sleep(pdlg->m_param->user_config.time.y_off);
          pdlg->m_io->WriteDo(AIR_FLAG1, IO_OFF);

          pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_io->GetYOffPos(12));//移动到档位
          pdlg->m_io->WriteDo(Y_NOZZLE, IO_ON);//吸片
          Sleep(pdlg->m_param->user_config.time.y_on);//延时

          pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_io->GetYOffPos(19));//移动到档位
          break;
        case -1:
          pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_io->GetYOffPos(9));//移动到档位

          pdlg->m_io->WriteDo(Y_NOZZLE, IO_OFF);//放片
          Sleep(pdlg->m_param->user_config.time.y_on);//延时

          pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_io->GetYOffPos(6));//后退

          pdlg->m_io->WriteDo(AIR_FLAG2, IO_ON);
          Sleep(pdlg->m_param->user_config.time.y_off);
          pdlg->m_io->WriteDo(AIR_FLAG2, IO_OFF);

          pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_io->GetYOffPos(9));//移动到档位
          pdlg->m_io->WriteDo(Y_NOZZLE, IO_ON);//吸片
          Sleep(pdlg->m_param->user_config.time.y_on);//延时

          pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_io->GetYOffPos(18));//移动到档位
          break;
        case 0:
          pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_io->GetYOffPos(30));//移动到档位
          break;
        }


        pdlg->m_run.state.y = END;
        pdlg->m_io->WriteDo(Y_NOZZLE, IO_OFF);//关闭吸嘴
        Sleep(pdlg->m_param->user_config.time.y_off);//延时
        pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_param->user_config.pos.y_wait);//移动到等待位置
      }
      break;
    case STOP:
      pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_param->user_config.pos.y_wait);//移动到等待位置
      pdlg->m_io->WriteDo(Y_NOZZLE, IO_OFF);//关闭吸嘴
      pdlg->m_run.state.y = END;
      return 0;
    default:
      Sleep(1);
      break;
      //gTrdYRun->SuspendThread();
    }
  }

  return 0;
}

UINT Thread_PNXRun(LPVOID pParam)
{
  SetThreadAffinityMask(GetCurrentThread(), 1);
  CDlgRun1* pdlg = (CDlgRun1*)pParam;

  while (1)
  {
    while (PAUSE == pdlg->m_run.state.mainrun)
    {
      Sleep(1);
    }

    switch (pdlg->m_run.state.x)
    {
    case START://等料
      if (END == pdlg->m_run.state.turntable) {//如果测量完毕
        Sleep(pdlg->m_param->user_config.time.x_on);//延时
        pdlg->m_run.state.x = WAIT;
      }
      break;
    case WAIT://等待上料
      if (START == pdlg->m_run.state.user)//等待用户按键
      {
        //Sleep(pdlg->m_param->user_config.time.x_off);//延时
        pdlg->m_run.state.x = START;
        pdlg->m_run.state.turntable = START;//启动测量
        pdlg->m_run.state.user = END;
      }
      else
        Sleep(1);
      break;
    case STOP:
      pdlg->m_run.state.x = END;
      return 0;
    default:
      return 0;
    }
  }

  return 0;
}

// ========================================================================其他===================================================================
UINT Thread_MainRun(LPVOID pParam)
{
  SetThreadAffinityMask(GetCurrentThread(), 1);
  CDlgRun1* pdlg = (CDlgRun1*)pParam;

  while (1)
  {
    switch (pdlg->m_run.state.mainrun) {
    case START://启动
      pdlg->SetDlgItemText(IDC_STATIC_MESSAGE, _T("正在启动"));
      //while (END != pdlg->m_run.state.x&&END != pdlg->m_run.state.y&&END != pdlg->m_run.state.turntable);
      ASSERT(!gTrdXRun && !gTrdYRun && !gTrdTurntableRun);
      pdlg->m_io->InitEfgIO();
      pdlg->m_run.state.x = START;
      pdlg->m_run.state.y = END;
      pdlg->m_run.state.turntable = END;
      pdlg->m_io->WriteDo(XRAY_GATE, IO_ON);//打开光门
	  memset(pdlg->m_run.transfer, 0, sizeof(int) * 6);

      if (-1 == pdlg->m_io->StartMeasure(1))//;//启动测量
      {
        pdlg->SetDlgItemText(IDC_STATIC_MESSAGE, _T("启动失败"));
        Sleep(1000);
        break;
      }


      gTrdXRun = AfxBeginThread(Thread_XRun, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
      ASSERT(gTrdXRun);
      gTrdXRun->m_bAutoDelete = FALSE;
      gTrdXRun->ResumeThread();

      //gTrdXRun->m_bAutoDelete = TRUE;
      //gTrdXRun->ResumeThread();

      gTrdYRun = AfxBeginThread(Thread_YRun, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
      ASSERT(gTrdYRun);
      gTrdYRun->m_bAutoDelete = FALSE;
      gTrdYRun->ResumeThread();
      //gTrdYRun->m_bAutoDelete = TRUE;
      //gTrdYRun->ResumeThread();

      gTrdTurntableRun = AfxBeginThread(Thread_TurntableRun, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
      ASSERT(gTrdTurntableRun);
      gTrdTurntableRun->m_bAutoDelete = FALSE;
      gTrdTurntableRun->ResumeThread();
      //gTrdTurntableRun->m_bAutoDelete = TRUE;
      //gTrdTurntableRun->ResumeThread();

      pdlg->m_run.state.mainrun = RUNNING;
      pdlg->SetDlgItemText(IDC_STATIC_MESSAGE, _T("正在运行"));
      ((CButton*)pdlg->GetDlgItem(IDC_BTN_CTRL))->EnableWindow(TRUE);
      break;
    case PAUSE:
      break;
    case STOP:
      pdlg->m_run.state.x = STOP;
      pdlg->m_run.state.y = STOP;
      pdlg->m_run.state.turntable = STOP;

      while (1)
      {
        DWORD dwCodeX, dwCodeY, dwCodeTurntable;
        GetExitCodeThread(gTrdXRun->m_hThread, &dwCodeX);
        GetExitCodeThread(gTrdYRun->m_hThread, &dwCodeY);
        GetExitCodeThread(gTrdTurntableRun->m_hThread, &dwCodeTurntable);

        if (dwCodeTurntable != STILL_ACTIVE && dwCodeY != STILL_ACTIVE && dwCodeX != STILL_ACTIVE)
          break;

        if (dwCodeX == STILL_ACTIVE && STOP != pdlg->m_run.state.x)
          pdlg->m_run.state.x = STOP;

        if (dwCodeY == STILL_ACTIVE && STOP != pdlg->m_run.state.y)
          pdlg->m_run.state.y = STOP;

        if (dwCodeTurntable == STILL_ACTIVE && STOP != pdlg->m_run.state.turntable)
          pdlg->m_run.state.turntable = STOP;

      };

      gTrdXRun->Delete();
      gTrdYRun->Delete();
      gTrdTurntableRun->Delete();
      gTrdXRun = NULL;
      gTrdYRun = NULL;
      gTrdTurntableRun = NULL;

      pdlg->m_io->WriteDo(XRAY_GATE, IO_OFF);//打开光门
      pdlg->SetDlgItemText(IDC_STATIC_MESSAGE, _T(""));


      ((CButton*)pdlg->GetDlgItem(IDC_BTN_CTRL))->EnableWindow(TRUE);
      return 0;
    case RUNNING:
      gTrdMainRun->SuspendThread();
      break;
    case STATIC_MEASURE_START:
      //pdlg->m_io->InitEfgIO();
      pdlg->SetDlgItemText(IDC_STATIC_MESSAGE, _T("启动静态测量"));
      //while (END != pdlg->m_run.state.staticmeasure&&END != pdlg->m_run.state.turntable);
      ASSERT(!gTrdStaticMeasure && !gTrdTurntableRun);
      memset(&pdlg->m_io->m_resultParam, 0, sizeof(pdlg->m_io->m_resultParam));
      pdlg->m_io->m_resultParam.measure.min_pluse_num = 100000;
      pdlg->m_run.state.staticmeasure = START;
      pdlg->m_run.state.y = END;
      pdlg->m_run.state.turntable = END;


      pdlg->m_io->WriteDo(XRAY_GATE, IO_ON);//打开光门
      if (-1 == pdlg->m_io->StartMeasure(1))//;//启动测量
      {
        pdlg->SetDlgItemText(IDC_STATIC_MESSAGE, _T("启动静态测量失败"));
        Sleep(1000);
        break;
      }



      //gTrdStaticMeasure = AfxBeginThread(Thread_StaticMeasure, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
      //gTrdStaticMeasure->m_bAutoDelete = TRUE;
      //gTrdStaticMeasure->ResumeThread();
      gTrdStaticMeasure = AfxBeginThread(Thread_StaticMeasure, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
      ASSERT(gTrdStaticMeasure);
      gTrdStaticMeasure->m_bAutoDelete = FALSE;
      gTrdStaticMeasure->ResumeThread();
      //gTrdTurntableRun = AfxBeginThread(Thread_TurntableRun, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
      //gTrdTurntableRun->m_bAutoDelete = TRUE;
      //gTrdTurntableRun->ResumeThread();

      gTrdTurntableRun = AfxBeginThread(Thread_TurntableRun, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
      ASSERT(gTrdTurntableRun);
      gTrdTurntableRun->m_bAutoDelete = FALSE;
      gTrdTurntableRun->ResumeThread();



      pdlg->m_run.state.mainrun = RUNNING;
      pdlg->SetDlgItemText(IDC_STATIC_MESSAGE, _T("正在静态测量"));
      ((CButton*)pdlg->GetDlgItem(IDC_CHK_STATICMEASURE))->EnableWindow(TRUE);
      break;
    case STATIC_MEASURE_STOP:
      //return 0;

      pdlg->m_run.state.staticmeasure = STOP;
      pdlg->m_run.state.turntable = STOP;
      //Sleep(1000);
      //   if (END != pdlg->m_run.state.staticmeasure)
      //   pdlg->m_run.state.staticmeasure = STOP;
      //if (END != pdlg->m_run.state.turntable)
      //   pdlg->m_run.state.turntable = STOP;
      //	
      //   //gTrdTurntableRun->ResumeThread();
      //while (!(END == pdlg->m_run.state.staticmeasure&&END == pdlg->m_run.state.turntable));
      while (1)
      {
        DWORD dwCodeStaticMeasure, dwCodeTurntable;
        GetExitCodeThread(gTrdStaticMeasure->m_hThread, &dwCodeStaticMeasure);
        GetExitCodeThread(gTrdTurntableRun->m_hThread, &dwCodeTurntable);

        if (dwCodeTurntable != STILL_ACTIVE && dwCodeStaticMeasure != STILL_ACTIVE)
          break;

        if (dwCodeStaticMeasure == STILL_ACTIVE && STOP != pdlg->m_run.state.staticmeasure)
          pdlg->m_run.state.staticmeasure = STOP;

        if (dwCodeTurntable == STILL_ACTIVE && STOP != pdlg->m_run.state.turntable)
          pdlg->m_run.state.turntable = STOP;

      };

      gTrdStaticMeasure->Delete();
      gTrdTurntableRun->Delete();
      gTrdStaticMeasure = NULL;
      gTrdTurntableRun = NULL;


      pdlg->m_io->WriteDo(XRAY_GATE, IO_OFF);//关闭光门

      pdlg->SetDlgItemText(IDC_STATIC_MESSAGE, _T(""));
      ((CButton*)pdlg->GetDlgItem(IDC_CHK_STATICMEASURE))->EnableWindow(TRUE);
      return 0;
    case PN_MEASURE_START:
      //pdlg->m_io->InitEfgIO();
      pdlg->SetDlgItemText(IDC_STATIC_MESSAGE, _T("启动正反测量"));
      //while (END != pdlg->m_run.state.staticmeasure&&END != pdlg->m_run.state.turntable);
      ASSERT(!gTrdStaticMeasure && !gTrdTurntableRun);
      memset(&pdlg->m_io->m_resultParam, 0, sizeof(pdlg->m_io->m_resultParam));
      pdlg->m_io->m_resultParam.measure.min_pluse_num = 100000;
      pdlg->m_run.state.x = WAIT;
      pdlg->m_run.state.y = END;
      pdlg->m_run.state.turntable = END;
      pdlg->m_run.state.user = END;


      pdlg->m_io->WriteDo(XRAY_GATE, IO_ON);//打开光门
      if (-1 == pdlg->m_io->StartMeasure(1))//;//启动测量
      {
        pdlg->SetDlgItemText(IDC_STATIC_MESSAGE, _T("启动正反测量失败"));
        Sleep(1000);
        break;
      }

      gTrdXRun = AfxBeginThread(Thread_PNXRun, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
      ASSERT(gTrdXRun);
      gTrdXRun->m_bAutoDelete = FALSE;
      gTrdXRun->ResumeThread();

      gTrdYRun = AfxBeginThread(Thread_PNYRun, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
      ASSERT(gTrdYRun);
      gTrdYRun->m_bAutoDelete = FALSE;
      gTrdYRun->ResumeThread();

      gTrdTurntableRun = AfxBeginThread(Thread_TurntableRun, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
      ASSERT(gTrdTurntableRun);
      gTrdTurntableRun->m_bAutoDelete = FALSE;
      gTrdTurntableRun->ResumeThread();

      pdlg->m_run.state.mainrun = RUNNING;
      pdlg->SetDlgItemText(IDC_STATIC_MESSAGE, _T("正在正反测量"));
      ((CButton*)pdlg->GetDlgItem(IDC_CHK_PNMEASURE))->EnableWindow(TRUE);
      break;
    case PN_MEASURE_STOP:
      //return 0;

      pdlg->m_run.state.x = STOP;
      pdlg->m_run.state.y = STOP;
      pdlg->m_run.state.turntable = STOP;
      //Sleep(1000);
      //   if (END != pdlg->m_run.state.staticmeasure)
      //   pdlg->m_run.state.staticmeasure = STOP;
      //if (END != pdlg->m_run.state.turntable)
      //   pdlg->m_run.state.turntable = STOP;
      //	
      //   //gTrdTurntableRun->ResumeThread();
      //while (!(END == pdlg->m_run.state.staticmeasure&&END == pdlg->m_run.state.turntable));
      while (1)
      {
        DWORD dwCodeX, dwCodeY, dwCodeTurntable;
        GetExitCodeThread(gTrdXRun->m_hThread, &dwCodeX);
        GetExitCodeThread(gTrdYRun->m_hThread, &dwCodeY);
        GetExitCodeThread(gTrdTurntableRun->m_hThread, &dwCodeTurntable);

        if (dwCodeTurntable != STILL_ACTIVE && dwCodeX != STILL_ACTIVE && dwCodeY != STILL_ACTIVE)
          break;

        if (dwCodeX == STILL_ACTIVE && STOP != pdlg->m_run.state.staticmeasure)
          pdlg->m_run.state.x = STOP;

        if (dwCodeY == STILL_ACTIVE && STOP != pdlg->m_run.state.staticmeasure)
          pdlg->m_run.state.y = STOP;

        if (dwCodeTurntable == STILL_ACTIVE && STOP != pdlg->m_run.state.turntable)
          pdlg->m_run.state.turntable = STOP;

      };

      gTrdXRun->Delete();
      gTrdYRun->Delete();
      gTrdTurntableRun->Delete();
      gTrdXRun = NULL;
      gTrdYRun = NULL;
      gTrdTurntableRun = NULL;


      pdlg->m_io->WriteDo(XRAY_GATE, IO_OFF);//关闭光门

      pdlg->SetDlgItemText(IDC_STATIC_MESSAGE, _T(""));
      ((CButton*)pdlg->GetDlgItem(IDC_CHK_PNMEASURE))->EnableWindow(TRUE);
      return 0;
    default:
      return 0;
    }
  }

  return 0;
}
CCamera* pdlg_cam = NULL;
// ========================================================================相机上料===================================================================
UINT Thread_Camera(LPVOID pParam)
{
  SetThreadAffinityMask(GetCurrentThread(), 1);
  CDlgRun1* pdlg = (CDlgRun1*)pParam;
  //CCamera* pdlg_cam = ((CDlgDebug1*)(GetMainFrame()->m_userFrame->m_splitUser.GetPane(0, 1)))->m_dlg_camera;
  char l_AnsiStr[MAX_PATH];
  int nWidth, nHeight;
  int nBitCount;
  int nBitsPerSample;
  int i = 0;
  BOOL shake = FALSE;
  NpcParm par;
  //POINT pPoint[5];
  Sleep(1000);
  // 两次回零
  pdlg->m_io->MotoZeroNoWait(MOTOR_A);
  pdlg->m_io->MotoZeroNoWait(MOTOR_B);
  while (!pdlg->m_io->CheckMotoEnd(MOTOR_A)) { Sleep(100); };
  while (!pdlg->m_io->CheckMotoEnd(MOTOR_B)) { Sleep(100); };

	  
  pdlg->m_io->MotoZeroNoWait(MOTOR_A);
  pdlg->m_io->MotoZeroNoWait(MOTOR_B);
  while (!pdlg->m_io->CheckMotoEnd(MOTOR_A)) { Sleep(100); };
  while (!pdlg->m_io->CheckMotoEnd(MOTOR_B)) { Sleep(100); };

  pdlg_cam->m_rcg.g_stu_square.nN = 0;

  while (1)
  {
    switch (pdlg->m_run.state.camera) {
    case START://启动

	  if(shake == TRUE){
		pdlg->m_io->MotoShakeNoWait(MOTOR_D, 300);
        while (!pdlg->m_io->CheckMotoEnd(MOTOR_D)) { Sleep(1); };
		/*pdlg->m_io->MotoRunNoWait(MOTOR_D, 300);
        while (!pdlg->m_io->CheckMotoEnd(MOTOR_D)) { Sleep(1); };*/
		//break;
	  }
	  Sleep(500);
	  shake = TRUE;

      WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, GetMainFrame()->m_exe_path + _T("\\PIC\\原图0.bmp"), -1, l_AnsiStr, sizeof(l_AnsiStr), NULL, NULL);
 //     if (nullptr == pdlg_cam->m_ksj.SnapEx(pdlg_cam->m_ch, &nWidth, &nHeight, &nBitCount, &nBitsPerSample, l_AnsiStr, NULL))
      if (-1 == pdlg_cam->m_cam.captureBmp(l_AnsiStr))
      {
        Sleep(500);

        if (i++ > 10)//取不到图像，退出
        {
          AfxMessageBox(_T("取不到相机图像，请检查相机"));
          i = 0;
          break;
        }
      }
      //m_rcg.RCGBMPSPLIT(NULL, NULL, l_AnsiStr, m_param->camera.nPToL, m_param->camera.nDefectPToL, m_param->camera.nThreshold, 0, m_param->camera.bIsCir, m_param->camera.bThrdAuto, m_param->camera.bDelNoise);
      pdlg_cam->m_rcg.RCGBMP(NULL, NULL, l_AnsiStr, pdlg_cam->m_param->camera.nPToL, pdlg_cam->m_param->camera.nDefectPToL, pdlg_cam->m_param->camera.nThreshold, 0, pdlg_cam->m_param->camera.bIsCir, pdlg_cam->m_param->camera.bThrdAuto, pdlg_cam->m_param->camera.bDelNoise);
      //
	  if(pdlg_cam->m_rcg.g_stu_square.nN > 0) {
        pdlg->m_run.state.camera = RUNNING;
	  } 
	  else {
		/*pdlg->m_io->MotoShakeNoWait(MOTOR_D, 300);
        while (!pdlg->m_io->CheckMotoEnd(MOTOR_D)) { Sleep(1); };
		pdlg->m_io->MotoRunNoWait(MOTOR_D, 300);
        while (!pdlg->m_io->CheckMotoEnd(MOTOR_D)) { Sleep(1); };*/
		break;
	  }

      i = -1;
      break;
    case RUNNING:
      i++;
      if (i >= pdlg_cam->m_rcg.g_stu_square.nN) {
        pdlg->m_run.state.camera = START;
        break;
      }

      if (
        pdlg_cam->m_rcg.g_stu_square.pnZPX[i] < pdlg_cam->m_npc_inf.left ||
        pdlg_cam->m_rcg.g_stu_square.pnZPX[i] > pdlg_cam->m_npc_inf.right ||
        pdlg_cam->m_rcg.g_stu_square.pnZPY[i] < pdlg_cam->m_npc_inf.top ||
        pdlg_cam->m_rcg.g_stu_square.pnZPY[i] > pdlg_cam->m_npc_inf.bottom
        ) 
      {
        break;
      }
      //长判断
      if (pdlg_cam->m_rcg.g_stu_square.pnLen[i] < (pdlg_cam->m_rcg.stuRef.Len - pdlg_cam->m_rcg.stuRef.Dev) || pdlg_cam->m_rcg.g_stu_square.pnLen[i] > (pdlg_cam->m_rcg.stuRef.Len + pdlg_cam->m_rcg.stuRef.Dev))
      {
        break;
      }
      //宽判断
      if (pdlg_cam->m_rcg.g_stu_square.pnWth[i] < (pdlg_cam->m_rcg.stuRef.Wth - pdlg_cam->m_rcg.stuRef.Dev) || pdlg_cam->m_rcg.g_stu_square.pnWth[i] > (pdlg_cam->m_rcg.stuRef.Wth + pdlg_cam->m_rcg.stuRef.Dev))
      {
        break;
      }



	  shake = FALSE;
      //for (int j = 0; j < 4; j++)
      //{
      //  pPoint[j].x = pdlg_cam->m_rcg.g_stu_square.pnPX[i][j];
      //  pPoint[j].y = pdlg_cam->m_rcg.g_stu_square.pnPY[i][j];
      //}
      //pPoint[4].x = pdlg_cam->m_rcg.g_stu_square.pnPX[i][0];
      //pPoint[4].y = pdlg_cam->m_rcg.g_stu_square.pnPY[i][0];

      //test
      //double angle_test = m_rcg.CalculateVectorAngle(pPoint[1].x - pPoint[0].x,
      //  pPoint[1].y - pPoint[0].y, 1, 0);
      /*int nLenNo = m_rcg.g_stu_square.lenNo1PN[i];
      int nLenNoNext = (3 == nLenNo) ? 0 : (nLenNo + 1);
      */

      par.deg0 = pdlg_cam->m_rcg.g_stu_square.angreePN[i];// angle_test;
      par.x0 = pdlg_cam->m_rcg.g_stu_square.pnZPX[i];
      par.y0 = pdlg_cam->m_rcg.g_stu_square.pnZPY[i];
      par.pn0 = pdlg_cam->m_rcg.g_stu_square.bPN[i];//m_param->camera.nPN;m_rcg.g_stu_square.bPN[i]

      //CString csTmp;
      //csTmp.Format(_T("长%d 宽%d 方向 %d 角度%.1f X:%d Y:%d"), m_rcg.g_stu_square.pnLen[i], m_rcg.g_stu_square.pnWth[i], (int)(par.pn0), par.deg0, (int)(par.x0), (int)(par.y0));
      //pdlg->GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);
      pdlg_cam->TranNpcParam(&par);

      //csTmp.Format(_T("No%d 方向 %d 角度%d X:%d Y:%d"), i, par.pn, par.deg, par.x, par.y);
      //pdlg->GetDlgItem(IDC_SELECT_XY)->SetWindowText(csTmp);

      pdlg->m_io->MotoRunNoWait(MOTOR_A, par.x);
      pdlg->m_io->MotoRunNoWait(MOTOR_B, par.y);
      while (!pdlg->m_io->CheckMotoEnd(MOTOR_A)) { Sleep(100); };
      while (!pdlg->m_io->CheckMotoEnd(MOTOR_B)) { Sleep(100); };
      //while (0 != g_dlgDevice->ParamMove(par.x, par.y, par.deg, par.pn))
      //{
      //  Sleep(1000);
      //}
      pdlg->m_io->WriteDo(AB_CYLINDER, IO_ON);//打开气缸
      pdlg->m_io->WriteDo(AB_NOZZLE, IO_ON);//打开吸嘴

      Sleep(pdlg->m_param->user_config.time.x_on);//延时

      pdlg->m_io->WriteDo(AB_CYLINDER, IO_OFF);//关闭气缸


      pdlg->m_io->MotoRunNoWait(MOTOR_A, pdlg_cam->m_param->camera.transferx);
      pdlg->m_io->MotoRunNoWait(MOTOR_B, pdlg_cam->m_param->camera.transfery);
      while (!pdlg->m_io->CheckMotoEnd(MOTOR_A)) { Sleep(100); };
      while (!pdlg->m_io->CheckMotoEnd(MOTOR_B)) { Sleep(100); };
      pdlg->m_run.state.camera = PAUSE;
      break;
    case PAUSE:
      if (0 == pdlg->m_run.transfer[0])//等待中转盘0位空
        pdlg->m_run.state.camera = WAIT;
      break;
    case  WAIT: // 等待放到中转
      if (pdlg->m_run.state.transfer == WAIT) 
      {
        pdlg->m_io->WriteDo(AB_NOZZLE, IO_OFF);//关闭吸嘴

        Sleep(pdlg->m_param->user_config.time.x_off);//延时
		pdlg->m_run.transfer[0]=1;
        pdlg->m_run.state.camera = RUNNING;

		//pdlg->m_io->MotoZero(MOTOR_A);
		//	pdlg->m_io->MotoZero(MOTOR_B);
		     pdlg->m_io->MotoZeroNoWait(MOTOR_A);
      pdlg->m_io->MotoZeroNoWait(MOTOR_B);
      while (!pdlg->m_io->CheckMotoEnd(MOTOR_A)) { Sleep(100); };
      while (!pdlg->m_io->CheckMotoEnd(MOTOR_B)) { Sleep(100); };

      }
      break;
    default:
      return 0;
    }
  }

  return 0;
}
// ========================================================================相机上料===================================================================
UINT Thread_Transfer(LPVOID pParam)
{
  SetThreadAffinityMask(GetCurrentThread(), 1);
  CDlgRun1* pdlg = (CDlgRun1*)pParam;
  const int tmp[6] = { 0 };
  pdlg->m_io->MotoZero(MOTOR_C);

  while (1)
  {
    switch (pdlg->m_run.state.transfer) {
    case START://启动
      memset(pdlg->m_run.transfer, 0, sizeof(int) * 6);
      pdlg->m_run.state.transfer = PAUSE;
      break;
    case RUNNING://转60°
      pdlg->m_io->MotoZero(MOTOR_C);
		//pdlg->m_io->MotoRun(MOTOR_C, 500);
      memcpy(pdlg->m_run.transfer + 1, pdlg->m_run.transfer, sizeof(int) * 5);
      pdlg->m_run.transfer[0] = 0;

      if (1 == pdlg->m_run.transfer[TRANSFER_TAKE])
        pdlg->m_run.state.user = START;//有片通知X臂

      pdlg->m_run.state.transfer = PAUSE;
      break;
    case PAUSE://等待事件
      if (pdlg->m_run.state.camera == WAIT)
        pdlg->m_run.state.transfer = WAIT;

      else if(0 == pdlg->m_run.transfer[TRANSFER_TAKE] && 0 != memcmp(pdlg->m_run.transfer, tmp,sizeof(int)*6))
        pdlg->m_run.state.transfer = RUNNING;

      break;
    case WAIT: // 等待上料
      if (pdlg->m_run.state.camera != WAIT)
        pdlg->m_run.state.transfer = PAUSE;

      break;
    default:
      return 0;
    }
  }

  return 0;
}
// CDlgRun1

IMPLEMENT_DYNCREATE(CDlgRun1, CFormView)

CDlgRun1::CDlgRun1()
  : CFormView(CDlgRun1::IDD)
{
  memset(&m_run, 0, sizeof(m_run));
  m_run.state.x = END;
  m_run.state.y = END;
  m_run.state.turntable = END;
  m_run.state.staticmeasure = END;
}

CDlgRun1::~CDlgRun1()
{
  m_laserdc->SelectObject(m_laseroldbm);
  m_laserbm->DeleteObject();
  m_laserdc->DeleteDC();

  m_xraydc->SelectObject(m_xrayoldbm);
  m_xraybm->DeleteObject();
  m_xraydc->DeleteDC();

}

void CDlgRun1::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_GRID_SORT, m_gridSort);
  DDX_Control(pDX, IDC_BTN_CTRL, m_btn_ctrl);
  DDX_Control(pDX, IDC_TAB_PREVIEW, m_tab_preview);
  DDX_Control(pDX, IDC_STATIC_RESULT, m_static_result);
  DDX_Control(pDX, IDC_STATIC_SORT, m_static_sort);
  DDX_Control(pDX, IDC_STATIC_MESSAGE, m_static_message);
}

BEGIN_MESSAGE_MAP(CDlgRun1, CFormView)
  ON_WM_CTLCOLOR()
  //  ON_BN_CLICKED(IDC_SORT_SET, &CDlgRun1::OnBnClickedSortSet)
  ON_BN_CLICKED(IDC_EDT_RUN_SET, &CDlgRun1::OnBnClickedEdtRunSet)
  ON_CBN_SELCHANGE(IDC_CMB_PRIMARY, &CDlgRun1::OnCbnSelchangeCmbPrimary)
  ON_EN_CHANGE(IDC_EDT_CARD, &CDlgRun1::OnEnChangeEdtCard)
  ON_CBN_SELCHANGE(IDC_CMB_SECONDARY, &CDlgRun1::OnCbnSelchangeCmbSecondary)
  ON_EN_CHANGE(IDC_EDT_CENTER_DEGREE, &CDlgRun1::OnEnChangeEdtCenterDegree)
  ON_EN_CHANGE(IDC_EDT_STEP_DEGREE, &CDlgRun1::OnEnChangeEdtStepDegree)
  ON_EN_CHANGE(IDC_EDT_MIN_DEGREE, &CDlgRun1::OnEnChangeEdtMinDegree)
  ON_EN_CHANGE(IDC_EDT_MAX_DEGREE, &CDlgRun1::OnEnChangeEdtMaxDegree)
  ON_EN_CHANGE(IDC_EDT_EQU_PHI, &CDlgRun1::OnEnChangeEdtEquPhi)
  ON_EN_CHANGE(IDC_EDT_EQU_FACTOR, &CDlgRun1::OnEnChangeEdtEquFactor)
  ON_BN_CLICKED(IDC_BTN_CTRL, &CDlgRun1::OnBnClickedBtnCtrl)
  ON_BN_CLICKED(IDC_CHK_PAUSE, &CDlgRun1::OnBnClickedChkPause)
  ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PREVIEW, &CDlgRun1::OnTcnSelchangeTabPreview)
  ON_BN_CLICKED(IDC_CHK_STATICMEASURE, &CDlgRun1::OnBnClickedChkStaticmeasure)
  ON_BN_CLICKED(IDC_CHK_PNMEASURE, &CDlgRun1::OnBnClickedChkPnmeasure)
  ON_BN_CLICKED(IDC_CHK_CAMERA, &CDlgRun1::OnBnClickedChkCamera)
END_MESSAGE_MAP()


// CDlgRun1 诊断

#ifdef _DEBUG
void CDlgRun1::AssertValid() const
{
  CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDlgRun1::Dump(CDumpContext& dc) const
{
  CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDlgRun1 消息处理程序


HBRUSH CDlgRun1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

  // TODO:  在此更改 DC 的任何特性
  if (pWnd == this) {
    return m_brush;
  }
  else if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_BTN /*|| nCtlColor == CTLCOLOR_MAX*/)
  {
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(0, 0, 0));
    if (pWnd->GetDlgCtrlID() == IDC_STATIC || pWnd->GetDlgCtrlID() == IDC_STATIC_SORT || pWnd->GetDlgCtrlID() == IDC_STATIC_RESULT || pWnd->GetDlgCtrlID() == IDC_STATIC_MESSAGE)
      return (HBRUSH)::GetStockObject(NULL_BRUSH);
  }
  // TODO:  如果默认的不是所需画笔，则返回另一个画笔

  return hbr;
}


void CDlgRun1::OnInitialUpdate()
{
  CFormView::OnInitialUpdate();

  m_param = &GetMainFrame()->m_efgio.m_configParam;
  m_io = &GetMainFrame()->m_efgio;
  m_result = &GetMainFrame()->m_efgio.m_resultParam;

  // TODO: 在此添加专用代码和/或调用基类
  //combobox必须放在tab控件之后
  ((CComboBox*)GetDlgItem(IDC_CMB_PRIMARY))->AddString(_T("无"));
  ((CComboBox*)GetDlgItem(IDC_CMB_PRIMARY))->AddString(_T("光轴"));
  ((CComboBox*)GetDlgItem(IDC_CMB_PRIMARY))->AddString(_T("电轴"));
  ((CComboBox*)GetDlgItem(IDC_CMB_PRIMARY))->AddString(_T("等效角"));
  //combobox必须放在tab控件之后
  ((CComboBox*)GetDlgItem(IDC_CMB_SECONDARY))->AddString(_T("无"));
  ((CComboBox*)GetDlgItem(IDC_CMB_SECONDARY))->AddString(_T("光轴"));
  ((CComboBox*)GetDlgItem(IDC_CMB_SECONDARY))->AddString(_T("电轴"));
  ((CComboBox*)GetDlgItem(IDC_CMB_SECONDARY))->AddString(_T("等效角"));

  m_btn_ctrl.LoadBitmaps(IDB_START);
  m_btn_ctrl.SizeToContent();

  //////////////tab
  //tab控件初始化
  CRect rect;
  m_tab_preview.InsertItem(0, _T("尖峰图"));
  m_tab_preview.InsertItem(1, _T("摇摆曲线"));
  m_tab_preview.SetCurSel(0);

  m_tab_preview.GetClientRect(&rect);
  m_preview_rect = rect;

  rect.left += 1;
  rect.right -= 1;
  rect.top += 22;
  rect.bottom -= 1;

  m_preview_rect.right -= 2;
  m_preview_rect.bottom -= 23;

  CDC* pDC = m_tab_preview.GetDC();
  m_laserdc = new CDC;
  m_laserbm = new CBitmap;
  m_laserdc->CreateCompatibleDC(pDC);
  m_laserbm->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
  m_laseroldbm = m_laserdc->SelectObject(m_laserbm);
  m_laserdc->SetStretchBltMode(HALFTONE);
  SetBrushOrgEx(m_laserdc->m_hDC, 0, 0, NULL);


  m_xraydc = new CDC;
  m_xraybm = new CBitmap;
  m_xraydc->CreateCompatibleDC(pDC);
  m_xraybm->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
  m_xrayoldbm = m_xraydc->SelectObject(m_xraybm);
  m_xraydc->SetStretchBltMode(HALFTONE);
  SetBrushOrgEx(m_xraydc->m_hDC, 0, 0, NULL);

  ReleaseDC(pDC);

  // 背景贴图
  CBitmap bmp, sbmp;
  HBITMAP bitmap = (HBITMAP)::LoadImage(NULL, _T("BK1.bmp"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);
  if (bitmap)
    bmp.Attach(bitmap);
  else
    bmp.LoadBitmap(IDB_BK1);   //IDB_BITMAP1是图片资源ID
  //  CRect rect;
  GetClientRect(&rect);
  ScaleBitmap(&bmp, sbmp, rect.Width(), rect.Height());
  m_brush.CreatePatternBrush(&sbmp);

  bmp.DeleteObject();
  //档位大字体
  m_font.CreatePointFont(800, _T("宋体"), NULL);
  GetDlgItem(IDC_STATIC_SORT)->SetFont(&m_font);

  //初始化文本框
  ((CComboBox*)GetDlgItem(IDC_CMB_PRIMARY))->SetCurSel(m_param->user_config.measure.primary.type);
  ((CComboBox*)GetDlgItem(IDC_CMB_SECONDARY))->SetCurSel(m_param->user_config.measure.secondary.type);
  SetDlgItemText(IDC_EDT_CARD, (m_param->user_config.measure.card));
  SetDlgItemInt(IDC_EDT_CENTER_DEGREE, (UINT)(m_param->user_config.measure.primary.center_degree));
  SetDlgItemInt(IDC_EDT_STEP_DEGREE, (UINT)(m_param->user_config.measure.primary.step_degree));
  SetDlgItemInt(IDC_EDT_MIN_DEGREE, (UINT)(m_param->user_config.measure.secondary.min_degree));
  SetDlgItemInt(IDC_EDT_MAX_DEGREE, (UINT)(m_param->user_config.measure.secondary.max_degree));
  SetDlgItemInt(IDC_EDT_EQU_PHI, (UINT)(m_param->user_config.measure.equivalent_angle.phi));
  SetDlgItemInt(IDC_EDT_EQU_FACTOR, (UINT)(m_param->user_config.measure.equivalent_angle.factor));

  InitGrid();
  UpdateGridWithRecalc();
  //test

  m_file.Open(_T("test.txt"), CFile::modeCreate | CFile::modeWrite | CFile::shareDenyRead);
  m_file.Seek(0, CFile::end);
}


//void CDlgRun1::OnBnClickedSortSet()
//{
//  // TODO: 在此添加控件通知处理程序代码
//}


void CDlgRun1::OnBnClickedEdtRunSet()
{
  // TODO: 在此添加控件通知处理程序代码

  GetMainFrame()->EfgParamSave();

  UpdateGridWithRecalc();
}


void CDlgRun1::OnCbnSelchangeCmbPrimary()
{
  // TODO: 在此添加控件通知处理程序代码
  m_param->user_config.measure.primary.type = ((CComboBox*)GetDlgItem(IDC_CMB_PRIMARY))->GetCurSel();
}


void CDlgRun1::OnEnChangeEdtCard()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CFormView::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  GetDlgItemText(IDC_EDT_CARD, m_param->user_config.measure.card, MAX_CARD_LEN);
}


void CDlgRun1::OnCbnSelchangeCmbSecondary()
{
  // TODO: 在此添加控件通知处理程序代码
  m_param->user_config.measure.secondary.type = ((CComboBox*)GetDlgItem(IDC_CMB_SECONDARY))->GetCurSel();
}


void CDlgRun1::OnEnChangeEdtCenterDegree()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CFormView::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->user_config.measure.primary.center_degree = GetDlgItemInt(IDC_EDT_CENTER_DEGREE, 0);
}


void CDlgRun1::OnEnChangeEdtStepDegree()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CFormView::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->user_config.measure.primary.step_degree = GetDlgItemInt(IDC_EDT_STEP_DEGREE, 0);

}


void CDlgRun1::OnEnChangeEdtMinDegree()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CFormView::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->user_config.measure.secondary.min_degree = GetDlgItemInt(IDC_EDT_MIN_DEGREE, 0);

}


void CDlgRun1::OnEnChangeEdtMaxDegree()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CFormView::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->user_config.measure.secondary.max_degree = GetDlgItemInt(IDC_EDT_MAX_DEGREE, 0);

}


void CDlgRun1::OnEnChangeEdtEquPhi()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CFormView::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->user_config.measure.equivalent_angle.phi = GetDlgItemInt(IDC_EDT_EQU_PHI, 0);

}


void CDlgRun1::OnEnChangeEdtEquFactor()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CFormView::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->user_config.measure.equivalent_angle.factor = GetDlgItemInt(IDC_EDT_EQU_FACTOR, 0);

}

void CDlgRun1::InitGrid(void)
{
  m_gridSort.SetEditable(false);
  m_gridSort.SetTextBkColor(RGB(204, 232, 207));//黄色背景
  m_gridSort.SetRowCount(2); //初始为2行

  for (int i = 1; i < 2; i++)
  {
    m_gridSort.SetRowHeight(i, 25);
  }
  m_gridSort.SetColumnCount(3); //初始化为3列
  m_gridSort.SetFixedRowCount(1); //表头为1行
  m_gridSort.SetFixedColumnCount(0); //表头为1列
  m_gridSort.SetRowHeight(0, 30); //设置各行高         
  //m_gridSort.SetColumnWidth(0, 320); //设置0列宽 
  GV_ITEM Item;
  Item.mask = GVIF_TEXT | GVIF_FORMAT;
  Item.nFormat = DT_CENTER | DT_WORDBREAK;
  Item.strText.Format(_T("档位"));
  Item.row = 0;
  Item.col = 0;
  m_gridSort.SetItem(&Item);
  Item.strText.Format(_T("角度"));
  Item.row = 0;
  Item.col = 1;
  m_gridSort.SetItem(&Item);
  Item.strText.Format(_T("片数"));
  Item.row = 0;
  Item.col = 2;
  m_gridSort.SetItem(&Item);

  CRect cRect;
  GetDlgItem(IDC_GRID_SORT)->GetClientRect(&cRect);
  m_gridSort.SetColumnWidth(0, cRect.Width() / 6 * 1); //设置列宽 
  m_gridSort.SetColumnWidth(1, cRect.Width() / 6 * 3); //
  m_gridSort.SetColumnWidth(2, cRect.Width() / 6 * 2); //
  m_gridSort.ExpandLastColumn();
  m_gridSort.SetColumnResize(FALSE);
  m_gridSort.SetRowResize(FALSE);
  //m_gridSort.SetSingleRowSelection(TRUE);
  m_gridSort.EnableSelection(FALSE);
  //SetAutoSizeStyle
  //m_gridSort.EnableScrollBar();
  // m_gridSort.EnableScrollBarCtrl();
  m_gridSort.ShowBar(SB_HORZ, FALSE);
  //m_gridSort.SetRedraw(TRUE);
}

void CDlgRun1::UpdateGrid()
{
  //m_gridSort.SetRedraw(FALSE);
  int min = 1;
  int max = m_gridSort.GetRowCount();
  //EFG_Param *m_param = &GetMainFrame()->m_efgio.m_efg_param;
  int sort_type = m_param->user_config.type;
  ASSERT(sort_type < MAX_TYPE_NUM);
  int sort_num = m_param->user_config.type_sort_num[sort_type];


  for (int i = max; i >= min; i--)
  {
    m_gridSort.DeleteRow(i);
  }
  int line = 1;
  //添加分档档位
  CString str;
  for (int i = 0; i < sort_num; i++, line++)
  {
    str.Format(_T("%d"), line);
    m_gridSort.InsertRow(str, -1);
    str.Format(_T("%02d°%02d′%02d″"), S_DEG(m_result->degree.sort_sec[i])
      , S_MIN(m_result->degree.sort_sec[i])
      , S_SEC(m_result->degree.sort_sec[i]));
    m_gridSort.SetItemText(line, 1, str);
    str.Format(_T("%d"), m_result->num.y_off[i]);
    m_gridSort.SetItemText(line, 2, str);
  }
  //添加不合格档位
  str.Format(_T("%d"), line);
  m_gridSort.InsertRow(str, -1);
  m_gridSort.SetItemText(line, 1
    , m_io->GetMeasureType(m_param->user_config.measure.primary.type) + _T("-"));
  str.Format(_T("%d"), m_result->num.primary_n);
  m_gridSort.SetItemText(line++, 2, str);


  str.Format(_T("%d"), line);
  m_gridSort.InsertRow(str, -1);
  m_gridSort.SetItemText(line, 1
    , m_io->GetMeasureType(m_param->user_config.measure.primary.type) + _T("+"));
  str.Format(_T("%d"), m_result->num.primary_p);
  m_gridSort.SetItemText(line++, 2, str);

  str.Format(_T("%d"), line);
  m_gridSort.InsertRow(str, -1);
  m_gridSort.SetItemText(line, 1
    , m_io->GetMeasureType(m_param->user_config.measure.secondary.type) + _T("-"));
  str.Format(_T("%d"), m_result->num.secondary_n);
  m_gridSort.SetItemText(line++, 2, str);

  str.Format(_T("%d"), line);
  m_gridSort.InsertRow(str, -1);
  m_gridSort.SetItemText(line, 1
    , m_io->GetMeasureType(m_param->user_config.measure.secondary.type) + _T("+"));
  str.Format(_T("%d"), m_result->num.secondary_p);
  m_gridSort.SetItemText(line++, 2, str);

  str.Format(_T("%d"), line);
  m_gridSort.InsertRow(str, -1);
  m_gridSort.SetItemText(line, 1, _T("未检出"));
  str.Format(_T("%d"), m_result->num.not_detected);
  m_gridSort.SetItemText(line++, 2, str);

  m_gridSort.ExpandLastColumn();
  m_gridSort.Refresh();
  //m_gridSort.SetRedraw(TRUE);
  //m_gridSort.Invalidate();
}
//重新计算各档中心值
void CDlgRun1::UpdateGridWithRecalc()
{
  EfgAlg alg;
  alg.GetAllSortDegree(m_result->degree.sort_sec
    , USER_TO_SEC(m_param->user_config.measure.primary.center_degree)
    , USER_TO_SEC(m_param->user_config.measure.primary.step_degree)
    , m_param->user_config.type_sort_num[m_param->user_config.type]);

  UpdateGrid();
}


void CDlgRun1::OnBnClickedBtnCtrl()
{
  // TODO: 在此添加控件通知处理程序代码
  CString str;
  GetDlgItemText(IDC_BTN_CTRL, str);
  ((CButton*)GetDlgItem(IDC_BTN_CTRL))->EnableWindow(FALSE);
  if ("开始" == str) {
    //SetTimer(0, 100, NULL);
    m_run.state.mainrun = START;

    gTrdMainRun = AfxBeginThread(Thread_MainRun, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
    gTrdMainRun->m_bAutoDelete = TRUE;
    gTrdMainRun->ResumeThread();


    m_btn_ctrl.LoadBitmaps(IDB_STOP);
    SetDlgItemText(IDC_BTN_CTRL, _T("停止"));
  }
  else {
    //KillTimer(0);
    m_run.state.mainrun = STOP;
    gTrdMainRun->ResumeThread();

    m_btn_ctrl.LoadBitmaps(IDB_START);
    SetDlgItemText(IDC_BTN_CTRL, _T("开始"));
  }

}


void CDlgRun1::OnBnClickedChkPause()
{
  // TODO: 在此添加控件通知处理程序代码
  CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_PAUSE);
  int nStat = pChk->GetCheck();
  if (nStat)
  {
  }
  else
  {

  }
}

//sw 1刷新，sw清零
void CDlgRun1::ShowResult(BOOL sw)
{
  CString str_deg = _T("");
  int str_pos = 0;
  if (sw)
  {
    //str_deg.Format(_T("类  别\t当 前 值\t平 均 值\t散    差\t测量总数：%d\r\n光轴\t%07.3lf\t%07.3lf\t%07.3lf\r\n电轴\t%07.3lf\t%3.3lf\t%3.3lf\r\n等效角\t%3.3f\t%3.3f\t%3.3f\r\n光轴0\t%3.3f\t%3.3f\t%3.3f\r\n电轴0\t%3.3f\t%3.3f\t%3.3f\r\n"),

    str_deg.Format(_T("类  别\t当前值\t平均值\t散  差\t测量总数：%d\r\n光轴\t%06d\t%06d\t%06.3f\r\n电轴\t%06d\t%06d\t%06.3f\r\n等效角\t%06d\t%06d\t%06.3f\r\n光轴0\t%06d\t%06d\t%06.3f\r\n电轴0\t%06d\t%06d\t%06.3f\r\n"),
      m_io->m_resultParam.measure.num,
      DEG_TO_USER(m_io->m_resultParam.measure.cur_theta1), DEG_TO_USER(m_io->m_resultParam.measure.avg_theta1), (m_io->m_resultParam.measure.std_theta1) * 3600,
      DEG_TO_USER(m_io->m_resultParam.measure.cur_phi1), DEG_TO_USER(m_io->m_resultParam.measure.avg_phi1), (m_io->m_resultParam.measure.std_phi1) * 3600,
      DEG_TO_USER(m_io->m_resultParam.measure.cur_equ), DEG_TO_USER(m_io->m_resultParam.measure.avg_equ), (m_io->m_resultParam.measure.std_equ) * 3600,
      DEG_TO_USER(m_io->m_resultParam.measure.cur_theta0), DEG_TO_USER(m_io->m_resultParam.measure.avg_theta0), (m_io->m_resultParam.measure.std_theta0) * 3600,
      DEG_TO_USER(m_io->m_resultParam.measure.cur_phi0), DEG_TO_USER(m_io->m_resultParam.measure.avg_phi0), (m_io->m_resultParam.measure.std_phi0) * 3600
    );

    str_pos = m_io->m_resultParam.measure.cur_pos;
  }
  else
  {
    m_io->ClearMeasureResult();
  }
  GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(str_deg);
  /////////////////////////////////////
  str_deg.Format(_T("类  别\t当前值\t平均值\t散  差\r\nD1\t%06d\t%06d\t%06d\r\nD2\t%06d\t%06d\t%06d\r\nDM\t%06d\t%06d\t%06d\r\nR1\t%06d\t%06d\t%06d\r\n脉冲数\t%.0f\t%.1f\t%.1f\r\nug\t%06d\t%06d\t%06d\r\nA\t%.3f\t%.3f\t%.3f\r\nw\t%.3f\t%.3f\t%.3f\r\nt\t%.3f\t%.3f\t%.3f\r\nk\t%.3f\t%.3f\t%.3f\r\nphase\t%.3f\t%.3f\t%.3f\r\n"),
    DEG_TO_USER(m_io->m_resultParam.measure.D1), DEG_TO_USER(m_io->m_resultParam.measure.avg_D1), DEG_TO_USER(m_io->m_resultParam.measure.std_D1),
    DEG_TO_USER(m_io->m_resultParam.measure.D2), DEG_TO_USER(m_io->m_resultParam.measure.avg_D2), DEG_TO_USER(m_io->m_resultParam.measure.std_D2),
    DEG_TO_USER(m_io->m_resultParam.measure.DM), DEG_TO_USER(m_io->m_resultParam.measure.avg_DM), DEG_TO_USER(m_io->m_resultParam.measure.std_DM),
    DEG_TO_USER(m_io->m_resultParam.measure.R1), DEG_TO_USER(m_io->m_resultParam.measure.avg_R1), DEG_TO_USER(m_io->m_resultParam.measure.std_R1),
    (m_io->m_resultParam.measure.pluse_num), (m_io->m_resultParam.measure.avg_pluse_num), (m_io->m_resultParam.measure.std_pluse_num),
    DEG_TO_USER(m_io->m_resultParam.measure.u_g), DEG_TO_USER(m_io->m_resultParam.measure.avg_u_g), DEG_TO_USER(m_io->m_resultParam.measure.std_u_g),
    (m_io->m_resultParam.measure.A), (m_io->m_resultParam.measure.avg_A), (m_io->m_resultParam.measure.std_A),
    (m_io->m_resultParam.measure.w), (m_io->m_resultParam.measure.avg_w), (m_io->m_resultParam.measure.std_w),
    (m_io->m_resultParam.measure.t), (m_io->m_resultParam.measure.avg_t), (m_io->m_resultParam.measure.std_t),
    (m_io->m_resultParam.measure.k), (m_io->m_resultParam.measure.avg_k), (m_io->m_resultParam.measure.std_k),
    (m_io->m_resultParam.measure.phase), (m_io->m_resultParam.measure.avg_phase), (m_io->m_resultParam.measure.std_phase)
  );
  SetDlgItemText(IDC_SHOW_DEGREE, str_deg);


  str_deg.Format(_T("%06.3f\t%06.3f\t%06.3f\t%06.3f\r\n"),
    (m_io->m_resultParam.measure.D1),
    (m_io->m_resultParam.measure.D2),
    (m_io->m_resultParam.measure.DM),
    (m_io->m_resultParam.measure.R1));
  m_file.Write(CStringA(str_deg).GetBuffer(), CStringA(str_deg).GetLength());
  CStringA(str_deg).ReleaseBuffer();

  //SetDlgItemText(IDC_STATIC_RESULT, str_deg);
  ///////////////

  OnTcnSelchangeTabPreview(NULL, NULL);
  //UpdateGrid();

  SetDlgItemInt(IDC_STATIC_SORT, str_pos + 1);
}


void CDlgRun1::OnTcnSelchangeTabPreview(NMHDR* pNMHDR, LRESULT* pResult)
{
  // TODO: 在此添加控件通知处理程序代码
  CRect tabRect;
  m_tab_preview.GetClientRect(&tabRect);
  tabRect.left += 1;
  tabRect.right -= 1;
  tabRect.top += 22;
  tabRect.bottom -= 1;
  CDC* pDC = m_tab_preview.GetDC();
  switch (m_tab_preview.GetCurSel())
  {
  case 0:
    pDC->BitBlt(tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), m_xraydc, 0, 0, SRCCOPY);
    break;
  case 1:
    pDC->BitBlt(tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), m_laserdc, 0, 0, SRCCOPY);

    break;
  }
  ReleaseDC(pDC);

  if (pResult)
    * pResult = 0;
}
//得到用户最终结果并刷新界面
int CDlgRun1::FitAndUpdate(int n)
{
  ASSERT(n > 0);

  if (-1 == GetMainFrame()->m_diIntCounterSnap.LaserFit(n)) {
    return -1;
  }

  if (GetMainFrame()->m_viewBoard)
    GetMainFrame()->m_viewBoard->DrawToDC(m_laserdc, m_preview_rect);

  if (-1 == GetMainFrame()->m_diIntCounterSnap.XrayFit(n)) {
    return -1;
  }

  if (GetMainFrame()->m_viewBoard)
    GetMainFrame()->m_viewBoard->DrawToDC(m_xraydc, m_preview_rect);

  OnTcnSelchangeTabPreview(NULL, NULL);
  return 0;
}
//得到用户最终结果并刷新界面
int CDlgRun1::CalcResult()
{
  /*if (-1 == GetMainFrame()->m_diIntCounterSnap.LaserFit()) {
  return -1;
  }

  if (GetMainFrame()->m_viewBoard)
  GetMainFrame()->m_viewBoard->DrawToDC(m_laserdc, m_preview_rect);

  if (-1 == GetMainFrame()->m_diIntCounterSnap.XrayFit()) {
  return -1;
  }

  if (GetMainFrame()->m_viewBoard)
  GetMainFrame()->m_viewBoard->DrawToDC(m_xraydc, m_preview_rect);*/

  EfgAlg alg;

  struct tagSinParam sin_param = {
    m_io->m_resultParam.measure.A,
    m_io->m_resultParam.measure.w,
    m_io->m_resultParam.measure.t,
    m_io->m_resultParam.measure.k
  };
  //计算光轴电轴

  double AMPL = atan(m_io->m_resultParam.measure.A * m_io->m_configParam.laser.factor_a / m_io->m_configParam.laser.factor_l) / DPI;
  double PHASE = -m_io->m_resultParam.measure.t / m_io->m_resultParam.measure.w * /*2 * PI*/360 / (m_io->m_resultParam.measure.l_num);  // 零位到sin的起始位置的角度，注意原始efg的图是上下颠倒的

  double PHASE_END = 0;

  alg.CalcDegree1(
    AMPL,
    PHASE/*m_io->m_resultParam.measure.t / DPI*/,
    m_io->m_resultParam.measure.R1,
    m_io->m_configParam.laser.offset,
    m_io->m_resultParam.measure.cur_theta0,
    m_io->m_resultParam.measure.cur_phi0,
    m_io->m_resultParam.measure.cur_theta1,
    m_io->m_resultParam.measure.cur_phi1,
    m_io->m_resultParam.measure.DM,
    &m_io->m_resultParam.measure.phase
  );

  m_io->m_resultParam.measure.cur_theta1 += m_io->m_configParam.laser.theta_offset / 3600;
  m_io->m_resultParam.measure.cur_phi1 += m_io->m_configParam.laser.phi_offset / 3600;

  //计算等效角
  alg.CalcEquAngle(
    m_io->m_resultParam.measure.cur_theta1,
    m_io->m_resultParam.measure.cur_phi1,
    USER_TO_DEG(m_io->m_configParam.user_config.measure.equivalent_angle.phi),
    m_io->m_configParam.user_config.measure.equivalent_angle.factor / 1000.0,
    m_io->m_resultParam.measure.cur_equ
  );

  // 累计测出值
  m_io->m_resultParam.measure.num++;

  // 累计平均值，散差
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.cur_theta0,
    m_io->m_resultParam.measure.avg_theta0,
    m_io->m_resultParam.measure.std_theta0,
    m_io->m_resultParam.measure.std2_theta0
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.cur_phi0,
    m_io->m_resultParam.measure.avg_phi0,
    m_io->m_resultParam.measure.std_phi0,
    m_io->m_resultParam.measure.std2_phi0
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.cur_theta1,
    m_io->m_resultParam.measure.avg_theta1,
    m_io->m_resultParam.measure.std_theta1,
    m_io->m_resultParam.measure.std2_theta1
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.cur_phi1,
    m_io->m_resultParam.measure.avg_phi1,
    m_io->m_resultParam.measure.std_phi1,
    m_io->m_resultParam.measure.std2_phi1
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.cur_equ,
    m_io->m_resultParam.measure.avg_equ,
    m_io->m_resultParam.measure.std_equ,
    m_io->m_resultParam.measure.std2_equ
  );
  //test
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.D1,
    m_io->m_resultParam.measure.avg_D1,
    m_io->m_resultParam.measure.std_D1,
    m_io->m_resultParam.measure.std2_D1
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.D2,
    m_io->m_resultParam.measure.avg_D2,
    m_io->m_resultParam.measure.std_D2,
    m_io->m_resultParam.measure.std2_D2
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.DM,
    m_io->m_resultParam.measure.avg_DM,
    m_io->m_resultParam.measure.std_DM,
    m_io->m_resultParam.measure.std2_DM
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.R1,
    m_io->m_resultParam.measure.avg_R1,
    m_io->m_resultParam.measure.std_R1,
    m_io->m_resultParam.measure.std2_R1
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.pluse_num,
    m_io->m_resultParam.measure.avg_pluse_num,
    m_io->m_resultParam.measure.std_pluse_num,
    m_io->m_resultParam.measure.std2_pluse_num
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.u_g,
    m_io->m_resultParam.measure.avg_u_g,
    m_io->m_resultParam.measure.std_u_g,
    m_io->m_resultParam.measure.std2_u_g
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.A,
    m_io->m_resultParam.measure.avg_A,
    m_io->m_resultParam.measure.std_A,
    m_io->m_resultParam.measure.std2_A
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.w,
    m_io->m_resultParam.measure.avg_w,
    m_io->m_resultParam.measure.std_w,
    m_io->m_resultParam.measure.std2_w
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.t,
    m_io->m_resultParam.measure.avg_t,
    m_io->m_resultParam.measure.std_t,
    m_io->m_resultParam.measure.std2_t
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.k,
    m_io->m_resultParam.measure.avg_k,
    m_io->m_resultParam.measure.std_k,
    m_io->m_resultParam.measure.std2_k
  );

  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.phase,
    m_io->m_resultParam.measure.avg_phase,
    m_io->m_resultParam.measure.std_phase,
    m_io->m_resultParam.measure.std2_phase
  );
  //计算档位，在别的地方，或许在efgio

  int pos;
  int pos_num;
  pos = m_io->GetCurOffPos(pos_num);//得到当前的下料位置
  ShowResult(1);//刷新界面

  CString str;
  str.Format(_T("%d"), pos_num);
  m_gridSort.SetItemText(pos + 1, 2, str);

  m_gridSort.Refresh();
  return 0;
}

void CDlgRun1::OnBnClickedChkStaticmeasure()
{
  // TODO: 在此添加控件通知处理程序代码
  CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_STATICMEASURE);
  int nStat = pChk->GetCheck();
  pChk->EnableWindow(FALSE);
  if (nStat)
  {
    m_run.state.mainrun = STATIC_MEASURE_START;
    gTrdMainRun = AfxBeginThread(Thread_MainRun, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
    gTrdMainRun->m_bAutoDelete = TRUE;
    gTrdMainRun->ResumeThread();

  }
  else
  {
    m_run.state.mainrun = STATIC_MEASURE_STOP;
    gTrdMainRun->ResumeThread();
  }
}


void CDlgRun1::OnBnClickedChkPnmeasure()
{
  // TODO: 在此添加控件通知处理程序代码
  CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_PNMEASURE);
  int nStat = pChk->GetCheck();
  pChk->EnableWindow(FALSE);
  if (nStat)
  {
    m_run.state.mainrun = PN_MEASURE_START;
    gTrdMainRun = AfxBeginThread(Thread_MainRun, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
    gTrdMainRun->m_bAutoDelete = TRUE;
    gTrdMainRun->ResumeThread();

  }
  else
  {
    m_run.state.mainrun = PN_MEASURE_STOP;
    gTrdMainRun->ResumeThread();
  }

  SetFocus();//为了响应空格
}


BOOL CDlgRun1::PreTranslateMessage(MSG* pMsg)
{
  // TODO: 在此添加专用代码和/或调用基类
  if (pMsg->message == WM_KEYDOWN)
  {
    if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
    {
      //判断是否按下键盘Enter键
      switch (pMsg->wParam)
      {
        /*case VK_RETURN:
        OnBnClickedBtnFit();
        return TRUE;*/
      case VK_SPACE:
        if (END == m_run.state.turntable) {
          CRect tabRect;
          m_tab_preview.GetClientRect(&tabRect);
          tabRect.left += 1;
          tabRect.right -= 1;
          tabRect.top += 22;
          tabRect.bottom -= 1;
          CDC* pDC = m_tab_preview.GetDC();

          pDC->FillSolidRect(&tabRect, RGB(199, 237, 204));

          ReleaseDC(pDC);

          m_run.state.user = START;
        }
        break;
	  case VK_CONTROL:
        m_run.transfer[TRANSFER_TAKE]=0;
        break;
      default:
        break;
      }
    }
  }
  return CFormView::PreTranslateMessage(pMsg);
}


void CDlgRun1::OnBnClickedChkCamera()
{
  // TODO: 在此添加控件通知处理程序代码
  CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_CAMERA);
  /*CCamera*/ pdlg_cam = ((CDlgDebug1*)(GetMainFrame()->m_userFrame->m_splitUser.GetPane(0, 1)))->m_dlg_camera;
  int nStat = pChk->GetCheck();
  pChk->EnableWindow(FALSE);

  if (nStat)
  {
	  pdlg_cam->VideoOnOff(TRUE);
	  Sleep(1000);
    m_run.state.camera = START;
    m_run.state.transfer = START;

    gTrdCamera = AfxBeginThread(Thread_Camera, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
    ASSERT(gTrdCamera);
    gTrdCamera->m_bAutoDelete = FALSE;
    gTrdCamera->ResumeThread();
    gTrdTransfer = AfxBeginThread(Thread_Transfer, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
    ASSERT(gTrdTransfer);
    gTrdTransfer->m_bAutoDelete = FALSE;
    gTrdTransfer->ResumeThread();

  }
  else
  {
    m_run.state.camera = STOP;
    m_run.state.transfer = STOP;

    while (1)
    {
      DWORD dwCodeCamera, dwCodeTransfer;
      GetExitCodeThread(gTrdCamera->m_hThread, &dwCodeCamera);
      GetExitCodeThread(gTrdTransfer->m_hThread, &dwCodeTransfer);

      if (dwCodeCamera != STILL_ACTIVE && dwCodeTransfer != STILL_ACTIVE)
        break;

      if (dwCodeCamera == STILL_ACTIVE && STOP != m_run.state.camera)
        m_run.state.camera = STOP;

      if (dwCodeTransfer == STILL_ACTIVE && STOP != m_run.state.transfer)
        m_run.state.transfer = STOP;

    };

    gTrdCamera->Delete();
    gTrdTransfer->Delete();
    gTrdCamera = NULL;
    gTrdTransfer = NULL;
	pdlg_cam->VideoOnOff(FALSE);
  }

  pChk->EnableWindow(TRUE);
}
