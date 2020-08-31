#pragma once

#include <QDialog>
namespace Ui { class TipsDialog; };

class TipsDialog : public QDialog
{
	Q_OBJECT

public:
	~TipsDialog();

	//ģʽ ֻ��OK ����ֻ��ȡ����ȷ��
	enum ReturnValue
	{
		OK, //���غõ� 0
		Confirm,//����ȷ�� 1
		Cancel,//����ȡ��  2
		None,//��ʼ��ΪNone
	};

	//�Զ��巵��ֵ ����QDialog���õ�accept��reject
	enum Mode
	{
		OK_Mode, //Okģʽ
		CancelAndConfirm_Mode,//ȷ�Ϻ�ȡ��ģʽ
	};
	static TipsDialog*GetMessageBox();

	//ģʽ������������һ���� ÿ������ֻҪ��һ�仰 ���parent����������ÿ�ζ�Ҫ�����ڸ���������λ��
	void SetProperty(Mode mode, QString title, QString text, QWidget *parent);
	ReturnValue GetReturnValue()
	{
		return return_value;
	};

private:
	TipsDialog(QWidget *parent = Q_NULLPTR);
	static TipsDialog*messbox;

	Ui::TipsDialog *ui;
	ReturnValue return_value;

	void InitFont();


	private slots:
	void on_confirm_btn_clicked();
	void on_ok_btn_clicked();
	void on_cancel_btn_clicked();
};
