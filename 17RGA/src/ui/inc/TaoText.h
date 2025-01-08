#ifndef TAOTEXT_H
#define TAOTEXT_H

#include <QLabel>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoTextPrivate;
class TAO_EXPORT TaoText : public QLabel
{
    Q_OBJECT
    Q_Q_CREATE(TaoText)
public:
    explicit TaoText(QWidget* parent = nullptr);
    explicit TaoText(QString text, QWidget* parent = nullptr);
    explicit TaoText(QString text, int pixelSize, QWidget* parent = nullptr);
    ~TaoText();
    void setIsWrapAnywhere(bool isWrapAnywhere);
    bool getIsWrapAnywhere() const;

    void setTextPixelSize(int size);
    int getTextPixelSize() const;
    void setTextPointSize(int size);
    int getTextPointSize() const;
    void setTextStyle(TaoTextType::TextStyle textStyle);
    TaoTextType::TextStyle getTextStyle() const;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // TAOTEXT_H
