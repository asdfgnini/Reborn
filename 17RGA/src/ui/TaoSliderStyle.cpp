#include "./inc/TaoSliderStyle.h"

#include <QPainter>
#include <QPropertyAnimation>
#include <QStyleOptionSlider>

#include "./inc/TaoTheme.h"

TaoSliderStyle::TaoSliderStyle(QStyle* style)
{
    setProperty("circleRadius", 0.01);
    _themeMode = tTheme->getThemeMode();
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

TaoSliderStyle::~TaoSliderStyle()
{
}

void TaoSliderStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    switch (control)
    {
    case QStyle::CC_Slider:
    {
        const QStyleOptionSlider* sopt = qstyleoption_cast<const QStyleOptionSlider*>(option);
        if (!sopt)
        {
            break;
        }

        painter->save();
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        QRect sliderRect = sopt->rect;
        QRect sliderHandleRect = subControlRect(control, sopt, SC_SliderHandle, widget);
        sliderHandleRect.adjust(1, 1, -1, -1);
        // 滑槽
        painter->setPen(Qt::NoPen);
        painter->setBrush(TaoThemeColor(_themeMode, BasicChute));
        if (sopt->orientation == Qt::Horizontal)
        {
            // 未滑过
            painter->drawRoundedRect(QRect(sliderRect.x() + sliderRect.height() / 8, sliderRect.y() + sliderRect.height() * 0.375, sliderRect.width() - sliderRect.height() / 4, sliderRect.height() / 4), sliderRect.height() / 8, sliderRect.height() / 8);
            // 已滑过
            painter->setBrush(TaoThemeColor(_themeMode, PrimaryNormal));
            painter->drawRoundedRect(QRect(sliderRect.x() + sliderRect.height() / 8, sliderRect.y() + sliderRect.height() * 0.375, sliderHandleRect.x(), sliderRect.height() / 4), sliderRect.height() / 8, sliderRect.height() / 8);
        }
        else
        {
            // 未滑过
            painter->drawRoundedRect(QRect(sliderRect.x() + sliderRect.width() * 0.375, sliderRect.y() + sliderRect.width() / 8, sliderRect.width() / 4, sliderRect.height() - sliderRect.width() / 4), sliderRect.width() / 8, sliderRect.width() / 8);
            // 已滑过
            painter->setBrush(TaoThemeColor(_themeMode, PrimaryNormal));
            painter->drawRoundedRect(QRect(sliderRect.x() + sliderRect.width() * 0.375, sliderHandleRect.y(), sliderRect.width() / 4, sliderRect.height() - sliderRect.width() / 8 - sliderHandleRect.y()), sliderRect.width() / 8, sliderRect.width() / 8);
        }
        // 滑块
        // 外圆形
        painter->setPen(TaoThemeColor(_themeMode, BasicBorder));
        painter->setBrush(TaoThemeColor(_themeMode, BasicBase));
        painter->drawEllipse(QPointF(sliderHandleRect.center().x() + 1, sliderHandleRect.center().y() + 1), sliderHandleRect.width() / 2, sliderHandleRect.width() / 2);
        // 内圆形
        painter->setPen(Qt::NoPen);
        painter->setBrush(TaoThemeColor(_themeMode, PrimaryNormal));
        if (_lastState == 0)
        {
            _lastState = sopt->state;
        }
        if (_circleRadius == 0)
        {
            _circleRadius = sliderHandleRect.width() / 3.8;
        }
        if (sopt->activeSubControls == SC_SliderHandle)
        {
            if (sopt->state & QStyle::State_Sunken)
            {
                if (sopt->state & QStyle::State_MouseOver)
                {
                    if (!_lastState.testFlag(QStyle::State_Sunken))
                    {
                        _startRadiusAnimation(sliderHandleRect.width() / 2.8, sliderHandleRect.width() / 4.5, const_cast<QWidget*>(widget));
                        _lastState = sopt->state;
                    }
                    painter->drawEllipse(QPointF(sliderHandleRect.center().x() + 1, sliderHandleRect.center().y() + 1), _circleRadius, _circleRadius);
                }
            }
            else
            {
                if (sopt->state & QStyle::State_MouseOver)
                {
                    if (!_lastState.testFlag(QStyle::State_MouseOver))
                    {
                        _startRadiusAnimation(_circleRadius, sliderHandleRect.width() / 2.8, const_cast<QWidget*>(widget));
                        _lastState = sopt->state;
                    }
                    if (_lastState.testFlag(QStyle::State_Sunken))
                    {
                        _startRadiusAnimation(_circleRadius, sliderHandleRect.width() / 2.8, const_cast<QWidget*>(widget));
                        _lastState = sopt->state;
                    }
                    painter->drawEllipse(QPointF(sliderHandleRect.center().x() + 1, sliderHandleRect.center().y() + 1), _circleRadius, _circleRadius);
                }
            }
        }
        else
        {
            if (_lastState.testFlag(QStyle::State_MouseOver) || _lastState.testFlag(QStyle::State_Sunken))
            {
                _startRadiusAnimation(_circleRadius, sliderHandleRect.width() / 3.8, const_cast<QWidget*>(widget));
                _lastState &= ~QStyle::State_MouseOver;
                _lastState &= ~QStyle::State_Sunken;
            }
            painter->drawEllipse(QPointF(sliderHandleRect.center().x() + 1, sliderHandleRect.center().y() + 1), _circleRadius, _circleRadius);
        }
        painter->restore();
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

int TaoSliderStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch (metric)
    {
    case QStyle::PM_SliderLength:
    {
        return 20;
    }
    case QStyle::PM_SliderThickness:
    {
        return 20;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}

int TaoSliderStyle::styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn* returnData) const
{
    if (hint == QStyle::SH_Slider_AbsoluteSetButtons)
    {
        return Qt::LeftButton;
    }
    return QProxyStyle::styleHint(hint, option, widget, returnData);
}

void TaoSliderStyle::_startRadiusAnimation(qreal startRadius, qreal endRadius, QWidget* widget) const
{
    TaoSliderStyle* style = const_cast<TaoSliderStyle*>(this);
    QPropertyAnimation* circleRadiusAnimation = new QPropertyAnimation(style, "circleRadius");
    connect(circleRadiusAnimation, &QPropertyAnimation::valueChanged, style, [=](const QVariant& value) {
                this->_circleRadius = value.toReal();
                widget->update(); });
    circleRadiusAnimation->setEasingCurve(QEasingCurve::InOutSine);
    circleRadiusAnimation->setStartValue(startRadius);
    circleRadiusAnimation->setEndValue(endRadius);
    circleRadiusAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
