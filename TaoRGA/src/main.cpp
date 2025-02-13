
#include <QApplication>
#include <QDebug>
#include <QThread>
#include "./ui/Mainwindow.h"
#include "../libs/ui/include/ElaApplication.h"
#include "include/data.h"


#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <Qvariant>
#include <QByteArray>
#include <qurl.h>

#include <QFuture>
#include <QPromise>




// // 模板函数，支持不同的数据类型
// template <typename T>
// void fetchData(const QString &url, const QString &contentType, std::function<void()> onComplete = nullptr)
// {
//     HttpClient client(url);

//     client.header("Content-Type", contentType)
//         .success([url](const QString &response) {
//             // 打印 URL 信息
//             qDebug() << "Request URL: " << url;

//             if constexpr (std::is_same_v<T, QByteArray>) {
//                 // 处理二进制数据
//                 QByteArray binaryData = response.toUtf8();
//                 qDebug() << "Response received, size: " << binaryData.size() << " bytes";
//                 qDebug() << "Binary data (hex): " << binaryData.toHex();
//             } 
//             else if constexpr (std::is_same_v<T, QVariant>) 
//             {
//                 // 假设返回的是 JSON 格式的数据
//                 QVariant jsonData = QVariant::fromValue(response);
//                 qDebug() << "Response received as QVariant: " << jsonData.toString();
//             }
//             else {
//                 qDebug() << "Unknown data type for response.";
//             }
//         })
//         .fail([](const QString &error, int statusCode) {
//             qDebug() << "Error: " << error << ", Status Code: " << statusCode;
//         })
//         .complete([onComplete,url]() {
//             qDebug() << "Request completed.";
//             qDebug() << "当前请求的URL是: " << url;
//             qDebug() << "----------------------------";

//             // 如果传入了 onComplete 回调，则执行它
//             if (onComplete) {
//                 onComplete();
//             }
//         })
//         .debug(true);

//     client.get();
// }







int main(int argc, char *argv[]) 
{
    QApplication a(argc, argv);

#ifdef TAO_DEBUG
    qDebug() << "MAIN is running " << QThread::currentThreadId();
#endif

    // ScanSetupChannel::ScanSetupChannelTest();

    // // 测试数据
    // QByteArray byteArray = QByteArray::fromHex("0A0B0C0D 04030030 00440000 00000000 0001622B 000000D5 00000030");
    // qDebug() << "byteArry size" << byteArray.size();

    // QString hexString = byteArray.toHex();
    // qDebug() << "generated test data (hex): " << hexString;

    // // 创建 ScanSetupChannels 对象
    // QList<ScanSetupChannel> scanSetupChannels;
    // scanSetupChannels.append(ScanSetupChannel(1, true, "FixedNumber", 0, 0,10, "U32Value"));
    // scanSetupChannels.append(ScanSetupChannel(2, true, "Timestamp", 0, 0, 10, "U32Value"));
    // scanSetupChannels.append(ScanSetupChannel(3, true, "SystemStatus", 0, 0, 10, "U32Value"));
    // scanSetupChannels.append(ScanSetupChannel(4, true, "Sweep", 0, 200, 1, "F32Value"));
    // scanSetupChannels.append(ScanSetupChannel(5, true, "TotalPressure", 0, 0, 1, "F32Value"));
    // scanSetupChannels.append(ScanSetupChannel(6, true, "Baseline", 0, 0, 1, "F32Value"));
    // scanSetupChannels.append(ScanSetupChannel(7, true, "TPQuad", 0, 0, 1, "F32Value"));
    // scanSetupChannels.append(ScanSetupChannel(8, true, "TotalPressureDetectorCurrent", 0, 0, 1, "F32Value"));
    // scanSetupChannels.append(ScanSetupChannel(9, true, "EmissionCurrent", 0, 0, 1, "U32Value"));
    // scanSetupChannels.append(ScanSetupChannel(10, true, "AnodePotential", 0, 0, 1, "U32Value"));
    // scanSetupChannels.append(ScanSetupChannel(11, true, "ElectronEnergy", 0, 0, 1, "U32Value"));
    // scanSetupChannels.append(ScanSetupChannel(12, true, "FocusPotential", 0, 0, 1, "U32Value"));
    // scanSetupChannels.append(ScanSetupChannel(13, true, "FilamentCurrent", 0, 0, 1, "U32Value"));

    // // 创建 ScanMessageParser 对象
    // ScanMessageParser parser(scanSetupChannels);

    // parser.parse(byteArray);
     // 初始化 UrlRequestManager 实例
    //单独调用
    // fetchData<QVariant>("http://httpbin.org/get", "application/json");
    // fetchData<QVariant>("http://jsonplaceholder.typicode.com/posts/1", "application/json");
    // fetchData<QVariant>("http://jsonplaceholder.typicode.com/posts/2", "application/json");

    // 链式调用
    // 在主函数中调用多个请求
    // fetchData<QVariant>("http://httpbin.org/get", "application/json", []() {
    //     qDebug() << "First request completed, now requesting second.";
    //     fetchData<QVariant>("http://reqres.in/api/users/1", "application/json", []() {
    //         qDebug() << "Second request completed, now requesting third.";
    //         fetchData<QVariant>("http://jsonplaceholder.typicode.com/posts/1", "application/json", []() {
    //             qDebug() << "Third request completed.";
    //         });
    //     });
    // });

    // UrlController urlController;
    // // urlController.startConnect("192.168.1.100", 80,true);
    // urlController.startScan("0", "100", "1", "32", true);

    // 在主线程中使用



    eApp->init();
    MainWindow maindown;
    maindown.show();

    return a.exec();  // 启动事件循环
}



