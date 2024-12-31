#ifndef RB_RGA_H
#define RB_RGA_H

#include <QMainWindow>
#include "api.h"
#include "set_communication_dialog.h"
#include "chart.h"
#include "amu_chart.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class RB_RGA;
}
QT_END_NAMESPACE




class RB_RGA : public QMainWindow
{
    Q_OBJECT

public:
    // Chart Tol_Pressure_Chart;
    // AMU_Chart AMP_Chart;
    // Setting_Data RGA_Setting;
    RB_RGA(QWidget *parent = nullptr);
    ~RB_RGA();

     void Display(API *addr);
    void RGA_Init();
    void Scan_Setting_Display(API *addr);
    void Control_Button_Display(API *addr);
    void Title_Display(API *addr);

private slots:

    void on_AMU_Start_spinBox_valueChanged(int arg1);
    void on_AMU_Stop_spinBox_valueChanged(int arg1);
    void on_Emission_pushButton_clicked();
     void on_EM_pushButton_clicked();
    void on_Scan_pushButton_clicked();

private:
    Ui::RB_RGA *ui;
};
#endif // RB_RGA_H
