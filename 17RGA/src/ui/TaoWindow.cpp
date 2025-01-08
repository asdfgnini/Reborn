#include "./inc/TaoWindow.h"

#include <QApplication>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QScreen>
#include <QStackedWidget>
#include <QStyleOption>
#include <QTimer>
#include <QToolBar>
#include <QVBoxLayout>

#include "./inc/TaoApplication.h"
#include "./inc/TaoCentralStackedWidget.h"
#include "./inc/TaoEventBus.h"
#include "./inc/TaoMenu.h"

#include "./inc/TaoNavigationRouter.h"
#include "./inc/TaoTheme.h"
#include "./inc/TaoWindowStyle.h"
#include "private/TaoAppBarPrivate.h"
#include "private/TaoNavigationBarPrivate.h"
#include "private/TaoWindowPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(TaoWindow, int, ThemeChangeTime)
Q_PROPERTY_CREATE_Q_CPP(TaoWindow, TaoNavigationType::NavigationDisplayMode, NavigationBarDisplayMode)
Q_TAKEOVER_NATIVEEVENT_CPP(TaoWindow, d_func()->_appBar);
TaoWindow::TaoWindow(QWidget* parent)
    : QMainWindow{parent}, d_ptr(new TaoWindowPrivate())
{
    Q_D(TaoWindow);
    d->q_ptr = this;

    setProperty("TaoBaseClassName", "TaoWindow");
    resize(1020, 680); // 默认宽高

    d->_pThemeChangeTime = 700;
    d->_pNavigationBarDisplayMode = TaoNavigationType::NavigationDisplayMode::Auto;
    connect(this, &TaoWindow::pNavigationBarDisplayModeChanged, d, &TaoWindowPrivate::onDisplayModeChanged);

    // 自定义AppBar
    d->_appBar = new TaoAppBar(this);
    connect(d->_appBar, &TaoAppBar::routeBackButtonClicked, this, []() {
        TaoNavigationRouter::getInstance()->navigationRouteBack();
    });
    connect(d->_appBar, &TaoAppBar::closeButtonClicked, this, &TaoWindow::closeButtonClicked);
    // 导航栏
    // 返回按钮状态变更
    connect(TaoNavigationRouter::getInstance(), &TaoNavigationRouter::navigationRouterStateChanged, this, [d](bool isEnable) {
        d->_appBar->setRouteBackButtonEnable(isEnable);
    });

    // 中心堆栈窗口
    d->_centerStackedWidget = new TaoCentralStackedWidget(this);
    d->_centerStackedWidget->setContentsMargins(0, 0, 0, 0);
    QWidget* centralWidget = new QWidget(this);
    d->_centerLayout = new QHBoxLayout(centralWidget);
    d->_centerLayout->setSpacing(0);
    d->_centerLayout->addWidget(d->_centerStackedWidget);
    d->_centerLayout->setContentsMargins(d->_contentsMargins, 0, 0, 0);

    // 事件总线
    d->_focusEvent = new TaoEvent("WMWindowClicked", "onWMWindowClickedEvent", d);
    d->_focusEvent->registerAndInit();

    // 展开导航栏
    connect(d->_appBar, &TaoAppBar::navigationButtonClicked, d, &TaoWindowPrivate::onNavigationButtonClicked);

    // 主题变更动画
    d->_themeMode = tTheme->getThemeMode();
    connect(tTheme, &TaoTheme::themeModeChanged, d, &TaoWindowPrivate::onThemeModeChanged);
    connect(d->_appBar, &TaoAppBar::themeChangeButtonClicked, d, &TaoWindowPrivate::onThemeReadyChange);
    d->_isInitFinished = true;
    setCentralWidget(centralWidget);
    centralWidget->installEventFilter(this);

    setObjectName("TaoWindow");
    setStyleSheet("#TaoWindow{background-color:transparent;}");
    setStyle(new TaoWindowStyle(style()));

    //延时渲染
    QTimer::singleShot(1, this, [=] {
        QPalette palette = this->palette();
        palette.setBrush(QPalette::Window, TaoThemeColor(d->_themeMode, WindowBase));
        this->setPalette(palette);
    });
    tApp->syncMica(this);
    connect(tApp, &TaoApplication::pIsEnableMicaChanged, this, [=]() {
        d->onThemeModeChanged(d->_themeMode);
    });
}

TaoWindow::~TaoWindow()
{
}

void TaoWindow::setIsStayTop(bool isStayTop)
{
    Q_D(TaoWindow);
    d->_appBar->setIsStayTop(isStayTop);
    Q_EMIT pIsStayTopChanged();
}

bool TaoWindow::getIsStayTop() const
{
    return d_ptr->_appBar->getIsStayTop();
}

void TaoWindow::setIsFixedSize(bool isFixedSize)
{
    Q_D(TaoWindow);
    d->_appBar->setIsFixedSize(isFixedSize);
    Q_EMIT pIsFixedSizeChanged();
}

