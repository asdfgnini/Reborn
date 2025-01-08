#ifndef TAOEXPONENTIALBLURPRIVATE_H
#define TAOEXPONENTIALBLURPRIVATE_H

#include <QObject>

#include "../../include/stdafx.h"

class TaoExponentialBlur;
class TaoExponentialBlurPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoExponentialBlur)
public:
    explicit TaoExponentialBlurPrivate(QObject* parent = nullptr);
    ~TaoExponentialBlurPrivate();

private:
    static int _aprec;
    static int _zprec;
    static void _drawExponentialBlur(QImage& image, const quint16& qRadius);
    static void _drawRowBlur(QImage& image, const int& row, const int& alpha);
    static void _drawColumnBlur(QImage& image, const int& column, const int& alpha);
    static void _drawInnerBlur(unsigned char* bptr, int& zR, int& zG, int& zB, int& zA, int alpha);
};

#endif // TAOEXPONENTIALBLURPRIVATE_H
