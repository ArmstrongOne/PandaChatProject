#include "TipsDialog.h"
#include "ui_TipsDialog.h"
#include<QApplication>
#include<QDesktopWidget>
//#include<CTTextSetting.h>
#include"Ini\FontIni.h"
#include<QFont>
//��̬��Ա��ʼ��
TipsDialog* TipsDialog::messbox = NULL;

void TipsDialog::InitFont()
{
	//������ʾ��
	ui->tips_lab->setFont(RX_FONT_CHINESE_LEVEL_3);

	//��������
	ui->text_lab->setFont(RX_FONT_CHINESE_LEVEL_3);

	//��ť����
	ui->confirm_btn->setFont(RX_FONT_CHINESE_LEVEL_3);
	ui->ok_btn->setFont(RX_FONT_CHINESE_LEVEL_3);
	ui->cancel_btn->setFont(RX_FONT_CHINESE_LEVEL_3);
}

TipsDialog::TipsDialog(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::TipsDialog;
	ui->setupUi(this);
	//ȥ���߿�
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->resize(304, 184);

	this->InitFont();
	return_value = None;
	messbox = NULL;
}

TipsDialog::~TipsDialog()
{
	delete ui;
}

TipsDialog* TipsDialog::GetMessageBox()
{
	if (!messbox)
	{
		messbox = new TipsDialog;
	}
	return messbox;
}

void TipsDialog::SetProperty(Mode mode, QString title, QString text, QWidget *parent)
{
	switch (mode)
	{
	case TipsDialog::OK_Mode:
		ui->ok_btn->setVisible(true);
		ui->confirm_btn->setVisible(false);
		ui->cancel_btn->setVisible(false);
		break;
	case TipsDialog::CancelAndConfirm_Mode:
		ui->ok_btn->setVisible(false);
		ui->confirm_btn->setVisible(true);
		ui->cancel_btn->setVisible(true);
		break;
	default:
		break;
	}
	ui->text_lab->setText(text);

	//������������
	//�ƶ�����Ļ����
	int desktop_width = QApplication::desktop()->width();
	int desktop_height = QApplication::desktop()->height();
	this->move((desktop_width - this->width()) / 2, (desktop_height - this->height()) / 2);

	//���ñ���
	ui->tips_lab->setText(title);
}

void TipsDialog::on_confirm_btn_clicked()
{
	this->close();
	return_value = Confirm;
}

void TipsDialog::on_ok_btn_clicked()
{
	this->close();
	return_value = OK;
}

void TipsDialog::on_cancel_btn_clicked()
{
	this->close();
	return_value = Cancel;
}
