#include "TaoWindowPrivate.h"

#include <QApplication>
#include <QPropertyAnimation>
#include <QTimer>
#include <QVBoxLayout>
#include <QtMath>

#include "../inc/TaoAppBar.h"
#include "TaoAppBarPrivate.h"
#include "../inc/TaoApplication.h"
#include "../inc/TaoCentralStackedWidget.h"
#include "../inc/TaoNavigationBar.h"
#include "../inc/TaoTheme.h"
#include "../inc/TaoThemeAnimationWidget.h"
#include "../inc/TaoWindow.h"
TaoWindowPrivate::TaoWindowPrivate(QObject* parent)
    : QObject{parent}
{
}

TaoWindowPrivate::~TaoWindowPrivate()
{
}

void TaoWindowPrivate::onNavigationButtonClicked()
{

    if (_isWMClickedAnimationFinished)
    {
        _isNavigationDisplayModeChanged = false;
        _resetWindowLayout(true);
        _navigationBar->setIsTransparent(false);
        _navigationBar->setDisplayMode(TaoNavigationType::Maximal, false);
        _navigationBar->move(-_navigationBar->width(), _navigationBar->pos().y());
        _navigationBar->resize(_navigationBar->width(), _centerStackedWidget->height() + 1);
        QPropertyAnimation* navigationMoveAnimation = new QPropertyAnimation(_navigationBar, "pos");
        connect(navigationMoveAnimation, &QPropertyAnimation::finished, this, [=]() {
            _isNavigationBarExpanded = true;
        });
        navigationMoveAnimation->setEasingCurve(QEasingCurve::InOutSine);
        navigationMoveAnimation->setDuration(300);
        navigationMoveAnimation->setStartValue(_navigationBar->pos());
        navigationMoveAnimation->setEndValue(QPoint(0, 0));
        navigationMoveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        _isWMClickedAnimationFinished = false;
    }
}

void TaoWindowPrivate::onWMWindowClickedEvent(QVariantMap data)
{
    TaoAppBarType::WMMouseActionType actionType = data.value("WMClickType").value<TaoAppBarType::WMMouseActionType>();
    if (actionType == TaoAppBarType::WMLBUTTONDBLCLK || actionType == TaoAppBarType::WMLBUTTONUP || actionType == TaoAppBarType::WMNCLBUTTONDOWN)
    {
        if (TaoApplication::containsCursorToItem(_navigationBar))
        {
            return;
        }
        if (_isNavigationBarExpanded)
        {
            QPropertyAnimation* navigationMoveAnimation = new QPropertyAnimation(_navigationBar, "pos");
            connect(navigationMoveAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
                if (_isNavigationDisplayModeChanged)
                {
                    _navigationBar->setIsTransparent(true);
                    _isWMClickedAnimationFinished = true;
                    _resetWindowLayout(false);
                    navigationMoveAnimation->deleteLater();
                }
            });
            connect(navigationMoveAnimation, &QPropertyAnimation::finished, this, [=]() {
                _navigationBar->setIsTransparent(true);
                if (!_isNavigationDisplayModeChanged)
                {
                    _navigationBar->setDisplayMode(TaoNavigationType::Minimal, false);
                    _resetWindowLayout(false);
                }
                _isWMClickedAnimationFinished = true;
            });
            navigationMoveAnimation->setEasingCurve(QEasingCurve::InOutSine);
            navigationMoveAnimation->setDuration(300);
            navigationMoveAnimation->setStartValue(_navigationBar->pos());
            navigationMoveAnimation->setEndValue(QPoint(-_navigationBar->width(), 0));
            navigationMoveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            _isNavigationBarExpanded = false;
        }
    }
}

void TaoWindowPrivate::onThemeReadyChange()
{
    Q_Q(TaoWindow);
    // 主题变更绘制窗口
    _appBar->setIsOnlyAllowMinAndClose(true);
    if (!_animationWidget)
    {
        QPoint centerPos = q->mapFromGlobal(QCursor::pos());
        _animationWidget = new TaoThemeAnimationWidget(q);
        connect(_animationWidget, &TaoThemeAnimationWidget::animationFinished, this, [=]() {
            _appBar->setIsOnlyAllowMinAndClose(false);
            _animationWidget = nullptr;
        });
        _animationWidget->move(0, 0);
        _animationWidget->setOldWindowBackground(q->grab(q->rect()).toImage());
        if (tTheme->getThemeMode() == TaoThemeType::Light)
        {
            tTheme->setThemeMode(TaoThemeType::Dark);
        }
        else
        {
            tTheme->setThemeMode(TaoThemeType::Light);
        }
        _animationWidget->setNewWindowBackground(q->grab(q->rect()).toImage());
        _animationWidget->setCenter(centerPos);
        qreal topLeftDis = _distance(centerPos, QPoint(0, 0));
        qreal topRightDis = _distance(centerPos, QPoint(q->width(), 0));
        qreal bottomLeftDis = _distance(centerPos, QPoint(0, q->height()));
        qreal bottomRightDis = _distance(centerPos, QPoint(q->width(), q->height()));
        QList<qreal> disList{topLeftDis, topRightDis, bottomLeftDis, bottomRightDis};
        std::sort(disList.begin(), disList.end());
        _animationWidget->setEndRadius(disList[3]);
        _animationWidget->resize(q->width(), q->height());
        _animationWidget->startAnimation(_pThemeChangeTime);
        _animationWidget->show();
    }
}

