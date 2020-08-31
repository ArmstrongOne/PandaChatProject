#include "stdafx.h"
#include "MainWin.h"
#include "ui_MainWin.h"
void MainWin::Init()
{

}

void MainWin::Show()
{

}

MainWin::MainWin(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::MainWin();
	ui->setupUi(this);

	this->InitUi();
	this->InitFont();
}

void MainWin::InitUi()
{

}

void MainWin::InitFont()
{

}


MainWin::~MainWin()
{
	delete ui;
}
