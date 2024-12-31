/********************************************************************************
** Form generated from reading UI file 'amu_chart.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AMU_CHART_H
#define UI_AMU_CHART_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AMU_Chart
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *X_Chart_Flash_PushButton;
    QPushButton *Y_Chart_Flash_PushButton;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *AMU_Chart)
    {
        if (AMU_Chart->objectName().isEmpty())
            AMU_Chart->setObjectName("AMU_Chart");
        AMU_Chart->resize(544, 389);
        horizontalLayout = new QHBoxLayout(AMU_Chart);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");

        horizontalLayout->addLayout(verticalLayout_2);

        widget_2 = new QWidget(AMU_Chart);
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


        retranslateUi(AMU_Chart);

        QMetaObject::connectSlotsByName(AMU_Chart);
    } // setupUi

    void retranslateUi(QWidget *AMU_Chart)
    {
        AMU_Chart->setWindowTitle(QCoreApplication::translate("AMU_Chart", "Form", nullptr));
        X_Chart_Flash_PushButton->setText(QString());
        Y_Chart_Flash_PushButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AMU_Chart: public Ui_AMU_Chart {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AMU_CHART_H
