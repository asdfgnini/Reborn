/********************************************************************************
** Form generated from reading UI file 'chart.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHART_H
#define UI_CHART_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Chart
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *chart_horizontalLayout;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *X_Chart_Flash_PushButton;
    QPushButton *Y_Chart_Flash_PushButton;
    QSpacerItem *verticalSpacer_2;
    QWidget *RGA_Device_Page;
    QVBoxLayout *verticalLayout_3;
    QFrame *AMU_List_Menu;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *Hide_SingalList_PushButton;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QPushButton *Remove_Signal_pushButton;
    QPushButton *Add_Signal_pushButton;
    QListWidget *AMU_listWidget;

    void setupUi(QWidget *Chart)
    {
        if (Chart->objectName().isEmpty())
            Chart->setObjectName("Chart");
        Chart->resize(645, 390);
        horizontalLayout = new QHBoxLayout(Chart);
        horizontalLayout->setObjectName("horizontalLayout");
        widget = new QWidget(Chart);
        widget->setObjectName("widget");
        horizontalLayout_3 = new QHBoxLayout(widget);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        chart_horizontalLayout = new QHBoxLayout();
        chart_horizontalLayout->setObjectName("chart_horizontalLayout");

        horizontalLayout_3->addLayout(chart_horizontalLayout);


        horizontalLayout->addWidget(widget);

        widget_2 = new QWidget(Chart);
        widget_2->setObjectName("widget_2");
        widget_2->setMinimumSize(QSize(50, 0));
        widget_2->setMaximumSize(QSize(50, 16777215));
        verticalLayout = new QVBoxLayout(widget_2);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer = new QSpacerItem(20, 136, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        X_Chart_Flash_PushButton = new QPushButton(widget_2);
        X_Chart_Flash_PushButton->setObjectName("X_Chart_Flash_PushButton");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(X_Chart_Flash_PushButton->sizePolicy().hasHeightForWidth());
        X_Chart_Flash_PushButton->setSizePolicy(sizePolicy);
        X_Chart_Flash_PushButton->setMinimumSize(QSize(50, 50));
        X_Chart_Flash_PushButton->setMaximumSize(QSize(50, 50));
        X_Chart_Flash_PushButton->setLayoutDirection(Qt::LeftToRight);
        X_Chart_Flash_PushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    /* \346\255\243\345\270\270\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"    background-color: rgb(23, 56, 101);/* \346\255\243\345\270\270\347\212\266\346\200\201\344\270\213\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"    border-radius: 5px; \n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    /* \351\274\240\346\240\207\346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"    background-color:rgb(32, 77, 136); /* \346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"}"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/icon/Chart_No_Flash.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/new/prefix1/icon/Chart_Flash.png"), QSize(), QIcon::Normal, QIcon::On);
        X_Chart_Flash_PushButton->setIcon(icon);
        X_Chart_Flash_PushButton->setIconSize(QSize(40, 40));
        X_Chart_Flash_PushButton->setCheckable(true);

        verticalLayout->addWidget(X_Chart_Flash_PushButton);

        Y_Chart_Flash_PushButton = new QPushButton(widget_2);
        Y_Chart_Flash_PushButton->setObjectName("Y_Chart_Flash_PushButton");
        sizePolicy.setHeightForWidth(Y_Chart_Flash_PushButton->sizePolicy().hasHeightForWidth());
        Y_Chart_Flash_PushButton->setSizePolicy(sizePolicy);
        Y_Chart_Flash_PushButton->setMinimumSize(QSize(50, 50));
        Y_Chart_Flash_PushButton->setMaximumSize(QSize(50, 50));
        Y_Chart_Flash_PushButton->setLayoutDirection(Qt::LeftToRight);
        Y_Chart_Flash_PushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    /* \346\255\243\345\270\270\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"    background-color: rgb(23, 56, 101);/* \346\255\243\345\270\270\347\212\266\346\200\201\344\270\213\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"    border-radius: 5px; \n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    /* \351\274\240\346\240\207\346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"    background-color:rgb(32, 77, 136); /* \346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/new/prefix1/icon/y_Chart_Flash.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon1.addFile(QString::fromUtf8(":/new/prefix1/icon/y_No_Chart_Flash.png"), QSize(), QIcon::Normal, QIcon::On);
        Y_Chart_Flash_PushButton->setIcon(icon1);
        Y_Chart_Flash_PushButton->setIconSize(QSize(40, 40));
        Y_Chart_Flash_PushButton->setCheckable(true);

        verticalLayout->addWidget(Y_Chart_Flash_PushButton);

        verticalSpacer_2 = new QSpacerItem(20, 136, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        horizontalLayout->addWidget(widget_2);

        RGA_Device_Page = new QWidget(Chart);
        RGA_Device_Page->setObjectName("RGA_Device_Page");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(RGA_Device_Page->sizePolicy().hasHeightForWidth());
        RGA_Device_Page->setSizePolicy(sizePolicy1);
        RGA_Device_Page->setMinimumSize(QSize(200, 0));
        RGA_Device_Page->setMaximumSize(QSize(200, 16777215));
        RGA_Device_Page->setStyleSheet(QString::fromUtf8("#RGA_Device_Page{\n"
"	background-color: rgb(1, 32, 79);\n"
"    border-radius: 5px; \n"
"    border:1px solid rgb(255, 255, 255);\n"
"}"));
        verticalLayout_3 = new QVBoxLayout(RGA_Device_Page);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(2, 2, 2, 2);
        AMU_List_Menu = new QFrame(RGA_Device_Page);
        AMU_List_Menu->setObjectName("AMU_List_Menu");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(AMU_List_Menu->sizePolicy().hasHeightForWidth());
        AMU_List_Menu->setSizePolicy(sizePolicy2);
        AMU_List_Menu->setStyleSheet(QString::fromUtf8("#AMU_List_Menu{\n"
"	background-color: #012047;\n"
"   border-left: 1; /* \345\267\246\350\276\271\350\276\271\346\241\206\345\256\275\345\272\246\344\270\2720 */\n"
"    border-right: 1; /* \345\217\263\350\276\271\350\276\271\346\241\206\345\256\275\345\272\246\344\270\2720 */\n"
"    border-top: 0; /* \351\241\266\351\203\250\350\276\271\346\241\206\345\256\275\345\272\246\344\270\2720 */\n"
"    border-bottom: 1px solid rgb(255, 255, 255); /* \345\272\225\351\203\250\350\276\271\346\241\206\345\256\275\345\272\246\344\270\2721\345\203\217\347\264\240\357\274\214\351\242\234\350\211\262\344\270\272\351\273\221\350\211\262 */\n"
"    border-radius: 0px; \n"
"}\n"
"\n"
"QPushButton {\n"
"    /* \346\255\243\345\270\270\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"	font: 700 14pt \"Microsoft YaHei UI\";\n"
" color:rgb(255, 255, 255);\n"
" background-color: #00BFFF;\n"
"    border-radius: 5px; \n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    /* \351\274\240\346\240\207\346\202\254\345"
                        "\201\234\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"    background-color:#87CEFA; /* \346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"}\n"
