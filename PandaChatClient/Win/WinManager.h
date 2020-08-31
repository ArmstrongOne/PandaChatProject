//���ڹ����� 
//ÿ�����̳��� ��Ӧһ����½���� һ�������� 
//�������¶�Ӧ���ý���Ͷ���������
//���Ҵ�����н����Ӧ�ľ�� ��Ϣ�ķַ�Ҳд����һ�㡣
#pragma once
#include <QObject>

class LoginWin;//��¼����
class MainWin; //������

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

	//��ʼ�����ڹ�����
	void InitWin();

	//��ʾ��½����
	void ShowLogin();

	//��ʾ������
	void ShowMainWin();

protected:
	bool        nativeEvent(const QByteArray &eventType, void *message, long *result) override;
	bool        winEvent(MSG *message, long *result);


private:
	LoginWin*               login_win;
	MainWin*                main_win;

	HWND                    win_manager_hwnd;


//=============���������Ϣ��������ͳһ·��
	void        HandleRegist(MSG *message);
};
