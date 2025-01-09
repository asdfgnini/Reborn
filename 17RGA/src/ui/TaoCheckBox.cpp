#include "./inc/TaoCheckBox.h"

#include "./inc/TaoCheckBoxStyle.h"

TaoCheckBox::TaoCheckBox(QWidget* parent)
    : QCheckBox(parent)
{
    _pBorderRadius = 3;
    setMouseTracking(true);
    setObjectName("TaoCheckBox");
    setStyle(new TaoCheckBoxStyle(style()));
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
}

TaoCheckBox::TaoCheckBox(const QString& text, QWidget* parent)
    : TaoCheckBox(parent)
{
    setText(text);
}

TaoCheckBox::~TaoCheckBox()
{
}
