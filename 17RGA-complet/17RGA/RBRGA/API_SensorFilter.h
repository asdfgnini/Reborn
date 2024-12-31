#ifndef API_SENSORFILTER_H
#define API_SENSORFILTER_H

#include <QString>

struct API_SensorFilter
{
    QString workingTune;
    int peakTopWidenStartMass;
    int rodPolarity;
    int peakTopWidenSlope;
    int peakTopWidenIntercept;
    int maxDwell;        //扫描间隔最小值
    int minDwell;        //扫描间隔最大值
    int massMin;         //质量范围最小值
    int massMax;         //质量范围最大值
    int designMassMin;   //质量范围最小值（设计）
    int designMassMax;   //质量范围最大值（设计）
    int dwellMin;    //扫描间隔最小值
    int dwellMax;    //扫描间隔最大值
};

#endif // API_SENSORFILTER_H
