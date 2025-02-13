#pragma once
#include <QThread>

#include <QDebug>
#include <qmutex.h>
#include <qwidget.h>
// #include "../BufferStruct/RingBuffer.h"
#include "../protocol/HttpClient.h"
#include "UrlGenerator.h"
#include "../Tools/singleton.h"
#include "ScanMessageParser.h"
#include "../../include/ui.h"


enum class ResponseFormat {
    JSON,
    XML,
    PlainText,
    Binary
};


class DataFetcher : public QThread
{
    Q_OBJECT

public:

    DataFetcher(UrlGenerator* urlGenerator);

    // 获取当前交互步骤
    RequestStep getStep();

    // 获取 scanSetupChannels 变量（线程安全）解析要用
    QList<ScanSetupChannel> getScanSetupChannels() const;
    
    // 设置 scanSetupChannels 变量（线程安全）解析要用
    void setScanSetupChannels(const QList<ScanSetupChannel>& channels);


    
    void setScanDevice(bool isScanDevice);
    bool getScanDevice();
    bool getConnectStatus();
    bool getScanStatus();

    void setissequence(bool issequence);
    bool getissequence();
    void setm_filter(const RequestFilter& filter);
    RequestFilter getm_filter();

    bool getJsonField(const QString &jsonStr); 
    //启动交互线程
    void startFetching();
    //停止交互线程
    void stopFetching();
    //暂停交互线程
    void pauseFetching();

    ~DataFetcher();
signals:
    void stopconnectSignal();
    void stopscanSignal();

    //传递设备扫描状态
    void scandeviceSignal(bool isScanDevice);
    //传递连接是否成功状态
    void connectStatusSignal(bool isConnect);

public slots:
    void setScanStatus(QString startMass,QString stopMass,QString ppamu,QString dwell,bool isScan);

    //设置与界面连接按钮状态一致
    void setConnectStatus(QString ip, int port,bool checked);
    //停止扫描，修改url缓冲区的url属性，发送停止扫描url,然后只有轮询url存在，回到刚连接成功的状态
    void stopScan();
    //用于扫描按钮按下后，数据准备完成后，启动下一交互步骤
    void startScan();
    void startconnect();
    void stopconnect();

private:
    QList<UrlRequest> sortRequestsByPriority(const QList<UrlRequest>& requests);
    //设置当前交互步骤
    void setStep1();
    void setStep2();
    void setStep3();
    void setStep4();
    void setStep5();    
    void setpauseStep();

    bool HttpRequestSync(const QString& urlStr, QString& retStr, QString& err, int timeout, ResponseFormat format);
    bool HttpRequestSync(const QString& urlStr, QString& retStr, QString& err, ResponseFormat format);

    void processStep1();
    void processStep2();
    void processStep3();
    void processStep4();
    void processStep5();
    

protected:
    void run() override;

private:
    QMutex mutex;
    // //主线程获取每轮数据的缓冲区
    // RingBuffer *m_buffer;
    //URl生成器
    UrlGenerator* m_urlGenerator;
    //是否退出本交互线程的标志位
    bool exitCondition;
    //与界面连接按钮状态一致 
    bool isConnect;
    //与界面扫描按钮状态一致
    bool isScan;
    //与设备扫描状态一致
    bool isScanDevice;
    //交互线程的交互步骤
    RequestStep currentStep;
    //数据解析类
    ScanMessageParser* m_scanMessageParser;
    //表示当前的事件所获取的url是否需要进行排序
    bool issequence;   

    //请求过滤器
    RequestFilter m_filter;

    //请求格式
    ResponseFormat m_format;
};
