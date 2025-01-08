#ifndef TAOEXPONENTIALBLUR_H
#define TAOEXPONENTIALBLUR_H

#include <QObject>

#include "../../include/stdafx.h"
#include "../../include/singleton.h"

class TaoExponentialBlurPrivate;
class TAO_EXPORT TaoExponentialBlur : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(TaoExponentialBlur)
    Q_Q_CREATE(TaoExponentialBlur)
private:
    explicit TaoExponentialBlur(QObject* parent = nullptr);
    ~TaoExponentialBlur();

public:
    static QPixmap doExponentialBlur(QImage img, const quint16& blurRadius);
};

#endif // TAOEXPONENTIALBLUR_H
