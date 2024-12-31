/********************************************************************************
** Form generated from reading UI file 'home.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOME_H
#define UI_HOME_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_home
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *RGA_Device_Page;
    QVBoxLayout *verticalLayout;
    QFrame *RGA_Menu;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *Hide_RGA_PushButton;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *Remove_RGA_PushButton;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *Add_RGA_PushButton;
    QSpacerItem *horizontalSpacer_3;
    QListWidget *RGA_listWidget;
    QWidget *Display_Page;
    QVBoxLayout *verticalLayout_2;
    QWidget *Menu_Widget;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *Show_RGA_PushButton;
    QPushButton *Sweep_Mode_PushButton;
    QPushButton *Leak_Mode_PushButton;
    QPushButton *Show_RGA_PushButton_4;
    QPushButton *Show_RGA_PushButton_5;
    QPushButton *Show_RGA_PushButton_6;
    QSpacerItem *horizontalSpacer_2;
    QStackedWidget *Display_stackedWidget;

    void setupUi(QWidget *home)
    {
        if (home->objectName().isEmpty())
            home->setObjectName("home");
        home->resize(1025, 517);
        home->setStyleSheet(QString::fromUtf8("#home{\n"
"	background-color: rgb(0, 0, 0);\n"
"}\n"
"\n"
"QWidget\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}"));
        horizontalLayout = new QHBoxLayout(home);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(5, 5, 5, 5);
        RGA_Device_Page = new QWidget(home);
        RGA_Device_Page->setObjectName("RGA_Device_Page");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RGA_Device_Page->sizePolicy().hasHeightForWidth());
        RGA_Device_Page->setSizePolicy(sizePolicy);
        RGA_Device_Page->setStyleSheet(QString::fromUtf8("#RGA_Device_Page{\n"
"	background-color: rgb(1, 32, 79);\n"
"    border-radius: 5px; \n"
"    border:2px solid rgb(255, 255, 255);\n"
"}"));
        verticalLayout = new QVBoxLayout(RGA_Device_Page);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(2, 0, 2, 2);
        RGA_Menu = new QFrame(RGA_Device_Page);
        RGA_Menu->setObjectName("RGA_Menu");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(RGA_Menu->sizePolicy().hasHeightForWidth());
        RGA_Menu->setSizePolicy(sizePolicy1);
        RGA_Menu->setMinimumSize(QSize(335, 50));
        RGA_Menu->setMaximumSize(QSize(335, 50));
        RGA_Menu->setStyleSheet(QString::fromUtf8("#RGA_Menu{\n"
"\n"
"   border-left: 0; /* \345\267\246\350\276\271\350\276\271\346\241\206\345\256\275\345\272\246\344\270\2720 */\n"
"    border-right: 0; /* \345\217\263\350\276\271\350\276\271\346\241\206\345\256\275\345\272\246\344\270\2720 */\n"
"    border-top: 0; /* \351\241\266\351\203\250\350\276\271\346\241\206\345\256\275\345\272\246\344\270\2720 */\n"
"    border-bottom: 2px solid rgb(255, 255, 255); /* \345\272\225\351\203\250\350\276\271\346\241\206\345\256\275\345\272\246\344\270\2721\345\203\217\347\264\240\357\274\214\351\242\234\350\211\262\344\270\272\351\273\221\350\211\262 */\n"
"}\n"
"\n"
"QPushButton {\n"
"    /* \346\255\243\345\270\270\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"	font: 700 18pt \"Microsoft YaHei UI\";\n"
" color:rgb(255, 255, 255);\n"
" background-color: #00BFFF;\n"
"    border-radius: 5px; \n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    /* \351\274\240\346\240\207\346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\346"
                        "\240\267\345\274\217 */\n"
