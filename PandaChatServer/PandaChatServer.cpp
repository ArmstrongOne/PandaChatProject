// PandaChatServer.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include <winsock2.h>
#include<windows.h>
#include".\Utils\AsyncLog.h"
#include <string>
#include ".\Utils\Path.h"
#include ".\Utils\FileOperation.h"
#include ".\ServerIni\ConfigFileReader.h"
#include ".\Utils\Singleton.h"
#include ".\mysqlmgr\MysqlManager.h"
#include ".\mysqlmgr\MysqlManager.h"
#include ".\chatserversrc\UserManager.h"
#include ".\chatserversrc\ChatServer.h"
using namespace std;
using namespace Hootina;

#ifdef _WIN64
//��ʼ��Windows socket��
NetworkInitializer windowsNetworkInitializer;
#endif

EventLoop g_mainLoop;

int main()
{
	//******************��ʼ����־ģ��
	tstring exe_folder = CPath::GetAppPath();//��ȡexeĿ¼
	string log_folder = FileOperation::GetFileOperation()->wstringTostring(exe_folder) + "Log";
	if (!FileOperation::GetFileOperation()->IsFolderExist(log_folder))
	{
		//������Ŀ¼����Ŀ¼
		FileOperation::GetFileOperation()->CreateFolder(log_folder);
	}
	string log_file_header = log_folder + "\\Log";

	bool ret=CAsyncLog::init(log_file_header.c_str());//�ⲽһ��һ����ɹ�
	if (ret)
	{
		LOGI("Log Mode Run Successfully");
	}

	//*****************�����ļ���ȡģ��
	string ini_folder_path = FileOperation::GetFileOperation()->wstringTostring(exe_folder) + "IniFile";
	CConfigFileReader config_reader(ini_folder_path.c_str());
	bool ini_ret= config_reader.LoadFile();
	if (ini_ret)
	{
		LOGI("Read Ini Mode Run Successfully");
	}
	else
	{
		LOGF("read ini file failed,run failed");
	}

	//*******************���ݿ�ģ��
	const char* dbserver = config_reader.getConfigName("dbserver");
	const char* dbuser = config_reader.getConfigName("dbuser");
	const char* dbpassword = config_reader.getConfigName("dbpassword");
	const char* dbname = config_reader.getConfigName("dbname");
	if (!Singleton<CMysqlManager>::Instance().init(dbserver, dbuser, dbpassword, dbname))
	{
		LOGF("Init mysql failed, please check your database config..............");
	}
	else
	{
		LOGI("Init mysql Successfully");
	}
	if (!Singleton<UserManager>::Instance().init(dbserver, dbuser, dbpassword, dbname))
	{
		LOGF("Init UserManager failed, please check your database config..............");
	}

	//******************����ģ��
	const char* listenip = config_reader.getConfigName("listenip");
	short listenport = (short)atol(config_reader.getConfigName("listenport"));
	Singleton<ChatServer>::Instance().init(listenip, listenport, &g_mainLoop);//tcpserver start�����档

	LOGI("Enter loop");
	g_mainLoop.loop();

	CAsyncLog::uninit();
    return 0;
}

