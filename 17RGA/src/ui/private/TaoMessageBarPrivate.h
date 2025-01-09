#ifndef TAOMESSAGEBARPRIVATE_H
#define TAOMESSAGEBARPRIVATE_H

#include <QMap>
#include <QObject>
#include <QQueue>
#include <QVariantMap>

#include "../../include/Def.h"
#include "../../include/singleton.h"
#include "../../include/stdafx.h"

enum WorkStatus
{
    Idle = 0x0000,
    CreateAnimation = 0x0001,
    OtherEventAnimation = 0x0002,
};

class TaoMessageBar;
class TaoMessageBarManager : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(TaoMessageBarManager)
private:
    explicit TaoMessageBarManager(QObject* parent = nullptr);
    ~TaoMessageBarManager();

public:
    //请求事件堆栈调用
    void requestMessageBarEvent(TaoMessageBar* messageBar);
    //发布创建事件
    void postMessageBarCreateEvent(TaoMessageBar* messageBar);
    //发布终止事件
    void postMessageBarEndEvent(TaoMessageBar* messageBar);
    //强制发布终止事件
    void forcePostMessageBarEndEvent(TaoMessageBar* messageBar);
    //获取当前事件数量
    int getMessageBarEventCount(TaoMessageBar* messageBar);
    //更新活动序列
    void updateActiveMap(TaoMessageBar* messageBar, bool isActive);

private:
    QMap<TaoMessageBar*, QList<QVariantMap>> _messageBarEventMap;
};

class TaoIconButton;
class QPainter;
class TaoMessageBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoMessageBar)
    Q_PROPERTY_CREATE(qreal, Opacity)
public:
    explicit TaoMessageBarPrivate(QObject* parent = nullptr);
    ~TaoMessageBarPrivate();
    void tryToRequestMessageBarEvent();
    WorkStatus getWorkMode() const;
    Q_INVOKABLE void onOtherMessageBarEnd(QVariantMap eventData);
    Q_INVOKABLE void messageBarEnd(QVariantMap eventData);
    Q_SLOT void onCloseButtonClicked();

private:
    friend class TaoMessageBarManager;
    TaoThemeType::ThemeMode _themeMode;
    int _borderRadius{6};
    QString _title{""};
    QString _text{""};
    TaoMessageBarType::PositionPolicy _policy;
    TaoMessageBarType::MessageMode _messageMode;
    qreal _createTime{0};

    // 位置数据
    int _leftPadding{20};                // 左边框到图标中心
    int _titleLeftSpacing{30};           // 图标中心到Title左侧
    int _textLeftSpacing{15};            // Title右侧到Text左侧
    int _closeButtonLeftRightMargin{20}; // closeButton左右总Margin
    int _closeButtonWidth{30};
    int _messageBarHorizontalMargin{20};
    int _messageBarVerticalBottomMargin{20};
    int _messageBarVerticalTopMargin{50};
    int _messageBarSpacing{15};
    int _shadowBorderWidth{6};

    // 逻辑数据
    bool _isMessageBarCreateAnimationFinished{false};
    bool _isReadyToEnd{false};
    bool _isNormalDisplay{false};
    bool _isMessageBarEventAnimationStart{false};
    TaoIconButton* _closeButton{nullptr};
    Q_INVOKABLE void _messageBarCreate(int displayMsec);

    // 初始坐标计算
    void _calculateInitialPos(int& startX, int& startY, int& endX, int& endY);
    //获取总高度和次序信息
    QList<int> _getOtherMessageBarTotalData(bool isJudgeCreateOrder = false);
    //计算目标坐标
    qreal _calculateTargetPosY();

    //创建次序判断
    bool _judgeCreateOrder(TaoMessageBar* otherMessageBar);

    // 绘制函数
    void _drawSuccess(QPainter* painter);
    void _drawWarning(QPainter* painter);
    void _drawInformation(QPainter* painter);
    void _drawError(QPainter* painter);
};

#endif // TAOMESSAGEBARPRIVATE_H
