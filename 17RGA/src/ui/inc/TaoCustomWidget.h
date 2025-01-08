#ifndef TAOCUSTOMWIDGET_H
#define TAOCUSTOMWIDGET_H

#include <QDialog>

#include "../../include/Def.h"
#include "TaoAppBar.h"

class QVBoxLayout;
class TaoCustomWidget : public QDialog
{
    Q_OBJECT
    Q_TAKEOVER_NATIVEEVENT_H
public:
    explicit TaoCustomWidget(QWidget* parent = nullptr);
    ~TaoCustomWidget();

    void setCentralWidget(QWidget* widget);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    QVBoxLayout* _mainLayout{nullptr};
    TaoAppBar* _appBar{nullptr};
    QWidget* _centralWidget{nullptr};

private:
    TaoThemeType::ThemeMode _themeMode;
    bool _isEnableMica;
};

#endif // TAOCUSTOMWIDGET_H
