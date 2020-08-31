#include "stdafx.h"
#include".\Net\ClientSocketManager.h"//�ͻ���ͨ�Ź���ģ��
#include "RunSetting.h"//������������
#include ".\Utils\ClientLog.h"//��־
#include"./Win/WinManager.h"//��½����
#include ".\Utils\Singleton.h" //����ģ��
#include ".\Ini\ConfigFileReader.h"//�����ļ���ȡ��
#include <QtWidgets/QApplication>

using namespace Net;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qApp->setStyleSheet("*{outline:0px;}"); //ȥ���ؼ�ѡ�б߿�

	//��־���ȼ���� ����RunSetting��ʼ�� �ŵ���ǰ��
	ClientLog::GetLogObj()->SetSaveFolderPath("C:\\Users\\DELL\\Desktop\\LOGO");
	ClientLog::GetLogObj()->InitLog();
	
	//��������ʼ�������Ƿ�����
	bool can_run=RunSetting::GetRunSetting()->RunSoftWare();
	if (!can_run)
	{
		//����ʧ�ܣ�����Crash��
		LOG_FATAL("run faild,program crash.");
	}
	LOG_INFO("run successuflly");

	//�������ͨ��ģ���Ƿ�����
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
