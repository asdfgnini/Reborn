#ifndef TAOTHEMEANIMATIONWIDGET_H
#define TAOTHEMEANIMATIONWIDGET_H

#include <QWidget>

#include "../../include/stdafx.h"

class TaoThemeAnimationWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY_CREATE(qreal, Radius)
    Q_PROPERTY_CREATE(qreal, EndRadius)
    Q_PROPERTY_CREATE(QPoint, Center)
    Q_PROPERTY_CREATE(QImage, OldWindowBackground)
    Q_PROPERTY_CREATE(QImage, NewWindowBackground)
public:
    explicit TaoThemeAnimationWidget(QWidget* parent = nullptr);
    ~TaoThemeAnimationWidget();
    void startAnimation(int msec);
Q_SIGNALS:
    Q_SIGNAL void animationFinished();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // TAOTHEMEANIMATIONWIDGET_H
