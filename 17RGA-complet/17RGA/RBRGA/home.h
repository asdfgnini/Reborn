#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include "qlistwidget.h"
#include "rga_item.h"  //RGA单个设备

namespace Ui {
class home;
}


struct RGA_item_addr
{
    RGA_item *Widget_addr;
    QListWidgetItem *WidgetItem_addr;
};

class home : public QWidget
{
    Q_OBJECT

public:
    QList<RGA_item_addr> RGA_addr_List;
    explicit home(QWidget *parent = nullptr);
    ~home();

    void Set_RGA_Visible(bool Switch); //按钮穿透
    Page_Number Page_Query();
private slots:
    void on_Add_RGA_PushButton_clicked(); //添加RGA设备
    void on_Remove_RGA_PushButton_clicked(); //删除RGA设备
    void on_RGA_listWidget_currentRowChanged(int currentRow); //RGA_List切换
    void on_Hide_RGA_PushButton_clicked();  //隐藏RGA设备选择界面
    void on_Show_RGA_PushButton_clicked();  //显示RGA设备选择界面
    void on_Sweep_Mode_PushButton_clicked(bool checked);//扫描页面切换

    void on_Leak_Mode_PushButton_clicked(bool checked);

signals:
   void Switch_Page(Page_Number Number);  //模式页面切换传递信号
 private:
    Ui::home *ui;
};

#endif // HOME_H
