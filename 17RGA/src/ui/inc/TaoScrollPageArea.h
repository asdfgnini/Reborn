#ifndef TAOSCROLLPAGEAREA_H
#define TAOSCROLLPAGEAREA_H

#include <QWidget>

#include "../../include/stdafx.h"

class TaoScrollPageAreaPrivate;
class TAO_EXPORT TaoScrollPageArea : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(TaoScrollPageArea)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
    explicit TaoScrollPageArea(QWidget* parent = nullptr);
    ~TaoScrollPageArea();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // TAOSCROLLPAGEAREA_H
