#include "stdafx.h"
#include "ChatWin.h"
#include "ui_ChatWin.h"

ChatWin::ChatWin(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::ChatWin();
	ui->setupUi(this);
}

ChatWin::~ChatWin()
{
	delete ui;
}
