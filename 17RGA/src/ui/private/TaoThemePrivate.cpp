#include "TaoThemePrivate.h"

TaoThemePrivate::TaoThemePrivate(QObject* parent)
    : QObject{parent}
{
}

TaoThemePrivate::~TaoThemePrivate()
{
}

void TaoThemePrivate::_initThemeColor()
{
    //TaoScrollBar
    _lightThemeColorList[TaoThemeType::ScrollBarHandle] = QColor(0xA0, 0xA0, 0xA0);
    _darkThemeColorList[TaoThemeType::ScrollBarHandle] = QColor(0x9F, 0x9F, 0x9F);

    //TaoToggleSwitch
    _lightThemeColorList[TaoThemeType::ToggleSwitchNoToggledCenter] = QColor(0x5A, 0x5A, 0x5A);
    _darkThemeColorList[TaoThemeType::ToggleSwitchNoToggledCenter] = QColor(0xD0, 0xD0, 0xD0);

    // 主题颜色
    _lightThemeColorList[TaoThemeType::PrimaryNormal] = QColor(0x00, 0x67, 0xC0);
    _darkThemeColorList[TaoThemeType::PrimaryNormal] = QColor(0x4C, 0xC2, 0xFF);
    _lightThemeColorList[TaoThemeType::PrimaryHover] = QColor(0x19, 0x75, 0xC5);
    _darkThemeColorList[TaoThemeType::PrimaryHover] = QColor(0x47, 0xB1, 0xE8);
    _lightThemeColorList[TaoThemeType::PrimaryPress] = QColor(0x31, 0x83, 0xCA);
    _darkThemeColorList[TaoThemeType::PrimaryPress] = QColor(0x42, 0xA1, 0xD2);

    // 通用颜色
    // 普通窗体
    _lightThemeColorList[TaoThemeType::WindowBase] = QColor(0xF3, 0xF3, 0xF3);
    _darkThemeColorList[TaoThemeType::WindowBase] = QColor(0x20, 0x20, 0x20);
    _lightThemeColorList[TaoThemeType::WindowCentralStackBase] = QColor(0xFF, 0xFF, 0xFF, 120);
    _darkThemeColorList[TaoThemeType::WindowCentralStackBase] = QColor(0x3E, 0x3E, 0x3E, 60);

    // 浮动窗体
    _lightThemeColorList[TaoThemeType::PopupBorder] = QColor(0xD6, 0xD6, 0xD6);
    _darkThemeColorList[TaoThemeType::PopupBorder] = QColor(0x47, 0x47, 0x47);
    _lightThemeColorList[TaoThemeType::PopupBorderHover] = QColor(0xCC, 0xCC, 0xCC);
    _darkThemeColorList[TaoThemeType::PopupBorderHover] = QColor(0x54, 0x54, 0x54);
    _lightThemeColorList[TaoThemeType::PopupBase] = QColor(0xFA, 0xFA, 0xFA);
    _darkThemeColorList[TaoThemeType::PopupBase] = QColor(0x2C, 0x2C, 0x2C);
    _lightThemeColorList[TaoThemeType::PopupHover] = QColor(0xF0, 0xF0, 0xF0);
    _darkThemeColorList[TaoThemeType::PopupHover] = QColor(0x38, 0x38, 0x38);

    // Dialog窗体
    _lightThemeColorList[TaoThemeType::DialogBase] = Qt::white;
    _darkThemeColorList[TaoThemeType::DialogBase] = QColor(0x1F, 0x1F, 0x1F);
    _lightThemeColorList[TaoThemeType::DialogLayoutArea] = QColor(0xF3, 0xF3, 0xF3);
    _darkThemeColorList[TaoThemeType::DialogLayoutArea] = QColor(0x20, 0x20, 0x20);

    // 基础颜色
    _lightThemeColorList[TaoThemeType::BasicText] = Qt::black;
    _darkThemeColorList[TaoThemeType::BasicText] = Qt::white;
    _lightThemeColorList[TaoThemeType::BasicTextInvert] = Qt::white;
    _darkThemeColorList[TaoThemeType::BasicTextInvert] = Qt::black;
    _lightThemeColorList[TaoThemeType::BasicDetailsText] = QColor(0x87, 0x87, 0x87);
    _darkThemeColorList[TaoThemeType::BasicDetailsText] = QColor(0xAD, 0xAD, 0xB0);
    _lightThemeColorList[TaoThemeType::BasicTextNoFocus] = QColor(0x86, 0x86, 0x8A);
    _darkThemeColorList[TaoThemeType::BasicTextNoFocus] = QColor(0x86, 0x86, 0x8A);
    _lightThemeColorList[TaoThemeType::BasicTextDisable] = QColor(0xB6, 0xB6, 0xB6);
    _darkThemeColorList[TaoThemeType::BasicTextDisable] = QColor(0xA7, 0xA7, 0xA7);
    _lightThemeColorList[TaoThemeType::BasicTextPress] = QColor(0x5A, 0x5A, 0x5D);
    _darkThemeColorList[TaoThemeType::BasicTextPress] = QColor(0xBB, 0xBB, 0xBF);
    _lightThemeColorList[TaoThemeType::BasicBorder] = QColor(0xE5, 0xE5, 0xE5);
    _darkThemeColorList[TaoThemeType::BasicBorder] = QColor(0x4B, 0x4B, 0x4B);
    _lightThemeColorList[TaoThemeType::BasicBorderDeep] = QColor(0xA8, 0xA8, 0xA8);
    _darkThemeColorList[TaoThemeType::BasicBorderDeep] = QColor(0x5C, 0x5C, 0x5C);
    _lightThemeColorList[TaoThemeType::BasicBorderHover] = QColor(0xDA, 0xDA, 0xDA);
    _darkThemeColorList[TaoThemeType::BasicBorderHover] = QColor(0x57, 0x57, 0x57);
    _lightThemeColorList[TaoThemeType::BasicBase] = QColor(0xFD, 0xFD, 0xFD);
    _darkThemeColorList[TaoThemeType::BasicBase] = QColor(0x34, 0x34, 0x34);
    _lightThemeColorList[TaoThemeType::BasicBaseDeep] = QColor(0xE6, 0xE6, 0xE6);
    _darkThemeColorList[TaoThemeType::BasicBaseDeep] = QColor(0x61, 0x61, 0x61);
    _lightThemeColorList[TaoThemeType::BasicDisable] = QColor(0xF5, 0xF5, 0xF5);
    _darkThemeColorList[TaoThemeType::BasicDisable] = QColor(0x2A, 0x2A, 0x2A);
    _lightThemeColorList[TaoThemeType::BasicHover] = QColor(0xF3, 0xF3, 0xF3);
    _darkThemeColorList[TaoThemeType::BasicHover] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[TaoThemeType::BasicPress] = QColor(0xF7, 0xF7, 0xF7);
    _darkThemeColorList[TaoThemeType::BasicPress] = QColor(0x3A, 0x3A, 0x3A);
    _lightThemeColorList[TaoThemeType::BasicBaseLine] = QColor(0xD1, 0xD1, 0xD1);
    _darkThemeColorList[TaoThemeType::BasicBaseLine] = QColor(0x45, 0x45, 0x45);
    _lightThemeColorList[TaoThemeType::BasicHemline] = QColor(0x86, 0x86, 0x86);
    _darkThemeColorList[TaoThemeType::BasicHemline] = QColor(0x9A, 0x9A, 0x9A);
    _lightThemeColorList[TaoThemeType::BasicIndicator] = QColor(0x75, 0x7C, 0x87);
    _darkThemeColorList[TaoThemeType::BasicIndicator] = QColor(0x75, 0x7C, 0x87);
    _lightThemeColorList[TaoThemeType::BasicChute] = QColor(0xD6, 0xD6, 0xD6);
    _darkThemeColorList[TaoThemeType::BasicChute] = QColor(0x63, 0x63, 0x63);

    // 基础透明
    _lightThemeColorList[TaoThemeType::BasicAlternating] = QColor(0xEF, 0xEF, 0xEF, 160);
    _darkThemeColorList[TaoThemeType::BasicAlternating] = QColor(0x45, 0x45, 0x45, 125);
    _lightThemeColorList[TaoThemeType::BasicBaseAlpha] = QColor(0xFF, 0xFF, 0xFF, 160);
    _darkThemeColorList[TaoThemeType::BasicBaseAlpha] = QColor(0x45, 0x45, 0x45, 95);
    _lightThemeColorList[TaoThemeType::BasicBaseDeepAlpha] = QColor(0xCC, 0xCC, 0xCC, 160);
    _darkThemeColorList[TaoThemeType::BasicBaseDeepAlpha] = QColor(0x72, 0x72, 0x72, 95);
    _lightThemeColorList[TaoThemeType::BasicHoverAlpha] = QColor(0xCC, 0xCC, 0xCC, 60);
    _darkThemeColorList[TaoThemeType::BasicHoverAlpha] = QColor(0x4B, 0x4B, 0x4B, 75);
    _lightThemeColorList[TaoThemeType::BasicPressAlpha] = QColor(0xCC, 0xCC, 0xCC, 40);
    _darkThemeColorList[TaoThemeType::BasicPressAlpha] = QColor(0x4B, 0x4B, 0x4B, 55);
    _lightThemeColorList[TaoThemeType::BasicSelectedAlpha] = QColor(0xCC, 0xCC, 0xCC, 60);
    _darkThemeColorList[TaoThemeType::BasicSelectedAlpha] = QColor(0x4B, 0x4B, 0x4B, 75);
    _lightThemeColorList[TaoThemeType::BasicSelectedHoverAlpha] = QColor(0xCC, 0xCC, 0xCC, 40);
    _darkThemeColorList[TaoThemeType::BasicSelectedHoverAlpha] = QColor(0x4B, 0x4B, 0x4B, 55);

    // 状态颜色
    _lightThemeColorList[TaoThemeType::StatusDanger] = QColor(0xE8, 0x11, 0x23);
    _darkThemeColorList[TaoThemeType::StatusDanger] = QColor(0xE8, 0x11, 0x23);
}
