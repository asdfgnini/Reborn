/********************************************************************************
** Form generated from reading UI file 'sweep_mode_page.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SWEEP_MODE_PAGE_H
#define UI_SWEEP_MODE_PAGE_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Sweep_Mode_Page
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QWidget *Scan_widget;
    QVBoxLayout *verticalLayout_7;
    QPushButton *Hide_Sweep_Set_PushButton;
    QWidget *Scan_Seting_widget;
    QVBoxLayout *verticalLayout_5;
    QLabel *title;
    QFrame *line_3;
    QWidget *Scan_Setting_item;
    QVBoxLayout *verticalLayout_6;
    QWidget *AMU_Range;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_21;
    QSpinBox *AMU_Start_spinBox;
    QLabel *label_22;
    QSpinBox *AMU_Stop_spinBox;
    QWidget *PPamu_Range;
    QHBoxLayout *horizontalLayout_17;
    QLabel *label_23;
    QComboBox *PPamu;
    QWidget *Dwell_Range;
    QHBoxLayout *horizontalLayout_18;
    QLabel *label_24;
    QComboBox *Dwell;
    QSpacerItem *verticalSpacer;
    QWidget *Control_widget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *Emission_pushButton;
    QPushButton *EM_pushButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *Scan_pushButton;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_2;
    QWidget *RGA_State;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *Show_Sweep_Set_PushButton;
    QWidget *Start_AMU_Display;
    QVBoxLayout *verticalLayout_9;
    QLabel *title_4;
    QFrame *line_5;
    QLabel *Start_AMU_Value;
    QWidget *Stop_AMU_Display;
    QVBoxLayout *verticalLayout_10;
    QLabel *title_6;
    QFrame *line_6;
    QLabel *Stop_AMU_Value;
    QWidget *Ppamu_Display;
    QVBoxLayout *verticalLayout_11;
    QLabel *title_8;
    QFrame *line_7;
    QLabel *Ppamu_Value;
    QSpacerItem *horizontalSpacer_4;
    QWidget *Scan_Number_Display;
    QVBoxLayout *verticalLayout_12;
    QLabel *title_10;
    QFrame *line_8;
    QLabel *Scan_Number_Value;
    QWidget *Tol_Pressure_Display;
    QVBoxLayout *verticalLayout_8;
    QLabel *title_2;
    QFrame *line_4;
    QLabel *Tol_Pressure_Value;
    QWidget *Chart_Display;
    QVBoxLayout *verticalLayout_14;
    QVBoxLayout *AMU_Chart_verticalLayout;
    QVBoxLayout *Chart_verticalLayout;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget_4;

    void setupUi(QWidget *Sweep_Mode_Page)
    {
        if (Sweep_Mode_Page->objectName().isEmpty())
            Sweep_Mode_Page->setObjectName("Sweep_Mode_Page");
        Sweep_Mode_Page->resize(1002, 520);
        Sweep_Mode_Page->setStyleSheet(QString::fromUtf8("background-color: rgb(10, 22, 46);"));
        horizontalLayout = new QHBoxLayout(Sweep_Mode_Page);
        horizontalLayout->setSpacing(4);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(Sweep_Mode_Page);
        widget->setObjectName("widget");
        widget->setMaximumSize(QSize(360, 16777215));
        widget->setFocusPolicy(Qt::ClickFocus);
        widget->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	background-color: rgb(1, 32, 79);\n"
"    border-radius: 5px; \n"
"    \n"
"	color: rgb(255, 255, 255);\n"
"}"));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        Scan_widget = new QWidget(widget);
        Scan_widget->setObjectName("Scan_widget");
        Scan_widget->setStyleSheet(QString::fromUtf8("#Scan_widget{\n"
"	background-color: rgb(1, 32, 79);\n"
"    border-radius: 5px; \n"
"  border:2px solid rgb(255, 255, 255);\n"
"}"));
        verticalLayout_7 = new QVBoxLayout(Scan_widget);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName("verticalLayout_7");
        verticalLayout_7->setContentsMargins(10, 10, 10, 6);
        Hide_Sweep_Set_PushButton = new QPushButton(Scan_widget);
        Hide_Sweep_Set_PushButton->setObjectName("Hide_Sweep_Set_PushButton");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Hide_Sweep_Set_PushButton->sizePolicy().hasHeightForWidth());
        Hide_Sweep_Set_PushButton->setSizePolicy(sizePolicy);
        Hide_Sweep_Set_PushButton->setMinimumSize(QSize(35, 35));
        QFont font;
        font.setFamilies({QString::fromUtf8("Microsoft YaHei UI")});
        font.setPointSize(18);
        font.setBold(true);
        font.setItalic(false);
        Hide_Sweep_Set_PushButton->setFont(font);
        Hide_Sweep_Set_PushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    /* \346\255\243\345\270\270\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"	font: 700 18pt \"Microsoft YaHei UI\";\n"
" color:rgb(255, 255, 255);\n"
" background-color: #00BFFF;\n"
"    border-radius: 5px; \n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    /* \351\274\240\346\240\207\346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"    background-color:#87CEFA; /* \346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"}"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/icon/hide.png"), QSize(), QIcon::Normal, QIcon::Off);
        Hide_Sweep_Set_PushButton->setIcon(icon);
        Hide_Sweep_Set_PushButton->setIconSize(QSize(20, 20));

        verticalLayout_7->addWidget(Hide_Sweep_Set_PushButton);

        Scan_Seting_widget = new QWidget(Scan_widget);
        Scan_Seting_widget->setObjectName("Scan_Seting_widget");
        Scan_Seting_widget->setEnabled(true);
        Scan_Seting_widget->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"background-color: rgb(23, 56, 101);\n"
"font: 15pt \"Microsoft YaHei UI\";\n"
"}\n"
"QcomboBox\n"
"{\n"
"background-color: rgb(58, 143, 255);\n"
"}\n"
"QLabel::disabled\n"
"{\n"
"color:rgb(158, 158, 158)\n"
"\n"
"}\n"
"\n"
""));
        verticalLayout_5 = new QVBoxLayout(Scan_Seting_widget);
        verticalLayout_5->setSpacing(4);
        verticalLayout_5->setObjectName("verticalLayout_5");
        title = new QLabel(Scan_Seting_widget);
        title->setObjectName("title");
        title->setStyleSheet(QString::fromUtf8("font: 700 16pt \"Microsoft YaHei UI\";"));
        title->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(title);

        line_3 = new QFrame(Scan_Seting_widget);
        line_3->setObjectName("line_3");
        line_3->setEnabled(true);
        line_3->setStyleSheet(QString::fromUtf8("#line_3\n"
"{\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"\n"
"#line_3::disabled\n"
"{\n"
"background-color: rgb(158, 158, 158);\n"
"}"));
        line_3->setFrameShadow(QFrame::Plain);
        line_3->setFrameShape(QFrame::HLine);

        verticalLayout_5->addWidget(line_3);

        Scan_Setting_item = new QWidget(Scan_Seting_widget);
        Scan_Setting_item->setObjectName("Scan_Setting_item");
        Scan_Setting_item->setStyleSheet(QString::fromUtf8("font: 14pt \"Microsoft YaHei UI\";"));
        verticalLayout_6 = new QVBoxLayout(Scan_Setting_item);
        verticalLayout_6->setSpacing(15);
        verticalLayout_6->setObjectName("verticalLayout_6");
        verticalLayout_6->setContentsMargins(0, 5, 0, 5);
        AMU_Range = new QWidget(Scan_Setting_item);
        AMU_Range->setObjectName("AMU_Range");
        AMU_Range->setEnabled(true);
        AMU_Range->setStyleSheet(QString::fromUtf8("\n"
"QSpinBox\n"
"{\n"
"  border:1px solid rgb(255, 255, 255);\n"
"background-color: rgb(58, 143, 255);\n"
"\n"
"}\n"
"QSpinBox::disabled\n"
"{\n"
" background-color: rgb(23, 56, 101);\n"
"  border:1px solid rgb(158, 158, 158);\n"
"color:rgb(158, 158, 158);\n"
"\n"
"}"));
        horizontalLayout_16 = new QHBoxLayout(AMU_Range);
        horizontalLayout_16->setSpacing(0);
        horizontalLayout_16->setObjectName("horizontalLayout_16");
        horizontalLayout_16->setContentsMargins(0, 0, 0, 0);
        label_21 = new QLabel(AMU_Range);
        label_21->setObjectName("label_21");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(100);
        sizePolicy1.setVerticalStretch(30);
        sizePolicy1.setHeightForWidth(label_21->sizePolicy().hasHeightForWidth());
        label_21->setSizePolicy(sizePolicy1);
        label_21->setMinimumSize(QSize(100, 30));
        label_21->setMaximumSize(QSize(100, 30));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Microsoft YaHei UI")});
        font1.setPointSize(14);
        font1.setBold(false);
        font1.setItalic(false);
        label_21->setFont(font1);
        label_21->setStyleSheet(QString::fromUtf8(""));
        label_21->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_16->addWidget(label_21);

        AMU_Start_spinBox = new QSpinBox(AMU_Range);
        AMU_Start_spinBox->setObjectName("AMU_Start_spinBox");
        sizePolicy.setHeightForWidth(AMU_Start_spinBox->sizePolicy().hasHeightForWidth());
        AMU_Start_spinBox->setSizePolicy(sizePolicy);
        AMU_Start_spinBox->setMinimumSize(QSize(90, 30));
        AMU_Start_spinBox->setFocusPolicy(Qt::ClickFocus);
        AMU_Start_spinBox->setStyleSheet(QString::fromUtf8(""));
        AMU_Start_spinBox->setAlignment(Qt::AlignCenter);
        AMU_Start_spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);

        horizontalLayout_16->addWidget(AMU_Start_spinBox);

        label_22 = new QLabel(AMU_Range);
        label_22->setObjectName("label_22");
        label_22->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_22->sizePolicy().hasHeightForWidth());
        label_22->setSizePolicy(sizePolicy2);
        label_22->setMinimumSize(QSize(15, 30));
        label_22->setMaximumSize(QSize(15, 30));
        label_22->setAlignment(Qt::AlignCenter);

        horizontalLayout_16->addWidget(label_22);

        AMU_Stop_spinBox = new QSpinBox(AMU_Range);
        AMU_Stop_spinBox->setObjectName("AMU_Stop_spinBox");
        AMU_Stop_spinBox->setEnabled(true);
        sizePolicy.setHeightForWidth(AMU_Stop_spinBox->sizePolicy().hasHeightForWidth());
        AMU_Stop_spinBox->setSizePolicy(sizePolicy);
        AMU_Stop_spinBox->setMinimumSize(QSize(90, 30));
        AMU_Stop_spinBox->setFocusPolicy(Qt::ClickFocus);
        AMU_Stop_spinBox->setStyleSheet(QString::fromUtf8(""));
        AMU_Stop_spinBox->setAlignment(Qt::AlignCenter);
        AMU_Stop_spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        AMU_Stop_spinBox->setMaximum(200);

        horizontalLayout_16->addWidget(AMU_Stop_spinBox);


        verticalLayout_6->addWidget(AMU_Range);

        PPamu_Range = new QWidget(Scan_Setting_item);
        PPamu_Range->setObjectName("PPamu_Range");
        sizePolicy.setHeightForWidth(PPamu_Range->sizePolicy().hasHeightForWidth());
        PPamu_Range->setSizePolicy(sizePolicy);
        PPamu_Range->setMinimumSize(QSize(0, 30));
        PPamu_Range->setMaximumSize(QSize(16777215, 30));
        PPamu_Range->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_17 = new QHBoxLayout(PPamu_Range);
        horizontalLayout_17->setSpacing(0);
        horizontalLayout_17->setObjectName("horizontalLayout_17");
        horizontalLayout_17->setContentsMargins(0, 0, 0, 0);
        label_23 = new QLabel(PPamu_Range);
        label_23->setObjectName("label_23");
        sizePolicy2.setHeightForWidth(label_23->sizePolicy().hasHeightForWidth());
        label_23->setSizePolicy(sizePolicy2);
        label_23->setMinimumSize(QSize(100, 30));
        label_23->setMaximumSize(QSize(100, 30));
        label_23->setFont(font1);
        label_23->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_17->addWidget(label_23);

        PPamu = new QComboBox(PPamu_Range);
        PPamu->setObjectName("PPamu");
        PPamu->setMinimumSize(QSize(0, 30));
        PPamu->setMaximumSize(QSize(16777215, 30));
        PPamu->setLayoutDirection(Qt::LeftToRight);
        PPamu->setStyleSheet(QString::fromUtf8("\n"
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
        PPamu->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);

        horizontalLayout_17->addWidget(PPamu);


        verticalLayout_6->addWidget(PPamu_Range);

        Dwell_Range = new QWidget(Scan_Setting_item);
        Dwell_Range->setObjectName("Dwell_Range");
        sizePolicy.setHeightForWidth(Dwell_Range->sizePolicy().hasHeightForWidth());
        Dwell_Range->setSizePolicy(sizePolicy);
        Dwell_Range->setMinimumSize(QSize(0, 30));
        Dwell_Range->setMaximumSize(QSize(16777215, 30));
        Dwell_Range->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_18 = new QHBoxLayout(Dwell_Range);
        horizontalLayout_18->setSpacing(0);
        horizontalLayout_18->setObjectName("horizontalLayout_18");
        horizontalLayout_18->setContentsMargins(0, 0, 0, 0);
        label_24 = new QLabel(Dwell_Range);
        label_24->setObjectName("label_24");
        sizePolicy2.setHeightForWidth(label_24->sizePolicy().hasHeightForWidth());
        label_24->setSizePolicy(sizePolicy2);
        label_24->setMinimumSize(QSize(100, 30));
        label_24->setMaximumSize(QSize(100, 30));
        label_24->setFont(font1);
        label_24->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_18->addWidget(label_24);

        Dwell = new QComboBox(Dwell_Range);
        Dwell->setObjectName("Dwell");
        Dwell->setEnabled(true);
        Dwell->setMinimumSize(QSize(0, 30));
        Dwell->setMaximumSize(QSize(16777215, 30));
        Dwell->setLayoutDirection(Qt::LeftToRight);
        Dwell->setStyleSheet(QString::fromUtf8("\n"
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
"\n"
"\n"
"QComboBox QAbstractItemView::item:selected {\n"
"    color: #FFFFF0;\n"
"	background-color: rgb(0, 85, 200);\n"
"}\n"
"\n"
"QComboBox::disabled\n"
"{\n"
" background-color: rgb(23, 56, 101);\n"
"  border:1px solid rgb(158, 158, 158);\n"
"color:rgb(158, 158, 158);\n"
"\n"
"}\n"
""));
        Dwell->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);

        horizontalLayout_18->addWidget(Dwell);


        verticalLayout_6->addWidget(Dwell_Range);


        verticalLayout_5->addWidget(Scan_Setting_item);


        verticalLayout_7->addWidget(Scan_Seting_widget);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_7->addItem(verticalSpacer);

        Control_widget = new QWidget(Scan_widget);
        Control_widget->setObjectName("Control_widget");
        horizontalLayout_2 = new QHBoxLayout(Control_widget);
        horizontalLayout_2->setSpacing(4);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        Emission_pushButton = new QPushButton(Control_widget);
        Emission_pushButton->setObjectName("Emission_pushButton");
        sizePolicy2.setHeightForWidth(Emission_pushButton->sizePolicy().hasHeightForWidth());
        Emission_pushButton->setSizePolicy(sizePolicy2);
        Emission_pushButton->setMinimumSize(QSize(50, 50));
        Emission_pushButton->setMaximumSize(QSize(50, 50));
        Emission_pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        icon1.addFile(QString::fromUtf8(":/new/prefix1/icon/Lamp_Closed.png"), QSize(), QIcon::Normal, QIcon::Off);
        Emission_pushButton->setIcon(icon1);
        Emission_pushButton->setIconSize(QSize(40, 40));

        horizontalLayout_2->addWidget(Emission_pushButton);

        EM_pushButton = new QPushButton(Control_widget);
        EM_pushButton->setObjectName("EM_pushButton");
        sizePolicy2.setHeightForWidth(EM_pushButton->sizePolicy().hasHeightForWidth());
        EM_pushButton->setSizePolicy(sizePolicy2);
        EM_pushButton->setMinimumSize(QSize(50, 50));
        EM_pushButton->setMaximumSize(QSize(50, 50));
        EM_pushButton->setLayoutDirection(Qt::LeftToRight);
        EM_pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    /* \346\255\243\345\270\270\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"    background-color: rgb(23, 56, 101);/* \346\255\243\345\270\270\347\212\266\346\200\201\344\270\213\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"    border-radius: 5px; \n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    /* \351\274\240\346\240\207\346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"    background-color:rgb(32, 77, 136); /* \346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"}"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/new/prefix1/icon/EM_Operation.png"), QSize(), QIcon::Normal, QIcon::Off);
        EM_pushButton->setIcon(icon2);
        EM_pushButton->setIconSize(QSize(40, 40));

        horizontalLayout_2->addWidget(EM_pushButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        Scan_pushButton = new QPushButton(Control_widget);
        Scan_pushButton->setObjectName("Scan_pushButton");
        sizePolicy2.setHeightForWidth(Scan_pushButton->sizePolicy().hasHeightForWidth());
        Scan_pushButton->setSizePolicy(sizePolicy2);
        Scan_pushButton->setMinimumSize(QSize(140, 50));
        Scan_pushButton->setMaximumSize(QSize(140, 50));
        Scan_pushButton->setLayoutDirection(Qt::RightToLeft);
        Scan_pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/new/prefix1/icon/Start.png"), QSize(), QIcon::Normal, QIcon::Off);
        Scan_pushButton->setIcon(icon3);
        Scan_pushButton->setIconSize(QSize(40, 40));

        horizontalLayout_2->addWidget(Scan_pushButton);


        verticalLayout_7->addWidget(Control_widget);


        verticalLayout->addWidget(Scan_widget);


        horizontalLayout->addWidget(widget);

        widget_3 = new QWidget(Sweep_Mode_Page);
        widget_3->setObjectName("widget_3");
        widget_3->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	background-color: rgb(10, 22, 46);\n"
"    border-radius: 5px; \n"
"    \n"
"	color: rgb(255, 255, 255);\n"
"}"));
        verticalLayout_2 = new QVBoxLayout(widget_3);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        RGA_State = new QWidget(widget_3);
        RGA_State->setObjectName("RGA_State");
        RGA_State->setMinimumSize(QSize(0, 95));
        RGA_State->setMaximumSize(QSize(16777215, 95));
        RGA_State->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	background-color: rgb(1, 32, 79);\n"
"    border-radius: 5px; \n"
"	color: rgb(255, 255, 255);\n"
"}\n"
"\n"
"#RGA_State{border:2px solid rgb(255, 255, 255)}"));
        horizontalLayout_5 = new QHBoxLayout(RGA_State);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        Show_Sweep_Set_PushButton = new QPushButton(RGA_State);
        Show_Sweep_Set_PushButton->setObjectName("Show_Sweep_Set_PushButton");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(Show_Sweep_Set_PushButton->sizePolicy().hasHeightForWidth());
        Show_Sweep_Set_PushButton->setSizePolicy(sizePolicy3);
        Show_Sweep_Set_PushButton->setMinimumSize(QSize(35, 0));
        Show_Sweep_Set_PushButton->setFont(font);
        Show_Sweep_Set_PushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    /* \346\255\243\345\270\270\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"	font: 700 18pt \"Microsoft YaHei UI\";\n"
" color:rgb(255, 255, 255);\n"
" background-color: #00BFFF;\n"
"    border-radius: 5px; \n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    /* \351\274\240\346\240\207\346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\346\240\267\345\274\217 */\n"
"    background-color:#87CEFA; /* \346\202\254\345\201\234\347\212\266\346\200\201\344\270\213\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"}"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/new/prefix1/icon/show.png"), QSize(), QIcon::Normal, QIcon::Off);
        Show_Sweep_Set_PushButton->setIcon(icon4);
        Show_Sweep_Set_PushButton->setIconSize(QSize(20, 20));

        horizontalLayout_5->addWidget(Show_Sweep_Set_PushButton);

        Start_AMU_Display = new QWidget(RGA_State);
        Start_AMU_Display->setObjectName("Start_AMU_Display");
        Start_AMU_Display->setMinimumSize(QSize(100, 0));
        Start_AMU_Display->setMaximumSize(QSize(100, 16777215));
        Start_AMU_Display->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"background-color: rgb(23, 56, 101);\n"