bool TaoWindow::getIsFixedSize() const
{
    return d_ptr->_appBar->getIsFixedSize();
}

void TaoWindow::setIsDefaultClosed(bool isDefaultClosed)
{
    Q_D(TaoWindow);
    d->_appBar->setIsDefaultClosed(isDefaultClosed);
    Q_EMIT pIsDefaultClosedChanged();
}

bool TaoWindow::getIsDefaultClosed() const
{
    return d_ptr->_appBar->getIsDefaultClosed();
}

void TaoWindow::setAppBarHeight(int appBarHeight)
{
    Q_D(TaoWindow);
    d->_appBar->setAppBarHeight(appBarHeight);
    Q_EMIT pAppBarHeightChanged();
}

int TaoWindow::getAppBarHeight() const
{
    Q_D(const TaoWindow);
    return d->_appBar->getAppBarHeight();
}

QWidget* TaoWindow::getCustomWidget() const
{
    Q_D(const TaoWindow);
    return d->_appBar->getCustomWidget();
}

void TaoWindow::setCustomWidgetMaximumWidth(int width)
{
    Q_D(TaoWindow);
    d->_appBar->setCustomWidgetMaximumWidth(width);
    Q_EMIT pCustomWidgetMaximumWidthChanged();
}

int TaoWindow::getCustomWidgetMaximumWidth() const
{
    Q_D(const TaoWindow);
    return d->_appBar->getCustomWidgetMaximumWidth();
}

void TaoWindow::setIsCentralStackedWidgetTransparent(bool isTransparent)
{
    Q_D(TaoWindow);
    d->_centerStackedWidget->setIsTransparent(isTransparent);
}

bool TaoWindow::getIsCentralStackedWidgetTransparent() const
{
    Q_D(const TaoWindow);
    return d->_centerStackedWidget->getIsTransparent();
}

void TaoWindow::moveToCenter()
{
    if (isMaximized() || isFullScreen())
    {
        return;
    }
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    auto geometry = screen()->availableGeometry();
#else
    auto geometry = qApp->screenAt(this->geometry().center())->geometry();
#endif
    setGeometry((geometry.left() + geometry.right() - width()) / 2, (geometry.top() + geometry.bottom() - height()) / 2, width(), height());
}

void TaoWindow::setCustomWidget(TaoAppBarType::CustomArea customArea, QWidget* widget)
{
    Q_D(TaoWindow);
    d->_appBar->setCustomWidget(customArea, widget);
    Q_EMIT customWidgetChanged();
}


bool TaoWindow::getIsNavigationBarEnable() const
{
    return d_ptr->_isNavigationEnable;
}

void TaoWindow::setWindowButtonFlag(TaoAppBarType::ButtonType buttonFlag, bool isEnable)
{
    Q_D(TaoWindow);
    d->_appBar->setWindowButtonFlag(buttonFlag, isEnable);
}

void TaoWindow::setWindowButtonFlags(TaoAppBarType::ButtonFlags buttonFlags)
{
    Q_D(TaoWindow);
    d->_appBar->setWindowButtonFlags(buttonFlags);
}

TaoAppBarType::ButtonFlags TaoWindow::getWindowButtonFlags() const
{
    return d_ptr->_appBar->getWindowButtonFlags();
}

void TaoWindow::closeWindow()
{
    Q_D(TaoWindow);
    d->_isWindowClosing = true;
    d->_appBar->closeWindow();
}

bool TaoWindow::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(TaoWindow);
    switch (event->type())
    {
    case QEvent::Resize:
    {
        d->_doNavigationDisplayModeChange();
        break;
    }
    default:
    {
        break;
    }
    }
    return QMainWindow::eventFilter(watched, event);
}

QMenu* TaoWindow::createPopupMenu()
{
    TaoMenu* menu = nullptr;
    QList<QDockWidget*> dockwidgets = findChildren<QDockWidget*>();
    if (dockwidgets.size())
    {
        menu = new TaoMenu(this);
        for (int i = 0; i < dockwidgets.size(); ++i)
        {
            QDockWidget* dockWidget = dockwidgets.at(i);
            if (dockWidget->parentWidget() == this)
            {
                menu->addAction(dockwidgets.at(i)->toggleViewAction());
            }
        }
        menu->addSeparator();
    }

    QList<QToolBar*> toolbars = findChildren<QToolBar*>();
    if (toolbars.size())
    {
        if (!menu)
        {
            menu = new TaoMenu(this);
        }
        for (int i = 0; i < toolbars.size(); ++i)
        {
            QToolBar* toolBar = toolbars.at(i);
            if (toolBar->parentWidget() == this)
            {
                menu->addAction(toolbars.at(i)->toggleViewAction());
            }
        }
    }
    if (menu)
    {
        menu->setMenuItemHeight(28);
    }
    return menu;
}
