#ifndef SINGAL_CHART_ITEM_H
#define SINGAL_CHART_ITEM_H

#include "create_signal_item_dialog.h"
#include <QWidget>
 #include<qcustomplot.h>

namespace Ui {
class Singal_Chart_Item;
}

struct Signal_Setting_Info
{
    int AMU;
    Signal_Type Type;
    QString Name;
    QPen Chart_Pen;
    QCPScatterStyle scatterStyle;
    bool Visible;

};

class Singal_Chart_Item : public QWidget
{
    Q_OBJECT

 public slots:
     void Chart_Add_Data(double key, double value);
 public:
    QCPGraph *ion_current_Graph; //离子流曲线指针
    Signal_Setting_Info Setting_Info;
    explicit Singal_Chart_Item(QWidget *parent,QCustomPlot *qcustomplot,Signal_Setting_Info Info);
    ~Singal_Chart_Item();

    void Chart_Setting(Signal_Setting_Info Setting_Info);
    void Clear_Chart();
private slots:

    void on_Color_pushButton_clicked();

    void on_Chart_Visible_pushButton_clicked(bool checked);

private:
    Ui::Singal_Chart_Item *ui;
};

#endif // SINGAL_CHART_ITEM_H
