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
    #include "URLManger.h"


    // UrlEntry 结构体
    struct UrlEntry {
        QString category;      // 一级目录
        QString path;          // URL 路径
        QMap<QString, QString> params;  // 查询参数
        QString timestamp;     // 时间戳
        QString method;        // HTTP 请求方法
    };

    class UrlGenerator : public QObject {
        Q_OBJECT
    public:
        // 构造函数
        UrlGenerator(const QString& ip = "192.168.1.101", int port = 80)
            : m_ip(ip), m_port(port) {}

        ~UrlGenerator() = default;


        QString stepToString(RequestStep step)
        {
            return requestManager.stepToString(step);
        }

        // 在子线程中处理 URL 配置列表,多次调用会创建多个对象，最好适合执行一次的任务，多次调用建议采用线程池封装
        void startUrlGenerationInThread() {
            // 创建并启动子线程
            QThread* workerThread = new QThread();
            this->moveToThread(workerThread);

            // 连接信号与槽，启动子线程操作
            QObject::connect(workerThread, &QThread::started, [this]() {
                this->onGenerateAndAddUrlsToChain();
            });
            QObject::connect(this, &UrlGenerator::destroyed, workerThread, &QThread::quit);
            QObject::connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater);

            // 启动线程
            workerThread->start();
        }

        // 修改url的参数值
        void updateUrlConfig(const QStringList& pathParts, const QString& paramName, const QVariant& newValue) {
            QMutexLocker locker(&mutex);

            // 查找目标 URL 配置
            for (auto& config : m_urlConfigs) {
                // 获取当前路径配置
                QStringList currentPath = config["path"].toStringList();
                
                // 如果路径匹配
                if (currentPath == pathParts) {
                    // 获取查询参数列表
                    QList<QPair<QString, QVariant>> params = qvariant_cast<QList<QPair<QString, QVariant>>>(config["params"]);

                    // 查找并修改指定参数的值
                    bool paramFound = false;
                    for (auto& param : params) {
                        if (param.first == paramName) {
                            param.second = newValue;  // 修改参数值
                            paramFound = true;
                            break;
                        }
                    }

                    // 如果参数不存在，则添加新的参数
                    if (!paramFound) {
                        params.append(QPair<QString, QVariant>(paramName, newValue));
                    }

                    // 更新 URL 配置
                    config["params"] = QVariant::fromValue(params);

                    // 打印修改后的 URL
                    QString updatedUrl = generateUrl(currentPath, params);
                    qDebug() << "Updated URL: " << updatedUrl;
                    return;
                }
            }

            qDebug() << "URL path not found: " << pathParts.join("/");
        }

        void addUrlConfig(const QStringList& pathParts, const QList<QPair<QString, QVariant>>& params, const QList<QPair<QString, QVariant>>& paramsForBuffer) {
            QMutexLocker locker(&mutex);

            // 构建 URL
            QString url = generateUrl(pathParts, params);

            // 将参数存储到 QMap<QString, QString> 中
            QMap<QString, QString> paramMap;
            for (const auto& param : params) {
                paramMap.insert(param.first, param.second.toString());
            }

            // 获取一级目录 (去掉 mmsp 作为根)
            QString category = pathParts.isEmpty() ? "" : pathParts[1];

            // 如果没有重复的 URL，则添加
            if (!m_uniqueUrls.contains(url)) {
                m_uniqueUrls.insert(url);

                // 创建新的 URL 配置项
                QMap<QString, QVariant> config;
                config.insert("category", category);
                config.insert("path", url);
                config.insert("params", QVariant::fromValue(paramMap));
                config.insert("method", "GET");  // 默认请求方法

                // 添加到 URL 配置列表
                m_urlConfigs.append(config);

                QMap<QString, QVariant> configToSave;
                configToSave.insert("category", category);
                configToSave.insert("path", url);
                configToSave.insert("params", QVariant::fromValue(paramMap));
                configToSave.insert("method", "GET");


        for (const auto& param : paramsForBuffer) {
            if (param.first == "isRunning") {
                configToSave.insert("isRunning", param.second.toBool());
            }
            else if (param.first == "step") {
                // 获取 step 字符串
                QString stepString = param.second.toString();
                
                // 将 step 字符串转换为枚举值
                RequestStep step = requestManager.stringToStep(stepString);  // 转换为枚举值

                // 将枚举值存入 configToSave，若需要存储为整数
                configToSave.insert("step", static_cast<int>(step));  // 转换为整数存储
                
                // 如果需要存储为字符串，也可以直接存储
                // configToSave.insert("step", stepString);
            }
        }


                // 添加到 URL 配置列表
                m_urlConfigstosave.append(configToSave);
            } else {
                qDebug() << "Duplicate URL: " << url;
            }
        }

        QList<UrlRequest> findRequestsByIds(const QList<QString>& requestIds)
        {
            return requestManager.findRequestsByIds(requestIds);
        }

        // 新增：获取所有请求
        QList<UrlRequest> getAllRequests() {
            return requestManager.getAllRequests();
        }

        // 新增：根据请求 ID 获取请求
        UrlRequest getRequestById(const QString& requestId) {
            return requestManager.findRequest(requestId);
        }

        // 新增：删除请求
        void deleteRequest(const QString& requestId) {
            requestManager.removeRequest(requestId);
            qDebug() << "Request deleted: " << requestId;
        }

        // 新增：清空所有请求
        void clearAllRequests() {
            requestManager.removeAllRequests();
            qDebug() << "All requests cleared.";
        }

        // 获取所有生成的请求 ID
        QList<QString> getAllRequestIds() {
            QMutexLocker locker(&mutex);  // 使用锁保护共享数据
            return m_requestIds;
        }

    signals:
        void dataUpdated();

    public slots:
        void onGenerateAndAddUrlsToChain() {
            qDebug() << "onGenerateAndAddUrlsToChain is running " << QThread::currentThreadId();

            // 配置 URL 列表
        addUrlConfig({"mmsp", "communication", "controlInfo", "get"}, {}, {{"isRunning", false},{"step","Step1"}});
        // addUrlConfig({"mmsp", "communication", "issueLog", "get"}, {},{{"isRunning", false},{"step","Step2"}});
        // addUrlConfig({"mmsp", "communication", "control", "set"}, {{"force", QVariant()}},{{"isRunning", false},{"step","Step2"}});

        // addUrlConfig({"mmsp", "generalControl", "set"}, {{"setEmission", "On"}},{{"isRunning", false},{"step","Step2"}});

        // addUrlConfig({"mmsp", "status", "systemStatus", "get"}, {},{{"isRunning", false},{"step","Step2"}});

        // addUrlConfig({"mmsp", "measurement", "get"}, {},{{"isRunning", false},{"step","Step2"}});
        // addUrlConfig({"mmsp", "measurement", "binaryData","get"}, {{"@start","0"}},{{"isRunning", false},{"step","Step2"}});

        // addUrlConfig({"mmsp", "scanInfo", "get"}, {},{{"isRunning", false},{"step","Step2"}});   

        // addUrlConfig({"mmsp", "electronicsInfo", "get"}, {},{{"isRunning", false},{"step","Step2"}});

        // addUrlConfig({"mmsp", "sensorFiler", "get"}, {},{{"isRunning", false},{"step","Step2"}});
        
        // addUrlConfig({"mmsp", "sensorIonSource", "get"}, {},{{"isRunning", false},{"step","Step2"}});

        // addUrlConfig({"mmsp", "sensorInfo", "get"}, {},{{"isRunning", false},{"step","Step2"}});


            // 遍历配置列表生成 URL 请求
            for (const auto& config : m_urlConfigstosave) {
                // 生成 URL
                QString url = generateUrl(config["path"].toStringList(), qvariant_cast<QList<QPair<QString, QVariant>>>(config["params"]));
                qDebug() << "Generated URL: " << url;
                // 创建 UrlRequest 对象
                UrlRequest request;
                request.url = url;
                
                if (config.contains("isRunning") && config["isRunning"].toBool()) {
                    request.isRunning = true;
                }

                if (config.contains("step")) {
                    int stepValue = config["step"].toInt();  // 获取 step 的整数值

                    // 检查该值是否在合法的范围内（1 到 5），然后转换为枚举类型
                    if (stepValue >= 1 && stepValue <= 5) {
                        request.step = static_cast<RequestStep>(stepValue);  // 将整数转换为 RequestStep 枚举
                    } else {
                        // 处理非法值
                        qDebug() << "Invalid step value: " << stepValue << ", defaulting to Invalid";
                        request.step = RequestStep::Invalid;  // 设置为 Invalid 表示无效
                    }
                }


                // 将请求添加到 UrlRequestManager
                QString requestId = requestManager.addRequest(request);

                // UrlRequest* request1 = requestManager.findRequest(requestId);
                // qDebug() << "URL In UrlRequestManager:" << request1->url << "isRunning:" << request1->isRunning;
                qDebug() << "已存入UrlRequestManager Request ID: " << requestId;            // 保存生成的请求 ID

                addRequestId(requestId);


            }

            saveUrlsToFile("RGA.json");
            qDebug() << "已存入文件";

            emit dataUpdated();
        }

        // 子线程中添加请求ID
        void addRequestId(const QString& requestId) {
            QMutexLocker locker(&mutex);  // 使用锁保护共享数据
            m_requestIds.append(requestId);
        }

    private:
        QString m_ip;  // IP 地址
        int m_port;    // 端口

        QList<QMap<QString, QVariant>> m_urlConfigs; // 存储 URL 配置
        QList<QMap<QString, QVariant>> m_urlConfigstosave; // 存储 URL 配置
        UrlRequestManager requestManager;
        QSet<QString> m_uniqueUrls;  // 用于去重的集合
        QMutex mutex; // 锁用于线程同步，避免多线程问题

        QList<QString> m_requestIds;  // 存储生成的请求 ID

        // 生成 URL
        QString generateUrl(const QStringList& pathParts, const QList<QPair<QString, QVariant>>& params) {
            QString baseUrl = QString("http://%1:%2").arg(m_ip).arg(m_port);  // 使用宏来构建基础 URL
            QString path = pathParts.join("/");

            // 如果路径没有包含协议，就加上基础 URL
            if (!path.startsWith("http://")) {
                path = baseUrl + "/" + path;
            }

            // 生成查询参数
            QStringList queryParams;
            for (const auto& param : params) {
                // 如果参数没有值，跳过它
                if (!param.second.isNull() && !param.second.toString().isEmpty()) {
                    queryParams.append(QString("%1=%2").arg(param.first).arg(param.second.toString()));
                } else {
                    queryParams.append(param.first);  // 没有值时只加参数名
                }
            }

            // 拼接完整的 URL
            if (!queryParams.isEmpty()) {
                return path + "?" + queryParams.join("&");
            } else {
                return path;
            }
        }

    void saveUrlsToFile(const QString& fileName) {
        QString appDir = QCoreApplication::applicationDirPath();
        QString urlsDir = appDir + "/urls";  // 设置 URLs 文件存储目录
        QDir dir(urlsDir);
        if (!dir.exists()) {
            dir.mkpath(urlsDir);
        }

        QString filePath = urlsDir + "/" + fileName;

        QMap<QString, QList<UrlEntry>> groupedUrls;

        // 将每个 URL 配置项转换为 UrlEntry 并按 category 分组
        for (const auto& config : m_urlConfigs) {
            UrlEntry entry;
            entry.category = config["category"].toString();
            entry.path = config["path"].toString();
            entry.timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);  // 保存本地时间
            entry.method = "GET";  // 请求方法可以扩展

            // 将 URL 请求分组
            groupedUrls[entry.category].append(entry);
        }

        // 保存到文件
        QJsonObject jsonObj;
        for (auto category : groupedUrls.keys()) {
            QJsonArray categoryArray;
            for (const auto& entry : groupedUrls[category]) {
                QJsonObject entryObj;
                entryObj["path"] = entry.path;
                entryObj["timestamp"] = entry.timestamp;
                entryObj["method"] = entry.method;
                categoryArray.append(entryObj);
            }
            jsonObj[category] = categoryArray;
        }

        // 写入文件
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
            QJsonDocument doc(jsonObj);
            file.write(doc.toJson());
        }
    }

};
