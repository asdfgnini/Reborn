#ifndef TAOTOOLBUTTON_H
#define TAOTOOLBUTTON_H

#include <QToolButton>

#include "../../include/Def.h"
class TaoMenu;
class TaoToolButtonPrivate;
class TAO_EXPORT TaoToolButton : public QToolButton
{
    Q_OBJECT
    Q_Q_CREATE(TaoToolButton)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius);
    Q_PROPERTY_CREATE_Q_H(bool, IsSelected);

public:
    explicit TaoToolButton(QWidget* parent = nullptr);
    ~TaoToolButton();

    void setIsTransparent(bool isTransparent);
    bool getIsTransparent() const;

    void setMenu(TaoMenu* menu);
    void setTaoIcon(TaoIconType::IconName icon);

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // TAOTOOLBUTTON_H
