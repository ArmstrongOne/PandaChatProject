#pragma once

#include <QDialog>
namespace Ui { class TipsDialog; };

class TipsDialog : public QDialog
{
	Q_OBJECT

public:
	~TipsDialog();

	//模式 只有OK 或者只有取消和确认
	enum ReturnValue
	{
		OK, //返回好的 0
		Confirm,//返回确定 1
		Cancel,//返回取消  2
		None,//初始化为None
	};

	//自定义返回值 不用QDialog内置的accept和reject
	enum Mode
	{
		OK_Mode, //Ok模式
		CancelAndConfirm_Mode,//确认和取消模式
	};
	static TipsDialog*GetMessageBox();

	//模式和内容索性连一起了 每次设置只要这一句话 这个parent的意义在于每次都要出现在父窗口中心位置
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