"font: 15pt \"Microsoft YaHei UI\";\n"
"}\n"
"\n"
"#Start_AMU_Display{border:2px solid rgb(255, 255, 255)}"));
        verticalLayout_9 = new QVBoxLayout(Start_AMU_Display);
        verticalLayout_9->setSpacing(0);
        verticalLayout_9->setObjectName("verticalLayout_9");
        verticalLayout_9->setContentsMargins(4, 4, 4, 4);
        title_4 = new QLabel(Start_AMU_Display);
        title_4->setObjectName("title_4");
        title_4->setStyleSheet(QString::fromUtf8("font: 700 14pt \"Microsoft YaHei UI\";"));
        title_4->setAlignment(Qt::AlignCenter);

        verticalLayout_9->addWidget(title_4);

        line_5 = new QFrame(Start_AMU_Display);
        line_5->setObjectName("line_5");
        line_5->setEnabled(true);
        line_5->setStyleSheet(QString::fromUtf8("#line_5\n"
"{\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"\n"
"#line_5::disabled\n"
"{\n"
"background-color: rgb(158, 158, 158);\n"
"}"));
        line_5->setFrameShadow(QFrame::Plain);
        line_5->setFrameShape(QFrame::HLine);

        verticalLayout_9->addWidget(line_5);

        Start_AMU_Value = new QLabel(Start_AMU_Display);
        Start_AMU_Value->setObjectName("Start_AMU_Value");
        Start_AMU_Value->setStyleSheet(QString::fromUtf8("font: 700 16pt \"Microsoft YaHei UI\";"));
        Start_AMU_Value->setAlignment(Qt::AlignCenter);

        verticalLayout_9->addWidget(Start_AMU_Value);

        verticalLayout_9->setStretch(0, 3);
        verticalLayout_9->setStretch(2, 8);

        horizontalLayout_5->addWidget(Start_AMU_Display);

        Stop_AMU_Display = new QWidget(RGA_State);
        Stop_AMU_Display->setObjectName("Stop_AMU_Display");
        Stop_AMU_Display->setMinimumSize(QSize(100, 0));
        Stop_AMU_Display->setMaximumSize(QSize(100, 16777215));
        Stop_AMU_Display->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"background-color: rgb(23, 56, 101);\n"
