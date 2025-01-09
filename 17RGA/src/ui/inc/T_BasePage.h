#ifndef T_BASEPAGE_H
#define T_BASEPAGE_H

#include "TaoScrollPage.h"

class QVBoxLayout;
class T_BasePage : public TaoScrollPage
{
    Q_OBJECT
public:
    explicit T_BasePage(QWidget* parent = nullptr);
    ~T_BasePage();

protected:
    void createCustomWidget(QString desText);
};

#endif // T_BASEPAGE_H
