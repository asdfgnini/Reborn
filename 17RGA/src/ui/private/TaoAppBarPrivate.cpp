#include "TaoAppBarPrivate.h"

#include "../inc/TaoToolButton.h"
#ifdef Q_OS_WIN
#include <Windows.h>
#endif
#include <QGuiApplication>
#include <QLabel>
#include <QPropertyAnimation>
#include <QScreen>
#include <QVBoxLayout>
#include <QWidget>

#include "../inc/TaoAppBar.h"
#include "../inc/TaoIconButton.h"
#include "../inc/TaoText.h"

TaoAppBarPrivate::TaoAppBarPrivate(QObject* parent)
    : QObject{parent}
{
}

TaoAppBarPrivate::~TaoAppBarPrivate()
{
}

void TaoAppBarPrivate::onMinButtonClicked()
{
    Q_Q(TaoAppBar);
    q->window()->showMinimized();
}

void TaoAppBarPrivate::onMaxButtonClicked()
{
    Q_Q(TaoAppBar);
    if (q->window()->isMaximized())
    {
        q->window()->showNormal();
    }
    else
    {
        q->window()->showMaximized();
    }
}

void TaoAppBarPrivate::onCloseButtonClicked()
{
    Q_Q(TaoAppBar);
    if (_pIsDefaultClosed)
    {
        q->window()->close();
    }
    else
    {
        Q_EMIT q_ptr->closeButtonClicked();
    }
}