"font: 15pt \"Microsoft YaHei UI\";\n"
"}\n"
"\n"
"#Stop_AMU_Display{border:2px solid rgb(255, 255, 255)}"));
        verticalLayout_10 = new QVBoxLayout(Stop_AMU_Display);
        verticalLayout_10->setSpacing(0);
        verticalLayout_10->setObjectName("verticalLayout_10");
        verticalLayout_10->setContentsMargins(4, 4, 4, 4);
        title_6 = new QLabel(Stop_AMU_Display);
        title_6->setObjectName("title_6");
        title_6->setStyleSheet(QString::fromUtf8("font: 700 14pt \"Microsoft YaHei UI\";"));
        title_6->setAlignment(Qt::AlignCenter);

        verticalLayout_10->addWidget(title_6);

        line_6 = new QFrame(Stop_AMU_Display);
        line_6->setObjectName("line_6");
        line_6->setEnabled(true);
        line_6->setStyleSheet(QString::fromUtf8("#line_6\n"
"{\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"\n"
"#line_6::disabled\n"
"{\n"
"background-color: rgb(158, 158, 158);\n"
"}"));
        line_6->setFrameShadow(QFrame::Plain);
        line_6->setFrameShape(QFrame::HLine);

        verticalLayout_10->addWidget(line_6);

        Stop_AMU_Value = new QLabel(Stop_AMU_Display);
        Stop_AMU_Value->setObjectName("Stop_AMU_Value");
        Stop_AMU_Value->setStyleSheet(QString::fromUtf8("font: 700 16pt \"Microsoft YaHei UI\";"));
        Stop_AMU_Value->setAlignment(Qt::AlignCenter);

        verticalLayout_10->addWidget(Stop_AMU_Value);

        verticalLayout_10->setStretch(0, 3);
        verticalLayout_10->setStretch(2, 8);

        horizontalLayout_5->addWidget(Stop_AMU_Display);

        Ppamu_Display = new QWidget(RGA_State);
        Ppamu_Display->setObjectName("Ppamu_Display");
        Ppamu_Display->setMinimumSize(QSize(100, 0));
        Ppamu_Display->setMaximumSize(QSize(100, 16777215));
        Ppamu_Display->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"background-color: rgb(23, 56, 101);\n"
