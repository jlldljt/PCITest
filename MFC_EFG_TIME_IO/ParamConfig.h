#pragma once
#include<iostream>
#include "tinyxml2.h"
//using namespace tinyxml2;
typedef struct tagConfigParam
{
  int device;
  int comboData;//combo选项值
  double param0;//参数0
  double param1;//参数1
  bool start;
}ConfigParam, *PConfigParam;

class CParamConfig
{
public:
  CParamConfig(void);
  ~CParamConfig(void);
private:
  std::string m_path;
  std::string m_element;

public:
  int Create(const char* xmlPath, const char* element);
  int SaveParam(const ConfigParam user);
  ConfigParam* ReadParam(void);
};

