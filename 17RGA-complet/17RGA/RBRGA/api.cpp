#include "api.h"
#include "qtimer.h"
#include "ui_api.h"

#include <API.h>

API::API(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::API)

{
    Ip_addr="192.168.1.179";
    RGA_TCPIP_Switch=false;
    Com_number=80;
    RGA_Name="MyRGA";

    manager = new QNetworkAccessManager(this);
    Get_Timer= new QTimer(this);
    Get_Timer->setSingleShot(false);
    Get_Timer->setTimerType(Qt::CoarseTimer);
    Get_Timer->stop();
    connect(Get_Timer,SIGNAL(timeout()),this,SLOT(TimerSendRequest()));

    Scan_Timer= new QTimer(this);
    Scan_Timer->setSingleShot(false);
    Scan_Timer->setTimerType(Qt::CoarseTimer);
    Scan_Timer->stop();
    connect(Scan_Timer,SIGNAL(timeout()),this,SLOT(RGA_Scan_Read()));
    API_Init();
    ui->setupUi(this);
}

void  API::API_Init()
{
    RGA_Communication_State=HTTP_NO_CONNECT;
    RGA_Scan_Data.Scan_Switch=false;
    RGA_Info_Init_Schedule=SensorInfo_CMD;
    RGA_Info_Normal_Schedule=ScanInfo_CMD;
    RGA_Scan_set_Schedule=Get_Force;
    info_Init_State=false;
    Communication_Error_Time=0;
    Data_Number=0;
    Get_Timer->stop();
    Scan_Timer->stop();
}

void  API::RGA_Scan_Setting_Read()
{
   GetRequest_Scan_Setting_Read(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanSetup/channels/4/get"));
}

void API::GetRequest_Scan_Setting_Read( QUrl url)
{
    QNetworkRequest request(url);
    request.setTransferTimeout(10000);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &API::Scan_Setting_Read_HandleResponse);
}

void API::Scan_Setting_Read_HandleResponse()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply && reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if (!jsonDoc.isNull())
        {
            QJsonObject jsonObject = jsonDoc.object();
            if((jsonObject["origin"].toString()=="/mmsp/scanSetup/channels/4"))
            {
                QJsonArray dataObject = jsonObject["data"].toArray();
                for (int i=0;i<dataObject.size();i++)
                {
                    QJsonObject dataItem = dataObject[i].toObject();
                    if(dataItem["channelType"].toString()=="Sweep")
                    {
                        RGA_Scan_Data.AMU_Start=dataItem["startMass"].toInt();
                        RGA_Scan_Data.AMU_Stop=dataItem["stopMass"].toInt();
                        RGA_Scan_Data.Ppamu=dataItem["ppamu"].toInt();
                        RGA_Scan_Data.Dwell=dataItem["dwell"].toInt();
                        Scan_Timer->start(100);
                        RGA_Scan_Data.Scan_Switch=true;
                    }
               }
            }
        }
    }
    else
    {

        // qDebug() << "Error:" +reply->errorString();
        Communication_Error_Time++;
        if(Communication_Error_Time>=3)   //初始化失败
        {
            RGA_Connect_State_Change(HTTP_NO_CONNECT);
        }
        else RGA_Scan_Setting_Read();
    }
    reply->deleteLater();
}


void  API::RGA_Scan_Read() //读取扫描点位
{
    Scan_Timer->stop();
    if(RGA_TCPIP_Switch)
    {
        // qDebug()<<"扫描结果读取发送起始点位"<<Data_Number;
        GetRequest_Scan_Read(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/measurement/data/get?@start="+QString::number(Data_Number)));   //读上一次测量数据
    }
}
void API::GetRequest_Scan_Read( QUrl url) //扫描指令发送
{
    QNetworkRequest request(url);
    request.setTransferTimeout(10000);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &API::Scan_Read_HandleResponse);
}

void API::Scan_Read_HandleResponse() //扫描结果读取处理
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply && reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if (!jsonDoc.isNull())
        {
            QJsonObject jsonObject = jsonDoc.object();
            if((jsonObject["origin"].toString()=="/mmsp/measurement/data"))
            {
             QJsonObject dataObject = jsonObject["data"].toObject();
             QJsonArray valuesArray = dataObject["values"].toArray(); //获取数据点
              int start_Point=dataObject["start"].toInt();    //获取本次数据起始点位
              int scansize=dataObject["scansize"].toInt();    //获取本次数据一次扫描的总点位数
              int Value_Size=valuesArray.size();              //获取本次数据点数量
              if(Data_Number==0)
              {
                  // qDebug()<<"数据删除";
                  RGA_Scan_Data.Scan_data_List.clear();
                  RGA_Scan_Data.Start_Point=start_Point; //更新数据起点
                  RGA_Scan_Data.Scan_size=scansize;
              }
              if(Data_Number!=start_Point) RGA_Scan_Data.Data_Load_Heavy=true;
              else RGA_Scan_Data.Data_Load_Heavy=false;
              for(int i=0;i<Value_Size;i++)
              {
                  RGA_Scan_Data.Scan_data_List.append(valuesArray[i].toDouble());    
              }
              RGA_Scan_Data.Cur_Data_Size=Value_Size;
              Data_Extract();
              Data_Number=start_Point+Value_Size; //刷新下一次读取命令起点
              if(RGA_Status.systemStatuss.versiscanActive==1)
              {
                  Scan_Timer->start(100);
                  RGA_Scan_Data.Scan_Switch=true;
              }
              else
              {
                  RGA_Scan_Data.Scan_Switch=false;
              }
            }
        }
    }
    else
    {
        // qDebug() << "Error:" +reply->errorString();
        Communication_Error_Time++;
        if(Communication_Error_Time>=3)   //初始化失败
        {
            RGA_Connect_State_Change(HTTP_NO_CONNECT);
        }
        else RGA_Scan_Read();
    }
    reply->deleteLater();
}


