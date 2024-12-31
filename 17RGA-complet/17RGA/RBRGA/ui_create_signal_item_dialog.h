/********************************************************************************
** Form generated from reading UI file 'create_signal_item_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATE_SIGNAL_ITEM_DIALOG_H
#define UI_CREATE_SIGNAL_ITEM_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
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

class Ui_Create_Signal_Item_Dialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_14;
    QFrame *line_2;
    QWidget *widget_11;
    QVBoxLayout *verticalLayout_5;
    QWidget *Signal_Type_Widget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_18;
    QComboBox *Signal_type;
    QWidget *AMU_Select_Widget;
    QHBoxLayout *horizontalLayout_25;
    QLabel *label_31;
    QSpinBox *AMU_spinBox;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *Cancel_Button;
    QPushButton *Ack_Button;

    void setupUi(QDialog *Create_Signal_Item_Dialog)
    {
        if (Create_Signal_Item_Dialog->objectName().isEmpty())
            Create_Signal_Item_Dialog->setObjectName("Create_Signal_Item_Dialog");
        Create_Signal_Item_Dialog->resize(301, 177);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Create_Signal_Item_Dialog->sizePolicy().hasHeightForWidth());
        Create_Signal_Item_Dialog->setSizePolicy(sizePolicy);
        Create_Signal_Item_Dialog->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	background-color: rgb(1, 32, 79);\n"
"    border-radius: 5px; \n"
"	color: rgb(255, 255, 255);\n"
"}"));
        verticalLayout = new QVBoxLayout(Create_Signal_Item_Dialog);
        verticalLayout->setObjectName("verticalLayout");
        widget_2 = new QWidget(Create_Signal_Item_Dialog);
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
        Signal_Type_Widget = new QWidget(widget_11);
        Signal_Type_Widget->setObjectName("Signal_Type_Widget");
        Signal_Type_Widget->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_2 = new QHBoxLayout(Signal_Type_Widget);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_18 = new QLabel(Signal_Type_Widget);
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

        horizontalLayout_2->addWidget(label_18);

        Signal_type = new QComboBox(Signal_Type_Widget);
        Signal_type->addItem(QString());
        Signal_type->addItem(QString());
        Signal_type->addItem(QString());
        Signal_type->addItem(QString());
        Signal_type->setObjectName("Signal_type");
        Signal_type->setMinimumSize(QSize(0, 30));
        Signal_type->setMaximumSize(QSize(16777215, 30));
        Signal_type->setLayoutDirection(Qt::LeftToRight);
        Signal_type->setStyleSheet(QString::fromUtf8("\n"
"QComboBox{\n"
" background-color: rgb(58, 143, 255);\n"
"  border:1px solid rgb(255, 255, 255);\n"
"  border-radius:5px;\n"
"} \n"
"\n"
"QComboBox::drop-down {\n"
"	background-color: rgb(23, 56, 101);\n"
"    width: 15px;/*\350\256\276\347\275\256\346\214\211\351\222\256\350\214\203\345\233\264\345\256\275\345\272\246*/\n"
"   border-radius: 5px;\n"
"}\n"
"\n"
"QComboBox::down-arrow {\n"
"	border-image: url(:/new/prefix1/icon/down_arrow.png);/*\350\207\252\345\256\232\344\271\211\345\233\276\347\211\207\345\241\253\345\205\205*/\n"
"	width: 10px;/*\350\256\276\347\275\256\350\257\245\345\233\276\346\240\207\347\232\204\345\256\275\351\253\230*/\n"
"	height: 10px;\n"
"}\n"
"\n"
"QComboBox QAbstractItemView {\n"
" background-color: rgb(58, 143, 255);\n"
"\n"
"}\n"
"\n"
"/* \344\270\213\346\213\211\345\220\216\357\274\214\346\225\264\344\270\252\344\270\213\346\213\211\347\252\227\344\275\223\346\257\217\351\241\271\347\232\204\346\240\267\345\274\217 */\n"
"QComboBox QAbstractItemView::item {\n"
" backgroun"
                        "d-color: rgb(58, 143, 255);\n"
