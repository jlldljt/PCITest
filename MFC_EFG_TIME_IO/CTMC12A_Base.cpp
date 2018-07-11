#include "StdAfx.h"
#include "CTMC12A_Base.h"

WORD CTMC12A_Base::m_tmc12Boards = 0;

CTMC12A_Base::CTMC12A_Base()
{
}


CTMC12A_Base::~CTMC12A_Base()
{
}

WORD CTMC12A_Base::DriverInit(WORD * wTotalBoards)
{
  WORD wRtn = Ixud_DriverInit(wTotalBoards);
  m_tmc12Boards = *wTotalBoards;
  return wRtn;
}

WORD CTMC12A_Base::DriverClose(void)
{
  m_tmc12Boards = 0;
  return Ixud_DriverClose();
}