"font: 15pt \"Microsoft YaHei UI\";\n"
"}\n"
"\n"
"#Ppamu_Display{border:2px solid rgb(255, 255, 255)}"));
        verticalLayout_11 = new QVBoxLayout(Ppamu_Display);
        verticalLayout_11->setSpacing(0);
        verticalLayout_11->setObjectName("verticalLayout_11");
        verticalLayout_11->setContentsMargins(4, 4, 4, 4);
        title_8 = new QLabel(Ppamu_Display);
        title_8->setObjectName("title_8");
        title_8->setStyleSheet(QString::fromUtf8("font: 700 14pt \"Microsoft YaHei UI\";"));
        title_8->setAlignment(Qt::AlignCenter);

        verticalLayout_11->addWidget(title_8);

        line_7 = new QFrame(Ppamu_Display);
        line_7->setObjectName("line_7");
        line_7->setEnabled(true);
        line_7->setStyleSheet(QString::fromUtf8("#line_7\n"
"{\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"\n"
"#line_7::disabled\n"
"{\n"
"background-color: rgb(158, 158, 158);\n"
"}"));
        line_7->setFrameShadow(QFrame::Plain);
        line_7->setFrameShape(QFrame::HLine);

        verticalLayout_11->addWidget(line_7);

        Ppamu_Value = new QLabel(Ppamu_Display);
        Ppamu_Value->setObjectName("Ppamu_Value");
        Ppamu_Value->setStyleSheet(QString::fromUtf8("font: 700 16pt \"Microsoft YaHei UI\";"));
        Ppamu_Value->setAlignment(Qt::AlignCenter);

        verticalLayout_11->addWidget(Ppamu_Value);

        verticalLayout_11->setStretch(0, 3);
        verticalLayout_11->setStretch(2, 8);

        horizontalLayout_5->addWidget(Ppamu_Display);

        horizontalSpacer_4 = new QSpacerItem(198, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);

        Scan_Number_Display = new QWidget(RGA_State);
        Scan_Number_Display->setObjectName("Scan_Number_Display");
        Scan_Number_Display->setMinimumSize(QSize(100, 0));
        Scan_Number_Display->setMaximumSize(QSize(100, 16777215));
        Scan_Number_Display->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"background-color: rgb(23, 56, 101);\n"
