#ifndef TAOTOOLBARPRIVATE_H
#define TAOTOOLBARPRIVATE_H
#include <QObject>

#include "../../include/Def.h"

class TaoToolBar;
class TaoToolBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoToolBar)
public:
    explicit TaoToolBarPrivate(QObject* parent = nullptr);
    ~TaoToolBarPrivate();

private:
    TaoThemeType::ThemeMode _themeMode;
    int _shadowBorderWidth{6};
};

#endif // TAOTOOLBARPRIVATE_H
