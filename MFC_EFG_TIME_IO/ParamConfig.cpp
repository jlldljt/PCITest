#include "StdAfx.h"
#include "MFC_EFG_TIME_IO.h"
#include "ParamConfig.h"

#include"string"

#define DEVICE_ "Device"
#define COMBO_DATA "ComboData"
#define PARAM_0 "Param0"
#define PARAM_1 "Param1"
#define START_ "Start"

CParamConfig::CParamConfig(void)
{
  m_path = "";
  m_element = "";
}


CParamConfig::~CParamConfig(void)
{
}

//0 success
int CParamConfig::Create(const char* path, const char* element)
{
  ASSERT(path);
  ASSERT(element);
  m_path = path;
  m_element = element;
  //const char* declaration = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
  tinyxml2::XMLDocument doc;
  int res = doc.LoadFile(m_path.c_str());
  if (res == 0)
  {
    tinyxml2::XMLElement* root = doc.FirstChildElement(element);
    if (!root) {//无节点
      goto param_config_new;
    }
    return res;
  }
  //doc.Parse(declaration);//会覆盖xml所有内容

                         //添加申明可以使用如下两行
  tinyxml2::XMLDeclaration* declaration = doc.NewDeclaration();
  doc.InsertFirstChild(declaration);

param_config_new:
  tinyxml2::XMLElement* root = doc.NewElement(element);
  doc.InsertEndChild(root);

  tinyxml2::XMLElement* node0 = doc.NewElement(DEVICE_);
  tinyxml2::XMLElement* node1 = doc.NewElement(COMBO_DATA);
  tinyxml2::XMLElement* node2 = doc.NewElement(PARAM_0);
  tinyxml2::XMLElement* node3 = doc.NewElement(PARAM_1);
  tinyxml2::XMLElement* node4 = doc.NewElement(START_);
  node0->SetText(0);
  node1->SetText(0);
  node2->SetText(0);
  node3->SetText(0);
  node4->SetText(0);
  root->InsertEndChild(node0);
  root->InsertEndChild(node1);
  root->InsertEndChild(node2);
  root->InsertEndChild(node3);
  root->InsertEndChild(node4);
  return doc.SaveFile(path);
}

//0 success
int CParamConfig::SaveParam(const ConfigParam user)
{
  ASSERT(m_path != "");
  ASSERT(m_element != "");
  tinyxml2::XMLDocument doc;
  int res = doc.LoadFile(m_path.c_str());
  if (res != 0)
  {
    //cout << "load xml file failed" << endl;
    return res;
  }
  tinyxml2::XMLElement* root = doc.FirstChildElement(m_element.c_str());
  tinyxml2::XMLElement* node = root->FirstChildElement(DEVICE_);
  if (node != NULL)
  {
    node->SetText(user.device);
  }
  else
  {
    return -1;
  }
  node = root->FirstChildElement(COMBO_DATA);
  if (node != NULL)
  {
    node->SetText(user.comboData);
  }
  else
  {
    return -1;
  }
  node = root->FirstChildElement(PARAM_0);
  if (node != NULL)
  {
    node->SetText(user.param0);
  }
  else
  {
    return -1;
  }
  node = root->FirstChildElement(PARAM_1);
  if (node != NULL)
  {
    node->SetText(user.param1);
  }
  else
  {
    return -1;
  }
  node = root->FirstChildElement(START_);
  if (node != NULL)
  {
    node->SetText(user.start);
  }
  else
  {
    return -1;
  }
  return doc.SaveFile(m_path.c_str());
}

ConfigParam* CParamConfig::ReadParam(void)
{
  tinyxml2::XMLDocument doc;
  ConfigParam* user = new ConfigParam;
  int res = doc.LoadFile(m_path.c_str());
  if (res != 0)
  {
    delete user;
    return NULL;
  }
  tinyxml2::XMLElement* root = doc.FirstChildElement(m_element.c_str());
  tinyxml2::XMLElement* node = root->FirstChildElement(DEVICE_);
  if (node != NULL)
  {
    node->QueryIntText(&user->device);
    //node->get
  }
  else
  {
    delete user;
    return NULL;
  }
  node = root->FirstChildElement(COMBO_DATA);
  if (node != NULL)
  {
    node->QueryIntText(&user->comboData);
  }
  else
  {
    delete user;
    return NULL;
  }
  node = root->FirstChildElement(PARAM_0);
  if (node != NULL)
  {
    node->QueryDoubleText(&user->param0);
  }
  else
  {
    delete user;
    return NULL;
  }
  node = root->FirstChildElement(PARAM_1);
  if (node != NULL)
  {
    node->QueryDoubleText(&user->param1);
  }
  else
  {
    delete user;
    return NULL;
  }
  node = root->FirstChildElement(START_);
  if (node != NULL)
  {
    node->QueryBoolText(&user->start);
  }
  else
  {
    delete user;
    return NULL;
  }
  return user;
}