#ifndef TAOMICABASEINITOBJECT_H
#define TAOMICABASEINITOBJECT_H

#include <QObject>
class TaoApplicationPrivate;
class TaoMicaBaseInitObject : public QObject
{
    Q_OBJECT
public:
    explicit TaoMicaBaseInitObject(TaoApplicationPrivate* appPrivate, QObject* parent = nullptr);
    ~TaoMicaBaseInitObject();
    Q_SLOT void onInitMicaBase(QImage img);
Q_SIGNALS:
    Q_SIGNAL void initFinished();

private:
    TaoApplicationPrivate* _appPrivate{nullptr};
};

#endif // TAOMICABASEINITOBJECT_H
