#ifndef TAOSPINBOXPRIVATE_H
#define TAOSPINBOXPRIVATE_H

#include <QObject>

#include "../../include/stdafx.h"

class TaoSpinBox;
class TaoMenu;
class TaoSpinBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoSpinBox)
public:
    explicit TaoSpinBoxPrivate(QObject* parent = nullptr);
    ~TaoSpinBoxPrivate();

private:
    TaoMenu* _createStandardContextMenu();
};

#endif // TAOSPINBOXPRIVATE_H
