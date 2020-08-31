//窗口管理者 
//每个进程程序 对应一个登陆界面 一个主窗口 
//主窗口下对应设置界面和多个聊天界面
//并且存放所有界面对应的句柄 消息的分发也写在这一层。
#pragma once
#include <QObject>

class LoginWin;//登录窗口
class MainWin; //主窗口

class WinManager : public QWidget
{
	Q_OBJECT
public:
	HWND GetWinHwnd();

public:
	WinManager();
	~WinManager();
	WinManager(WinManager&) = delete;
	WinManager&operator=(WinManager&) = delete;

	//初始化窗口管理类
	void InitWin();

	//显示登陆界面
	void ShowLogin();

	//显示主界面
	void ShowMainWin();

protected:
	bool        nativeEvent(const QByteArray &eventType, void *message, long *result) override;
	bool        winEvent(MSG *message, long *result);


private:
	LoginWin*               login_win;
	MainWin*                main_win;

	HWND                    win_manager_hwnd;


//=============网络接受消息发给窗口统一路径
	void        HandleRegist(MSG *message);
};
