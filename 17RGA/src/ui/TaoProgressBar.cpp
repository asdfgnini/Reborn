#include "./inc/TaoProgressBar.h"

#include <QPropertyAnimation>
#include <QStyleOption>

#include "private/TaoProgressBarPrivate.h"
#include "./inc/TaoProgressBarStyle.h"

TaoProgressBar::TaoProgressBar(QWidget* parent)
    : QProgressBar(parent), d_ptr(new TaoProgressBarPrivate())
{
    Q_D(TaoProgressBar);
    setObjectName("TaoProgressBar");
    setStyleSheet("#TaoProgressBar{background-color:transparent;}");
    d->q_ptr = this;
    d->_style = new TaoProgressBarStyle(style());
    setStyle(d->_style);
    d->_busyAnimation = new QPropertyAnimation(d->_style, "busyStartValue");
    connect(d->_busyAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        d->_style->setProperty("busyEndValue", value.toInt() + 75);
        update();
    });
    d->_busyAnimation->setDuration(2000);
    d->_busyAnimation->setLoopCount(-1);
}

TaoProgressBar::~TaoProgressBar()
{
}

void TaoProgressBar::setMinimum(int minimum)
{
    Q_D(TaoProgressBar);
    if (d->_isBusyAnimation && !(maximum() == 0 && minimum == 0))
    {
        d->_isBusyAnimation = false;
        d->_busyAnimation->stop();
    }
    QProgressBar::setMinimum(minimum);
}

void TaoProgressBar::setMaximum(int maximum)
{
    Q_D(TaoProgressBar);
    if (d->_isBusyAnimation && !(minimum() == 0 && maximum == 0))
    {
        d->_isBusyAnimation = false;
        d->_busyAnimation->stop();
    }
    QProgressBar::setMaximum(maximum);
}

void TaoProgressBar::paintEvent(QPaintEvent* event)
{
    Q_D(TaoProgressBar);
    if (!d->_isBusyAnimation && minimum() == 0 && maximum() == 0)
    {
        QStyleOptionProgressBar option;
        option.initFrom(this);
        d->_isBusyAnimation = true;
        d->_busyAnimation->setStartValue(-75);
        if (orientation() == Qt::Horizontal)
        {
            d->_busyAnimation->setEndValue(this->width());
        }
        else
        {
            d->_busyAnimation->setEndValue(this->height());
        }
        d->_busyAnimation->start();
    }
    QProgressBar::paintEvent(event);
}

void TaoProgressBar::resizeEvent(QResizeEvent* event)
{
    Q_D(TaoProgressBar);
    if (orientation() == Qt::Horizontal)
    {
        d->_busyAnimation->setEndValue(this->width());
    }
    else
    {
        d->_busyAnimation->setEndValue(this->height());
    }
    QProgressBar::resizeEvent(event);
}
