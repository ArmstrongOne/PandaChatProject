#include "stdafx.h"
#include "LoginWin.h"
#include "ui_LoginWin.h"
#include <QDesktopWidget>
#include".\Ini\FontIni.h"
#include"LoginAvatarWid.h"
#include"LoginStateWid.h"
#include ".\Ini\LoginSetting.h"
#include"TipsDialog.h"
#include".\Net\NetProtocol.h"
#include<QDebug>
#pragma execution_character_set("utf-8")

LoginWin::LoginWin(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::LoginWin();
	ui->setupUi(this);

	//注册类型
	qRegisterMetaType<USER_STATE>("USER_STATE");
	this->InitUi();
	this->InitFont();


}

void LoginWin::Init()
{



}

void LoginWin::Show()
{
	//移到桌面中间
	QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以 
	this->move((desktop->width()-this->width())/2,(desktop->height() - this->height())/2);
	this->show();
}

LoginWin::~LoginWin()
{
	delete ui;
}

bool LoginWin::eventFilter(QObject *obj, QEvent *e)
{
	if (obj == ui->edit_account&&e->type() == QEvent::FocusIn)
	{
		ui->wid_account->setStyleSheet
		(
			"#wid_account"
			"{"
			"border:1px solid;"
			"border-color:rgb(80,90,196);"
			"}"
		);

		ui->lab_account_logo->setStyleSheet
		(
			"border-image: url(:/PandaChatClient/all_image/Win_Login/check_account_ logo.png);"
		);

		//焦点进入的时候取消一些界面逻辑
		state_wid->hide();
	}

	if (obj == ui->edit_account&&e->type() == QEvent::FocusOut)
	{
		ui->wid_account->setStyleSheet
		(
			"#wid_account"
			"{"
			"border:1px solid;"
			"border-top:0px;"
			"border-left:0px;"
			"border-right:0px;"
			"border-color:rgb(229, 229, 229);"
			"}"
		);


		ui->lab_account_logo->setStyleSheet
		(
			"border-image: url(:/PandaChatClient/all_image/Win_Login/uncheck_account_ logo.png);"
		);
	}

	if (obj == ui->edit_password&&e->type() == QEvent::FocusIn)
	{
		ui->wid_password->setStyleSheet
		(
			"#wid_password"
			"{"
			"border:1px solid;"
			"border-color:rgb(80,90,196);"
			"}"
		);

		ui->lab_password_logo->setStyleSheet
		(
			"border-image:url(:/PandaChatClient/all_image/Win_Login/check_code_logo.png);"
		);

		state_wid->hide();
	}

	if (obj == ui->edit_password&&e->type() == QEvent::FocusOut)
	{
		ui->wid_password->setStyleSheet
		(
			"#wid_password"
			"{"
			"border:1px solid;"
			"border-top:0	px;"
			"border-left:0px;"
			"border-right:0px;"
			"border-color:rgb(229, 229, 229);"
			"}"
		);

		ui->lab_password_logo->setStyleSheet
		(
			"border-image:url(:/PandaChatClient/all_image/Win_Login/uncheck_code_logo.png);"
		);
	}

	//捕捉该所有按钮的点击事件 用于焦点清除
	if ((obj == ui->btn_setting || obj == ui->btn_min || obj == ui->btn_close || obj == ui->checkbtn_auto_login || obj == ui->checkbtn_remb_password || obj == ui->btn_search_password || obj == ui->btn_register || obj == ui->btn_login_in)&&e->type() == QEvent::MouseButtonPress)
	{
		state_wid->hide();
	}

	return QWidget::eventFilter(obj, e);
}

void LoginWin::paintEvent(QPaintEvent *event)
{
	//窗口黑边
	//相当于依次画10个a值递减德矩形
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addRect(10, 10, this->width() - 20, this->height() - 20);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillPath(path, QBrush(Qt::white));

	QColor color(0, 0, 0, 50);
	for (int i = 0; i < 10; i++)
	{
		QPainterPath path;
		path.setFillRule(Qt::WindingFill);
		path.addRect(10 - i, 10 - i, this->width() - (10 - i) * 2, this->height() - (10 - i) * 2);
		color.setAlpha(150 - qSqrt(i) * 50);
		/*if (i > 8)
		{
			color.setAlpha(0);
		}*/

		painter.setPen(color);
		painter.drawPath(path);
	}
}

void LoginWin::mouseMoveEvent(QMouseEvent *e)
{
	if(drag_state)
	{
		move(e->pos() + pos() - start_move_point);
	}
}

void LoginWin::mouseReleaseEvent(QMouseEvent *e)
{
	drag_state = false;
}

