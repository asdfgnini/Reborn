#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QMutex>

// Scan_Data 结构体定义
struct Scan_Data {
    int AMU_Start;
    int AMU_Stop;
    float Ppamu;
    int Dwell;
    bool Scan_Switch;
    int Current_Scan_Number;
    float Current_TotalPressure;
    float Current_Baseline;
    float Current_TPQuad;
    int Scan_size;
    int Start_Point;
    int Cur_Data_Size;
    int Data_Load_Heavy;
    QList<double> Scan_data_List;
};

class HttpClient : public QObject
{
    Q_OBJECT

public:
    explicit HttpClient(QObject* parent = nullptr);
    ~HttpClient();
    void fetchDeviceStatus(const QString& url);
    void fetchDeviceData(const QString& url);

signals:
    void deviceStatusFetched(const QString& data);
    void deviceDataFetched(const QString& data);

private slots:
    void onStatusFetched();
    void onDataFetched();

private:
    QNetworkAccessManager* m_manager;
    QMutex* m_mutex;  // 互斥锁，确保线程安全
};

#endif // HTTPCLIENT_H
