#ifndef TAORADIOBUTTON_H
#define TAORADIOBUTTON_H

#include <QRadioButton>

#include "../../include/stdafx.h"

class TaoRadioButtonPrivate;
class TAO_EXPORT TaoRadioButton : public QRadioButton
{
    Q_OBJECT
    Q_Q_CREATE(TaoRadioButton)
public:
    explicit TaoRadioButton(QWidget* parent = nullptr);
    explicit TaoRadioButton(const QString& text, QWidget* parent = nullptr);
    ~TaoRadioButton();
};

#endif // TAORADIOBUTTON_H
