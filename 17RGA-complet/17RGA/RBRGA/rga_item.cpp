#include "rga_item.h"
#include "ui_rga_item.h"

RGA_item::RGA_item(QWidget *parent,Page_Number Number)
    : QWidget(parent)
    , ui(new Ui::RGA_item)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_TransparentForMouseEvents,true); //扫描穿透
    RGA_Display_Init(&RGA_INFO,Number);  //扫描界面初始化
    UI_Refresh();

    connect(&Item_IP_Setting_Dialog, &Set_communication_Dialog::Ask_Set_Signal, this, &RGA_item::Connect_Setting);   //TCP/IP新协议设置
    connect(&RGA_INFO, &API::Communication_State_refresh, this, &RGA_item::UI_Refresh);   //连接状态刷新
    RGA_INFO.Communication_State_refresh();
}

RGA_item::~RGA_item()
{
    delete ui;
}

void RGA_item::RGA_Display_Init(API *addr,Page_Number Number)
{
    RGA_Display *Display_Page=new RGA_Display(this,addr,Number);  //创建界面管理页面
    RGA_Display_Page =Display_Page;


}

void RGA_item::RGA_Select(bool Switch)
{
    this->setAttribute(Qt::WA_TransparentForMouseEvents,Switch);
}

void RGA_item::UI_Refresh()
{

    Connect_Button_Display();
    IP_Display();
}

void RGA_item::IP_Display()
{
    QStringList ipParts = RGA_INFO.Ip_addr.split('.', Qt::SkipEmptyParts);
    if (ipParts.size() == 4)
    {
        ui->IP1->setValue( ipParts[0].toInt());
        ui->IP2->setValue( ipParts[1].toInt());
        ui->IP3->setValue( ipParts[2].toInt());
        ui->IP4->setValue( ipParts[3].toInt());
    }
    ui->RGA_lineEdit->setText(RGA_INFO.RGA_Name);
}

void RGA_item::Connect_Setting(void)
{
    RGA_INFO.Ip_addr=Item_IP_Setting_Dialog.ip_Addr;
    RGA_INFO.Com_number=Item_IP_Setting_Dialog.Com_number;
    ui->IP1->setValue(Item_IP_Setting_Dialog.ip1);
    ui->IP2->setValue(Item_IP_Setting_Dialog.ip2);
    ui->IP3->setValue(Item_IP_Setting_Dialog.ip3);
    ui->IP4->setValue(Item_IP_Setting_Dialog.ip4);
}


void RGA_item::on_Setting_pushButton_clicked()
{
    qDebug()<<RGA_INFO.Ip_addr;
    Item_IP_Setting_Dialog.Refresh(RGA_INFO.Ip_addr,RGA_INFO.Com_number);
    Item_IP_Setting_Dialog.exec();
}


void RGA_item::on_Connect_pushButton_clicked()
{
    switch(RGA_INFO.RGA_Communication_State)
    {
    case HTTP_NO_CONNECT:
        RGA_INFO.RGA_Start_Connect();
        break;

    case HTTP_CONNECTING:
        break;

    case HTTP_OK_CONNECT:
        RGA_INFO.RGA_Stop_Connect();
        break;
    }
}


void RGA_item::Connect_Button_Display()
{
    switch(RGA_INFO.RGA_Communication_State)
    {
        qDebug()<<RGA_INFO.RGA_Communication_State;
    case HTTP_CONNECTING:
        ui->Connect_pushButton->setIcon(QIcon(":/new/prefix1/icon/TCP_Try.png"));
        ui->Connect_pushButton->setText(" 连接中 ");
        ui->Setting_pushButton->setVisible(false);
        ui->force_pushButton->setVisible(false);
        break;
    case HTTP_NO_CONNECT:
        ui->Connect_pushButton->setIcon(QIcon(":/new/prefix1/icon/TCP_NoConnect.png"));
        ui->Connect_pushButton->setText(" 未连接 ");
        ui->Setting_pushButton->setVisible(true);
        ui->force_pushButton->setVisible(false);
        break;

    case HTTP_OK_CONNECT:
        ui->Connect_pushButton->setIcon(QIcon(":/new/prefix1/icon/TCP_Connect.png"));
        ui->Connect_pushButton->setText(" 已连接 ");
        ui->Setting_pushButton->setVisible(false);
        ui->force_pushButton->setVisible(true);
        if(RGA_INFO.RGA_Communication.amInControl==true)
        {
            ui->force_pushButton->setIcon(QIcon(":/new/prefix1/icon/Control_RGA.png"));
        }
        else
        {
            ui->force_pushButton->setIcon(QIcon(":/new/prefix1/icon/NoControl_RGA.png"));
        }
        break;
    }
}

