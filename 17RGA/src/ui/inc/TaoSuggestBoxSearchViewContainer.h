#ifndef TAOSUGGESTBOXSEARCHVIEWCONTAINER_H
#define TAOSUGGESTBOXSEARCHVIEWCONTAINER_H

#include <QWidget>

#include "../../include/Def.h"

class TaoSuggestBoxSearchViewContainer : public QWidget
{
    Q_OBJECT
public:
    explicit TaoSuggestBoxSearchViewContainer(QWidget* parent = nullptr);
    ~TaoSuggestBoxSearchViewContainer();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    TaoThemeType::ThemeMode _themeMode;
};

#endif // TAOSUGGESTBOXSEARCHVIEWCONTAINER_H
