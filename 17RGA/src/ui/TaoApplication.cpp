#include "./inc/TaoApplication.h"

#include <QApplication>
#include <QCursor>
#include <QFontDatabase>
#include <QWidget>

#include "./inc/TaoTheme.h"
#include "private/TaoApplicationPrivate.h"

Q_SINGLETON_CREATE_CPP(TaoApplication)
TaoApplication::TaoApplication(QObject* parent)
    : QObject{parent}, d_ptr(new TaoApplicationPrivate())
{
    Q_D(TaoApplication);
    d->q_ptr = this;
    d->_pIsEnableMica = false;
    d->_pMicaImagePath = ":/include/Image/MicaBase.png";
    d->_themeMode = tTheme->getThemeMode();
    connect(tTheme, &TaoTheme::themeModeChanged, d, &TaoApplicationPrivate::onThemeModeChanged);
}

TaoApplication::~TaoApplication()
{
}

void TaoApplication::setIsEnableMica(bool isEnable)
{
    Q_D(TaoApplication);
    d->_pIsEnableMica = isEnable;
    if (isEnable)
    {
        d->_initMicaBaseImage(QImage(d->_pMicaImagePath));
    }
    else
    {
        d->onThemeModeChanged(d->_themeMode);
        Q_EMIT pIsEnableMicaChanged();
    }
}

bool TaoApplication::getIsEnableMica() const
{
    Q_D(const TaoApplication);
    return d->_pIsEnableMica;
}

void TaoApplication::setMicaImagePath(QString micaImagePath)
{
    Q_D(TaoApplication);
    d->_pMicaImagePath = micaImagePath;
    d->_initMicaBaseImage(QImage(d->_pMicaImagePath));
    Q_EMIT pMicaImagePathChanged();
}

QString TaoApplication::getMicaImagePath() const
{
    Q_D(const TaoApplication);
    return d->_pMicaImagePath;
}

void TaoApplication::init()
{
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    int fontId = QFontDatabase::addApplicationFont(":/ui/res/Font/FontAwesome6.otf");
    if (fontId == -1)
    {
        qWarning() << "字体加载失败!";
        return;
    }

    //默认字体
    QFont font = qApp->font();
    font.setPixelSize(13);
    font.setFamily("Microsoft YaHei");
    font.setHintingPreference(QFont::PreferNoHinting);
    qApp->setFont(font);
}

void TaoApplication::syncMica(QWidget* widget, bool isSync)
{
    Q_D(TaoApplication);
    if (!widget)
    {
        return;
    }
    if (isSync)
    {
        widget->installEventFilter(d);
        d->_micaWidgetList.append(widget);
        if (d->_pIsEnableMica)
        {
            d->_updateMica(widget, false);
        }
    }
    else
    {
        widget->removeEventFilter(d);
        d->_micaWidgetList.removeOne(widget);
    }
}

bool TaoApplication::containsCursorToItem(QWidget* item)
{
    if (!item || !item->isVisible())
    {
        return false;
    }
    auto point = item->window()->mapFromGlobal(QCursor::pos());
    QRectF rect = QRectF(item->mapTo(item->window(), QPoint(0, 0)), item->size());
    if (rect.contains(point))
    {
        return true;
    }
    return false;
}
