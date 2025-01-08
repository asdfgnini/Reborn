#ifndef TAOTEXTPRIVATE_H
#define TAOTEXTPRIVATE_H

#include <QObject>

#include "../../include/Def.h"
#include "../../include/stdafx.h"
class TaoText;
class TaoTextPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoText)
public:
    explicit TaoTextPrivate(QObject* parent = nullptr);
    ~TaoTextPrivate();
    Q_SLOT void onThemeChanged(TaoThemeType::ThemeMode themeMode);

private:
    TaoThemeType::ThemeMode _themeMode;
    bool _isWrapAnywhere{false};
    TaoTextType::TextStyle _textStyle{TaoTextType::NoStyle};
    qreal _textSpacing{0.5};
};

#endif // TAOTEXTPRIVATE_H
