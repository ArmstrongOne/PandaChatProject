/********************************************************************************
** Form generated from reading UI file 'TipsDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TIPSDIALOG_H
#define UI_TIPSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TipsDialog
{
public:
    QGridLayout *gridLayout;
    QWidget *main_wid;
    QGridLayout *gridLayout_5;
    QWidget *tip_widget;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QLabel *tips_lab;
    QSpacerItem *horizontalSpacer_2;
    QWidget *text_widget;
    QGridLayout *gridLayout_3;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *horizontalSpacer_3;
    QLabel *text_lab;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_4;
    QWidget *btgroup_widget;
    QGridLayout *gridLayout_4;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *horizontalSpacer_9;
    QSpacerItem *jnmyuhjnnbh;
    QSpacerItem *verticalSpacer_6;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *confirm_btn;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *ok_btn;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *cancel_btn;
    QSpacerItem *horizontalSpacer_8;

    void setupUi(QDialog *TipsDialog)
    {
        if (TipsDialog->objectName().isEmpty())
            TipsDialog->setObjectName(QStringLiteral("TipsDialog"));
        TipsDialog->resize(335, 185);
        TipsDialog->setMinimumSize(QSize(335, 185));
        TipsDialog->setMaximumSize(QSize(335, 185));
        TipsDialog->setStyleSheet(QStringLiteral(""));
        gridLayout = new QGridLayout(TipsDialog);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        main_wid = new QWidget(TipsDialog);
        main_wid->setObjectName(QStringLiteral("main_wid"));
        main_wid->setStyleSheet(QLatin1String("#main_wid\n"
"{\n"
"border:0px solid;\n"
"border-color:rgb(31, 193, 199);\n"
"background-color:rgb(26,26,26);\n"
"}\n"
"\n"
""));
        gridLayout_5 = new QGridLayout(main_wid);
        gridLayout_5->setSpacing(0);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        tip_widget = new QWidget(main_wid);
        tip_widget->setObjectName(QStringLiteral("tip_widget"));
        tip_widget->setMinimumSize(QSize(0, 30));
        tip_widget->setMaximumSize(QSize(16777215, 30));
        tip_widget->setStyleSheet(QLatin1String("#tip_widget\n"
"{\n"
"background-color:rgb(81,93,202);\n"
"\n"
"border:0px solid;\n"
"border-bottom-color: rgb(31, 193, 199);\n"
"\n"
"border-left:0px;\n"
"border-right:0px;\n"
"border-top:0px;\n"
"\n"
"}"));
        horizontalLayout_2 = new QHBoxLayout(tip_widget);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 4);
        horizontalSpacer = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        label = new QLabel(tip_widget);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(30, 30));
        label->setMaximumSize(QSize(30, 30));
        label->setSizeIncrement(QSize(30, 0));
        label->setStyleSheet(QLatin1String("border-image: url(:/PandaChatClient/all_image/Win_Login/logo.png);\n"
"margin-top:5px;\n"
"margin-bottom:5px;\n"
"margin-left:5px;\n"
"margin-right:5px;"));

        horizontalLayout_2->addWidget(label);

        tips_lab = new QLabel(tip_widget);
        tips_lab->setObjectName(QStringLiteral("tips_lab"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tips_lab->sizePolicy().hasHeightForWidth());
        tips_lab->setSizePolicy(sizePolicy);
        tips_lab->setMinimumSize(QSize(1, 1));
        tips_lab->setStyleSheet(QLatin1String("\n"
"color: rgb(255, 255, 255);"));

        horizontalLayout_2->addWidget(tips_lab);

        horizontalSpacer_2 = new QSpacerItem(163, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 10);
        horizontalLayout_2->setStretch(2, 10);
        horizontalLayout_2->setStretch(3, 40);

        gridLayout_5->addWidget(tip_widget, 0, 0, 1, 1);

        text_widget = new QWidget(main_wid);
        text_widget->setObjectName(QStringLiteral("text_widget"));
        text_widget->setStyleSheet(QStringLiteral("background-color:rgb(244,243,244);"));
        gridLayout_3 = new QGridLayout(text_widget);
        gridLayout_3->setSpacing(0);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(30, 0, 0, 0);
        verticalSpacer_3 = new QSpacerItem(20, 9, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_3, 0, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_3, 1, 0, 2, 1);

        text_lab = new QLabel(text_widget);
        text_lab->setObjectName(QStringLiteral("text_lab"));
        sizePolicy.setHeightForWidth(text_lab->sizePolicy().hasHeightForWidth());
        text_lab->setSizePolicy(sizePolicy);
        text_lab->setMinimumSize(QSize(1, 1));
        text_lab->setStyleSheet(QLatin1String("\n"
"color: rgb(255, 255, 255);\n"
"\n"
"\n"
""));
        text_lab->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_3->addWidget(text_lab, 1, 1, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_4, 1, 2, 2, 1);

        verticalSpacer_4 = new QSpacerItem(20, 9, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_4, 2, 1, 1, 1);

        gridLayout_3->setRowStretch(0, 2);
        gridLayout_3->setRowStretch(1, 10);
        gridLayout_3->setRowStretch(2, 2);
        gridLayout_3->setColumnStretch(0, 1);
        gridLayout_3->setColumnStretch(1, 100);
        gridLayout_3->setColumnStretch(2, 1);

        gridLayout_5->addWidget(text_widget, 1, 0, 1, 1);

        btgroup_widget = new QWidget(main_wid);
        btgroup_widget->setObjectName(QStringLiteral("btgroup_widget"));
        btgroup_widget->setMinimumSize(QSize(0, 35));
        btgroup_widget->setMaximumSize(QSize(16777215, 35));
        btgroup_widget->setStyleSheet(QStringLiteral("background-color:rgb(226,226,227);"));
        gridLayout_4 = new QGridLayout(btgroup_widget);
        gridLayout_4->setSpacing(0);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_5 = new QSpacerItem(20, 9, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_5, 0, 1, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_9, 1, 0, 1, 1);

        jnmyuhjnnbh = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(jnmyuhjnnbh, 1, 2, 1, 1);

        verticalSpacer_6 = new QSpacerItem(20, 9, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_6, 2, 1, 1, 1);

        widget = new QWidget(btgroup_widget);
        widget->setObjectName(QStringLiteral("widget"));
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(4);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(4, 4, 4, 4);
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_7);

        confirm_btn = new QPushButton(widget);
        confirm_btn->setObjectName(QStringLiteral("confirm_btn"));
        sizePolicy.setHeightForWidth(confirm_btn->sizePolicy().hasHeightForWidth());
        confirm_btn->setSizePolicy(sizePolicy);
        confirm_btn->setMinimumSize(QSize(75, 23));
        confirm_btn->setMaximumSize(QSize(75, 23));
        confirm_btn->setStyleSheet(QLatin1String("color: rgb(0, 0, 0);\n"
"background-color: rgb(225, 225, 225);\n"
""));

        horizontalLayout->addWidget(confirm_btn);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);

        ok_btn = new QPushButton(widget);
        ok_btn->setObjectName(QStringLiteral("ok_btn"));
        sizePolicy.setHeightForWidth(ok_btn->sizePolicy().hasHeightForWidth());
        ok_btn->setSizePolicy(sizePolicy);
        ok_btn->setMinimumSize(QSize(75, 23));
        ok_btn->setMaximumSize(QSize(75, 23));
        ok_btn->setStyleSheet(QLatin1String("color: rgb(0, 0, 0);\n"
"background-color: rgb(225, 225, 225);"));

        horizontalLayout->addWidget(ok_btn);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_6);

        cancel_btn = new QPushButton(widget);
        cancel_btn->setObjectName(QStringLiteral("cancel_btn"));
        sizePolicy.setHeightForWidth(cancel_btn->sizePolicy().hasHeightForWidth());
        cancel_btn->setSizePolicy(sizePolicy);
        cancel_btn->setMinimumSize(QSize(75, 23));
        cancel_btn->setMaximumSize(QSize(75, 23));
        cancel_btn->setStyleSheet(QLatin1String("color: rgb(0, 0, 0);\n"
"background-color: rgb(225, 225, 225);"));

        horizontalLayout->addWidget(cancel_btn);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_8);

        horizontalLayout->setStretch(0, 10);
        horizontalLayout->setStretch(1, 4);
        horizontalLayout->setStretch(2, 1);
        horizontalLayout->setStretch(3, 4);
        horizontalLayout->setStretch(4, 1);
        horizontalLayout->setStretch(5, 4);
        horizontalLayout->setStretch(6, 1);

        gridLayout_4->addWidget(widget, 1, 1, 1, 1);

        gridLayout_4->setRowStretch(0, 4);
        gridLayout_4->setRowStretch(1, 10);
        gridLayout_4->setRowStretch(2, 4);
        gridLayout_4->setColumnStretch(0, 1);
        gridLayout_4->setColumnStretch(1, 100);
        gridLayout_4->setColumnStretch(2, 1);

        gridLayout_5->addWidget(btgroup_widget, 2, 0, 1, 1);

        gridLayout_5->setRowStretch(0, 15);
        gridLayout_5->setRowStretch(1, 30);
        gridLayout_5->setRowStretch(2, 15);

        gridLayout->addWidget(main_wid, 0, 0, 1, 1);


        retranslateUi(TipsDialog);

        QMetaObject::connectSlotsByName(TipsDialog);
    } // setupUi

    void retranslateUi(QDialog *TipsDialog)
    {
        TipsDialog->setWindowTitle(QApplication::translate("TipsDialog", "TipsDialog", Q_NULLPTR));
        label->setText(QString());
        tips_lab->setText(QApplication::translate("TipsDialog", "\346\217\220\347\244\272!", Q_NULLPTR));
        text_lab->setText(QString());
        confirm_btn->setText(QApplication::translate("TipsDialog", "\346\230\257", Q_NULLPTR));
        ok_btn->setText(QApplication::translate("TipsDialog", "\346\230\257", Q_NULLPTR));
        cancel_btn->setText(QApplication::translate("TipsDialog", "\345\220\246", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class TipsDialog: public Ui_TipsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIPSDIALOG_H
