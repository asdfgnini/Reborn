#ifndef TAOAPPLICATION_H
#define TAOAPPLICATION_H

#include <QIcon>
#include <QObject>

#include "../../include/stdafx.h"
#include "../../include/singleton.h"

#define tApp TaoApplication::getInstance()
class TaoApplicationPrivate;
class TAO_EXPORT TaoApplication : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(TaoApplication)
    Q_SINGLETON_CREATE_H(TaoApplication)
    Q_PROPERTY_CREATE_Q_H(bool, IsEnableMica)
    Q_PROPERTY_CREATE_Q_H(QString, MicaImagePath)
private:
    explicit TaoApplication(QObject* parent = nullptr);
    ~TaoApplication();

public:
    void init();
    void syncMica(QWidget* widget, bool isSync = true);
    static bool containsCursorToItem(QWidget* item);
};

#endif // TAOAPPLICATION_H
