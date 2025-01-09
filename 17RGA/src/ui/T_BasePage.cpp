#include "./inc/T_BasePage.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "./inc/TaoMenu.h"
#include "./inc/TaoText.h"
#include "./inc/TaoTheme.h"
#include "./inc/TaoToolButton.h"


T_BasePage::T_BasePage(QWidget* parent)
    : TaoScrollPage(parent)
{
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=]() {
        if (!parent)
        {
            update();
        }
    });
}

T_BasePage::~T_BasePage()
{
}

void T_BasePage::createCustomWidget(QString desText)
{
    // 顶部元素
    QWidget* customWidget = new QWidget(this);
    TaoText* subTitleText = new TaoText(this);
    subTitleText->setText("https://github.com/Liniyous/TaoWidgetTools");
    subTitleText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    subTitleText->setTextPixelSize(11);

    TaoToolButton* documentationButton = new TaoToolButton(this);
    documentationButton->setFixedHeight(35);
    documentationButton->setIsTransparent(false);
    documentationButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //_toolButton->setPopupMode(QToolButton::MenuButtonPopup);
    documentationButton->setText("Documentation");
    documentationButton->setTaoIcon(TaoIconType::FileDoc);
    TaoMenu* documentationMenu = new TaoMenu(this);
    documentationMenu->addTaoIconAction(TaoIconType::CardsBlank, "CardsBlank");
    documentationMenu->addTaoIconAction(TaoIconType::EarthAmericas, "EarthAmericas");
    documentationButton->setMenu(documentationMenu);

    TaoToolButton* sourceButton = new TaoToolButton(this);
    sourceButton->setFixedHeight(35);
    sourceButton->setIsTransparent(false);
    sourceButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    sourceButton->setText("Source");
    sourceButton->setTaoIcon(TaoIconType::NfcSymbol);
    TaoMenu* sourceMenu = new TaoMenu(this);
    sourceMenu->addTaoIconAction(TaoIconType::FireBurner, "FireBurner");
    sourceMenu->addTaoIconAction(TaoIconType::Galaxy, "Galaxy~~~~");
    sourceButton->setMenu(sourceMenu);

    TaoToolButton* themeButton = new TaoToolButton(this);
    themeButton->setFixedSize(35, 35);
    themeButton->setIsTransparent(false);
    themeButton->setTaoIcon(TaoIconType::MoonStars);
    connect(themeButton, &TaoToolButton::clicked, this, [=]() {
        tTheme->setThemeMode(tTheme->getThemeMode() == TaoThemeType::Light ? TaoThemeType::Dark : TaoThemeType::Light);
    });

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(documentationButton);
    buttonLayout->addSpacing(5);
    buttonLayout->addWidget(sourceButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(themeButton);
    buttonLayout->addSpacing(15);

    TaoText* descText = new TaoText(this);
    descText->setText(desText);
    descText->setTextPixelSize(13);

    QVBoxLayout* topLayout = new QVBoxLayout(customWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->addWidget(subTitleText);
    topLayout->addSpacing(5);
    topLayout->addLayout(buttonLayout);
    topLayout->addSpacing(5);
    topLayout->addWidget(descText);
    setCustomWidget(customWidget);
}
