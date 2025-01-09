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


void TaoWindowPrivate::onThemeReadyChange()
{
    Q_Q(TaoWindow);
    qDebug() << "123";
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
        qDebug() << "456";
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


qreal TaoWindowPrivate::_distance(QPoint point1, QPoint point2)
{
    return std::sqrt((point1.x() - point2.x()) * (point1.x() - point2.x()) + (point1.y() - point2.y()) * (point1.y() - point2.y()));
}

