#include "./inc/TaoRadioButtonStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "./inc/TaoTheme.h"
TaoRadioButtonStyle::TaoRadioButtonStyle(QStyle* style)
{
    _themeMode = tTheme->getThemeMode();
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

TaoRadioButtonStyle::~TaoRadioButtonStyle()
{
}

void TaoRadioButtonStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case PE_IndicatorRadioButton:
    {
        const QStyleOptionButton* bopt = qstyleoption_cast<const QStyleOptionButton*>(option);
        if (!bopt)
        {
            break;
        }
        QRect buttonRect = bopt->rect;
        buttonRect.adjust(1, 1, -1, -1);
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

        if (bopt->state & QStyle::State_Off)
        {
            painter->setPen(QPen(TaoThemeColor(_themeMode, BasicBorder), 1.5));
            if (bopt->state & QStyle::State_MouseOver)
            {
                painter->setBrush(TaoThemeColor(_themeMode, BasicHover));
            }
            else
            {
                painter->setBrush(TaoThemeColor(_themeMode, BasicBase));
            }
            painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), 8.5, 8.5);
        }
        else
        {
            painter->setPen(Qt::NoPen);
            // 外圆形
            painter->setBrush(TaoThemeColor(_themeMode, PrimaryNormal));
            painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), buttonRect.width() / 2, buttonRect.width() / 2);
            // 内圆形
            painter->setBrush(TaoThemeColor(_themeMode, BasicTextInvert));
            if (bopt->state & QStyle::State_Sunken)
            {
                if (bopt->state & QStyle::State_MouseOver)
                {
                    painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), buttonRect.width() / 4.5, buttonRect.width() / 4.5);
                }
            }
            else
            {
                if (bopt->state & QStyle::State_MouseOver)
                {
                    painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), buttonRect.width() / 3.5, buttonRect.width() / 3.5);
                }
                else
                {
                    painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), buttonRect.width() / 4, buttonRect.width() / 4);
                }
            }
        }
        painter->restore();
        return;
    }
    default:
    {
        break;
    }
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

int TaoRadioButtonStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch (metric)
    {
    case QStyle::PM_ExclusiveIndicatorWidth:
    {
        return 20;
    }
    case QStyle::PM_ExclusiveIndicatorHeight:
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
