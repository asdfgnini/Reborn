#ifndef TAONAVIGATIONSTYLE_H
#define TAONAVIGATIONSTYLE_H
#include <QModelIndex>
#include <QProxyStyle>

#include "../../include/Def.h"

class TaoNavigationNode;
class TaoNavigationView;
class QPropertyAnimation;
class TaoNavigationStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY_CREATE(qreal, Opacity)
    Q_PROPERTY_CREATE(qreal, Rotate)
    Q_PROPERTY_CREATE(int, ItemHeight)
    Q_PRIVATE_CREATE(TaoNavigationView*, NavigationView)
    Q_PROPERTY_CREATE(qreal, LastSelectMarkTop)
    Q_PROPERTY_CREATE(qreal, LastSelectMarkBottom)
    Q_PROPERTY_CREATE(qreal, SelectMarkTop)
    Q_PROPERTY_CREATE(qreal, SelectMarkBottom)
    Q_PRIVATE_CREATE(QModelIndex, PressIndex)
public:
    explicit TaoNavigationStyle(QStyle* style = nullptr);
    ~TaoNavigationStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;

    void navigationNodeStateChange(QVariantMap data);

private:
    TaoThemeType::ThemeMode _themeMode;
    bool _isSelectMarkDisplay{true};
    int _leftPadding{11};
    int _iconAreaWidth{40};
    int _textRightSpacing{3};
    int _indicatorIconAreaWidth{34};
    TaoNavigationNode* _opacityAnimationTargetNode{nullptr};
    TaoNavigationNode* _expandAnimationTargetNode{nullptr};
    TaoNavigationNode* _lastSelectedNode{nullptr};
    QPropertyAnimation* _lastSelectMarkTopAnimation{nullptr};
    QPropertyAnimation* _lastSelectMarkBottomAnimation{nullptr};
    QPropertyAnimation* _selectMarkTopAnimation{nullptr};
    QPropertyAnimation* _selectMarkBottomAnimation{nullptr};
    bool _compareItemY(TaoNavigationNode* node1, TaoNavigationNode* node2);
};

#endif // TAONAVIGATIONSTYLE_H
