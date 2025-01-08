#ifndef TAODOCKWIDGETTITLEBAR_H
#define TAODOCKWIDGETTITLEBAR_H

#include <QDockWidget>
#include <QLabel>
#include <QWidget>

#include "../../include/Def.h"
class TaoText;
class TaoIconButton;
class TaoDockWidget;
class TaoDockWidgetTitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TaoDockWidgetTitleBar(QWidget* parent = nullptr);
    ~TaoDockWidgetTitleBar();

    Q_SLOT void onFloatButtonClicked();
    Q_SLOT void onCloseButtonClicked();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    friend class TaoDockWidgetPrivate;
    TaoThemeType::ThemeMode _themeMode;
    TaoDockWidget* _dockWidget{nullptr};
    QLabel* _iconLabel{nullptr};
    TaoText* _titleLabel{nullptr};
    TaoIconButton* _floatButton{nullptr};
    TaoIconButton* _closeButton{nullptr};
    void _setVisibleFromFeatures(QDockWidget::DockWidgetFeatures features);
};

#endif // TAODOCKWIDGETTITLEBAR_H
