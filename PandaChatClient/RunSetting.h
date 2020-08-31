//启动管理对象
#pragma once
#include <windows.h>
class CConfigFileReader;
class RunSetting
{
public:
	static RunSetting*GetRunSetting();

	//判断启动软件各种条件
	bool RunSoftWare();

	bool InitWinSock(); //初始化Windows网络库
	bool InitClentIni();//读取配置文件模块

	CConfigFileReader*GetCinfigFileInfo();
	~RunSetting();
private:
	 RunSetting();
	 RunSetting(RunSetting&) = delete;
	 RunSetting&operator=(RunSetting&) = delete;
	 static RunSetting*run_set_obj;

	//******************网络初始化
	 void UninitSocket();
	 BOOL InitSocket();

	 //******************配置文件读取
	 CConfigFileReader *base_ini_reader;//配置信息基础读取
};