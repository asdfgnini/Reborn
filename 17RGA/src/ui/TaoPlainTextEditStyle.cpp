#include "./inc/TaoPlainTextEditStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QtMath>

#include "./inc/TaoTheme.h"
TaoPlainTextEditStyle::TaoPlainTextEditStyle(QStyle* style)
{
    _pExpandMarkWidth = 0;
    _themeMode = tTheme->getThemeMode();
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

TaoPlainTextEditStyle::~TaoPlainTextEditStyle()
{
}

void TaoPlainTextEditStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_ShapedFrame:
    {
        if (const QStyleOptionFrame* fopt = qstyleoption_cast<const QStyleOptionFrame*>(option))
        {
            //背景绘制
            QRect editRect = option->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing);
            painter->setPen(Qt::NoPen);
            // 边框绘制
            painter->setBrush(TaoThemeColor(_themeMode, BasicBorder));
            painter->drawRoundedRect(editRect, 6, 6);

            painter->setBrush(TaoThemeColor(_themeMode, BasicBase));
            painter->drawRoundedRect(QRectF(editRect.x() + 1.5, editRect.y() + 1.5, editRect.width() - 3, editRect.height() - 3), 6, 6);

            // 底边线绘制
            painter->setBrush(TaoThemeColor(_themeMode, BasicHemline));
            QPainterPath path;
            path.moveTo(6, editRect.height());
            path.lineTo(editRect.width() - 6, editRect.height());
            path.arcTo(QRectF(editRect.width() - 12, editRect.height() - 12, 12, 12), -90, 45);
            path.lineTo(6 - 3 * std::sqrt(2), editRect.height() - (6 - 3 * std::sqrt(2)));
            path.arcTo(QRectF(0, editRect.height() - 12, 12, 12), 270, 45);
            path.closeSubpath();
            painter->drawPath(path);

            //焦点指示器
            painter->setPen(Qt::NoPen);
            painter->setBrush(TaoThemeColor(_themeMode, PrimaryNormal));
            painter->drawRoundedRect(QRectF(editRect.width() / 2 - _pExpandMarkWidth, editRect.height() - 2.5, _pExpandMarkWidth * 2, 2.5), 2, 2);

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
