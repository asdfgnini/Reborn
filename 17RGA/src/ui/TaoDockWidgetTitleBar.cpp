#include "./inc/TaoDockWidgetTitleBar.h"

#include <QHBoxLayout>
#include <QPainter>

#include "./inc/TaoDockWidget.h"
#include "./inc/TaoIconButton.h"
#include "./inc/TaoText.h"
#include "./inc/TaoTheme.h"

TaoDockWidgetTitleBar::TaoDockWidgetTitleBar(QWidget* parent)
    : QWidget{parent}
{
    _dockWidget = dynamic_cast<TaoDockWidget*>(parent);
    _iconLabel = new QLabel(this);
    _iconLabel->setPixmap(_dockWidget->windowIcon().pixmap(QSize(18, 18)));
    _titleLabel = new TaoText(_dockWidget->windowTitle(), this);
    _titleLabel->setWordWrap(false);
    _titleLabel->setTextPixelSize(13);

    _floatButton = new TaoIconButton(TaoIconType::TAOwindow_restore, 13, 32, 26, this);
    _floatButton->setLightHoverColor(TaoThemeColor(TaoThemeType::Light, BasicHoverAlpha));
    _floatButton->setDarkHoverColor(TaoThemeColor(TaoThemeType::Dark, BasicHoverAlpha));
    connect(_floatButton, &TaoIconButton::clicked, this, &TaoDockWidgetTitleBar::onFloatButtonClicked);
    _closeButton = new TaoIconButton(TaoIconType::TAOXmark, 17, 32, 26, this);
    _closeButton->setLightHoverColor(TaoThemeColor(TaoThemeType::Light, StatusDanger));
    _closeButton->setDarkHoverColor(TaoThemeColor(TaoThemeType::Dark, StatusDanger));
    connect(_closeButton, &TaoIconButton::clicked, this, &TaoDockWidgetTitleBar::onCloseButtonClicked);

    _setVisibleFromFeatures(_dockWidget->features());
    connect(_dockWidget, &QDockWidget::featuresChanged, this, [=](QDockWidget::DockWidgetFeatures features) {
        _setVisibleFromFeatures(features);
    });
    connect(_dockWidget, &QDockWidget::windowTitleChanged, this, [=](const QString& title) {
        _titleLabel->setText(title);
    });
    connect(_dockWidget, &QDockWidget::windowIconChanged, this, [=](const QIcon& icon) {
        _iconLabel->setPixmap(icon.pixmap(QSize(18, 18)));
    });

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(_iconLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(_titleLabel);
    mainLayout->addSpacing(10);
    mainLayout->addStretch();
    mainLayout->addWidget(_floatButton);
    mainLayout->addWidget(_closeButton);

    //主题变更
    _themeMode = tTheme->getThemeMode();
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

TaoDockWidgetTitleBar::~TaoDockWidgetTitleBar()
{
}

void TaoDockWidgetTitleBar::onFloatButtonClicked()
{
    _dockWidget->setFloating(_dockWidget->isFloating() ? false : true);
}

void TaoDockWidgetTitleBar::onCloseButtonClicked()
{
    _dockWidget->close();
}

void TaoDockWidgetTitleBar::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(_dockWidget->isFloating() ? Qt::transparent : TaoThemeColor(_themeMode, BasicBaseAlpha));
    painter.drawRect(rect());
    painter.restore();
}

void TaoDockWidgetTitleBar::_setVisibleFromFeatures(QDockWidget::DockWidgetFeatures features)
{
    _floatButton->setVisible(features.testFlag(QDockWidget::DockWidgetFloatable));
    _closeButton->setVisible(features.testFlag(QDockWidget::DockWidgetClosable));
}
