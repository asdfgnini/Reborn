#ifndef TAOPUSHBUTTON_H
#define TAOPUSHBUTTON_H

#include <QPushButton>

#include "../../include/stdafx.h"
class TaoPushButtonPrivate;
class TAO_EXPORT TaoPushButton : public QPushButton
{
    Q_OBJECT
    Q_Q_CREATE(TaoPushButton)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightPressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkPressColor)
public:
    explicit TaoPushButton(QWidget* parent = nullptr);
    explicit TaoPushButton(QString text, QWidget* parent = nullptr);
    ~TaoPushButton();

    void setLightTextColor(QColor color);
    QColor getLightTextColor() const;

    void setDarkTextColor(QColor color);
    QColor getDarkTextColor() const;

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // TaoPUSHBUTTON_H
