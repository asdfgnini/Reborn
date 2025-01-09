#ifndef TAOMESSAGEBUTTON_H
#define TAOMESSAGEBUTTON_H

#include <QPushButton>
#include <QWidget>

#include "../../include/Def.h"
#include "../../include/stdafx.h"
class TaoMessageButtonPrivate;
class TAO_EXPORT TaoMessageButton : public QPushButton
{
    Q_OBJECT
    Q_Q_CREATE(TaoMessageButton)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QString, BarTitle);
    Q_PROPERTY_CREATE_Q_H(QString, BarText);
    Q_PROPERTY_CREATE_Q_H(int, DisplayMsec);
    Q_PROPERTY_CREATE_Q_H(QWidget*, MessageTargetWidget)
    Q_PROPERTY_CREATE_Q_H(TaoMessageBarType::MessageMode, MessageMode);
    Q_PROPERTY_CREATE_Q_H(TaoMessageBarType::PositionPolicy, PositionPolicy);

public:
    explicit TaoMessageButton(QWidget* parent = nullptr);
    explicit TaoMessageButton(QString text, QWidget* parent = nullptr);
    ~TaoMessageButton();

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // TAOMESSAGEBUTTON_H
