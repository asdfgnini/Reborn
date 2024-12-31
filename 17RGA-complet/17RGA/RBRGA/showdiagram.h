#ifndef SHOWDIAGRAM_H
#define SHOWDIAGRAM_H

#include <QWidget>
#include "showitem.h"
#include <qvector.h>
#include "qcustomplot.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "qaesencryption.h"
#include <QCryptographicHash>
struct translate{
    double resolution;//分辨率
    int startSpectrum;//质谱范围起点
    int endSpectrum;//质谱范围终点
    QString SN;
    QString name;
    QString time;
};
namespace Ui {
class showDiagram;
}          //注意含不含打印及提示框
        //主页面存数据：saveData：用户名√
        //登录页面:连接网站，读取sn码，校验，发送sn码、name√
        //历史：界面跳转及跳回，查看：getDataFromSqlite+addListItem√(修改插入条件为sn=+其他为空)
        //更新：√
        //删除：
class showDiagram : public QWidget
{
    Q_OBJECT

public:
    QString EncodedText(QString data, QString key);
    QString DecodedText(QString data, QString key);
    explicit showDiagram(QWidget *parent = nullptr);
    ~showDiagram();
    int saveData(translate data= translate{0.0,0,30,"",""},const QVector<QVector<double>> fromUp= QVector<QVector<double>>());
    void insertBaseHistory(const translate &mes);
public slots:
    void Verfiy(QString ID,bool& ver);
    void Search_In_Sqlite(QString ID,QString name,bool& ver,bool& flag);
    void updateBaseHistory(const translate& mes);//不更新xy数据，仅更新分辨率、账户名等
    void updateBaseData(QString ID,const QVector<QVector<double>>& data);//更新xy数据
    void getDataFromSqlite();
protected:
    bool eventFilter(QObject *watched, QEvent *event)override;
signals:
    void SnActively(QString name);
    void windowClosed();
    void closeUnique(int index);
    //void itemNameChanged(QString Name);
private slots:
    void closeEvent(QCloseEvent *event) override;
    //params:需要显示的label数据，需要显示的核心数据
    void addListItem(translate& data,const QVector<QVector<double>>& fromUp);
    //历史记录
    void mouseMove(QMouseEvent *event);
    //LineShowSet:qcustomplot线型设置
    void qcustomplot_receive(const QVector<QVector<double>>& data,int index,VacuumLocal LineShowSet={Qt::blue,0,0,true,""});
    void qcustomplotclose();
    //void on_pushButton_clicked();
    void nameChanged(const QString Namebefore,const QString nameAfter);
private:
    Ui::showDiagram *ui;
    QCPItemLine *lastLine;
    QCPItemTracer *lastPoint;
    QCPItemText *textLabel;
    int showListIndex;//当前展示的list下标
    QSqlQuery *query;
    bool operation;//程序第一次运行
    QSqlDatabase db;
     QVector<QVector<double>> Data;

};

#endif // SHOWDIAGRAM_H
