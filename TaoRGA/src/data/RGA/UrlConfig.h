#pragma once

#include "URLManger.h"
#include <QPair>




// URL配置类
class UrlConfig {
public:
    // 在 ConnectConfig 和 ScanConfig 中也相应修改
    struct ConnectConfig {
        QStringList pathParts;                          // URL路径部分
        QList<QPair<QString, QVariant>> params;         // URL参数
        QList<QPair<QString, QVariant>> bufferParams;   // 缓冲区参数
        QMap<QString, ParsePath> parsePaths;            // 修改为 QMap
        RequestType requestType;                        // 请求类型
        bool addToBufferImmediately{true};             // 是否立即添加到缓冲区
    };

    struct ScanConfig {
        QStringList pathParts;                          // URL路径部分
        QList<QPair<QString, QVariant>> params;         // URL参数
        QList<QPair<QString, QVariant>> bufferParams;   // 缓冲区参数
        QMap<QString, ParsePath> parsePaths;            // 修改为 QMap
        RequestType requestType;                        // 请求类型
        bool needScanParams{false};                     // 是否需要扫描参数
        bool addToBufferImmediately{true};             // 是否立即添加到缓冲区
    };


static QList<ConnectConfig> getConnectConfigs() {
        return {
            {
                {"mmsp", "scanSetup", "startChannel","get"},
                {

                    
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step1"},  // 使用逗号分隔的字符串表示多个步骤
                    {"responseFormat", "JSON"},
                    {"priority", "1"},
                    {"needParse", "true"}
                },
                {
                    {
                            "deviceId",
                            {
                                "controlInfo.deviceId", 
                                "设备ID", 
                                "string",
                                ""
                            }
                    },
                    {
                            "status",
                            {
                                "controlInfo.status", 
                                "设备状态", 
                                "string",
                                ""
                            }
                    },
                    {
                        "temperature",
                        {
                            "controlInfo.temperature", 
                            "温度", 
                            "double",
                            "0.0"
                        }
                    }      
                },
                RequestType::ControlInfo,  // requestType
                true
            },

            {
                {"mmsp", "scanSetup", "stopChannel","get"},
                { },
                {
                    {"isRunning", "true"},
                    {"steps", "Step1"},  // 使用逗号分隔的字符串表示多个步骤
                    {"responseFormat", "JSON"},
                    {"priority", "1"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ControlInfo,  // requestType
                true
            },
            {
                {"mmsp", "scanSetup", "scanCount","get"},
                { },
                {
                    {"isRunning", "true"},
                    {"steps", "Step1"},  // 使用逗号分隔的字符串表示多个步骤
                    {"responseFormat", "JSON"},
                    {"priority", "1"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                true
            },
            {
                {"mmsp", "scanSetup", "scanInterval","get"},
                { },
                {
                    {"isRunning", "true"},
                    {"steps", "Step1"},  // 使用逗号分隔的字符串表示多个步骤
                    {"responseFormat", "JSON"},
                    {"priority", "1"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                true
            },
            {
                {"mmsp", "scanSetup", "leakCheckMass","get"},
                { },
                {
                    {"isRunning", "true"},
                    {"steps", "Step1"},  // 使用逗号分隔的字符串表示多个步骤
                    {"responseFormat", "JSON"},
                    {"priority", "1"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                true
            },
            {
                {"mmsp", "scanSetup", "scanTimeTotal","get"},
                { },
                {
                    {"isRunning", "true"},
                    {"steps", "Step1"},  // 使用逗号分隔的字符串表示多个步骤
                    {"responseFormat", "JSON"},
                    {"priority", "1"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                true
            },
            {
                {"mmsp", "scanSetup", "captureFileStatus","get"},
                { },
                {
                    {"isRunning", "true"},
                    {"steps", "Step1"},  // 使用逗号分隔的字符串表示多个步骤
                    {"responseFormat", "JSON"},
                    {"priority", "1"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                true
            },
            {
                {"mmsp", "scanSetup", "channels", "1", "get"},
                { },
                {
                    {"isRunning", "true"},
                    {"steps", "Step2"},  // 可以定义不同的步骤组合
                    {"responseFormat", "JSON"},
                    {"priority", "1"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                true
            },
            {
                {"mmsp", "scanSetup", "channels", "2", "get"},
                { },
                {
                    {"isRunning", "true"},
                    {"steps", "Step2"},  // 可以定义不同的步骤组合
                    {"responseFormat", "JSON"},
                    {"priority", "2"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                true
            },
            {
                {"mmsp", "scanSetup", "channels", "3", "get"},
                { },
                {
                    {"isRunning", "true"},
                    {"steps", "Step2"},  // 可以定义不同的步骤组合
                    {"responseFormat", "JSON"},
                    {"priority", "3"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                true
            },
            {
                {"mmsp", "scanSetup", "channels", "4", "get"},
                { },
                {
                    {"isRunning", "true"},
                    {"steps", "Step2"},  // 可以定义不同的步骤组合
                    {"responseFormat", "JSON"},
                    {"priority", "4"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                true
            },
            {
                {"mmsp", "scanSetup", "channels", "5", "get"},
                { },
                {
                    {"isRunning", "true"},
                    {"steps", "Step2"},  // 可以定义不同的步骤组合
                    {"responseFormat", "JSON"},
                    {"priority", "5"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                true
            },
            {
                {"mmsp", "scanSetup", "channels", "6", "get"},
                { },
                {
                    {"isRunning", "true"},
                    {"steps", "Step2"},  // 可以定义不同的步骤组合
                    {"responseFormat", "JSON"},
                    {"priority", "6"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                true
            },
            {
                {"mmsp", "scanSetup", "channels", "7", "get"},
                { },
                {
                    {"isRunning", "true"},
                    {"steps", "Step2"},  // 可以定义不同的步骤组合
                    {"responseFormat", "JSON"},
                    {"priority", "7"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                true
            },
            {
                {"mmsp", "scanSetup", "channels", "8", "get"},
                { },
                {
                    {"isRunning", "true"},
                    {"steps", "Step2"},  // 可以定义不同的步骤组合
                    {"responseFormat", "JSON"},
                    {"priority", "8"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                true
            },
            {
                {"mmsp", "scanSetup", "channels", "9", "get"},
                { },
                {
                    {"isRunning", "true"},
                    {"steps", "Step2"},  // 可以定义不同的步骤组合
                    {"responseFormat", "JSON"},
                    {"priority", "9"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                true
            },
            {
                {"mmsp", "scanSetup", "channels", "10", "get"},
                { },
                {
                    {"isRunning", "true"},
                    {"steps", "Step2"},  // 可以定义不同的步骤组合
                    {"responseFormat", "JSON"},
                    {"priority", "10"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                true
            },
            {
                {"mmsp", "scanSetup", "channels", "11", "get"},
                { },
                {
                    {"isRunning", "true"},
                    {"steps", "Step2"},  // 可以定义不同的步骤组合
                    {"responseFormat", "JSON"},
                    {"priority", "11"},
                    {"needParse", "true"}
                },  
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                true
            },
            {
                {"mmsp", "scanSetup", "channels", "12", "get"},
                { },
                {
                    {"isRunning", "true"},
                    {"steps", "Step2"},  // 可以定义不同的步骤组合
                    {"responseFormat", "JSON"},
                    {"priority", "12"},
                    {"needParse", "true"}       
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                true
            },
            {
                {"mmsp", "scanSetup", "channels", "13", "get"},
                { },
                {
                    {"isRunning", "true"},
                    {"steps", "Step2"},  // 可以定义不同的步骤组合
                    {"responseFormat", "JSON"},
                    {"priority", "13"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                true
            },
            {
                {"mmsp", "scanInfo", "get"},
                {
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step3"},
                    {"responseFormat", "JSON"},
                    {"priority", "1"},
                    {"needParse", "true"}
                },
                {
                    {
                        "firstScan",
                        {
                            "data.firstScan", 
                            "第1次扫描", 
                            "uint32_t",
                            ""
                        },
                    },
                    {
                        "lastScan",
                        {
                            "data.lastScan", 
                            "最后1次扫描", 
                            "uint32_t",
                            ""
                        }

                    },
                    {
                        "pointsInCurrentScan",
                        {
                            "data.pointsInCurrentScan", 
                            "当前扫描点数", 
                            "uint32_t",
                            ""
                        }
                    },
                    {
                        "pointsPerScan",
                        {

                            "data.pointsPerScan", 
                            "每次扫描点数", 
                            "uint32_t",
                            ""
                        }
                    },
                    {
                        "currentScan",
                        {
                            "data.currentScan", 
                            "当前扫描", 
                            "uint32_t",
                            ""
                        }
                    },
                    {
                        "scanning",
                        {
                            "data.scanning", 
                            "扫描状态", 
                            "bool",
                            ""
                        }
                    }
                },
                RequestType::ScanStatus,  // requestType
                true
            },
            {
                {"mmsp", "communication", "controlInfo", "get"},
                {
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step3"},
                    {"responseFormat", "JSON"},
                    {"priority", "1"},
                    {"needParse", "true"}
                },
                {
                    {
                        {
                            "amInControl",
                            {
                                "data.amInControl", 
                                "是否在控制", 
                                "bool",
                                ""
                            }
                        },
                        {
                            "canForceControl",
                            {
                                "data.canForceControl", 
                                "是否可以强制控制", 
                                "bool",
                                ""
                            }
                        },
                        {
                            "canRequestControl",
                            {
                                "data.canRequestControl", 
                                "是否可以请求控制", 
                                "bool",
                                ""
                            }
                        },
                        {
                            "canTakeControl",
                            {
                                "data.canTakeControl", 
                                "是否可以接管控制", 
                                "bool",
                                ""
                            }
                        },
                        {
                            "controlled",
                            {
                                "data.controlled", 
                                "是否被控制", 
                                "bool",
                                ""
                            }
                        },
                        {
                            "ipAddress",
                            {
                                "data.ipAddress", 
                                "IP地址", 
                                "string",
                                ""
                            }
                        },
                        {
                            "locked",
                            {
                                "data.locked", 
                                "是否锁定", 
                                "bool",
                                ""
                            }
                        },
                        {
                            "secondsSinceLastControlRequest",
                            {
                                "data.secondsSinceLastControlRequest", 
                                "上次控制请求时间", 
                                "uint16_t",
                                ""
                            }
                        },
                        {
                            "secondsSinceLastDataRequest",
                            {
                                "data.secondsSinceLastDataRequest", 
                                "上次数据请求时间", 
                                "uint16_t",
                                ""
                            }
                        },
                        {
                            "secondsSinceLastRequest",
                            {
                                "data.secondsSinceLastRequest", 
                                "上次请求时间", 
                                "uint16_t",
                                ""
                            }
                        }
                    },
                },
                RequestType::ControlInfo,  // requestType
                true
            },
            {
                {"mmsp", "communication", "control", "set"},
                {
                    {"force", ""}
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step3"},
                    {"responseFormat", "JSON"},
                    {"priority", "1"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ControlInfo,  // requestType
                false
            },

            {
                {"mmsp", "status", "systemStatus", "get"},
                {
                },
                {

                    {"isRunning", "true"},
                    {"steps", "Step3"},
                    {"responseFormat", "JSON"},
                    {"priority", "2"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::SystemStatus,  // requestType
                true
            },
            {
                {"mmsp", "measurement", "get"},
                {
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step3"},
                    {"responseFormat", "JSON"},
                    {"priority", "3"},
                    {"needParse", "true"}
                },
                {
                    {
                        "nextScanNumber",
                        {
                            "data.nextScanNumber", 
                            "下次扫描序号", 
                            "uint32_t",
                            ""
                        }
                    },
                    {
                        "totalPressure",
                        {
                            "data.totalPressure", 
                            "总压", 
                            "double",
                            "0.0"
                        }
                    },
                },
                RequestType::Measurement,  // requestType
                true
            },
            {
                {"mmsp", "measurement", "binaryData","get"},
                {

                    {"@start","0"}
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step5"},
                    {"responseFormat", "JSON"},
                    {"priority", "3"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::Measurement,  // requestType
                true
            },
                {
                {"mmsp", "scanSetup", "set"},
                {
                    {"scanStop","Immediately"}
                },
                {
                    {"isRunning", "false"},
                    {"steps", "Step4"},
                    {"responseFormat", "JSON"},
                    {"priority", "2"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::DeviceBaseInfo,  // requestType
                true
            },
            {
                {"mmsp", "sensorIonSource", "get"},
                {
                    
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step6"},
                    {"responseFormat", "JSON"},
                    {"priority", "3"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::DeviceBaseInfo,  // requestType
                true
            }
        };
    }

    
    static QList<ScanConfig> getScanConfigs() 
    {
        return {
            {
                {"mmsp", "scanSetup", "set"},
                {
                    {"scanStop","Immediately"}
                },
                {
                    {"isRunning", "false"},
                    {"steps", "Step4"},
                    {"responseFormat", "JSON"},
                    {"priority", "2"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::DeviceBaseInfo,  // requestType
                false
                ,true
            },
            {
                {"mmsp", "generalControl", "set"},
                {
                    {"setEmission", "On"}
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step4"},
                    {"responseFormat", "JSON"},
                    {"priority", "1"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                false
                ,true
            },
            {
                {"mmsp", "scanSetup", "channels", "1", "set"},
                {
                    {"channelMode", "FixedNumber"},
                    {"dwell", "1"},
                    {"enabled", "True"}
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step4"},
                    {"responseFormat", "JSON"},
                    {"priority", "3"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                false
                ,true
            },
            {
                {"mmsp", "scanSetup", "channels", "2", "set"},
                {
                    {"channelMode", "Timestamp"},
                    {"dwell", "1"},
                    {"enabled", "True"}
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step4"},
                    {"responseFormat", "JSON"},
                    {"priority", "4"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                false
                ,true
            },
            {
                {"mmsp", "scanSetup", "channels", "3", "set"},
                {
                    {"channelMode", "SystemStatus"},
                    {"dwell", "1"},
                    {"enabled", "True"}
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step4"},
                    {"responseFormat", "JSON"},
                    {"priority", "5"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                false
                ,true
            },
            {
                {"mmsp", "scanSetup", "channels", "4", "set"},
                {
                    {"channelMode", "Sweep"},
                    {"enabled", "True"}
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step4"},
                    {"responseFormat", "JSON"},
                    {"priority", "6"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                true
                ,true
            },
            {
                {"mmsp", "scanSetup", "channels", "5", "set"},
                {
                    {"channelMode", "TotalPressure"},
                    {"dwell", "1"},
                    {"enabled", "True"}
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step4"},
                    {"responseFormat", "JSON"},
                    {"priority", "7"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                false
                ,true
            },
            {
                {"mmsp", "scanSetup", "channels", "6", "set"},
                {
                    {"channelMode", "Baseline"},
                    {"dwell", "32"},
                    {"ppamu", "1"},
                    {"enabled", "True"}
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step4"},
                    {"responseFormat", "JSON"},
                    {"priority", "8"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                false
                ,true
            },
            {
                {"mmsp", "scanSetup", "channels", "7", "set"},
                {
                    {"channelMode", "TPQuad"},
                    {"dwell", "32"},
                    {"ppamu", "10"},
                    {"enabled", "True"}
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step4"},
                    {"responseFormat", "JSON"},
                    {"priority", "9"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                false
                ,true
            },
             {
                {"mmsp", "scanSetup", "channels", "8", "set"},
                {
                    {"channelMode", "TotalPressureDetectorCurrent"},
                    {"dwell", "1"},
                    {"enabled", "True"}
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step4"},
                    {"responseFormat", "JSON"},
                    {"priority", "10"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                false
                ,true
             },
             {
                {"mmsp", "scanSetup", "channels", "9", "set"},
                {
                    {"channelMode", "EmissionCurrent"},
                    {"dwell", "1"},
                    {"enabled", "True"}
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step4"},
                    {"responseFormat", "JSON"},
                    {"priority", "11"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                false
                ,true
             },
             {
                {"mmsp", "scanSetup", "channels", "10", "set"},
                {
                    {"channelMode", "AnodePotential"},
                    {"dwell", "1"},
                    {"enabled", "True"}
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step4"},
                    {"responseFormat", "JSON"},
                    {"priority", "12"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                false
                ,true
            },
            {
                {"mmsp", "scanSetup", "channels", "11", "set"},
                {
                    {"channelMode", "ElectronEnergy"},
                    {"dwell", "1"},
                    {"enabled", "True"}
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step4"},
                    {"responseFormat", "JSON"},
                    {"priority", "13"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                false
                ,true
            },
            {
                {"mmsp", "scanSetup", "channels", "12", "set"},
                {
                    {"channelMode", "FocusPotential"},
                    {"dwell", "1"},
                    {"enabled", "True"}
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step4"},
                    {"responseFormat", "JSON"},
                    {"priority", "14"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                false
                ,true
            },
            {
                {"mmsp", "scanSetup", "channels", "13", "set"},
                {
                    {"channelMode", "FilamentCurrent"},
                    {"dwell", "1"},
                    {"enabled", "True"}
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step4"},
                    {"responseFormat", "JSON"},
                    {"priority", "15"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                false
                ,true
            },
            {
                {"mmsp", "scanSetup", "set"},
                {
                    {"startChannel","1"},{"stopChannel","13"}
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step4"},
                    {"responseFormat", "JSON"},
                    {"priority", "16"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                false
                ,true
            },
            {
                {"mmsp", "scanSetup", "set"},
                {
                    {"startChannel","1"},{"stopChannel","13"},
                    {"scanCount","-1"},{"scanInterval","0"},
                    {"scanStart","1"}
                },

                {
                    {"isRunning", "true"},
                    {"steps", "Step4"},
                    {"responseFormat", "JSON"},
                    {"priority", "17"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                false
                ,true
            },
            {
                {"mmsp", "measurement", "binaryData","get"},
                {

                    {"@start","0"}
                },
                {
                    {"isRunning", "true"},
                    {"steps", "Step5"},
                    {"responseFormat", "JSON"},
                    {"priority", "1"},
                    {"needParse", "true"}
                },
                {
                    {
                        "deviceId",
                        {
                            "controlInfo.deviceId", 
                            "设备ID", 
                            "string",
                            ""
                        }
                    },
                },
                RequestType::ScanStatus,  // requestType
                false
                ,true
            }

        };
    }
};
// 定义配置过滤器
class ConfigFilter {
public:
    // 链式设置方法
    ConfigFilter& withPathParts(const QStringList& parts) {
        pathParts = parts;
        return *this;
    }

    ConfigFilter& withParamKeys(const QStringList& keys) {
        paramKeys = keys;
        return *this;
    }

    ConfigFilter& withParamValues(const QStringList& values) {
        paramValues = values;
        return *this;
    }

    ConfigFilter& withBufferParamKeys(const QStringList& keys) {
        bufferParamKeys = keys;
        return *this;
    }

    ConfigFilter& withBufferParamValues(const QStringList& values) {
        bufferParamValues = values;
        return *this;
    }

    // 单个添加方法
    ConfigFilter& addPathPart(const QString& part) {
        pathParts.append(part);
        return *this;
    }

    ConfigFilter& addParamKey(const QString& key) {
        paramKeys.append(key);
        return *this;
    }

    ConfigFilter& addParamValue(const QString& value) {
        paramValues.append(value);
        return *this;
    }

    // 匹配 ConnectConfig
    bool matches(const UrlConfig::ConnectConfig& config) const {
        return matchesConfig(config.pathParts, config.params, config.bufferParams);
    }

    // 匹配 ScanConfig
    bool matches(const UrlConfig::ScanConfig& config) const {
        return matchesConfig(config.pathParts, config.params, config.bufferParams);
    }

private:
    // 通用的匹配逻辑
    bool matchesConfig(const QStringList& configPathParts,
                      const QList<QPair<QString, QVariant>>& configParams,
                      const QList<QPair<QString, QVariant>>& configBufferParams) const 
    {
        // 检查路径部分
        if (!pathParts.isEmpty()) {
            for (const QString& part : pathParts) {
                if (!configPathParts.contains(part)) {
                    qDebug() << "Path part not matched:" << part;
                    return false;
                }
            }
        }
        
        // 检查参数键和值
        if (!paramKeys.isEmpty() || !paramValues.isEmpty()) {
            for (int i = 0; i < paramKeys.size(); ++i) {
                bool found = false;
                for (const auto& param : configParams) {
                    if (paramKeys.at(i) == param.first &&
                        (paramValues.isEmpty() || param.second.toString() == paramValues.value(i))) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    qDebug() << "Param not matched:" << paramKeys.at(i);
                    return false;
                }
            }
        }
        
        // 检查缓冲区参数键和值
        if (!bufferParamKeys.isEmpty() || !bufferParamValues.isEmpty()) {
            for (int i = 0; i < bufferParamKeys.size(); ++i) {
                bool found = false;
                for (const auto& param : configBufferParams) {
                    if (bufferParamKeys.at(i) == param.first &&
                        (bufferParamValues.isEmpty() || param.second.toString() == bufferParamValues.value(i))) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    qDebug() << "Buffer param not matched:" << bufferParamKeys.at(i);
                    return false;
                }
            }
        }
        
        return true;
    }

private:
    QStringList pathParts;        // 路径部分匹配
    QStringList paramKeys;        // 参数键匹配
    QStringList paramValues;      // 参数值匹配
    QStringList bufferParamKeys;  // 缓冲区参数键匹配
    QStringList bufferParamValues;// 缓冲区参数值匹配
};
