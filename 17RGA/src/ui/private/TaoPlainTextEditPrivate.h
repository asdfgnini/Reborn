#ifndef TAOPLAINTEXTEDITPRIVATE_H
#define TAOPLAINTEXTEDITPRIVATE_H

#include <QObject>

#include "../../include/Def.h"
class TaoEvent;
class TaoPlainTextEdit;
class TaoPlainTextEditStyle;
class TaoPlainTextEditPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoPlainTextEdit)

public:
    explicit TaoPlainTextEditPrivate(QObject* parent = nullptr);
    ~TaoPlainTextEditPrivate();
    Q_INVOKABLE void onWMWindowClickedEvent(QVariantMap data);
    Q_SLOT void onThemeChanged(TaoThemeType::ThemeMode themeMode);

private:
    TaoThemeType::ThemeMode _themeMode;
    TaoPlainTextEditStyle* _style{nullptr};
    TaoEvent* _focusEvent{nullptr};
};

#endif // TAOPLAINTEXTEDITPRIVATE_H