"font: 15pt \"Microsoft YaHei UI\";\n"
"}\n"
"\n"
"\n"
"\n"
"#Scan_Number_Display{border:2px solid rgb(255, 255, 255)}"));
        verticalLayout_12 = new QVBoxLayout(Scan_Number_Display);
        verticalLayout_12->setObjectName("verticalLayout_12");
        verticalLayout_12->setContentsMargins(4, 4, 4, 4);
        title_10 = new QLabel(Scan_Number_Display);
        title_10->setObjectName("title_10");
        title_10->setStyleSheet(QString::fromUtf8("font: 700 14pt \"Microsoft YaHei UI\";"));
        title_10->setAlignment(Qt::AlignCenter);

        verticalLayout_12->addWidget(title_10);

        line_8 = new QFrame(Scan_Number_Display);
        line_8->setObjectName("line_8");
        line_8->setEnabled(true);
        line_8->setStyleSheet(QString::fromUtf8("#line_8\n"
"{\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"\n"
"#line_8::disabled\n"
"{\n"
"background-color: rgb(158, 158, 158);\n"
"}"));
        line_8->setFrameShadow(QFrame::Plain);
        line_8->setFrameShape(QFrame::HLine);

        verticalLayout_12->addWidget(line_8);

        Scan_Number_Value = new QLabel(Scan_Number_Display);
        Scan_Number_Value->setObjectName("Scan_Number_Value");
        Scan_Number_Value->setStyleSheet(QString::fromUtf8("font: 700 16pt \"Microsoft YaHei UI\";"));
        Scan_Number_Value->setAlignment(Qt::AlignCenter);

        verticalLayout_12->addWidget(Scan_Number_Value);

        verticalLayout_12->setStretch(0, 3);
        verticalLayout_12->setStretch(2, 8);

        horizontalLayout_5->addWidget(Scan_Number_Display);

        Tol_Pressure_Display = new QWidget(RGA_State);
        Tol_Pressure_Display->setObjectName("Tol_Pressure_Display");
        Tol_Pressure_Display->setMinimumSize(QSize(200, 0));
        Tol_Pressure_Display->setMaximumSize(QSize(100, 16777215));
        Tol_Pressure_Display->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"background-color: rgb(23, 56, 101);\n"
