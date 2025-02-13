#pragma once

#include <QString>
#include <QList>
#include <QMap>
#include <QVariant>
#include <QDebug>
#include <QThread>
#include <QSet>
#include <QPair>
#include <QMutex>
#include <QDir>
#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "../Thread/ThreadPool.h"
#include "../Thread/ThreadCommon.h"
#include "../Thread/ThreadWorkerController.h"
#include "URLManger.h"


// UrlEntry 结构体,写入json文件所使用的
struct UrlEntry {
    QString category;      // 一级目录
    QString path;          // URL 路径
    QMap<QString, QString> params;  // 查询参数
    QString timestamp;     // 时间戳
    QString method;        // HTTP 请求方法
};


class UrlGenerator : public QObject
{
    Q_OBJECT

public:
    explicit UrlGenerator(const QString& ip = "192.168.1.101", int port = 80);
    ~UrlGenerator();


    QList<UrlRequest> findRequestsByIds(const QList<QString>& requestIds);
    QList<UrlRequest> getAllRequests();
    UrlRequest getRequestById(const QString& requestId);
    void deleteRequest(const QString& requestId);
    void clearAllRequests();
    QList<QString> getconnectAllRequestIds();
    QString stepToString(RequestStep step);

    void updateRequestsByFilter(const RequestFilter& filter, 
                                 const std::optional<bool>& newIsRunning = std::nullopt, 
                                 const std::optional<QList<RequestStep>>& newSteps = std::nullopt,
                                 const std::optional<QString>& newResponseFormat = std::nullopt,
                                 const std::optional<int>& newPriority = std::nullopt);

    QList<UrlRequest> getRequestsByFilter(const RequestFilter& filter);
    void removeRequest(const UrlRequest& request);

    void updateRequest(const UrlRequest& request,
                    const std::optional<bool>& newIsRunning = std::nullopt,
                    const std::optional<QList<RequestStep>>& newSteps = std::nullopt,
                    const std::optional<QString>& newResponseFormat = std::nullopt,
                    const std::optional<int>& newPriority = std::nullopt);

    bool findUrlInRequests(const QList<UrlRequest>& requests, const QString& urlPart);
    // 给外界调用，启用URL生成线程,主要响应 连接 按钮的信号
    void startconnect(QString ip, int port,bool checked);
    void stopconnect();

    // 给外界调用，启用URL生成线程,主要响应 扫描 按钮的信号
    void startscan(QString startMass,QString stopMass,QString ppamu,QString dwell,bool scanstate);
    void stopscan();
signals:

   void dataUpdated();
   void dataUpdatedscan(QString startMass,QString stopMass,QString ppamu,QString dwell);
    
public slots:


private:
//URL生成
    //生成URL缓存和生成文件
    bool GenUrlDatawork();
    void GenUrlDataresult(bool result);

    bool ScanGenUrlDatawork();
    void ScanGenUrlDataresult(bool result);

    void addconnectUrlConfig(const QStringList& pathParts, const QList<QPair<QString, QVariant>>& params, const QList<QPair<QString, QVariant>>& paramsForBuffer);
    void addscanUrlConfig(const QStringList& pathParts, const QList<QPair<QString, QVariant>>& params, const QList<QPair<QString, QVariant>>& paramsForBuffer);
    QString generateUrl(const QStringList& pathParts, const QList<QPair<QString, QVariant>>& params);
    
    
    //将存入缓存的url获取ID进行登记
    void connectaddRequestId(const QString& requestId);
    void ScanaddRequestId(const QString& requestId);
    //将URL信息存入文件
    void saveUrlsToFile(const QString& fileName);

    void setRGAip(const QString& ip);
    void setRGAport(int port);

    QString getRGAip();
    int getRGAport();

    void setConnectStatus(bool checked);
    void setScanStatus(bool checked);
    void setConnectDeviceStatus(bool checked);

    bool getConnectStatus();
    bool getScanStatus();
    bool getConnectDeviceStatus();

    void setStartMass(const QString& startMass);
    void setStopMass(const QString& stopMass);
    void setPpamu(const QString& ppamu);
    void setDwell(const QString& dwell);

    QString getStartMass();
    QString getStopMass();
    QString getPpamu();
    QString getDwell();

private:
//URL生成
    QMutex mutex; // 锁用于线程同步，避免多线程问题  
    QString _pRGAip;
    int _pRGAport;  

    //存储到文件的url信息
    QList<QMap<QString, QVariant>>* m_urlConfigs;

    //存储到缓冲区的url信息
    QList<QMap<QString, QVariant>>* m_urlconnectConfigstosave;
    QList<QMap<QString, QVariant>>* m_urlscanConfigstosave;

    //防止重复添加url
    QSet<QString>* m_connectuniqueUrls;
    QSet<QString>* m_scanuniqueUrls;

    //记录存储到缓冲区的url ID号
    QList<QString>* connectm_requestIds;
    QList<QString>* Scanm_requestIds;
    //保存url的缓冲区，给交互线程使用
    UrlRequestManager* requestManager;

    //与界面的连接按钮状态一致
    bool isConnect;
    //与界面的扫描按钮状态一致
    bool isScan;
    //与设备的扫描状态一致
    bool isConnectDevice;
    //是否重复
    bool isrepeatconnect;
    bool isrepeatscan;

    QString m_startMass;
    QString m_stopMass;
    QString m_ppamu;
    QString m_dwell;
};
