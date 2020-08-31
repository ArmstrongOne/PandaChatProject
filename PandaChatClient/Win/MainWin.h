//登陆成功后的主窗口 每个进程只有一个 
#pragma once
#include <QWidget>
namespace Ui { class MainWin; };

class MainWin : public QWidget
{
	Q_OBJECT

public:
	void Init();
	void Show();

public:
	MainWin(QWidget *parent = Q_NULLPTR);
	~MainWin();
	MainWin(MainWin&) = delete;
	MainWin&operator=(MainWin&)=delete;

private:
	Ui::MainWin *ui;
	void InitUi();
	void InitFont();

};