void LoginWin::mousePressEvent(QMouseEvent *e)
{
	//qDebug() << "enter press event";

	QMouseEvent *mouse_event;
	mouse_event = static_cast<QMouseEvent*>(e);

	//如果不包含在账号密码框内 清里ui焦点
	QPoint temp_point = mouse_event->pos();

	//这里的Rect实际上是账号框和密码框在上级父窗口中的点,要转换成在最上级父窗口的坐标 也不要用屏幕坐标
	QRect new_account_rect = ui->edit_account->rect();
	new_account_rect.moveTo(ui->edit_account->mapTo(this, QPoint(0,0)));

	QRect new_password_rect = ui->edit_password->rect();
	new_password_rect.moveTo(ui->edit_password->mapTo(this, QPoint(0,0)));

	bool is_in_account = new_account_rect.contains(temp_point);
	bool is_in_password = new_password_rect.contains(temp_point);
	//qDebug()<<is_in_account<<" "<< is_in_password;
	if (!is_in_account&&!is_in_password)
	{
		this->ClearUi();
	}

	//记录移动鼠标点击点
	if (mouse_event->button() == Qt::LeftButton)
	{
		QPoint temp_point = mouse_event->pos();
		if (ui->wid_top->rect().contains(temp_point))
		{
			start_move_point = temp_point;
			drag_state = true;
		}
		else
		{
			drag_state = false;
		}
		return QWidget::mousePressEvent(e);
	}
}

void LoginWin::showEvent(QShowEvent *event)
{
	this->ClearUi();
}

void LoginWin::InitUi()
{
	//无边框
	this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setStyleSheet("background:rgba(0,255,255,0.0);");

	//安装事件过滤器
	//ui->wid_top->installEventFilter(this);
	ui->edit_account->installEventFilter(this);
	ui->edit_password->installEventFilter(this);
	ui->btn_close->installEventFilter(this);
	ui->btn_min->installEventFilter(this);
	ui->btn_setting->installEventFilter(this);
	ui->checkbtn_auto_login->installEventFilter(this);
	ui->checkbtn_remb_password->installEventFilter(this);
	ui->btn_search_password->installEventFilter(this);
	ui->btn_register->installEventFilter(this);
	ui->btn_login_in->installEventFilter(this);

	//头像窗口
	avatar_wid = new LoginAvatarWid(ui->main_page);
	connect(avatar_wid, SIGNAL(OpenLoginStateWinSig()), this, SLOT(OpenStateMenuSlot()));
	avatar_wid->resize(70, 70);
	//获取登陆配置文件头像
	avatar_wid->SetAvatarPic(LoginSetting::GetLoginSetting()->GetLoginPixmap());

	//移动到中间位置
	int avatar_wid_pos_x =(this->width()-avatar_wid->width())/2;
	int avatar_wid_pos_y = ui->wid_top->height() - avatar_wid->width() / 2 + 5;//5是外层margin
	avatar_wid->move(avatar_wid_pos_x, avatar_wid_pos_y);

	//状态窗口
	state_wid = new LoginStateWid(ui->main_page);
	connect(state_wid, SIGNAL(ChooseState(USER_STATE)), this, SLOT(ChooseStateSlot(USER_STATE)));
	state_wid->hide();
	//移动到avatar_wid右下角
	state_wid->move(QPoint(avatar_wid->pos().x() + avatar_wid->width()-5, avatar_wid->pos().y() + avatar_wid->height()-5));

	//登陆头像窗口
	logining_avatar_wid = new LoginAvatarWid(ui->wid_logining);
	//这里直接移动到与avatar_wid_pos_y相对应位置
	logining_avatar_wid->move(avatar_wid_pos_x, avatar_wid_pos_y);

	//输入窗口控制
	this->InitEditInupt();

	//显示登陆界面
	ui->stack_wid_main->setCurrentIndex(LoginPage);
}

void LoginWin::InitEditInupt()
{
	//正则表达式输入控制
	//账号正则控制----只输入数字
	QRegExp reg_account("^[0-9]+$");
	ui->edit_account->setValidator(new QRegExpValidator(reg_account, this));

	//密码正则控制----只输入数字或者字母
	QRegExp reg_password("^[A-Za-z0-9]+$");
	ui->edit_password->setValidator(new QRegExpValidator(reg_password, this));


    //注册昵称 不限制

	//注册密码---只输入数字或者字母
	QRegExp reg_regist_password("^[A-Za-z0-9]+$");
	ui->edit_regist_password->setValidator(new QRegExpValidator(reg_regist_password, this));
	ui->edit_regist_password_confirm->setValidator(new QRegExpValidator(reg_regist_password, this));
}

