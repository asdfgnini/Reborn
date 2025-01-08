#ifndef TAOAPPLICATIONPRIVATE_H
#define TAOAPPLICATIONPRIVATE_H

#include <QColor>
#include <QIcon>
#include <QObject>

#include "../../include/Def.h"

class TaoApplication;
class TaoApplicationPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoApplication)
    Q_PROPERTY_CREATE_D(bool, IsEnableMica)
    Q_PROPERTY_CREATE_D(QString, MicaImagePath)
public:
    explicit TaoApplicationPrivate(QObject* parent = nullptr);
    ~TaoApplicationPrivate();
    Q_SLOT void onThemeModeChanged(TaoThemeType::ThemeMode themeMode);
Q_SIGNALS:
    Q_SIGNAL void initMicaBase(QImage img);

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    friend class TaoMicaBaseInitObject;
    TaoThemeType::ThemeMode _themeMode;
    QList<QWidget*> _micaWidgetList;
    QImage _lightBaseImage;
    QImage _darkBaseImage;
    void _initMicaBaseImage(QImage img);
    QRect _calculateWindowVirtualGeometry(QWidget* widget);
    void _updateMica(QWidget* widget, bool isProcessEvent = true);
    void _updateAllMicaWidget();
};

#endif // TAOAPPLICATIONPRIVATE_H