"\n"
"}\n"
"\n"
"QComboBox QAbstractItemView::item:selected {\n"
"    color: #FFFFF0;\n"
"	background-color: rgb(0, 85, 200);\n"
"}\n"
"QComboBox::disabled\n"
"{\n"
" background-color: rgb(23, 56, 101);\n"
"  border:1px solid rgb(158, 158, 158);\n"
"color:rgb(158, 158, 158);\n"
"\n"
"}"));
        Signal_type->setInsertPolicy(QComboBox::NoInsert);
        Signal_type->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);

        horizontalLayout_2->addWidget(Signal_type);


        verticalLayout_5->addWidget(Signal_Type_Widget);

        AMU_Select_Widget = new QWidget(widget_11);
        AMU_Select_Widget->setObjectName("AMU_Select_Widget");
        AMU_Select_Widget->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_25 = new QHBoxLayout(AMU_Select_Widget);
        horizontalLayout_25->setSpacing(0);
        horizontalLayout_25->setObjectName("horizontalLayout_25");
        horizontalLayout_25->setContentsMargins(0, 0, 0, 0);
        label_31 = new QLabel(AMU_Select_Widget);
        label_31->setObjectName("label_31");
        sizePolicy1.setHeightForWidth(label_31->sizePolicy().hasHeightForWidth());
        label_31->setSizePolicy(sizePolicy1);
        label_31->setMinimumSize(QSize(100, 30));
        label_31->setMaximumSize(QSize(100, 30));
        label_31->setFont(font);
        label_31->setStyleSheet(QString::fromUtf8(""));
        label_31->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_25->addWidget(label_31);

        AMU_spinBox = new QSpinBox(AMU_Select_Widget);
        AMU_spinBox->setObjectName("AMU_spinBox");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(AMU_spinBox->sizePolicy().hasHeightForWidth());
        AMU_spinBox->setSizePolicy(sizePolicy2);
        AMU_spinBox->setMinimumSize(QSize(0, 30));
        AMU_spinBox->setFocusPolicy(Qt::ClickFocus);
        AMU_spinBox->setStyleSheet(QString::fromUtf8("background-color: rgb(58, 143, 255);\n"
"  border:1px solid rgb(255, 255, 255);"));
        AMU_spinBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        AMU_spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        AMU_spinBox->setMaximum(200);
        AMU_spinBox->setValue(1);
        AMU_spinBox->setDisplayIntegerBase(10);

        horizontalLayout_25->addWidget(AMU_spinBox);


        verticalLayout_5->addWidget(AMU_Select_Widget);


        verticalLayout_3->addWidget(widget_11);


        verticalLayout->addWidget(widget_2);

        widget = new QWidget(Create_Signal_Item_Dialog);
        widget->setObjectName("widget");
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        Cancel_Button = new QPushButton(widget);
        Cancel_Button->setObjectName("Cancel_Button");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(Cancel_Button->sizePolicy().hasHeightForWidth());
        Cancel_Button->setSizePolicy(sizePolicy3);
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
        sizePolicy3.setHeightForWidth(Ack_Button->sizePolicy().hasHeightForWidth());
        Ack_Button->setSizePolicy(sizePolicy3);
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


        retranslateUi(Create_Signal_Item_Dialog);

        QMetaObject::connectSlotsByName(Create_Signal_Item_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Create_Signal_Item_Dialog)
    {
        Create_Signal_Item_Dialog->setWindowTitle(QCoreApplication::translate("Create_Signal_Item_Dialog", "Dialog", nullptr));
        label_14->setText(QCoreApplication::translate("Create_Signal_Item_Dialog", "\345\210\233\345\273\272\346\233\262\347\272\277", nullptr));
        label_18->setText(QCoreApplication::translate("Create_Signal_Item_Dialog", "\346\233\262\347\272\277\347\261\273\345\236\213\357\274\232", nullptr));
        Signal_type->setItemText(0, QCoreApplication::translate("Create_Signal_Item_Dialog", "\345\272\225\345\231\252", nullptr));
        Signal_type->setItemText(1, QCoreApplication::translate("Create_Signal_Item_Dialog", "\345\233\233\346\236\201\346\235\206\345\216\213\345\212\233", nullptr));
        Signal_type->setItemText(2, QCoreApplication::translate("Create_Signal_Item_Dialog", "\346\211\253\346\217\217\344\277\241\345\217\267", nullptr));
        Signal_type->setItemText(3, QCoreApplication::translate("Create_Signal_Item_Dialog", "\346\200\273\345\216\213\344\277\241\345\217\267", nullptr));

        label_31->setText(QCoreApplication::translate("Create_Signal_Item_Dialog", "\350\264\250\350\215\267\346\257\224\357\274\232", nullptr));
        AMU_spinBox->setSuffix(QString());
        AMU_spinBox->setPrefix(QString());
        Cancel_Button->setText(QCoreApplication::translate("Create_Signal_Item_Dialog", "\345\217\226\346\266\210", nullptr));
        Ack_Button->setText(QCoreApplication::translate("Create_Signal_Item_Dialog", "\347\241\256\350\256\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Create_Signal_Item_Dialog: public Ui_Create_Signal_Item_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATE_SIGNAL_ITEM_DIALOG_H
