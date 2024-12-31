// HttpClient.h
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
    explicit HttpClient(QObject *parent = nullptr);
    ~HttpClient();

    // 发送GET请求并获取数据
    void fetchData(const QString &url);

signals:
    void dataFetched(const QVariantMap &data);

private slots:
    void onFinished();

private:
    void saveJsonToFile(const QByteArray &data);


    QNetworkAccessManager *m_manager;
    bool m_isRequestInProgress;  // 请求是否正在进行的标志
    QNetworkReply *m_reply;
    QString m_currentUrl;  // 保存请求的 URL
};

#endif // HTTPCLIENT_H
