#ifndef TAOBREADCRUMBBAR_H
#define TAOBREADCRUMBBAR_H
#include <QWidget>

#include "../../include/stdafx.h"

class TaoBreadcrumbBarPrivate;
class TAO_EXPORT TaoBreadcrumbBar : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(TaoBreadcrumbBar)
    Q_PROPERTY_CREATE_Q_H(int, TextPixelSize)
    Q_PROPERTY_CREATE_Q_H(bool, IsAutoRemove)
public:
    explicit TaoBreadcrumbBar(QWidget* parent = nullptr);
    ~TaoBreadcrumbBar();
    void setBreadcrumbList(QStringList breadcrumbList);
    QStringList appendBreadcrumb(QString breadcrumb);
    QStringList removeBreadcrumb(QString breadcrumb);

    int getBreadcrumbListCount() const;
    QStringList getBreadcrumbList() const;

Q_SIGNALS:
    Q_SIGNAL void breadcrumbClicked(QString breadcrumb, QStringList lastBreadcrumbList);
};

#endif // TAOBREADCRUMBBAR_H
