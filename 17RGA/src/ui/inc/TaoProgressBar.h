#ifndef TAOPROGRESSBAR_H
#define TAOPROGRESSBAR_H

#include <QProgressBar>

#include "../../include/stdafx.h"

class TaoProgressBarPrivate;
class TAO_EXPORT TaoProgressBar : public QProgressBar
{
    Q_OBJECT
    Q_Q_CREATE(TaoProgressBar)
public:
    explicit TaoProgressBar(QWidget* parent = nullptr);
    ~TaoProgressBar();
    void setMinimum(int minimum);
    void setMaximum(int maximum);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
};

#endif // TAOPROGRESSBAR_H
