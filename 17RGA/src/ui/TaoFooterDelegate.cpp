#include "./inc/TaoFooterDelegate.h"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "./inc/TaoBaseListView.h"
#include "./inc/TaoFooterModel.h"
#include "./inc/TaoNavigationNode.h"
#include "./inc/TaoTheme.h"

TaoFooterDelegate::TaoFooterDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
    _pTaoListView = nullptr;
    _themeMode = tTheme->getThemeMode();
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
    setProperty("lastSelectMarkTop", 10.0);
    setProperty("lastSelectMarkBottom", 10.0);
    setProperty("selectMarkTop", 10.0);
    setProperty("selectMarkBottom", 10.0);
    // Mark向上
    _lastSelectMarkTopAnimation = new QPropertyAnimation(this, "lastSelectMarkTop");
    connect(_lastSelectMarkTopAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        _lastSelectMarkTop = value.toReal();
        _pTaoListView->viewport()->update(); });
    _lastSelectMarkTopAnimation->setDuration(300);
    _lastSelectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);

    _selectMarkBottomAnimation = new QPropertyAnimation(this, "selectMarkBottom");
    connect(_selectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        _selectMarkBottom = value.toReal();
        _pTaoListView->viewport()->update(); });
    _selectMarkBottomAnimation->setDuration(300);
    _selectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(_lastSelectMarkTopAnimation, &QPropertyAnimation::finished, this, [=]() {
        _isSelectMarkDisplay = true;
        _lastSelectedNode = nullptr;
        _selectMarkBottomAnimation->setStartValue(0);
        _selectMarkBottomAnimation->setEndValue(10);
        _selectMarkBottomAnimation->start(); });

    // Mark向下
    _lastSelectMarkBottomAnimation = new QPropertyAnimation(this, "lastSelectMarkBottom");
    connect(_lastSelectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        _lastSelectMarkBottom = value.toReal();
        _pTaoListView->viewport()->update(); });
    _lastSelectMarkBottomAnimation->setDuration(300);
    _lastSelectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);

    _selectMarkTopAnimation = new QPropertyAnimation(this, "selectMarkTop");
    connect(_selectMarkTopAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        _selectMarkTop = value.toReal();
        _pTaoListView->viewport()->update(); });
    _selectMarkTopAnimation->setDuration(300);
    _selectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(_lastSelectMarkBottomAnimation, &QPropertyAnimation::finished, this, [=]() {
        _isSelectMarkDisplay = true;
        _lastSelectedNode = nullptr;
        _selectMarkTopAnimation->setStartValue(0);
        _selectMarkTopAnimation->setEndValue(10);
        _selectMarkTopAnimation->start(); });
}

TaoFooterDelegate::~TaoFooterDelegate()
{
}

void TaoFooterDelegate::navigationNodeStateChange(QVariantMap data)
{
    if (data.contains("SelectMarkChanged"))
    {
        _lastSelectedNode = data.value("LastSelectedNode").value<TaoNavigationNode*>();
        TaoNavigationNode* selectedNode = data.value("SelectedNode").value<TaoNavigationNode*>();
        bool direction = _compareItemY(selectedNode, _lastSelectedNode);
        _lastSelectMarkTop = 10;
        _lastSelectMarkBottom = 10;
        _selectMarkTop = 10;
        _selectMarkBottom = 10;
        if (direction)
        {
            _lastSelectMarkTopAnimation->setStartValue(10);
            _lastSelectMarkTopAnimation->setEndValue(0);
            _lastSelectMarkTopAnimation->start();
            _lastSelectMarkBottomAnimation->stop();
            _selectMarkTopAnimation->stop();
            _isSelectMarkDisplay = false;
        }
        else
        {
            _lastSelectMarkBottomAnimation->setStartValue(10);
            _lastSelectMarkBottomAnimation->setEndValue(0);
            _lastSelectMarkBottomAnimation->start();
            _lastSelectMarkTopAnimation->stop();
            _selectMarkBottomAnimation->stop();
            _isSelectMarkDisplay = false;
        }
    }
}

void TaoFooterDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    TaoFooterModel* model = dynamic_cast<TaoFooterModel*>(const_cast<QAbstractItemModel*>(index.model()));
    TaoNavigationNode* node = index.data(Qt::UserRole).value<TaoNavigationNode*>();
    if (option.state.testFlag(QStyle::State_HasFocus))
    {
        viewOption.state &= ~QStyle::State_HasFocus;
    }
    QStyledItemDelegate::paint(painter, viewOption, index);
    // 背景绘制
    QRect itemRect = option.rect;
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    itemRect.setTop(itemRect.top() + 2);
    itemRect.setBottom(itemRect.bottom() - 2);
    QPainterPath path;
    path.addRoundedRect(itemRect, 8, 8);
    if (option.state & QStyle::State_Selected)
    {
        if (index == _pPressIndex)
        {
            // 选中时点击
            painter->fillPath(path, TaoThemeColor(_themeMode, BasicHoverAlpha));
        }
        else
        {
            if (option.state & QStyle::State_MouseOver)
            {
                // 选中时覆盖
                painter->fillPath(path, TaoThemeColor(_themeMode, BasicSelectedHoverAlpha));
            }
            else
            {
                // 选中
                painter->fillPath(path, TaoThemeColor(_themeMode, BasicSelectedAlpha));
            }
        }
    }
    else
    {
        if (index == _pPressIndex)
        {
            // 点击时颜色
            painter->fillPath(path, TaoThemeColor(_themeMode, BasicSelectedHoverAlpha));
        }
        else
        {
            if (option.state & QStyle::State_MouseOver)
            {
                // 覆盖时颜色
                painter->fillPath(path, TaoThemeColor(_themeMode, BasicHoverAlpha));
            }
        }
    }
    painter->restore();

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    itemRect = option.rect;

    //顶边线绘制
    if (index.row() == 0)
    {
        painter->setPen(TaoThemeColor(_themeMode, BasicBaseLine));
        painter->drawLine(option.rect.x(), itemRect.y() + 1, option.rect.x() + option.rect.width(), itemRect.y() + 1);
    }
    // 图标绘制
    painter->setPen(index == _pPressIndex ? TaoThemeColor(_themeMode, BasicTextPress) : TaoThemeColor(_themeMode, BasicText));
    if (node->getAwesome() != TaoIconType::None)
    {
        painter->save();
        QFont iconFont = QFont("TaoAwesome");
        iconFont.setPixelSize(17);
        painter->setFont(iconFont);
        painter->drawText(QRect(itemRect.x(), itemRect.y(), _iconAreaWidth, itemRect.height()), Qt::AlignCenter, QChar((unsigned short)node->getAwesome()));
        painter->restore();
    }

    int keyPoints = node->getKeyPoints();
    if (keyPoints)
    {
        // KeyPoints
        painter->save();
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::white);
        painter->drawEllipse(QPoint(255, itemRect.y() + itemRect.height() / 2), 10, 10);
        painter->setBrush(TaoThemeColor(_themeMode, StatusDanger));
        painter->drawEllipse(QPoint(255, itemRect.y() + itemRect.height() / 2), 9, 9);
        painter->setPen(QPen(Qt::white, 2));
        QFont font = painter->font();
        font.setBold(true);
        if (keyPoints > 99)
        {
            keyPoints = 99;
        }
        if (keyPoints > 9)
        {
            font.setPixelSize(11);
        }
        else
        {
            font.setPixelSize(12);
        }
        painter->setFont(font);
        painter->drawText(keyPoints > 9 ? 248 : 251, itemRect.y() + itemRect.height() / 2 + 4, QString::number(keyPoints));
        painter->restore();
    }

    // 文字绘制
    painter->setPen(index == _pPressIndex ? TaoThemeColor(_themeMode, BasicTextPress) : TaoThemeColor(_themeMode, BasicText));
    QRect textRect;
    if (node->getAwesome() != TaoIconType::None)
    {
        textRect = QRect(itemRect.x() + _iconAreaWidth, itemRect.y(), itemRect.width() - _textRightSpacing - _indicatorIconAreaWidth - _iconAreaWidth, itemRect.height());
    }
    else
    {
        textRect = QRect(itemRect.x() + _leftPadding, itemRect.y(), itemRect.width() - _textRightSpacing - _indicatorIconAreaWidth - _leftPadding, itemRect.height());
    }
    QString text = painter->fontMetrics().elidedText(node->getNodeTitle(), Qt::ElideRight, textRect.width());
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);
    // 选中特效
    if (_isSelectMarkDisplay && (node == model->getSelectedNode()))
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(TaoThemeColor(_themeMode, PrimaryNormal));
        painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + _selectMarkTop, 3, itemRect.height() - _selectMarkTop - _selectMarkBottom), 3, 3);
    }
    if (node == _lastSelectedNode)
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(TaoThemeColor(_themeMode, PrimaryNormal));
        painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + _lastSelectMarkTop, 3, itemRect.height() - _lastSelectMarkTop - _lastSelectMarkBottom), 3, 3);
    }
    painter->restore();
}

QSize TaoFooterDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(40);
    return size;
}

bool TaoFooterDelegate::_compareItemY(TaoNavigationNode* node1, TaoNavigationNode* node2)
{
    if (!node1)
    {
        return true;
    }
    if (!node2)
    {
        return false;
    }
    if (node1->getModelIndex().row() < node2->getModelIndex().row())
    {
        return true;
    }
    else
    {
        return false;
    }
}
