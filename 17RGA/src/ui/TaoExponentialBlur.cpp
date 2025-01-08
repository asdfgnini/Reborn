#include "./inc/TaoExponentialBlur.h"

#include <QPainter>
#include <QPixmap>

#include "./private/TaoExponentialBlurPrivate.h"
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <cmath>
#endif
Q_SINGLETON_CREATE_CPP(TaoExponentialBlur)
TaoExponentialBlur::TaoExponentialBlur(QObject* parent)
    : QObject{parent}, d_ptr(new TaoExponentialBlurPrivate())
{
    Q_D(TaoExponentialBlur);
    d->q_ptr = this;
}

TaoExponentialBlur::~TaoExponentialBlur()
{
}

QPixmap TaoExponentialBlur::doExponentialBlur(QImage img, const quint16& blurRadius)
{
    QImage shadowImage = img.convertToFormat(QImage::Format_ARGB32);
    TaoExponentialBlur::getInstance()->d_ptr->_drawExponentialBlur(shadowImage, blurRadius);
    return QPixmap::fromImage(shadowImage);
}
