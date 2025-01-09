#ifndef TAOPLAINTEXTEDIT_H
#define TAOPLAINTEXTEDIT_H

#include <QPlainTextEdit>

#include "../../include/stdafx.h"

class TaoPlainTextEditPrivate;
class TAO_EXPORT TaoPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
    Q_Q_CREATE(TaoPlainTextEdit)
public:
    explicit TaoPlainTextEdit(QWidget* parent = nullptr);
    explicit TaoPlainTextEdit(const QString& text, QWidget* parent = nullptr);
    virtual ~TaoPlainTextEdit();

protected:
    virtual void focusInEvent(QFocusEvent* event) override;
    virtual void focusOutEvent(QFocusEvent* event) override;
    virtual void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // TAOPLAINTEXTEDIT_H
