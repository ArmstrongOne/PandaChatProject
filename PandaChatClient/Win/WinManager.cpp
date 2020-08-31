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


	//��ȡ���ھ��
	win_manager_hwnd=(HWND)this->winId();
}

void WinManager::InitWin()
{
	login_win->Init();
	main_win->Init();
}

void WinManager::ShowLogin()
{
	//��ʾ��¼����
	login_win->Show();
	//�����ڹر�
	main_win->close();
}

void WinManager::ShowMainWin()
{
	//��ʾ������
	login_win->close();
	//��½���ڹر�
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
	//�ع����߳�
	//����messageָ���ڸ���ģ���ͷ�,��½������Ϣ�ڵ�½�ͷ�,��������Ϣ���������ͷ�
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