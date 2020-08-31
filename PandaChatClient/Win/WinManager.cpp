#include "stdafx.h"
#include "WinManager.h"
#include "LoginWin.h"
#include "MainWin.h"
#include "..\Net\UserSessionData.h"
#include"..\Net\IUProtocolData.h"
HWND WinManager::GetWinHwnd()
{
	return win_manager_hwnd;
}

WinManager::WinManager()
{
	login_win = NULL;
	main_win = NULL;

	login_win = new LoginWin;
	main_win = new MainWin;


	//获取窗口句柄
	win_manager_hwnd=(HWND)this->winId();
}

void WinManager::InitWin()
{
	login_win->Init();
	main_win->Init();
}

void WinManager::ShowLogin()
{
	//显示登录窗口
	login_win->Show();
	//主窗口关闭
	main_win->close();
}

void WinManager::ShowMainWin()
{
	//显示主窗口
	login_win->close();
	//登陆窗口关闭
	main_win->Show();
}

WinManager::~WinManager()
{


}


bool WinManager::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	Q_UNUSED(eventType);
	MSG* msg = reinterpret_cast<MSG*>(message);
	return winEvent(msg, result);
}

bool WinManager::winEvent(MSG *message, long *result)
{
	//回归主线程
	//所有message指针在各自模块释放,登陆返回消息在登陆释放,主窗口消息在主窗口释放
	switch (message->message)
	{
	case FMG_MSG_LOGIN_RESULT:
		HandleRegist(message);
		break;

	default:
		break;
	}

	return QWidget::nativeEvent("", message, result);
}

void WinManager::HandleRegist(MSG *message)
{
	CRegisterResult* pResult = (CRegisterResult*)message->lParam;
	if (pResult == NULL)
	{
		return;
	}
	login_win->HandleRegist(pResult);
}