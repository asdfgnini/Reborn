#include "./inc/TaoMessageBar.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QResizeEvent>

#include "./inc/TaoIconButton.h"
#include "./inc/TaoTheme.h"
#include "private/TaoMessageBarPrivate.h"

TaoMessageBar::TaoMessageBar(TaoMessageBarType::PositionPolicy policy, TaoMessageBarType::MessageMode messageMode, QString& title, QString& text, int displayMsec, QWidget* parent)
    : QWidget{parent}, d_ptr(new TaoMessageBarPrivate())
{
    Q_D(TaoMessageBar);
    d->q_ptr = this;
    d->_borderRadius = 6;
    d->_title = title;
    d->_text = text;
    d->_policy = policy;
    d->_messageMode = messageMode;
    d->_themeMode = tTheme->getThemeMode();
    setFixedHeight(60);
    setMouseTracking(true);
    d->_pOpacity = 1;
    setFont(QFont("微软雅黑"));
    parent->installEventFilter(this);
    d->_closeButton = new TaoIconButton(TaoIconType::TAOXmark, 17, d->_closeButtonWidth, 30, this);
    switch (d->_messageMode)
    {
    case TaoMessageBarType::Success:
    {
        d->_closeButton->setLightHoverColor(QColor(0xE6, 0xFC, 0xE3));
        d->_closeButton->setDarkHoverColor(QColor(0xE6, 0xFC, 0xE3));
        d->_closeButton->setDarkIconColor(Qt::black);
        break;
    }
    case TaoMessageBarType::Warning:
    {
        d->_closeButton->setLightHoverColor(QColor(0x5E, 0x4C, 0x22));
        d->_closeButton->setDarkHoverColor(QColor(0x5E, 0x4C, 0x22));
        d->_closeButton->setLightIconColor(Qt::white);
        d->_closeButton->setDarkIconColor(Qt::white);
        break;
    }
    case TaoMessageBarType::Information:
    {
        d->_closeButton->setLightHoverColor(QColor(0xEB, 0xEB, 0xEB));
        d->_closeButton->setDarkHoverColor(QColor(0xEB, 0xEB, 0xEB));
        d->_closeButton->setDarkIconColor(Qt::black);
        break;
    }
    case TaoMessageBarType::Error:
    {
        d->_closeButton->setLightHoverColor(QColor(0xF7, 0xE1, 0xE4));
        d->_closeButton->setDarkHoverColor(QColor(0xF7, 0xE1, 0xE4));
        d->_closeButton->setDarkIconColor(Qt::black);
        break;
    }
    }
    d->_closeButton->setBorderRadius(5);
    connect(d->_closeButton, &TaoIconButton::clicked, d, &TaoMessageBarPrivate::onCloseButtonClicked);
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 10, 0);
    mainLayout->addStretch();
    mainLayout->addWidget(d->_closeButton);
    setObjectName("TaoMessageBar");
    setStyleSheet("#TaoMessageBar{background-color:transparent;}");
    d->_messageBarCreate(displayMsec);
}

TaoMessageBar::~TaoMessageBar()
{
}

void TaoMessageBar::success(TaoMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent)
{
    // qDebug() << QApplication::topLevelWidgets();
    if (!parent)
    {
        QList<QWidget*> widgetList = QApplication::topLevelWidgets();
        for (auto widget : widgetList)
        {
            if (widget->property("TaoBaseClassName").toString() == "TaoWindow")
            {
                parent = widget;
            }
        }
        if (!parent)
        {
            return;
        }
    }

    TaoMessageBar* bar = new TaoMessageBar(policy, TaoMessageBarType::Success, title, text, displayMsec, parent);
    Q_UNUSED(bar);
}

void TaoMessageBar::warning(TaoMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent)
{
    if (!parent)
    {
        QList<QWidget*> widgetList = QApplication::topLevelWidgets();
        for (auto widget : widgetList)
        {
            if (widget->property("TaoBaseClassName").toString() == "TaoWindow")
            {
                parent = widget;
            }
        }
        if (!parent)
        {
            return;
        }
    }
    TaoMessageBar* bar = new TaoMessageBar(policy, TaoMessageBarType::Warning, title, text, displayMsec, parent);
    Q_UNUSED(bar);
}

void TaoMessageBar::information(TaoMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent)
{
    if (!parent)
    {
        QList<QWidget*> widgetList = QApplication::topLevelWidgets();
        for (auto widget : widgetList)
        {
            if (widget->property("TaoBaseClassName").toString() == "TaoWindow")
            {
                parent = widget;
            }
        }
        if (!parent)
        {
            return;
        }
    }
    TaoMessageBar* bar = new TaoMessageBar(policy, TaoMessageBarType::Information, title, text, displayMsec, parent);
    Q_UNUSED(bar);
}

