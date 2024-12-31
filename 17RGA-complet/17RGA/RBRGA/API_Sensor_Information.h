#ifndef API_SENSOR_INFORMATION_H
#define API_SENSOR_INFORMATION_H

#include <QString>

enum FILAMENT_TYPE   //灯丝类型
{
    FILAMENT_TYPE_TUNGSTEN,               //钨丝
    FILAMENT_TYPE_YTTRIA_COATED_IRIDIUM   //铱丝
};

enum DETECTOR_TYPE   //收集器类型
{
    DETECTOR_TYPE_FC,   //法拉第杯
    DETECTOR_TYPE_CDEM, //电子倍增器
    DETECTOR_TYPE_MCP,
    DETECTOR_TYPE_366B
};

enum IONSOURCE_TYPE   //离子源类型
{
    IONSOURCE_TYPE_OPEN,   //开发
    IONSOURCE_TYPE_CLOSED, //封闭
    IONSOURCE_TYPE_GRID,   //栅格
    IONSOURCE_TYPE_CROSSBEAM  //横梁
};

enum FILTER_TYPE   //四极杆类型
{
    FILTER_TYPE_STANDARD
};

struct Filament_State
{
    int Id;
    FILAMENT_TYPE Type;

};


struct API_Sensor_Information
{
    QString name;     //名称
    int serialNumber; //编号
    IONSOURCE_TYPE ionSourceType;       //Enumerated Ion Source Type: 0=open, 1=closed, 2=grid, 3=crossbeam  枚举离子源类型:0=开放，1=封闭，2=栅格，3=横梁
    QString detectorType;               //Enumerated detector type: 0=FC, 1=CDEM, 2=MCP, 3=366B 枚举检测器类型:0=FC, 1=CDEM, 2=MCP, 3=366B
    Filament_State filament[3];     //Enumerated Filament Type: 0=Tungsten, 1=Yttria coated Iridium 枚举灯丝类型:0=钨，1=钇包覆铱
    FILTER_TYPE   massFilterType;       //Enumerated Mass filter type: 0=Standard（枚举质量过滤器类型:0=标准）
    QString description;                //The Sensor Description, stored in the SD card（传感器描述信息，存储在SD卡中）

};



#endif // API_SENSOR_INFORMATION_H


