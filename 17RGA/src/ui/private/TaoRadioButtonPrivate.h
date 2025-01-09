#ifndef TAORADIOBUTTONPRIVATE_H
#define TAORADIOBUTTONPRIVATE_H
#include <QObject>

#include "../../include/Def.h"
class TaoRadioButton;
class TaoRadioButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoRadioButton)
public:
    explicit TaoRadioButtonPrivate(QObject* parent = nullptr);
    ~TaoRadioButtonPrivate();
    Q_SLOT void onThemeChanged(TaoThemeType::ThemeMode themeMode);
};

#endif // TAORADIOBUTTONPRIVATE_H
