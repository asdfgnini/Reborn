#ifndef TAOSPINBOX_H
#define TAOSPINBOX_H

#include <QSpinBox>

#include "../../include/stdafx.h"

class TaoSpinBoxPrivate;
class TAO_EXPORT TaoSpinBox : public QSpinBox
{
    Q_OBJECT
    Q_Q_CREATE(TaoSpinBox)
public:
    explicit TaoSpinBox(QWidget* parent = nullptr);
    ~TaoSpinBox();

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // TAOSPINBOX_H
