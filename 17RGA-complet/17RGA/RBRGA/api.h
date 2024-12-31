#ifndef API_H
#define API_H

#include <QWidget>
#include "API_Communication.h"
#include "API_ScanInfo.h"
#include "API_ScanSetup.h"
#include "API_SensorFilter.h"
#include "API_Sensor_Information.h"
#include "API_Status.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <map>
#include <iostream>
namespace Ui {
class API;
}

#define API_NORMAL_TIME_MS 500


enum Signal_Type
{
    Fix_Number_Type,
    Timer_Type,
    Sys_State_Type,
    Sweep_Type,
    BaseLine_Type,
    TP_Quad_Type,
    Total_Preussure_Type,
};


enum Communication_State
{
    HTTP_NO_CONNECT,
    HTTP_CONNECTING,
    HTTP_OK_CONNECT
};

enum Info_Init_Schedule
{
    SensorInfo_CMD,
    ScanInfo_CMD,
    SensorFilter_CMD,
    SystemStatus_CMD,
    SystemStatus2_CMD,
    Set_Force_CMD,
    ScanSetup_Ppamurange_CMD,
    Communication_CMD
};

enum Scan_Set_Schedule
{
    Get_Force,
    Channel1,
    Channel2,
    Channel3,
    Channel4,
    Channel5,
    Channel6,
    Channel7,
    Channel8,
    Channel9,
    Channel10,
    Channel11,
    Channel12,
    Channel13,
    Scan_Info_Set,
    Start_Scan_Set,
    Stop_Scan_Set
};

enum Scan_Read_Schedule
{
    last_Data,
    current_Data,
};

struct Setting_Data
{
    int AMU_Start;
    int AMU_Stop;
    int Ppamu;
    int Dwell;
};

struct AMU_Data
{
    double point;
    double value;

};

struct Scan_Data
{
    int AMU_Start;
    int AMU_Stop;
    int Ppamu;
    int Dwell;
    bool Scan_Switch;
    int   Current_Scan_Number;
    float Current_TotalPressure;
    float Current_Baseline;
    float Current_TPQuad;
    int Scan_size;
    int Start_Point;
    int Cur_Data_Size;
    bool Data_Load_Heavy;
    QList<double> Scan_data_List;   //本次扫描数据点存储map
};



class API : public QWidget
{
    Q_OBJECT

public:
    bool RGA_TCPIP_Switch;
    QString Ip_addr;  //IP地址
    int Com_number;   //端口号
    QString RGA_Name; //RGA名称
    Scan_Data RGA_Scan_Data;
    Setting_Data RGA_Setting_Data;
    bool control_Switch;
    Communication_State RGA_Communication_State;
    int Data_Number;
    int Communication_Error_Time;
    bool info_Init_State;
    Info_Init_Schedule  RGA_Info_Init_Schedule;
    Info_Init_Schedule  RGA_Info_Normal_Schedule;
    Scan_Read_Schedule  RGA_Scan_Read_Schedule;
    Scan_Set_Schedule   RGA_Scan_set_Schedule;
    API_Sensor_Information RGA_Sensor_Information;   //传感器信息
    API_Communication  RGA_Communication;            //通讯设置
    API_ScanInfo     RGA_ScanInfo;                   //扫描信息
    API_ScanSetup    RGA_ScanSetup;                  //扫描设置
    API_SensorFilter RGA_SensorFilter;               //四极杆参数
    API_Status       RGA_Status;                     //RGA状态
    QNetworkAccessManager *manager;

    QTimer *Get_Timer;
    QTimer *Scan_Timer;

signals:
    void  scan_Start();
    void  scan_Stop();
    void  Send_AMU_Signal(Signal_Type Type,double AMU,int Time,double value);
    void  SensorFilter_Info(); //扫描信息读取完成
    void  Communication_State_refresh();
    void  RGA_Init_ok();
    void  TotalPressure_Flash(double TotalPressure_Value);
    void  Partial_Pressure_Flash(QVector<double> x_newXData,QVector<double> y_newXData,int Start_AMU,int Stop_AMU,double Current_AMU);
public:
    explicit API(QWidget *parent = nullptr);
    ~API();
    void GetRequest(QUrl url);
    void RGA_Sensor_Information_Read(QJsonObject jsonObject);
    void RGA_Communication_Read(QJsonObject jsonObject);
    void RGA_Status_Read(QJsonObject jsonObject);
    bool extractSingleBit(uint32_t status, int bitIndex);
    void RGA_ScanInfo_Read(QJsonObject jsonObject);
    void RGA_SensorFilter_Read(QJsonObject jsonObject);
    void RGA_ScanSetup_Read(QJsonObject jsonObject);
    void UI_Play();
    void systemStatuss_Change();
    void RGA_communication_Read(QJsonObject jsonObject);
    void RGA_Start_Connect();
    void RGA_Stop_Connect();
    void RGA_INfo_Init();
    void Info_Init_Request(QUrl url);
    void RGA_Connect_State_Change(Communication_State State);
    void RGA_INfo_Normal();
    void SetRequest(QUrl url);
    void RGA_Scan_Setting();
    void GetRequest_Scan_Set(QUrl url);
    void Scan_Set_HandleResponse();
    void GetRequest_Scan_Read(QUrl url);
    void Scan_Read_HandleResponse();
    void Data_Extract();
    void RGA_Scan_Setting_Read();
    void GetRequest_Scan_Setting_Read(QUrl url);
    void Scan_Setting_Read_HandleResponse();
    void API_Init();
    void AMU_Data_Send(int Data_Size);
public slots:
    void SET_Emission();   //开灯丝
    void SET_EM();         //开电子倍增器
    void SET_Scan(Setting_Data info);       //开始扫描
    void RGA_Scan_Read();
    void TimerSendRequest();
    void Info_Init_HandleResponse();
    void SethandleResponse();
private:
     QString URL_get_Sensor_Information="/mmsp/sensorInfo/get";


private:
    Ui::API *ui;
    void handleResponse();
};

#endif // API_H
