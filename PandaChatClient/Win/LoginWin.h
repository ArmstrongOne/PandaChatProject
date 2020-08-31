#pragma once
#include <QWidget>
#include"Panda.h"
#include"..\Net\IUProtocolData.h"
using namespace Panda;
namespace Ui { class LoginWin; };

class LoginAvatarWid;//��½ͷ�񴰿�
class LoginStateWid; //��½״̬��

enum LoginPageType
{
	LoginPage,       //��¼ҳ
	RegisterPage     //ע��ҳ
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

		void InitUi();                                  //ui����
		void InitEditInupt();                           //��ʼ���������
		void InitFont();                                //�������
		void InitVar();                                 //��ʼ������
												        
		void SetRegistTips(QString);                    //������ʾ
		void ClearRegistWin();                          //����ע�����
		void ClearUi();                                 //������������

		QPoint                    start_move_point;     //�϶���ʼ��
		bool                      drag_state;           //�϶�״̬
		LoginAvatarWid*           avatar_wid;           //ͷ�񴰿�
		LoginStateWid*            state_wid;            //״̬����
		bool                      RegistCheck();        //ע�����Ƿ����
		LoginAvatarWid*           logining_avatar_wid;  //��½�д���


 private slots:
		void        on_btn_close_clicked();              //�ر�
		void        on_btn_min_clicked();                //��С��
		void        OpenStateMenuSlot();                 //��״̬ѡ����
		void        ChooseStateSlot(USER_STATE);         //״̬ѡ��
		void        on_btn_login_in_clicked();           //��½
		void        on_btn_register_clicked();           //ע��ҳ��
		void        on_btn_regist_clicked();             //��ʼע��
		void        on_btn_cancel_regist_clicked();      //ȡ��ע��
		void        on_btn_regist_close_clicked();       //�ر�

public: //������������
     	void        HandleRegist(CRegisterResult *p_result);      //����ע��
};
