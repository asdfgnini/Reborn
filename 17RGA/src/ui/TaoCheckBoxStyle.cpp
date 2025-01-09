#include "./inc/TaoCheckBoxStyle.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>

#include "./inc/TaoTheme.h"
TaoCheckBoxStyle::TaoCheckBoxStyle(QStyle* style)
{
    _pCheckIndicatorWidth = 21;
    _themeMode = tTheme->getThemeMode();
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

TaoCheckBoxStyle::~TaoCheckBoxStyle()
{
}

void TaoCheckBoxStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    // qDebug() << element << option->rect;
    switch (element)
    {
    case QStyle::CE_CheckBox:
    {
        if (const QStyleOptionButton* bopt = qstyleoption_cast<const QStyleOptionButton*>(option))
        {
            bool isEnabled = bopt->state.testFlag(QStyle::State_Enabled);
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
            QRect checkBoxRect = bopt->rect;
            QRect checkRect(checkBoxRect.x(), checkBoxRect.y(), _pCheckIndicatorWidth, _pCheckIndicatorWidth);
            checkRect.adjust(1, 1, -1, -1);
            //复选框绘制
            painter->setPen(Qt::NoPen);
            if (bopt->state.testFlag(QStyle::State_On) || bopt->state.testFlag(QStyle::State_NoChange))
            {
                painter->setPen(Qt::NoPen);
                if (bopt->state.testFlag(QStyle::State_Sunken))
                {
                    painter->setBrush(TaoThemeColor(_themeMode, PrimaryPress));
                }
                else
                {
                    if (bopt->state.testFlag(QStyle::State_MouseOver))
                    {
                        painter->setBrush(TaoThemeColor(_themeMode, PrimaryHover));
                    }
                    else
                    {
                        painter->setBrush(TaoThemeColor(_themeMode, PrimaryNormal));
                    }
                }
            }
            else
            {
                if (bopt->state.testFlag(QStyle::State_Sunken))
                {
                    painter->setPen(TaoThemeColor(_themeMode, BasicBorderDeep));
                }
                else
                {
                    painter->setPen(TaoThemeColor(_themeMode, BasicBorderDeep));
                    if (bopt->state.testFlag(QStyle::State_MouseOver))
                    {
                        painter->setBrush(TaoThemeColor(_themeMode, BasicHover));
                    }
                    else
                    {
                        painter->setBrush(TaoThemeColor(_themeMode, BasicBase));
                    }
                }
            }
            painter->drawRoundedRect(checkRect, 2, 2);
            //图标绘制
            painter->setPen(TaoThemeColor(TaoThemeType::Dark, BasicText));
            if (bopt->state.testFlag(QStyle::State_On))
            {
                painter->save();
                QFont iconFont = QFont("TaoAwesome");
                iconFont.setPixelSize(_pCheckIndicatorWidth * 0.75);
                painter->setFont(iconFont);
                painter->drawText(checkRect, Qt::AlignCenter, QChar((unsigned short)TaoIconType::Check));
                painter->restore();
            }
            else if (bopt->state.testFlag(QStyle::State_NoChange))
            {
                QLine checkLine(checkRect.x() + 3, checkRect.center().y(), checkRect.right() - 3, checkRect.center().y());
                painter->drawLine(checkLine);
            }
            //文字绘制
            painter->setPen(isEnabled ? TaoThemeColor(_themeMode, BasicText) : TaoThemeColor(_themeMode, BasicTextDisable));
            QRect textRect(checkRect.right() + 10, checkBoxRect.y(), checkBoxRect.width(), checkBoxRect.height());
            painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, bopt->text);
            painter->restore();
        }
        return;
    }
    default:
    {
        break;
    }
    }

    QProxyStyle::drawControl(element, option, painter, widget);
}

int TaoCheckBoxStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    // qDebug() << metric << QProxyStyle::pixelMetric(metric, option, widget);
    switch (metric)
    {
    case QStyle::PM_IndicatorWidth:
    {
        return _pCheckIndicatorWidth;
    }
    case QStyle::PM_IndicatorHeight:
    {
        return _pCheckIndicatorWidth;
    }
    case QStyle::PM_CheckBoxLabelSpacing:
    {
        return 10;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}
