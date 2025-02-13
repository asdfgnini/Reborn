#pragma once

#include <QWidget>
#include <QTimer>
#include <QDebug>

#include "../include/data.h"
#include "../include/ui.h"



struct PageInfo {
    int index;
    DeviceType deviceName;
    ModeType modeName;

    PageInfo(int index, DeviceType deviceName, ModeType modeName)
        : index(index), deviceName(deviceName), modeName(modeName) {}
};



class MainWindow : public ElaWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString DeviceToString(DeviceType deviceName);
    QString ModeToString(ModeType modeName);
    QString ModeToStringtoiconMenu(ModeType modeName);


private:
    void ui_init();
    void initWindow();
    void initEdgeLayout();

    void addPageInfo(int index, DeviceType deviceName, ModeType modeName);
    void printPageInfo();
    int findPageIndex(DeviceType deviceName, ModeType modeName);
    void setCurrentIndex(int index);

    void flushIconMenu(DeviceType deviceName, ModeType modeName);
private slots:
    void switchToPage(DeviceType deviceName, ModeType modeName);

    void onIconMenuActionTriggered();

    void onRouteBackButtonClicked();
private:
    QAction* action;                // 当前选中的action
    ElaMenuBar* menuBar;           // 菜单栏
    QList<PageInfo> pageInfoList;    // 页面信息列表
    int currentIndex;               // 当前页面索引
    DeviceType currentDevice;       // 当前设备
    ModeType currentMode;           // 当前模式
    T_Home* home;                   // 主页面

    // 设备页面
    RGADevice* rgaDevice;           // RGA设备页面    
    T_ScanMode* scanMode;           // 扫描模式页面
    T_BaseComponents* baseComponents; // 基础组件页面





};


