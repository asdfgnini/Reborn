/********************************************************************************
** Form generated from reading UI file 'singal_chart_item.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SINGAL_CHART_ITEM_H
#define UI_SINGAL_CHART_ITEM_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Singal_Chart_Item
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QFrame *frame_3;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_4;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *Chart_Name;
    QSpacerItem *horizontalSpacer_4;
    QLabel *Ion_Value;
    QSpacerItem *verticalSpacer_3;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *Color_pushButton;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *Chart_Visible_pushButton;

    void setupUi(QWidget *Singal_Chart_Item)
    {
        if (Singal_Chart_Item->objectName().isEmpty())
            Singal_Chart_Item->setObjectName("Singal_Chart_Item");
        Singal_Chart_Item->resize(115, 35);
        Singal_Chart_Item->setMaximumSize(QSize(16777215, 35));
        Singal_Chart_Item->setStyleSheet(QString::fromUtf8("QWidget{\n"
"\n"
"	background-color: transparent;\n"
"\n"
"}"));
        horizontalLayout = new QHBoxLayout(Singal_Chart_Item);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(Singal_Chart_Item);
        frame->setObjectName("frame");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setMinimumSize(QSize(0, 0));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame);
        horizontalLayout_2->setSpacing(1);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_2 = new QSpacerItem(1, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        frame_3 = new QFrame(frame);
        frame_3->setObjectName("frame_3");
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_3);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_4 = new QSpacerItem(20, 3, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_4);

        frame_2 = new QFrame(frame_3);
        frame_2->setObjectName("frame_2");
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame_2);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        Chart_Name = new QLabel(frame_2);
        Chart_Name->setObjectName("Chart_Name");
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        Chart_Name->setFont(font);

        horizontalLayout_3->addWidget(Chart_Name);

        horizontalSpacer_4 = new QSpacerItem(40, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        verticalLayout_2->addWidget(frame_2);

        Ion_Value = new QLabel(frame_3);
        Ion_Value->setObjectName("Ion_Value");
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(false);
        Ion_Value->setFont(font1);

        verticalLayout_2->addWidget(Ion_Value);

        verticalSpacer_3 = new QSpacerItem(20, 3, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);


        horizontalLayout_2->addWidget(frame_3);

        widget = new QWidget(frame);
        widget->setObjectName("widget");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy1);
        widget->setMinimumSize(QSize(30, 30));
        widget->setMaximumSize(QSize(30, 30));
        widget->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    /* \346\255\243\345\270\270\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"    border-radius: 2px; \n"
"    border:2px solid rgb(255, 255, 255); /* \350\276\271\346\241\206\351\242\234\350\211\262 */\n"
"}\n"
""));
        horizontalLayout_4 = new QHBoxLayout(widget);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        Color_pushButton = new QPushButton(widget);
        Color_pushButton->setObjectName("Color_pushButton");
        Color_pushButton->setMinimumSize(QSize(25, 25));
        Color_pushButton->setMaximumSize(QSize(25, 25));
        Color_pushButton->setStyleSheet(QString::fromUtf8(""));
        Color_pushButton->setIconSize(QSize(25, 25));
        Color_pushButton->setAutoRepeat(true);

        horizontalLayout_4->addWidget(Color_pushButton);


        horizontalLayout_2->addWidget(widget);

        horizontalSpacer_3 = new QSpacerItem(2, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        Chart_Visible_pushButton = new QPushButton(frame);
        Chart_Visible_pushButton->setObjectName("Chart_Visible_pushButton");
        Chart_Visible_pushButton->setMinimumSize(QSize(32, 32));
        Chart_Visible_pushButton->setMaximumSize(QSize(32, 32));
        Chart_Visible_pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    /* \346\255\243\345\270\270\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"    border-radius: 5px; \n"
"}\n"
""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/icon/Chart_False.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/new/prefix1/icon/Chart_true.png"), QSize(), QIcon::Normal, QIcon::On);
        Chart_Visible_pushButton->setIcon(icon);
        Chart_Visible_pushButton->setIconSize(QSize(30, 30));
        Chart_Visible_pushButton->setCheckable(true);
        Chart_Visible_pushButton->setChecked(true);

        horizontalLayout_2->addWidget(Chart_Visible_pushButton);


        horizontalLayout->addWidget(frame);


        retranslateUi(Singal_Chart_Item);

        QMetaObject::connectSlotsByName(Singal_Chart_Item);
    } // setupUi

    void retranslateUi(QWidget *Singal_Chart_Item)
    {
        Singal_Chart_Item->setWindowTitle(QCoreApplication::translate("Singal_Chart_Item", "Form", nullptr));
        Chart_Name->setText(QCoreApplication::translate("Singal_Chart_Item", "\344\277\241\345\217\267\345\220\215\347\247\260", nullptr));
        Ion_Value->setText(QCoreApplication::translate("Singal_Chart_Item", "\345\200\274", nullptr));
        Color_pushButton->setText(QString());
        Chart_Visible_pushButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Singal_Chart_Item: public Ui_Singal_Chart_Item {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SINGAL_CHART_ITEM_H
