#ifndef RGA_ITEM_H
#define RGA_ITEM_H

#include <QWidget>
#include "set_communication_dialog.h" //ip地址设置页面
#include "api.h"
#include "rb_rga.h"
#include "rga_display.h"   //模式界面管理
namespace Ui {
class RGA_item;
}

class RGA_item : public QWidget
{
    Q_OBJECT

public:
    explicit RGA_item(QWidget *parent,Page_Number Number);
    ~RGA_item();

    void RGA_Select(bool Switch);
    void IP_Display();
    void Connect_Button_Display();

    API RGA_INFO;      //RGA api端口
    Set_communication_Dialog Item_IP_Setting_Dialog;  //通讯ip地址与端口设置页面
    RGA_Display *RGA_Display_Page;
    void RGA_Display_Init(API *addr,Page_Number Number);
public slots:
    void Connect_Setting();
    void UI_Refresh();
private slots:


    void on_Setting_pushButton_clicked();

    void on_Connect_pushButton_clicked();

private:

    Ui::RGA_item *ui;  //item界面

};

#endif // RGA_ITEM_H