void TaoMessageBar::error(TaoMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent)
{
    if (!parent)
    {
        QList<QWidget*> widgetList = QApplication::topLevelWidgets();
        for (auto widget : widgetList)
        {
            if (widget->property("TaoBaseClassName").toString() == "TaoWindow")
            {
                parent = widget;
            }
        }
        if (!parent)
        {
            return;
        }
    }
    TaoMessageBar* bar = new TaoMessageBar(policy, TaoMessageBarType::Error, title, text, displayMsec, parent);
    Q_UNUSED(bar);
}

void TaoMessageBar::paintEvent(QPaintEvent* event)
{
    Q_D(TaoMessageBar);
    QPainter painter(this);
    painter.setOpacity(d->_pOpacity);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 高性能阴影
    tTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_borderRadius);

    // 背景和图标绘制
    painter.save();
    painter.setPen(d->_themeMode == TaoThemeType::Light ? QColor(0xBE, 0xBA, 0xBE) : QColor(0x52, 0x50, 0x52));
    switch (d->_messageMode)
    {
    case TaoMessageBarType::Success:
    {
        d->_drawSuccess(&painter);
        break;
    }
    case TaoMessageBarType::Warning:
    {
        d->_drawWarning(&painter);
        break;
    }
    case TaoMessageBarType::Information:
    {
        d->_drawInformation(&painter);
        break;
    }
    case TaoMessageBarType::Error:
    {
        d->_drawError(&painter);
        break;
    }
    }
    // 文字绘制
    // 标题
    QFont font = this->font();
    font.setWeight(QFont::Bold);
    font.setPixelSize(16);
    painter.setFont(font);
    int titleTextWidth = painter.fontMetrics().horizontalAdvance(d->_title) + 1;
    if (titleTextWidth > 100)
    {
        titleTextWidth = 100;
    }
    int textFlags = Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap | Qt::TextWrapAnywhere;
    painter.drawText(QRect(d->_leftPadding + d->_titleLeftSpacing, -1, titleTextWidth, height()), textFlags, d->_title);
    // 正文
    font.setWeight(QFont::Light);
    font.setPixelSize(15);
    painter.setFont(font);
    painter.drawText(QRect(d->_leftPadding + d->_titleLeftSpacing + titleTextWidth + d->_textLeftSpacing, 0, width() - (d->_leftPadding + d->_titleLeftSpacing + titleTextWidth + d->_textLeftSpacing + d->_closeButtonWidth + d->_closeButtonLeftRightMargin / 2), height()), textFlags, d->_text);
    int textHeight = painter.fontMetrics().boundingRect(QRect(d->_leftPadding + d->_titleLeftSpacing + titleTextWidth + d->_textLeftSpacing, 0, width() - (d->_leftPadding + d->_titleLeftSpacing + titleTextWidth + d->_textLeftSpacing + d->_closeButtonWidth + d->_closeButtonLeftRightMargin), height()), textFlags, d->_text).height();
    if (textHeight >= minimumHeight() - 20)
    {
        setMinimumHeight(textHeight + 20);
    }
    painter.restore();
}

bool TaoMessageBar::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(TaoMessageBar);
    if (watched == parentWidget())
    {
        switch (event->type())
        {
        case QEvent::Resize:
        {
            QResizeEvent* resizeEvent = dynamic_cast<QResizeEvent*>(event);
            QSize offsetSize = parentWidget()->size() - resizeEvent->oldSize();
            if (d->_isNormalDisplay)
            {
                switch (d->_policy)
                {
                case TaoMessageBarType::Top:
                {
                    this->move(parentWidget()->width() / 2 - minimumWidth() / 2, this->y());
                    break;
                }
                case TaoMessageBarType::Bottom:
                {
                    this->move(parentWidget()->width() / 2 - minimumWidth() / 2, this->pos().y() + offsetSize.height());
                    break;
                }
                case TaoMessageBarType::Left:
                case TaoMessageBarType::TopLeft:
                {
                    this->move(d->_messageBarHorizontalMargin, this->pos().y());
                    break;
                }
                case TaoMessageBarType::BottomLeft:
                {
                    this->move(d->_messageBarHorizontalMargin, this->pos().y() + offsetSize.height());
                    break;
                }
                case TaoMessageBarType::Right:
                case TaoMessageBarType::TopRight:
                {
                    this->move(parentWidget()->width() - minimumWidth() - d->_messageBarHorizontalMargin, this->y());
                    break;
                }
                case TaoMessageBarType::BottomRight:
                {
                    this->move(parentWidget()->width() - minimumWidth() - d->_messageBarHorizontalMargin, this->pos().y() + offsetSize.height());
                    break;
                }
                }
            }
            break;
        }
        default:
        {
            break;
        }
        }
    }
    return QWidget::eventFilter(watched, event);
}
