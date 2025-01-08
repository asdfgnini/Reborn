#include "./inc/TaoSuggestDelegate.h"

#include <QPainter>
#include <QPainterPath>

#include "private/TaoSuggestBoxPrivate.h"
#include "./inc/TaoSuggestModel.h"
#include "./inc/TaoTheme.h"
TaoSuggestDelegate::TaoSuggestDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
    _themeMode = tTheme->getThemeMode();
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

TaoSuggestDelegate::~TaoSuggestDelegate()
{
}

void TaoSuggestDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    int margin = 2;
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);

    TaoSuggestModel* model = dynamic_cast<TaoSuggestModel*>(const_cast<QAbstractItemModel*>(index.model()));
    TaoSuggestion* suggest = model->getSearchSuggestion(index.row());
    if (option.state.testFlag(QStyle::State_HasFocus))
    {
        viewOption.state &= ~QStyle::State_HasFocus;
    }
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPainterPath path;
    QRect optionRect = option.rect;
    optionRect.adjust(margin * 2, margin, -margin * 2, -margin);
    path.addRoundedRect(optionRect, 8, 8);
    if (option.state & QStyle::State_Selected)
    {
        if (option.state & QStyle::State_MouseOver)
        {
            //选中时覆盖
            painter->fillPath(path, TaoThemeColor(_themeMode, BasicSelectedHoverAlpha));
        }
        else
        {
            //选中
            painter->fillPath(path, TaoThemeColor(_themeMode, BasicSelectedAlpha));
        }
    }
    else
    {
        if (option.state & QStyle::State_MouseOver)
        {
            //覆盖时颜色
            painter->fillPath(path, TaoThemeColor(_themeMode, BasicHoverAlpha));
        }
    }
    //文字绘制
    painter->setPen(TaoThemeColor(_themeMode, BasicText));
    painter->drawText(option.rect.x() + 37, option.rect.y() + 25, suggest->getSuggestText());

    //图标绘制
    if (suggest->getTaoIcon() != TaoIconType::None)
    {
        QFont iconFont = QFont("TaoAwesome");
        iconFont.setPixelSize(17);
        painter->setFont(iconFont);
        painter->drawText(option.rect.x() + 11, option.rect.y() + 26, QChar((unsigned short)suggest->getTaoIcon()));
    }
    painter->restore();
}

QSize TaoSuggestDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(40);
    return size;
}
