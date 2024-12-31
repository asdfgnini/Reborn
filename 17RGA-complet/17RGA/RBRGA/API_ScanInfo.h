#ifndef API_SCANINFO_H
#define API_SCANINFO_H

#include <QString>



struct API_ScanInfo
{

    QString scanning;        //是否在扫描
    int currentScan;         //当前扫描序列编号
    int pointsPerScan;       //一个扫描数据点
    int pointsInCurrentScan; //当前扫描数据点编号
    int lastScan;            //最后一次扫描编号
    int firstScan;           //第一次扫描编号

};





#endif // API_SCANINFO_H