"font: 15pt \"Microsoft YaHei UI\";\n"
"}\n"
"\n"
"\n"
"#Tol_Pressure_Display{border:2px solid rgb(255, 255, 255)}"));
        verticalLayout_8 = new QVBoxLayout(Tol_Pressure_Display);
        verticalLayout_8->setObjectName("verticalLayout_8");
        verticalLayout_8->setContentsMargins(4, 4, 4, 4);
        title_2 = new QLabel(Tol_Pressure_Display);
        title_2->setObjectName("title_2");
        title_2->setStyleSheet(QString::fromUtf8("font: 700 14pt \"Microsoft YaHei UI\";"));
        title_2->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(title_2);

        line_4 = new QFrame(Tol_Pressure_Display);
        line_4->setObjectName("line_4");
        line_4->setEnabled(true);
        line_4->setStyleSheet(QString::fromUtf8("#line_4\n"
"{\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"\n"
"#line_4::disabled\n"
"{\n"
"background-color: rgb(158, 158, 158);\n"
"}"));
        line_4->setFrameShadow(QFrame::Plain);
        line_4->setLineWidth(1);
        line_4->setFrameShape(QFrame::HLine);

        verticalLayout_8->addWidget(line_4);

        Tol_Pressure_Value = new QLabel(Tol_Pressure_Display);
        Tol_Pressure_Value->setObjectName("Tol_Pressure_Value");
        Tol_Pressure_Value->setStyleSheet(QString::fromUtf8("font: 700 16pt \"Microsoft YaHei UI\";"));
        Tol_Pressure_Value->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(Tol_Pressure_Value);

        verticalLayout_8->setStretch(0, 3);
        verticalLayout_8->setStretch(2, 8);

        horizontalLayout_5->addWidget(Tol_Pressure_Display);


        verticalLayout_2->addWidget(RGA_State);

        Chart_Display = new QWidget(widget_3);
        Chart_Display->setObjectName("Chart_Display");
        Chart_Display->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	background-color: rgb(28, 28, 30);\n"
