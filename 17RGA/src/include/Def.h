#ifndef DEF_H
#define DEF_H
#include <QObject>

#include "stdafx.h"

//枚举类导出  兼容QT5低版本
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#define Q_BEGIN_ENUM_CREATE(CLASS) \
    namespace CLASS                \
    {                              \
    Q_NAMESPACE_EXPORT(TAO_EXPORT)

#define Q_END_ENUM_CREATE(CLASS) }

#define Q_ENUM_CREATE(CLASS) Q_ENUM_NS(CLASS)
#else
#define Q_BEGIN_ENUM_CREATE(CLASS)          \
    class TAO_EXPORT CLASS : public QObject \
    {                                       \
        Q_OBJECT                            \
    public:

#define Q_END_ENUM_CREATE(CLASS) \
private:                         \
    Q_DISABLE_COPY(CLASS)        \
    }                            \
    ;

#define Q_ENUM_CREATE(CLASS) Q_ENUM(CLASS)
#endif

Q_BEGIN_ENUM_CREATE(TaoThemeType)
enum ThemeMode
{
    Light = 0x0000,
    Dark = 0x0001,
};
Q_ENUM_CREATE(ThemeMode)

enum ThemeColor
{
    ScrollBarHandle,
    ToggleSwitchNoToggledCenter,
    WindowBase,
    WindowCentralStackBase,
    PrimaryNormal,
    PrimaryHover,
    PrimaryPress,
    PopupBorder,
    PopupBorderHover,
    PopupBase,
    PopupHover,
    DialogBase,
    DialogLayoutArea,
    BasicText,
    BasicTextInvert,
    BasicDetailsText,
    BasicTextNoFocus,
    BasicTextDisable,
    BasicTextPress,
    BasicBorder,
    BasicBorderDeep,
    BasicBorderHover,
    BasicBase,
    BasicBaseDeep,
    BasicDisable,
    BasicHover,
    BasicPress,
    BasicSelectedHover,
    BasicBaseLine,
    BasicHemline,
    BasicIndicator,
    BasicChute,
    BasicAlternating,
    BasicBaseAlpha,
    BasicBaseDeepAlpha,
    BasicHoverAlpha,
    BasicPressAlpha,
    BasicSelectedAlpha,
    BasicSelectedHoverAlpha,
    StatusDanger,
};
Q_ENUM_CREATE(ThemeColor)
Q_END_ENUM_CREATE(TaoThemeType)

Q_BEGIN_ENUM_CREATE(TaoAppBarType)
enum ButtonType
{
    RouteBackButtonHint = 0x0001,
    NavigationButtonHint = 0x0002,
    StayTopButtonHint = 0x0004,
    ThemeChangeButtonHint = 0x0008,
    MinimizeButtonHint = 0x00010,
    MaximizeButtonHint = 0x0020,
    CloseButtonHint = 0x0040,
};
Q_ENUM_CREATE(ButtonType)
Q_DECLARE_FLAGS(ButtonFlags, ButtonType)

enum CustomArea
{
    LeftArea = 0x0001,
    MiddleArea = 0x0002,
    RightArea = 0x0003,
};
Q_ENUM_CREATE(CustomArea)

enum WMMouseActionType
{
    WMLBUTTONDOWN = 0x0001,
    WMLBUTTONUP = 0x0002,
    WMLBUTTONDBLCLK = 0x0004,
    WMNCLBUTTONDOWN = 0x0008,
};
Q_ENUM_CREATE(WMMouseActionType)
Q_DECLARE_FLAGS(WMMouseActionTypes, WMMouseActionType)
Q_END_ENUM_CREATE(TaoAppBarType)
Q_DECLARE_OPERATORS_FOR_FLAGS(TaoAppBarType::ButtonFlags)
Q_DECLARE_OPERATORS_FOR_FLAGS(TaoAppBarType::WMMouseActionTypes)

Q_BEGIN_ENUM_CREATE(TaoTextType)
enum TextStyle
{
    NoStyle = 0x0000,
    Caption = 0x0001,
    Body = 0x0002,
    BodyStrong = 0x0003,
    Subtitle = 0x0004,
    Title = 0x0005,
    TitleLarge = 0x0006,
    Display = 0x0007,
};
Q_ENUM_CREATE(TextStyle)
Q_END_ENUM_CREATE(TaoTextType)

Q_BEGIN_ENUM_CREATE(TaoNavigationType)
enum NodeOperateReturnType
{
    Success = 0x0000,
    TargetNodeInvalid = 0x0001,
    TargetNodeTypeError = 0x0002,
    TargetNodeDepthLimit = 0x0003,
    PageInvalid = 0x0004,
    FooterUpperLimit = 0x0005,
};
Q_ENUM_CREATE(NodeOperateReturnType)

enum NavigationDisplayMode
{
    Auto = 0x0000,
    Minimal = 0x0001,
    Compact = 0x0002,
    Maximal = 0x0003,
};
Q_ENUM_CREATE(NavigationDisplayMode)

enum NavigationNodeType
{
    PageNode = 0x0000,
    FooterNode = 0x0001,
};
Q_ENUM_CREATE(NavigationNodeType)
Q_END_ENUM_CREATE(TaoNavigationType)

Q_BEGIN_ENUM_CREATE(TaoNavigationRouterType)
enum NavigationRouteType
{
    Success = 0x0000,
    ObjectInvalid = 0x0001,
    FunctionNameInvalid = 0x0002,
};
Q_ENUM_CREATE(NavigationRouteType)
Q_END_ENUM_CREATE(TaoNavigationRouterType)

Q_BEGIN_ENUM_CREATE(TaoEventBusType)
enum EventBusReturnType
{
    Success = 0x0000,
    EventInvalid = 0x0001,
    EventNameInvalid = 0x0002,

};
Q_ENUM_CREATE(EventBusReturnType)
Q_END_ENUM_CREATE(TaoEventBusType)

Q_BEGIN_ENUM_CREATE(TaoCardPixType)
enum PixMode
{
    Default = 0x0000,
    RoundedRect = 0x0001,
    Ellipse = 0x0002,
};
Q_ENUM_CREATE(PixMode)
Q_END_ENUM_CREATE(TaoCardPixType)

Q_BEGIN_ENUM_CREATE(TaoGraphicsSceneType)
enum SceneMode
{
    Default = 0x0000,
    DragMove = 0x0001,
    MultiSelect = 0x0002,
    ItemLink = 0x0003,
};
Q_ENUM_CREATE(SceneMode)
Q_END_ENUM_CREATE(TaoGraphicsSceneType)

Q_BEGIN_ENUM_CREATE(TaoMessageBarType)
enum PositionPolicy
{
    Top = 0x0000,
    Left = 0x0001,
    Bottom = 0x0002,
    Right = 0x0003,
    TopRight = 0x0004,
    TopLeft = 0x0005,
    BottomRight = 0x0006,
    BottomLeft = 0x0007,
};
Q_ENUM_CREATE(PositionPolicy)

enum MessageMode
{
    Success = 0x0000,
    Warning = 0x0001,
    Information = 0x0002,
    Error = 0x0003,
};
Q_ENUM_CREATE(MessageMode)
Q_END_ENUM_CREATE(TaoMessageBarType)

Q_BEGIN_ENUM_CREATE(TaoIconType)
enum IconName
{
    None = 0x0,

    TAObars = 0xf0c9,
    TAOatom = 0xf5d2,
    TAOlist_check = 0xf0ae,
    TAOcopy = 0xf0c5,
    TAOvideo = 0xf03d,
    TAOrotate_left = 0xf2ea,
    TAOrotate_right = 0xf2f9,
    TAOangle_right = 0xf105,
    TAOcredit_card = 0xf09d,
    TAOfile = 0xf15b,
    TAOwindow_restore = 0xf2d2,
    TAOangle_down = 0xf107,
    TAOXmark = 0xf00d,
    TAOarrow_up = 0xf062,
    TAOwindow_minimize = 0xf2d1,
    TAOexpand = 0xf065,
    TAOwindow_maximize = 0xf2d0,
};
Q_ENUM_CREATE(IconName)
Q_END_ENUM_CREATE(TaoIconType)
#endif // DEF_H
