#include "./inc/TaoScrollPage.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QResizeEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QScroller>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "./inc/TaoBreadcrumbBar.h"
#include "./inc/TaoScrollArea.h"
#include "./inc/TaoScrollBar.h"
#include "private/TaoScrollPagePrivate.h"

TaoScrollPage::TaoScrollPage(QWidget* parent)
    : QWidget(parent), d_ptr(new TaoScrollPagePrivate())
{
    Q_D(TaoScrollPage);
    setProperty("TaoBaseClassName", "TaoScrollPage");
    d->q_ptr = this;
    d->_pCustomWidget = nullptr;
    d->_breadcrumbBar = new TaoBreadcrumbBar(this);
    d->_breadcrumbBar->setTextPixelSize(28);
    connect(d->_breadcrumbBar, &TaoBreadcrumbBar::breadcrumbClicked, this, [=](QString breadcrumb, QStringList lastBreadcrumbList) {
        if (d->_centralWidgetMap.contains(breadcrumb))
        {
            int widgetIndex = d->_centralWidgetMap.value(breadcrumb);
            d->_switchCentralStackIndex(widgetIndex, d->_navigationTargetIndex);
            d->_navigationTargetIndex = widgetIndex;
            QVariantMap routeData = QVariantMap();
            routeData.insert("TaoScrollPageCheckSumKey", "BreadcrumbClicked");
            routeData.insert("LastBreadcrumbList", lastBreadcrumbList);
        } });
    d->_pageTitleLayout = new QHBoxLayout();
    d->_pageTitleLayout->setContentsMargins(0, 0, 0, 0);
    d->_pageTitleLayout->addWidget(d->_breadcrumbBar);

    d->_centralStackedWidget = new QStackedWidget(this);
    d->_centralStackedWidget->setContentsMargins(0, 0, 0, 0);

    d->_mainLayout = new QVBoxLayout(this);
    d->_mainLayout->setContentsMargins(0, 0, 0, 0);
    d->_mainLayout->addLayout(d->_pageTitleLayout);
    d->_mainLayout->addWidget(d->_centralStackedWidget);
    setContentsMargins(20, 20, 0, 0);
}

TaoScrollPage::~TaoScrollPage()
{
}

void TaoScrollPage::addCentralWidget(QWidget* centralWidget, bool isWidgetResizeable, bool isVerticalGrabGesture, qreal mousePressEventDelay)
{
    Q_D(TaoScrollPage);
    if (!centralWidget)
    {
        return;
    }
    if (centralWidget->windowTitle().isEmpty())
    {
        centralWidget->setWindowTitle(QString("Page_%1").arg(d->_centralStackedWidget->count()));
    }
    if (d->_centralStackedWidget->count() == 0)
    {
        d->_breadcrumbBar->appendBreadcrumb(centralWidget->windowTitle());
    }
    TaoScrollArea* scrollArea = new TaoScrollArea(this);
    scrollArea->setMouseTracking(true);
    scrollArea->setIsAnimation(Qt::Vertical, true);
    scrollArea->setWidgetResizable(isWidgetResizeable);
    scrollArea->setIsGrabGesture(isVerticalGrabGesture, mousePressEventDelay);
    scrollArea->setIsOverShoot(Qt::Vertical, true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    TaoScrollBar* floatVScrollBar = new TaoScrollBar(scrollArea->verticalScrollBar(), scrollArea);
    floatVScrollBar->setIsAnimation(true);
    scrollArea->setWidget(centralWidget);
    centralWidget->setObjectName("TaoScrollPage_CentralPage");
    centralWidget->setStyleSheet("#TaoScrollPage_CentralPage{background-color:transparent;}");
    d->_centralWidgetMap.insert(centralWidget->windowTitle(), d->_centralStackedWidget->count());
    d->_centralStackedWidget->addWidget(scrollArea);
}

void TaoScrollPage::setCustomWidget(QWidget* widget)
{
    Q_D(TaoScrollPage);
    if (!widget || widget == this)
    {
        return;
    }
    if (d->_pCustomWidget)
    {
        d->_mainLayout->removeWidget(d->_pCustomWidget);
    }
    d->_mainLayout->insertWidget(1, widget);
    d->_pCustomWidget = widget;
    Q_EMIT pCustomWidgetChanged();
}

QWidget* TaoScrollPage::getCustomWidget() const
{
    Q_D(const TaoScrollPage);
    return d->_pCustomWidget;
}

void TaoScrollPage::navigation(int widgetIndex, bool isLogRoute)
{
    Q_D(TaoScrollPage);
    if (widgetIndex >= d->_centralStackedWidget->count() || d->_navigationTargetIndex == widgetIndex)
    {
        return;
    }
    d->_switchCentralStackIndex(widgetIndex, d->_navigationTargetIndex);
    d->_navigationTargetIndex = widgetIndex;
    if (isLogRoute)
    {
        QVariantMap routeData = QVariantMap();
        routeData.insert("TaoScrollPageCheckSumKey", "Navigation");
        QStringList breadcrumbList = d->_breadcrumbBar->getBreadcrumbList();
        routeData.insert("TaoPageTitle", breadcrumbList.last());
    }
    d->_breadcrumbBar->appendBreadcrumb(d->_centralWidgetMap.key(widgetIndex));
}

void TaoScrollPage::setPageTitleSpacing(int spacing)
{
    Q_D(TaoScrollPage);
    d->_pageTitleLayout->takeAt(0);
    d->_pageTitleLayout->addSpacing(spacing);
    d->_pageTitleLayout->addWidget(d->_breadcrumbBar);
}

int TaoScrollPage::getPageTitleSpacing() const
{
    return d_ptr->_pageTitleSpacing;
}

void TaoScrollPage::setTitleVisible(bool isVisible)
{
    Q_D(TaoScrollPage);
    d->_breadcrumbBar->setVisible(isVisible);
}
