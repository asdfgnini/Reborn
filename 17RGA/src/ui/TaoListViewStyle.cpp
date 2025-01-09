#include "./inc/TaoListViewStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "./inc/TaoListView.h"
#include "./inc/TaoTheme.h"
TaoListViewStyle::TaoListViewStyle(QStyle* style)
{
    _pItemHeight = 35;
    _pIsTransparent = false;
    _themeMode = tTheme->getThemeMode();
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

TaoListViewStyle::~TaoListViewStyle()
{
}

void TaoListViewStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::PE_PanelItemViewItem:
    {
        // Item背景
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            QRect itemRect = vopt->rect;
            itemRect.adjust(0, 2, 0, -2);
            QPainterPath path;
            path.addRoundedRect(itemRect, 4, 4);
            if (vopt->state & QStyle::State_Selected)
            {
                if (vopt->state & QStyle::State_MouseOver)
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
            else
            {
                if (vopt->state & QStyle::State_MouseOver)
                {
                    // 覆盖时颜色
                    painter->fillPath(path, TaoThemeColor(_themeMode, BasicHoverAlpha));
                }
            }
            painter->restore();
        }
        return;
    }
    case QStyle::PE_PanelItemViewRow:
    {
        // Item背景隔行变色
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            if (vopt->features == QStyleOptionViewItem::Alternate)
            {
                painter->save();
                painter->setRenderHint(QPainter::Antialiasing);
                painter->setPen(Qt::NoPen);
                painter->setBrush(TaoThemeColor(_themeMode, BasicAlternating));
                painter->drawRect(vopt->rect);
                painter->restore();
            }
        }
        return;
    }
    case QStyle::PE_Widget:
    {
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void TaoListViewStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_ShapedFrame:
    {
        // viewport视口外的其他区域背景
        if (!_pIsTransparent)
        {
            QRect frameRect = option->rect;
            frameRect.adjust(1, 1, -1, -1);
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing);
            painter->setPen(TaoThemeColor(_themeMode, PopupBorder));
            painter->setBrush(TaoThemeColor(_themeMode, BasicBase));
            painter->drawRoundedRect(frameRect, 3, 3);
            painter->restore();
        }
        return;
    }
    case QStyle::CE_ItemViewItem:
    {
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            // 背景绘制
            this->drawPrimitive(QStyle::PE_PanelItemViewItem, option, painter, widget);

            // 内容绘制
            QRect itemRect = option->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
            const TaoListView* listView = dynamic_cast<const TaoListView*>(widget);
            QListView::ViewMode viewMode = listView->viewMode();
            // QRect checkRect = proxy()->subElementRect(SE_ItemViewItemCheckIndicator, vopt, widget);
            QRect iconRect = proxy()->subElementRect(SE_ItemViewItemDecoration, vopt, widget);
            QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, widget);
            iconRect.adjust(_leftPadding, 0, 0, 0);
            textRect.adjust(_leftPadding, 0, 0, 0);
            // 图标绘制
            if (!vopt->icon.isNull())
            {
                QIcon::Mode mode = QIcon::Normal;
                if (!(vopt->state.testFlag(QStyle::State_Enabled)))
                {
                    mode = QIcon::Disabled;
                }
                else if (vopt->state.testFlag(QStyle::State_Selected))
                {
                    mode = QIcon::Selected;
                }
                QIcon::State state = vopt->state & QStyle::State_Open ? QIcon::On : QIcon::Off;
                vopt->icon.paint(painter, iconRect, vopt->decorationAlignment, mode, state);
            }
            // 文字绘制
            if (!vopt->text.isEmpty())
            {
                painter->setPen(TaoThemeColor(_themeMode, BasicText));
                painter->drawText(textRect, vopt->displayAlignment, vopt->text);
            }
            // 选中特效
            if (vopt->state.testFlag(QStyle::State_Selected) && viewMode == QListView::ListMode)
            {
                int heightOffset = itemRect.height() / 4;
                painter->setPen(Qt::NoPen);
                painter->setBrush(TaoThemeColor(_themeMode, PrimaryNormal));
                painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + heightOffset, 3, itemRect.height() - 2 * heightOffset), 3, 3);
            }
            painter->restore();
        }

        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

QSize TaoListViewStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    switch (type)
    {
    case QStyle::CT_ItemViewItem:
    {
        QSize itemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
        const TaoListView* listView = dynamic_cast<const TaoListView*>(widget);
        QListView::ViewMode viewMode = listView->viewMode();
        if (viewMode == QListView::ListMode)
        {
            itemSize.setWidth(itemSize.width() + _leftPadding);
        }
        itemSize.setHeight(_pItemHeight);
        return itemSize;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}
