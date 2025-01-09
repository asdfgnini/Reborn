#ifndef TAOMESSAGEBAR_H
#define TAOMESSAGEBAR_H

#include <QWidget>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoMessageBarPrivate;
class TAO_EXPORT TaoMessageBar : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(TaoMessageBar)

public:
    static void success(TaoMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent = nullptr);
    static void warning(TaoMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent = nullptr);
    static void information(TaoMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent = nullptr);
    static void error(TaoMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    friend class TaoMessageBarManager;
    explicit TaoMessageBar(TaoMessageBarType::PositionPolicy policy, TaoMessageBarType::MessageMode messageMode, QString& title, QString& text, int displayMsec, QWidget* parent = nullptr);
    ~TaoMessageBar();
};

#endif // TAOMESSAGEBAR_H
