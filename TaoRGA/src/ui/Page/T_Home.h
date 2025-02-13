#ifndef T_HOME_H
#define T_HOME_H

#include "T_homeBasePage.h"
#include "../../include/data.h"


enum class DeviceType
{
    Invalid = -1,
    RBGAseries = 1
};

enum class ModeType
{
    Invalid = -1,
    ScanMode = 1,
    BaseComponents = 2
};



class ElaMenu;
class T_Home : public T_homeBasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Home(QWidget* parent = nullptr);
    ~T_Home();
Q_SIGNALS:
     Q_SIGNAL void elaCardNavigation(DeviceType deviceName, ModeType modeName);   
     //以下信号未使用
    Q_SIGNAL void elaScreenNavigation();
    Q_SIGNAL void elaBaseComponentNavigation();
    Q_SIGNAL void elaSceneNavigation();
    Q_SIGNAL void elaIconNavigation();

protected:
    virtual void mouseReleaseEvent(QMouseEvent* event);

private:
    ElaMenu* _homeMenu{nullptr};
};

#endif // T_HOME_H
