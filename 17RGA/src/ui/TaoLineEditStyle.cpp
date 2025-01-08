#include "./inc/TaoLineEditStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QtMath>

#include "./inc/TaoTheme.h"

TaoLineEditStyle::TaoLineEditStyle(QStyle* style)
{
    _themeMode = tTheme->getThemeMode();
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

TaoLineEditStyle::~TaoLineEditStyle()
{
}

void TaoLineEditStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case PE_PanelLineEdit:
    {
        if (const QStyleOptionFrame* fopt = qstyleoption_cast<const QStyleOptionFrame*>(option))
        {
            QRect lineEditRect = fopt->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing);
            painter->setPen(Qt::NoPen);
            // 边框绘制
            painter->setBrush(TaoThemeColor(_themeMode, BasicBorder));
            painter->drawRoundedRect(lineEditRect, 6, 6);
            //  背景绘制
            if (fopt->state & QStyle::State_HasFocus)
            {
                painter->setBrush(TaoThemeColor(_themeMode, DialogBase));
            }
            else if (fopt->state & QStyle::State_MouseOver)
            {
                painter->setBrush(TaoThemeColor(_themeMode, BasicHover));
            }
            else
            {
                painter->setBrush(TaoThemeColor(_themeMode, BasicBase));
            }
            painter->drawRoundedRect(QRectF(lineEditRect.x() + 1.5, lineEditRect.y() + 1.5, lineEditRect.width() - 3, lineEditRect.height() - 3), 6, 6);

            // 底边线绘制
            painter->setBrush(TaoThemeColor(_themeMode, BasicHemline));
            QPainterPath path;
            path.moveTo(6, lineEditRect.height());
            path.lineTo(lineEditRect.width() - 6, lineEditRect.height());
            path.arcTo(QRectF(lineEditRect.width() - 12, lineEditRect.height() - 12, 12, 12), -90, 45);
            path.lineTo(6 - 3 * std::sqrt(2), lineEditRect.height() - (6 - 3 * std::sqrt(2)));
            path.arcTo(QRectF(0, lineEditRect.height() - 12, 12, 12), 270, 45);
            path.closeSubpath();
            painter->drawPath(path);
            painter->restore();
        }
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
