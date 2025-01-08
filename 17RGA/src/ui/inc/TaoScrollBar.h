#ifndef TAOSCROLLBAR_H
#define TAOSCROLLBAR_H

#include <QAbstractScrollArea>
#include <QScrollBar>

#include "../../include/stdafx.h"

class TaoScrollBarPrivate;
class TAO_EXPORT TaoScrollBar : public QScrollBar
{
    Q_OBJECT
    Q_Q_CREATE(TaoScrollBar)
    Q_PROPERTY_CREATE_Q_H(bool, IsAnimation)
    Q_PROPERTY_CREATE_Q_H(qreal, SpeedLimit)
public:
    explicit TaoScrollBar(QWidget* parent = nullptr);
    explicit TaoScrollBar(Qt::Orientation orientation, QWidget* parent = nullptr);
    explicit TaoScrollBar(QScrollBar* originScrollBar, QAbstractScrollArea* parent = nullptr);
    ~TaoScrollBar();

Q_SIGNALS:
    Q_SIGNAL void rangeAnimationFinished();

protected:
    virtual bool event(QEvent* event) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // TAOSCROLLBAR_H
