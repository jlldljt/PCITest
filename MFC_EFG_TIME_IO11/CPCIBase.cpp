#include "StdAfx.h"
#include "CPCIBase.h"

CArray<DevInf, DevInf&> CPCIBase::devices;

CPCIBase::CPCIBase()
{
  ///devices.RemoveAll();
}


CPCIBase::~CPCIBase()
{
}

int CPCIBase::getDevice(int no, DevInf & devInf)
{
  ASSERT(no >= 0);
  int count = devices.GetCount();
  if (no < count) {
    devInf = devices.GetAt(no);
    return 1;
  }
  else {
    return -1;
  }
}
