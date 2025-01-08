#ifndef TAOLINEEDIT_H
#define TAOLINEEDIT_H

#include <QLineEdit>

#include "../../include/stdafx.h"

class TaoLineEditPrivate;
class TAO_EXPORT TaoLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_Q_CREATE(TaoLineEdit)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(bool, IsClearButtonEnable)
public:
    explicit TaoLineEdit(QWidget* parent = nullptr);
    ~TaoLineEdit();

Q_SIGNALS:
    Q_SIGNAL void focusIn(QString text);
    Q_SIGNAL void focusOut(QString text);
    Q_SIGNAL void wmFocusOut(QString text);

protected:
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // TAOLINEEDIT_H
