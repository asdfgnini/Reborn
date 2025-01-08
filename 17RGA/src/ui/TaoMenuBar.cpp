#include "./inc/TaoMenuBar.h"

#include <QEvent>
#include <QPainter>
#include <QToolButton>

#include "./inc/TaoIcon.h"
#include "./inc/TaoMenu.h"
#include "./inc/TaoMenuBarStyle.h"

TaoMenuBar::TaoMenuBar(QWidget* parent)
    : QMenuBar(parent)
{
    setMouseTracking(true);
    setObjectName("TaoMenuBar");
    setStyle(new TaoMenuBarStyle(style()));
    QToolButton* tool = this->findChild<QToolButton*>();
    if (tool->objectName() == "qt_menubar_ext_button")
    {
        QMenu* oldMenu = tool->menu();
        TaoMenu* menu = new TaoMenu(this);
        menu->setObjectName("TaoExtendMenu");
        menu->setMenuItemHeight(27);
        if (oldMenu)
        {
            oldMenu->deleteLater();
        }
        tool->setMenu(menu);
    }
}

TaoMenuBar::~TaoMenuBar()
{
}

QAction* TaoMenuBar::addMenu(QMenu* menu)
{
    TaoMenu* elaMenu = dynamic_cast<TaoMenu*>(menu);
    if (elaMenu)
    {
        elaMenu->setMenuItemHeight(27);
    }
    return QMenuBar::addMenu(menu);
}

TaoMenu* TaoMenuBar::addMenu(const QString& title)
{
    TaoMenu* menu = new TaoMenu(title, this);
    menu->setMenuItemHeight(27);
    QMenuBar::addAction(menu->menuAction());
    return menu;
}

TaoMenu* TaoMenuBar::addMenu(const QIcon& icon, const QString& title)
{
    TaoMenu* menu = new TaoMenu(title, this);
    menu->setMenuItemHeight(27);
    menu->setIcon(icon);
    QMenuBar::addAction(menu->menuAction());
    return menu;
}

TaoMenu* TaoMenuBar::addMenu(TaoIconType::IconName icon, const QString& title)
{
    TaoMenu* menu = new TaoMenu(title, this);
    menu->setMenuItemHeight(27);
    menu->menuAction()->setProperty("TaoIconType", QChar((unsigned short)icon));
    menu->menuAction()->setIcon(TaoIcon::getInstance()->getTaoIcon(TaoIconType::Broom, 1));
    QMenuBar::addAction(menu->menuAction());
    return menu;
}

QAction* TaoMenuBar::addTaoIconAction(TaoIconType::IconName icon, const QString& text)
{
    QAction* action = new QAction(text, this);
    action->setProperty("TaoIconType", QChar((unsigned short)icon));
    action->setIcon(TaoIcon::getInstance()->getTaoIcon(TaoIconType::Broom, 1));
    QMenuBar::addAction(action);
    return action;
}
