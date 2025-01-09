#ifndef TAOCHECKBOX_H
#define TAOCHECKBOX_H

#include <QCheckBox>

#include "../../include/stdafx.h"

class TAO_EXPORT TaoCheckBox : public QCheckBox
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, BorderRadius)
public:
    explicit TaoCheckBox(QWidget* parent = nullptr);
    explicit TaoCheckBox(const QString& text, QWidget* parent = nullptr);
    ~TaoCheckBox();
};

#endif // TAOCHECKBOX_H