void TaoAppBarPrivate::onStayTopButtonClicked()
{
#ifdef Q_OS_WIN
    HWND hwnd = (HWND)_currentWinID;
    ::SetWindowPos(hwnd, _pIsStayTop ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
#else
    Q_Q(const TaoAppBar);
    bool isVisible = q->window()->isVisible();
    q->window()->setWindowFlag(Qt::WindowStaysOnTopHint, _pIsStayTop);
    if (isVisible)
    {
        q->window()->show();
    }
#endif
    _stayTopButton->setIsSelected(_pIsStayTop);
    _stayTopButton->update();
}

void TaoAppBarPrivate::_changeMaxButtonAwesome(bool isMaximized)
{
    if (isMaximized)
    {
        _maxButton->setTaoIcon(TaoIconType::TAOwindow_maximize);
    }
    else
    {
        _maxButton->setTaoIcon(TaoIconType::TAOexpand);
    }
}

void TaoAppBarPrivate::_showSystemMenu(QPoint point)
{
    Q_Q(const TaoAppBar);
#ifdef Q_OS_WIN
    QScreen* screen = qApp->screenAt(QCursor::pos());
    if (!screen)
    {
        screen = QGuiApplication::primaryScreen();
    }
    if (!screen)
    {
        return;
    }
    const QPoint origin = screen->geometry().topLeft();
    auto nativePos = QPointF(QPointF(point - origin) * screen->devicePixelRatio()).toPoint() + origin;
    HWND hwnd = reinterpret_cast<HWND>(q->window()->winId());
    const HMENU hMenu = ::GetSystemMenu(hwnd, FALSE);
    if (q->window()->isMaximized() || q->window()->isFullScreen())
    {
        ::EnableMenuItem(hMenu, SC_MOVE, MFS_DISABLED);
        ::EnableMenuItem(hMenu, SC_RESTORE, MFS_ENABLED);
    }
    else
    {
        ::EnableMenuItem(hMenu, SC_MOVE, MFS_ENABLED);
        ::EnableMenuItem(hMenu, SC_RESTORE, MFS_DISABLED);
    }
    if (!_pIsFixedSize && !q->window()->isMaximized() && !q->window()->isFullScreen())
    {
        ::EnableMenuItem(hMenu, SC_SIZE, MFS_ENABLED);
        ::EnableMenuItem(hMenu, SC_MAXIMIZE, MFS_ENABLED);
    }
    else
    {
        ::EnableMenuItem(hMenu, SC_SIZE, MFS_DISABLED);
        ::EnableMenuItem(hMenu, SC_MAXIMIZE, MFS_DISABLED);
    }
    const int result = ::TrackPopupMenu(hMenu, (TPM_RETURNCMD | (QGuiApplication::isRightToLeft() ? TPM_RIGHTALIGN : TPM_LEFTALIGN)), nativePos.x(),
                                        nativePos.y(), 0, hwnd, nullptr);
    if (result != FALSE)
    {
        ::PostMessageW(hwnd, WM_SYSCOMMAND, result, 0);
    }
#endif
}

void TaoAppBarPrivate::_updateCursor(int edges)
{
    Q_Q(const TaoAppBar);
    switch (edges)
    {
    case 0:
    {
        q->window()->setCursor(Qt::ArrowCursor);
        break;
    }
    case Qt::LeftEdge:
    case Qt::RightEdge:
    {
        q->window()->setCursor(Qt::SizeHorCursor);
        break;
    }
    case Qt::TopEdge:
    case Qt::BottomEdge:
    {
        q->window()->setCursor(Qt::SizeVerCursor);
        break;
    }
    case Qt::LeftEdge | Qt::TopEdge:
    case Qt::RightEdge | Qt::BottomEdge:
    {
        q->window()->setCursor(Qt::SizeFDiagCursor);
        break;
    }
    case Qt::RightEdge | Qt::TopEdge:
    case Qt::LeftEdge | Qt::BottomEdge:
    {
        q->window()->setCursor(Qt::SizeBDiagCursor);
        break;
    }
    default:
    {
        break;
    }
    }
}

bool TaoAppBarPrivate::_containsCursorToItem(QWidget* item)
{
    Q_Q(const TaoAppBar);
    if (!item || !item->isVisible())
    {
        return false;
    }
    auto point = item->window()->mapFromGlobal(QCursor::pos());
    QRectF rect = QRectF(item->mapTo(item->window(), QPoint(0, 0)), item->size());
    if (item == q)
    {
        if (_containsCursorToItem(_pCustomWidget) || _containsCursorToItem(_stayTopButton) || _containsCursorToItem(_themeChangeButton) || _containsCursorToItem(_minButton) || _containsCursorToItem(_maxButton) || _containsCursorToItem(_closeButton))
        {
            return false;
        }
    }
    if (rect.contains(point))
    {
        return true;
    }
    return false;
}

void TaoAppBarPrivate::_onThemeModeChange(TaoThemeType::ThemeMode themeMode)
{
    if (themeMode == TaoThemeType::Light)
    {
        _themeChangeButton->setTaoIcon(TaoIconType::TAOfile);
    }
    else
    {
        _themeChangeButton->setTaoIcon(TaoIconType::TAOfile);
    }
}

int TaoAppBarPrivate::_calculateMinimumWidth()
{
    Q_Q(TaoAppBar);
    int width = 0;
    if (_titleLabel->isVisible())
    {
        width += _titleLabel->width();
        width += 10;
    }
    if (_iconLabel->isVisible())
    {
        width += _iconLabel->width();
        width += 10;
    }

    width += 5;

    if (_pCustomWidget)
    {
        int customWidgetWidth = _pCustomWidget->width();

        width += customWidgetWidth;

    }
    QList<QAbstractButton*> buttonList = q->findChildren<QAbstractButton*>();
    for (auto button : buttonList)
    {
        if (button->isVisible() && button->objectName() != "NavigationButton")
        {
            width += button->width();
        }
    }
    return width;
}

QVBoxLayout* TaoAppBarPrivate::_createVLayout(QWidget* widget)
{
    if (!widget)
    {
        return nullptr;
    }
    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    if (widget == _iconLabel || widget == _titleLabel)
    {
        vLayout->addSpacing(6);
    }
    vLayout->addWidget(widget);
    vLayout->addStretch();
    return vLayout;
}