"    border-radius: 5px; \n"
"	color: rgb(255, 255, 255);\n"
"}\n"
"\n"
"#Chart_Display{\n"
"	background-color: rgb(28, 28, 30);\n"
"    border-radius: 5px; \n"
"     border:1px solid rgb(255, 255, 255);\n"
"	color: rgb(255, 255, 255);\n"
"}"));
        verticalLayout_14 = new QVBoxLayout(Chart_Display);
        verticalLayout_14->setObjectName("verticalLayout_14");
        AMU_Chart_verticalLayout = new QVBoxLayout();
        AMU_Chart_verticalLayout->setObjectName("AMU_Chart_verticalLayout");

        verticalLayout_14->addLayout(AMU_Chart_verticalLayout);

        Chart_verticalLayout = new QVBoxLayout();
        Chart_verticalLayout->setObjectName("Chart_verticalLayout");
        widget_2 = new QWidget(Chart_Display);
        widget_2->setObjectName("widget_2");
        verticalLayout_3 = new QVBoxLayout(widget_2);
        verticalLayout_3->setObjectName("verticalLayout_3");
        widget_4 = new QWidget(widget_2);
        widget_4->setObjectName("widget_4");

        verticalLayout_3->addWidget(widget_4);


        Chart_verticalLayout->addWidget(widget_2);


        verticalLayout_14->addLayout(Chart_verticalLayout);


        verticalLayout_2->addWidget(Chart_Display);

        verticalLayout_2->setStretch(1, 10);

        horizontalLayout->addWidget(widget_3);


        retranslateUi(Sweep_Mode_Page);

        QMetaObject::connectSlotsByName(Sweep_Mode_Page);
    } // setupUi

    void retranslateUi(QWidget *Sweep_Mode_Page)
    {
        Sweep_Mode_Page->setWindowTitle(QCoreApplication::translate("Sweep_Mode_Page", "Form", nullptr));
        Hide_Sweep_Set_PushButton->setText(QString());
        title->setText(QCoreApplication::translate("Sweep_Mode_Page", "\346\211\253\346\217\217\350\256\276\347\275\256", nullptr));
        label_21->setText(QCoreApplication::translate("Sweep_Mode_Page", "\350\264\250\350\260\261\350\214\203\345\233\264\357\274\232", nullptr));
        AMU_Start_spinBox->setSuffix(QCoreApplication::translate("Sweep_Mode_Page", " Amu", nullptr));
        AMU_Start_spinBox->setPrefix(QString());
        label_22->setText(QCoreApplication::translate("Sweep_Mode_Page", "-", nullptr));
        AMU_Stop_spinBox->setSuffix(QCoreApplication::translate("Sweep_Mode_Page", " Amu", nullptr));
        AMU_Stop_spinBox->setPrefix(QString());
        label_23->setText(QCoreApplication::translate("Sweep_Mode_Page", "\345\210\206\350\276\250\347\216\207\357\274\232", nullptr));
        label_24->setText(QCoreApplication::translate("Sweep_Mode_Page", "\346\211\253\346\217\217\346\227\266\351\227\264\357\274\232", nullptr));
        Emission_pushButton->setText(QString());
        EM_pushButton->setText(QString());
        Scan_pushButton->setText(QCoreApplication::translate("Sweep_Mode_Page", "\345\274\200\345\247\213\346\211\253\346\217\217", nullptr));
        Show_Sweep_Set_PushButton->setText(QString());
        title_4->setText(QCoreApplication::translate("Sweep_Mode_Page", "\350\265\267\345\247\213\350\264\250\350\260\261", nullptr));
        Start_AMU_Value->setText(QCoreApplication::translate("Sweep_Mode_Page", "-", nullptr));
        title_6->setText(QCoreApplication::translate("Sweep_Mode_Page", "\347\273\210\346\255\242\350\264\250\350\260\261", nullptr));
        Stop_AMU_Value->setText(QCoreApplication::translate("Sweep_Mode_Page", "-", nullptr));
        title_8->setText(QCoreApplication::translate("Sweep_Mode_Page", "\345\210\206\350\276\250\347\216\207", nullptr));
        Ppamu_Value->setText(QCoreApplication::translate("Sweep_Mode_Page", "-", nullptr));
        title_10->setText(QCoreApplication::translate("Sweep_Mode_Page", "\346\211\253\346\217\217\350\275\256\346\254\241", nullptr));
        Scan_Number_Value->setText(QCoreApplication::translate("Sweep_Mode_Page", "-", nullptr));
        title_2->setText(QCoreApplication::translate("Sweep_Mode_Page", "\346\200\273\345\216\213\345\274\272", nullptr));
        Tol_Pressure_Value->setText(QCoreApplication::translate("Sweep_Mode_Page", "-", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Sweep_Mode_Page: public Ui_Sweep_Mode_Page {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SWEEP_MODE_PAGE_H
