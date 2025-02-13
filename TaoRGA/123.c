    addUrlConfig({"mmsp", "communication", "controlInfo", "get"}, {}, {{"isRunning", false},{"step","Step1"}});
    addUrlConfig({"mmsp", "communication", "issueLog", "get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "communication", "control", "set"}, {{"force", QVariant()}},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "generalControl", "set"}, {{"setEmission", "On"}},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "status", "systemStatus", "get"}, {},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "measurement", "get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "measurement", "binaryData","get"}, {{"@start","0"}},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "scanInfo", "get"}, {},{{"isRunning", false},{"step","Step2"}});   

    addUrlConfig({"mmsp", "electronicsInfo", "get"}, {},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "sensorFiler", "get"}, {},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "sensorIonSource", "get"}, {},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "sensorInfo", "get"}, {},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "scanSetup", "startChannel", "get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "scanSetup", "stopChannel", "get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "scanSetup", "scanCount", "get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "scanSetup", "scanInterval", "get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "scanSetup", "leakCheckMass", "get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "scanSetup", "scanTimeTotal", "get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "scanSetup", "captureFileStatus", "get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "scanSetup", "channels", "1","get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "scanSetup", "channels", "2","get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "scanSetup", "channels", "3","get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "scanSetup", "channels", "4","get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "scanSetup", "channels", "5","get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "scanSetup", "channels", "6","get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "scanSetup", "channels", "7","get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "scanSetup", "channels", "8","get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "scanSetup", "channels", "9","get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "scanSetup", "channels", "10","get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "scanSetup", "channels", "11","get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "scanSetup", "channels", "12","get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "scanSetup", "channels", "13","get"}, {},{{"isRunning", false},{"step","Step2"}});
    addUrlConfig({"mmsp", "scanSetup", "set"}, {{"scanStop","Immediately"}},{{"isRunning", false},{"step","Step2"}});
    

    addUrlConfig({"mmsp", "scanSetup", "channels","6","set"}, 
    {{"channelMode","Baseline"},{"dwell","32"},{"ppamu","1"},{"enabled","True"}},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "scanSetup", "channels","7","set"}, 
    {{"channelMode","TPQuad"},{"dwell","32"},{"ppamu","10"},{"enabled","True"}},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "scanSetup", "channels","1","set"}, 
    {{"channelMode","FixedNumber"},{"dwell","1"},{"enabled","True"}},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "scanSetup", "channels","2","set"}, 
    {{"channelMode","Timestamp"},{"dwell","1"},{"enabled","True"}},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "scanSetup", "channels","3","set"}, 
    {{"channelMode","SystemStatus"},{"dwell","1"},{"enabled","True"}},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "scanSetup", "channels","4","set"}, 
    {{"channelMode","Sweep"},{"startMass","0"},{"stopMass","200"},{"dwell","8"},{"ppamu","1"},{"enabled","True"}},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "scanSetup", "channels","5","set"}, 
    {{"channelMode","TotalPressure"},{"dwell","1"},{"enable","True"}},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "scanSetup", "channels","8","set"}, 
    {{"channelMode","TotalPressureDetectorCurrent"},{"dwell","1"},{"enable","True"}},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "scanSetup", "channels","9","set"}, 
    {{"channelMode","EmissionCurrent"},{"dwell","1"},{"enable","True"}},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "scanSetup", "channels","10","set"}, 
    {{"channelMode","AnodePotential"},{"dwell","1"},{"enable","True"}},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "scanSetup", "channels","11","set"}, 
    {{"channelMode","ElectronEnergy"},{"dwell","1"},{"enable","True"}},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "scanSetup", "channels","12","set"}, 
    {{"channelMode","FocusPotential"},{"dwell","1"},{"enable","True"}},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "scanSetup", "channels","13","set"}, 
    {{"channelMode","FilamentCurrent"},{"dwell","1"},{"enable","True"}},{{"isRunning", false},{"step","Step2"}});

    addUrlConfig({"mmsp", "scanSetup","set"}, {{"startChannel","1"},{"stopChannel","13"}},{{"isRunning", false},{"step","Step2"}});
    
    addUrlConfig({"mmsp", "scanSetup","set"}, 
    {{"startChannel","1"},{"stopChannel","13"},{"scanCount","-1"},{"scanInterval","0"},{"scanStart","1"}},{{"isRunning", false},{"step","Step2"}});



http://httpbin.org/get
http://reqres.in/api/users/1

http://jsonplaceholder.typicode.com/posts/1

// QString url = "https://jsonplaceholder.typicode.com/todos/1";
// HttpRequestSync("http://jsonplaceholder.typicode.com/posts/1", retStr, err, 3000, ResponseFormat::JSON);
// HttpRequestSync("http://httpbin.org/get", retStr, err, 3000, ResponseFormat::JSON);
// HttpRequestSync("https://reqres.in/api/users?page=2", retStr, err, 3000, ResponseFormat::JSON);
// HttpRequestSync("https://jsonplaceholder.typicode.com/todos/1", retStr, err, 3000, ResponseFormat::JSON);
