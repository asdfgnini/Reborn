#ifndef LOG_H
#define LOG_H

#include <QWidget>
#include "showdiagram.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>


#include "api.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include <QRegExp>

#include <QJsonDocument>        //json文档
#include <QJsonObject>          //json对象  {}
#include <QJsonArray>           //json数组  []
#include <QJsonValue>           //json的value值
QT_BEGIN_NAMESPACE
namespace Ui {
class Log;
}
QT_END_NAMESPACE
// enum Communication_State
// {
//     HTTP_NO_CONNECT,
//     HTTP_CONNECTING,
//     HTTP_OK_CONNECT
// };
class Log : public QWidget
{
    Q_OBJECT

public:
    Log(QWidget *parent = nullptr);
    ~Log();
    showDiagram *histShow;//历史记录页面类
signals:
    void verfiy(QString ID,bool& ver);
    void searchInSqlite(QString ID,QString name,bool& ver,bool& flag);
    void mesAfterLog(QString ID,QString name);

private slots:
    void on_pushButton_log_clicked();

    void on_pushButton_HistoryData_clicked();

    void on_pushButton_verification_clicked();
    void netReply(QNetworkReply *);

    void on_pushButton_users_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_amplify_clicked();

    void on_pushButton_reduce_clicked();

private:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    Ui::Log *ui;
    QPoint whereismouse;
    Communication_State RGA_Communication_Stat;
    QTcpSocket *socket;
    QNetworkAccessManager *manager;
    QString serialNumber;
    bool ver;//true:含有，flase没有
};
#endif // LOG_H
