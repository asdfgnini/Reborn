#ifndef TAOCENTRALSTACKEDWIDGET_H
#define TAOCENTRALSTACKEDWIDGET_H

#include <QStackedWidget>

#include "../../include/Def.h"

class TaoCentralStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit TaoCentralStackedWidget(QWidget* parent = nullptr);
    ~TaoCentralStackedWidget();
    Q_SLOT void onThemeModeChanged(TaoThemeType::ThemeMode themeMode);

    void setIsTransparent(bool isTransparent);
    bool getIsTransparent() const;

    void setIsHasRadius(bool isHasRadius);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    TaoThemeType::ThemeMode _themeMode;
    bool _isTransparent{false};
    bool _isHasRadius{true};
};

#endif // TAOCENTRALSTACKEDWIDGET_H
