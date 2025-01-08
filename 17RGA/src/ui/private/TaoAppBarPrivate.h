#ifndef TAOAPPBARPRIVATE_H
#define TAOAPPBARPRIVATE_H
#include <QObject>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class QLabel;
class QScreen;
class QHBoxLayout;
class QVBoxLayout;
class TaoText;
class TaoAppBar;
class TaoIconButton;
class TaoToolButton;
class TaoAppBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoAppBar)
    Q_PROPERTY_CREATE_D(bool, IsStayTop)
    Q_PROPERTY_CREATE_D(bool, IsFixedSize)
    Q_PROPERTY_CREATE_D(bool, IsDefaultClosed)
    Q_PROPERTY_CREATE_D(bool, IsOnlyAllowMinAndClose)
    Q_PROPERTY_CREATE_D(int, AppBarHeight)
    Q_PROPERTY_CREATE_D(QWidget*, CustomWidget)
    Q_PROPERTY_CREATE_D(int, CustomWidgetMaximumWidth)
public:
    explicit TaoAppBarPrivate(QObject* parent = nullptr);
    ~TaoAppBarPrivate();
    Q_SLOT void onMinButtonClicked();
    Q_SLOT void onMaxButtonClicked();
    Q_SLOT void onCloseButtonClicked();
    Q_SLOT void onStayTopButtonClicked();

private:
    QHBoxLayout* _mainLayout{nullptr};
    QVBoxLayout* _iconLabelLayout{nullptr};
    QVBoxLayout* _titleLabelLayout{nullptr};
    TaoAppBarType::ButtonFlags _buttonFlags;
    TaoToolButton* _routeBackButton{nullptr};
    TaoToolButton* _navigationButton{nullptr};
    TaoToolButton* _themeChangeButton{nullptr};
    TaoToolButton* _stayTopButton{nullptr};
    TaoToolButton* _minButton{nullptr};
    TaoToolButton* _maxButton{nullptr};
    TaoIconButton* _closeButton{nullptr};
    QScreen* _lastScreen{nullptr};
    TaoText* _titleLabel{nullptr};
    QLabel* _iconLabel{nullptr};
    qint64 _currentWinID{0};
    int _lastMinTrackWidth{0};
    quint64 _clickTimer{0};
    int _edges{0};
    int _margins{8};
    bool _isHoverMaxButton{false};
    void _changeMaxButtonAwesome(bool isMaximized);
    void _showSystemMenu(QPoint point);
    void _updateCursor(int edges);
    bool _containsCursorToItem(QWidget* item);
    void _onThemeModeChange(TaoThemeType::ThemeMode themeMode);
    int _calculateMinimumWidth();
    QVBoxLayout* _createVLayout(QWidget* widget);
};

#endif // TAOAPPBARPRIVATE_H
