#include "./inc/T_UpdateWidget.h"

#include <QVBoxLayout>

#include "./inc/TaoText.h"

T_UpdateWidget::T_UpdateWidget(QWidget* parent)
    : QWidget{parent}
{
    setMinimumSize(200, 260);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSizeConstraint(QLayout::SetMaximumSize);
    mainLayout->setContentsMargins(5, 10, 5, 5);
    mainLayout->setSpacing(4);
    TaoText* updateTitle = new TaoText("2024-11-17更新", 15, this);
    TaoText* update1 = new TaoText("1、新增TaoKeyBinder单键位绑定组件，支持鼠标按键", 13, this);
    TaoText* update2 = new TaoText("2、TaoContentDialog中间按钮按下行为优化", 13, this);
    TaoText* update3 = new TaoText("3、TaoWindow导航模式切换相关问题修正", 13, this);
    TaoText* update4 = new TaoText("4、CMAKE编译配置和体验优化", 13, this);
    update1->setIsWrapAnywhere(true);
    update2->setIsWrapAnywhere(true);
    update3->setIsWrapAnywhere(true);
    update4->setIsWrapAnywhere(true);

    mainLayout->addWidget(updateTitle);
    mainLayout->addWidget(update1);
    mainLayout->addWidget(update2);
    mainLayout->addWidget(update3);
    mainLayout->addWidget(update4);
    mainLayout->addStretch();
}

T_UpdateWidget::~T_UpdateWidget()
{
}
