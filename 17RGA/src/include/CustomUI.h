#ifndef CUSTOMUI_H
#define CUSTOMUI_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <qpushbutton.h>
#include "RGAData.h"


class CustomUI : public QWidget
{
    Q_OBJECT

public:
    explicit CustomUI(QWidget *parent = nullptr);
    ~CustomUI();

private slots:

    void readFromBuffer();      // 从缓冲区读取数据
    void toggleReading();       //按钮触发启动和停止
private:
    void updateUIWithData(const Scan_Data& data);  // 更新 UI 显示数据

private:
    QPushButton* startStopButton;  //启停按钮
    QLabel *amuStartLabel;      // 显示 AMU Start 数据
    QLabel *amuStopLabel;       // 显示 AMU Stop 数据
    QLabel *ppamuLabel;         // 显示 Ppamu 数据
    QLabel *dwellLabel;         // 显示 Dwell 数据
    QTimer *m_readTimer;        // 定时器，用于周期性读取
    RGAData *m_rgaData;         // 后端数据处理对象
    bool m_isReading;           //是否停止刷新数据，缺省为false
};

#endif // CUSTOMUI_H
