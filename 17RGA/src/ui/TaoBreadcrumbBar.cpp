#include "./inc/TaoBreadcrumbBar.h"

#include <QHeaderView>
#include <QScroller>
#include <QVBoxLayout>

#include "./inc/TaoBreadcrumbBarDelegate.h"
#include "./inc/TaoBreadcrumbBarModel.h"
#include "./inc/TaoBaseListView.h"
#include "private/TaoBreadcrumbBarPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(TaoBreadcrumbBar, bool, IsAutoRemove);
TaoBreadcrumbBar::TaoBreadcrumbBar(QWidget* parent)
    : QWidget{parent}, d_ptr(new TaoBreadcrumbBarPrivate())
{
    Q_D(TaoBreadcrumbBar);
    d->q_ptr = this;
    d->_pTextPixelSize = 18;
    d->_pIsAutoRemove = true;
    setFixedHeight(37);
    setObjectName("TaoBreadcrumbBar");
    setStyleSheet("#TaoBreadcrumbBar{background-color:transparent;}");
    setMouseTracking(true);

    d->_listView = new TaoBaseListView(this);
    d->_listView->setMinimumHeight(0);
    d->_listView->setFlow(QListView::LeftToRight);
    d->_listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_listModel = new TaoBreadcrumbBarModel(this);
    d->_listView->setModel(d->_listModel);
    d->_listDelegate = new TaoBreadcrumbBarDelegate(this);
    d->_listView->setItemDelegate(d->_listDelegate);
    connect(d->_listView, &QListView::clicked, this, [=](const QModelIndex& index) {
        if (d->_pIsAutoRemove)
        {
            if (d->_listModel->getBreadcrumbListCount() != 1 && index.row() != d->_listModel->getBreadcrumbListCount() * 2 - 2 && index.data(Qt::DisplayRole).toString() != ">")
            {
                Q_EMIT breadcrumbClicked(index.data(Qt::DisplayRole).toString(), d->_listModel->getBreadcrumbList());
                d->_listModel->removeBreadcrumb(index.row() / 2 + 1);
            }
        }
        else
        {
            Q_EMIT breadcrumbClicked(index.data(Qt::DisplayRole).toString(), d->_listModel->getBreadcrumbList());
        }
    });
    QFont textFont = this->font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
    textFont.setPixelSize(d->_pTextPixelSize);
    d->_listView->setFont(textFont);

    QScroller::grabGesture(d->_listView->viewport(), QScroller::LeftMouseButtonGesture);
    QScroller* scroller = QScroller::scroller(d->_listView->viewport());
    QScrollerProperties properties = scroller->scrollerProperties();
    properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0);
    properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
    properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
    properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
    properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
    scroller->setScrollerProperties(properties);

    connect(scroller, &QScroller::stateChanged, this, [=](QScroller::State newstate) {
        if (newstate == QScroller::Pressed)
        {
            d->_listDelegate->setPressIndex(d->_listView->indexAt(d->_listView->mapFromGlobal(QCursor::pos())));
            d->_listView->viewport()->update();
        }
        else if (newstate == QScroller::Scrolling || newstate == QScroller::Inactive)
        {
            d->_listDelegate->setPressIndex(QModelIndex());
        }
    });

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(d->_listView);
}

TaoBreadcrumbBar::~TaoBreadcrumbBar()
{
}

void TaoBreadcrumbBar::setTextPixelSize(int textPixelSize)
{
    Q_D(TaoBreadcrumbBar);
    if (textPixelSize > 0)
    {
        d->_pTextPixelSize = textPixelSize;
        QFont textFont = this->font();
        textFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
        textFont.setPixelSize(d->_pTextPixelSize);
        d->_listView->setFont(textFont);
        Q_EMIT pTextPixelSizeChanged();
    }
}

int TaoBreadcrumbBar::getTextPixelSize() const
{
    Q_D(const TaoBreadcrumbBar);
    return d->_pTextPixelSize;
}

void TaoBreadcrumbBar::setBreadcrumbList(QStringList breadcrumbList)
{
    Q_D(TaoBreadcrumbBar);
    d->_listModel->setBreadcrumbList(breadcrumbList);
}

QStringList TaoBreadcrumbBar::appendBreadcrumb(QString breadcrumb)
{
    Q_D(TaoBreadcrumbBar);
    d->_listModel->appendBreadcrumb(breadcrumb);
    return d->_listModel->getBreadcrumbList();
}

QStringList TaoBreadcrumbBar::removeBreadcrumb(QString breadcrumb)
{
    Q_D(TaoBreadcrumbBar);
    d->_listModel->removeBreadcrumb(breadcrumb);
    return d->_listModel->getBreadcrumbList();
}

int TaoBreadcrumbBar::getBreadcrumbListCount() const
{
    Q_D(const TaoBreadcrumbBar);
    return d->_listModel->getBreadcrumbListCount();
}

QStringList TaoBreadcrumbBar::getBreadcrumbList() const
{
    Q_D(const TaoBreadcrumbBar);
    return d->_listModel->getBreadcrumbList();
}
