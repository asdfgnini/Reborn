
#include <QApplication>
#include <QDebug>
#include <QThread>
#include "include/Mainwindow.h"



int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    qDebug() << "MAIN is running " << QThread::currentThreadId();


    //初始化整个前端,加载图标字体库
    tApp->init();

    MainWindow maindown;
    maindown.show();

    

    return a.exec();  // 启动事件循环
}
