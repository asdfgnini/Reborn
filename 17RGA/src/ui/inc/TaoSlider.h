#ifndef TAOSLIDER_H
#define TAOSLIDER_H

#include <QSlider>

#include "../../include/stdafx.h"
class TAO_EXPORT TaoSlider : public QSlider
{
    Q_OBJECT
public:
    explicit TaoSlider(QWidget* parent = nullptr);
    explicit TaoSlider(Qt::Orientation orientation, QWidget* parent = nullptr);
    ~TaoSlider();
};

#endif // TAOSLIDER_H
