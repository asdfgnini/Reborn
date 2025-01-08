#include "./inc/TaoCentralStackedWidget.h"

#include <QPainter>
#include <QPainterPath>

#include "./inc/TaoTheme.h"

TaoCentralStackedWidget::TaoCentralStackedWidget(QWidget* parent)
    : QStackedWidget(parent)
{
    setObjectName("TaoCentralStackedWidget");
    _themeMode = tTheme->getThemeMode();
    connect(tTheme, &TaoTheme::themeModeChanged, this, &TaoCentralStackedWidget::onThemeModeChanged);
}

TaoCentralStackedWidget::~TaoCentralStackedWidget()
{
}

void TaoCentralStackedWidget::onThemeModeChanged(TaoThemeType::ThemeMode themeMode)
{
    _themeMode = themeMode;
}

void TaoCentralStackedWidget::setIsTransparent(bool isTransparent)
{
    this->_isTransparent = isTransparent;
    update();
}

bool TaoCentralStackedWidget::getIsTransparent() const
{
    return _isTransparent;
}

void TaoCentralStackedWidget::setIsHasRadius(bool isHasRadius)
{
    this->_isHasRadius = isHasRadius;
    update();
}

void TaoCentralStackedWidget::paintEvent(QPaintEvent* event)
{
    if (!_isTransparent)
    {
        QPainter painter(this);
        QRect targetRect = this->rect();
        targetRect.adjust(1, 1, 10, 10);
        painter.save();
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(TaoThemeColor(_themeMode, BasicBaseLine), 1.5));
        painter.setBrush(TaoThemeColor(_themeMode, WindowCentralStackBase));
        if (_isHasRadius)
        {
            painter.drawRoundedRect(targetRect, 10, 10);
        }
        else
        {
            painter.drawRect(targetRect);
        }
        painter.restore();
    }
    QStackedWidget::paintEvent(event);
}
