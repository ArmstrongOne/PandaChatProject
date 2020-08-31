#include "stdafx.h"
#include".\Net\ClientSocketManager.h"//客户端通信管理模块
#include "RunSetting.h"//程序启动设置
#include ".\Utils\ClientLog.h"//日志
#include"./Win/WinManager.h"//登陆窗口
#include ".\Utils\Singleton.h" //单例模板
#include ".\Ini\ConfigFileReader.h"//配置文件读取类
#include <QtWidgets/QApplication>

using namespace Net;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qApp->setStyleSheet("*{outline:0px;}"); //去除控件选中边框

	//日志优先级最高 不在RunSetting初始化 放到最前面
	ClientLog::GetLogObj()->SetSaveFolderPath("C:\\Users\\DELL\\Desktop\\LOGO");
	ClientLog::GetLogObj()->InitLog();
	
	//检测各个初始启动项是否正常
	bool can_run=RunSetting::GetRunSetting()->RunSoftWare();
	if (!can_run)
	{
		//启动失败，程序Crash掉
		LOG_FATAL("run faild,program crash.");
	}
	LOG_INFO("run successuflly");

	//检测网络通信模块是否正常
	bool client_net_ini = Singleton<ClientSocketManager>::Instance().Init();
	if (!client_net_ini)
	{
		LOG_FATAL("ClientSocketManager Init faild,program crash.");
	}
	std::string ip = RunSetting::GetRunSetting()->GetCinfigFileInfo()->getConfigName("server_ip");
	short port = atoi(RunSetting::GetRunSetting()->GetCinfigFileInfo()->getConfigName("server_port"));
	Singleton<ClientSocketManager>::Instance().SetIpAndPort(ip, port);
	bool connect_ret=Singleton<ClientSocketManager>::Instance().Connect(3);


	WinManager win_manager;
	win_manager.InitWin();
	win_manager.ShowLogin();

	return a.exec();
}