void API::Data_Extract() //扫描数据处理
{
    int Data_Size=RGA_Scan_Data.Cur_Data_Size;
    int Max_Point=RGA_Scan_Data.Start_Point+RGA_Scan_Data.Scan_data_List.size();  //本次扫描当前最大点位
    int Sweep_Size=(RGA_Scan_Data.AMU_Stop-RGA_Scan_Data.AMU_Start)*RGA_Scan_Data.Ppamu+1; //频率扫描切换
    int Pre_Size=3;
    int Last_Size=9;
    int RGA_ScanSize=Pre_Size+Sweep_Size+Last_Size;
    int TotalPressure_Point=Pre_Size+Sweep_Size;
    int Baseline_Point=Pre_Size+Sweep_Size+1;
    int TPQuad_Point=Pre_Size+Sweep_Size+2;
    double Cur_Max_Point=0;
    RGA_Scan_Data.Current_Scan_Number=Max_Point/RGA_ScanSize; //当前扫描次数
    QList<AMU_Data> Data;
    Data.clear();
        for(int i=0;i<Sweep_Size;i++)
        {
            AMU_Data Data_Point;
            Data_Point.point=i*(1.0f/RGA_Scan_Data.Ppamu);
            int i_Point=RGA_ScanSize*RGA_Scan_Data.Current_Scan_Number+Pre_Size+i; //计算当前质荷比最新数据点位置
            int i_Last_Point=i_Point-RGA_ScanSize; //计算当前质荷比上一次数据点位置
            if((i_Point<=(Max_Point-1))&&((i_Point-RGA_Scan_Data.Start_Point)>=0))  //本次最新扫描部分结果包含该点数据
            {
                if(RGA_Status.systemStatuss.electronMultiplier) Data_Point.value=-RGA_Scan_Data.Scan_data_List[i_Point-RGA_Scan_Data.Start_Point];
                else Data_Point.value=RGA_Scan_Data.Scan_data_List[i_Point-RGA_Scan_Data.Start_Point];
                 Data.append(Data_Point);
                 if(i>=Cur_Max_Point) Cur_Max_Point=i; //计算本次扫描包含最大质荷比的位置
            }
            else if((i_Last_Point<=(Max_Point-1))&&((i_Last_Point-RGA_Scan_Data.Start_Point)>=0)) //本次最新扫描部分结果不包含该点数据
            {
                if(RGA_Status.systemStatuss.electronMultiplier) Data_Point.value=-RGA_Scan_Data.Scan_data_List[i_Last_Point-RGA_Scan_Data.Start_Point];
                else Data_Point.value=RGA_Scan_Data.Scan_data_List[i_Last_Point-RGA_Scan_Data.Start_Point];

                Data.append(Data_Point);
            }
        }
            QVector<double> x_newXData;
            QVector<double>  y_newXData;
            for(int i=0;i<Data.size();i++)
            {
                x_newXData<<Data[i].point;
                y_newXData<<Data[i].value;
            }
          emit Partial_Pressure_Flash(x_newXData,y_newXData,RGA_Scan_Data.AMU_Start,RGA_Scan_Data.AMU_Stop,Cur_Max_Point/RGA_Scan_Data.Ppamu);
 if(!RGA_Scan_Data.Data_Load_Heavy)
  {
    for(int i=0;i<Data_Size;i++)
    {

        int Point=RGA_Scan_Data.Scan_data_List.size()-Data_Size+RGA_Scan_Data.Start_Point+i;
        double value=RGA_Scan_Data.Scan_data_List[Point-RGA_Scan_Data.Start_Point];
        if(value<=0 ) value=-value;
        int  Cur_Sweep_Number=Point/RGA_ScanSize;
        int  Cur_Sweep_Time_Point=Cur_Sweep_Number*RGA_ScanSize+1-RGA_Scan_Data.Start_Point;
        int  Cur_Sweep_Point=Point%RGA_ScanSize;
        if((Cur_Sweep_Time_Point<=(RGA_Scan_Data.Scan_data_List.size()-1))&&(Cur_Sweep_Time_Point>=0))
        {
         int time=RGA_Scan_Data.Scan_data_List[Cur_Sweep_Time_Point];
            if(Cur_Sweep_Point<Pre_Size)
           {
             // qDebug()<<"dian"<<Cur_Sweep_Point<<"时间戳"<<time<<"值"<<value;
             emit Send_AMU_Signal(Sys_State_Type,0,time,value);
            }
            else if((Cur_Sweep_Point>=Pre_Size+Sweep_Size))
            {
                if(Cur_Sweep_Point==TotalPressure_Point)
                {
                    emit Send_AMU_Signal(Total_Preussure_Type,0,time,value);
                    // qDebug()<<"时间戳"<<time<<"总压"<<value;
                }
                else if(Cur_Sweep_Point==Baseline_Point)
                {
                    emit Send_AMU_Signal(BaseLine_Type,0,time,value);
                      // qDebug()<<"时间戳"<<time<<"噪声"<<value;
                }
                else if(Cur_Sweep_Point==TPQuad_Point)
                {
                    emit Send_AMU_Signal(TP_Quad_Type,0,time,value);
                      // qDebug()<<"时间戳"<<time<<"四极杆压力"<<value;
                }
            }
            else
            {

                double AMU=(Cur_Sweep_Point-Pre_Size)*(1.0f/RGA_Scan_Data.Ppamu);
                // qDebug()<<"时间戳"<<time<<"质量数"<<AMU<<"值"<<value;
                emit Send_AMU_Signal(Sweep_Type,AMU,time,value);
            }
        }
        else
        {
            // qDebug()<<"数据点"<<Point<<"错误Cur_Sweep_Point"<<RGA_Scan_Data.AMU_Stop;
        }
    }
 }
    // if((Current_Data_Point>=TotalPressure_Point))
    // {

    //     RGA_Scan_Data.Current_TotalPressure=RGA_Scan_Data.Scan_data_List[RGA_Scan_Data.Current_Scan_Number*RGA_ScanSize-1+TotalPressure_Point-RGA_Scan_Data.Start_Point];
    //    emit TotalPressure_Flash(RGA_Scan_Data.Current_TotalPressure);
    // }
    // else
    // {
    //     if(RGA_Scan_Data.Current_Scan_Number!=0)  RGA_Scan_Data.Current_TotalPressure=RGA_Scan_Data.Scan_data_List[(RGA_Scan_Data.Current_Scan_Number-1)*RGA_ScanSize-1+TotalPressure_Point-RGA_Scan_Data.Start_Point];
    //     else RGA_Scan_Data.Current_TotalPressure=0;
    // }
    // if((Current_Data_Point>=Baseline_Point)) RGA_Scan_Data.Current_Baseline=RGA_Scan_Data.Scan_data_List[RGA_Scan_Data.Current_Scan_Number*RGA_ScanSize-1+Baseline_Point-RGA_Scan_Data.Start_Point];
    // else
    // {
    //     if(RGA_Scan_Data.Current_Scan_Number!=0)  RGA_Scan_Data.Current_Baseline=RGA_Scan_Data.Scan_data_List[(RGA_Scan_Data.Current_Scan_Number-1)*RGA_ScanSize-1+Baseline_Point-RGA_Scan_Data.Start_Point];
    //     else RGA_Scan_Data.Current_Baseline=0;
    // }
    // if((Current_Data_Point>=TPQuad_Point)) RGA_Scan_Data.Current_TPQuad=RGA_Scan_Data.Scan_data_List[RGA_Scan_Data.Current_Scan_Number*RGA_ScanSize-1+TPQuad_Point-RGA_Scan_Data.Start_Point];
    // else
    // {
    //     if(RGA_Scan_Data.Current_Scan_Number!=0)  RGA_Scan_Data.Current_TPQuad=RGA_Scan_Data.Scan_data_List[(RGA_Scan_Data.Current_Scan_Number-1)*RGA_ScanSize-1+TPQuad_Point-RGA_Scan_Data.Start_Point];
    //     else RGA_Scan_Data.Current_TPQuad=0;
    // }
    // emit Communication_State_refresh();
    // qDebug()<<"单次数据总尺寸"<<RGA_Scan_Data.Scan_size<<"当前扫描次数"<<RGA_Scan_Data.Current_Scan_Number<<"新扫描点位"<<Current_Data_Point;
    // qDebug()<<"数据总数:"<<RGA_Scan_Data.Scan_data_List.size()<<"起始点位:"<<RGA_Scan_Data.Start_Point;
    // qDebug()<<"总压信号:"<<RGA_Scan_Data.Current_TotalPressure;
    // qDebug()<<"底噪信号:"<<RGA_Scan_Data.Current_Baseline;
    // qDebug()<<"四极杆压力信号:"<<RGA_Scan_Data.Current_TPQuad;

}

