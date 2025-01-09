#include "./inc/TaoRadioButton.h"

#include "./inc/TaoRadioButtonStyle.h"
#include "./inc/TaoTheme.h"
#include "private/TaoRadioButtonPrivate.h"

TaoRadioButton::TaoRadioButton(QWidget* parent)
    : QRadioButton(parent), d_ptr(new TaoRadioButtonPrivate())
{
    Q_D(TaoRadioButton);
    d->q_ptr = this;
    setFixedHeight(20);
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
    setStyle(new TaoRadioButtonStyle(style()));
    d->onThemeChanged(tTheme->getThemeMode());
    connect(tTheme, &TaoTheme::themeModeChanged, d, &TaoRadioButtonPrivate::onThemeChanged);
}

TaoRadioButton::TaoRadioButton(const QString& text, QWidget* parent)
    : TaoRadioButton(parent)
{
    setText(text);
}

TaoRadioButton::~TaoRadioButton()
{
}
