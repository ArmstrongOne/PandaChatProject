#include "stdafx.h"
#include "RunSetting.h"
#include ".\Utils\ClientLog.h"
#include ".\Ini\FontIni.h"
#include ".\Ini\ConfigFileReader.h"
#include ".\Ini\LoginSetting.h"
#include <QApplication>
#include "winsock2.h"
RunSetting*RunSetting::run_set_obj = nullptr;
#pragma comment( lib, "ws2_32.lib" ) 
RunSetting* RunSetting::GetRunSetting()
{
	if (run_set_obj == nullptr)
	{
		run_set_obj = new RunSetting;
	}
	return run_set_obj;
}

bool RunSetting::RunSoftWare()
{
	bool is_init_net_success = InitWinSock();
	if (!is_init_net_success)
	{
		LOG_INFO("net_ini_failed");
		return false;
	}

	bool is_init_inifile = InitClentIni();
	if (!is_init_inifile)
	{
		LOG_INFO("file_ini_failed");
		return false;
	}
	return true;
}

bool RunSetting::InitWinSock()
{
	bool is_init_net_success = this->InitSocket();
	return is_init_net_success;
}


bool RunSetting::InitClentIni()
{
	//获取配置文件文件夹路径
	string dir_path = (QApplication::applicationDirPath() + "\\ClientIni").toStdString();
	base_ini_reader = new CConfigFileReader(dir_path.c_str());

	bool ret_read_ini = base_ini_reader->LoadFile();
	if (!ret_read_ini)
	{
		return false;
	}

	//读取字体配置文件
	FontIni::GetRXFont()->InitFontIni();

	//读取登陆配置文件
	LoginSetting::GetLoginSetting()->ReadLoginSettingFile();

	return true;
}

CConfigFileReader* RunSetting::GetCinfigFileInfo()
{
	return base_ini_reader;
}

RunSetting::~RunSetting()
{
	if (base_ini_reader != NULL)
	{
		delete base_ini_reader;
		base_ini_reader = NULL;
	}
}

RunSetting::RunSetting()
{

}

void RunSetting::UninitSocket()
{
	::WSACleanup();
}

BOOL RunSetting::InitSocket()
{
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	int nErrorID = ::WSAStartup(wVersionRequested, &wsaData);
	if (nErrorID != 0)
	{
		return FALSE;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		UninitSocket();
		return FALSE;
	}
	return TRUE;
}