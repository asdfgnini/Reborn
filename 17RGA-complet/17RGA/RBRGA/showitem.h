#ifndef SHOWITEM_H
#define SHOWITEM_H

#include <QWidget>
#include <qvector.h>
#include <QDateTime>
#include <QColorDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class showItem;
}
QT_END_NAMESPACE

struct VacuumLocal{
    QColor Color;
    int lineStyle;//线型0-2，Qt::SolidLine、Qt::DashLine、Qt::DashDotLine
    int lineWidth;//2-5
    bool lineShowPoint;//true-QCPScatterStyle::ssNone,false-QCPScatterStyle::ssDisc
    QString SN;//SN码
};

class showItem : public QWidget
{
    Q_OBJECT

public:
    showItem(QWidget *parent = nullptr);
    ~showItem();
    QString currentDateTime;
    QIcon lineStyleIcons[3],lineWidthIcons[4];
    QVector<QVector<double>> scanResults;//xy数据
    void setLocal(QString,QString);//
    int index;//在上层listwidget的下标
protected:
    //void focusInEvent(QFocusEvent *e) override;
signals:
    void scanResultsData(const QVector<QVector<double>> data,int index,VacuumLocal LineShowSet);//发
    void scanResultsClose();//关闭
    void plotColorChange(QColor color);
    void LineStyle(int index);//改类型
    void LineWidth(int index);
    void LineShow(bool index);
    // void NameChanged(const QString Namebefore,const QString& name);

public slots:
    void closeClick2(int indexUp);
    void ItemNameChanged(QString Name);
private slots:
    void on_check_1_1_clicked();
    void on_check_2_clicked();
    //void SelectChanged(int index);
    void on_check_1_2_clicked();
    void on_check_1_3_clicked();
    void on_check_1_4_clicked();

private:
    Ui::showItem *ui;
    bool pushbutton2_check;
    //translate local;//label数据
    VacuumLocal LineShowSet;//线型数据

    //QString NameBefore;//更改前的数据库表名

};
#endif // SHOWITEM_H
