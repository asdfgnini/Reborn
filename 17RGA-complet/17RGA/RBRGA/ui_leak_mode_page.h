/********************************************************************************
** Form generated from reading UI file 'leak_mode_page.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEAK_MODE_PAGE_H
#define UI_LEAK_MODE_PAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_leak_mode_page
{
public:
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;

    void setupUi(QWidget *leak_mode_page)
    {
        if (leak_mode_page->objectName().isEmpty())
            leak_mode_page->setObjectName("leak_mode_page");
        leak_mode_page->resize(400, 300);
        horizontalLayout = new QHBoxLayout(leak_mode_page);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(leak_mode_page);
        pushButton->setObjectName("pushButton");

        horizontalLayout->addWidget(pushButton);


        retranslateUi(leak_mode_page);

        QMetaObject::connectSlotsByName(leak_mode_page);
    } // setupUi

    void retranslateUi(QWidget *leak_mode_page)
    {
        leak_mode_page->setWindowTitle(QCoreApplication::translate("leak_mode_page", "Form", nullptr));
        pushButton->setText(QCoreApplication::translate("leak_mode_page", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class leak_mode_page: public Ui_leak_mode_page {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEAK_MODE_PAGE_H
