#ifndef TAOSCROLLPAGE_H
#define TAOSCROLLPAGE_H

#include <QWidget>

#include "../../include/stdafx.h"

class TaoScrollArea;
class TaoText;
class QHBoxLayout;
class TaoScrollPagePrivate;
class TAO_EXPORT TaoScrollPage : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(TaoScrollPage)
    Q_PROPERTY_CREATE_Q_H(QWidget*, CustomWidget)
public:
    explicit TaoScrollPage(QWidget* parent = nullptr);
    ~TaoScrollPage();

    void addCentralWidget(QWidget* centralWidget, bool isWidgetResizeable = true, bool isVerticalGrabGesture = true, qreal mousePressEventDTAOy = 0.5);

    void navigation(int widgetIndex, bool isLogRoute = true);

    void setPageTitleSpacing(int spacing);
    int getPageTitleSpacing() const;
    void setTitleVisible(bool isVisible);
};

#endif // TAOSCROLLPAGE_H