"    background-color:#87CEFA; /* \346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"}"));
        RGA_Menu->setFrameShape(QFrame::StyledPanel);
        RGA_Menu->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(RGA_Menu);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(4, 4, 4, 2);
        horizontalSpacer_6 = new QSpacerItem(2, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_6);

        Hide_RGA_PushButton = new QPushButton(RGA_Menu);
        Hide_RGA_PushButton->setObjectName("Hide_RGA_PushButton");
        sizePolicy1.setHeightForWidth(Hide_RGA_PushButton->sizePolicy().hasHeightForWidth());
        Hide_RGA_PushButton->setSizePolicy(sizePolicy1);
        Hide_RGA_PushButton->setMinimumSize(QSize(35, 35));
        Hide_RGA_PushButton->setMaximumSize(QSize(35, 35));
        QFont font;
        font.setFamilies({QString::fromUtf8("Microsoft YaHei UI")});
        font.setPointSize(18);
        font.setBold(true);
        font.setItalic(false);
        Hide_RGA_PushButton->setFont(font);
        Hide_RGA_PushButton->setStyleSheet(QString::fromUtf8(""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/icon/hide.png"), QSize(), QIcon::Normal, QIcon::Off);
        Hide_RGA_PushButton->setIcon(icon);
        Hide_RGA_PushButton->setIconSize(QSize(20, 20));

        horizontalLayout_2->addWidget(Hide_RGA_PushButton);

        horizontalSpacer = new QSpacerItem(15, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        label = new QLabel(RGA_Menu);
        label->setObjectName("label");
        QFont font1;
        font1.setPointSize(18);
        font1.setBold(true);
        label->setFont(font1);

        horizontalLayout_2->addWidget(label);

        horizontalSpacer_5 = new QSpacerItem(10, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        Remove_RGA_PushButton = new QPushButton(RGA_Menu);
        Remove_RGA_PushButton->setObjectName("Remove_RGA_PushButton");
        sizePolicy1.setHeightForWidth(Remove_RGA_PushButton->sizePolicy().hasHeightForWidth());
        Remove_RGA_PushButton->setSizePolicy(sizePolicy1);
        Remove_RGA_PushButton->setMinimumSize(QSize(35, 35));
        Remove_RGA_PushButton->setMaximumSize(QSize(35, 35));
        Remove_RGA_PushButton->setFont(font);

        horizontalLayout_2->addWidget(Remove_RGA_PushButton);

        horizontalSpacer_4 = new QSpacerItem(10, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        Add_RGA_PushButton = new QPushButton(RGA_Menu);
        Add_RGA_PushButton->setObjectName("Add_RGA_PushButton");
        sizePolicy1.setHeightForWidth(Add_RGA_PushButton->sizePolicy().hasHeightForWidth());
        Add_RGA_PushButton->setSizePolicy(sizePolicy1);
        Add_RGA_PushButton->setMinimumSize(QSize(35, 35));
        Add_RGA_PushButton->setMaximumSize(QSize(35, 35));
        Add_RGA_PushButton->setFont(font);

        horizontalLayout_2->addWidget(Add_RGA_PushButton);

        horizontalSpacer_3 = new QSpacerItem(5, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addWidget(RGA_Menu);

        RGA_listWidget = new QListWidget(RGA_Device_Page);
        RGA_listWidget->setObjectName("RGA_listWidget");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(RGA_listWidget->sizePolicy().hasHeightForWidth());
        RGA_listWidget->setSizePolicy(sizePolicy2);
        RGA_listWidget->setMinimumSize(QSize(335, 0));
        RGA_listWidget->setMaximumSize(QSize(335, 16777215));
        QFont font2;
        font2.setBold(false);
        RGA_listWidget->setFont(font2);
        RGA_listWidget->setFocusPolicy(Qt::ClickFocus);
        RGA_listWidget->setStyleSheet(QString::fromUtf8("QListWidget\n"
"{\n"
"	background-color: #012047;\n"
" border:none;\n"
"    outline: none;\n"
"}\n"
"\n"
"\n"
"QListWidget::item\n"
"{\n"
"   background-color: #005F87;\n"
"   width:320px;\n"
"   height:65px;\n"
"   border-radius: 10px; \n"
"    border:none; /* \350\276\271\346\241\206\351\242\234\350\211\262 */\n"
"   margin-top: 5px;\n"
"   margin-bottom: 2px;\n"
"   margin-left: 5px;\n"
"   margin-right: 5px;\n"
"}\n"
"\n"
"\n"
"QListWidget::item:hover\n"
"{\n"
"    background-color:#87CEFA; /* \346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"}\n"
"\n"
"QListWidget::item:selected\n"
"{\n"
"   outline: 0px;\n"
"  background-color: rgb(26, 200, 255); /* \346\233\264\344\272\256\347\232\204\350\223\235\350\211\262 */\n"
"}\n"
"\n"
"QListWidget::item:selected:!active\n"
"{\n"
"  background-color: rgb(26, 200, 255);; /* \346\233\264\344\272\256\347\232\204\350\223\235\350\211\262 */\n"
"}\n"
"\n"
""));

        verticalLayout->addWidget(RGA_listWidget);


        horizontalLayout->addWidget(RGA_Device_Page);

        Display_Page = new QWidget(home);
        Display_Page->setObjectName("Display_Page");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(Display_Page->sizePolicy().hasHeightForWidth());
        Display_Page->setSizePolicy(sizePolicy3);
        Display_Page->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_2 = new QVBoxLayout(Display_Page);
        verticalLayout_2->setSpacing(3);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        Menu_Widget = new QWidget(Display_Page);
        Menu_Widget->setObjectName("Menu_Widget");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(Menu_Widget->sizePolicy().hasHeightForWidth());
        Menu_Widget->setSizePolicy(sizePolicy4);
        Menu_Widget->setMinimumSize(QSize(0, 50));
        Menu_Widget->setMaximumSize(QSize(16777215, 50));
        Menu_Widget->setStyleSheet(QString::fromUtf8("#Menu_Widget{\n"
"	background-color: rgb(1, 32, 79);\n"
"    border-radius: 5px; \n"
"    border:2px solid rgb(255, 255, 255);\n"
"}\n"
"\n"
"QPushButton {\n"
"    /* \346\255\243\345\270\270\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"	font: 700 18pt \"Microsoft YaHei UI\";\n"
"   color:rgb(255, 255, 255);\n"
"   background-color: #005F87;\n"
"    border-radius: 5px; \n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    /* \351\274\240\346\240\207\346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"    background-color:#87CEFA; /* \346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"}\n"
"\n"
"QPushButton:pressed,QPushButton::checked {\n"
"    background-color: #00BFFF; /* \351\200\211\344\270\255\345\220\216\347\232\204\351\242\234\350\211\262 */\n"
"    color: white; /* \351\200\211\344\270\255\345\220\216\347\232\204\345\255\227\344\275\223\351\242\234\350\211"
                        "\262 */\n"
"}"));
        horizontalLayout_3 = new QHBoxLayout(Menu_Widget);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        Show_RGA_PushButton = new QPushButton(Menu_Widget);
        Show_RGA_PushButton->setObjectName("Show_RGA_PushButton");
        sizePolicy1.setHeightForWidth(Show_RGA_PushButton->sizePolicy().hasHeightForWidth());
        Show_RGA_PushButton->setSizePolicy(sizePolicy1);
        Show_RGA_PushButton->setMinimumSize(QSize(35, 35));
        Show_RGA_PushButton->setMaximumSize(QSize(35, 35));
        Show_RGA_PushButton->setFont(font);
        Show_RGA_PushButton->setStyleSheet(QString::fromUtf8(""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/new/prefix1/icon/show.png"), QSize(), QIcon::Normal, QIcon::Off);
        Show_RGA_PushButton->setIcon(icon1);
        Show_RGA_PushButton->setIconSize(QSize(20, 20));

        horizontalLayout_3->addWidget(Show_RGA_PushButton);

        Sweep_Mode_PushButton = new QPushButton(Menu_Widget);
        Sweep_Mode_PushButton->setObjectName("Sweep_Mode_PushButton");
        sizePolicy1.setHeightForWidth(Sweep_Mode_PushButton->sizePolicy().hasHeightForWidth());
        Sweep_Mode_PushButton->setSizePolicy(sizePolicy1);
        Sweep_Mode_PushButton->setMinimumSize(QSize(120, 35));
        Sweep_Mode_PushButton->setMaximumSize(QSize(120, 35));
        Sweep_Mode_PushButton->setFont(font);
        Sweep_Mode_PushButton->setStyleSheet(QString::fromUtf8(""));
        Sweep_Mode_PushButton->setCheckable(true);
        Sweep_Mode_PushButton->setChecked(true);
        Sweep_Mode_PushButton->setAutoExclusive(true);

        horizontalLayout_3->addWidget(Sweep_Mode_PushButton);

        Leak_Mode_PushButton = new QPushButton(Menu_Widget);
        Leak_Mode_PushButton->setObjectName("Leak_Mode_PushButton");
        sizePolicy1.setHeightForWidth(Leak_Mode_PushButton->sizePolicy().hasHeightForWidth());
        Leak_Mode_PushButton->setSizePolicy(sizePolicy1);
        Leak_Mode_PushButton->setMinimumSize(QSize(120, 35));
        Leak_Mode_PushButton->setMaximumSize(QSize(120, 35));
        Leak_Mode_PushButton->setFont(font);
        Leak_Mode_PushButton->setStyleSheet(QString::fromUtf8(""));
        Leak_Mode_PushButton->setIconSize(QSize(20, 20));
        Leak_Mode_PushButton->setCheckable(true);
        Leak_Mode_PushButton->setChecked(false);
        Leak_Mode_PushButton->setAutoExclusive(true);

        horizontalLayout_3->addWidget(Leak_Mode_PushButton);

        Show_RGA_PushButton_4 = new QPushButton(Menu_Widget);
        Show_RGA_PushButton_4->setObjectName("Show_RGA_PushButton_4");
        sizePolicy1.setHeightForWidth(Show_RGA_PushButton_4->sizePolicy().hasHeightForWidth());
        Show_RGA_PushButton_4->setSizePolicy(sizePolicy1);
        Show_RGA_PushButton_4->setMinimumSize(QSize(120, 35));
        Show_RGA_PushButton_4->setMaximumSize(QSize(120, 35));
        Show_RGA_PushButton_4->setFont(font);
        Show_RGA_PushButton_4->setStyleSheet(QString::fromUtf8(""));
        Show_RGA_PushButton_4->setIconSize(QSize(20, 20));
        Show_RGA_PushButton_4->setCheckable(true);
        Show_RGA_PushButton_4->setChecked(false);
        Show_RGA_PushButton_4->setAutoExclusive(true);

        horizontalLayout_3->addWidget(Show_RGA_PushButton_4);

        Show_RGA_PushButton_5 = new QPushButton(Menu_Widget);
        Show_RGA_PushButton_5->setObjectName("Show_RGA_PushButton_5");
        sizePolicy1.setHeightForWidth(Show_RGA_PushButton_5->sizePolicy().hasHeightForWidth());
        Show_RGA_PushButton_5->setSizePolicy(sizePolicy1);
        Show_RGA_PushButton_5->setMinimumSize(QSize(120, 35));
        Show_RGA_PushButton_5->setMaximumSize(QSize(120, 35));
        Show_RGA_PushButton_5->setFont(font);
        Show_RGA_PushButton_5->setStyleSheet(QString::fromUtf8(""));
        Show_RGA_PushButton_5->setIconSize(QSize(20, 20));
        Show_RGA_PushButton_5->setCheckable(true);
        Show_RGA_PushButton_5->setChecked(false);
        Show_RGA_PushButton_5->setAutoExclusive(true);

        horizontalLayout_3->addWidget(Show_RGA_PushButton_5);

        Show_RGA_PushButton_6 = new QPushButton(Menu_Widget);
        Show_RGA_PushButton_6->setObjectName("Show_RGA_PushButton_6");
        sizePolicy1.setHeightForWidth(Show_RGA_PushButton_6->sizePolicy().hasHeightForWidth());
        Show_RGA_PushButton_6->setSizePolicy(sizePolicy1);
        Show_RGA_PushButton_6->setMinimumSize(QSize(120, 35));
        Show_RGA_PushButton_6->setMaximumSize(QSize(120, 35));
        Show_RGA_PushButton_6->setFont(font);
        Show_RGA_PushButton_6->setStyleSheet(QString::fromUtf8(""));
        Show_RGA_PushButton_6->setIconSize(QSize(20, 20));
        Show_RGA_PushButton_6->setCheckable(true);
        Show_RGA_PushButton_6->setChecked(false);
        Show_RGA_PushButton_6->setAutoExclusive(true);

        horizontalLayout_3->addWidget(Show_RGA_PushButton_6);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout_2->addWidget(Menu_Widget);

        Display_stackedWidget = new QStackedWidget(Display_Page);
        Display_stackedWidget->setObjectName("Display_stackedWidget");
        Display_stackedWidget->setStyleSheet(QString::fromUtf8("#Display_stackedWidget{\n"
"\n"
"    border-radius: 5px; \n"
"}\n"
""));

        verticalLayout_2->addWidget(Display_stackedWidget);


        horizontalLayout->addWidget(Display_Page);


        retranslateUi(home);

        QMetaObject::connectSlotsByName(home);
    } // setupUi

    void retranslateUi(QWidget *home)
    {
        home->setWindowTitle(QCoreApplication::translate("home", "Form", nullptr));
        Hide_RGA_PushButton->setText(QString());
        label->setText(QCoreApplication::translate("home", "\346\256\213\344\275\231\346\260\224\344\275\223\345\210\206\346\236\220\344\273\252", nullptr));
        Remove_RGA_PushButton->setText(QCoreApplication::translate("home", "-", nullptr));
        Add_RGA_PushButton->setText(QCoreApplication::translate("home", "+", nullptr));
        Show_RGA_PushButton->setText(QString());
        Sweep_Mode_PushButton->setText(QCoreApplication::translate("home", "\346\211\253\346\217\217\346\250\241\345\274\217", nullptr));
        Leak_Mode_PushButton->setText(QCoreApplication::translate("home", "\346\243\200\346\274\217\346\250\241\345\274\217", nullptr));
        Show_RGA_PushButton_4->setText(QCoreApplication::translate("home", "\345\217\202\346\225\260\350\256\276\347\275\256", nullptr));
        Show_RGA_PushButton_5->setText(QCoreApplication::translate("home", "\350\256\276\345\244\207\344\277\241\346\201\257", nullptr));
        Show_RGA_PushButton_6->setText(QCoreApplication::translate("home", "\345\216\206\345\217\262\346\225\260\346\215\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class home: public Ui_home {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOME_H