API::~API()
{
    delete ui;
}

void::API::SET_Scan(Setting_Data info)
{
    RGA_Setting_Data=info;
    RGA_Scan_Setting();
}

void  API::RGA_Scan_Setting() //扫描参数设置
{
    if(RGA_TCPIP_Switch)
    {
 // qDebug()<<"扫描参数设定发送编号"<<RGA_Scan_set_Schedule;
  switch(RGA_Scan_set_Schedule)
  {
    case  Get_Force:
        GetRequest_Scan_Set(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/communication/control/set?force"));   //固定编码
        break;
     case  Channel1:
      GetRequest_Scan_Set(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanSetup/channels/1/set?channelMode=FixedNumber&dwell=1&enabled=True"));   //固定编码
         break;
     case  Channel2:
       GetRequest_Scan_Set(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanSetup/channels/2/set?channelMode=Timestamp&dwell=1&enabled=True"));
         break;
     case  Channel3:
         GetRequest_Scan_Set(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanSetup/channels/3/set?channelMode=SystemStatus&dwell=1&enabled=True"));
         break;
     case  Channel4:
         GetRequest_Scan_Set(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanSetup/channels/4/set?channelMode=Sweep&startMass="+QString::number(RGA_Setting_Data.AMU_Start)+"&stopMass="+QString::number(RGA_Setting_Data.AMU_Stop)+"&dwell="+QString::number(RGA_Setting_Data.Dwell)+"&ppamu="+QString::number(RGA_Setting_Data.Ppamu)+"&enabled=True"));
         // qDebug()<<"扫描时间"<<RGA_Setting_Data.Dwell;
         break;
     case  Channel5:
             GetRequest_Scan_Set(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanSetup/channels/5/set?channelMode=TotalPressure&dwell=1&enabled=True"));
         break;
     case  Channel6:
         GetRequest_Scan_Set(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanSetup/channels/6/set?channelMode=Baseline&dwell="+QString::number(RGA_Setting_Data.Dwell)+"&ppamu=1&enabled=True"));
         break;
     case  Channel7:
             GetRequest_Scan_Set(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanSetup/channels/7/set?channelMode=TPQuad&dwell="+QString::number(RGA_Setting_Data.Dwell)+"&ppamu=1&enabled=True"));
         break;
     case  Channel8:
             GetRequest_Scan_Set(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanSetup/channels/8/set?channelMode=TotalPressureDetectorCurrent&dwell=1&enabled=True"));
         break;
     case  Channel9:
             GetRequest_Scan_Set(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanSetup/channels/9/set?channelMode=EmissionCurrent&dwell=1&enabled=True"));
         break;
     case  Channel10:
             GetRequest_Scan_Set(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanSetup/channels/10/set?channelMode=AnodePotential&dwell=1&enabled=True"));
         break;
     case  Channel11:
             GetRequest_Scan_Set(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanSetup/channels/11/set?channelMode=ElectronEnergy&dwell=1&enabled=True"));
         break;
     case  Channel12:
             GetRequest_Scan_Set(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanSetup/channels/12/set?channelMode=FocusPotential&dwell=1&enabled=True"));
         break;
     case  Channel13:
             GetRequest_Scan_Set(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanSetup/channels/13/set?channelMode=FilamentCurrent&dwell=1&enabled=True"));
         break;
     case  Scan_Info_Set:
             GetRequest_Scan_Set(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanSetup/set?startChannel=1&stopChannel=13&scanCount=-1&scanInterval=0"));
         break;
     case  Start_Scan_Set:
            GetRequest_Scan_Set(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanSetup/set?scanStart=1"));
         break;
     case  Stop_Scan_Set:
            GetRequest_Scan_Set(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanSetup/set?scanStop=Immediately"));
         break;
     default :
         break;
 }
    }
}
void  API::GetRequest_Scan_Set( QUrl url) //扫描参数设置指令发送
 {
     QNetworkRequest request(url);
     request.setTransferTimeout(10000);
    QNetworkReply *reply = manager->get(request);
     connect(reply, &QNetworkReply::finished, this, &API::Scan_Set_HandleResponse);
 }
void  API::Scan_Set_HandleResponse()   //扫描通道设置指令回复
 {
     QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
     if (reply && reply->error() == QNetworkReply::NoError)
     {
         QByteArray responseData = reply->readAll();
         QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
         if (!jsonDoc.isNull())
         {
             QJsonObject jsonObject = jsonDoc.object();
             switch(RGA_Scan_set_Schedule)
             {
             case Get_Force:
                 if(jsonObject["name"].toString()=="wasSet")
                 {
                     if(RGA_Status.systemStatuss.versiscanActive==0) RGA_Scan_set_Schedule=Channel1;
                     else RGA_Scan_set_Schedule=Stop_Scan_Set;
                   RGA_Scan_Setting();
                 }
                 break;
             case Channel1:
                 if(jsonObject["name"].toString()=="wasSet")
                 {
                 RGA_Scan_set_Schedule=Channel2;
                 RGA_Scan_Setting();
                 }
                 break;
             case Channel2:
                 if(jsonObject["name"].toString()=="wasSet")
                 {

                     RGA_Scan_set_Schedule=Channel3;
                 RGA_Scan_Setting();
                 }
                 break;
             case Channel3:
                 if(jsonObject["name"].toString()=="wasSet")
                 {

                     RGA_Scan_set_Schedule=Channel4;
                   RGA_Scan_Setting();
                 }
                 break;
             case Channel4:
                 if(jsonObject["name"].toString()=="wasSet")
                 {
                     RGA_Scan_set_Schedule=Channel5;
                 RGA_Scan_Setting();
                 }
                 break;
             case Channel5:
                 if(jsonObject["name"].toString()=="wasSet")
                 {

                     RGA_Scan_set_Schedule=Channel6;
                 RGA_Scan_Setting();
                 }
                 break;
             case Channel6:
                 if(jsonObject["name"].toString()=="wasSet")
                 {

                     RGA_Scan_set_Schedule=Channel7;
                 RGA_Scan_Setting();
                 }
                 break;
             case Channel7:
                 if(jsonObject["name"].toString()=="wasSet")
                 {
                  RGA_Scan_set_Schedule=Channel8;
                  RGA_Scan_Setting();
                 }
                 break;
             case Channel8:
                 if(jsonObject["name"].toString()=="wasSet")
                 {

                 RGA_Scan_set_Schedule=Channel9;
                  RGA_Scan_Setting();
                 }
                 break;
             case Channel9:
                 if(jsonObject["name"].toString()=="wasSet")
                 {

                     RGA_Scan_set_Schedule=Channel10;
                 RGA_Scan_Setting();
                 }
                 break;
             case Channel10:
                 if(jsonObject["name"].toString()=="wasSet")
                 {

                     RGA_Scan_set_Schedule=Channel11;
                 RGA_Scan_Setting();
                 }
                 break;

             case Channel11:
                 if(jsonObject["name"].toString()=="wasSet")
                 {

                     RGA_Scan_set_Schedule=Channel12;
                 RGA_Scan_Setting();
                 }
                 break;

             case Channel12:
                 if(jsonObject["name"].toString()=="wasSet")
                 {
                     RGA_Scan_set_Schedule=Channel13;
                 RGA_Scan_Setting();
                 }
                 break;

             case Channel13:
                 if(jsonObject["name"].toString()=="wasSet")
                 {

                   RGA_Scan_set_Schedule=Scan_Info_Set;
                   RGA_Scan_Setting();
                  }
                 break;

             case Scan_Info_Set:
                 if(jsonObject["name"].toString()=="wasSet")
                 {

                  RGA_Scan_set_Schedule=Start_Scan_Set;
                  RGA_Scan_Setting();
                 }
                 break;

             case Start_Scan_Set:
                 if(jsonObject["name"].toString()=="wasSet")    RGA_Scan_set_Schedule=Get_Force;
                 break;

             case Stop_Scan_Set:
                 if(jsonObject["name"].toString()=="wasSet")    RGA_Scan_set_Schedule=Get_Force;
                 break;
             default:
                 break;
             }
         }
     }
     else
     {
         // qDebug() << "Error:" +reply->errorString();
         Communication_Error_Time++;
         if(Communication_Error_Time>=3)   //初始化失败
         {
             RGA_Connect_State_Change(HTTP_NO_CONNECT);
         }
         else RGA_Scan_Setting();
     }
     reply->deleteLater();
 }
void  API::RGA_INfo_Init()    //RGA基本信息读取(初始化)
{
    if((info_Init_State==false)&&(RGA_TCPIP_Switch))
    {
          // qDebug()<<"基本信息读取指令发送编号"<<RGA_Info_Init_Schedule;
        switch(RGA_Info_Init_Schedule)
        {
          case SensorInfo_CMD:
            Info_Init_Request(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/sensorInfo/get"));   //四极杆信息读取
            break;
          case ScanInfo_CMD:
              Info_Init_Request(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanInfo/get"));   //扫描信息读取
              break;
          case SensorFilter_CMD:
              Info_Init_Request(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/sensorFilter/get"));   //四极杆信息读取
              break;
          case SystemStatus_CMD:
              Info_Init_Request(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/status/systemStatus/get"));   //系统信息读取
              break;
          case SystemStatus2_CMD:
              Info_Init_Request(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/status/systemStatus2/get"));   //系统信息2读取
              break;
          case ScanSetup_Ppamurange_CMD:
              Info_Init_Request(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanSetup/ppamurange/get"));   //扫描信息读取
              break;
          case Communication_CMD:
              Info_Init_Request(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/communication/get"));   //通讯信息
              break;
        }
    }
    else
    {
        TimerSendRequest();
    }
}
void  API::Info_Init_Request( QUrl url) //RGA基本信息指令发送
{

    QNetworkRequest request(url);
    request.setTransferTimeout(1000);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &API::Info_Init_HandleResponse);
}
void  API::Info_Init_HandleResponse()  //RGA信息答复处理
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply && reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if (!jsonDoc.isNull())
        {
            QJsonObject jsonObject = jsonDoc.object();
            switch(RGA_Info_Init_Schedule)
            {
              case SensorInfo_CMD:
                RGA_Sensor_Information_Read(jsonObject);  //传感器信息
                RGA_Info_Init_Schedule=ScanInfo_CMD;
                RGA_INfo_Init();
                break;
              case ScanInfo_CMD:
                  RGA_ScanInfo_Read(jsonObject);  //传感器信息
                  RGA_Info_Init_Schedule=SensorFilter_CMD;
                  RGA_INfo_Init();
                  break;
              case SensorFilter_CMD:
                  RGA_SensorFilter_Read(jsonObject);  //传感器信息
                  RGA_Info_Init_Schedule=SystemStatus_CMD;
                  RGA_INfo_Init();
                  break;
              case SystemStatus_CMD:
                  RGA_Status_Read(jsonObject);  //传感器信息
                  RGA_Info_Init_Schedule=SystemStatus2_CMD;
                  RGA_INfo_Init();
                  break;
              case SystemStatus2_CMD:
                  RGA_Status_Read(jsonObject);  //传感器信息
                  RGA_Info_Init_Schedule=ScanSetup_Ppamurange_CMD;
                  RGA_INfo_Init();
                  break;
              case ScanSetup_Ppamurange_CMD:
                  RGA_ScanSetup_Read(jsonObject);  //传感器信息
                  RGA_Info_Init_Schedule=Communication_CMD;
                  RGA_INfo_Init();
                  break;
              case Communication_CMD:
                  RGA_communication_Read(jsonObject);          //扫描信息
                  RGA_Connect_State_Change(HTTP_OK_CONNECT);
                  RGA_INfo_Init();
                  emit RGA_Init_ok(); //基本信息读取完成
                  break;
            }
        }
    }
    else
    {
        // qDebug() << "Error:" +reply->errorString();
        Communication_Error_Time++;
        if(Communication_Error_Time>=3)   //初始化失败
        {
            RGA_Connect_State_Change(HTTP_NO_CONNECT);
        }
        else RGA_INfo_Init();
    }
    reply->deleteLater();
}
void  API::RGA_Connect_State_Change(Communication_State State)  //TCPIP连接状态发生改变
{
    if(State==HTTP_NO_CONNECT)
    {
        RGA_Communication_State=State;
        emit Communication_State_refresh();
        RGA_Info_Init_Schedule=SensorInfo_CMD;
        Communication_Error_Time=0;
        info_Init_State=false;
    }
    else if(State==HTTP_CONNECTING)
    {
        RGA_Communication_State=State;
        emit Communication_State_refresh();
    }
    else if(State==HTTP_OK_CONNECT)
    {
        RGA_Communication_State=HTTP_OK_CONNECT;
        emit Communication_State_refresh();
        RGA_Info_Init_Schedule=SensorInfo_CMD;
        Communication_Error_Time=0;
        info_Init_State=true;
    }
}
void  API::RGA_Start_Connect() //开始连接TCPIP
{

    API_Init();
    RGA_TCPIP_Switch=true;
    RGA_INfo_Init();
    RGA_Connect_State_Change(HTTP_CONNECTING);

}
void  API::RGA_Stop_Connect()  //断开连接TCPIP
{
    Get_Timer->stop();
    RGA_TCPIP_Switch=false;
    RGA_Connect_State_Change(HTTP_NO_CONNECT);

}
void API::TimerSendRequest()  //轮询定时器函数
{
    // qDebug()<<"轮询读取"+QString::number(API_NORMAL_TIME_MS)+"ms";
    Get_Timer->stop();
    if(RGA_TCPIP_Switch) RGA_INfo_Normal();
}
void  API::RGA_INfo_Normal()   //RGA参数轮询读取
{
    if(info_Init_State==true)
    {
        // qDebug()<<"轮询指令发送编号"<<RGA_Info_Normal_Schedule;
        switch(RGA_Info_Normal_Schedule)
        {
        case ScanInfo_CMD:
            // qDebug()<<"send:http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanInfo/get";
            GetRequest(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/scanInfo/get"));   //扫描信息读取
            break;
        case SystemStatus_CMD:
            // qDebug()<<"send:http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/status/systemStatus/get";
            GetRequest(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/status/systemStatus/get"));   //系统信息读取
            break;
        case SystemStatus2_CMD:
            // qDebug()<<"send:http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/status/systemStatus2/get";
            GetRequest(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/status/systemStatus2/get"));   //系统信息2读取
            break;
        case Communication_CMD:
            // qDebug()<<"send:http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/communication/controlInfo/get";
            GetRequest(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/communication/controlInfo/get"));   //通讯信息
            break;
        case Set_Force_CMD:
            // qDebug()<<"send:http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/communication/control/set?force";
            GetRequest(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/communication/control/set?force"));   
            break;
        default :
            break;
        }
    }
}
void  API::GetRequest( QUrl url)  //RGA轮询指令发送
{

    QNetworkRequest request(url);
    request.setTransferTimeout(10000);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &API::handleResponse);
}
void  API::handleResponse()  //RGA轮询答复处理
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply && reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if (!jsonDoc.isNull())
        {
            QJsonObject jsonObject = jsonDoc.object();
            switch(RGA_Info_Normal_Schedule)
            {
            case ScanInfo_CMD:
                RGA_ScanInfo_Read(jsonObject);  //传感器信息
                RGA_Info_Normal_Schedule=SystemStatus_CMD;
                RGA_INfo_Normal();
                break;
            case SystemStatus_CMD:
                RGA_Status_Read(jsonObject);  //传感器信息
                RGA_Info_Normal_Schedule=SystemStatus2_CMD;
                RGA_INfo_Normal();
                break;
            case SystemStatus2_CMD:
                RGA_Status_Read(jsonObject);  //传感器信息
                RGA_Info_Normal_Schedule=Communication_CMD;
                RGA_INfo_Normal();
                break;
            case Communication_CMD:
                RGA_Info_Normal_Schedule=ScanInfo_CMD;
                RGA_communication_Read(jsonObject);          //扫描信息
                emit Communication_State_refresh();
                if(RGA_Communication.amInControl==false)
                {
                    // RGA_Info_Normal_Schedule=Set_Force_CMD;
                    // RGA_INfo_Normal();
                    Get_Timer->start(API_NORMAL_TIME_MS);
                }
                else Get_Timer->start(API_NORMAL_TIME_MS);
                break;
            case Set_Force_CMD:
                // qDebug()<<jsonObject;
                RGA_Info_Normal_Schedule=ScanInfo_CMD;
                Get_Timer->start(API_NORMAL_TIME_MS);
                break;
            default:
                RGA_Info_Normal_Schedule=ScanInfo_CMD;
                Get_Timer->start(API_NORMAL_TIME_MS);
                break;
            }
            UI_Play();
        }
    }
    else
    {
        // qDebug() << "Error:" +reply->errorString();
        Communication_Error_Time++;
        if(Communication_Error_Time>=3)   //初始化失败
        {
            RGA_Connect_State_Change(HTTP_NO_CONNECT);
        }
        else RGA_INfo_Normal();
    }
    reply->deleteLater();
}
void  API::SET_Emission()  //RGA灯丝启停
{
    if(RGA_Status.systemStatuss.requestPendingEmission==false)
    {
        if(RGA_Status.systemStatuss.regulatedCurrentModeEmission==true)
        {
            // qDebug()<<"set setEmission Off";
            SetRequest(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/communication/control/set?force"));   //固定编码
            SetRequest(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/generalControl/set?setEmission=Off"));
        }
        else
        {
            // qDebug()<<"set setEmission ON";
            SetRequest(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/communication/control/set?force"));   //固定编码
            SetRequest(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/generalControl/set?setEmission=On"));
        }
    }
}
void  API::SET_EM()  //RGA电子倍增器启停
{
    if(RGA_Status.systemStatuss.requestPendingElectronMultiplier==false)
    {
        if(RGA_Status.systemStatuss.electronMultiplier==true)
        {
            // qDebug()<<"set EM Off";
            SetRequest(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/communication/control/set?force"));   //固定编码
            SetRequest(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/generalControl/set?setEM=Off"));
        }
        else
        {
            // qDebug()<<"set EM ON";
            SetRequest(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/communication/control/set?force"));   //固定编码
            SetRequest(QUrl("http://"+Ip_addr+":"+QString::number(Com_number)+"/mmsp/generalControl/set?setEM=On"));
        }
    }
}
void  API::SethandleResponse()  //RGA常规单次指令发送
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply && reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if (!jsonDoc.isNull())
        {
            QJsonObject jsonObject = jsonDoc.object();
            // qDebug()<<jsonObject;

        }
}
}
void  API::SetRequest( QUrl url) //RGA常规指令答复
{

    QNetworkRequest request(url);
    request.setTransferTimeout(1000);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &API::SethandleResponse);
}
void API::UI_Play()
{
    int rowIndex=1;
    QColor randomColor(rand() % 256, rand() % 256, rand() % 256);
    randomColor.setAlpha(128);
    QBrush backgroundBrush(randomColor);
     ui->tableWidget->item(34,1)->setText(QString::number(RGA_Status.systemStatus));
    for(int i=0;i<32;i++)
    {
         if(ui->tableWidget->item(i,1)->text()!=QString::number(extractSingleBit(RGA_Status.systemStatus,i)))
        {
            ui->tableWidget->item(i,1)->setBackground(backgroundBrush);
         }
         // else
         // {
         //     QBrush backgroundBrush(QColor("white"));
         //     ui->tableWidget->item(i,1)->setBackground(backgroundBrush);
         // }
        ui->tableWidget->item(i,1)->setText(QString::number(extractSingleBit(RGA_Status.systemStatus,i)));
    }
}

void API::RGA_Sensor_Information_Read(QJsonObject jsonObject)
{
    if((jsonObject["origin"].toString()=="/mmsp/sensorInfo")&&(jsonObject["name"].toString()=="got"))
    {
        QJsonObject dataObject=jsonObject["data"].toObject();
        if (dataObject.contains("name"))             RGA_Sensor_Information.name = dataObject["name"].toString();
        if (dataObject.contains("description"))      RGA_Sensor_Information.description = dataObject["description"].toString();
        if (dataObject.contains("serialNumber"))     RGA_Sensor_Information.serialNumber = dataObject["serialNumber"].toInt();
        if (dataObject.contains("ionSourceType"))    RGA_Sensor_Information.ionSourceType = static_cast<IONSOURCE_TYPE>(dataObject["ionSourceType"].toInt());
        if (dataObject.contains("massFilterType"))   RGA_Sensor_Information.massFilterType =static_cast<FILTER_TYPE>(dataObject["massFilterType"].toInt());
        if (dataObject.contains("detectorType"))     RGA_Sensor_Information.detectorType =dataObject["detectorType"].toString();
        if(dataObject.contains("filaments"))
        {
           QJsonArray filaments = dataObject["filaments"].toArray();
           for (int i=0;i<filaments.size();i++)
           {
               QJsonObject filamentObj = filaments[i].toObject();
               RGA_Sensor_Information.filament[i].Id= filamentObj["@id"].toInt();
               RGA_Sensor_Information.filament[i].Type = static_cast<FILAMENT_TYPE>(filamentObj["filamentType"].toInt());
           }
        }
    }
}
void API::RGA_SensorFilter_Read(QJsonObject jsonObject)
{
    if((jsonObject["origin"].toString()=="/mmsp/sensorFilter")&&(jsonObject["name"].toString()=="got"))
    {
        QJsonObject dataObject=jsonObject["data"].toObject();
        if (dataObject.contains("massMax"))     RGA_SensorFilter.massMax = dataObject["massMax"].toInt();
        if (dataObject.contains("massMin"))     RGA_SensorFilter.massMin = dataObject["massMin"].toInt();
        if (dataObject.contains("minDwell"))     RGA_SensorFilter.minDwell = dataObject["minDwell"].toInt();
        if (dataObject.contains("maxDwell"))     RGA_SensorFilter.maxDwell = dataObject["maxDwell"].toInt();
        emit SensorFilter_Info();
        // qDebug()<< RGA_SensorFilter.massMax<< RGA_SensorFilter.massMin<<RGA_SensorFilter.minDwell<<RGA_SensorFilter.maxDwell;
    }
}
void API::RGA_ScanInfo_Read(QJsonObject jsonObject)
{
    if((jsonObject["origin"].toString()=="/mmsp/scanInfo")&&(jsonObject["name"].toString()=="got"))
    {
        QJsonObject dataObject=jsonObject["data"].toObject();
        if (dataObject.contains("firstScan"))                  RGA_ScanInfo.firstScan = dataObject["firstScan"].toInt();
        if (dataObject.contains("lastScan"))                   RGA_ScanInfo.lastScan = dataObject["lastScan"].toInt();
        if (dataObject.contains("currentScan"))                RGA_ScanInfo.currentScan = dataObject["currentScan"].toInt();
        if (dataObject.contains("pointsPerScan"))              RGA_ScanInfo.pointsPerScan = dataObject["pointsPerScan"].toInt();
        if (dataObject.contains("pointsInCurrentScan"))        RGA_ScanInfo.pointsInCurrentScan = dataObject["pointsInCurrentScan"].toInt();
        if (dataObject.contains("scanning"))                   RGA_ScanInfo.scanning = dataObject["scanning"].toString();
    }
}
void API::RGA_communication_Read(QJsonObject jsonObject)
{
    if((jsonObject["origin"].toString()=="/mmsp/communication/controlInfo")&&(jsonObject["name"].toString()=="got"))
    {
        QJsonObject dataObject=jsonObject["data"].toObject();
        if (dataObject.contains("sessionId"))                   RGA_Communication.sessionId= dataObject["sessionId"].toString();
        if (dataObject.contains("ipAddress"))                   RGA_Communication.ipAddress = dataObject["ipAddress"].toString();
        if (dataObject.contains("controlled"))                  RGA_Communication.controlled = dataObject["controlled"].toBool();
        if (dataObject.contains("locked"))                  RGA_Communication.locked = dataObject["locked"].toBool();
        if (dataObject.contains("amInControl"))                  RGA_Communication.amInControl = dataObject["amInControl"].toBool();
        if (dataObject.contains("canRequestControl"))                  RGA_Communication.canRequestControl = dataObject["canRequestControl"].toBool();
        if (dataObject.contains("canTakeControl"))                  RGA_Communication.canTakeControl = dataObject["canTakeControl"].toBool();
        if (dataObject.contains("canForceControl"))                  RGA_Communication.canForceControl = dataObject["canForceControl"].toBool();
    }
    if((jsonObject["origin"].toString()=="/mmsp/communication/issueLog")&&(jsonObject["name"].toString()=="got"))
    {
        QJsonObject dataObject=jsonObject["data"].toObject();
        if (dataObject.contains("sessionId"))                   RGA_Communication.sessionId= dataObject["sessionId"].toString();
        if (dataObject.contains("ipAddress"))                   RGA_Communication.ipAddress = dataObject["ipAddress"].toString();
        if (dataObject.contains("controlled"))                  RGA_Communication.controlled = dataObject["controlled"].toBool();
        if (dataObject.contains("locked"))                  RGA_Communication.controlled = dataObject["locked"].toBool();
        if (dataObject.contains("amInControl"))                  RGA_Communication.amInControl = dataObject["amInControl"].toBool();
        if (dataObject.contains("canRequestControl"))                  RGA_Communication.controlled = dataObject["canRequestControl"].toBool();
        if (dataObject.contains("canTakeControl"))                  RGA_Communication.controlled = dataObject["canTakeControl"].toBool();
        if (dataObject.contains("canForceControl"))                  RGA_Communication.controlled = dataObject["canForceControl"].toBool();
    }
}
void API::RGA_ScanSetup_Read(QJsonObject jsonObject)
{
    if((jsonObject["origin"].toString()=="/mmsp/scanSetup/ppamurange")&&(jsonObject["name"].toString()=="got"))
    {
       if (jsonObject.contains("data")) RGA_ScanSetup.ppamurange=jsonObject["data"].toString();
    }
    else  if((jsonObject["origin"].toString()=="/mmsp/scanSetup/scanCount")&&(jsonObject["name"].toString()=="got"))
    {
        if (jsonObject.contains("data")) RGA_ScanSetup.scanCount=jsonObject["data"].toInt();
    }
    else  if((jsonObject["origin"].toString()=="/mmsp/scanSetup/startChannel")&&(jsonObject["name"].toString()=="got"))
    {
        if (jsonObject.contains("data")) RGA_ScanSetup.startChannel=jsonObject["data"].toInt();
    }
    else  if((jsonObject["origin"].toString()=="/mmsp/scanSetup/stopChannel")&&(jsonObject["name"].toString()=="got"))
    {
        if (jsonObject.contains("data")) RGA_ScanSetup.stopChannel=jsonObject["data"].toInt();
    }
    else  if((jsonObject["origin"].toString()=="/mmsp/scanSetup/leakCheckMass")&&(jsonObject["name"].toString()=="got"))
    {
        if (jsonObject.contains("data")) RGA_ScanSetup.leakCheckMass=jsonObject["data"].toInt();
    }
    else  if((jsonObject["origin"].toString()=="/mmsp/scanSetup/leakCheckMass")&&(jsonObject["name"].toString()=="got"))
    {
        if (jsonObject.contains("data")) RGA_ScanSetup.leakCheckMass=jsonObject["data"].toInt();
    }
    else
    {
        // qDebug()<<1;
    }

}
void API::RGA_Status_Read(QJsonObject jsonObject)
{
    if((jsonObject["origin"].toString()=="/mmsp/status")&&(jsonObject["name"].toString()=="got"))
    {
        QJsonObject dataObject=jsonObject["data"].toObject();
        if (dataObject.contains("systemStatus"))
        {
             RGA_Status.systemStatus = dataObject["systemStatus"].toVariant().toUInt();
             systemStatuss_Change();
        }
        if (dataObject.contains("systemStatus2"))
        {
            RGA_Status.systemStatus2= dataObject["systemStatus2"].toInt();
            RGA_Status.systemStatuss.Filament1_Open=extractSingleBit(RGA_Status.systemStatus2,0);                    //systemStatus1 0:灯丝1状态
            RGA_Status.systemStatuss.Filament2_Open=extractSingleBit(RGA_Status.systemStatus2,1);                    //systemStatus2 1:灯丝2状态
        }
        if(dataObject.contains("emPressTrip")) RGA_Status.emPressTrip= dataObject["emPressTrip"].toInt();
        if(dataObject.contains("hardwareWarnings"))
        {
            RGA_Status.hardwareWarning= dataObject["hardwareWarnings"].toInt();
            RGA_Status.hardwareWarnings.emission           =extractSingleBit(RGA_Status.hardwareWarning,31);   // 31位 发射级
            RGA_Status.hardwareWarnings.electronMultiplier =extractSingleBit(RGA_Status.hardwareWarning,30);   // 30位 电子倍增器
            RGA_Status.hardwareWarnings.anode              =extractSingleBit(RGA_Status.hardwareWarning,29);   // 29位 阳极
            RGA_Status.hardwareWarnings.rfBoard            =extractSingleBit(RGA_Status.hardwareWarning,28);   // 28位 射频电路板
            RGA_Status.hardwareWarnings.totalPressure      =extractSingleBit(RGA_Status.hardwareWarning,27);   // 27位 总压测量
            RGA_Status.hardwareWarnings.internalPower      =extractSingleBit(RGA_Status.hardwareWarning,26);   // 26位 内部电源
            RGA_Status.hardwareWarnings.electronicsBoxTemp =extractSingleBit(RGA_Status.hardwareWarning,25);   // 25位 电路板温度
            RGA_Status.hardwareWarnings.electrometer       =extractSingleBit(RGA_Status.hardwareWarning,24);   // 24位 静电计
            RGA_Status.hardwareWarnings.database           =extractSingleBit(RGA_Status.hardwareWarning,23);   // 23位 数据库
            RGA_Status.hardwareWarnings.massFilter         =extractSingleBit(RGA_Status.hardwareWarning,22);   // 22位 四极杆
            RGA_Status.hardwareWarnings.ionSource          =extractSingleBit(RGA_Status.hardwareWarning,21);   // 21位 离子源
            RGA_Status.hardwareWarnings.detector           =extractSingleBit(RGA_Status.hardwareWarning,20);   // 20位 收集极
            RGA_Status.hardwareWarnings.dds                =extractSingleBit(RGA_Status.hardwareWarning,19);   // 19位数字频率合成器
            RGA_Status.hardwareWarnings.dspComm            =extractSingleBit(RGA_Status.hardwareWarning,18);   // 18位 DSP通信
            RGA_Status.hardwareWarnings.decComm            =extractSingleBit(RGA_Status.hardwareWarning,17);   // 17位
            RGA_Status.hardwareWarnings.peakfind           =extractSingleBit(RGA_Status.hardwareWarning,16);   // 16位 巡峰
            RGA_Status.hardwareWarnings.filamentPotential  =extractSingleBit(RGA_Status.hardwareWarning,15);   // 15位 灯丝驱动
            RGA_Status.hardwareWarnings.filamentCurrent    =extractSingleBit(RGA_Status.hardwareWarning,14);   // 14位 灯丝电流
            RGA_Status.hardwareWarnings.focusPotential     =extractSingleBit(RGA_Status.hardwareWarning,13);   // 13位 聚焦透镜
            RGA_Status.hardwareWarnings.cathodePotential   =extractSingleBit(RGA_Status.hardwareWarning,12);   // 12位 阴极驱动
            RGA_Status.hardwareWarnings.decControlExcursion=extractSingleBit(RGA_Status.hardwareWarning,11);   // 11位 DEC控制
        }
        if(dataObject.contains("emStretch"))       RGA_Status.emPressTrip= dataObject["emStretch"].toInt();
        if(dataObject.contains("emissionStretch")) RGA_Status.emissionStretch= dataObject["emissionStretch"].toInt();
        if(dataObject.contains("motherBoardPowerOnTime")) RGA_Status.motherBoardPowerOnTime= dataObject["motherBoardPowerOnTime"].toInt();
        if(dataObject.contains("emOnTime")) RGA_Status.emOnTime= dataObject["emOnTime"].toInt();
        if(dataObject.contains("cpuPowerStretch")) RGA_Status.cpuPowerStretch= dataObject["cpuPowerStretch"].toInt();
        if(dataObject.contains("fil1Open")) RGA_Status.fil1Open= dataObject["fil1Open"].toInt();
        if(dataObject.contains("fil2Open")) RGA_Status.fil2Open= dataObject["fil2Open"].toInt();
        if(dataObject.contains("powerSupplyPowerOnTime")) RGA_Status.powerSupplyPowerOnTime= dataObject["powerSupplyPowerOnTime"].toInt();
        if(dataObject.contains("cpuPowerOnTime")) RGA_Status.cpuPowerOnTime= dataObject["cpuPowerOnTime"].toInt();
        if(dataObject.contains("filaments"))
        {
            QJsonArray filaments = dataObject["filaments"].toArray();
            for (int i=0;i<filaments.size();i++)
            {
                QJsonObject filamentObj = filaments[i].toObject();
                RGA_Status.filaments[i].id= filamentObj["@id"].toInt();
                RGA_Status.filaments[i].emisOnTime= filamentObj["emisOnTime"].toInt();
                RGA_Status.filaments[i].emisPressTrip = filamentObj["emisPressTrip"].toInt();
            }
        }
        if(dataObject.contains("peakfind"))
        {
            QJsonArray peakfind = dataObject["peakfind"].toArray();
            for (int i=0;i<peakfind.size();i++)
            {
                QJsonObject peakfindObj = peakfind[i].toObject();
                RGA_Status.peakfinds[i].id= peakfindObj["@id"].toInt();
                RGA_Status.peakfinds[i].peakfindStatus= peakfindObj["peakfindStatus"].toInt();
                RGA_Status.peakfinds[i].peakfindOffset = peakfindObj["peakfindOffset"].toInt();
            }
        }
    }
    else if((jsonObject["origin"].toString()=="/mmsp/status/systemStatus")&&(jsonObject["name"].toString()=="got"))
    {
        RGA_Status.systemStatus = jsonObject["data"].toVariant().toUInt();
        systemStatuss_Change();
    }
}
void API::systemStatuss_Change()
{
    RGA_Status.systemStatuss.resetOccurred=extractSingleBit(RGA_Status.systemStatus,0);                    // 0: 重置是否发生
    RGA_Status.systemStatuss.versiscanActive =extractSingleBit(RGA_Status.systemStatus,1);                 // 1: Versiscan是否激活
    RGA_Status.systemStatuss.reResonateAlways =extractSingleBit(RGA_Status.systemStatus,2);                // 2: RF生成器是否总是重新共振
    RGA_Status.systemStatuss.tempCompensationRFBoard =extractSingleBit(RGA_Status.systemStatus,3);         // 3: RF板温度补偿设定点是否开启
    RGA_Status.systemStatuss.ionCurrentEMEquivalent =extractSingleBit(RGA_Status.systemStatus,4);          // 5: EM等效离子电流是否开启
    RGA_Status.systemStatuss.ionSourceNormalization =extractSingleBit(RGA_Status.systemStatus,5);          // 4: 离子源标准化是否开启
    RGA_Status.systemStatuss.gainOffsetCompensationRFBoard =extractSingleBit(RGA_Status.systemStatus,6);   // 6: RF板增益和偏移补偿是否开启
    RGA_Status.systemStatuss.freqChangeCompensationRF =extractSingleBit(RGA_Status.systemStatus,7);        // 7: RF频率变化补偿是否开启
    RGA_Status.systemStatuss.highPowerReResonateRF =extractSingleBit(RGA_Status.systemStatus,8);           // 8: RF生成器高功率是否重新共振
    RGA_Status.systemStatuss.baselineCorrection =extractSingleBit(RGA_Status.systemStatus,9);              // 9: 基线校正是否开启
    RGA_Status.systemStatuss.singleMeasurementBaseline =extractSingleBit(RGA_Status.systemStatus,10);       // 10: 单次基线测量计划是否开启
    RGA_Status.systemStatuss.continuousMeasurementBaseline =extractSingleBit(RGA_Status.systemStatus,11);   // 11: 连续基线测量计划是否开启
    RGA_Status.systemStatuss.peakPositionCorrection =extractSingleBit(RGA_Status.systemStatus,12);          // 12: 峰位校正是否开启
    RGA_Status.systemStatuss.measurementScheduledPeakPosition =extractSingleBit(RGA_Status.systemStatus,13);// 13: 峰位测量计划是否开启
    RGA_Status.systemStatuss.linearizationPartialPressure =extractSingleBit(RGA_Status.systemStatus,14);    // 14: 总压基部分压线性化是否开启
    RGA_Status.systemStatuss.temperatureCompensationElectrometer=extractSingleBit(RGA_Status.systemStatus,15); // 15: 电表温度补偿是否开启
    RGA_Status.systemStatuss.statusRelay =extractSingleBit(RGA_Status.systemStatus,16);                     // 16: 状态继电器是否开启
    RGA_Status.systemStatuss.dataSmoothing =extractSingleBit(RGA_Status.systemStatus,17);                   // 17: 数据平滑是否开启
    RGA_Status.systemStatuss.peakTopWiden =extractSingleBit(RGA_Status.systemStatus,18);                    // 18: 峰顶展宽是否开启
    RGA_Status.systemStatuss.reserved =extractSingleBit(RGA_Status.systemStatus,19);                        // 19: 保留位
    RGA_Status.systemStatuss.globalIonEnergy =extractSingleBit(RGA_Status.systemStatus,20);                 // 20: 是否使用全局离子能量
    RGA_Status.systemStatuss.interlockElectronMultiplier =extractSingleBit(RGA_Status.systemStatus,21);     // 21: 电子倍增器互锁是否激活
    RGA_Status.systemStatuss.requestPendingElectronMultiplier=extractSingleBit(RGA_Status.systemStatus,22); // 22: 电子倍增器请求是否待处理
    RGA_Status.systemStatuss.electronMultiplier =extractSingleBit(RGA_Status.systemStatus,23);              // 23: 电子倍增器是否开启
    RGA_Status.systemStatuss.rodPolarity =extractSingleBit(RGA_Status.systemStatus,24);                     // 24: 极板极性是否反转
    RGA_Status.systemStatuss.rfGenerator =extractSingleBit(RGA_Status.systemStatus,25);                     // 25: RF生成器是否开启
    RGA_Status.systemStatuss.filamentSelected =extractSingleBit(RGA_Status.systemStatus,26);                // 26: 是否选择了特定的灯丝
    RGA_Status.systemStatuss.interlockEmission =extractSingleBit(RGA_Status.systemStatus,27);               // 27: 发射互锁是否激活
    RGA_Status.systemStatuss.requestPendingEmission =extractSingleBit(RGA_Status.systemStatus,28);          // 28: 发射请求是否待处理
    RGA_Status.systemStatuss.degasModeEmission =extractSingleBit(RGA_Status.systemStatus,29);               // 29: 发射灯丝是否处于除气模式
    RGA_Status.systemStatuss.constantPowerModeEmission =extractSingleBit(RGA_Status.systemStatus,30);       // 30: 发射灯丝是否处于恒功率模式
    RGA_Status.systemStatuss.regulatedCurrentModeEmission =extractSingleBit(RGA_Status.systemStatus,31);    // 31: 发射灯丝是否处于调节电流模式
    RGA_Status.systemStatuss.Filament1_Open=extractSingleBit(RGA_Status.systemStatus,1);                    //systemStatus1 0:灯丝1状态
    RGA_Status.systemStatuss.Filament2_Open=extractSingleBit(RGA_Status.systemStatus,2);                    //systemStatus2 1:灯丝2状态
    if( RGA_Status.systemStatuss.versiscanActive==1)
    {

        if((!Scan_Timer->isActive())&&(RGA_Scan_Data.Scan_Switch==false)&&(RGA_TCPIP_Switch))
        {
            qDebug()<<"Scan_sTart";
            emit scan_Start();
            Data_Number=0;
            RGA_Scan_Setting_Read();
        }
    }
    else
    {
        if((Scan_Timer->isActive())&&(RGA_Scan_Data.Scan_Switch==true))
          {
            qDebug()<<"Scan_sTop";
            emit scan_Stop();
            Scan_Timer->stop();

          }
        RGA_Scan_Data.Scan_Switch=false;
    }

}


bool API::extractSingleBit(uint32_t status, int bitIndex) {
    // 确保索引在有效范围内
    if (bitIndex < 0 || bitIndex >= 32) {
        return false; // 或者抛出异常
    }
    // 提取特定位的状态
    return (status >> ( bitIndex)) & 1;
}
