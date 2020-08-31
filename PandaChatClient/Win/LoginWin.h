#pragma once
#include <QWidget>
#include"Panda.h"
#include"..\Net\IUProtocolData.h"
using namespace Panda;
namespace Ui { class LoginWin; };

class LoginAvatarWid;//登陆头像窗口
class LoginStateWid; //登陆状态栏

enum LoginPageType
{
	LoginPage,       //登录页
	RegisterPage     //注册页
};

class LoginWin : public QWidget
{
		Q_OBJECT
 public:
		LoginWin(QWidget *parent = Q_NULLPTR);
		~LoginWin();
		LoginWin(LoginWin&) = delete;
		LoginWin&operator=(LoginWin&) = delete;

		void Init();
		void Show();

 private:
		Ui::LoginWin *ui;
		virtual bool eventFilter(QObject *obj, QEvent *e) override;
		virtual void paintEvent(QPaintEvent *event)override;
		virtual void mouseMoveEvent(QMouseEvent *e)override;
		virtual void mouseReleaseEvent(QMouseEvent *e)override;
		virtual void mousePressEvent(QMouseEvent *e)override;
		virtual void showEvent(QShowEvent *event)override;

		void InitUi();                                  //ui调整
		void InitEditInupt();                           //初始化输入控制
		void InitFont();                                //字体调整
		void InitVar();                                 //初始化变量
												        
		void SetRegistTips(QString);                    //设置提示
		void ClearRegistWin();                          //清理注册界面
		void ClearUi();                                 //做焦点清理工作

		QPoint                    start_move_point;     //拖动开始点
		bool                      drag_state;           //拖动状态
		LoginAvatarWid*           avatar_wid;           //头像窗口
		LoginStateWid*            state_wid;            //状态窗口
		bool                      RegistCheck();        //注册检测是否合理
		LoginAvatarWid*           logining_avatar_wid;  //登陆中窗口


 private slots:
		void        on_btn_close_clicked();              //关闭
		void        on_btn_min_clicked();                //最小化
		void        OpenStateMenuSlot();                 //打开状态选择栏
		void        ChooseStateSlot(USER_STATE);         //状态选择
		void        on_btn_login_in_clicked();           //登陆
		void        on_btn_register_clicked();           //注册页面
		void        on_btn_regist_clicked();             //开始注册
		void        on_btn_cancel_regist_clicked();      //取消注册
		void        on_btn_regist_close_clicked();       //关闭

public: //处理服务端请求
     	void        HandleRegist(CRegisterResult *p_result);      //处理注册
};
