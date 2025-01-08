#ifndef TAOTOGGLESWITCH_H
#define TAOTOGGLESWITCH_H

#include <QWidget>

#include "../../include/stdafx.h"
class TaoToggleSwitchPrivate;
class TAO_EXPORT TaoToggleSwitch : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(TaoToggleSwitch);

public:
    explicit TaoToggleSwitch(QWidget* parent = nullptr);
    ~TaoToggleSwitch();
    void setIsToggled(bool isToggled);
    bool getIsToggled() const;
Q_SIGNALS:
    Q_SIGNAL void toggled(bool checked);

protected:
    virtual bool event(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // TAOTOGGLESWITCH_H
