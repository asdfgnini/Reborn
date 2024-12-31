#ifndef SWEEP_MODE_PAGE_H
#define SWEEP_MODE_PAGE_H

#include <QWidget>
#include "api.h"
#include "amu_chart.h"
#include "chart.h"

namespace Ui {
class Sweep_Mode_Page;
}

class Sweep_Mode_Page : public QWidget
{
    Q_OBJECT
public slots:
  void UI_Refresh();   //API传数据->刷新界面

private slots:
    void on_Emission_pushButton_clicked();  //开关灯丝按钮
    void on_EM_pushButton_clicked();        //打开电子倍增器按钮
    void on_Scan_pushButton_clicked();      //开始扫描按钮
    void on_AMU_Start_spinBox_valueChanged(int arg1);  //设定扫描起始点位
    void on_AMU_Stop_spinBox_valueChanged(int arg1);   //设定扫描终止点位
    void on_Hide_Sweep_Set_PushButton_clicked();  //隐藏扫描配置
    void on_Show_Sweep_Set_PushButton_clicked();  //显示扫描配置
signals:
    void Set_Emission(); //开关灯丝信号
    void Set_EM();       //开关电子倍增器信号
    void Set_Scan(Setting_Data Info);     //开关扫描信号
public:
    explicit Sweep_Mode_Page(QWidget *parent,API *RGA_Info);
    ~Sweep_Mode_Page();
    AMU_Chart *RGA_AMU_Chart;       //扫描质荷比曲线
    Chart *RGA_Singal_Chart;        //单个质谱扫描信号叠加
    void UI_Init();                //界面初始化
    void Scan_Setting_Display();   //扫描设置刷新
    void Control_Button_Display(); //控制按钮刷新
    void Title_Display();          //数据栏刷新
    void Set_Sweep_Set_Visible(bool Switch); //设定扫描配置是否隐藏
    void Page_Disabled(); //设定扫描配置页面是否可控
private:
    Ui::Sweep_Mode_Page *ui;
    API *API_INFO;
};

#endif // SWEEP_MODE_PAGE_H
