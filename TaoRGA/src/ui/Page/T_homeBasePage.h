#ifndef T_HOMEBASEPAGE_H
#define T_HOMEBASEPAGE_H

#include <ElaScrollPage.h>

class QVBoxLayout;
class T_homeBasePage : public ElaScrollPage
{
    Q_OBJECT
public:
    explicit T_homeBasePage(QWidget* parent = nullptr);
    ~T_homeBasePage();

protected:
    void createCustomWidget(QString desText);
};

#endif // T_BASEPAGE_H