"\n"
"\n"
""));
        AMU_List_Menu->setFrameShape(QFrame::StyledPanel);
        AMU_List_Menu->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(AMU_List_Menu);
        horizontalLayout_2->setSpacing(2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(4, 4, 4, 2);
        horizontalSpacer_6 = new QSpacerItem(2, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_6);

        Hide_SingalList_PushButton = new QPushButton(AMU_List_Menu);
        Hide_SingalList_PushButton->setObjectName("Hide_SingalList_PushButton");
        sizePolicy.setHeightForWidth(Hide_SingalList_PushButton->sizePolicy().hasHeightForWidth());
        Hide_SingalList_PushButton->setSizePolicy(sizePolicy);
        Hide_SingalList_PushButton->setMinimumSize(QSize(20, 20));
        Hide_SingalList_PushButton->setMaximumSize(QSize(20, 20));
        QFont font;
        font.setFamilies({QString::fromUtf8("Microsoft YaHei UI")});
        font.setPointSize(14);
        font.setBold(true);
        font.setItalic(false);
        Hide_SingalList_PushButton->setFont(font);
        Hide_SingalList_PushButton->setStyleSheet(QString::fromUtf8(""));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/new/prefix1/icon/show.png"), QSize(), QIcon::Normal, QIcon::Off);
        Hide_SingalList_PushButton->setIcon(icon2);
        Hide_SingalList_PushButton->setIconSize(QSize(15, 15));

        horizontalLayout_2->addWidget(Hide_SingalList_PushButton);

        horizontalSpacer = new QSpacerItem(15, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        label = new QLabel(AMU_List_Menu);
        label->setObjectName("label");
        QFont font1;
        font1.setPointSize(14);
        font1.setBold(true);
        label->setFont(font1);
        label->setStyleSheet(QString::fromUtf8("	background-color: #012047;\n"
"color: rgb(255, 255, 255);"));

        horizontalLayout_2->addWidget(label);

        Remove_Signal_pushButton = new QPushButton(AMU_List_Menu);
        Remove_Signal_pushButton->setObjectName("Remove_Signal_pushButton");
        sizePolicy.setHeightForWidth(Remove_Signal_pushButton->sizePolicy().hasHeightForWidth());
        Remove_Signal_pushButton->setSizePolicy(sizePolicy);
        Remove_Signal_pushButton->setMinimumSize(QSize(20, 20));
        Remove_Signal_pushButton->setMaximumSize(QSize(20, 20));

        horizontalLayout_2->addWidget(Remove_Signal_pushButton);

        Add_Signal_pushButton = new QPushButton(AMU_List_Menu);
        Add_Signal_pushButton->setObjectName("Add_Signal_pushButton");
        sizePolicy.setHeightForWidth(Add_Signal_pushButton->sizePolicy().hasHeightForWidth());
        Add_Signal_pushButton->setSizePolicy(sizePolicy);
        Add_Signal_pushButton->setMinimumSize(QSize(20, 20));
        Add_Signal_pushButton->setMaximumSize(QSize(20, 20));
        Add_Signal_pushButton->setFont(font);

        horizontalLayout_2->addWidget(Add_Signal_pushButton);


        verticalLayout_3->addWidget(AMU_List_Menu);

        AMU_listWidget = new QListWidget(RGA_Device_Page);
        AMU_listWidget->setObjectName("AMU_listWidget");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(AMU_listWidget->sizePolicy().hasHeightForWidth());
        AMU_listWidget->setSizePolicy(sizePolicy3);
        QFont font2;
        font2.setBold(false);
        AMU_listWidget->setFont(font2);
        AMU_listWidget->setFocusPolicy(Qt::ClickFocus);
        AMU_listWidget->setStyleSheet(QString::fromUtf8("QListWidget\n"
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
"   width:173px;\n"
"   height:35px;\n"
"   border-radius: 5px; \n"
"    border:none; /* \350\276\271\346\241\206\351\242\234\350\211\262 */\n"
"   margin-top: 2px;\n"
"   margin-bottom: 2px;\n"
"   margin-left: 2px;\n"
"   margin-right: 2px;\n"
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
""));

        verticalLayout_3->addWidget(AMU_listWidget);


        horizontalLayout->addWidget(RGA_Device_Page);


        retranslateUi(Chart);

        QMetaObject::connectSlotsByName(Chart);
    } // setupUi

    void retranslateUi(QWidget *Chart)
    {
        Chart->setWindowTitle(QCoreApplication::translate("Chart", "Form", nullptr));
        X_Chart_Flash_PushButton->setText(QString());
        Y_Chart_Flash_PushButton->setText(QString());
        Hide_SingalList_PushButton->setText(QString());
        label->setText(QCoreApplication::translate("Chart", "\344\277\241\345\217\267\346\233\262\347\272\277", nullptr));
        Remove_Signal_pushButton->setText(QCoreApplication::translate("Chart", "-", nullptr));
        Add_Signal_pushButton->setText(QCoreApplication::translate("Chart", "+", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Chart: public Ui_Chart {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHART_H
