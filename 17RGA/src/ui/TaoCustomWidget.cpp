#include "./inc/TaoCustomWidget.h"

#include <QPainter>
#include <QVBoxLayout>

#include "./inc/TaoApplication.h"
#include "./inc/TaoTheme.h"
Q_TAKEOVER_NATIVEEVENT_CPP(TaoCustomWidget, _appBar);
TaoCustomWidget::TaoCustomWidget(QWidget* parent)
    : QDialog{parent}
{
    resize(500, 500); // 默认宽高
    setObjectName("TaoCustomWidget");
    // 自定义AppBar
    _appBar = new TaoAppBar(this);
    _appBar->setWindowButtonFlags(TaoAppBarType::MinimizeButtonHint | TaoAppBarType::MaximizeButtonHint | TaoAppBarType::CloseButtonHint);
    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setContentsMargins(0, 0, 0, 0);

    _themeMode = tTheme->getThemeMode();
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        update();
    });

    _isEnableMica = tApp->getIsEnableMica();
    connect(tApp, &TaoApplication::pIsEnableMicaChanged, this, [=]() {
        _isEnableMica = tApp->getIsEnableMica();
        update();
    });
    tApp->syncMica(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

TaoCustomWidget::~TaoCustomWidget()
{
}

void TaoCustomWidget::setCentralWidget(QWidget* widget)
{
    if (!widget)
    {
        return;
    }
    _centralWidget = widget;
    _mainLayout->addWidget(widget);
    widget->setVisible(true);
}

void TaoCustomWidget::paintEvent(QPaintEvent* event)
{
    if (!_isEnableMica)
    {
        QPainter painter(this);
        painter.save();
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(TaoThemeColor(_themeMode, WindowBase));
        painter.drawRect(rect());
        painter.restore();
    }
}
