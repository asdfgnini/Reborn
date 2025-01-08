#include "./inc/TaoIcon.h"

#include <QPainter>
#include <QPixmap>

Q_SINGLETON_CREATE_CPP(TaoIcon)
TaoIcon::TaoIcon()
{
}

TaoIcon::~TaoIcon()
{
}

QIcon TaoIcon::getTaoIcon(TaoIconType::IconName awesome)
{
    QFont iconFont = QFont("TaoAwesome");
    QPixmap pix(30, 30);
    pix.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    // painter.setPen(QColor("#1570A5"));
    // painter.setBrush(QColor("#1570A5"));
    iconFont.setPixelSize(25);
    painter.setFont(iconFont);
    painter.drawText(pix.rect(), Qt::AlignCenter, QChar((unsigned short)awesome));
    painter.end();
    return QIcon(pix);
}

QIcon TaoIcon::getTaoIcon(TaoIconType::IconName awesome, QColor iconColor)
{
    QFont iconFont = QFont("TaoAwesome");
    QPixmap pix(30, 30);
    pix.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(iconColor);
    // painter.setBrush(QColor("#1570A5"));
    iconFont.setPixelSize(25);
    painter.setFont(iconFont);
    painter.drawText(pix.rect(), Qt::AlignCenter, QChar((unsigned short)awesome));
    painter.end();
    return QIcon(pix);
}

QIcon TaoIcon::getTaoIcon(TaoIconType::IconName awesome, int pixelSize)
{
    QFont iconFont = QFont("TaoAwesome");
    QPixmap pix(pixelSize, pixelSize);
    pix.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    iconFont.setPixelSize(pixelSize);
    painter.setFont(iconFont);
    // 画图形字体
    painter.drawText(pix.rect(), Qt::AlignCenter, QChar((unsigned short)awesome));
    painter.end();
    return QIcon(pix);
}

QIcon TaoIcon::getTaoIcon(TaoIconType::IconName awesome, int pixelSize, QColor iconColor)
{
    QFont iconFont = QFont("TaoAwesome");
    QPixmap pix(pixelSize, pixelSize);
    pix.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(iconColor);
    iconFont.setPixelSize(pixelSize);
    painter.setFont(iconFont);
    // 画图形字体
    painter.drawText(pix.rect(), Qt::AlignCenter, QChar((unsigned short)awesome));
    painter.end();
    return QIcon(pix);
}

QIcon TaoIcon::getTaoIcon(TaoIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight)
{
    QFont iconFont = QFont("TaoAwesome");
    QPixmap pix(fixedWidth, fixedHeight);
    pix.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    iconFont.setPixelSize(pixelSize);
    painter.setFont(iconFont);
    // 画图形字体
    painter.drawText(pix.rect(), Qt::AlignCenter, QChar((unsigned short)awesome));
    painter.end();
    return QIcon(pix);
}

QIcon TaoIcon::getTaoIcon(TaoIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, QColor iconColor)
{
    QFont iconFont = QFont("TaoAwesome");
    QPixmap pix(fixedWidth, fixedHeight);
    pix.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(iconColor);
    iconFont.setPixelSize(pixelSize);
    painter.setFont(iconFont);
    // 画图形字体
    painter.drawText(pix.rect(), Qt::AlignCenter, QChar((unsigned short)awesome));
    painter.end();
    return QIcon(pix);
}
