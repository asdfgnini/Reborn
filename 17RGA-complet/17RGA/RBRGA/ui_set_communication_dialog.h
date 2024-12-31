/********************************************************************************
** Form generated from reading UI file 'set_communication_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SET_COMMUNICATION_DIALOG_H
#define UI_SET_COMMUNICATION_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Set_communication_Dialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_14;
    QFrame *line_2;
    QWidget *widget_11;
    QVBoxLayout *verticalLayout_5;
    QWidget *widget_17;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_18;
    QSpinBox *IP1_spinBox;
    QLabel *label_19;
    QSpinBox *IP2_spinBox;
    QLabel *label_29;
    QSpinBox *IP3_spinBox;
    QLabel *label_30;
    QSpinBox *IP4_spinBox;
    QWidget *widget_29;
    QHBoxLayout *horizontalLayout_25;
    QLabel *label_31;
    QSpinBox *Com_number_spinBox;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *Cancel_Button;
    QPushButton *Ack_Button;

    void setupUi(QDialog *Set_communication_Dialog)
    {
        if (Set_communication_Dialog->objectName().isEmpty())
            Set_communication_Dialog->setObjectName("Set_communication_Dialog");
        Set_communication_Dialog->resize(301, 177);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Set_communication_Dialog->sizePolicy().hasHeightForWidth());
        Set_communication_Dialog->setSizePolicy(sizePolicy);
        Set_communication_Dialog->setMaximumSize(QSize(301, 177));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/icon/setting_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        Set_communication_Dialog->setWindowIcon(icon);
        Set_communication_Dialog->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	background-color: rgb(1, 32, 79);\n"
"    border-radius: 5px; \n"
"	color: rgb(255, 255, 255);\n"
"}"));
        Set_communication_Dialog->setModal(false);
        verticalLayout = new QVBoxLayout(Set_communication_Dialog);
        verticalLayout->setObjectName("verticalLayout");
        widget_2 = new QWidget(Set_communication_Dialog);
        widget_2->setObjectName("widget_2");
        widget_2->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"background-color: rgb(23, 56, 101);\n"
"font: 14pt \"Microsoft YaHei UI\";\n"
"}\n"
"QcomboBox\n"
"{\n"
"background-color: rgb(58, 143, 255);\n"
"}"));
        verticalLayout_3 = new QVBoxLayout(widget_2);
        verticalLayout_3->setSpacing(4);
        verticalLayout_3->setObjectName("verticalLayout_3");
        label_14 = new QLabel(widget_2);
        label_14->setObjectName("label_14");
        label_14->setStyleSheet(QString::fromUtf8("font: 700 18pt \"Microsoft YaHei UI\";"));
        label_14->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_14);

        line_2 = new QFrame(widget_2);
        line_2->setObjectName("line_2");
        line_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        line_2->setFrameShadow(QFrame::Plain);
        line_2->setFrameShape(QFrame::HLine);

        verticalLayout_3->addWidget(line_2);

        widget_11 = new QWidget(widget_2);
        widget_11->setObjectName("widget_11");
        verticalLayout_5 = new QVBoxLayout(widget_11);
        verticalLayout_5->setSpacing(15);
        verticalLayout_5->setObjectName("verticalLayout_5");
        verticalLayout_5->setContentsMargins(0, 5, 0, 5);
        widget_17 = new QWidget(widget_11);
        widget_17->setObjectName("widget_17");
        widget_17->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_14 = new QHBoxLayout(widget_17);
        horizontalLayout_14->setSpacing(0);
        horizontalLayout_14->setObjectName("horizontalLayout_14");
        horizontalLayout_14->setContentsMargins(0, 0, 0, 0);
        label_18 = new QLabel(widget_17);
        label_18->setObjectName("label_18");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(100);
        sizePolicy1.setVerticalStretch(30);
        sizePolicy1.setHeightForWidth(label_18->sizePolicy().hasHeightForWidth());
        label_18->setSizePolicy(sizePolicy1);
        label_18->setMinimumSize(QSize(100, 30));
        label_18->setMaximumSize(QSize(100, 30));
        QFont font;
        font.setFamilies({QString::fromUtf8("Microsoft YaHei UI")});
        font.setPointSize(14);
        font.setBold(false);
        font.setItalic(false);
        label_18->setFont(font);
        label_18->setStyleSheet(QString::fromUtf8(""));
        label_18->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_14->addWidget(label_18);

        IP1_spinBox = new QSpinBox(widget_17);
        IP1_spinBox->setObjectName("IP1_spinBox");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(IP1_spinBox->sizePolicy().hasHeightForWidth());
        IP1_spinBox->setSizePolicy(sizePolicy2);
        IP1_spinBox->setMinimumSize(QSize(0, 30));
        IP1_spinBox->setFocusPolicy(Qt::ClickFocus);
        IP1_spinBox->setStyleSheet(QString::fromUtf8("background-color: rgb(58, 143, 255);\n"
"  border:1px solid rgb(255, 255, 255);"));
        IP1_spinBox->setAlignment(Qt::AlignCenter);
        IP1_spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        IP1_spinBox->setMaximum(255);
        IP1_spinBox->setValue(192);
        IP1_spinBox->setDisplayIntegerBase(10);

        horizontalLayout_14->addWidget(IP1_spinBox);

        label_19 = new QLabel(widget_17);
        label_19->setObjectName("label_19");
        sizePolicy.setHeightForWidth(label_19->sizePolicy().hasHeightForWidth());
        label_19->setSizePolicy(sizePolicy);
        label_19->setMinimumSize(QSize(15, 30));
        label_19->setMaximumSize(QSize(15, 30));
        label_19->setAlignment(Qt::AlignCenter);

        horizontalLayout_14->addWidget(label_19);

        IP2_spinBox = new QSpinBox(widget_17);
        IP2_spinBox->setObjectName("IP2_spinBox");
        sizePolicy2.setHeightForWidth(IP2_spinBox->sizePolicy().hasHeightForWidth());
        IP2_spinBox->setSizePolicy(sizePolicy2);
        IP2_spinBox->setMinimumSize(QSize(0, 30));
        IP2_spinBox->setFocusPolicy(Qt::ClickFocus);
        IP2_spinBox->setStyleSheet(QString::fromUtf8("background-color: rgb(58, 143, 255);\n"
"  border:1px solid rgb(255, 255, 255);"));
        IP2_spinBox->setAlignment(Qt::AlignCenter);
        IP2_spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        IP2_spinBox->setMaximum(255);
        IP2_spinBox->setValue(168);
        IP2_spinBox->setDisplayIntegerBase(10);

        horizontalLayout_14->addWidget(IP2_spinBox);

        label_29 = new QLabel(widget_17);
        label_29->setObjectName("label_29");
        sizePolicy.setHeightForWidth(label_29->sizePolicy().hasHeightForWidth());
        label_29->setSizePolicy(sizePolicy);
        label_29->setMinimumSize(QSize(15, 30));
        label_29->setMaximumSize(QSize(15, 30));
        label_29->setAlignment(Qt::AlignCenter);

        horizontalLayout_14->addWidget(label_29);

        IP3_spinBox = new QSpinBox(widget_17);
        IP3_spinBox->setObjectName("IP3_spinBox");
        sizePolicy2.setHeightForWidth(IP3_spinBox->sizePolicy().hasHeightForWidth());
        IP3_spinBox->setSizePolicy(sizePolicy2);
        IP3_spinBox->setMinimumSize(QSize(0, 30));
        IP3_spinBox->setFocusPolicy(Qt::ClickFocus);
        IP3_spinBox->setStyleSheet(QString::fromUtf8("background-color: rgb(58, 143, 255);\n"
"  border:1px solid rgb(255, 255, 255);"));
        IP3_spinBox->setAlignment(Qt::AlignCenter);
        IP3_spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        IP3_spinBox->setMaximum(255);
        IP3_spinBox->setValue(1);
        IP3_spinBox->setDisplayIntegerBase(10);

        horizontalLayout_14->addWidget(IP3_spinBox);

        label_30 = new QLabel(widget_17);
        label_30->setObjectName("label_30");
        sizePolicy.setHeightForWidth(label_30->sizePolicy().hasHeightForWidth());
        label_30->setSizePolicy(sizePolicy);
        label_30->setMinimumSize(QSize(15, 30));
        label_30->setMaximumSize(QSize(15, 30));
        label_30->setAlignment(Qt::AlignCenter);

        horizontalLayout_14->addWidget(label_30);

        IP4_spinBox = new QSpinBox(widget_17);
        IP4_spinBox->setObjectName("IP4_spinBox");
        sizePolicy2.setHeightForWidth(IP4_spinBox->sizePolicy().hasHeightForWidth());
        IP4_spinBox->setSizePolicy(sizePolicy2);
        IP4_spinBox->setMinimumSize(QSize(0, 30));
        IP4_spinBox->setFocusPolicy(Qt::ClickFocus);
        IP4_spinBox->setStyleSheet(QString::fromUtf8("background-color: rgb(58, 143, 255);\n"
"  border:1px solid rgb(255, 255, 255);"));
        IP4_spinBox->setAlignment(Qt::AlignCenter);
        IP4_spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        IP4_spinBox->setMaximum(255);
        IP4_spinBox->setValue(100);
        IP4_spinBox->setDisplayIntegerBase(10);

        horizontalLayout_14->addWidget(IP4_spinBox);


        verticalLayout_5->addWidget(widget_17);

        widget_29 = new QWidget(widget_11);
        widget_29->setObjectName("widget_29");
        widget_29->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_25 = new QHBoxLayout(widget_29);
        horizontalLayout_25->setSpacing(0);
        horizontalLayout_25->setObjectName("horizontalLayout_25");
        horizontalLayout_25->setContentsMargins(0, 0, 0, 0);
        label_31 = new QLabel(widget_29);
        label_31->setObjectName("label_31");
        sizePolicy1.setHeightForWidth(label_31->sizePolicy().hasHeightForWidth());
        label_31->setSizePolicy(sizePolicy1);
        label_31->setMinimumSize(QSize(100, 30));
        label_31->setMaximumSize(QSize(100, 30));
        label_31->setFont(font);
        label_31->setStyleSheet(QString::fromUtf8(""));
        label_31->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_25->addWidget(label_31);

        Com_number_spinBox = new QSpinBox(widget_29);
        Com_number_spinBox->setObjectName("Com_number_spinBox");
        sizePolicy2.setHeightForWidth(Com_number_spinBox->sizePolicy().hasHeightForWidth());
        Com_number_spinBox->setSizePolicy(sizePolicy2);
        Com_number_spinBox->setMinimumSize(QSize(0, 30));
        Com_number_spinBox->setFocusPolicy(Qt::ClickFocus);
        Com_number_spinBox->setStyleSheet(QString::fromUtf8("background-color: rgb(58, 143, 255);\n"
"  border:1px solid rgb(255, 255, 255);"));
        Com_number_spinBox->setAlignment(Qt::AlignCenter);
        Com_number_spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        Com_number_spinBox->setMaximum(65535);
        Com_number_spinBox->setValue(80);
        Com_number_spinBox->setDisplayIntegerBase(10);

        horizontalLayout_25->addWidget(Com_number_spinBox);


        verticalLayout_5->addWidget(widget_29);


        verticalLayout_3->addWidget(widget_11);


        verticalLayout->addWidget(widget_2);

        widget = new QWidget(Set_communication_Dialog);
        widget->setObjectName("widget");
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        Cancel_Button = new QPushButton(widget);
        Cancel_Button->setObjectName("Cancel_Button");
        sizePolicy.setHeightForWidth(Cancel_Button->sizePolicy().hasHeightForWidth());
        Cancel_Button->setSizePolicy(sizePolicy);
        Cancel_Button->setMinimumSize(QSize(70, 30));
        Cancel_Button->setMaximumSize(QSize(70, 30));
        Cancel_Button->setLayoutDirection(Qt::RightToLeft);
        Cancel_Button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    /* \346\255\243\345\270\270\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"	font: 700 16pt \"Microsoft YaHei UI\";\n"
" color:rgb(255, 255, 255);\n"
"    background-color: rgb(23, 56, 101);/* \346\255\243\345\270\270\347\212\266\346\200\201\344\270\213\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"    border-radius: 5px; \n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    /* \351\274\240\346\240\207\346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"    background-color:rgb(32, 77, 136); /* \346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"}"));
        Cancel_Button->setIconSize(QSize(40, 40));

        horizontalLayout->addWidget(Cancel_Button);

        Ack_Button = new QPushButton(widget);
        Ack_Button->setObjectName("Ack_Button");
        sizePolicy.setHeightForWidth(Ack_Button->sizePolicy().hasHeightForWidth());
        Ack_Button->setSizePolicy(sizePolicy);
        Ack_Button->setMinimumSize(QSize(70, 30));
        Ack_Button->setMaximumSize(QSize(70, 30));
        Ack_Button->setLayoutDirection(Qt::RightToLeft);
        Ack_Button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    /* \346\255\243\345\270\270\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"	font: 700 16pt \"Microsoft YaHei UI\";\n"
" color:rgb(255, 255, 255);\n"
"    background-color: rgb(23, 56, 101);/* \346\255\243\345\270\270\347\212\266\346\200\201\344\270\213\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"    border-radius: 5px; \n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    /* \351\274\240\346\240\207\346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"    background-color:rgb(32, 77, 136); /* \346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"}"));
        Ack_Button->setIconSize(QSize(40, 40));

        horizontalLayout->addWidget(Ack_Button);


        verticalLayout->addWidget(widget);


        retranslateUi(Set_communication_Dialog);

        QMetaObject::connectSlotsByName(Set_communication_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Set_communication_Dialog)
    {
        Set_communication_Dialog->setWindowTitle(QCoreApplication::translate("Set_communication_Dialog", "\350\277\236\346\216\245\350\207\263RGA", nullptr));
        label_14->setText(QCoreApplication::translate("Set_communication_Dialog", "\350\277\236\346\216\245\350\256\276\347\275\256", nullptr));
        label_18->setText(QCoreApplication::translate("Set_communication_Dialog", "IP\345\234\260\345\235\200\357\274\232", nullptr));
        IP1_spinBox->setSuffix(QString());
        IP1_spinBox->setPrefix(QString());
        label_19->setText(QCoreApplication::translate("Set_communication_Dialog", "\302\267", nullptr));
        IP2_spinBox->setSuffix(QString());
        IP2_spinBox->setPrefix(QString());
        label_29->setText(QCoreApplication::translate("Set_communication_Dialog", "\302\267", nullptr));
        IP3_spinBox->setSuffix(QString());
        IP3_spinBox->setPrefix(QString());
        label_30->setText(QCoreApplication::translate("Set_communication_Dialog", "\302\267", nullptr));
        IP4_spinBox->setSuffix(QString());
        IP4_spinBox->setPrefix(QString());
        label_31->setText(QCoreApplication::translate("Set_communication_Dialog", "\347\253\257\345\217\243\345\217\267\357\274\232", nullptr));
        Com_number_spinBox->setSuffix(QString());
        Com_number_spinBox->setPrefix(QString());
        Cancel_Button->setText(QCoreApplication::translate("Set_communication_Dialog", "\345\217\226\346\266\210", nullptr));
        Ack_Button->setText(QCoreApplication::translate("Set_communication_Dialog", "\347\241\256\350\256\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Set_communication_Dialog: public Ui_Set_communication_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SET_COMMUNICATION_DIALOG_H
