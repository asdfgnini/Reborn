#pragma once

#include <QString>
#include <QUrl>
#include <QUrlQuery>
#include <QMap>
#include <QList>
#include <QDebug>
#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDir>

// 定义宏来设置 IP 和端口
#define IP_ADDRESS "192.168.1.101"
#define PORT 80

// UrlEntry 结构体
struct UrlEntry {
    QString category;      // 一级目录
    QString path;          // URL 路径
    QMap<QString, QString> params;  // 查询参数
    QString description;   // 描述信息
    QString timestamp;     // 时间戳
    QString method;        // HTTP 请求方法
};

// UrlGenerator 类
class UrlGenerator {
public:
    // 使用宏定义的 IP 和端口来初始化
    UrlGenerator(QStringList categories = {}, QString dynamicPath = "",
                 QMap<QString, QString> queryParams = {})
        : m_ip(IP_ADDRESS), m_port(PORT), m_categories(categories), 
          m_dynamicPath(dynamicPath), m_queryParams(queryParams) {}

    // 设置多级目录
    void setCategories(const QStringList& categories) {
        m_categories = categories;
    }

    // 设置动态路径（可选）
    void setDynamicPath(const QString& dynamicPath) {
        m_dynamicPath = dynamicPath;
    }

    // 添加查询参数
    void addQueryParameter(const QString& key, const QString& value) {
        m_queryParams[key] = value;
    }

    // 生成 URL
    QUrl generateUrl() const {
        QString urlStr = "http://" + m_ip + ":" + QString::number(m_port) + "/mmsp/";

        // 拼接目录
        for (const QString& category : m_categories) {
            urlStr += category + "/";
        }

        // 添加动态路径（如果有）
        if (!m_dynamicPath.isEmpty()) {
            urlStr += m_dynamicPath;
        }

        // 添加查询参数
        if (!m_queryParams.isEmpty()) {
            urlStr += "?";
            for (auto it = m_queryParams.begin(); it != m_queryParams.end(); ++it) {
                urlStr += it.key() + "=" + it.value();
                if (it != std::prev(m_queryParams.end())) {
                    urlStr += "&";
                }
            }
        }

        return QUrl(urlStr);
    }

    // 创建 UrlEntry 对象
    UrlEntry createUrlEntry(const QString& description, const QString& category) const {
        UrlEntry entry;
        entry.category = category;
        entry.path = generateUrl().toString();
        entry.description = description;
        entry.timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
        entry.method = "GET";  // 假设默认请求方法为 GET
        return entry;
    }

    // 写入 URL 数据到 JSON 文件的函数
    void writeUrlToJson(const QString& filename, const UrlEntry& entry) {
        // 获取当前程序的工作目录
        QString currentDir = QDir::currentPath();  // 获取当前工作目录
        QString folderPath = currentDir + "/urls"; // 指定新目录为 "urls"
        QDir dir(folderPath);

        // 如果文件夹不存在，创建它
        if (!dir.exists()) {
            if (!dir.mkpath(".")) {  // 创建文件夹及其父文件夹
                qDebug() << "Failed to create directory:" << folderPath;
                return;
            }
        }

        // 创建 JSON 对象表示当前 URL
        QJsonObject entryObject;
        entryObject["path"] = entry.path;

        // 写入查询参数
        if (!entry.params.isEmpty()) {
            QJsonObject paramsObject;
            for (auto it = entry.params.begin(); it != entry.params.end(); ++it) {
                paramsObject[it.key()] = it.value();
            }
            entryObject["params"] = paramsObject;
        }

        // 添加其他信息：描述、时间戳、请求方法等
        entryObject["description"] = entry.description;
        entryObject["timestamp"] = entry.timestamp;
        entryObject["method"] = entry.method;

        // 打开文件并读取现有内容
        QFile file(folderPath + "/" + filename);  // 使用新创建的 "urls" 目录
        if (!file.open(QIODevice::ReadWrite | QIODevice::Append)) {
            qDebug() << "Error opening file for writing";
            return;
        }

        QJsonDocument doc;
        QJsonObject rootObj;

        // 如果文件非空，读取原始内容
        if (file.size() > 0) {
            QByteArray existingData = file.readAll();
            doc = QJsonDocument::fromJson(existingData);
            rootObj = doc.object();
        }

        // 将新 URL 添加到相应的一级目录数组中
        if (!rootObj.contains(entry.category)) {
            rootObj[entry.category] = QJsonArray();
        }

        QJsonArray entriesArray = rootObj[entry.category].toArray();
        entriesArray.append(entryObject);
        rootObj[entry.category] = entriesArray;

        // 保存 JSON 数据到文件
        doc.setObject(rootObj);
        file.resize(0);  // 清空文件内容
        file.write(doc.toJson());
        file.close();
        qDebug() << "URL data written to" << folderPath + "/" + filename;
    }

private:
    QString m_ip;
    int m_port;
    QStringList m_categories;
    QString m_dynamicPath;
    QMap<QString, QString> m_queryParams;
};