void LoginWin::InitFont()
{
	//***************登陆界面
	//账号输入
	ui->edit_account->setFont(RX_FONT_CHINESE_LEVEL_2);
	//密码输入
	ui->edit_password->setFont(RX_FONT_CHINESE_LEVEL_2);
	//自动登陆
	ui->checkbtn_auto_login->setFont(RX_FONT_CHINESE_LEVEL_3);
	//记住密码
	ui->checkbtn_remb_password->setFont(RX_FONT_CHINESE_LEVEL_3);
	//找回密码
	ui->btn_search_password->setFont(RX_FONT_CHINESE_LEVEL_3);
	//注册账号
	ui->btn_register->setFont(RX_FONT_CHINESE_LEVEL_3);
	//登陆
	ui->btn_login_in->setFont(RX_FONT_CHINESE_LEVEL_2);


	//***************注册界面
	//注册昵称
	ui->lab_regist_name->setFont(RX_FONT_CHINESE_LEVEL_3);
	//注册昵称输入
	ui->edit_regist_name->setFont(RX_FONT_CHINESE_LEVEL_3);
	//注册密码
	ui->lab_regist_password->setFont(RX_FONT_CHINESE_LEVEL_3);
	//注册密码输入
	ui->edit_regist_password->setFont(RX_FONT_CHINESE_LEVEL_3);
	//注册密码确认
	ui->lab_regist_password_confirm->setFont(RX_FONT_CHINESE_LEVEL_3);
	//注册密码确认输入
	ui->edit_regist_password_confirm->setFont(RX_FONT_CHINESE_LEVEL_3);
	//注册按钮
	ui->btn_regist->setFont(RX_FONT_CHINESE_LEVEL_3);
	//注册取消按钮
	ui->btn_cancel_regist->setFont(RX_FONT_CHINESE_LEVEL_3);
	//注册提示
	ui->lab_regist_tips->setFont(RX_FONT_CHINESE_LEVEL_3);
}

void LoginWin::InitVar()
{
	drag_state = false;
}

void LoginWin::ClearUi()
{
	//自己不断添加
	state_wid->hide();                    //状态窗口
	ui->edit_account->clearFocus();       //账号框焦点清理
	ui->edit_password->clearFocus();      //密码框焦点清理
}

bool LoginWin::RegistCheck()
{
	//首先客户端检测
	//昵称是否为空
	QString nick_name = ui->edit_regist_name->text();
	if (nick_name.isEmpty())
	{
		ui->lab_regist_tips->setText(QString::fromUtf8("昵称不能为空!"));
		return false;
	}

	//密码是否为空
	QString password = ui->edit_regist_password->text();
	QString confirm_password = ui->edit_regist_password_confirm->text();
	if (password.isEmpty()|| confirm_password.isEmpty())
	{
		ui->lab_regist_tips->setText(QString::fromUtf8("密码不能为空!"));
		return false;
	}

	//密码是否一致
	if (password != confirm_password)
	{
		ui->lab_regist_tips->setText(QString::fromUtf8("两次输入密码不一致!"));
		return false;
	}

	return true;
}

void LoginWin::on_btn_close_clicked()
{
	qApp->quit();
}

void LoginWin::on_btn_min_clicked()
{
	this->showMinimized();
}

void LoginWin::OpenStateMenuSlot()
{
	state_wid->show();
}

void LoginWin::ChooseStateSlot(USER_STATE state)
{
	avatar_wid->SetUserCurState(state);

	//去除下焦点 这里会将焦点切入账号框 原因未知。
	this->ClearUi();
}

void LoginWin::on_btn_login_in_clicked()
{



}

void LoginWin::on_btn_register_clicked()
{
	this->ClearRegistWin();
	ui->stack_wid_main->setCurrentIndex(RegisterPage);
}

void LoginWin::SetRegistTips(QString tips)
{
	ui->lab_regist_tips->setText(tips);
}

void LoginWin::ClearRegistWin()
{
	ui->edit_regist_name->clear();
	ui->edit_regist_password->clear();
	ui->edit_regist_password_confirm->clear();
	this->SetRegistTips("");
}

void LoginWin::on_btn_regist_clicked()
{
	//业务层

	//检查输入
	bool can_regist = RegistCheck();

	if (!can_regist)
	{
		return;
	}








}

void LoginWin::on_btn_cancel_regist_clicked()
{
	ui->stack_wid_main->setCurrentIndex(LoginPage);
}

void LoginWin::on_btn_regist_close_clicked()
{
	qApp->quit();
}

void LoginWin::HandleRegist(CRegisterResult *p_result)
{	
	long regier_result_code = p_result->m_nResultCode;

	if (RegisteredAlready == regier_result_code)
	{
		//已经注册
		TipsDialog::GetMessageBox()->SetProperty(TipsDialog::OK_Mode, QString::fromUtf8("提示"), QString::fromUtf8("该账号已经注册"), this);
		TipsDialog::GetMessageBox()->exec();
	}
	else if (RegisteredFailed == regier_result_code)
	{
		//注册失败
		TipsDialog::GetMessageBox()->SetProperty(TipsDialog::OK_Mode, QString::fromUtf8("提示"), QString::fromUtf8("账号注册失败"), this);
		TipsDialog::GetMessageBox()->exec();
	}
	else if (RegisteredSuccess == regier_result_code)
	{
		//注册成功
		TipsDialog::GetMessageBox()->SetProperty(TipsDialog::OK_Mode, QString::fromUtf8("提示"), QString::fromUtf8("账号注册成功"), this);
		TipsDialog::GetMessageBox()->exec();

		//回到登陆界面
		ui->stack_wid_main->setCurrentIndex(LoginPage);
	}
	delete p_result;
	p_result = NULL;
}
