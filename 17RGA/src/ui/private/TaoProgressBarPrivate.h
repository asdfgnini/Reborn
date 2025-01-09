#ifndef TAOPROGRESSBARPRIVATE_H
#define TAOPROGRESSBARPRIVATE_H

#include <QObject>

#include "../../include/stdafx.h"

class TaoProgressBarStyle;
class QPropertyAnimation;
class TaoProgressBar;
class TaoProgressBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoProgressBar)
public:
    explicit TaoProgressBarPrivate(QObject* parent = nullptr);
    ~TaoProgressBarPrivate();

private:
    bool _isBusyAnimation{false};
    TaoProgressBarStyle* _style{nullptr};
    QPropertyAnimation* _busyAnimation{nullptr};
};

#endif // TAOPROGRESSBARPRIVATE_H
