#ifndef CHART_H
#define CHART_H

#include <QWidget>
 #include<qcustomplot.h>
#include "singal_chart_item.h"
#include "create_signal_item_dialog.h"  //创建曲线窗口
namespace Ui {
class Chart;
}

class Chart : public QWidget
{
    Q_OBJECT

public:
    bool x_Auto_Flash_Switch;
    bool y_Auto_Flash_Switch;
    QCustomPlot *QCBar;

public:

    explicit Chart(QWidget *parent,API *RGA);
    ~Chart();
public slots:
    // void Chart_Flash();
    void Received_Signal(Signal_Type Type,double AMU,int Time,double value);
    void Create_Item(Signal_Type Type ,int AMU);
    void Chart_Clear();
private slots:

    void on_X_Chart_Flash_PushButton_clicked(bool checked);
    void on_Y_Chart_Flash_PushButton_clicked(bool checked);
    void on_Remove_Signal_pushButton_clicked();
    void on_Add_Signal_pushButton_clicked(bool checked);

private:
    QList<Singal_Chart_Item*> Signal_List;
    Ui::Chart *ui;
    Create_Signal_Item_Dialog create_Signal_Item_Dialog;
    API *API_RGA;
};

#endif // CHART_H
