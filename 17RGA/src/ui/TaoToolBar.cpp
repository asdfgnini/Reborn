#include "./inc/TaoToolBar.h"

#include <QLayout>
#include <QPainter>
#include <QStyleOption>

#include "./inc/TaoIcon.h"
#include "./inc/TaoTheme.h"
#include "./private/TaoToolBarPrivate.h"
#include "./inc/TaoToolBarStyle.h"

TaoToolBar::TaoToolBar(QWidget* parent)
    : QToolBar(parent), d_ptr(new TaoToolBarPrivate())
{
    Q_D(TaoToolBar);
    d->q_ptr = this;
    setObjectName("TaoToolBar");
    setStyle(new TaoToolBarStyle(style()));
    layout()->setSpacing(10);
    layout()->setContentsMargins(3, 3, 3, 3);

    d->_themeMode = tTheme->getThemeMode();
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        if (this->isFloating())
        {
            update();
        }
    });
    setAttribute(Qt::WA_TranslucentBackground);

    connect(this, &TaoToolBar::topLevelChanged, this, [=](bool topLevel) {
        if (topLevel)
        {
            layout()->setContentsMargins(d->_shadowBorderWidth + 3, d->_shadowBorderWidth + 3, d->_shadowBorderWidth + 3, d->_shadowBorderWidth + 3);
        }
        else
        {
            layout()->setContentsMargins(3, 3, 3, 3);
        }
    });
}

TaoToolBar::TaoToolBar(const QString& title, QWidget* parent)
    : TaoToolBar(parent)
{
    setWindowTitle(title);
}

TaoToolBar::~TaoToolBar()
{
}

void TaoToolBar::setToolBarSpacing(int spacing)
{
    layout()->setSpacing(spacing);
}

int TaoToolBar::getToolBarSpacing() const
{
    return layout()->spacing();
}

QAction* TaoToolBar::addTaoIconAction(TaoIconType::IconName icon, const QString& text)
{
    QAction* action = new QAction(text, this);
    action->setProperty("TaoIconType", QChar((unsigned short)icon));
    action->setIcon(TaoIcon::getInstance()->getTaoIcon(TaoIconType::TAOfile, 1));
    addAction(action);
    return action;
}

QAction* TaoToolBar::addTaoIconAction(TaoIconType::IconName icon, const QString& text, const QKeySequence& shortcut)
{
    QAction* action = new QAction(text, this);
    action->setShortcut(shortcut);
    action->setProperty("TaoIconType", QChar((unsigned short)icon));
    action->setIcon(TaoIcon::getInstance()->getTaoIcon(TaoIconType::TAOfile, 1));
    addAction(action);
    return action;
}

void TaoToolBar::paintEvent(QPaintEvent* event)
{
    Q_D(TaoToolBar);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing);
    if (isFloating())
    {
        // 高性能阴影
        tTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, 6);
        //背景
        painter.setPen(TaoThemeColor(d->_themeMode, PopupBorder));
        painter.setBrush(TaoThemeColor(d->_themeMode, DialogBase));
        QRect foregroundRect(d->_shadowBorderWidth, d->_shadowBorderWidth, width() - 2 * d->_shadowBorderWidth, height() - 2 * d->_shadowBorderWidth);
        painter.drawRoundedRect(foregroundRect, 5, 5);
        QStyle* style = this->style();
        QStyleOptionToolBar opt;
        initStyleOption(&opt);
        opt.rect = style->subElementRect(QStyle::SE_ToolBarHandle, &opt, this);
        if (opt.rect.isValid())
        {
            style->drawPrimitive(QStyle::PE_IndicatorToolBarHandle, &opt, &painter, this);
        }
    }
    else
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::transparent);
        painter.drawRoundedRect(rect(), 5, 5);
        QToolBar::paintEvent(event);
    }
    painter.restore();
}
