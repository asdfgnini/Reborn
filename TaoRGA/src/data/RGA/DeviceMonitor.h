#pragma once


#include "../Thread/MyThread.h"
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QTimer>
#include <QDebug>
#include "UrlGenerator.h"
#include <QtConcurrent/QtConcurrent>


enum class ResponseFormat {
    JSON,
    XML,
    PlainText,
    Binary
};



class DeviceMonitor : public MyThread
{
    Q_OBJECT
public:
    explicit DeviceMonitor(QObject *parent = nullptr,UrlGenerator* urlGenerator = nullptr);
    ~DeviceMonitor();

    void setissequence(bool issequence);
    bool getissequence();

    void setm_filter(const RequestFilter& filter);
    RequestFilter getm_filter();

    void setisFirstConnect(bool isFirstConnect);
    bool getisFirstConnect();

    // 添加新的工具函数
    static QVariant getJsonValue(const QJsonObject& json, const QString& path, const QVariant& defaultValue = QVariant());
    static QJsonObject parseJsonString(const QString& jsonString, bool* ok = nullptr);

signals:
    void deviceConnected(QString);
    void deviceDisconnected(QString);

    void stopconnectSignal();
    void parseResult(const QString& url, const QString& key, const QVariant& value);

public slots:

    void startconnect();
    void stopconnect();
private slots:


protected:
    void processTask() override;

private:
    // 用于分割 JSON 路径的辅助函数
    static QStringList splitJsonPath(const QString& path);
    bool HttpRequestSync(const QString& urlStr, QString& retStr, QString& err, int timeout, ResponseFormat format);
    QList<UrlRequest> sortRequestsByPriority(const QList<UrlRequest>& requests);

private:
    QMutex m_mutex;
    UrlGenerator* m_urlGenerator;
    bool issequence;
    RequestFilter m_filter;
    bool isFirstConnect;
};



// // 假设有如下JSON数据：
// /*
// {
//     "user": {
//         "name": "张三",
//         "age": 25,
//         "active": true,
//         "scores": [85, 90, 95],
//         "address": {
//             "city": "北京",
//             "street": "朝阳区"
//         }
//     }
// }
// */

// // 可以这样使用：
// QJsonObject json = parseJsonString(jsonString);
// QString name = getJsonValue(json, "user.name", "未知").toString();  // 返回 "张三"
// int age = getJsonValue(json, "user.age", 0).toInt();  // 返回 25
// bool active = getJsonValue(json, "user.active", false).toBool();  // 返回 true
// int firstScore = getJsonValue(json, "user.scores.0", 0).toInt();  // 返回 85
// QString city = getJsonValue(json, "user.address.city", "未知").toString();  // 返回 "北京"

// // 处理不存在的路径
// QString notExist = getJsonValue(json, "user.notexist", "默认值").toString();  // 返回 "默认值"