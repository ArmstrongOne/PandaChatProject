#pragma once

#include <QWidget>
namespace Ui { class ChatWin; };

class ChatWin : public QWidget
{
	Q_OBJECT

public:
	ChatWin(QWidget *parent = Q_NULLPTR);
	~ChatWin();

private:
	Ui::ChatWin *ui;
};
