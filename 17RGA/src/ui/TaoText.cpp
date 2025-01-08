#include "./inc/TaoText.h"

#include <QContextMenuEvent>
#include <QPainter>

#include "./inc/TaoTheme.h"
#include "private/TaoTextPrivate.h"

TaoText::TaoText(QWidget* parent)
    : QLabel(parent), d_ptr(new TaoTextPrivate())
{
    Q_D(TaoText);
    d->q_ptr = this;
    setObjectName("TaoText");
    setStyleSheet("#TaoText{background-color:transparent;}");
    QFont textFont = font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, d->_textSpacing);
    textFont.setPixelSize(28);
    setFont(textFont);
    setWordWrap(true);
    d->_themeMode = tTheme->getThemeMode();
    d->onThemeChanged(tTheme->getThemeMode());
    connect(tTheme, &TaoTheme::themeModeChanged, d, &TaoTextPrivate::onThemeChanged);
}

TaoText::TaoText(QString text, QWidget* parent)
    : TaoText(parent)
{
    setText(text);
}

TaoText::TaoText(QString text, int pixelSize, QWidget* parent)
    : TaoText(text, parent)
{
    QFont font = this->font();
    font.setPixelSize(pixelSize);
    setFont(font);
}

TaoText::~TaoText()
{
}

void TaoText::setIsWrapAnywhere(bool isWrapAnywhere)
{
    Q_D(TaoText);
    setWordWrap(isWrapAnywhere);
    d->_isWrapAnywhere = isWrapAnywhere;
}

bool TaoText::getIsWrapAnywhere() const
{
    Q_D(const TaoText);
    return d->_isWrapAnywhere;
}

void TaoText::setTextPixelSize(int size)
{
    QFont font = this->font();
    font.setPixelSize(size);
    setFont(font);
}

int TaoText::getTextPixelSize() const
{
    return this->font().pixelSize();
}

void TaoText::setTextPointSize(int size)
{
    QFont font = this->font();
    font.setPointSize(size);
    setFont(font);
}

int TaoText::getTextPointSize() const
{
    return this->font().pointSize();
}

void TaoText::setTextStyle(TaoTextType::TextStyle textStyle)
{
    Q_D(TaoText);
    QFont textFont = font();
    d->_textStyle = textStyle;
    switch (textStyle)
    {
    case TaoTextType::NoStyle:
    {
        break;
    }
    case TaoTextType::Caption:
    {
        textFont.setPixelSize(12);
        break;
    }
    case TaoTextType::Body:
    {
        textFont.setPixelSize(13);
        break;
    }
    case TaoTextType::BodyStrong:
    {
        textFont.setPixelSize(13);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case TaoTextType::Subtitle:
    {
        textFont.setPixelSize(20);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case TaoTextType::Title:
    {
        textFont.setPixelSize(28);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case TaoTextType::TitleLarge:
    {
        textFont.setPixelSize(40);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case TaoTextType::Display:
    {
        textFont.setPixelSize(48);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    }
    setFont(textFont);
}

TaoTextType::TextStyle TaoText::getTextStyle() const
{
    Q_D(const TaoText);
    return d->_textStyle;
}

void TaoText::paintEvent(QPaintEvent* event)
{
    Q_D(TaoText);
    if (wordWrap() && d->_isWrapAnywhere)
    {
        Q_D(TaoText);
        QPainter painter(this);
        painter.save();
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        painter.setPen(TaoThemeColor(d->_themeMode, BasicText));
        painter.drawText(rect(), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap | Qt::TextWrapAnywhere, text());
        painter.restore();
    }
    else
    {
        QLabel::paintEvent(event);
    }
}
