/********************************************************************************
** Form generated from reading UI file 'ChatWin.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATWIN_H
#define UI_CHATWIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatWin
{
public:

    void setupUi(QWidget *ChatWin)
    {
        if (ChatWin->objectName().isEmpty())
            ChatWin->setObjectName(QStringLiteral("ChatWin"));
        ChatWin->resize(400, 300);

        retranslateUi(ChatWin);

        QMetaObject::connectSlotsByName(ChatWin);
    } // setupUi

    void retranslateUi(QWidget *ChatWin)
    {
        ChatWin->setWindowTitle(QApplication::translate("ChatWin", "ChatWin", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ChatWin: public Ui_ChatWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATWIN_H
