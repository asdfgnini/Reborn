/********************************************************************************
** Form generated from reading UI file 'rga_display.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RGA_DISPLAY_H
#define UI_RGA_DISPLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RGA_Display
{
public:
    QHBoxLayout *horizontalLayout;
    QStackedWidget *stackedWidget;

    void setupUi(QWidget *RGA_Display)
    {
        if (RGA_Display->objectName().isEmpty())
            RGA_Display->setObjectName("RGA_Display");
        RGA_Display->resize(694, 444);
        horizontalLayout = new QHBoxLayout(RGA_Display);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(RGA_Display);
        stackedWidget->setObjectName("stackedWidget");

        horizontalLayout->addWidget(stackedWidget);


        retranslateUi(RGA_Display);

        QMetaObject::connectSlotsByName(RGA_Display);
    } // setupUi

    void retranslateUi(QWidget *RGA_Display)
    {
        RGA_Display->setWindowTitle(QCoreApplication::translate("RGA_Display", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RGA_Display: public Ui_RGA_Display {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RGA_DISPLAY_H
