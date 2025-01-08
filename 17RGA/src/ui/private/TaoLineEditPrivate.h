#ifndef TAOLINEEDITPRIVATE_H
#define TAOLINEEDITPRIVATE_H

#include <QObject>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoEvent;
class TaoLineEdit;
class TaoLineEditPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoLineEdit)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(bool, IsClearButtonEnable)
    Q_PROPERTY_CREATE(qreal, ExpandMarkWidth)
public:
    explicit TaoLineEditPrivate(QObject* parent = nullptr);
    ~TaoLineEditPrivate();
    Q_INVOKABLE void onWMWindowClickedEvent(QVariantMap data);
    Q_SLOT void onThemeChanged(TaoThemeType::ThemeMode themeMode);

private:
    TaoThemeType::ThemeMode _themeMode;
    TaoEvent* _focusEvent{nullptr};
    qreal _textSpacing{0.5};
};

#endif // TAOLINEEDITPRIVATE_H
