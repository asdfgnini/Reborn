#ifndef TAOTOOLBUTTONPRIVATE_H
#define TAOTOOLBUTTONPRIVATE_H

#include <QObject>

#include "../../include/stdafx.h"

class TaoToolButton;
class TaoToolButtonStyle;
class TaoToolButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoToolButton)
public:
    explicit TaoToolButtonPrivate(QObject* parent = nullptr);
    ~TaoToolButtonPrivate();

private:
    TaoToolButtonStyle* _toolButtonStyle{nullptr};
};

#endif // TAOTOOLBUTTONPRIVATE_H
