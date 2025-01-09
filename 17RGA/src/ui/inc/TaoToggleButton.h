#ifndef TAOTOGGLEBUTTON_H
#define TAOTOGGLEBUTTON_H

#include <QPushButton>

#include "../../include/stdafx.h"

class TaoToggleButtonPrivate;
class TAO_EXPORT TaoToggleButton : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(TaoToggleButton)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QString, Text)
public:
    explicit TaoToggleButton(QWidget* parent = nullptr);
    explicit TaoToggleButton(QString text, QWidget* parent = nullptr);
    ~TaoToggleButton();

    void setIsToggled(bool isToggled);
    bool getIsToggled() const;
Q_SIGNALS:
    Q_SIGNAL void toggled(bool checked);

protected:
    virtual bool event(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // TAOTOGGLEBUTTON_H
