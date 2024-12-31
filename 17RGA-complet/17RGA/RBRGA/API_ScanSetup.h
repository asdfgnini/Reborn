#ifndef API_SCANSETUP_H
#define API_SCANSETUP_H

#include <QString>



struct Channel_Setting
{
    int startMass;
    int stopMass;
    int ppamu;
    int dwell;
    bool enable;
    QString channelMode;
};


struct API_ScanSetup
{
    QString ppamurange;    //分辨率范围
    int scanCount;         //扫描次数
    int dwellGlobal;       //扫描间隙
    int leakCheckMass;     //检漏质量数
    int scanInterval;      //扫描间隔时间
    int maxChannels;       //最大扫描通道
    int scanTimeTotal;     //完整一次扫描时间
    int startChannel;      //报告开始通道
    int stopChannel;       //报告结束通道
    int leadInDwell;       //扫描开始前延时时间
    Channel_Setting Channel[340];

};



#endif // API_SCANSETUP_H
