/********************************************************************************
** Form generated from reading UI file 'rga_item.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RGA_ITEM_H
#define UI_RGA_ITEM_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RGA_item
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *RGA_Info;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *RGA_lineEdit;
    QSpacerItem *horizontalSpacer;
    QPushButton *force_pushButton;
    QPushButton *Connect_pushButton;
    QFrame *TCPIP_Set;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QSpinBox *IP1;
    QLabel *label_4;
    QSpinBox *IP2;
    QLabel *label_5;
    QSpinBox *IP3;
    QLabel *label_6;
    QSpinBox *IP4;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *Setting_pushButton;

    void setupUi(QWidget *RGA_item)
    {
        if (RGA_item->objectName().isEmpty())
            RGA_item->setObjectName("RGA_item");
        RGA_item->resize(320, 65);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RGA_item->sizePolicy().hasHeightForWidth());
        RGA_item->setSizePolicy(sizePolicy);
        RGA_item->setMinimumSize(QSize(320, 65));
        RGA_item->setMaximumSize(QSize(320, 65));
        RGA_item->setFocusPolicy(Qt::NoFocus);
        RGA_item->setToolTipDuration(-1);
        RGA_item->setStyleSheet(QString::fromUtf8("\n"
"QWidget {\n"
"   font: 700 14pt \"Microsoft YaHei UI\";\n"
"	color: rgb(255, 255, 255);\n"
"}\n"
"\n"
"QSpinBox{\n"
"	background-color: transparent;\n"
"    border-radius: 5px; \n"
"\n"
"\n"
"}\n"
""));
        verticalLayout = new QVBoxLayout(RGA_item);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        RGA_Info = new QFrame(RGA_item);
        RGA_Info->setObjectName("RGA_Info");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(RGA_Info->sizePolicy().hasHeightForWidth());
        RGA_Info->setSizePolicy(sizePolicy1);
        RGA_Info->setMinimumSize(QSize(0, 30));
        RGA_Info->setMaximumSize(QSize(16777215, 30));
        RGA_Info->setFrameShape(QFrame::StyledPanel);
        RGA_Info->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(RGA_Info);
        horizontalLayout_3->setSpacing(4);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        RGA_lineEdit = new QLineEdit(RGA_Info);
        RGA_lineEdit->setObjectName("RGA_lineEdit");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(RGA_lineEdit->sizePolicy().hasHeightForWidth());
        RGA_lineEdit->setSizePolicy(sizePolicy2);
        RGA_lineEdit->setMinimumSize(QSize(140, 25));
        RGA_lineEdit->setMaximumSize(QSize(140, 25));
        QFont font;
        font.setFamilies({QString::fromUtf8("Microsoft YaHei UI")});
        font.setPointSize(14);
        font.setBold(true);
        font.setItalic(false);
        RGA_lineEdit->setFont(font);
        RGA_lineEdit->setCursor(QCursor(Qt::ArrowCursor));
        RGA_lineEdit->setFocusPolicy(Qt::ClickFocus);
        RGA_lineEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    border: none;   \n"
"    border-radius: 5px; \n"
"    background: transparent;  \n"
"}\n"
"\n"
"QLineEdit:focus { \n"
"                        background-color: rgb(58, 143, 255);\n"
"}"));
        RGA_lineEdit->setMaxLength(8);
        RGA_lineEdit->setReadOnly(false);
        RGA_lineEdit->setClearButtonEnabled(false);

        horizontalLayout_3->addWidget(RGA_lineEdit);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        force_pushButton = new QPushButton(RGA_Info);
        force_pushButton->setObjectName("force_pushButton");
        sizePolicy.setHeightForWidth(force_pushButton->sizePolicy().hasHeightForWidth());
        force_pushButton->setSizePolicy(sizePolicy);
        force_pushButton->setMinimumSize(QSize(25, 25));
        force_pushButton->setMaximumSize(QSize(25, 25));
        force_pushButton->setFocusPolicy(Qt::NoFocus);
        force_pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    /* \346\255\243\345\270\270\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"	font: 700 14pt \"Microsoft YaHei UI\";\n"
"   color:rgb(255, 255, 255);\n"
"    border-radius: 5px; \n"
"}\n"
""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/icon/NoControl_RGA.png"), QSize(), QIcon::Normal, QIcon::Off);
        force_pushButton->setIcon(icon);
        force_pushButton->setIconSize(QSize(20, 20));

        horizontalLayout_3->addWidget(force_pushButton);

        Connect_pushButton = new QPushButton(RGA_Info);
        Connect_pushButton->setObjectName("Connect_pushButton");
        sizePolicy.setHeightForWidth(Connect_pushButton->sizePolicy().hasHeightForWidth());
        Connect_pushButton->setSizePolicy(sizePolicy);
        Connect_pushButton->setMinimumSize(QSize(100, 25));
        Connect_pushButton->setMaximumSize(QSize(100, 25));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Microsoft YaHei UI")});
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setItalic(false);
        font1.setStyleStrategy(QFont::PreferDefault);
        Connect_pushButton->setFont(font1);
        Connect_pushButton->setFocusPolicy(Qt::ClickFocus);
        Connect_pushButton->setLayoutDirection(Qt::RightToLeft);
        Connect_pushButton->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"    /* \346\255\243\345\270\270\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"	font: 700 12pt \"Microsoft YaHei UI\";\n"
"   color:rgb(255, 255, 255);\n"
"    border-radius: 5px; \n"
"    border:1px solid rgb(255, 255, 255);\n"
"}\n"
"QPushButton:hover {\n"
"    /* \351\274\240\346\240\207\346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"   background-color:#87CEFA; /* \346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"}\n"
""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/new/prefix1/icon/TCP_NoConnect.png"), QSize(), QIcon::Normal, QIcon::Off);
        Connect_pushButton->setIcon(icon1);
        Connect_pushButton->setIconSize(QSize(20, 20));
        Connect_pushButton->setCheckable(true);

        horizontalLayout_3->addWidget(Connect_pushButton);


        verticalLayout->addWidget(RGA_Info);

        TCPIP_Set = new QFrame(RGA_item);
        TCPIP_Set->setObjectName("TCPIP_Set");
        sizePolicy.setHeightForWidth(TCPIP_Set->sizePolicy().hasHeightForWidth());
        TCPIP_Set->setSizePolicy(sizePolicy);
        TCPIP_Set->setMinimumSize(QSize(250, 30));
        TCPIP_Set->setMaximumSize(QSize(16777215, 30));
        TCPIP_Set->setStyleSheet(QString::fromUtf8(""));
        TCPIP_Set->setFrameShape(QFrame::StyledPanel);
        TCPIP_Set->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(TCPIP_Set);
        horizontalLayout_4->setSpacing(4);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(TCPIP_Set);
        label->setObjectName("label");

        horizontalLayout_4->addWidget(label);

        IP1 = new QSpinBox(TCPIP_Set);
        IP1->setObjectName("IP1");
        IP1->setMinimumSize(QSize(45, 30));
        IP1->setFont(font);
        IP1->setFocusPolicy(Qt::NoFocus);
        IP1->setStyleSheet(QString::fromUtf8(""));
        IP1->setWrapping(false);
        IP1->setFrame(true);
        IP1->setAlignment(Qt::AlignCenter);
        IP1->setReadOnly(true);
        IP1->setButtonSymbols(QAbstractSpinBox::NoButtons);
        IP1->setMaximum(255);
        IP1->setValue(192);

        horizontalLayout_4->addWidget(IP1);

        label_4 = new QLabel(TCPIP_Set);
        label_4->setObjectName("label_4");
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label_4);

        IP2 = new QSpinBox(TCPIP_Set);
        IP2->setObjectName("IP2");
        IP2->setMinimumSize(QSize(45, 30));
        IP2->setFont(font);
        IP2->setFocusPolicy(Qt::NoFocus);
        IP2->setStyleSheet(QString::fromUtf8(""));
        IP2->setAlignment(Qt::AlignCenter);
        IP2->setReadOnly(true);
        IP2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        IP2->setMaximum(255);
        IP2->setValue(168);

        horizontalLayout_4->addWidget(IP2);

        label_5 = new QLabel(TCPIP_Set);
        label_5->setObjectName("label_5");
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label_5);

        IP3 = new QSpinBox(TCPIP_Set);
        IP3->setObjectName("IP3");
        IP3->setMinimumSize(QSize(45, 30));
        IP3->setFont(font);
        IP3->setFocusPolicy(Qt::NoFocus);
        IP3->setStyleSheet(QString::fromUtf8(""));
        IP3->setAlignment(Qt::AlignCenter);
        IP3->setReadOnly(true);
        IP3->setButtonSymbols(QAbstractSpinBox::NoButtons);
        IP3->setCorrectionMode(QAbstractSpinBox::CorrectToPreviousValue);
        IP3->setMinimum(0);
        IP3->setMaximum(255);
        IP3->setStepType(QAbstractSpinBox::DefaultStepType);
        IP3->setValue(1);

        horizontalLayout_4->addWidget(IP3);

        label_6 = new QLabel(TCPIP_Set);
        label_6->setObjectName("label_6");
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label_6);

        IP4 = new QSpinBox(TCPIP_Set);
        IP4->setObjectName("IP4");
        IP4->setMinimumSize(QSize(45, 30));
        IP4->setFont(font);
        IP4->setFocusPolicy(Qt::NoFocus);
        IP4->setStyleSheet(QString::fromUtf8(""));
        IP4->setAlignment(Qt::AlignCenter);
        IP4->setReadOnly(true);
        IP4->setButtonSymbols(QAbstractSpinBox::NoButtons);
        IP4->setMaximum(255);
        IP4->setValue(101);

        horizontalLayout_4->addWidget(IP4);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        Setting_pushButton = new QPushButton(TCPIP_Set);
        Setting_pushButton->setObjectName("Setting_pushButton");
        sizePolicy.setHeightForWidth(Setting_pushButton->sizePolicy().hasHeightForWidth());
        Setting_pushButton->setSizePolicy(sizePolicy);
        Setting_pushButton->setMinimumSize(QSize(25, 25));
        Setting_pushButton->setMaximumSize(QSize(25, 25));
        Setting_pushButton->setFocusPolicy(Qt::NoFocus);
        Setting_pushButton->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"    /* \346\255\243\345\270\270\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"	font: 700 12pt \"Microsoft YaHei UI\";\n"
"   color:rgb(255, 255, 255);\n"
"    border-radius: 5px; \n"
"    border:1px solid rgb(255, 255, 255);\n"
"}\n"
"QPushButton:hover {\n"
"    /* \351\274\240\346\240\207\346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"   background-color:#87CEFA; /* \346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"}"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/new/prefix1/icon/setting_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        Setting_pushButton->setIcon(icon2);
        Setting_pushButton->setIconSize(QSize(20, 20));

        horizontalLayout_4->addWidget(Setting_pushButton);


        verticalLayout->addWidget(TCPIP_Set);


        retranslateUi(RGA_item);

        QMetaObject::connectSlotsByName(RGA_item);
    } // setupUi

    void retranslateUi(QWidget *RGA_item)
    {
        RGA_item->setWindowTitle(QCoreApplication::translate("RGA_item", "Form", nullptr));
#if QT_CONFIG(tooltip)
        RGA_item->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        RGA_lineEdit->setToolTip(QCoreApplication::translate("RGA_item", "\350\256\276\345\244\207\345\220\215\347\247\260", nullptr));
#endif // QT_CONFIG(tooltip)
        RGA_lineEdit->setText(QCoreApplication::translate("RGA_item", "MyRGA", nullptr));
        RGA_lineEdit->setPlaceholderText(QString());
#if QT_CONFIG(tooltip)
        force_pushButton->setToolTip(QCoreApplication::translate("RGA_item", "\350\256\276\345\244\207\347\212\266\346\200\201", nullptr));
#endif // QT_CONFIG(tooltip)
        force_pushButton->setText(QString());
#if QT_CONFIG(tooltip)
        Connect_pushButton->setToolTip(QCoreApplication::translate("RGA_item", "TCP/IP\350\277\236\346\216\245", nullptr));
#endif // QT_CONFIG(tooltip)
        Connect_pushButton->setText(QCoreApplication::translate("RGA_item", " \346\234\252\350\277\236\346\216\245 ", nullptr));
        label->setText(QCoreApplication::translate("RGA_item", "IP:", nullptr));
        label_4->setText(QCoreApplication::translate("RGA_item", "\302\267", nullptr));
        label_5->setText(QCoreApplication::translate("RGA_item", "\302\267", nullptr));
        label_6->setText(QCoreApplication::translate("RGA_item", "\302\267", nullptr));
#if QT_CONFIG(tooltip)
        Setting_pushButton->setToolTip(QCoreApplication::translate("RGA_item", "ip\350\256\276\345\256\232", nullptr));
#endif // QT_CONFIG(tooltip)
        Setting_pushButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RGA_item: public Ui_RGA_item {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RGA_ITEM_H
