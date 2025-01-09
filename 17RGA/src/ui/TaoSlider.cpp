#include "./inc/TaoSlider.h"

#include <QEvent>

#include "./inc/TaoSliderStyle.h"

TaoSlider::TaoSlider(QWidget* parent)
    : QSlider(parent)
{
    setOrientation(Qt::Horizontal);
    setStyle(new TaoSliderStyle(style()));
}

TaoSlider::TaoSlider(Qt::Orientation orientation, QWidget* parent)
    : QSlider(orientation, parent)
{
    setStyle(new TaoSliderStyle(style()));
}

TaoSlider::~TaoSlider()
{
}
