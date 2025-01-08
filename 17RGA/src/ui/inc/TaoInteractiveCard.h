#ifndef TAOINTERACTIVECARD_H
#define TAOINTERACTIVECARD_H

#include <QPixmap>
#include <QPushButton>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoInteractiveCardPrivate;
class TAO_EXPORT TaoInteractiveCard : public QPushButton
{
    Q_OBJECT
    Q_Q_CREATE(TaoInteractiveCard)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QString, Title);
    Q_PROPERTY_CREATE_Q_H(QString, SubTitle);
    Q_PROPERTY_CREATE_Q_H(int, TitlePixelSize);
    Q_PROPERTY_CREATE_Q_H(int, SubTitlePixelSize);
    Q_PROPERTY_CREATE_Q_H(int, TitleSpacing);
    Q_PROPERTY_CREATE_Q_H(QPixmap, CardPixmap);
    Q_PROPERTY_CREATE_Q_H(QSize, CardPixmapSize);
    Q_PROPERTY_CREATE_Q_H(int, CardPixmapBorderRadius)
    Q_PROPERTY_CREATE_Q_H(TaoCardPixType::PixMode, CardPixMode);

public:
    explicit TaoInteractiveCard(QWidget* parent = nullptr);
    ~TaoInteractiveCard();
    void setCardPixmapSize(int width, int height);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // TAOINTERACTIVECARD_H
