#include <QTimer>
#include <QApplication>
#include <QDebug>

#include <QThread>
#include "include/Mainwindow.h"
#include "include/URLManger.h"
#include "include/UrlGenerator.h"

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    qDebug() << "MAIN is running " << QThread::currentThreadId();

    // MainWindow maindown;
    // maindown.show();

    // UrlRequestManager manager;
    // // 创建一个请求
    // UrlRequest request = {"http://example.com", true, false};
    // // 增加请求并获取唯一ID
    // QString requestId = manager.addRequest(request);
    // qDebug() << "Request ID:" << requestId;
    // // 使用ID查找请求
    // UrlRequest* foundRequest = manager.findRequest(requestId);
    // if (foundRequest) {
    //     qDebug() << "Found request with URL:" << foundRequest->url;
    // }
    // // 更新请求的属性（仅更新 isRunning 和 isPeriodic）
    // manager.updateRequest(requestId, std::nullopt, false, true);  // 只更新 isRunning 和 isPeriodic
    // // 查找并输出更新后的请求
    // foundRequest = manager.findRequest(requestId);
    // if (foundRequest) {
    //     qDebug() << "Updated request with URL:" << foundRequest->url
    //              << ", isRunning:" << foundRequest->isRunning
    //              << ", isPeriodic:" << foundRequest->isPeriodic;
    // }
    // // 删除请求
    // manager.removeRequest(requestId);

    UrlGenerator urlGen1({"scanSetup", "channels", "5"}, "set",
                    {{"channelMode", "TotalPressure"}, {"dwell", "1"}, {"enabled", "True"}});
    QUrl url1 = urlGen1.generateUrl();
    qDebug() << "Generated URL 1: " << url1.toString(); 

    // 创建 UrlEntry 对象
    UrlEntry entry = urlGen1.createUrlEntry("配置扫描通道 5", "scanSetup");
    
    // 将生成的 URL 写入 JSON 文件
    urlGen1.writeUrlToJson("urls.json", entry);



    return a.exec();  // 启动事件循环
}
