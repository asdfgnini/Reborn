#ifndef TAOWINDOW_H
#define TAOWINDOW_H

#include <QMainWindow>

#include "../../include/Def.h"
#include "TaoAppBar.h"
#include "../../include/stdafx.h"

class TaoWindowPrivate;
class TAO_EXPORT TaoWindow : public QMainWindow
{
    Q_OBJECT
    Q_Q_CREATE(TaoWindow)
    Q_PROPERTY_CREATE_Q_H(bool, IsStayTop)
    Q_PROPERTY_CREATE_Q_H(bool, IsFixedSize)
    Q_PROPERTY_CREATE_Q_H(bool, IsDefaultClosed)
    Q_PROPERTY_CREATE_Q_H(int, AppBarHeight)
    Q_PROPERTY_CREATE_Q_H(int, CustomWidgetMaximumWidth)
    Q_PROPERTY_CREATE_Q_H(int, ThemeChangeTime)
    Q_PROPERTY_CREATE_Q_H(bool, IsCentralStackedWidgetTransparent)
    Q_TAKEOVER_NATIVEEVENT_H
public:
    explicit TaoWindow(QWidget* parent = nullptr);
    ~TaoWindow();

    void moveToCenter();

    void setCustomWidget(TaoAppBarType::CustomArea customArea, QWidget* customWidget);
    QWidget* getCustomWidget() const;

    void setWindowButtonFlag(TaoAppBarType::ButtonType buttonFlag, bool isEnable = true);
    void setWindowButtonFlags(TaoAppBarType::ButtonFlags buttonFlags);
    TaoAppBarType::ButtonFlags getWindowButtonFlags() const;

    void addCustomWidget(QWidget* widget);
    void removeCustomWidget(QWidget* widget);

    void closeWindow();
Q_SIGNALS:
    Q_SIGNAL void closeButtonClicked();
    Q_SIGNAL void customWidgetChanged();

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    virtual QMenu* createPopupMenu() override;
};

#endif // TAOWINDOW_H
