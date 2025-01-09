#ifndef TAOCOMBOBOX_H
#define TAOCOMBOBOX_H

#include <QComboBox>

#include "../../include/stdafx.h"

class TaoComboBoxPrivate;
class TAO_EXPORT TaoComboBox : public QComboBox
{
    Q_OBJECT
    Q_Q_CREATE(TaoComboBox);
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
    explicit TaoComboBox(QWidget* parent = nullptr);
    ~TaoComboBox();

protected:
    virtual void showPopup() override;
    virtual void hidePopup() override;
};

#endif // TAOCOMBOBOX_H
