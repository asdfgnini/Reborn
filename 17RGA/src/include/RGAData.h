#ifndef RGADATA_H
#define RGADATA_H

#include <QObject>
#include "HttpClient.h"
#include "mutiltimer_thread.h"

class RGAData : public QObject
{
    Q_OBJECT

public:
    explicit RGAData(QObject *parent = nullptr);
    void start();
    
private slots:
    void fetchDeviceStatus();
    void fetchDeviceData();

    void onDeviceStatusFetched(const QString &data);
    void onDeviceDataFetched(const QString &data);

private:
    HttpClient* m_client;
    MultiMY_Timer* m_timerManager;
    int m_statusTimerId;
    int m_dataTimerId;
};

#endif // RGADATA_H