void TaoWindowPrivate::onDisplayModeChanged()
{
    _currentNavigationBarDisplayMode = _pNavigationBarDisplayMode;
    switch (_pNavigationBarDisplayMode)
    {
    case TaoNavigationType::Auto:
    {
        _appBar->setWindowButtonFlag(TaoAppBarType::NavigationButtonHint, false);
        _doNavigationDisplayModeChange();
        break;
    }
    case TaoNavigationType::Minimal:
    {
        _navigationBar->setDisplayMode(TaoNavigationType::Minimal, true);
        _appBar->setWindowButtonFlag(TaoAppBarType::NavigationButtonHint);
        break;
    }
    case TaoNavigationType::Compact:
    {
        _navigationBar->setDisplayMode(TaoNavigationType::Compact, true);
        _appBar->setWindowButtonFlag(TaoAppBarType::NavigationButtonHint, false);
        break;
    }
    case TaoNavigationType::Maximal:
    {
        _navigationBar->setDisplayMode(TaoNavigationType::Maximal, true);
        _appBar->setWindowButtonFlag(TaoAppBarType::NavigationButtonHint, false);
        break;
    }
    }
}

void TaoWindowPrivate::onThemeModeChanged(TaoThemeType::ThemeMode themeMode)
{
    Q_Q(TaoWindow);
    _themeMode = themeMode;
    if (!tApp->getIsEnableMica())
    {
        QPalette palette = q->palette();
        palette.setBrush(QPalette::Window, TaoThemeColor(_themeMode, WindowBase));
        q->setPalette(palette);
    }
}

void TaoWindowPrivate::onNavigationNodeClicked(TaoNavigationType::NavigationNodeType nodeType, QString nodeKey)
{
    int nodeIndex = _routeMap.value(nodeKey);
    if (nodeIndex == -1)
    {
        // 页脚没有绑定页面
        return;
    }
    if (_navigationTargetIndex == nodeIndex || _centerStackedWidget->count() <= nodeIndex)
    {
        return;
    }
    _navigationTargetIndex = nodeIndex;
    QTimer::singleShot(180, this, [=]() {
        QWidget* currentWidget = _centerStackedWidget->widget(nodeIndex);
        _centerStackedWidget->setCurrentIndex(nodeIndex);
        QPropertyAnimation* currentWidgetAnimation = new QPropertyAnimation(currentWidget, "pos");
        currentWidgetAnimation->setEasingCurve(QEasingCurve::OutCubic);
        currentWidgetAnimation->setDuration(300);
        QPoint currentWidgetPos = currentWidget->pos();
        currentWidgetAnimation->setEndValue(currentWidgetPos);
        currentWidgetPos.setY(currentWidgetPos.y() + 80);
        currentWidgetAnimation->setStartValue(currentWidgetPos);
        currentWidgetAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    });
}

void TaoWindowPrivate::onNavigationNodeAdded(TaoNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page)
{
    if (nodeType == TaoNavigationType::PageNode)
    {
        _routeMap.insert(nodeKey, _centerStackedWidget->count());
        _centerStackedWidget->addWidget(page);
    }
    else
    {
        if (page)
        {
            _routeMap.insert(nodeKey, _centerStackedWidget->count());
            _centerStackedWidget->addWidget(page);
        }
        else
        {
            _routeMap.insert(nodeKey, -1);
        }
    }
}

qreal TaoWindowPrivate::_distance(QPoint point1, QPoint point2)
{
    return std::sqrt((point1.x() - point2.x()) * (point1.x() - point2.x()) + (point1.y() - point2.y()) * (point1.y() - point2.y()));
}

void TaoWindowPrivate::_resetWindowLayout(bool isAnimation)
{
    if (isAnimation)
    {
        while (_centerLayout->count() > 0)
        {
            _centerLayout->takeAt(0);
        }
    }
    else
    {
        if (_centerLayout->count() == 0)
        {
            _navigationBar->setDisplayMode(TaoNavigationType::Minimal, false);
            _centerLayout->addWidget(_navigationBar);
            _centerLayout->addWidget(_centerStackedWidget);
        }
    }
}

void TaoWindowPrivate::_doNavigationDisplayModeChange()
{
    Q_Q(TaoWindow);
    if (_isWindowClosing || !_isNavigationEnable || !_isInitFinished)
    {
        return;
    }
    if (_pNavigationBarDisplayMode == TaoNavigationType::Minimal)
    {
        _resetWindowLayout(false);
    }
    if (_pNavigationBarDisplayMode == TaoNavigationType::Auto)
    {
        _isNavigationDisplayModeChanged = true;
        _resetWindowLayout(false);
        int width = q->centralWidget()->width();
        if (width >= 850 && _currentNavigationBarDisplayMode != TaoNavigationType::Maximal)
        {
            _navigationBar->setDisplayMode(TaoNavigationType::Maximal);
            _currentNavigationBarDisplayMode = TaoNavigationType::Maximal;
            _appBar->setWindowButtonFlag(TaoAppBarType::NavigationButtonHint, false);
        }
        else if (width >= 550 && width < 850 && _currentNavigationBarDisplayMode != TaoNavigationType::Compact)
        {
            _navigationBar->setDisplayMode(TaoNavigationType::Compact);
            _currentNavigationBarDisplayMode = TaoNavigationType::Compact;
            _appBar->setWindowButtonFlag(TaoAppBarType::NavigationButtonHint, false);
        }
        else if (width < 550 && _currentNavigationBarDisplayMode != TaoNavigationType::Minimal)
        {
            _navigationBar->setDisplayMode(TaoNavigationType::Minimal);
            _currentNavigationBarDisplayMode = TaoNavigationType::Minimal;
            _appBar->setWindowButtonFlag(TaoAppBarType::NavigationButtonHint);
        }
        _isNavigationBarExpanded = false;
    }
}
