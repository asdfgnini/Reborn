#ifndef TAOICONBUTTON_H
#define TAOICONBUTTON_H

#include <QPushButton>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoIconButtonPrivate;
class TAO_EXPORT TaoIconButton : public QPushButton
{
    Q_OBJECT
    Q_Q_CREATE(TaoIconButton)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(qreal, Opacity);
    Q_PROPERTY_CREATE_Q_H(QColor, LightHoverColor);
    Q_PROPERTY_CREATE_Q_H(QColor, DarkHoverColor);
    Q_PROPERTY_CREATE_Q_H(QColor, LightIconColor);
    Q_PROPERTY_CREATE_Q_H(QColor, DarkIconColor);
    Q_PROPERTY_CREATE_Q_H(QColor, LightHoverIconColor);
    Q_PROPERTY_CREATE_Q_H(QColor, DarkHoverIconColor);
    Q_PROPERTY_CREATE_Q_H(bool, IsSelected);

public:
    TaoIconButton(QPixmap pix, QWidget* parent = nullptr);
    TaoIconButton(TaoIconType::IconName awesome, QWidget* parent = nullptr);
    TaoIconButton(TaoIconType::IconName awesome, int pixelSize, QWidget* parent = nullptr);
    TaoIconButton(TaoIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, QWidget* parent = nullptr);
    ~TaoIconButton();
    void setAwesome(TaoIconType::IconName awesome);
    TaoIconType::IconName getAwesome() const;

    void setPixmap(QPixmap pix);

protected:
    virtual bool event(QEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // TAOICONBUTTON_H
