#include "./inc/TaoMenu.h"

#include <QApplication>
#include <QCloseEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QVBoxLayout>

#include "./inc/TaoMenuStyle.h"
#include "private/TaoMenuPrivate.h"

TaoMenu::TaoMenu(QWidget* parent)
    : QMenu(parent), d_ptr(new TaoMenuPrivate())
{
    Q_D(TaoMenu);
    d->q_ptr = this;
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setObjectName("TaoMenu");
    d->_menuStyle = new TaoMenuStyle(style());
    setStyle(d->_menuStyle);
    d->_pAnimationImagePosY = 0;
}

TaoMenu::TaoMenu(const QString& title, QWidget* parent)
    : TaoMenu(parent)
{
    setTitle(title);
}

TaoMenu::~TaoMenu()
{
}

void TaoMenu::setMenuItemHeight(int menuItemHeight)
{
    Q_D(TaoMenu);
    d->_menuStyle->setMenuItemHeight(menuItemHeight);
}

int TaoMenu::getMenuItemHeight() const
{
    Q_D(const TaoMenu);
    return d->_menuStyle->getMenuItemHeight();
}

QAction* TaoMenu::addMenu(QMenu* menu)
{
    return QMenu::addMenu(menu);
}

TaoMenu* TaoMenu::addMenu(const QString& title)
{
    TaoMenu* menu = new TaoMenu(title, this);
    QMenu::addAction(menu->menuAction());
    return menu;
}

TaoMenu* TaoMenu::addMenu(const QIcon& icon, const QString& title)
{
    TaoMenu* menu = new TaoMenu(title, this);
    menu->setIcon(icon);
    QMenu::addAction(menu->menuAction());
    return menu;
}

TaoMenu* TaoMenu::addMenu(TaoIconType::IconName icon, const QString& title)
{
    TaoMenu* menu = new TaoMenu(title, this);
    QMenu::addAction(menu->menuAction());
    menu->menuAction()->setProperty("TaoIconType", QChar((unsigned short)icon));
    return menu;
}

QAction* TaoMenu::addTaoIconAction(TaoIconType::IconName icon, const QString& text)
{
    QAction* action = new QAction(text, this);
    action->setProperty("TaoIconType", QChar((unsigned short)icon));
    QMenu::addAction(action);
    return action;
}

QAction* TaoMenu::addTaoIconAction(TaoIconType::IconName icon, const QString& text, const QKeySequence& shortcut)
{
    QAction* action = new QAction(text, this);
    action->setShortcut(shortcut);
    action->setProperty("TaoIconType", QChar((unsigned short)icon));
    QMenu::addAction(action);
    return action;
}

bool TaoMenu::isHasChildMenu() const
{
    QList<QAction*> actionList = this->actions();
    for (auto action : actionList)
    {
        if (action->isSeparator())
        {
            continue;
        }
        if (action->menu())
        {
            return true;
        }
    }
    return false;
}

bool TaoMenu::isHasIcon() const
{
    QList<QAction*> actionList = this->actions();
    for (auto action : actionList)
    {
        if (action->isSeparator())
        {
            continue;
        }
        QMenu* menu = action->menu();
        if (menu && (!menu->icon().isNull() || !menu->property("TaoIconType").toString().isEmpty()))
        {
            return true;
        }
        if (!action->icon().isNull() || !action->property("TaoIconType").toString().isEmpty())
        {
            return true;
        }
    }
    return false;
}

void TaoMenu::showEvent(QShowEvent* event)
{
    Q_EMIT menuShow();
    Q_D(TaoMenu);
    //消除阴影偏移
    move(this->pos().x() - 6, this->pos().y());
    if (!d->_animationPix.isNull())
    {
        d->_animationPix = QPixmap();
    }
    d->_animationPix = this->grab(this->rect());
    QPropertyAnimation* posAnimation = new QPropertyAnimation(d, "pAnimationImagePosY");
    connect(posAnimation, &QPropertyAnimation::finished, this, [=]() {
        d->_animationPix = QPixmap();
        update();
    });
    connect(posAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        update();
    });
    posAnimation->setEasingCurve(QEasingCurve::OutCubic);
    posAnimation->setDuration(400);
    int targetPosY = height();
    if (targetPosY > 160)
    {
        if (targetPosY < 320)
        {
            targetPosY = 160;
        }
        else
        {
            targetPosY /= 2;
        }
    }

    if (pos().y() + d->_menuStyle->getMenuItemHeight() + 9 >= QCursor::pos().y())
    {
        posAnimation->setStartValue(-targetPosY);
    }
    else
    {
        posAnimation->setStartValue(targetPosY);
    }

    posAnimation->setEndValue(0);
    posAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    QMenu::showEvent(event);
}

void TaoMenu::paintEvent(QPaintEvent* event)
{
    Q_D(TaoMenu);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    if (!d->_animationPix.isNull())
    {
        painter.drawPixmap(QRect(0, d->_pAnimationImagePosY, width(), height()), d->_animationPix);
    }
    else
    {
        QMenu::paintEvent(event);
    }
}
