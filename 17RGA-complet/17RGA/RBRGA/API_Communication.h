#ifndef API_COMMUNICATION_H
#define API_COMMUNICATION_H

#include <QString>

struct Issue_Log
{



};

struct Error_Log
{



};


struct API_Communication
{
    QString sessionId;
    QString ipAddress;      //ip地址
    bool controlled;
    bool locked;
    bool amInControl;
    bool canRequestControl;
    bool canTakeControl;
    bool canForceControl;
    QString macAddress; //物理地址
    int   locationIdentifyCPU;
    QString mask;       //子网掩码
    int port;           //端口号
    int DHCP;           //DHCP
    QString gateway;    //网关
    Issue_Log RGA_Log;
    Error_Log RGA_Error_Log;
};














#endif // API_COMMUNICATION_H
