#ifndef API_STATUS_H
#define API_STATUS_H



struct hardware_bit {
    bool emission           ;  // 31位 发射级
    bool electronMultiplier ;  // 30位 电子倍增器
    bool anode              ;  // 29位 阳极
    bool rfBoard            ;  // 28位 射频电路板
    bool totalPressure      ;  // 27位 总压测量
    bool internalPower      ;  // 26位 内部电源
    bool electronicsBoxTemp ;  // 25位 电路板温度
    bool electrometer       ;  // 24位 静电计
    bool database           ;  // 23位 数据库
    bool massFilter         ;  // 22位 四极杆
    bool ionSource          ;  // 21位 离子源
    bool detector           ;  // 20位 收集极
    bool dds                ;  // 19位数字频率合成器
    bool dspComm            ;  // 18位 DSP通信
    bool decComm            ;  // 17位
    bool peakfind           ;  // 16位 巡峰
    bool filamentPotential  ;  // 15位 灯丝驱动
    bool filamentCurrent    ;  // 14位 灯丝电流
    bool focusPotential     ;  // 13位 聚焦透镜
    bool cathodePotential   ;  // 12位 阴极驱动
    bool decControlExcursion;  // 11位 DEC控制
};

struct StatusFlag_bit {
    bool resetOccurred ;                   // 0: 重置是否发生
    bool versiscanActive ;                 // 1: Versiscan是否激活
    bool reResonateAlways ;                // 2: RF生成器是否总是重新共振
    bool tempCompensationRFBoard ;         // 3: RF板温度补偿设定点是否开启
    bool ionCurrentEMEquivalent ;          // 5: EM等效离子电流是否开启
    bool ionSourceNormalization ;          // 4: 离子源标准化是否开启
    bool gainOffsetCompensationRFBoard ;   // 6: RF板增益和偏移补偿是否开启
    bool freqChangeCompensationRF ;        // 7: RF频率变化补偿是否开启
    bool highPowerReResonateRF ;           // 8: RF生成器高功率是否重新共振
    bool baselineCorrection ;              // 9: 基线校正是否开启
    bool singleMeasurementBaseline ;       // 10: 单次基线测量计划是否开启
    bool continuousMeasurementBaseline ;   // 11: 连续基线测量计划是否开启
    bool peakPositionCorrection ;          // 12: 峰位校正是否开启
    bool measurementScheduledPeakPosition ;// 13: 峰位测量计划是否开启
    bool linearizationPartialPressure ;    // 14: 总压基部分压线性化是否开启
    bool temperatureCompensationElectrometer; // 15: 电表温度补偿是否开启
    bool statusRelay ;                     // 16: 状态继电器是否开启
    bool dataSmoothing ;                   // 17: 数据平滑是否开启
    bool peakTopWiden ;                    // 18: 峰顶展宽是否开启
    bool reserved ;                        // 19: 保留位
    bool globalIonEnergy ;                 // 20: 是否使用全局离子能量
    bool interlockElectronMultiplier ;     // 21: 电子倍增器互锁是否激活
    bool requestPendingElectronMultiplier; // 22: 电子倍增器请求是否待处理
    bool electronMultiplier ;              // 23: 电子倍增器是否开启
    bool rodPolarity ;                     // 24: 极板极性是否反转
    bool rfGenerator ;                     // 25: RF生成器是否开启
    bool filamentSelected ;                // 26: 是否选择了特定的灯丝
    bool interlockEmission ;               // 27: 发射互锁是否激活
    bool requestPendingEmission ;          // 28: 发射请求是否待处理
    bool degasModeEmission ;               // 29: 发射灯丝是否处于除气模式
    bool constantPowerModeEmission ;       // 30: 发射灯丝是否处于恒功率模式
    bool regulatedCurrentModeEmission ;    // 31: 发射灯丝是否处于调节电流模式
    bool Filament1_Open;                   //systemStatus1 0:灯丝1状态
    bool Filament2_Open;                   //systemStatus2 1:灯丝2状态
};

struct filaments_State{
    int id;
    int emisOnTime;     //灯丝导通累计时间
    int emisPressTrip;  //过压次数
};

struct peakfind{
    int id;
    int peakfindOffset;
    int peakfindStatus;
};

struct API_Status
{
    int hardwareError;          //硬件故障，写1清除故障
    hardware_bit hardwareErrors;
    int hardwareWarning;        //硬件警告，写1清除故障
    hardware_bit hardwareWarnings;
    int emPressTrip;            //灯丝过压次数
    int emStretch;              //EM打开时间
    int emissionStretch;        //发射极打开时间
    unsigned  int systemStatus;           //系统状态1
    unsigned  systemStatus2;          //系统状态2
    StatusFlag_bit systemStatuss;
    int motherBoardPowerOnTime; //主板累计开机时间
    int emOnTime;               //EM累计开机时间
    int cpuPowerStretch;        //本次CPU电源持续时间
    int fil1Open;               //灯丝1
    int fil2Open;               //灯丝2
    peakfind peakfinds[650];      //巡峰状态
    filaments_State filaments[3]; //灯丝状态
    int powerSupplyPowerOnTime; //电源板累计开机时间
    int cpuPowerOnTime;         //CPU累计开机时间

};










#endif // API_STATUS_H
